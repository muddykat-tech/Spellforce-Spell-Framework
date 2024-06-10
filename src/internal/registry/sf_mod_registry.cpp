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

std::list<SFSpell *> internal_spell_list;

SFSpell *__thiscall registerSpell(uint16_t spell_id)
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

void __thiscall linkSpellTags(SFSpell *spell, SpellTag tags, ...)
{
    va_list args;
    va_start(args, tags);

    // Always check if the tag is valid before using it
    SpellTag tag = static_cast<SpellTag>(va_arg(args, int));
    while (tag != SPELL_TAG_COUNT)
    {
        spell->spell_tags[tag] = true;
        tag = static_cast<SpellTag>(va_arg(args, int));
    }

    va_end(args);
}

void __thiscall linkTypeHandler(SFSpell *spell, handler_ptr typeHandler)
{
    spell->spell_type_handler = typeHandler;
}

void __thiscall linkEffectHandler(SFSpell *spell, uint16_t spell_effect_id, handler_ptr effectHandler)
{
    spell->spell_effect_id = spell_effect_id;
    spell->spell_effect_handler = effectHandler;
}

void __thiscall linkEndHandler(SFSpell *spell, handler_ptr endHandler)
{
    spell->spell_end_handler = endHandler;
}

/**
 * Registers the mod spells and performs basic conflict checking.
 *
 * This function iterates over the internal_spell_list and registers each spell by
 * adding it to the spell_id_map and spell_effect_id_map. It checks for conflicts
 * by checking if the spell_id or spell_effect_id is already registered by another
 * mod. If a conflict is detected, an error message is logged. If no conflict is
 * detected, the spell is added to the respective map.
 *
 * Additionally, it registers the spell type handler, spell effect handler, and spell
 * end handler if the SFSpell struct has a non-null handler pointer.
 *
 * After registering all the spells, the memory allocated for each spell is freed.
 */
void register_mod_spells()
{
    // Basic Conflict Checking
    std::map<uint16_t, SFMod *> spell_id_map;
    std::map<uint16_t, SFMod *> spell_effect_id_map;

    for (SFSpell *spell_data : internal_spell_list)
    {
        uint16_t spell_id = spell_data->spell_id;
        uint16_t spell_effect_id = spell_data->spell_effect_id;
        handler_ptr spell_type_handler = spell_data->spell_type_handler;
        handler_ptr spell_effect_handler = spell_data->spell_effect_handler;
        handler_ptr spell_end_handler = spell_data->spell_end_handler;

        SFMod *parent_mod = spell_data->parent_mod;
        char info[256];
        snprintf(info, sizeof(info), "| - Registration of spell from %s", parent_mod->mod_id);
        log_info(info);

        current_mod = spell_data->parent_mod;

        // Check for conflicts
        if (spell_id_map.find(spell_id) != spell_id_map.end())
        {
            char error_msg[256];
            SFMod *conflict_mod = spell_id_map[spell_id];
            snprintf(error_msg, sizeof(error_msg), "| - Mod Conflict Detected [%s]: Spell ID [%d] is already registered by [%s]", parent_mod->mod_id, spell_id, conflict_mod->mod_id);
            log_error(error_msg);
        }

        if (spell_effect_id_map.find(spell_effect_id) != spell_effect_id_map.end())
        {
            char error_msg[256];
            SFMod *conflict_mod = spell_effect_id_map[spell_effect_id];
            snprintf(error_msg, sizeof(error_msg), "| - Mod Conflict Detected [%s]: Spell Effect ID [%d] is already registered by [%s]", parent_mod->mod_id, spell_effect_id, conflict_mod->mod_id);
            log_error(error_msg);
        }

        // Update Conflict Maps
        spell_id_map[spell_id] = parent_mod;
        spell_effect_id_map[spell_effect_id] = parent_mod;

        // Do Registration
        if (spell_type_handler)
        {
            registerSpellTypeHandler(spell_id, spell_type_handler);
        }

        if (spell_effect_handler)
        {
            registerEffectHandler(spell_effect_id, spell_effect_handler);
        }

        if (spell_end_handler)
        {
            registerSpellEndHandler(spell_id, spell_end_handler);
        }
    }

    // Free Memory
    for (SFSpell *spell_data : internal_spell_list)
    {
        free(spell_data->parent_mod);
        free(spell_data);
    }
}
