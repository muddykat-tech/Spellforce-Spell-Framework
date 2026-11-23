#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sf_wrappers.h"
#include "sf_hooks.h"

#include "hooks/sf_menu_hook.h"
#include "hooks/sf_console_hook.h"
#include "hooks/sf_vanilla_fix_hook.h"
#include "../registry/sf_mod_registry.h"

#define LOG_BUFFER_SIZE 1024

/**
 * @defgroup wrappers Framework Wrappers
 * @ingroup Internal
 * @brief Contains wrapper functions that streamline complex setups.
 * @details This file consolidates multiple setup steps into single functions for ease of use,
 * handling the initialization of variables, flags, and resources required for specific tasks.
 * @addtogroup wrappers
 * @{
 */

typedef uint32_t (__thiscall *XDataGet_ptr)(void *_this, uint16_t key, uint8_t keyType);
typedef uint32_t (__thiscall *CGdXDataExists_ptr)(void *_this, uint16_t key, uint8_t keyType);

SF_String_ctor_ptr g_create_sf_string;
SF_String_dtor_ptr g_destroy_sf_string;
has_spell_effect_ptr has_spell_effect;
FUN_0069eaf0_ptr FUN_0069eaf0;
fidfree_ptr fidFree;

XDataGet_ptr XDataGet;
CGdXDataExists_ptr CGdXDataExists;

get_phys_damage_reduction_ptr g_get_damage_reduction;

//checky thiscall but not really. ~muddykat (fix this later add support for non thiscalls)
uint32_t __thiscall getDistance(SF_Coord *pointA, SF_Coord *pointB)
{
    uint32_t delta;
    uint32_t uVar1;
    uint32_t uVar2;
    uint32_t uVar3;
    uint32_t uVar4;

    delta = (uint32_t)(uint16_t)pointA->X - (uint32_t)(uint16_t)pointB->X;
    uVar2 = (int)delta >> 0x1f;
    uVar2 = (delta ^ uVar2) - uVar2;
    uVar4 = uVar2 & 0xffff;
    delta = (uint32_t)(uint16_t)pointA->Y - (uint32_t)(uint16_t)pointB->Y;
    uVar3 = (int)delta >> 0x1f;
    uVar3 = (delta ^ uVar3) - uVar3;
    uVar1 = uVar3 & 0xffff;
    delta = uVar1;
    if ((uint16_t)uVar2 < (uint16_t)uVar3)
    {
        delta = uVar4;
        uVar4 = uVar1;
    }
    return ((delta * 0xd) >> 5) + uVar4;
}

bool __thiscall isSiegeUnit (SF_CGdFigure *_this, uint16_t figure_index)
{
    if ((_this->figures[figure_index].unit_data_id == 2236) ||
        (_this->figures[figure_index].unit_data_id == 2238) ||
        (_this->figures[figure_index].unit_data_id == 2239) ||
        (_this->figures[figure_index].unit_data_id == 2244) ||
        (_this->figures[figure_index].unit_data_id == 2245) ||
        (_this->figures[figure_index].unit_data_id == 2249) )
    {
        return true;
    }
    return false;
}

void initialize_wrapper_data_hooks()
{
    XDataGet = (XDataGet_ptr)(ASI::AddrOf(0x354210));
    FUN_0069eaf0 = (FUN_0069eaf0_ptr)(ASI::AddrOf(0x29EAF0));
    fidFree = (fidfree_ptr)(ASI::AddrOf(0x6B6E25));
    has_spell_effect = (has_spell_effect_ptr)(ASI::AddrOf(0x2fe46f));
    g_get_damage_reduction = (get_phys_damage_reduction_ptr)(ASI::AddrOf(0x317070));

    CGdXDataExists = (CGdXDataExists_ptr)(ASI::AddrOf(0x3549d0));
}

void log_message(const char *filename, const char *format, ...)
{
    char buffer[LOG_BUFFER_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, LOG_BUFFER_SIZE, format, args);
    va_end(args);

    FILE *file = fopen(filename, "a");
    if (file != NULL)
    {
        fprintf(file, "%s\n", buffer);
        fclose(file);
    }
}

