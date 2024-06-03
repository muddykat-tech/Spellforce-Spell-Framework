
#include "sf_spellend_registry.h"
#include "../core/sf_wrappers.h"
#include "../handlers/sf_spellend_handlers.h"
#include "../core/sf_modloader.h"

#include <iostream>
#include <map>
#include <cstdint>

std::map<uint16_t, handler_ptr> spellend_handler_map;

void registerSpellEndHandler(uint16_t spell_line, handler_ptr handler)
{
    auto check = spellend_handler_map.find(spell_line);
    if (check != spellend_handler_map.end())
    { 
        char message[256];
        sprintf(message, "%s (v%s) has replaced an End Handler [%d] (Was this on purpose?)", current_mod.mod_id, current_mod.mod_version, spell_line);
        log_warning(message);
    }

    spellend_handler_map[spell_line] = handler;
}

handler_ptr get_spell_end(uint16_t spell_line)
{
    auto it = spellend_handler_map.find(spell_line);
    if (it == spellend_handler_map.end()) 
    {
        log_error("Unknown Job ID, No spell end handler registered.");
        return NULL;
    }
    return it->second;
}

void register_vanilla_spell_end_handlers()
{
    //As we define ALL of the end handlers ourself inside sf_spellend_handlers.cpp, we do NOT need to initialize them here. (unlike sf_spelleffect_registry.cpp)

    // from 0x06 -> 0xda
    registerSpellEndHandler(0x06, &invulnerability_end_handler);
    registerSpellEndHandler(0xb, &illuminate_end_handler);
    registerSpellEndHandler(0x29, &remediless_end_handler);
    registerSpellEndHandler(0x55, &unkn_spell_end_handler);
    registerSpellEndHandler(0x80, &demoralization_end_handler);
    
    registerSpellEndHandler(0x09, &freeze_end_handler);
    registerSpellEndHandler(0x19, &petrify_end_handler);
    
    registerSpellEndHandler(0x0a, &fog_end_handler);

    registerSpellEndHandler(0xc, &fireshield_end_handler);
    registerSpellEndHandler(0x5d, &feign_death_end_handler);
    registerSpellEndHandler(0xda, &manashield_end_handler);
    
}