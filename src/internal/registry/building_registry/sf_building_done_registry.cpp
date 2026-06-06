#include "sf_building_done_registry.h"
#include "../../handlers/sf_building_done_handlers.h"
#include <cstdint>
#include <iostream>
#include <map>
#include <list>
#include <utility>

static std::map<uint8_t, building_done_handler_ptr> s_bulding_done_map;

void registerBuildingDoneHandler(uint8_t building_type,
                                 building_done_handler_ptr handler)
{
    auto check = s_bulding_done_map.find(building_type);
    if (check != s_bulding_done_map.end())
    {
        char message[256];
        sprintf(message, "%s (v%s) has replaced a Building Done Handler [%d] (Was this on purpose?)",
                g_current_mod->mod_id, g_current_mod->mod_version, building_type);
        log_warning(message);
    }

    s_bulding_done_map[building_type] = handler;
}

building_done_handler_ptr get_building_done_handler(uint8_t building_type)
{
    auto it = s_bulding_done_map.find(building_type);
    if (it == s_bulding_done_map.end())
    {
        // Element doesn't exist, insert the default value
        log_debug (DEBUG_HIGH,"Unknown buidling type [%d] for done handler, Assigning a default handler.",
                   building_type);
        it = s_bulding_done_map.emplace(building_type, &default_done_handler).first;
    }
    return it->second;
}