/**
 * @brief Shared body for the warning logger.
 *
 */
static void log_warning_v(DebugLevel level, const char *format, va_list args)
{
    if (level > global_debug_level)
        return;

    char buffer[LOG_BUFFER_SIZE];
    vsnprintf(buffer, LOG_BUFFER_SIZE, format, args);

    char modifiedMessage[LOG_BUFFER_SIZE];
    snprintf(modifiedMessage, LOG_BUFFER_SIZE, "[WARNING] %s", buffer);
    console_log(modifiedMessage);
}

void log_warning(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    log_warning_v(DEBUG_INFO, format, args);
    va_end(args);
}

void log_warning_level(DebugLevel level, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    log_warning_v(level, format, args);
    va_end(args);
}

void log_info(const char *format, ...)
{
    char buffer[LOG_BUFFER_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, LOG_BUFFER_SIZE, format, args);
    va_end(args);

    char modifiedMessage[LOG_BUFFER_SIZE];
    snprintf(modifiedMessage, LOG_BUFFER_SIZE, "[INFO] %s", buffer);
    console_log(modifiedMessage);
}

void log_error(const char *format, ...)
{
    char buffer[LOG_BUFFER_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, LOG_BUFFER_SIZE, format, args);
    va_end(args);

    int lastError = GetLastError();

    char modifiedMessage[LOG_BUFFER_SIZE];
    snprintf(modifiedMessage, LOG_BUFFER_SIZE,
             "[ERROR] %s [(Win32 SysErr) Last Error: %d]", buffer, lastError);
    console_log(modifiedMessage);
}

const char *debug_level_to_string(DebugLevel level)
{
    switch (level)
    {
        case DEBUG_INFO: return "INFO";
        case DEBUG_LOW:  return "LOW";
        case DEBUG_MED:  return "MED";
        case DEBUG_HIGH: return "HIGH";
        case DEBUG_ALL:  return "ALL";
        default:         return "ERR";
    }
}

void log_debug(DebugLevel level, const char *format, ...)
{
    if (level > global_debug_level)
        return;

    char buffer[LOG_BUFFER_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, LOG_BUFFER_SIZE, format, args);
    va_end(args);

    char modifiedMessage[LOG_BUFFER_SIZE];
    snprintf(modifiedMessage, LOG_BUFFER_SIZE, "[DEBUG %s] %s", debug_level_to_string(level), buffer);
    console_log(modifiedMessage);
}

SFLog sf_logger;
SFLog *setup_logger()
{
    sf_logger.logError = &log_error;
    sf_logger.logInfo = &log_info;
    sf_logger.logWarning = &log_warning;
    sf_logger.logDebug = &log_debug;
    return &sf_logger;
}

// TODO: Parse me through an as an ActionAPI Wrapped Function
bool __thiscall isActionMelee(SF_SGtFigureAction *_this)
{
    if ((_this->type == 10000) || (_this->type == 10001))
    {
        return 1;
    }
    return 0;
}

uint32_t __thiscall getBuildingXData(SF_CGdBuildingToolbox *_this, uint16_t building_index, uint8_t key_type)
{
    uint16_t key = _this->CGdBuilding->buildings[building_index].xdata_key;
    if (key!=0)
    {
        return XDataGet(_this->CGdXDataList, key, key_type);
    }
    return 0;
}

void __thiscall setupFigureIterator(CGdFigureIterator *iterator,
                                    SF_CGdSpell *spell)
{
    iteratorAPI.figureIteratorInit(iterator, 0x0, 0x0, 0x3ff, 0x3ff);
    iteratorAPI.figureIteratorSetPointers(iterator, spell->CGdFigure, spell->CGdTile, spell->CGdWorld);
}

bool __thiscall hasSpellTag(uint16_t spell_id, SpellTag tag)
{
    return spellAPI.getSpellTags(spell_id) & tag;
}

