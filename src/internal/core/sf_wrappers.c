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

FUN_0069eaf0_ptr FUN_0069eaf0;
fidfree_ptr fidFree;
SF_String_ctor_ptr g_create_sf_string;
SF_String_dtor_ptr g_destroy_sf_string;
has_spell_effect_ptr has_spell_effect;
create_button_ptr create_button_func;
attach_string_ptr vfunction_apply_string;
vfunction_ptr vfunction176;
vfunction_ptr vfunction25;
vfunction12_ptr vfunction12;
initialize_smp_button_ptr initialize_smp_button;
set_btn_name_ptr set_button_name;
initialize_menu_container_ptr initialize_menu_container;
set_label_color_ptr set_label_color;
set_base_alpha_ptr set_base_alpha;
get_visual_control_ptr get_visual_control;
setup_menu_container_data_ptr setup_menu_container_data;
vfunction_2_ptr set_font;
set_btn_index_ptr set_button_index;
set_button_flag_ptr set_menu_button_flag;
vfunction2_callback_attach_ptr attach_callback;
vfunction_ptr vfunction16_attach_callback;
CMnuBase_setname_ptr CMnuBase_setname;
get_phys_damage_reduction_ptr g_get_damage_reduction;
destory_container_ptr f_destory_container;
destory_button_ptr f_destory_button;
set_menu_id_ptr set_menu_id;
set_container_visible_ptr set_container_visible;
XDataGet_ptr XDataGet;

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
    set_label_color = (set_label_color_ptr)(ASI::AddrOf(0x530330));
    set_base_alpha = (set_base_alpha_ptr)(ASI::AddrOf(0x512EB0));
    get_visual_control = (get_visual_control_ptr)(ASI::AddrOf(0x507BC0));//
    set_button_name = (set_btn_name_ptr) (ASI::AddrOf(0x52f8a0));
    initialize_menu_container =
        (initialize_menu_container_ptr)(ASI::AddrOf(0x505780));
    setup_menu_container_data =
        (setup_menu_container_data_ptr)(ASI::AddrOf(0x50FD30));
    initialize_smp_button = (initialize_smp_button_ptr) (ASI::AddrOf(0x51a9d0));
    create_button_func = (create_button_ptr) (ASI::AddrOf(0x52E1E0));
    set_font = (vfunction_2_ptr)(ASI::AddrOf(0x530C20));
    set_button_index = (set_btn_index_ptr)(ASI::AddrOf(0x5136a0));
    set_menu_button_flag = (set_button_flag_ptr)(ASI::AddrOf(0x5308A0));
    attach_callback = (vfunction2_callback_attach_ptr)(ASI::AddrOf(0x6188B0));
    vfunction16_attach_callback = (vfunction_ptr) (ASI::AddrOf(0x532B90));
    vfunction176 = (vfunction_ptr)(ASI::AddrOf(0x52f520));
    vfunction25 = (vfunction_ptr)(ASI::AddrOf(0x511ae0));
    CMnuBase_setname = (CMnuBase_setname_ptr)(ASI::AddrOf(0x512E30));
    g_get_damage_reduction = (get_phys_damage_reduction_ptr)(ASI::AddrOf(0x317070));
    f_destory_container = (destory_container_ptr)(ASI::AddrOf(0x512AB0));
    f_destory_button = (destory_button_ptr)(ASI::AddrOf(0xB7B270));
    set_menu_id = (set_menu_id_ptr)(ASI::AddrOf(0x50E660));
    set_container_visible = (set_container_visible_ptr)(ASI::AddrOf(0x513910));
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
    if ((_this->type == 10000) || (_this->type == 0x2711))
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
    return buildingAPI.getBuildingTags(building_type) & tag;
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

CMnuLabel * __thiscall attach_new_label(CMnuLabel *label_ptr,
                                        CMnuContainer *parent,
                                        char *label_chars, uint8_t font_index,
                                        uint16_t x_pos, uint16_t y_pos,
                                        uint16_t width, uint16_t height)
{
    char empty[] = "";
    return attach_new_meshed_label(label_ptr, parent, empty, label_chars,
                                   font_index, x_pos, y_pos, width, height);
}

