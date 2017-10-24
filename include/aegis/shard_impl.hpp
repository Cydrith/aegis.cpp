//
// shard_impl.hpp
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


#include "config.hpp"
#include "common.hpp"
#include "aegis.hpp"
#include "shard.hpp"
#include "utility.hpp"


#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/common/connection_hdl.hpp>
#include <websocketpp/client.hpp>

namespace aegiscpp
{


inline void shard::do_reset()
{
    heartbeat_ack = 0;
    if (connection != nullptr)
        connection.reset();
    if (keepalivetimer != nullptr)
    {
        keepalivetimer->cancel();
        keepalivetimer.reset();
    }
}

inline bool shard::conn_test(std::function<void()> func)
{
    if (connection == nullptr)
    {
        connection_state = Reconnecting;
        do_reset();
        reconnect_timer = state->core->websocket_o.set_timer(6000, [this](const asio::error_code & ec)
        {
            if (ec == asio::error::operation_aborted)
                return;
            connection_state = Connecting;
            asio::error_code wsec;
            connection = state->core->websocket_o.get_connection(state->core->gateway_url, wsec);
            state->core->setup_callbacks(this);
            state->core->websocket_o.connect(connection);
        });
        return false;
    }
    func();
    return true;
}


}