bool __thiscall hasBuildingTag(uint8_t building_type, BuildingTag tag)
{
    if ((buildingAPI.getBuildingTags(building_type) & tag) != 0)
    {
        return 1;
    }
    return 0;
}

bool __thiscall buildingIsScavenger(SF_CGdBuilding *_this, uint16_t building_index)
{
    return buildingAPI.hasBuildingTag(_this->buildings[building_index].type, BuildingTag::SCAVENGER_BUILDING);
}

bool __thiscall buildingIsHabitable(SF_CGdBuilding *_this, uint16_t building_index)
{
    return buildingAPI.hasBuildingTag(_this->buildings[building_index].type, BuildingTag::HABITABLE_BUILDING);
}

bool __thiscall buildingIsHabitableSingle(SF_CGdBuilding *_this, uint16_t building_index)
{
    return buildingAPI.hasBuildingTag(_this->buildings[building_index].type, BuildingTag::HABITABLE_SINGLE_BUILDING);
}

bool __thiscall buildingIsFisher(SF_CGdBuilding *_this, uint16_t building_index)
{
    return buildingAPI.hasBuildingTag(_this->buildings[building_index].type, BuildingTag::FISHER_BUILDING);
}

bool __thiscall buildingIsFoodstore(SF_CGdBuilding *_this, uint16_t building_index)
{
    return buildingAPI.hasBuildingTag(_this->buildings[building_index].type, BuildingTag::FOODSTORE_BUILDING);
}

bool __thiscall buildingIsForge(SF_CGdBuilding *_this, uint16_t building_index)
{
    return buildingAPI.hasBuildingTag(_this->buildings[building_index].type, BuildingTag::FORGE_BUILDING);
}

bool __thiscall buildingIsGatherer(SF_CGdBuilding *_this, uint16_t building_index)
{
    return buildingAPI.hasBuildingTag(_this->buildings[building_index].type, BuildingTag::GATHERER_BUILDING);
}

bool __thiscall buildingIsHQ(SF_CGdBuilding *_this, uint16_t building_index)
{
    return buildingAPI.hasBuildingTag(_this->buildings[building_index].type, BuildingTag::HQ_BUILDING);
}

bool __thiscall buildingIsHunter(SF_CGdBuilding *_this, uint16_t building_index)
{
    return buildingAPI.hasBuildingTag(_this->buildings[building_index].type, BuildingTag::HUNTER_BUILDING);
}

bool __thiscall buildingIsIronMine(SF_CGdBuilding *_this, uint16_t building_index)
{
    return (buildingAPI.hasBuildingTag(_this->buildings[building_index].type, BuildingTag::MINER_BUILDING)
            && !buildingAPI.hasBuildingTag(_this->buildings[building_index].type, BuildingTag::MOONSILVER_BUILDING));
}

bool __thiscall buildingIsMaceCarver(SF_CGdBuilding *_this, uint16_t building_index)
{
    return buildingAPI.hasBuildingTag(_this->buildings[building_index].type, BuildingTag::MACE_CARVER_BUILDING);
}

bool __thiscall buildingIsMoonsilverMine(SF_CGdBuilding *_this, uint16_t building_index)
{
    return (buildingAPI.hasBuildingTag(_this->buildings[building_index].type, BuildingTag::MINER_BUILDING)
            && buildingAPI.hasBuildingTag(_this->buildings[building_index].type, BuildingTag::MOONSILVER_BUILDING));
}

bool __thiscall buildingIsQuarry(SF_CGdBuilding *_this, uint16_t building_index)
{
    return buildingAPI.hasBuildingTag(_this->buildings[building_index].type, BuildingTag::QUARRY_BUILDING);
}

bool __thiscall buildingIsSawmill(SF_CGdBuilding *_this, uint16_t building_index)
{
    return buildingAPI.hasBuildingTag(_this->buildings[building_index].type, BuildingTag::SAWMILL_BUILDING);
}

