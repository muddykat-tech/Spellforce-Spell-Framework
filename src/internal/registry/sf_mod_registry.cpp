#include "sf_mod_registry.h"

#include "spell_data_registries/sf_spelltype_registry.h"
#include "spell_data_registries/sf_spelleffect_registry.h"
#include "spell_data_registries/sf_spellend_registry.h"
#include "spell_data_registries/sf_subeffect_registry.h"
#include "spell_data_registries/sf_spellrefresh_registry.h"
#include "spell_data_registries/sf_onhit_registry.h"
#include "spell_data_registries/sf_spelldamage_registry.h"
#include "../../api/structures/sf_building_structures.h"
#include "ai_data_registries/sf_ai_aoe_registry.h"
#include "ai_data_registries/sf_ai_avoidance_registry.h"
#include "ai_data_registries/sf_ai_single_target_registry.h"

#include <windows.h>
#include <iostream>
#include <map>
#include <list>
#include <cstdint>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "../core/sf_building_loader.h"

std::list<SFBuilding *> g_internal_building_list;

SFBuilding *__thiscall registerBuilding(const char *building_json_name)
{
    SFBuilding *building = new SFBuilding;
    building->building_id = -1;
    strncpy(building->building_json_name, building_json_name, sizeof(building->building_json_name) - 1);
    building->parent_mod = g_current_mod;

    g_internal_building_list.push_back(building);

    return building;
}

std::list<SFSpell *> g_internal_spell_list;

SFSpell *__thiscall registerSpell(uint16_t spell_id)
{
    SFSpell *sf_spell = new SFSpell;
    sf_spell->spell_id = spell_id;
    sf_spell->spell_effect_id = 0x00;
    sf_spell->spell_tags = 0x0;
    sf_spell->spell_type_handler = nullptr;
    sf_spell->spell_effect_handler = nullptr;
    sf_spell->spell_end_handler = nullptr;
    sf_spell->spell_onhit_handler = nullptr;
    sf_spell->spell_refresh_handler = nullptr;
    sf_spell->sub_effect_handler = nullptr;
    sf_spell->parent_mod = g_current_mod;
    sf_spell->deal_damage_handler = nullptr;
    sf_spell->ai_aoe_handler = nullptr;
    sf_spell->ai_single_handler = nullptr;
    sf_spell->ai_avoidance_handler = nullptr;
    sf_spell->damage_phase = SpellDamagePhase::DEFAULT;
    sf_spell->hit_phase = OnHitPhase::PHASE_5;
    g_internal_spell_list.push_back(sf_spell);

    return sf_spell;
}

void __thiscall applySpellTag(SFSpell *spell, SpellTag tag)
{
    uint16_t current_tags = spell->spell_tags;
    spell->spell_tags = current_tags | tag;
}

void __thiscall linkTypeHandler(SFSpell *spell, handler_ptr typeHandler)
{
    spell->spell_type_handler = typeHandler;
}

void __thiscall linkSingleTargetAIHandler(SFSpell *spell,
                                          ai_single_handler_ptr handler)
{
    spell->ai_single_handler = handler;
}

void __thiscall linkAvoidanceAIHandler(SFSpell *spell,
                                       ai_avoidance_handler_ptr handler)
{
    spell->ai_avoidance_handler = handler;
}

void __thiscall linkAOEAIHandler(SFSpell *spell, ai_aoe_handler_ptr handler)
{
    spell->ai_aoe_handler = handler;
}

void __thiscall linkOnHitHandler(SFSpell *spell, onhit_handler_ptr onhitHandler,
                                 OnHitPhase phase)
{
    spell->spell_onhit_handler = onhitHandler;
    spell->hit_phase = phase;
}

void __thiscall linkEffectHandler(SFSpell *spell, uint16_t spell_effect_id,
                                  handler_ptr effectHandler)
{
    spell->spell_effect_id = spell_effect_id;
    spell->spell_effect_handler = effectHandler;
}

void __thiscall linkEndHandler(SFSpell *spell, handler_ptr endHandler)
{
    spell->spell_end_handler = endHandler;
}

void __thiscall linkSubEffectHandler(SFSpell *spell,
                                     sub_effect_handler_ptr handler)
{
    spell->sub_effect_handler = handler;
}

void __thiscall linkRefreshHandler(SFSpell *spell, refresh_handler_ptr handler)
{
    spell->spell_refresh_handler = handler;
}

void __thiscall linkDealDamageHandler(SFSpell *spell,
                                      damage_handler_ptr handler,
                                      SpellDamagePhase phase)
{
    spell->deal_damage_handler = handler;
    spell->damage_phase = phase;
}

uint16_t __thiscall getSpellTags(uint16_t spell_line_id)
{
    for (auto &entry : g_internal_spell_list)
    {
        if (entry->spell_id == spell_line_id)
        {
            return entry->spell_tags;
        }
    }
    return 0x0;
}

