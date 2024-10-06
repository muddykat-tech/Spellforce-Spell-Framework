
#include "sf_onhit_registry.h"
#include "../core/sf_wrappers.h"
#include "../handlers/sf_onhit_handlers.h"
#include "../core/sf_modloader.h"

#include <iostream>
#include <map>
#include <cstdint>
#include <list>
#include <utility>

static std::map<OnHitPhase, std::list<std::pair<uint16_t, onhit_handler_ptr>>> s_onhit_handler_map;

void registerOnHitHandler(uint16_t spell_line_id, onhit_handler_ptr handler, OnHitPhase phase)
{
    // Check if there are existing handlers for the specified phase
    auto &handler_list = s_onhit_handler_map[phase];

    char buffer[256];
    sprintf(buffer, "Mod [%s] setting on hit handler for Spell line ID [%d]", g_current_mod->mod_id, spell_line_id);
    log_info(buffer);

    for (auto &entry : handler_list)
    {
        // Check if there's an existing handler for this spell_line_id
        if (entry.first == spell_line_id)
        {
            // Log a warning since we are replacing an existing handler
            char message[256];
            sprintf(message, "%s (v%s) has replaced an On Hit Handler [%d] (Was this on purpose?)", g_current_mod->mod_id, g_current_mod->mod_version, spell_line_id);
            log_warning(message);

            // Replace the existing handler
            entry.second = handler;
            return; // Early exit since we updated an existing handler
        }
    }

    // If we didn't find an existing handler, add a new handler to back of list. (we iterate in reverse)
    handler_list.emplace_back(spell_line_id, handler);
}

std::list<std::pair<uint16_t, onhit_handler_ptr>> get_onhit_phase(OnHitPhase phase)
{
    int enumValue = static_cast<int>(phase);
    auto it = s_onhit_handler_map.find(phase);
    if (it == s_onhit_handler_map.end())
    {
        // Element doesn't exist, log a warning and return an empty list
    //    log_warning("Unknown Phase ID for On Hit Handler.");
        return {}; // Return an empty list
    }
  /*  char info_msg[50];
    sprintf(info_msg, "On Hit Handler Phase: %d", enumValue);
    log_info(info_msg); */
    return it->second; // Return the list associated with the phase
}

void register_vanilla_onhit_handlers()
{
}