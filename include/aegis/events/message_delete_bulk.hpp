//
// message_delete_bulk.hpp
// ***********************
//
// Copyright (c) 2018 Sharon W (sharon at aegis dot gg)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include "aegis/config.hpp"
#include "base_event.hpp"

namespace aegis
{

namespace gateway
{

namespace events
{

/**\todo Needs documentation
 */
struct message_delete_bulk : public base_event
{
};

/**\todo Needs documentation
 */
inline void from_json(const nlohmann::json& j, message_delete_bulk& m)
{
}

}

}

}
