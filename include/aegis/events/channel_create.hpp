//
// channel_create.hpp
// ******************
//
// Copyright (c) 2018 Sharon W (sharon at aegis dot gg)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include "aegis/config.hpp"
#include "aegis/snowflake.hpp"
#include "aegis/objects/channel.hpp"
#include <string>
#include <vector>

namespace aegis
{

namespace gateway
{

namespace events
{

/// Channel object sent over the gateway on create
struct channel_create
{
    shards::shard * _shard = nullptr; /**< Pointer to shard object this message came from */
    core * bot = nullptr; /**< Pointer to the main bot object */
    objects::channel _channel; /**< gateway channel object */
};

/// \cond TEMPLATES
inline void from_json(const nlohmann::json& j, channel_create& m)
{
    m._channel = j;
}
/// \endcond

}

}

}
