#include "../core/sf_modloader.h"
#include "../core/sf_wrappers.h"
#include "../core/sf_hooks.h"

#include "sf_registry.h"
#include "sf_spelltype_registry.h"
#include "sf_spelleffect_registry.h"
#include "sf_spellend_registry.h"

#include <windows.h>
#include <iostream>
#include <map>
#include <cstdint>
#include <stdio.h>
#include <stdarg.h>

// Exposed in sfsf.h
SpellforceSpellFramework frameworkAPI;
SFMod current_mod;

// We can include pointers to our own functions we define.
// Note these functions are ASSIGNED to a function group when we DEFINE said group, names are VERY important.

std::map<uint16_t, SFSpell*> internal_spell_map; 

void __thiscall registerSpell(uint16_t spell_id, uint16_t spell_effect_id)
{
    SFSpell *sf_spell = new SFSpell;
    sf_spell->spell_id = spell_id;
    sf_spell->spell_effect_id = spell_effect_id;
    sf_spell->spell_type_handler = nullptr;
    sf_spell->spell_effect_handler = nullptr;
    sf_spell->spell_end_handler = nullptr;
    
    internal_spell_map[spell_id] = sf_spell;
}

void __thiscall linkSpellTags(uint16_t spell_id, SpellTag tags, ...)
{
    auto it = internal_spell_map.find(spell_id);
    SFSpell *spell = it->second;
    va_list args;
    va_start(args, tags);
    SpellTag tag;

    // Functions in using ... do not know how many have been parsed through, so we use this logic to go through all spell tags, check if the input is valid, if so set the tag true.
    while ((tag = static_cast<SpellTag>(va_arg(args, int))) != SPELL_TAG_COUNT) {
        spell->spell_tags[tag] = true;
    }
    va_end(args);
}

void __thiscall linkTypeHandler(uint16_t spell_id, handler_ptr typeHandler)
{
    log_info("Link Type Handler");
    auto it = internal_spell_map.find(spell_id);
    SFSpell *spell = it->second;
    spell->spell_type_handler = typeHandler;
}

void __thiscall linkEffectHandler(uint16_t spell_id, handler_ptr effectHandler)
{
    log_info("Link Effect Handler");
    auto it = internal_spell_map.find(spell_id);
    SFSpell *spell = it->second;
    spell->spell_effect_handler = effectHandler;
}

void __thiscall linkEndHandler(uint16_t spell_id, handler_ptr endHandler)
{
    log_info("Link End Handler");
    auto it = internal_spell_map.find(spell_id);
    SFSpell *spell = it->second;
    spell->spell_end_handler = endHandler;
}

// exposed in sf_registry.h
void registerFrameworkAPI()
{
    frameworkAPI.figureAPI = &figureAPI;
    frameworkAPI.spellAPI = &spellAPI;
    frameworkAPI.toolboxAPI = &toolboxAPI;
    frameworkAPI.iteratorAPI = &iteratorAPI;
    frameworkAPI.registrationAPI = &registrationAPI;

	frameworkAPI.registerSpellTypeHandler = &registerSpellTypeHandler;
    frameworkAPI.registerEffectHandler = &registerEffectHandler;
    frameworkAPI.registerSpellEndHandler = &registerSpellEndHandler;
    frameworkAPI.createModInfo = &createModInfo;
    frameworkAPI.logAPI = setup_logger();

    current_mod = createModInfo("SFSF", "The Spellforce Spell Framework", "4.0.0", "Muddykat, UnSchtalch, shovel_knight");
}

void formatSFSpell(const SFSpell* spell, char* buffer, size_t bufferSize) {
    snprintf(buffer, bufferSize,
             "SFSpell: { spell_id = %d, spell_effect_id = %d, spell_tags = [%d, %d, ...], spell_type_handler = %p, spell_effect_handler = %p, spell_end_handler = %p }",
             spell->spell_id, spell->spell_effect_id, spell->spell_tags[0], spell->spell_tags[1],
             spell->spell_type_handler, spell->spell_effect_handler, spell->spell_end_handler);
}

void register_mod_spells()
{
    log_info("Mass Registration of Mod Spells");
    for (const auto& entry : internal_spell_map)
    {
        uint16_t spell_id = entry.first;
        SFSpell *spell_data = entry.second;

        const size_t buffer_size = 256;
        char buffer[buffer_size];

        formatSFSpell(spell_data, buffer, buffer_size);
        
        log_info(buffer);

        uint16_t spell_effect_id = spell_data->spell_effect_id;
        handler_ptr spell_type_handler = spell_data->spell_type_handler;
        handler_ptr spell_effect_handler = spell_data->spell_effect_handler;
        handler_ptr spell_end_handler = spell_data->spell_end_handler;
            
        log_info("Validation of Spell Registration");
        // Validation
        if(!spell_effect_id)
        {
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg), "No Spell Effect ID Found for Spell [%d]", spell_id);
            log_error(error_msg);
        }   
        
        if(!spell_id)
        {
            log_error("Somehow you registered a spell with no ID, Congratulations, that's honestly impressive. ~Muddykat");
        }

        if(spell_type_handler)
        {
            log_info("Registration of Spell Type Handler");
            registerSpellTypeHandler(spell_id, spell_type_handler);
        }  

        if(spell_effect_handler)
        {
            log_info("Registration of Spell Effect Handler");
            registerEffectHandler(spell_effect_id, spell_effect_handler);
        }   
        
        if(spell_end_handler)
        {
            log_info("Registration of Spell End Handler");
            registerSpellEndHandler(spell_id, spell_end_handler);
        }
        
        // This frees the memory that we allocate when we run registerSpell
        delete entry.second;
    }
}

void initialize_framework() 
{
    log_info("Initializing Data Hooks");

	initialize_data_hooks();

    log_info("Linking API functions");

    // setup framework api structure references
    registerFrameworkAPI();
    
    log_info("Registration of Vanilla Spelltype Handlers");

    // Setup Vanilla Spells -> see sf_spelltype_handler.h
    register_vanilla_spelltype_handlers();

    log_info("Registration of Vanilla Effect Handlers");

    register_vanilla_effect_handlers();

    log_info("Registration of Vanilla Spell End Handlers");

    register_vanilla_spell_end_handlers();
    
    log_info("Initializing Mods");

    // Attempt to load all mods -> see sf_modloader.h
    initialize_mods();

    // Now try and register these spells
    register_mod_spells();
}
