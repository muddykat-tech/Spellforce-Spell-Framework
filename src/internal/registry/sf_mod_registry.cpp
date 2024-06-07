#include "sf_mod_registry.h"

#include "sf_spelltype_registry.h"
#include "sf_spelleffect_registry.h"
#include "sf_spellend_registry.h"

#include <windows.h>
#include <iostream>
#include <map>
#include <list>
#include <cstdint>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>  

std::list<SFSpell*> internal_spell_list; 

SFSpell* __thiscall registerSpell(uint16_t spell_id)
{ 
    SFSpell *sf_spell = new SFSpell;
    sf_spell->spell_id = spell_id;
    sf_spell->spell_effect_id = 0x00;       
    sf_spell->spell_type_handler = nullptr;
    sf_spell->spell_effect_handler = nullptr;
    sf_spell->spell_end_handler = nullptr;
    sf_spell->parent_mod = current_mod;
    
    internal_spell_list.push_back(sf_spell);

    return sf_spell;
}

void __thiscall linkSpellTags(SFSpell* spell, SpellTag tags, ...)
{
    // auto it = internal_spell_map.find(spell_id);
    // SFSpell *spell = it->second;
    // va_list args;
    // va_start(args, tags);
    // SpellTag tag;

    // // Functions in using ... do not know how many have been parsed through, so we use this logic to go through all spell tags, check if the input is valid, if so set the tag true.
    // while ((tag = static_cast<SpellTag>(va_arg(args, int))) != SPELL_TAG_COUNT) {
    //     spell->spell_tags[tag] = true;
    // }
    // va_end(args);
}

void __thiscall linkTypeHandler(SFSpell* spell, handler_ptr typeHandler)
{
    spell->spell_type_handler = typeHandler;
}

void __thiscall linkEffectHandler(SFSpell* spell, uint16_t spell_effect_id, handler_ptr effectHandler)
{
    spell->spell_effect_id = spell_effect_id;
    spell->spell_effect_handler = effectHandler;
}

void __thiscall linkEndHandler(SFSpell *spell, handler_ptr endHandler)
{
    spell->spell_end_handler = endHandler;
}

void formatSFSpell(const SFSpell* spell, char* buffer, size_t bufferSize) {
    snprintf(buffer, bufferSize,
             "SFSpell: { spell_id = %d, spell_effect_id = %d, spell_tags = [%d, %d, ...], spell_type_handler = %p, spell_effect_handler = %p, spell_end_handler = %p }",
             spell->spell_id, spell->spell_effect_id, spell->spell_tags[0], spell->spell_tags[1],
             spell->spell_type_handler, spell->spell_effect_handler, spell->spell_end_handler);
}

void register_mod_spells()
{
    log_info("-====== Mod Registration Phase Start ======-");

    // Basic Conflict Checking
    std::map<uint16_t, SFMod*> spell_id_map;
    std::map<uint16_t, SFMod*> spell_effect_id_map;

    for (SFSpell *spell_data: internal_spell_list)
    {
        uint16_t spell_id = spell_data->spell_id;
        uint16_t spell_effect_id = spell_data->spell_effect_id;
        handler_ptr spell_type_handler = spell_data->spell_type_handler;
        handler_ptr spell_effect_handler = spell_data->spell_effect_handler;
        handler_ptr spell_end_handler = spell_data->spell_end_handler;

        SFMod *parent_mod = spell_data->parent_mod;
        char info[256];
        snprintf(info, sizeof(info), "Registration of spell from %s", parent_mod->mod_id);
        log_info(info);

        current_mod = spell_data->parent_mod;

        if (spell_id_map.find(spell_id) != spell_id_map.end())
        {
            char error_msg[256];
            SFMod* conflict_mod = spell_id_map[spell_id];
            snprintf(error_msg, sizeof(error_msg), "Mod Conflict Detected [%s]: Spell ID [%d] is already registered by %s", parent_mod->mod_id, spell_id, conflict_mod->mod_id);
            log_error(error_msg);
        }
        else
        {   
            spell_id_map[spell_id] = parent_mod;
        }

        if (spell_effect_id_map.find(spell_effect_id) != spell_effect_id_map.end())
        {
            char error_msg[256];
            SFMod *conflict_mod = spell_effect_id_map[spell_id];
            snprintf(error_msg, sizeof(error_msg), "Mod Conflict Detected [%s]: Spell Effect ID [%d] is already registered by [%s]", parent_mod, spell_effect_id, conflict_mod->mod_id);
            log_error(error_msg);
        }
        else
        {
            spell_effect_id_map[spell_effect_id] = parent_mod;
        }

        if(spell_type_handler)
        {
            registerSpellTypeHandler(spell_id, spell_type_handler);
        }  

        if(spell_effect_handler)
        {
            registerEffectHandler(spell_effect_id, spell_effect_handler);
        }
        
        if(spell_end_handler)
        {
            registerSpellEndHandler(spell_id, spell_end_handler);
        }
        
        // This frees the memory that we allocate when we run registerSpell
        delete spell_data;
    }
    
    log_info("-====== Mod Registration Phase End ======-");
}
