//
// user_update.hpp
// ***************
//
// Copyright (c) 2018 Sharon W (sharon at aegis dot gg)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include "aegis/config.hpp"
#include "aegis/fwd.hpp"
#include "aegis/snowflake.hpp"
#include "aegis/gateway/objects/user.hpp"


namespace aegis
{

namespace gateway
{

namespace events
{

/**\todo Needs documentation
 */
struct user_update
{
    objects::user _user; /**<\todo Needs documentation */
#if !defined(AEGIS_DISABLE_ALL_CACHE)
    explicit user_update(aegis::member * m) : _member(m) {}
    shards::shard * _shard = nullptr; /**< Pointer to shard object this message came from */
    core * bot = nullptr; /**< Pointer to the main bot object */
    aegis::member * const _member = nullptr; /**<\todo Needs documentation */
#endif
};

/// \cond TEMPLATES
AEGIS_DECL void from_json(const nlohmann::json& j, user_update& m);
/// \endcond

}

}

}

#if defined(AEGIS_HEADER_ONLY)
#include "aegis/gateway/events/impl/user_update.cpp"
#endif
