//
// guild_member.hpp
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
#include "../objects/user.hpp"
#include <string>
#include <vector>
#include <json.hpp>



namespace aegiscpp
{

struct guild_member
{
    user _user;
    snowflake guild_id;
    std::string nick;
    std::vector<snowflake> roles;
    std::string joined_at;
    bool deaf = false;
    bool mute = false;
    shard * _shard;
    aegis * bot;
};

inline void from_json(const nlohmann::json& j, guild_member& m)
{
    m._user = j["user"];
    if (j.count("nick") && !j["nick"].is_null())
        m.nick = j["nick"];
    if (j.count("guild_id") && !j["guild_id"].is_null())
        m.guild_id = j["guild_id"];
    if (j.count("roles") && !j["roles"].is_null())
        for (auto i : j["roles"])
            m.roles.push_back(i);
    if (j.count("joined_at"))
        m.joined_at = j["joined_at"];
    if (j.count("deaf"))
        m.deaf = j["deaf"];
    if (j.count("mute"))
        m.mute = j["mute"];
}

}

