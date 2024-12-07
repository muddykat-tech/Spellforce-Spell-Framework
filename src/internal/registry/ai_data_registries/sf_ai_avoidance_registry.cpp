#include "sf_ai_avoidance_registry.h"

static std::map<uint16_t, ai_avoidance_handler_ptr> s_ai_avoidance_handler_map;

void registerAiAvoidanceHandler(uint16_t spell_line, ai_avoidance_handler_ptr handler)
{
    auto check = s_ai_avoidance_handler_map.find(spell_line);
    if (check != s_ai_avoidance_handler_map.end())
    {
        char message[256];
        sprintf(message, "%s (v%s) has replaced a avoidance target AI Handler [%d] (Was this on purpose?)", g_current_mod->mod_id, g_current_mod->mod_version, spell_line);
        log_warning(message);
    }

    s_ai_avoidance_handler_map[spell_line] = handler;
}

ai_avoidance_handler_ptr get_ai_avoidance_handler(uint16_t spell_line)
{
    auto it = s_ai_avoidance_handler_map.find(spell_line);
    if (it == s_ai_avoidance_handler_map.end())
    {
        // Element doesn't exist, insert the default value
        log_warning("Unknown spell ID for avoidance ai handler, Assigning a default handler.");
        it = s_ai_avoidance_handler_map.emplace(spell_line, &sf_ai_avoidance_default_handler).first;
    }
    return it->second;
}
