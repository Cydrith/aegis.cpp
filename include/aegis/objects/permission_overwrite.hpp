//
// permission_overwrite.hpp
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
#include <json.hpp>
#include <string>
#include <vector>



namespace aegiscpp
{

class member;
class channel;

struct permission_overwrite
{
    snowflake id;
    //either "role" or "member"
    overwrite_type type;
    int64_t allow;
    int64_t deny;
};
void from_json(const nlohmann::json& j, permission_overwrite& m)
{
    m.id = j["id"];
    m.type = (j["type"] == "type")?(overwrite_type::Role):(overwrite_type::User);
    m.allow = j["allow"];
    m.deny = j["deny"];
}
void to_json(nlohmann::json& j, const permission_overwrite& m)
{
    j["id"] = m.id;
    j["type"] = m.type;
    j["allow"] = m.allow;
    j["deny"] = m.deny;
}

}