bool __thiscall buildingIsSmelter(SF_CGdBuilding *_this, uint16_t building_index)
{
    return buildingAPI.hasBuildingTag(_this->buildings[building_index].type, BuildingTag::SMELTER_BUILDING);
}

bool __thiscall buildingIsStonecutter(SF_CGdBuilding *_this, uint16_t building_index)
{
    return buildingAPI.hasBuildingTag(_this->buildings[building_index].type, BuildingTag::STONEMASON_BUILDING);
}

bool __thiscall buildingIsTemple(SF_CGdBuilding *_this, uint16_t building_index)
{
    return buildingAPI.hasBuildingTag(_this->buildings[building_index].type, BuildingTag::TEMPLE_BUILDING);
}

bool __thiscall buildingIsTower(SF_CGdBuilding *_this, uint16_t building_index)
{
    return buildingAPI.hasBuildingTag(_this->buildings[building_index].type, BuildingTag::TOWER_BUILDING);
}

bool __thiscall buildingIsWoodcutter(SF_CGdBuilding *_this, uint16_t building_index)
{
    return buildingAPI.hasBuildingTag(_this->buildings[building_index].type, BuildingTag::WOODCUTTER_BUILDING);
}

bool __thiscall buildingIsShrine(SF_CGdBuilding *_this, uint16_t building_index)
{
    return buildingAPI.hasBuildingTag(_this->buildings[building_index].type, BuildingTag::SHRINE_BUILDING);
}

// Some funky stuff to clean up Iterator memory, not 100% sure if correct
void __thiscall disposeFigureIterator(CGdFigureIterator *iterator)
{
    uint32_t unused;
    FUN_0069eaf0(&(iterator->data.offset_0x30), &unused,
                 ((AutoClass69 *)iterator->data.offset_0x30.ac69_ptr1)->
                 ac69_ptr1, iterator->data.offset_0x30.ac69_ptr1);
    fidFree(iterator->data.offset_0x30.ac69_ptr1);
}

void __thiscall spellEffectCallback(SF_CGdSpell *_this, uint16_t source_index,
                                    uint16_t spell_index,
                                    bool (*condition)(SF_CGdSpell *_this,
                                                      uint16_t source_index,
                                                      uint16_t spell_index),
                                    void (*callback)(SF_CGdSpell *_this,
                                                     uint16_t source_index,
                                                     uint16_t spell_index))
{
    if (!condition || !callback)
        return;

    uint16_t node_id = figureAPI.getSpellJobStartNode(_this->CGdFigure,
                                                      source_index);
    while (node_id != 0)
    {
        uint16_t current_index =
            toolboxAPI.getSpellIndexFromDLL(
                (uint32_t *)_this->CGdDoubleLinkedList, node_id);
        if ((*condition)(_this, current_index, spell_index))
        {
            (*callback)(_this, source_index, current_index);
            break;
        }
        node_id =
            toolboxAPI.getNextNode((uint32_t *)_this->CGdDoubleLinkedList,
                                   node_id);
    }
}


int8_t __thiscall addBonusMultExt (FigureStatisticExt *_this, int8_t value)
{
    if (value < 0)
    {
        if ((int)(_this->bonus_multiplier) + (int)value < -127)
        {
            _this->bonus_multiplier = 128;
            return _this->bonus_multiplier;
        }
        _this->bonus_multiplier += value;
        return _this->bonus_multiplier;
    }
    if ((int)_this->bonus_multiplier + (int)value > 126)
    {
        _this->bonus_multiplier = 127;
        return _this->bonus_multiplier;
    }
    _this->bonus_multiplier += value;
    return _this->bonus_multiplier;
}