void wrap_text(const char *input, char *output, size_t max_width)
{
    size_t input_len = strlen(input);
    size_t current_line_width = 0;
    size_t output_index = 0;

    for (size_t i = 0; i < input_len; ++i)
    {
        output[output_index++] = input[i];
        current_line_width++;

        if (current_line_width >= max_width)
        {
            size_t break_point = output_index;
            while (break_point > 0 && !isspace(output[break_point - 1]))
            {
                break_point--;
            }

            if (break_point > 0)
            {
                output[break_point - 1] = '\n';
                current_line_width = output_index - break_point;
            }
            else
            {
                output[output_index++] = '\n';
                current_line_width = 0;
            }
        }
    }

    output[output_index] = '\0';
}

static bool is_init_finished = false;

/**
 * Helper function to update label text using SF_String
 *
 * @param label The label to update
 * @param text New text to set
 */
static void update_label_text(CMnuLabel *label, const char *text)
{
    if (!label || !text)
    {
        return;
    }

    SF_String string_obj;
    SF_String *sf_string = g_create_sf_string(&string_obj, text);
    g_menu_label_set_string(label, sf_string);
    g_destroy_sf_string(sf_string);
}


SFSF_ModlistStruct modinformation;
CMnuContainer *mod_list;
CMnuContainer *mod_container;
CMnuContainer *mod_info_page;

CMnuLabel* mod_list_title;
CMnuSmpButton* left_nav;
CMnuSmpButton* right_nav;
static bool is_mod_list_shown = false;
static bool does_mod_list_exist = false;


int calculate_total_unique_mods()
{
    int total_unique_mods = 0;
    SFMod *last_parent = nullptr;

    for (const SFSpell *spell_data : g_internal_spell_list)
    {
        if (spell_data->parent_mod != last_parent)
        {
            total_unique_mods++;
            last_parent = spell_data->parent_mod;
        }
    }

    return total_unique_mods;
}

int calculate_total_pages(int total_unique_mods, int mods_per_page)
{
    int total_pages = (total_unique_mods + mods_per_page - 1) / mods_per_page;
    return (total_pages == 0) ? 1 : total_pages;
}

int normalize_page_index(int page, int total_pages)
{
    return (page + total_pages) % total_pages;
}

void prepare_mod_title(SFMod *parent_mod, char *mod_title, size_t buffer_size)
{
    snprintf(mod_title, buffer_size, "%s %s\nby %s",
             parent_mod->mod_id,
             parent_mod->mod_version,
             parent_mod->mod_author);
}

void prepare_mod_description(SFMod *parent_mod, char *mod_description, size_t description_buffer_size,
                             char *wrapped_description, size_t wrapped_buffer_size)
{
    snprintf(mod_description, description_buffer_size, "%s",
             parent_mod->mod_description);

    wrap_text(mod_description, wrapped_description, 80);
}

void prepare_mod_page_info(int page, int total_pages, char *mod_page_info, size_t buffer_size)
{
    snprintf(mod_page_info, buffer_size, "(%d / %d)",
             (page + 1), total_pages);
}

void prepare_mod_error_info(SFMod *parent_mod, char *mod_error_info, size_t error_buffer_size,
                            char *wrapped_error_info, size_t wrapped_buffer_size)
{
    if (parent_mod->mod_errors && (parent_mod->mod_errors[0] != '\0'))
    {
        snprintf(mod_error_info, error_buffer_size, "%s",
                 parent_mod->mod_errors);
    }
    else
    {
        snprintf(mod_error_info, error_buffer_size, "");
    }

    wrap_text(mod_error_info, wrapped_error_info, 80);
}

/**
 * Attaches mod labels to the container based on pagination settings
 *
 * @param container The menu container to attach labels to
 * @param mods_per_page Number of mods to display per page
 * @param page Current page number (0-based)
 */
