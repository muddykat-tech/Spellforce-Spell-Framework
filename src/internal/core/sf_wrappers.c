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

typedef uint32_t (__thiscall *XDataGet_ptr)(void *_this,uint16_t key,uint8_t keyType);
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

void log_warning(const char *format, ...)
{
    char buffer[LOG_BUFFER_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, LOG_BUFFER_SIZE, format, args);
    va_end(args);

    char modifiedMessage[LOG_BUFFER_SIZE];
    snprintf(modifiedMessage, LOG_BUFFER_SIZE, "[WARNING] %s", buffer);
    console_log(modifiedMessage);
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

const char * debug_level_to_string(DebugLevel level)
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
    iteratorAPI.figureIteratorSetPointers(iterator, spell->SF_CGdFigure,
                                          spell->unkn3, spell->SF_CGdWorld);
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

    uint16_t node_id = figureAPI.getSpellJobStartNode(_this->SF_CGdFigure,
                                                      source_index);
    while (node_id != 0)
    {
        uint16_t current_index =
            toolboxAPI.getSpellIndexFromDLL(
                (uint32_t *)_this->SF_CGdDoubleLinkedList, node_id);
        if ((*condition)(_this, current_index, spell_index))
        {
            (*callback)(_this, source_index, current_index);
            break;
        }
        node_id =
            toolboxAPI.getNextNode((uint32_t *)_this->SF_CGdDoubleLinkedList,
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

        default:
            log_warning("INVALID STATISTIC KEY: %d", key);
            return;
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

bool __thiscall XDataExists(SF_CGDEffect *_this, uint16_t effect_index, SpellDataKey data)
{
    uint16_t xdata_key = _this->active_effect_list[effect_index].xdata_key;
    uint32_t result = CGdXDataExists(_this->SF_CGdXDataList, xdata_key, data);
    if ((result != 0) && (xdata_key != 0))
    {
        return 1;
    }
    return 0;
}

uint16_t __thiscall sf_get_spell_id(SF_CGdSpell *_this, uint16_t spell_index)
{
    return _this->active_spell_list[spell_index].spell_id;
}


void __thiscall tryClearCheckSpellsBeforeJob(SF_CGdSpell *_this, uint16_t spell_index, uint16_t figure_index)
{
    uint16_t spell_node = figureAPI.getSpellJobStartNode(_this->SF_CGdFigure, figure_index);
    while (spell_node != 0)
    {
        uint16_t spell_id = toolboxAPI.getSpellIndexFromDLL(_this->SF_CGdDoubleLinkedList, spell_node);
        if ((spell_index != spell_id) && ((_this->active_spell_list[spell_id].flags & 0x2) != 0))
        {
            return;
        }
        spell_node = toolboxAPI.getNextNode(_this->SF_CGdDoubleLinkedList, spell_node);
    }
    _this->SF_CGdFigure->figures[figure_index].flags &= ~(F_CHECK_SPELLS_BEFORE_JOB);

}