uint16_t __thiscall getCurrentStat(SF_CGdFigure *_this, uint16_t target, StatisticDataKey key)
{
    switch (key)
    {
        case ARMOR:
        {
            return get_figure_statistic_current_ac(_this, target);
        }
        case AGILITY:
        {
            return get_figure_statistic_current_agi(_this, target);
        }
        case CHARISMA:
        {
            return get_figure_statistic_current_cha(_this, target);
        }
        case DEXTERITY:
        {
            return get_figure_statistic_current_dex(_this, target);
        }
        case INTELLIGENCE:
        {
            return get_figure_statistic_current_int(_this, target);
        }
        case STRENGTH:
        {
            return get_figure_statistic_current_str(_this, target);
        }
        case STAMINA:
        {
            return get_figure_statistic_current_sta(_this, target);
        }
        case MANA:
        {
            return get_figure_statistic_current_mp(_this, target);
        }
        case HEALTH:
        {
            return get_figure_statistic_current_hp(_this, target);
        }
        case WISDOM:
        {
            return get_figure_statistic_current_wis(_this, target);
        }
        case RESISTANCE_FIRE:
        {
            return get_figure_statistic_current_fire_res(_this, target);
        }
        case RESISTANCE_ICE:
        {
            return get_figure_statistic_current_ice_res(_this, target);
        }
        case RESISTANCE_MENTAL:
        {
            return get_figure_statistic_current_mental_res(_this, target);
        }
        case RESISTANCE_BLACK:
        {
            return get_figure_statistic_current_black_res(_this, target);
        }
        case WALK_SPEED:
        {
            return get_figure_statistic_current_walk_spd(_this, target);
        }
        case FIGHT_SPEED:
        {
            return get_figure_statistic_current_fight_spd(_this, target);
        }
        case CAST_SPEED:
        {
            return get_figure_statistic_current_cast_spd(_this, target);
        }
        default:
        {
            return 0;
        }
    }
}


uint16_t __thiscall getMaxStat(SF_CGdFigure *_this, uint16_t target, StatisticDataKey key)
{
    switch (key)
    {
        case ARMOR:
        {
            return get_figure_statistic_current_ac(_this, target);
        }
        case AGILITY:
        {
            return get_figure_statistic_current_agi(_this, target);
        }
        case CHARISMA:
        {
            return get_figure_statistic_current_cha(_this, target);
        }
        case DEXTERITY:
        {
            return get_figure_statistic_current_dex(_this, target);
        }
        case INTELLIGENCE:
        {
            return get_figure_statistic_current_int(_this, target);
        }
        case STRENGTH:
        {
            return get_figure_statistic_current_str(_this, target);
        }
        case STAMINA:
        {
            return get_figure_statistic_max_sta(_this, target);
        }
        case MANA:
        {
            return get_figure_statistic_max_mp(_this, target);
        }
        case HEALTH:
        {
            return get_figure_statistic_max_hp(_this, target);
        }
        case WISDOM:
        {
            return get_figure_statistic_current_wis(_this, target);
        }
        case RESISTANCE_FIRE:
        {
            return get_figure_statistic_current_fire_res(_this, target);
        }
        case RESISTANCE_ICE:
        {
            return get_figure_statistic_current_ice_res(_this, target);
        }
        case RESISTANCE_MENTAL:
        {
            return get_figure_statistic_current_mental_res(_this, target);
        }
        case RESISTANCE_BLACK:
        {
            return get_figure_statistic_current_black_res(_this, target);
        }
        case WALK_SPEED:
        {
            return get_figure_statistic_current_walk_spd(_this, target);
        }
        case FIGHT_SPEED:
        {
            return get_figure_statistic_current_fight_spd(_this, target);
        }
        case CAST_SPEED:
        {
            return get_figure_statistic_current_cast_spd(_this, target);
        }
        default:
        {
            return 0;
        }
    }
}

