//
// message_update.hpp
// ******************
//
// Copyright (c) 2018 Sharon W (sharon at aegis dot gg)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include "aegis/config.hpp"
#include "aegis/snowflake.hpp"
#include "aegis/objects/message.hpp"
#include "aegis/fwd.hpp"
#include <string>
#include <vector>

namespace aegis
{

namespace gateway
{

namespace events
{

/**\todo Needs documentation
 */
struct message_update
{
    objects::message msg; /**<\todo Needs documentation */
#if !defined(AEGIS_DISABLE_ALL_CACHE)
    message_update(const json & j, channel * c, member * m) : msg(j), _channel(c), _member(m) {};
    shards::shard * _shard = nullptr; /**< Pointer to shard object this message came from */
    core * bot = nullptr; /**< Pointer to the main bot object */
    channel * const _channel = nullptr; /**<\todo Needs documentation */
    member * const _member = nullptr; /**<\todo Needs documentation */
#else
    message_update(const json & j) : msg(j) {};
#endif
};

/**\todo Needs documentation
 */
inline void from_json(const nlohmann::json& j, message_update& m)
{
    m.msg = j;
}

}

}

}