void attach_mod_labels(CMnuContainer *container, int mods_per_page, int page)
{
    if (g_internal_spell_list.empty())
    {
        return;
    }

    int total_unique_mods = calculate_total_unique_mods();

    int total_pages = calculate_total_pages(total_unique_mods, mods_per_page);

    page = normalize_page_index(page, total_pages);
    mod_struct.index = page;

    const int y_base_offset = 100;
    const int y_item_spacing = 150;
    const int x_title_pos = 468;
    const int x_page_number_pos = 468;
    const int x_desc_pos = 200;
    const int title_width = 600;
    const int desc_width = 800;

    const int start_index = mods_per_page * page;
    const int end_index = start_index + mods_per_page;

    SFMod *current_parent = nullptr;
    int mod_index = 0;
    int displayed_mods = 0;

    for (const SFSpell *spell_data : g_internal_spell_list)
    {
        SFMod *parent_mod = spell_data->parent_mod;

        // Only process each unique mod once
        if (parent_mod != current_parent)
        {
            if (mod_index >= start_index && mod_index < end_index)
            {
                // Prepare display strings
                char mod_title[512] = {0};
                prepare_mod_title(parent_mod, mod_title, sizeof(mod_title));

                char mod_description[512] = {0};
                char wrapped_description[1024] = {0};
                prepare_mod_description(parent_mod, mod_description, sizeof(mod_description),
                                        wrapped_description, sizeof(wrapped_description));

                char mod_page_info[48] = {0};
                prepare_mod_page_info(mod_index, total_unique_mods, mod_page_info, sizeof(mod_page_info));

                char mod_error_info[512] = {0};
                char wrapped_error_info[1024] = {0};
                prepare_mod_error_info(parent_mod, mod_error_info, sizeof(mod_error_info),
                                       wrapped_error_info, sizeof(wrapped_error_info));

                // Calculate Y position
                const int relative_y_pos = displayed_mods * y_item_spacing;
                const int absolute_y_pos = y_base_offset + relative_y_pos;

                // Attach or update labels
                if (is_init_finished)
                {
                    update_label_text(mod_struct.title_label, mod_title);
                    update_label_text(mod_struct.desc_label, wrapped_description);
                    update_label_text(mod_struct.page_label, mod_page_info);
                    update_label_text(mod_struct.error_label, wrapped_error_info);
                }
                else
                {
                    mod_struct.title_label =
                        attach_new_label(nullptr, container, mod_title,
                                         6, x_title_pos, absolute_y_pos - 48, title_width,
                                         y_item_spacing);
                    set_menu_id(mod_struct.title_label, 0x6);
                    set_label_color(mod_struct.title_label, 0.85, 0.64, 0.12, '\0');
                    set_label_color(mod_struct.title_label, 0.85, 0.64, 0.12, '\x01');

                    mod_struct.desc_label =
                        attach_new_label(nullptr, container, wrapped_description,
                                         11, 48, absolute_y_pos + 24, desc_width, y_item_spacing);
                    set_menu_id(mod_struct.desc_label, 0x6);

                    mod_struct.page_label =
                        attach_new_label(nullptr, container, mod_page_info,
                                         6, 92, absolute_y_pos + 382, 50, y_item_spacing);
                    set_menu_id(mod_struct.page_label, 0x6);

                    mod_struct.error_label =
                        attach_new_label(nullptr, container, wrapped_error_info,
                                         11, 48, absolute_y_pos + 224, desc_width, y_item_spacing);
                    set_menu_id(mod_struct.error_label, 0x6);
                    set_label_color(mod_struct.error_label, 1.0, 0.0, 0.0, '\x01');

                    is_init_finished = true;
                }

                displayed_mods++;
            }

            current_parent = parent_mod;
            mod_index++;
        }

        // Stop once we have filled the page
        if (displayed_mods >= mods_per_page)
            break;
    }
}

static void navigate_page(CMnuSmpButton *button, int delta)
{
    CMnuContainer *parent =
        (CMnuContainer *)button->CMnuBase_data.param_2_callback;

    int total = calculate_total_pages(calculate_total_unique_mods(), 1);
    mod_struct.index = normalize_page_index(mod_struct.index + delta, total);

    attach_mod_labels(parent, 1, mod_struct.index);
}

