#include "sf_spelldamage_registry.h"
#include "../core/sf_wrappers.h"
#include "../core/sf_modloader.h"
#include "../handlers/sf_spelldamage_handlers.h"

#include <iostream>
#include <map>
#include <cstdint>

static std::map<uint16_t, damage_handler_ptr> s_spelldamage_handler_map;

void registerSpellDamageHandler(uint16_t spell_line_id, damage_handler_ptr handler)
{
    auto check = s_spelldamage_handler_map.find(spell_line_id);
    if (check != s_spelldamage_handler_map.end())
    {
        char message[256];
        sprintf(message, "%s (v%s) has replaced an Damage Handler [%d] (Was this on purpose?)", g_current_mod->mod_id, g_current_mod->mod_version, spell_line_id);
        log_warning(message);
    }
    s_spelldamage_handler_map[spell_line_id] = handler;
}

damage_handler_ptr get_spell_damage(const uint16_t spell_line_id)
{
    auto it = s_spelldamage_handler_map.find(spell_line_id);
    if (it == s_spelldamage_handler_map.end())
    {
        log_error("Unknown Spell Line ID, No damage handler has been registered for this spell.");
        return NULL;
    }
    return it->second;
}

void __thiscall register_vanilla_spell_damage_handlers()
{
}