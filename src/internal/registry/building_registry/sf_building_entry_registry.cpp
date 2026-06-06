#include "sf_building_entry_registry.h"
#include "../../handlers/sf_worker_building_entry_handlers.h"
#include <cstdint>
#include <map>
#include <list>
#include <utility>

static std::map<uint8_t, building_entry_handler_ptr> s_bulding_entry_map;

void registerBuildingEntryHandler(uint8_t building_type,
                                  building_entry_handler_ptr handler)
{
    auto check = s_bulding_entry_map.find(building_type);
    if (check != s_bulding_entry_map.end())
    {
        log_warning("%s (v%s) has replaced a Building Entry Handler [%d] (Was this on purpose?)",
                    g_current_mod->mod_id, g_current_mod->mod_version, building_type);
    }

    s_bulding_entry_map[building_type] = handler;
}

building_entry_handler_ptr get_building_entry_handler(uint8_t building_type)
{
    auto it = s_bulding_entry_map.find(building_type);
    if (it == s_bulding_entry_map.end())
    {
        // Element doesn't exist, insert the default value
        log_warning("Unknown buidling type [%d] for done handler, Assigning a default handler.", building_type);
        it = s_bulding_entry_map.emplace(building_type, &default_entry_handler).first;
    }
    return it->second;
}