void add_navigation_buttons(CMnuContainer *parent)
{
    char btn_disabled[128] = "ui_btn_togglearrow_right_disabled.msh";
    char btn_pressed[128]  = "ui_btn_togglearrow_right_pressed.msh";
    char btn_load[1] = "";
    char btn_default[128]  = "ui_btn_togglearrow_right_default.msh";
    char btn_label[1] = "";

    right_nav = attach_new_button(parent, btn_default, btn_pressed, btn_load,
                      btn_disabled, btn_label, 7, (886 - 96), 619 - 80,
                      48, 48, 0, (uint32_t) &navigate_page, 1);

    char btn_disabled_left[128] = "ui_btn_togglearrow_left_disabled.msh";
    char btn_pressed_left[128] = "ui_btn_togglearrow_left_pressed.msh";
    char btn_default_left[128] = "ui_btn_togglearrow_left_default.msh";

    left_nav = attach_new_button(parent, btn_default_left, btn_pressed_left,
                      btn_load, btn_disabled_left, btn_label, 7, 48, 619 - 80,
                      48, 48, 1, (uint32_t) &navigate_page, -1);

    attach_mod_labels(parent, 1, 0);
}

void __fastcall close_mod_list_callback(CMnuSmpButton *button, int32_t *cui_menu_ptr_maybe)
{
    CMnuContainer *mod_list =
        (CMnuContainer *) button->CMnuBase_data.param_2_callback;
    set_container_visible(mod_list, false, false);
    is_mod_list_shown = false;
}

CMnuContainer *create_menu_container(
    uint16_t x,
    uint16_t y,
    uint16_t width,
    uint16_t height,
    const char *background_msb,
    const char *border_msb,
    float alpha
)
{
    CMnuContainer *container;
    SF_String s_bg, s_border;
    SF_String *p_bg, *p_border;

    container = (CMnuContainer *)g_new_operator(0x340);
    if (!container) {
        log_error("Failed to allocate CMnuContainer");
        return NULL;
    }

    initialize_menu_container(container);

    p_bg = g_create_sf_string(&s_bg, (char *)background_msb);
    p_border = g_create_sf_string(&s_border, (char *)border_msb);

    setup_menu_container_data(
        container,
        x,
        y,
        width,
        height,
        p_bg,
        p_border
    );

    g_destroy_sf_string(p_bg);
    g_destroy_sf_string(p_border);
    void* CMnuVisControl = (void*) get_visual_control(container);
    set_base_alpha(CMnuVisControl, alpha);

    return container;
}

void add_close_button(CMnuContainer *mod_list)
{
    char close_btn_default[128] = "ui_btn_nav_back_default.msh";
    char close_btn_pressed[128] = "ui_btn_nav_back_pressed.msh";
    char close_btn_disabled[128] = "ui_btn_nav_back_disabled.msh";
    char close_btn_load[1] = "";
    char close_btn_label[1] = "";

    CMnuSmpButton *close_btn = attach_new_button(
        mod_list,
        close_btn_default,
        close_btn_pressed,
        close_btn_load,
        close_btn_disabled,
        close_btn_label,
        7,
        28,
        700,
        48,
        48,
        2,
        (uint32_t) &close_mod_list_callback, 0
    );
}

void __thiscall show_mod_list(CMnuSmpButton *button)
{
    CMnuContainer *parent = (CMnuContainer *) button->CMnuBase_data.param_2_callback;

    if(!does_mod_list_exist)
    {
        is_mod_list_shown = true;
        mod_info_page = create_menu_container(
            0, 0, 1024, 768,
            "ui_bgr_landscape_bg.msb",
            "", 0.99f
        );

        mod_container = create_menu_container(
            11,6,1008,757,
            "ui_bgr_pregame_border_transparency.msb",
            "ui_bgr_pregame_border.msb", 0.5f
        );

        mod_list = create_menu_container(
            59, 50, 886, 619,
            "ui_bgr_options_border_transparency.msb",
            "ui_bgr_options_border.msb", 0.5f
        );

        g_container_add_control(parent, (CMnuBase *)mod_info_page, '\x01', '\x01', 0);
        g_container_add_control(mod_info_page, (CMnuBase *)mod_container, '\x01', '\x01', 0);
        g_container_add_control(mod_container, (CMnuBase *)mod_list, '\x01', '\x01', 0);

        add_close_button(mod_info_page);
        add_navigation_buttons(mod_list);

        char sfsf_mod_info[32] = "Mod Information";
        mod_list_title = attach_new_label(nullptr, mod_container, sfsf_mod_info, 6, 468, 16, 128, 16);
        set_menu_id(mod_list_title, 0x6);
        set_label_color(mod_list_title, 0.85, 0.64, 0.12, '\0');
        set_label_color(mod_list_title, 0.85, 0.64, 0.12, '\x01');

        if(!mod_list)
        {
            log_error("Unable to create Mod Menu, Mod List Container is NULL");
            return;
        }
        does_mod_list_exist = true;
    }
    else
    {
        is_mod_list_shown = !is_mod_list_shown;
        set_container_visible(mod_info_page, is_mod_list_shown, 0);
    }
}