/**
 * @brief Registers the mod spells and performs basic conflict checking.
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
        onhit_handler_ptr onhit_handler = spell_data->spell_onhit_handler;
        damage_handler_ptr deal_damage_handler = spell_data->deal_damage_handler;
        ai_aoe_handler_ptr ai_aoe_handler = spell_data->ai_aoe_handler;
        ai_avoidance_handler_ptr ai_avoidance_handler = spell_data->ai_avoidance_handler;
        ai_single_handler_ptr ai_single_handler = spell_data->ai_single_handler;

        SpellDamagePhase damage_phase = spell_data->damage_phase;
        OnHitPhase onhit_phase = spell_data->hit_phase;
        SFMod *parent_mod = spell_data->parent_mod;
        g_current_mod = spell_data->parent_mod;

        if (temp != g_current_mod)
        {
            if (spell_count_for_mod != 0)
            {
                char spell_count_info[256];
                snprintf(spell_count_info, sizeof(spell_count_info),
                         "| - Finished Registration of %d spells for %s",
                         spell_count_for_mod, temp->mod_id);
                log_info(spell_count_info);
                spell_count_for_mod = 0;
            }

            char info[256];
            snprintf(info, sizeof(info),
                     "| - Starting Registration for [%s by %s]",
                     parent_mod->mod_id, parent_mod->mod_author);
            parent_mod->mod_errors[0] = '\0';
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
            if (spell_id < 242)
            {
                snprintf(error_msg, sizeof(error_msg),
                         "| - %s has Overwritten a vanilla spell ID [%d], this was previously registered by [%s]",
                         parent_mod->mod_id, spell_id, conflict_mod->mod_id);
                log_warning(error_msg);
            }
            else
            {
                snprintf(error_msg, sizeof(error_msg),
                         "| - Mod Conflict Detected [%s]: Spell ID [%d] is already registered by [%s]",
                         parent_mod->mod_id, spell_id, conflict_mod->mod_id);
                log_error(error_msg);
                snprintf(conflict_mod->mod_errors,
                         sizeof(parent_mod->mod_errors),
                         "%sSpell ID [%d] was overwritten by %s\n",
                         conflict_mod->mod_errors, spell_id,
                         parent_mod->mod_id);
                g_error_count = g_error_count + 1;
            }
        }

        if (spell_effect_id_map.find(spell_effect_id) !=
            spell_effect_id_map.end())
        {
            char error_msg[256];
            SFMod *conflict_mod = spell_effect_id_map[spell_effect_id];
            if (spell_effect_id < 0xa6)
            {
                snprintf(error_msg, sizeof(error_msg),
                         "| - %s has Overwritten a vanilla spell effect ID [%d] this was previously registered by [%s]",
                         parent_mod->mod_id, spell_effect_id,
                         conflict_mod->mod_id);
                log_warning(error_msg);
            }
            else
            {
                snprintf(error_msg, sizeof(error_msg),
                         "| - Mod Conflict Detected [%s]: Spell Effect ID [%d] is already registered by [%s]",
                         parent_mod->mod_id, spell_effect_id,
                         conflict_mod->mod_id);
                log_error(error_msg);

                snprintf(conflict_mod->mod_errors,
                         sizeof(parent_mod->mod_errors),
                         "%sSpell Effect ID [%d] was overwritten by %s\n",
                         conflict_mod->mod_errors, spell_effect_id,
                         parent_mod->mod_id);

                g_error_count = g_error_count + 1;
            }
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

        if (onhit_handler != nullptr)
        {
            registerOnHitHandler(spell_id, onhit_handler, onhit_phase);
        }

        if (deal_damage_handler != nullptr)
        {
            registerSpellDamageHandler(spell_id, deal_damage_handler,
                                       damage_phase);
        }

        if(ai_single_handler != nullptr)
        {
            registerAiSingleTargetHandler(spell_id, ai_single_handler);
        }

        if(ai_aoe_handler != nullptr)
        {
            registerAiAOEHandler(spell_id, ai_aoe_handler);
        }

        if(ai_avoidance_handler != nullptr)
        {
            registerAiAvoidanceHandler(spell_id, ai_avoidance_handler);
        }
    }

    if (spell_count_for_mod != 0)
    {
        char spell_count_info[256];
        snprintf(spell_count_info, sizeof(spell_count_info),
                 "| - Finished Registration of %d spells for %s",
                 spell_count_for_mod, temp->mod_id);
        log_info(spell_count_info);
        spell_count_for_mod = 0;
    }
}

addBuilding_ptr AddBuilding;
addBuildingAuxData_ptr AddAuxData;
setCollisionListSize_ptr setListSize;

void addCollisionEntry(uint_list_node *list, int32_t posX, int32_t posY, int8_t index)
{
    uint32_t *offset = list->first;
    offset[index*2] = (posX * 0x10000) / 140;
    offset[index*2+1] = (posY * 0x10000) / 140;
}

void register_building_to_game(SFBuilding *building, const Building *src)
{
    SF_CGdResource *CGdResource = (SF_CGdResource*)(*(uint32_t *)ASI::AddrOf(0x94867C));
    BuildingAuxEntry *core_entry = (BuildingAuxEntry *)((uint32_t)CGdResource + 0x8);

    addBuilding_ptr AddBuilding = (addBuilding_ptr)(ASI::AddrOf(0x2669f0));
    addBuildingAuxData_ptr AddAuxData = (addBuildingAuxData_ptr)(ASI::AddrOf(0x266210));
    setCollisionListSize_ptr setListSize = (setCollisionListSize_ptr)(ASI::AddrOf(0x274b30));

    astruct32 new_building = {0};
    BuildingAuxEntry_related new_entry = {0};

    if(!src->found_id)
    {
        log_error("Missing Required 'id' field in buidling JSON: %s", building->building_json_name);
    }

    building->building_id = src->id;

    AddBuilding(CGdResource, &new_building, &building->building_id);
    auto &bd = new_building.ref1->building;

    bd.id = building->building_id;
    bd.race = building->race;
    bd.can_enter = building->can_enter;
    bd.slot_count = building->slot_count;
    bd.building_required = building->building_required;
    bd.worker_cycle = building->worker_cycle;
    bd.name_id = building->name_id;
    bd.health = building->health;
    bd.ext_description_id = building->ext_description_id;
    bd.initial_angle = building->initial_angle;
    bd.flags = building->flags;

    AddAuxData(core_entry, &new_entry, &building->building_id);
    new_entry.data->centerX = building->centerX;
    new_entry.data->centerY = building->centerY;
    memcpy(new_entry.data->shadows, building->shadows, sizeof(building->shadows));

    for (int i = 0; i < src->collision_count; i++)
    {
        int point_count = src->collisions[i].point_count;
        setListSize(&(new_entry.data->collisions[i]), point_count);
        for(int k = 0; k < point_count; k++)
        {
            WorldCoord wc = src->collisions[i].points[k];
            addCollisionEntry(&(new_entry.data->collisions[i]),
                          wc.X,
                          wc.Y,
                          i);
        }
    }

    new_entry.data->poly_count = building->poly_count;
    new_entry.data->resource_req_num = building->resource_req_num;
    for (int i = 0; i < building->resource_req_num; ++i)
    {
        new_entry.data->resource_req_type[i] = building->resource_req_type[i];
        new_entry.data->resource_req_amount[i] = building->resource_req_amount[i];
    }

    AddAuxData(core_entry, &new_entry, &building->building_id);
    log_info("%s has successfully registered the [%s] building to the game using ID [%u]", g_current_mod->mod_id, building->building_json_name, building->building_id);
}

void register_mod_buildings()
{
    std::map<int, SFMod *> building_id_map;

    SFMod *temp = g_current_mod;
    int building_count_for_mod = 0;

    for (SFBuilding *building_data : g_internal_building_list)
    {
        SFMod *parent_mod = building_data->parent_mod;
        g_current_mod = parent_mod;

        if (temp != g_current_mod)
        {
            if (building_count_for_mod > 0)
            {
                char log_line[256];
                snprintf(log_line, sizeof(log_line),
                         "| - Finished Registration of %d buildings for %s",
                         building_count_for_mod, temp->mod_id);
                log_info(log_line);
                building_count_for_mod = 0;
            }

            char info[256];
            snprintf(info, sizeof(info),
                     "| - Starting Registration for [%s by %s]",
                     parent_mod->mod_id, parent_mod->mod_author);
            parent_mod->mod_errors[0] = '\0';
            log_info(info);
            temp = g_current_mod;
        }
        else
        {
            building_count_for_mod++;
        }

        Building parsed_building;
        if (!parse_building_json_entrypoint(building_data->building_json_name, g_current_mod->mod_id, &parsed_building))
        {
            log_error("Building JSON structure invalid: %s", building_data->building_json_name);
            continue;
        }

        building_data->race = parsed_building.race;
        building_data->can_enter = parsed_building.can_enter;
        building_data->slot_count = parsed_building.slot_count;
        building_data->building_required = parsed_building.building_required;
        building_data->worker_cycle = 0;
        building_data->name_id = parsed_building.name_id;
        building_data->health = parsed_building.health;
        building_data->ext_description_id = parsed_building.description_id;
        building_data->initial_angle = 0;
        building_data->flags = parsed_building.flags;
        building_data->centerX = parsed_building.center_x;
        building_data->centerY = parsed_building.center_y;

        building_data->poly_count = parsed_building.collision_count;
        building_data->resource_req_num = parsed_building.resource_count;
        for (int i = 0; i < parsed_building.resource_count && i < MAX_RESOURCES; i++)
        {
            // TODO UnSchtalch Convert the string 'parsed_building.resources[i].type'
            // into the ID type we need for the AUX data
            building_data->resource_req_type[i] = 0;
            building_data->resource_req_amount[i] = 0;//parsed_building.resources[i].amount;
        }

        memcpy(building_data->shadows, (uint8_t[]){1, 0, 0, 0}, sizeof(building_data->shadows));

        register_building_to_game(building_data, &parsed_building);
    }
}