void __thiscall addBonusMultToStatistic(SF_CGdFigure *figure,
                                        StatisticDataKey key, uint16_t target,
                                        int8_t value)
{
    FigureStatistic *statistic = NULL;
    switch (key)
    {
        case STAMINA: addBonusMultExt(&figure->figures[target].stamina, value); return;
        case MANA: addBonusMultExt(&figure->figures[target].mana, value); return;
        case HEALTH: addBonusMultExt(&figure->figures[target].health, value); return;

        case ARMOR:              statistic = &figure->figures[target].armor; break;
        case AGILITY:            statistic = &figure->figures[target].agility; break;
        case CHARISMA:           statistic = &figure->figures[target].charisma; break;
        case DEXTERITY:          statistic = &figure->figures[target].dexterity; break;
        case INTELLIGENCE:       statistic = &figure->figures[target].intelligence; break;
        case STRENGTH:           statistic = &figure->figures[target].strength; break;
        case WISDOM:             statistic = &figure->figures[target].wisdom; break;
        case RESISTANCE_FIRE:    statistic = &figure->figures[target].resistance_fire; break;
        case RESISTANCE_ICE:     statistic = &figure->figures[target].resistance_ice; break;
        case RESISTANCE_MENTAL:  statistic = &figure->figures[target].resistance_mental; break;
        case RESISTANCE_BLACK:   statistic = &figure->figures[target].resistance_black; break;
        case WALK_SPEED:         statistic = &figure->figures[target].walk_speed; break;
        case FIGHT_SPEED:        statistic = &figure->figures[target].fight_speed; break;
        case CAST_SPEED:         statistic = &figure->figures[target].cast_speed; break;

        default: log_warning("INVALID STATISTIC KEY: %d", key); return;
    }

    figureAPI.addBonusMult(statistic, value);
}

void __thiscall spellClearFigureFlag(SF_CGdSpell *_this, uint16_t spell_id,
                                     SpellFlagKey key)
{
    switch (key)
    {
        case CHECK_SPELLS_BEFORE_CHECK_BATTLE:
        {
            spellAPI.figClrChkSplBfrChkBattle(_this, spell_id, 0);
            break;
        }
        case CHECK_SPELLS_BEFORE_JOB2:
        {
            spellAPI.figTryClrCHkSPlBfrJob2(_this, spell_id);
            break;
        }
        case UNFREEZE:
        {
            spellAPI.figTryUnfreeze(_this, spell_id, 0);
            break;
        }
    }
}

bool __thiscall hasAuraActive(SF_CGdFigureToolbox *_this, uint16_t figure_id)
{
    return has_spell_effect(_this, figure_id, 0x49);
}

uint16_t __thiscall getPhysDamageReduction(SF_CGdFigureToolbox *_this, uint16_t source_index, uint16_t target_index,
                                           uint16_t action_id)
{
    return g_get_damage_reduction(_this->autoclass34, source_index, target_index, action_id);
}

bool __thiscall XDataExists(SF_CGdEffect *_this, uint16_t effect_index, SpellDataKey data)
{
    uint16_t xdata_key = _this->effects[effect_index].xdata_key;
    if (xdata_key != 0)
    {
        uint32_t result = CGdXDataExists(_this->CGdXDataList, xdata_key, data);
        if (result != 0)
        {
            return 1;
        }
    }
    return 0;
}

uint16_t __thiscall sf_get_spell_id(SF_CGdSpell *_this, uint16_t spell_index)
{
    return _this->active_spell_list[spell_index].spell_id;
}


void __thiscall tryClearCheckSpellsBeforeJob(SF_CGdSpell *_this, uint16_t spell_index, uint16_t figure_index)
{
    uint16_t spell_node = figureAPI.getSpellJobStartNode(_this->CGdFigure, figure_index);
    while (spell_node != 0)
    {
        uint16_t spell_id = toolboxAPI.getSpellIndexFromDLL(_this->CGdDoubleLinkedList, spell_node);
        if ((spell_index != spell_id) && ((_this->active_spell_list[spell_id].flags & 0x2) != 0))
        {
            return;
        }
        spell_node = toolboxAPI.getNextNode(_this->CGdDoubleLinkedList, spell_node);
    }
    _this->CGdFigure->figures[figure_index].flags &= ~(F_CHECK_SPELLS_BEFORE_JOB);

}