void __fastcall show_mod_list_callback(CMnuSmpButton *button)
{
    CMnuContainer *parent =
        (CMnuContainer *) button->CMnuBase_data.param_2_callback;
    uint8_t toggle = mod_struct.toggle;

    show_mod_list(button);
}

CMnuSmpButton* __thiscall attach_new_button(CMnuContainer *parent,
                                  char *button_mesh_default,
                                  char *button_mesh_pressed,
                                  char *button_initial_load_mesh,
                                  char *button_mesh_disabled, char *label_char,
                                  uint8_t font_index, uint16_t x_pos,
                                  uint16_t y_pos, uint16_t width,
                                  uint16_t height, int button_index,
                                  uint32_t callback_func_ptr, int callback_param1)
{
    SF_String m_mesh_string_default;
    SF_String m_mesh_string_pressed;
    SF_String m_button_initial_load_mesh;
    SF_String m_mesh_string_disabled;

    SF_String m_label_string;
    CMnuSmpButton *new_button;
    void *new_btn_operation;

    SF_FontStruct *fonts = g_get_smth_fonts();
    SF_String *label_string = g_create_sf_string(&m_label_string, label_char);

    // Default
    SF_String *mesh_string_default = g_create_sf_string(&m_mesh_string_default,
                                                        button_mesh_default);

    // Pressed
    SF_String *mesh_string_pressed = g_create_sf_string(&m_mesh_string_pressed,
                                                        button_mesh_pressed);

    // Highlight
    SF_String *init_load_mesh = g_create_sf_string(&m_button_initial_load_mesh,
                                                   button_initial_load_mesh);

    // Disabled
    SF_String *mesh_string_disabled =
        g_create_sf_string(&m_mesh_string_disabled, button_mesh_disabled);

    // 0x3b0 seems to corralate to CUiStartMenu, but is directly cast to be a type of CUiFrameStats
    new_button = (CMnuSmpButton *)g_new_operator(0x428); // 0x368, 0x3b0 and 0x708 are all valid. (I suspect that they're creating objects that have CMnuLabel as an Parent Class).

    if (font_index > 32)
    {
        log_error("Invalid font index 0~32, defaulting to font 6");
        font_index = 6;
    }

    new_button = initialize_smp_button(new_button);
    SF_Font *selected_font = g_get_font(fonts, font_index);

    CMnuBase_setname((CMnuBase *)new_button, label_string);

    // This seems to fill out the actual button data itself.
    create_button_func(new_button,x_pos,y_pos,width,height,mesh_string_default,
                       init_load_mesh,mesh_string_pressed,mesh_string_disabled);

    set_font(new_button, selected_font);

    set_button_index(new_button, button_index);

    set_menu_button_flag(new_button, '\x01');

    set_button_name(new_button, label_string);

    CUtlCallback2 callback;
    callback.vtable_ptr = *(uint32_t *)(ASI::AddrOf(0x7F9C64));
    callback.param_ptr = (uint32_t) parent;
    callback.callback_func = callback_func_ptr;

    uint32_t param2, param3;

    attach_callback(&callback, &callback_param1, &param2, &param3);

    new_button->CMnuBase_data.param_1_callback = callback_param1;
    new_button->CMnuBase_data.param_2_callback = param2;
    new_button->CMnuBase_data.param_3_callback = param3;

    vfunction16_attach_callback(new_button, '\x01');

    g_container_add_control(parent,  (CMnuBase *)new_button, '\x01', '\x01', 0);

    g_destroy_sf_string(mesh_string_default);
    g_destroy_sf_string(mesh_string_pressed);
    g_destroy_sf_string(init_load_mesh);
    g_destroy_sf_string(mesh_string_disabled);
    g_destroy_sf_string(label_string);

    return new_button;
}

