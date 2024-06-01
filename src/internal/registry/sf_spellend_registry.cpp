
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
    register_spell_end_handler(0xb, &default_end_handler); // And so on, (not sure if 0xb is default but it may be based on ghidra? CGdSpell:EndSpell)
}