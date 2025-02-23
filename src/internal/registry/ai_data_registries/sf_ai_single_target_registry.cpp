#include "sf_ai_single_target_registry.h"

static std::map<uint16_t, ai_single_handler_ptr> s_ai_single_target_handler_map;

void registerAiSingleTargetHandler(uint16_t spell_line, ai_single_handler_ptr handler)
{
    auto check = s_ai_single_target_handler_map.find(spell_line);
    if (check != s_ai_single_target_handler_map.end())
    {
        char message[256];
        sprintf(message, "%s (v%s) has replaced a single target AI Handler [%d] (Was this on purpose?)", g_current_mod->mod_id, g_current_mod->mod_version, spell_line);
        log_warning(message);
    }

    s_ai_single_target_handler_map[spell_line] = handler;
}

ai_single_handler_ptr get_single_ai_handler(uint16_t spell_line)
{
    auto it = s_ai_single_target_handler_map.find(spell_line);
    if (it == s_ai_single_target_handler_map.end())
    {
        // Element doesn't exist, insert the default value
        char message[256];
        sprintf(message, "%s Unknown spell line [%d], for ai single target handler, Assigning a default handler", g_current_mod->mod_id, spell_line);
        log_warning(message);
        it = s_ai_single_target_handler_map.emplace(spell_line, &default_support_ai_handler).first;
    }
    return it->second;
}
