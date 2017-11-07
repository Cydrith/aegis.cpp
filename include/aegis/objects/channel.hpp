//
// channel.hpp
// aegis.cpp
//
// Copyright (c) 2017 Sara W (sara at xandium dot net)
//
// This file is part of aegis.cpp .
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once


#include "../config.hpp"
#include "../snowflake.hpp"
#include "../structs.hpp"
#include "../objects/user.hpp"
#include "permission_overwrite.hpp"
#include "user.hpp"
#include <json.hpp>
#include <string>
#include <vector>



namespace aegiscpp
{

struct channel_gw
{
    snowflake channel_id;
    channel_type type;
    snowflake guild_id = 0;//?
    int position = 0;//?
    std::vector<permission_overwrite> permission_overwrites;//?
    std::string name;//?
    std::string topic;//?
    bool nsfw = false;//?
    snowflake last_message_id = 0;//?
    int bitrate = 0;//?
    int userlimit = 0;//?
    std::vector<user> recipients;//?
    std::string icon;//?
    snowflake owner_id;//?
    snowflake application_id;//?
    snowflake parent_id;//?
};

inline void from_json(const nlohmann::json& j, channel_gw& m)
{
    m.channel_id = j["id"];
    m.type = j["type"];
    if (j.count("guild_id") && !j["guild_id"].is_null())
        m.guild_id = j["guild_id"];
    if (j.count("position") && !j["position"].is_null())
        m.position = j["position"];
    if (j.count("permission_overwrites") && !j["permission_overwrites"].is_null())
        for (auto i : j["permission_overwrites"])
            m.permission_overwrites.push_back(i);
    if (j.count("name") && !j["name"].is_null())
        m.name = j["name"];
    if (j.count("topic") && !j["topic"].is_null())
        m.topic = j["topic"];
    if (j.count("nsfw") && !j["nsfw"].is_null())
        m.nsfw = j["nsfw"];
    if (j.count("last_message_id") && !j["last_message_id"].is_null())
        m.last_message_id = j["last_message_id"];
    if (j.count("bitrate") && !j["bitrate"].is_null())
        m.bitrate = j["bitrate"];
    if (j.count("userlimit") && !j["userlimit"].is_null())
        m.userlimit = j["userlimit"];
    if (j.count("icon") && !j["icon"].is_null())
        m.icon = j["icon"];
    if (j.count("owner_id") && !j["owner_id"].is_null())
        m.owner_id = j["owner_id"];
    if (j.count("application_id") && !j["application_id"].is_null())
        m.application_id = j["application_id"];
    if (j.count("parent_id") && !j["parent_id"].is_null())
        m.parent_id = j["parent_id"];
    if (j.count("recipients") && !j["recipients"].is_null())
        for (auto i : j["recipients"])
            m.recipients.push_back(i);
}
inline void to_json(nlohmann::json& j, const channel_gw& m)
{
    j["id"] = m.channel_id;
    j["type"] = m.type;
    j["guild_id"] = m.guild_id;
    j["position"] = m.position;
    j["name"] = m.name;
    j["topic"] = m.topic;
    j["nsfw"] = m.nsfw;
    j["last_message_id"] = m.last_message_id;
    j["bitrate"] = m.bitrate;
    j["userlimit"] = m.userlimit;
    j["icon"] = m.icon;
    j["owner_id"] = m.owner_id;
    j["application_id"] = m.application_id;
    j["parent_id"] = m.parent_id;
    if (m.permission_overwrites.size() > 0)
        for (auto i : m.permission_overwrites)
            j["permission_overwrites"].push_back(i);
    if (m.recipients.size() > 0)
        for (auto i : m.recipients)
            j["recipients"].push_back(i);
}

}