CMnuLabel * __thiscall attach_new_meshed_label(CMnuLabel *new_label,
                                               CMnuContainer *parent,
                                               char *mesh_char,
                                               char *label_char,
                                               uint8_t font_index,
                                               uint16_t x_pos, uint16_t y_pos,
                                               uint16_t width, uint16_t height)
{
    SF_String m_mesh_string;
    SF_String m_label_string;

    SF_FontStruct *fonts = g_get_smth_fonts();
    SF_String *label_string = g_create_sf_string(&m_label_string, label_char);
    SF_String *mesh_string = g_create_sf_string(&m_mesh_string, mesh_char);

    new_label = (CMnuLabel *)g_new_operator(0x3b0);

    if (font_index > 32)
    {
        log_error("Invalid font index 0~32, defaulting to font 6");
        font_index = 6;
    }

    SF_Font *selected_font = g_get_font(fonts, font_index);

    g_menu_label_constructor(new_label);

    g_set_label_flags(new_label, 7);

    g_init_menu_element(new_label, x_pos, y_pos, width, height, mesh_string);

    g_menu_label_set_font(new_label, selected_font);

    g_container_add_control(parent, (CMnuBase *) new_label, '\x01', '\x01', 0);

    g_menu_label_set_string(new_label, label_string);

    g_destroy_sf_string(label_string);
    g_destroy_sf_string(mesh_string);

    return new_label;
}

uint16_t __thiscall sf_get_spell_id(SF_CGdSpell *_this, uint16_t spell_index)
{
    return _this->active_spell_list[spell_index].spell_id;
}

SFMod *createModInfo(const char *mod_id, char *mod_version,
                     const char *mod_author, const char *mod_description)
{
    SFMod *mod = (SFMod *)malloc(sizeof(SFMod));

    strncpy(mod->mod_id, mod_id, 63);
    mod->mod_id[63] = '\0';

    strncpy(mod->mod_version, mod_version, 23);
    mod->mod_version[23] = '\0';

    strncpy(mod->mod_description, mod_description, 127);
    mod->mod_description[127] = '\0';

    strncpy(mod->mod_author, mod_author, 127);
    mod->mod_author[127] = '\0';

    return mod;
}

// WARNING: Function not fully implemented.
// This function successfully loads a video, but the video container is attached to a layer above the default container.
// As a result, the video container intercepts click events and prevents proper interaction with the underlying elements.
void attachVideo(CAppMenu *CAppMenu_ptr, CMnuContainer *parent,
                 char *video_loc_and_name_chars)
{
    SF_CUiVideo *video_ptr = (SF_CUiVideo *) g_new_operator(0x348);
    SF_String m_video_name_string;
    SF_String *video_name_string = g_create_sf_string(&m_video_name_string,
                                                      video_loc_and_name_chars);
    // TODO Cleanup: Not Sequence, just normal Video, haven't renamed it from previous investigations.
    video_ptr =(SF_CUiVideo *) cuiVideoSequence_constructor(video_ptr,
                                                            video_name_string);

    char controller_mark_chars[64];
    sprintf(controller_mark_chars, "<Cont>CreditsVideoController");
    SF_String m_controller_mark_string;
    SF_String *controller_mark_string =
        g_create_sf_string(&m_controller_mark_string, controller_mark_chars);
    CMnuBase_setname((CMnuBase *)video_ptr, controller_mark_string);

    //void *_CMnuScreen_ptr, CMnuBase* base, char flag
    //CMnuScreen_attach_control(parent, CAppMenu_ptr->CAppMenu_data.CMnuBase_ptr, '\x01');
}
