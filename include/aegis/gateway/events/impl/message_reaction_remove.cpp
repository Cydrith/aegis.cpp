//
// message_reaction_remove.cpp
// ***************************
//
// Copyright (c) 2019 Sharon W (sharon at aegis dot gg)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "aegis/config.hpp"
#include "aegis/fwd.hpp"
#include "aegis/gateway/events/message_reaction_remove.hpp"
#include <nlohmann/json.hpp>


namespace aegis
{

namespace gateway
{

namespace events
{

/// \cond TEMPLATES
AEGIS_DECL void from_json(const nlohmann::json& j, message_reaction_remove& m)
{
    m.user_id = j["user_id"];
    m.channel_id = j["channel_id"];
    m.message_id = j["message_id"];
    m.guild_id = j["guild_id"];
    m._emoji = j["emoji"];
}
/// \endcond

}

}

}
