//
// common.hpp
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


#include <string>
#include <string_view>
#include <stdint.h>

#include <cstdio>
#include <iostream>
#include <streambuf>
#include <sstream>
#include <functional>
#include <memory>
#include <optional>
#include <map>
#include <set>
#include <queue>
#include <chrono>
#include <mutex>
#include <atomic>

#include <json.hpp>
#include <zstr.hpp>

#include <asio.hpp>
#include <asio/ssl.hpp>
#include <spdlog/spdlog.h>
#include <websocketpp/common/random.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/connection_hdl.hpp>
#include <websocketpp/roles/client_endpoint.hpp>
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <asio/steady_timer.hpp>
