//
// channel.cpp
// ***********
//
// Copyright (c) 2018 Sharon W (sharon at aegis dot gg)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "aegis/channel.hpp"
#include <spdlog/spdlog.h>
#include <asio.hpp>
#include "aegis/core.hpp"
#include "aegis/error.hpp"
#include "aegis/guild.hpp"
#include "aegis/shards/shard.hpp"
#include "aegis/member.hpp"
#include "aegis/rest/rest_reply.hpp"
#include "aegis/ratelimit/ratelimit.hpp"

namespace aegis
{

using json = nlohmann::json;

AEGIS_DECL channel::channel(const snowflake channel_id, const snowflake guild_id, core * _bot, asio::io_context & _io)
    : channel_id(channel_id)
    , guild_id(guild_id)
    , channel_bucket(_bot->get_ratelimit().get_bucket(ratelimit::Channel, channel_id))
    , emoji_bucket(_bot->get_ratelimit().get_bucket(ratelimit::Emoji, guild_id))
    , _guild(nullptr)
    , _io_context(_io)
    , _bot(_bot)
{
    emoji_bucket.ignore_rates = true;
}

AEGIS_DECL std::future<rest::rest_reply> channel::post_task(const std::string & path, const std::string & method, const std::string & obj, const std::string & host)
{
    return channel_bucket.post_task(path, method, obj, host);
}

AEGIS_DECL std::future<rest::rest_reply> channel::post_emoji_task(const std::string & path, const std::string & method, const std::string & obj, const std::string & host)
{
    return emoji_bucket.post_task(path, method, obj, host);
}

AEGIS_DECL guild & channel::get_guild() const
{
    if (_guild == nullptr)
        throw exception("Guild not set", make_error_code(error::guild_not_found));
    return *_guild;
}

AEGIS_DECL guild & channel::get_guild(std::error_code & ec) const AEGIS_NOEXCEPT
{
    if (_guild == nullptr)
        ec = make_error_code(error::guild_not_found);
    else
        ec = error_code();
    return *_guild;
}

#if !defined(AEGIS_DISABLE_ALL_CACHE)
AEGIS_DECL permission channel::perms()
{
    return permission(_guild->get_permissions(_guild->self(), this));
}

AEGIS_DECL void channel::load_with_guild(guild & _guild, const json & obj, shards::shard * _shard)
{
    channel_id = obj["id"];
    guild_id = _guild.get_id();
    try
    {
        //log->debug("Shard#{} : Channel[{}] created for guild[{}]", shard.m_shardid, channel_id, _channel.m_guild_id);
        if (!obj["name"].is_null()) name = obj["name"].get<std::string>();
        position = obj["position"];
        type = static_cast<gateway::objects::channel_type>(obj["type"].get<int>());// 0 = text, 2 = voice

        //voice channels
        if (obj.count("bitrate") && !obj["bitrate"].is_null())
        {
            bitrate = obj["bitrate"];
            user_limit = obj["user_limit"];
        }
        else
        {
            //not a voice channel, so has a topic field and last message id
            if (obj.count("topic") && !obj["topic"].is_null()) topic = obj["topic"].get<std::string>();
            if (obj.count("last_message_id") && !obj["last_message_id"].is_null()) last_message_id = obj["last_message_id"];
        }

        if (obj.count("permission_overwrites") && !obj["permission_overwrites"].is_null())
        {
            json permission_overwrites = obj["permission_overwrites"];
            for (auto & permission : permission_overwrites)
            {
                uint32_t allow = permission["allow"];
                uint32_t deny = permission["deny"];
                snowflake p_id = permission["id"];
                std::string p_type = permission["type"];

                overrides[p_id].allow = allow;
                overrides[p_id].deny = deny;
                overrides[p_id].id = p_id;
                if (p_type == "role")
                    overrides[p_id].type = gateway::objects::overwrite_type::Role;
                else
                    overrides[p_id].type = gateway::objects::overwrite_type::User;
            }
        }

        //_channel.update_permission_cache();
    }
    catch (std::exception&e)
    {
        throw exception(fmt::format("Shard#{} : Error processing channel[{}] of guild[{}] {}", _shard->get_id(), channel_id, guild_id, e.what()), make_error_code(error::channel_error));
    }
}
#else
AEGIS_DECL void channel::load_with_guild(guild & _guild, const json & obj, shard * _shard)
{
    channel_id = obj["id"];
    guild_id = _guild.get_id();
}
#endif

AEGIS_DECL std::future<rest::rest_reply> channel::create_message(std::error_code & ec, const std::string & content, int64_t nonce)
{
#if !defined(AEGIS_DISABLE_ALL_CACHE)
    if (_guild != nullptr)//probably a DM
        if (!perms().can_send_messages())
        {
            ec = make_error_code(error::no_permission);
            return {};
        }
#endif

    json obj;
    obj["content"] = content;

    if (nonce)
        obj["nonce"] = nonce;

    ec = error_code();
    return post_task(fmt::format("/channels/{}/messages", channel_id), "POST", obj.dump());
}

AEGIS_DECL std::future<rest::rest_reply> channel::create_message_embed(std::error_code & ec, const std::string & content, const json & embed, int64_t nonce)
{
#if !defined(AEGIS_DISABLE_ALL_CACHE)
    if (_guild != nullptr)//probably a DM
        if (!perms().can_send_messages())
        {
            ec = make_error_code(error::no_permission);
            return {};
        }
#endif

    json obj;
    if (!content.empty())
        obj["content"] = content;
    obj["embed"] = embed;

    if (nonce)
        obj["nonce"] = nonce;

    ec = error_code();
    return post_task(fmt::format("/channels/{}/messages", channel_id), "POST", obj.dump());
}

AEGIS_DECL std::future<rest::rest_reply> channel::edit_message(std::error_code & ec, snowflake message_id, const std::string & content)
{
    json obj;
    obj["content"] = content;

    ec = error_code();
    return post_task(fmt::format("/channels/{}/messages/{}", channel_id, message_id), "PATCH", obj.dump());
}

AEGIS_DECL std::future<rest::rest_reply> channel::edit_message_embed(std::error_code & ec, snowflake message_id, const std::string & content, const json & embed)
{
    json obj;
    if (!content.empty())
        obj["content"] = content;
    obj["embed"] = embed;
    obj["content"] = content;

    ec = error_code();
    return post_task(fmt::format("/channels/{}/messages/{}", channel_id, message_id), "PATCH", obj.dump());
}

/**\todo can delete your own messages freely - provide separate function or keep history of messages
 */
AEGIS_DECL std::future<rest::rest_reply> channel::delete_message(std::error_code & ec, snowflake message_id)
{
#if !defined(AEGIS_DISABLE_ALL_CACHE)
    if (!perms().can_manage_messages())
    {
        ec = make_error_code(error::no_permission);
        return {};
    }
#endif

    ec = error_code();
    return post_task(fmt::format("/channels/{}/messages/{}", guild_id, message_id), "DELETE");
}

AEGIS_DECL std::future<rest::rest_reply> channel::bulk_delete_message(std::error_code & ec, const std::vector<int64_t> & messages)
{
#if !defined(AEGIS_DISABLE_ALL_CACHE)
    if ((!perms().can_manage_messages()) || (messages.size() < 2 || messages.size() > 100))
    {
        ec = make_error_code(error::no_permission);
        return {};
    }
#endif

    json obj = messages;
    ec = error_code();
    return post_task(fmt::format("/channels/{}/messages/bulk-delete", channel_id), "POST", obj.dump());
}

AEGIS_DECL std::future<rest::rest_reply> channel::modify_channel(std::error_code & ec, std::optional<std::string> _name, std::optional<int> _position, std::optional<std::string> _topic,
                                    std::optional<bool> _nsfw, std::optional<int> _bitrate, std::optional<int> _user_limit,
                                    std::optional<std::vector<gateway::objects::permission_overwrite>> _permission_overwrites, std::optional<snowflake> _parent_id)
{
#if !defined(AEGIS_DISABLE_ALL_CACHE)
    if (!perms().can_manage_channels())
    {
        ec = make_error_code(error::no_permission);
        return {};
    }
#endif

    json obj;
    if (_name.has_value())
        obj["name"] = _name.value();
    if (_position.has_value())
        obj["position"] = _position.value();
    if (_topic.has_value())
        obj["topic"] = _topic.value();
    if (_nsfw.has_value())
        obj["nsfw"] = _nsfw.value();
    if (_bitrate.has_value())//voice only
        obj["bitrate"] = _bitrate.value();
    if (_user_limit.has_value())//voice only
        obj["user_limit"] = _user_limit.value();
    if (_permission_overwrites.has_value())//requires OWNER
    {
// #if !defined(AEGIS_DISABLE_ALL_CACHE)
//         if (_guild->owner_id != _guild->self()->member_id)
//         {
//             ec = make_error_code(value::no_permission);
//             return {};
//         }
// #endif

        obj["permission_overwrites"] = json::array();
        for (auto & p_ow : _permission_overwrites.value())
        {
            obj["permission_overwrites"].push_back(p_ow);
        }
    }
    if (_parent_id.has_value())//VIP only
        obj["parent_id"] = _parent_id.value();

    ec = error_code();
    return post_task(fmt::format("/channels/{}", channel_id), "PATCH", obj.dump());
}

AEGIS_DECL std::future<rest::rest_reply> channel::delete_channel(std::error_code & ec)
{
#if !defined(AEGIS_DISABLE_ALL_CACHE)
    if (!perms().can_manage_channels())
    {
        ec = make_error_code(error::no_permission);
        return {};
    }
#endif

    ec = error_code();
    return post_task(fmt::format("/channels/{}", channel_id), "DELETE");
}

AEGIS_DECL std::future<rest::rest_reply> channel::create_reaction(std::error_code & ec, snowflake message_id, const std::string & emoji_text)
{
#if !defined(AEGIS_DISABLE_ALL_CACHE)
    if (!perms().can_add_reactions())
    {
        ec = make_error_code(error::no_permission);
        return {};
    }
#endif

    ec = error_code();
    return post_task(fmt::format("/channels/{}/messages/{}/reactions/{}/@me", channel_id, message_id, emoji_text), "PUT");
}

AEGIS_DECL std::future<rest::rest_reply> channel::delete_own_reaction(std::error_code & ec, snowflake message_id, const std::string & emoji_text)
{
#if !defined(AEGIS_DISABLE_ALL_CACHE)
    if (!perms().can_add_reactions())
    {
        ec = make_error_code(error::no_permission);
        return {};
    }
#endif

    ec = error_code();
    return post_task(fmt::format("/channels/{}/messages/{}/reactions/{}/@me", channel_id, message_id, emoji_text), "DELETE");
}

AEGIS_DECL std::future<rest::rest_reply> channel::delete_user_reaction(std::error_code & ec, snowflake message_id, const std::string & emoji_text, snowflake member_id)
{
#if !defined(AEGIS_DISABLE_ALL_CACHE)
    if (!perms().can_manage_messages())
    {
        ec = make_error_code(error::no_permission);
        return {};
    }
#endif

    ec = error_code();
    return post_task(fmt::format("/channels/{}/messages/{}/reactions/{}/{}", channel_id, message_id, emoji_text, member_id), "DELETE");
}

/**\todo Support query parameters
 *  \todo before[snowflake], after[snowflake], limit[int]
 */
AEGIS_DECL std::future<rest::rest_reply> channel::get_reactions(std::error_code & ec, snowflake message_id, const std::string & emoji_text)
{
    ec = error_code();
    return post_task(fmt::format("/channels/{}/messages/{}/reactions/{}", channel_id, message_id, emoji_text), "GET");
}

AEGIS_DECL std::future<rest::rest_reply> channel::delete_all_reactions(std::error_code & ec, snowflake message_id)
{
#if !defined(AEGIS_DISABLE_ALL_CACHE)
    if (!perms().can_manage_messages())
    {
        ec = make_error_code(error::no_permission);
        return {};
    }
#endif

    ec = error_code();
    return post_task(fmt::format("/channels/{}/messages/{}/reactions", channel_id, message_id), "DELETE");
}

AEGIS_DECL std::future<rest::rest_reply> channel::edit_channel_permissions(std::error_code & ec, snowflake _overwrite_id, int64_t _allow, int64_t _deny, const std::string & _type)
{
#if !defined(AEGIS_DISABLE_ALL_CACHE)
    if (!perms().can_manage_roles())
    {
        ec = make_error_code(error::no_permission);
        return {};
    }
#endif

    json obj;
    obj["allow"] = _allow;
    obj["deny"] = _deny;
    obj["type"] = _type;
 
    ec = error_code();
    return post_task(fmt::format("/channels/{}/permissions/{}", channel_id, _overwrite_id), "PUT", obj.dump());
}

AEGIS_DECL std::future<rest::rest_reply> channel::get_channel_invites(std::error_code & ec)
{
#if !defined(AEGIS_DISABLE_ALL_CACHE)
    if (!perms().can_manage_channels())
    {
        ec = make_error_code(error::no_permission);
        return {};
    }
#endif

    ec = error_code();
    return post_task(fmt::format("/channels/{}/invites", channel_id), "GET");
}

AEGIS_DECL std::future<rest::rest_reply> channel::create_channel_invite(std::error_code & ec, std::optional<int> max_age, std::optional<int> max_uses, std::optional<bool> temporary, std::optional<bool> unique)
{
#if !defined(AEGIS_DISABLE_ALL_CACHE)
    if (!perms().can_invite())
    {
        ec = make_error_code(error::no_permission);
        return {};
    }
#endif

    json obj;
    if (max_age.has_value())
        obj["max_age"] = max_age.value();
    if (max_uses.has_value())
        obj["max_uses"] = max_uses.value();
    if (temporary.has_value())
        obj["temporary"] = temporary.value();
    if (unique.has_value())
        obj["unique"] = unique.value();

    ec = error_code();
    return post_task(fmt::format("/channels/{}/invites", channel_id), "POST", obj.dump());
}

AEGIS_DECL std::future<rest::rest_reply> channel::delete_channel_permission(std::error_code & ec, snowflake overwrite_id)
{
#if !defined(AEGIS_DISABLE_ALL_CACHE)
    if (!perms().can_manage_roles())
    {
        ec = make_error_code(error::no_permission);
        return {};
    }
#endif

    ec = error_code();
    return post_task(fmt::format("/channels/{}/permissions/{}", channel_id, overwrite_id), "DELETE");
}

AEGIS_DECL std::future<rest::rest_reply> channel::trigger_typing_indicator(std::error_code & ec)
{
    ec = error_code();
    return post_task(fmt::format("/channels/{}/typing", channel_id));
}

AEGIS_DECL std::future<rest::rest_reply> channel::get_pinned_messages(std::error_code & ec)
{
    ec = make_error_code(error::not_implemented);
    return {};
}

AEGIS_DECL std::future<rest::rest_reply> channel::add_pinned_channel_message(std::error_code & ec)
{
#if !defined(AEGIS_DISABLE_ALL_CACHE)
    if (!perms().can_manage_messages())
    {
        ec = make_error_code(error::no_permission);
        return {};
    }
#endif

    ec = make_error_code(error::not_implemented);
    return {};
}

AEGIS_DECL std::future<rest::rest_reply> channel::delete_pinned_channel_message(std::error_code & ec)
{
#if !defined(AEGIS_DISABLE_ALL_CACHE)
    if (!perms().can_manage_messages())
    {
        ec = make_error_code(error::no_permission);
        return {};
    }
#endif

    ec = make_error_code(error::not_implemented);
    return {};
}

/**\todo Will likely move to aegis class
 * requires OAuth permissions to perform
 */
AEGIS_DECL std::future<rest::rest_reply> channel::group_dm_add_recipient(std::error_code & ec)//will go in aegis::aegis
{
    ec = make_error_code(error::not_implemented);
    return {};
}

/**\todo Will likely move to aegis class
 * requires OAuth permissions to perform
 */
AEGIS_DECL std::future<rest::rest_reply> channel::group_dm_remove_recipient(std::error_code & ec)//will go in aegis::aegis
{
    ec = make_error_code(error::not_implemented);
    return {};
}

}