void __thiscall sf_end_spell_now(SF_CGdSpell *_this, uint16_t spell_index, bool try_unfreeze)
{
    if (try_unfreeze)
    {
        spellAPI.figTryUnfreeze(_this, spell_index, 0);
    }
    spellAPI.figTryClrCHkSPlBfrJob2(_this, spell_index);
    spellAPI.figClrChkSplBfrChkBattle(_this, spell_index, 0);
    spellAPI.removeDLLNode(_this, spell_index);
    spellAPI.setEffectDone(_this, spell_index, 0);
}

void __thiscall addSpellVisualEffect(SF_CGdSpell *_this, uint16_t spell_index, CGdEffectType effect_id,
                                     const SF_CGdTargetData *target, uint16_t tick_count,
                                     const SF_Rectangle *corner_coords)
{
    uint32_t unused_out;
    SF_CGdTargetData target_copy = *target;
    SF_Rectangle rect_arg = (corner_coords != NULL) ? *corner_coords : SF_Rectangle{0, 0};
    spellAPI.addVisualEffect(_this, spell_index, effect_id, &unused_out, &target_copy,
                             _this->OpaqueClass->current_step, tick_count, &rect_arg);
}

void __thiscall addSpellHitEffect(SF_CGdSpell *_this, uint16_t spell_index,
                                  uint16_t target_index, uint16_t tick_count)
{
    SF_CGdTargetData target;
    target.entity_type = 1;
    target.entity_index = target_index;
    target.position = {0, 0};
    addSpellVisualEffect(_this, spell_index, kGdEffectSpellHitTarget, &target, tick_count, NULL);
}

void __thiscall addSpellResistedEffect(SF_CGdSpell *_this, uint16_t spell_index,
                                       uint16_t target_index, uint16_t tick_count)
{
    SF_CGdTargetData target;
    target.entity_type = 1;
    target.entity_index = target_index;
    target.position = {0, 0};
    addSpellVisualEffect(_this, spell_index, kGdEffectSpellTargetResisted, &target, tick_count, NULL);
}

void __thiscall addSpellHitWorldEffect(SF_CGdSpell *_this, uint16_t spell_index, SF_Coord position,
                                       uint16_t tick_count, const SF_Rectangle *area)
{
    SF_CGdTargetData target;
    target.entity_type = 4;
    target.entity_index = 0;
    target.position = position;
    addSpellVisualEffect(_this, spell_index, kGdEffectSpellHitWorld, &target, tick_count, area);
}

void __thiscall buildTargetRectangle(SF_Coord pos_a, SF_Coord pos_b, SF_Rectangle *out_rect)
{
    uint16_t maxY = (pos_a.Y <= pos_b.Y) ? pos_b.Y : pos_a.Y;

    uint16_t maxX = (pos_a.X <= pos_b.X) ? pos_b.X : pos_a.X;

    uint16_t minY = (pos_a.Y > pos_b.Y) ? pos_b.Y : pos_a.Y;

    uint16_t minX = (pos_a.X > pos_b.X) ? pos_b.X : pos_a.X;
    SF_Coord p1;
    SF_Coord p2;
    p1.X = maxX;
    if (minX < maxX)
    {
        p1.X = minX;
    }
    p1.Y = maxY;
    if (minY < maxY)
    {
        p1.Y = minY;
    }

    p2.X = maxX;
    if (maxX <= minX)
    {
        p2.X = minX;
    }
    p2.Y = maxY;
    if (maxY <= minY)
    {
        p2.Y = minY;
    }
    out_rect->partA = p1.Y << 0x10 | p1.X;
    out_rect->partB = p2.Y << 0x10 | p2.X;
}

uint16_t __thiscall addSpellToFigure(SF_CGdSpell *_this, uint16_t spell_id,
                                     uint16_t source_index, uint16_t target_index)
{
    uint16_t start_tick = (uint16_t)_this->OpaqueClass->current_step;
    SF_CGdTargetData source;
    source.entity_type = 1;
    source.entity_index = source_index;
    source.position = {0, 0};
    SF_CGdTargetData target;
    target.entity_type = 1;
    target.entity_index = target_index;
    target.position = {0, 0};
    return spellAPI.addSpell(_this, spell_id, start_tick, &source, &target, 0);
}

