#include "sf_mod_registry.h"

#include "sf_spelltype_registry.h"
#include "sf_spelleffect_registry.h"
#include "sf_spellend_registry.h"
#include "sf_subeffect_registry.h"
#include "sf_spellrefresh_registry.h"
#include "sf_spelldamage_registry.h"

#include <windows.h>
#include <iostream>
#include <map>
#include <list>
#include <cstdint>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

std::list<SFSpell *> g_internal_spell_list;

SFSpell *__thiscall registerSpell(uint16_t spell_id)
{
    SFSpell *sf_spell = new SFSpell;
    sf_spell->spell_id = spell_id;
    sf_spell->spell_effect_id = 0x00;
    sf_spell->spell_type_handler = nullptr;
    sf_spell->spell_effect_handler = nullptr;
    sf_spell->spell_end_handler = nullptr;
    sf_spell->spell_refresh_handler = nullptr;
    sf_spell->sub_effect_handler = nullptr;
    sf_spell->parent_mod = g_current_mod;
    sf_spell->deal_damage_handler = nullptr;
    sf_spell->damage_phase = SpellDamagePhase::DEFAULT;
    g_internal_spell_list.push_back(sf_spell);

    return sf_spell;
}

void __thiscall applySpellTag(SFSpell *spell, SpellTag tag)
{
    spell->spell_tag = tag;
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

void __thiscall linkSubEffectHandler(SFSpell *spell, sub_effect_handler_ptr handler)
{
    spell->sub_effect_handler = handler;
}

void __thiscall linkRefreshHandler(SFSpell *spell, refresh_handler_ptr handler)
{
    spell->spell_refresh_handler = handler;
}

void __thiscall linkDealDamageHandler(SFSpell *spell, damage_handler_ptr handler, SpellDamagePhase phase)
{
    spell->deal_damage_handler = handler;
    spell->damage_phase = phase;
}
/**
 * Registers the mod spells and performs basic conflict checking.
 *
 * This function iterates over the g_internal_spell_list and registers each spell by
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

    SFMod *temp = g_current_mod;
    uint8_t spell_count_for_mod = 0;
    for (SFSpell *spell_data : g_internal_spell_list)
    {
        uint16_t spell_id = spell_data->spell_id;
        uint16_t spell_effect_id = spell_data->spell_effect_id;
        handler_ptr spell_type_handler = spell_data->spell_type_handler;
        handler_ptr spell_effect_handler = spell_data->spell_effect_handler;
        handler_ptr spell_end_handler = spell_data->spell_end_handler;
        refresh_handler_ptr spell_refresh_handler = spell_data->spell_refresh_handler;
        sub_effect_handler_ptr sub_effect_handler = spell_data->sub_effect_handler;
        damage_handler_ptr deal_damage_handler = spell_data->deal_damage_handler;
        SpellDamagePhase phase = spell_data->damage_phase;
        SFMod *parent_mod = spell_data->parent_mod;

        g_current_mod = spell_data->parent_mod;

        if (temp != g_current_mod)
        {
            if (spell_count_for_mod != 0)
            {
                char spell_count_info[256];
                snprintf(spell_count_info, sizeof(spell_count_info), "| - Finished Registration of %d spells for %s", spell_count_for_mod, temp->mod_id);
                log_info(spell_count_info);
                spell_count_for_mod = 0;
            }

            char info[256];
            snprintf(info, sizeof(info), "| - Starting Registration for [%s by %s]", parent_mod->mod_id, parent_mod->mod_author);
            log_info(info);
            temp = g_current_mod;
        }
        else
        {
            spell_count_for_mod = spell_count_for_mod + 1;
        }

        // Check for conflicts
        if (spell_id_map.find(spell_id) != spell_id_map.end())
        {
            char error_msg[256];
            SFMod *conflict_mod = spell_id_map[spell_id];
            snprintf(error_msg, sizeof(error_msg), "| - Mod Conflict Detected [%s]: Spell ID [%d] is already registered by [%s]", parent_mod->mod_id, spell_id, conflict_mod->mod_id);
            log_error(error_msg);
            g_error_count = g_error_count + 1;
        }

        if (spell_effect_id_map.find(spell_effect_id) != spell_effect_id_map.end())
        {
            char error_msg[256];
            SFMod *conflict_mod = spell_effect_id_map[spell_effect_id];
            snprintf(error_msg, sizeof(error_msg), "| - Mod Conflict Detected [%s]: Spell Effect ID [%d] is already registered by [%s]", parent_mod->mod_id, spell_effect_id, conflict_mod->mod_id);
            log_error(error_msg);
            g_error_count = g_error_count + 1;
        }

        // Update Conflict Maps
        spell_id_map[spell_id] = parent_mod;

        if (spell_effect_id != 0x00)
        {
            spell_effect_id_map[spell_effect_id] = parent_mod;
        }

        // Do Registration
        if (spell_type_handler != nullptr)
        {
            registerSpellTypeHandler(spell_id, spell_type_handler);
        }

        if (spell_effect_handler != nullptr)
        {
            registerEffectHandler(spell_effect_id, spell_effect_handler);
        }

        if (spell_refresh_handler != nullptr)
        {

            registerSpellRefreshHandler(spell_id, spell_refresh_handler);
        }

        if (spell_end_handler != nullptr)
        {
            registerSpellEndHandler(spell_id, spell_end_handler);
        }

        if (sub_effect_handler != nullptr)
        {
            registerSubEffectHandler(spell_id, sub_effect_handler);
        }

        if (deal_damage_handler != nullptr) 
        {
            registerSpellDamageHandler(spell_id, deal_damage_handler, phase);
        }
    }

    if (spell_count_for_mod != 0)
    {
        char spell_count_info[256];
        snprintf(spell_count_info, sizeof(spell_count_info), "| - Finished Registration of %d spells for %s", spell_count_for_mod, temp->mod_id);
        log_info(spell_count_info);
        spell_count_for_mod = 0;
    }
}
