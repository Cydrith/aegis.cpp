//
// config.hpp
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


#define AEGIS_VERSION_LONG      0x00000100
#define AEGIS_VERSION_TEXT      "aegis.cpp 0.1.0 2017/11/07"

#define AEGIS_VERSION_MAJOR     ((AEGIS_VERSION_LONG & 0x00ff0000) >> 16)
#define AEGIS_VERSION_MINOR     ((AEGIS_VERSION_LONG & 0x0000ff00) >> 8)
#define AEGIS_VERSION_PATCH     (AEGIS_VERSION_LONG & 0x000000ff)

#define ASIO_STANDALONE
#define _WEBSOCKETPP_CPP11_STL_
#define BOOST_DATE_TIME_NO_LIB
#define BOOST_REGEX_NO_LIB



namespace aegiscpp
{





}