static uint32_t pending_heal_of_figure(SF_CGdFigure *figures, SF_CGdResource *resource,
                                       uint16_t figure_index, uint16_t target_index)
{
    GdFigure *figure = &figures->figures[figure_index];
    if ((figure->flags & (IS_DEAD | RESESRVED_ONLY)) != 0)
    {
        return 0;
    }
    FigureJobData *job = &figure->current_job;
    if ((job->GdJobId != kGdJobWalkToTarget) && (job->GdJobId != kGdJobManualWalkToTarget) &&
        (job->GdJobId != kGdJobPreCast))
    {
        return 0;
    }
    /* Walk-phase target gate. SetFigureWalkToTargetJob
     * (0x6f4916, line 43) copies the AI cast target verbatim into
     * current_job.target (CGdFigure::SetTargetData 0x6b70b0 ->
     * JobData_maybe::SetTargetData 0x6c55a0). For ground-cast spells
     * (cast_type1 == 2: area/sentinel healing, spell_cast_maybe 0x75c5c1)
     * that target is a world point - entity_type = spell
     * cast_type2 in {4,5}, entity_index 0 - not the patient figure, so a
     * positional walker is matched by its healing job action alone; a
     * figure target that is not this patient and non-walkable kinds
     * (0 uninitialized, 2 building, 3 object) never count as walk
     * matches. */
    if (job->target.entity_type == 1)
    {
        if (job->target.entity_index != target_index)
        {
            return 0;
        }
    }
    else if ((job->target.entity_type != 4) && (job->target.entity_type != 5))
    {
        return 0;
    }
    if ((job->action.type == 0) ||
        (!spellAPI.hasSpellTag(job->action.type, SpellTag::HEALING_SPELL)))
    {
        return 0;
    }
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(resource, &spell_data, job->action.subtype);
    return spell_data.params[0];
}

uint32_t __thiscall sumIncomingHealFromList(SF_CGdFigure *figures, SF_CGdResource *resource,
                                            uint16_t target_index)
{
    uint_list_node some_list;
    uint32_t incoming_sum = 0;
    figureAPI.getIncomingSpellList(figures, &some_list, target_index);
    uint32_t *pair = some_list.first;
    while (pair != some_list.data)
    {
        uint16_t spell_id = (uint16_t)((*pair) >> 16);
        SF_CGdResourceSpell pair_data;
        spellAPI.getResourceSpellData(resource, &pair_data, spell_id);
        if (spellAPI.hasSpellTag(pair_data.spell_line_id, SpellTag::HEALING_SPELL))
        {
            incoming_sum += pair_data.params[0];
        }
        pair++;
    }
    figureAPI.releaseIncomingSpellList(&some_list);
    return incoming_sum;
}

uint32_t __thiscall sumIncomingPendingHeals(SF_CGdFigure *figures, SF_CGdResource *resource,
                                            const EntityPriorityList *allies,
                                            uint16_t target_index)
{
    uint32_t sum = sumIncomingHealFromList(figures, resource, target_index);
    for (uint16_t i = 0; i < allies->entityCount; i++)
    {
        sum += pending_heal_of_figure(figures, resource, allies->data[i].entity_index, target_index);
    }
    return sum;
}

uint32_t __thiscall sumIncomingPendingHealsAllFigures(SF_CGdFigure *figures, SF_CGdResource *resource,
                                                      SF_CGdFigureToolbox *toolbox, uint16_t target_index)
{
    uint32_t sum = 0;
    for (uint16_t f = 1; f <= figures->max_used; f++)
    {
        if ((figures->figures[f].owner == (uint16_t)-1) ||
            ((figures->figures[f].flags & (IS_DEAD | RESESRVED_ONLY)) != 0))
        {
            continue;
        }
        if (toolboxAPI.figuresCheckHostile(toolbox, f, target_index))
        {
            continue;
        }
        sum += pending_heal_of_figure(figures, resource, f, target_index);
    }
    return sum;
}

