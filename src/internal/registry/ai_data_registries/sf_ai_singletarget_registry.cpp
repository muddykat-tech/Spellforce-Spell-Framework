#include "sf_ai_singletarget_registry.h"
#include "../../core/sf_wrappers.h"
#include "../../handlers/sf_ai_spell_handlers.h"
#include "../../core/sf_modloader.h"

#include <iostream>
#include <map>
#include <cstdint>
#include <list>
#include <utility>

//TODO remember to EXPOSE handler functions

static std::map<uint16_t, ai_single_hander_ptr> s_ai_handler_map;

void registerAiSingleTargetHandler(uint16_t spell_line, ai_single_hander_ptr handler)
{
    auto check = s_ai_handler_map.find(spell_line);
    if (check != s_ai_handler_map.end())
    {
        char message[256];
        sprintf(message, "%s (v%s) has replaced a single target AI Handler [%d] (Was this on purpose?)", g_current_mod->mod_id, g_current_mod->mod_version, spell_line);
        log_warning(message);
    }

    s_ai_handler_map[spell_line] = handler;
}

ai_single_hander_ptr get_single_ai_handler(uint16_t spell_line)
{
    auto it = s_ai_handler_map.find(spell_line);
    if (it == s_ai_handler_map.end())
    {
        // Element doesn't exist, insert the default value
        log_warning("Unknown spell line for ai single target handler, Assigning a default handler.");
        it = s_ai_handler_map.emplace(spell_line, &default_support_ai_handler).first;
    }
    return it->second;
}
