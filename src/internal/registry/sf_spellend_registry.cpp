
#include "sf_spellend_registry.h"
#include "../core/sf_wrappers.h"
#include "../handlers/sf_spellend_handlers.h"

#include <iostream>
#include <map>
#include <cstdint>

std::map<uint16_t, handler_ptr> spellend_handler_map;

void register_spell_end_handler(uint16_t spell_line, handler_ptr handler)
{
    auto check = spellend_handler_map.find(spell_line);
    if (check != spellend_handler_map.end())
    { 
        char message[256];
        sprintf(message, "An On Spell End Handler has been replaced! [%d] (Was this on purpose?)", spell_line);
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
    register_spell_end_handler(0xe, &default_end_handler); // And so on, (not sure if 0xb is default but it may be based on ghidra? CGdSpell:EndSpell)
    
    // from 0x06 -> 0xda
    register_spell_end_handler(0x06, &invulnerability_end_handler);
    register_spell_end_handler(0xb, &illuminate_end_handler);
    register_spell_end_handler(0x29, &remediless_end_handler);
    register_spell_end_handler(0x55, &unkn_spell_end_handler);
    register_spell_end_handler(0x80, &demoralization_end_handler);
    
    register_spell_end_handler(0x09, &freeze_end_handler);
    register_spell_end_handler(0x19, &petrify_end_handler);
    
    register_spell_end_handler(0x0a, &fog_end_handler);

    register_spell_end_handler(0xc, &fireshield_end_handler);
    register_spell_end_handler(0x5d, &feign_death_end_handler);
    register_spell_end_handler(0xda, &manashield_end_handler);
    
}