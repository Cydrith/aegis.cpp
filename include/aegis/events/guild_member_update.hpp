//
// guild_member_update.hpp
// ***********************
//
// Copyright (c) 2018 Sharon W (sharon at aegis dot gg)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include "aegis/config.hpp"
#include "aegis/snowflake.hpp"
#include "aegis/objects/user.hpp"
#include "aegis/fwd.hpp"
#include <string>
#include <vector>

namespace aegis
{

namespace gateway
{

namespace events
{

/// Sent when a guild member is updated
struct guild_member_update
{
    shards::shard * _shard = nullptr; /**< Pointer to shard object this message came from */
    core * bot = nullptr; /**< Pointer to the main bot object */
    objects::user _user; /**< User that was updated */
    snowflake guild_id; /**< Snowflake of guild */
    std::vector<snowflake> roles; /**< Array of roles the user has */
    std::string nick; /**< Nickname the user currently has (if any) */
};

/// \cond TEMPLATES
inline void from_json(const nlohmann::json& j, guild_member_update& m)
{
    m._user = j["user"];
    if (j.count("nick") && !j["nick"].is_null())
        m.nick = j["nick"];
    if (j.count("guild_id") && !j["guild_id"].is_null())
        m.guild_id = j["guild_id"];
    if (j.count("roles") && !j["roles"].is_null())
        for (const auto & i : j["roles"])
            m.roles.push_back(i);
}
/// \endcond

}

}

}
