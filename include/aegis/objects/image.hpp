//
// image.hpp
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
#include <string>
#include <vector>



namespace aegiscpp
{


struct image
{
    std::string url;
    std::string proxy_url;
    int32_t height = 0;
    int32_t width = 0;
};

inline void from_json(const nlohmann::json& j, image& m)
{
    if (j.count("url") && !j["url"].is_null())
        m.url = j["url"];
    if (j.count("proxy_url") && !j["proxy_url"].is_null())
        m.proxy_url = j["proxy_url"];
    if (j.count("height") && !j["height"].is_null())
        m.height = j["height"];
    if (j.count("width") && !j["width"].is_null())
        m.width = j["width"];
}
inline void to_json(nlohmann::json& j, const image& m)
{
    j["url"] = m.url;
    j["proxy_url"] = m.proxy_url;
    j["height"] = m.height;
    j["width"] = m.width;
}

}

