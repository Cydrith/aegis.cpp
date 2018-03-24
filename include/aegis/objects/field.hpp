//
// field.hpp
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
#include <nlohmann/json.hpp>
#include <string>



namespace aegiscpp
{



/**\todo Needs documentation
*/
struct field
{
    field(std::string n, std::string v) : name(n), value(v) {}
    field(std::string n, std::string v, bool il) : name(n), value(v), is_inline(il) {}
    field() = default;
    std::string name; /**<\todo Needs documentation */
    std::string value; /**<\todo Needs documentation */
    bool is_inline = false; /**<\todo Needs documentation */
};

/**\todo Needs documentation
*/
inline void from_json(const nlohmann::json& j, field& m)
{
    if (j.count("name"))
        m.name = j["name"];
    if (j.count("value"))
        m.value = j["value"];
    if (j.count("inline"))
        m.is_inline = j["inline"];
}

/**\todo Needs documentation
*/
inline void to_json(nlohmann::json& j, const field& m)
{
    j["name"] = m.name;
    j["value"] = m.value;
    j["inline"] = m.is_inline;
}

}

