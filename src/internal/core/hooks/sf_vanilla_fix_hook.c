#include "../sf_wrappers.h"
#include "../../../asi/sf_asi.h"
#include "sf_vanilla_fix_hook.h"
#define _USE_MATH_DEFINES
#include <math.h>

#include "../../registry/spell_data_registries/sf_enchant_registry.h"

extern SpellFunctions spellAPI;
extern ToolboxFunctions toolboxAPI;
extern BuildingFunctions buildingAPI;
extern FigureFunctions figureAPI;
extern IteratorFunctions iteratorAPI;
extern RegistrationFunctions registrationAPI;
extern EffectFunctions effectAPI;
extern AiFunctions aiAPI;

typedef void (__thiscall *add_item_ptr)(ushort_list_node *_this,uint16_t *item);
add_item_ptr add_item;


typedef SF_String *(__thiscall *AC95_get_figure_name_ptr)(void *AC95, SF_String *name_buffer, uint32_t figure_id);
typedef void (__cdecl *GetColoredText_ptr)(SF_String *param_1, uint32_t param_2);
typedef void (__thiscall *GetDescriptionText_ptr)(void *_this,uint32_t param_1,SF_String *name_buffer);
typedef void (__thiscall *GetTextById_ptr)(void *_this,uint16_t text_id,SF_String *out_string);
typedef SF_String *(__thiscall *ExpandSpellDescription_ptr)(AutoClass101 *_this, SF_String *out_string,
                                                            SF_String *in_string, SF_CGdResourceSpell *param_3);


typedef uint32_t (__cdecl *SFprintf_ptr)(SF_String *_this, const wchar_t *format, ...);
typedef SF_String *(__thiscall *SFStringConcat_ptr)(SF_String *destination, SF_String *source);
typedef void (__thiscall *SFStringSetLength_ptr)(SF_String *_this, uint32_t length);
typedef SF_String *(__thiscall *SFStringFromWchar_ptr)(SF_String *_this, wchar_t value, uint32_t length);
typedef void (__thiscall *SFStringDestructor_ptr)(SF_String *_this);
typedef SF_String *(__thiscall *SFStringCopy_ptr)(SF_String *destination, SF_String *source);
typedef SF_String *(__thiscall *GetItemInfo_ptr)(SF_CGdResource *_this, CGdResourceWeaponData *stats,
                                                 uint16_t item_id);
typedef uint8_t (__thiscall *GetWeaponType_ptr)(SF_CGdResource *_this, uint16_t item_id);
typedef uint32_t (__thiscall *GetDamageMultiplierPerLevel_ptr)(SF_CGdResource *_this, uint8_t level);
typedef uint16_t *(__thiscall *GetWeaponEffects_ptr)(SF_CGdResource *_this, uint16_t *param_1, uint16_t item_id);
typedef uint32_t (__thiscall *AC34calculateWeaponSpeed_ptr) (void *_this,uint32_t base_fight_speed,uint8_t race,
                                                             uint16_t item_id, uint32_t dual_wield,uint32_t is_ranged);
typedef void *(__thiscall *CGdMainGetAC34_ptr)(void *_this);

typedef SF_String *(__cdecl *getSpecialDesc_ptr)(SF_String *param_1,uint32_t param_2);
typedef void (__stdcall *getWeaponRangeDescription_ptr)(SF_String *param_1, uint16_t param_2,uint16_t param_3,
                                                        SF_String *param_4, uint8_t param_5);

typedef uint32_t (__thiscall *GetResourceSpellLine_ptr)(SF_CGdResource *_this, uint16_t spell_line_id,
                                                        SF_CGdResourceSpellLine *value);

typedef uint16_t (__thiscall *getUnitStringID_ptr)(SF_CGdResource *_this,uint16_t unit_id);
typedef bool (__thiscall *getTextString_ptr)(SF_CGdResource *_this,uint16_t text_id,char *out_string);

AC95_get_figure_name_ptr AC95_get_figure_name;
GetColoredText_ptr GetColoredText;
GetDescriptionText_ptr GetDescriptionText;
SFprintf_ptr SFprintf;
SFStringConcat_ptr SFStringConcat;
SFStringSetLength_ptr SFStringSetLength;
SFStringFromWchar_ptr SFStringFromWchar;
SFStringDestructor_ptr SFStringDestructor;
SFStringCopy_ptr SFStringCopy;

getSpecialDesc_ptr getSpecialDesc;
GetItemInfo_ptr GetItemInfo;
GetWeaponType_ptr GetWeaponType;
GetDamageMultiplierPerLevel_ptr GetDamageMultiplierPerLevel;
AC34calculateWeaponSpeed_ptr AC34calculateWeaponSpeed;
getWeaponRangeDescription_ptr getWeaponRangeDescription;
CGdMainGetAC34_ptr CGdMainGetAC34;
GetWeaponEffects_ptr GetWeaponEffects;
GetResourceSpellLine_ptr GetResourceSpellLine;
GetTextById_ptr GetTextById;
ExpandSpellDescription_ptr ExpandSpellDescription;
getUnitStringID_ptr getUnitStringID;
getTextString_ptr getTextString;

bool __thiscall get_salvo_target_list (SF_CGdFigureJobs *_this, SF_CGdTargetData *source, SF_CGdTargetData *target,
                                       uint32_t angle, uint16_t *ranges, uint32_t target_count,
                                       ushort_list_node *output)
{
    CGdFigureIterator iter;
    uint16_t next_figure = 0;
    SF_Coord source_pos = _this->CGdFigure->figures[source->entity_index].position;
    SF_Coord target_pos = target->position;
    //   log_info("Salvo source_pos X: %d, Y: %d ", source_pos.X, source_pos.Y);
    //   log_info("Salvo target_pos X: %d, Y: %d ", target_pos.X, target_pos.Y);

    uint32_t distance = toolboxAPI.getDistance(&source_pos, &target_pos);

    uint32_t radius =  (uint32_t)round((distance << 10) * tan((angle*M_PI_2)/180)) >> 10;
    //   log_info("Salvo distance %d, radius %d, angle %d", distance, radius, angle);
    uint16_t min_range = ranges[0];
    uint16_t max_range = ranges[1];
    //   log_info("Salvo min range %d, max range %d", min_range, max_range);

    output->data = output->first;
    iteratorAPI.figureIteratorInit(&iter, 0, 0, 0x3ff, 0x3ff);
    iteratorAPI.figureIteratorSetPointers(&iter, _this->CGdFigure, _this->AutoClass22, _this->CGdWorld);
    iteratorAPI.iteratorSetArea(&iter, &target_pos, radius);
    next_figure = iteratorAPI.getNextFigure(&iter);
    while ((next_figure != 0) && (target_count != 0))
    {
        SF_Coord secondary_pos = _this->CGdFigure->figures[next_figure].position;
        distance = toolboxAPI.getDistance(&source_pos, &secondary_pos);
        if ((distance > min_range) && (distance < max_range))
        {
            if (toolboxAPI.figuresCheckHostile(_this->CGdFigureToolBox, source->entity_index, next_figure))
            {
                add_item(output, &next_figure);
                target_count--;
            }
        }
        next_figure = iteratorAPI.getNextFigure(&iter);
    }
    iteratorAPI.disposeFigureIterator(&iter);
    return (((uint32_t)output->data - (uint32_t)output->first) != 0);
}

uint16_t __thiscall get_figure_statistic_max_hp(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].health.base_val;
    int32_t bonus_val = _this->figures[figure_id].health.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 1;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].health.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    (result == 0) ? (result = 1) : (result);
    return result;
}

uint16_t __thiscall get_figure_statistic_max_mp(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].mana.base_val;
    int32_t bonus_val = _this->figures[figure_id].mana.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 1;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].mana.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    (result == 0) ? (result = 1) : (result);
    return result;
}

uint16_t __thiscall get_figure_statistic_max_sta(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].stamina.base_val;
    int32_t bonus_val = _this->figures[figure_id].stamina.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 1;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].stamina.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    (result == 0) ? (result = 1) : (result);
    return result;
}

uint16_t __thiscall get_figure_statistic_current_ac(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].armor.base_val;
    int32_t bonus_val = _this->figures[figure_id].armor.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 1;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].armor.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    (result == 0) ? (result = 1) : (result);
    return result;
}

uint16_t __thiscall get_figure_statistic_current_agi(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].agility.base_val;
    int32_t bonus_val = _this->figures[figure_id].agility.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 1;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].agility.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    (result == 0) ? (result = 1) : (result);
    return result;
}

uint16_t __thiscall get_figure_statistic_current_cha(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].charisma.base_val;
    int32_t bonus_val = _this->figures[figure_id].charisma.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 1;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].charisma.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    (result == 0) ? (result = 1) : (result);
    return result;
}

uint16_t __thiscall get_figure_statistic_current_dex(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].dexterity.base_val;
    int32_t bonus_val = _this->figures[figure_id].dexterity.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 1;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].dexterity.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    (result == 0) ? (result = 1) : (result);
    return result;
}

uint16_t __thiscall get_figure_statistic_current_hp(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].health.base_val;
    int32_t bonus_val = _this->figures[figure_id].health.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 0;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].health.bonus_multiplier + 100;
        uint16_t missing_val = _this->figures[figure_id].health.missing_val;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
        (result - missing_val < 0) ? (result = 0) : (result -= missing_val);
    }
    return result;
}

uint16_t __thiscall get_figure_statistic_current_int(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].intelligence.base_val;
    int32_t bonus_val = _this->figures[figure_id].intelligence.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 1;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].intelligence.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    (result == 0) ? (result = 1) : (result);
    return result;
}

uint16_t __thiscall get_figure_statistic_current_mp(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].mana.base_val;
    int32_t bonus_val = _this->figures[figure_id].mana.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 0;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].mana.bonus_multiplier + 100;
        uint16_t missing_val = _this->figures[figure_id].mana.missing_val;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
        (result - missing_val < 0) ? (result = 0) : (result -= missing_val);
    }
    return result;
}

uint16_t __thiscall get_figure_statistic_current_sta(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].stamina.base_val;
    int32_t bonus_val = _this->figures[figure_id].stamina.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 1;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].stamina.bonus_multiplier + 100;
        uint16_t missing_val = _this->figures[figure_id].stamina.missing_val;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
        (result - missing_val < 0) ? (result = 1) : (result -= missing_val);
    }
    return result;
}

uint16_t __thiscall get_figure_statistic_current_str(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].strength.base_val;
    int32_t bonus_val = _this->figures[figure_id].strength.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 1;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].strength.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    (result == 0) ? (result = 1) : (result);
    return result;
}

uint16_t __thiscall get_figure_statistic_current_wis(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].wisdom.base_val;
    int32_t bonus_val = _this->figures[figure_id].wisdom.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 1;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].wisdom.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    (result == 0) ? (result = 1) : (result);
    return result;
}

uint16_t __thiscall get_figure_statistic_current_fire_res(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].resistance_fire.base_val;
    int32_t bonus_val = _this->figures[figure_id].resistance_fire.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 1;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].resistance_fire.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    (result == 0) ? (result = 1) : (result);
    return result;
}

uint16_t __thiscall get_figure_statistic_current_ice_res(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].resistance_ice.base_val;
    int32_t bonus_val = _this->figures[figure_id].resistance_ice.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 1;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].resistance_ice.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    (result == 0) ? (result = 1) : (result);
    return result;
}

uint16_t __thiscall get_figure_statistic_current_mental_res(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].resistance_mental.base_val;
    int32_t bonus_val = _this->figures[figure_id].resistance_mental.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 1;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].resistance_mental.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    (result == 0) ? (result = 1) : (result);
    return result;
}

uint16_t __thiscall get_figure_statistic_current_black_res(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].resistance_black.base_val;
    int32_t bonus_val = _this->figures[figure_id].resistance_black.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 1;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].resistance_black.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    (result == 0) ? (result = 1) : (result);
    return result;
}

uint16_t __thiscall get_figure_statistic_current_walk_spd(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].walk_speed.base_val;
    int32_t bonus_val = _this->figures[figure_id].walk_speed.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 1;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].walk_speed.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    (result == 0) ? (result = 1) : (result);
    return result;
}

uint16_t __thiscall get_figure_statistic_current_fight_spd(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].fight_speed.base_val;
    int32_t bonus_val = _this->figures[figure_id].fight_speed.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 1;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].fight_speed.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    (result == 0) ? (result = 1) : (result);
    return result;
}

uint16_t __thiscall get_figure_statistic_current_cast_spd(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].cast_speed.base_val;
    int32_t bonus_val = _this->figures[figure_id].cast_speed.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 1;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].cast_speed.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    (result == 0) ? (result = 1) : (result);
    return result;
}

uint8_t __thiscall getEffectiveRace(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t master_id = _this->figures[figure_id].master_figure;

    while (master_id != 0)
    {
        if ((_this->figures[figure_id].flags & FOLLOW_MODE) == 0)
        {
            figure_id = master_id;
            master_id = _this->figures[figure_id].master_figure;
        }
        else
        {
            break;
        }
    }
    return _this->figures[figure_id].race;
}

/* Figure Statistics */
static void figure_statistic_hook_current_ac()
{
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(0x2b18e0), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2b18e0)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b18e1)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b18e2)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x2b18e3)) = (int)(&get_figure_statistic_current_ac)-
                                      ASI::AddrOf((0x2b18e7));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_agi()
{
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(0x2b1af0), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2b1af0)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b1af1)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b1af2)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x2b1af3)) = (int)(&get_figure_statistic_current_agi)-
                                      ASI::AddrOf((0x2b1af7));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_cha()
{
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(0x2b1c30), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2b1c30)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b1c31)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b1c32)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x2b1c33)) = (int)(&get_figure_statistic_current_cha)-
                                      ASI::AddrOf((0x2b1c37));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_dex()
{
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(0x2b2600), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2b2600)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b2601)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b2602)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x2b2603)) = (int)(&get_figure_statistic_current_dex)-
                                      ASI::AddrOf((0x2b2607));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_int()
{
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(0x2b28a0), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2b28a0)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b28a1)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b28a2)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x2b28a3)) = (int)(&get_figure_statistic_current_int)-
                                      ASI::AddrOf((0x2b28a7));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_hp()
{
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(0x279350), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(0x279350)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x279351)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x279352)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x279353)) = (int)(&get_figure_statistic_current_hp)-
                                      ASI::AddrOf((0x279357));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_mp()
{
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(0x2b29c0), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2b29c0)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b29c1)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b29c2)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x2b29c3)) = (int)(&get_figure_statistic_current_mp)-
                                      ASI::AddrOf((0x2b29c7));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_sta()
{
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(0x2b2e00), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2b2e00)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b2e01)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b2e02)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x2b2e03)) = (int)(&get_figure_statistic_current_sta)-
                                      ASI::AddrOf((0x2b2e07));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_str()
{
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(0x2b2eb0), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2b2eb0)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b2eb1)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b2eb2)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x2b2eb3)) = (int)(&get_figure_statistic_current_str)-
                                      ASI::AddrOf((0x2b2eb7));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_wis()
{
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(0x2b3160), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2b3160)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b3161)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b3162)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x2b3163)) = (int)(&get_figure_statistic_current_wis)-
                                      ASI::AddrOf((0x2b3167));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_fire_res()
{
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(0x2b2c00), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2b2c00)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b2c01)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b2c02)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x2b2c03)) = (int)(&get_figure_statistic_current_fire_res)-
                                      ASI::AddrOf((0x2b2c07));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_ice_res()
{
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(0x2b2ca0), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2b2ca0)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b2ca1)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b2ca2)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x2b2ca3)) = (int)(&get_figure_statistic_current_ice_res) - ASI::AddrOf((0x2b2ca7));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_mental_res()
{
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(0x2b2d40), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2b2d40)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b2d41)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b2d42)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x2b2d43)) = (int)(&get_figure_statistic_current_mental_res) - ASI::AddrOf((0x2b2d47));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_black_res()
{
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(0x2b2b60), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2b2b60)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b2b61)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b2b62)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x2b2b63)) = (int)(&get_figure_statistic_current_black_res) - ASI::AddrOf((0x2b2b67));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_walk_spd()
{
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(0x2b2fb0), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2b2fb0)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b2fb1)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b2fb2)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x2b2fb3)) = (int)(&get_figure_statistic_current_walk_spd) - ASI::AddrOf((0x2b2fb7));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_fight_spd()
{
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(0x2b26d0), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2b26d0)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b26d1)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b26d2)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x2b26d3)) = (int)(&get_figure_statistic_current_fight_spd) - ASI::AddrOf((0x2b26d7));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_cast_spd()
{
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(0x2b1b90), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2b1b90)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b1b91)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(0x2b1b92)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x2b1b93)) = (int)(&get_figure_statistic_current_cast_spd) - ASI::AddrOf((0x2b1b97));
    ASI::EndRewrite(ac_mreg);
}

static void salvo_fix_hook()
{
    ASI::MemoryRegion salvo_mreg1 (ASI::AddrOf(0x2eb294), 5);
    ASI::BeginRewrite(salvo_mreg1);
    *(unsigned char *)(ASI::AddrOf(0x2eb294)) = 0xE8;
    *(int *)(ASI::AddrOf(0x2eb295)) = (int)(&get_salvo_target_list) - ASI::AddrOf((0x2eb299));
    ASI::EndRewrite(salvo_mreg1);

}

static void get_race_fix_hook()
{
    ASI::MemoryRegion race_mreg1 (ASI::AddrOf(0x29b230), 7);
    ASI::BeginRewrite(race_mreg1);
    *(unsigned char *)(ASI::AddrOf(0x29b230)) = 0x90;
    *(unsigned char *)(ASI::AddrOf(0x29b231)) = 0x90;
    *(unsigned char *)(ASI::AddrOf(0x29b232)) = 0xE9;
    *(int *)(ASI::AddrOf(0x29b233)) = (int)(&getEffectiveRace) - ASI::AddrOf((0x29b237));
    ASI::EndRewrite(race_mreg1);
}

bool __thiscall isTitan(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t figure_data_id = _this->figures[figure_id].unit_data_id;
    bool result = false;

    switch (figure_data_id)
    {
        case 1047:
        case 1079:
        case 1098:
        case 1099:
        case 1100:
        case 1101:
        case 2281:
        case 2282:
        case 2283:
        case 2284:
        case 2285:
        case 2286:
        {
            result = true;
            break;
        }
        default:
        {
            result = false;
            break;
        }
    }
    return result;
}

bool __thiscall isSwarm(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t figure_data_id = _this->figures[figure_id].unit_data_id;
    bool result = ((figure_data_id > 2219) && (figure_data_id < 2226));
    return result;
}
void __thiscall decreaseUsedArmyUnitCount(SF_CGdPlayer *_this, uint16_t owner, uint8_t side)
{
    uint16_t unit_count = _this->players[owner].warrior_count[side];
    if (unit_count != 0)
    {
        unit_count--;
    }
    _this->players[owner].warrior_count[side] = unit_count;
}

uint32_t __thiscall decreaseUnitCount(AutoClass46 *_this, uint16_t figure_id)
{
    uint8_t race = _this->CGdFigure->figures[figure_id].race;
    uint16_t owner = _this->CGdFigure->figures[figure_id].owner;
    if ((owner != 0)  && (race != 0) && (race < 7))
    {
        if ((_this->CGdFigure->figures[figure_id].flags & ILLUSION) != 0)
        {
            return 0;
        }
        if ((!isTitan(_this->CGdFigure, figure_id)) && (!isSwarm(_this->CGdFigure, figure_id)))
        {
            if (!figureAPI.isWarrior(_this->CGdFigure, figure_id))
            {
                uint16_t worker_count = _this->CGdPlayer->players[owner].civilian_count[race - 1];
                if (worker_count > 0)
                {
                    worker_count--;
                }
                _this->CGdPlayer->players[owner].civilian_count[race - 1] = worker_count;
                return 0;
            }
            else
            {
                bool side = ((race != 3) && (race != 2) && (race != 1));
                decreaseUsedArmyUnitCount(_this->CGdPlayer, owner, side);
                if (figureAPI.isSiegeUnit(_this->CGdFigure, figure_id))
                {
                    decreaseUsedArmyUnitCount(_this->CGdPlayer, owner, side);
                    decreaseUsedArmyUnitCount(_this->CGdPlayer, owner, side);
                }
            }
        }

    }
    return 0;
}

static void army_size_fix_hook()
{
    ASI::MemoryRegion race_mreg1 (ASI::AddrOf(0x326e40), 5);
    ASI::BeginRewrite(race_mreg1);
    *(unsigned char *)(ASI::AddrOf(0x326e40)) = 0xE9;
    *(int *)(ASI::AddrOf(0x326e41)) = (int)(&decreaseUnitCount) - ASI::AddrOf((0x326e45));
    ASI::EndRewrite(race_mreg1);
}

static SF_CGdFigure * getFigurePtr(void *CGdMain)
{
    return (SF_CGdFigure *)(*(uint32_t *)((uint32_t)CGdMain + 0x30));
}

void __thiscall SFStringConstructor(SF_String *_this)
{
    _this->raw_data = nullptr;
    _this->char_data = nullptr;
    _this->str_length = 0x0;
    _this->str_length_char = 0x0;
}

SF_String * __thiscall SFStringConstructor_char(SF_String *_this, const char *char_string)
{
    SFStringConstructor(_this);
    uint32_t length = 0;
    if (char_string != 0)
    {
        //not sure here, might need to use internal implementation?
        length = strlen(char_string);
    }
    SFStringSetLength(_this, length);
    if (length != 0)
    {
        mbstowcs(_this->raw_data, char_string, length);
    }
    _this->str_length = length;
    return _this;
}

void __thiscall prepareDamageString(AutoClass101 *_this, SF_CGdFigure *figures, uint16_t figure_id,
                                    CGdResourceWeaponData *weapon_data, SF_String *out_string)
{
    uint32_t lvl_mult = 0;
    SF_String description;
    SFStringConstructor(&description);

    //TASK_MAIN_CHAR or TASK_HERO
    if ((figures->figures[figure_id].current_job.task == 0xa) ||
        (figures->figures[figure_id].current_job.task == 0x9))
    {
        lvl_mult = 100;
    }
    else
    {
        lvl_mult = GetDamageMultiplierPerLevel(_this->SF_CGdResource, figures->figures[figure_id].level);
    }
    uint32_t str_mult = figureAPI.getCurrentStat(figures, figure_id, STRENGTH);
    str_mult *= 3 * (figures->figures[figure_id].level + 9);
    GetDescriptionText(_this->UiDbProxy, 4531, &description);
    if (weapon_data->min_dmg == weapon_data->max_dmg)
    {
        uint32_t damage = (weapon_data->max_dmg * str_mult * lvl_mult + 50000) / 100000;
        SFprintf(out_string,L"%s %i ", description.raw_data, damage);
    }
    else
    {
        uint32_t max_damage = (weapon_data->max_dmg * str_mult * lvl_mult + 50000) / 100000;
        uint32_t min_damage = (weapon_data->min_dmg * str_mult * lvl_mult + 50000) / 100000;
        SFprintf(out_string,L"%s %i-%i ", description.raw_data, min_damage,max_damage);
    }
    SFStringDestructor(&description);
}


void prepareHPString(AutoClass101 *_this, SF_CGdFigure *figures, uint16_t figure_id, SF_String *out_string)
{
    SF_String full_desc;
    SF_String description;

    SFStringConstructor(&full_desc);
    SFStringConstructor(&description);

    uint32_t max_hp = figureAPI.getMaxStat(figures, figure_id, HEALTH);
    uint32_t hp = figureAPI.getCurrentStat(figures, figure_id, HEALTH);
    SFprintf(&full_desc, L"%i/%i", hp, max_hp);
    GetColoredText(&full_desc, 280);
    GetDescriptionText(_this->UiDbProxy, 4407, &description);
    SFprintf(out_string, L"%s: %s", description.raw_data, full_desc.raw_data);

    SFStringDestructor(&description);
    SFStringDestructor(&full_desc);
}

void prepareMPString(AutoClass101 *_this, SF_CGdFigure *figures, uint16_t figure_id, SF_String *out_string)
{
    SF_String full_desc;
    SF_String description;

    SFStringConstructor(&full_desc);
    SFStringConstructor(&description);

    uint32_t max_mp = figureAPI.getMaxStat(figures, figure_id, MANA);
    uint32_t mp = figureAPI.getCurrentStat(figures, figure_id, MANA);
    SFprintf(&full_desc, L"%i/%i", mp, max_mp);
    GetColoredText(&full_desc, 280);
    GetDescriptionText(_this->UiDbProxy, 4408, &description);
    SFprintf(out_string, L"%s: %s", description.raw_data, full_desc.raw_data);

    SFStringDestructor(&description);
    SFStringDestructor(&full_desc);
}

void prepareSpellsHeaderString(AutoClass101 *_this, SF_String *out_string)
{
    SF_String header_string;
    SF_String desc_str1;
    SF_String desc_str2;
    SF_String newline;

    SFStringFromWchar(&header_string, L'\n', 1);
    SFStringFromWchar(&newline, L'\n', 1);
    SFStringConstructor(&desc_str1);
    SFStringConstructor(&desc_str2);

    GetDescriptionText(_this->UiDbProxy, 4802, &desc_str1);
    SFprintf(&desc_str2, L"%s ", desc_str1);
    SFStringConcat(&header_string, &desc_str2);
    SFprintf(&header_string, L"%%c%x%s%%d",0xc0d0ff,header_string.raw_data);
//    SFStringConcat(&header_string, &newline);

    SFStringCopy(out_string, &header_string);

    SFStringDestructor(&newline);
    SFStringDestructor(&header_string);
    SFStringDestructor(&desc_str1);
    SFStringDestructor(&desc_str2);
}

void prepareSpellString(AutoClass101 *_this, SF_String *out_string, uint16_t spell_id)
{
    SF_CGdResourceSpell spell_data;
    SF_CGdResourceSpellLine spell_line_data;

    SF_String spell_string;
    SF_String newline;
    SF_String desc_str1;
    SF_String desc_str2;
    SF_String full_desc;

    SFStringConstructor(&desc_str1);
    SFStringConstructor(&desc_str2);
    SFStringFromWchar(&spell_string, L'\n', 1);
    SFStringFromWchar(&newline, L'\n', 1);

    spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, spell_id);
    if ((GetResourceSpellLine(_this->SF_CGdResource, spell_data.spell_line_id,
                              &spell_line_data) != 0) && (spell_line_data.description_id != 0))
    {
        GetTextById(_this->UiDbProxy, spell_line_data.text_id, &desc_str1);
        GetColoredText(&desc_str1, 278);
        SFStringConcat(&spell_string, &desc_str1);
        SFStringConcat(&spell_string, &newline);

        GetDescriptionText(_this->UiDbProxy, spell_line_data.description_id, &desc_str2);
        ExpandSpellDescription(_this,&full_desc, &desc_str2, &spell_data);
        SFStringConcat(&spell_string, &full_desc);
    }
    if (spell_string.str_length > 1)
    {
        SFStringCopy(out_string, &spell_string);
    }

    SFStringDestructor(&desc_str1);
    SFStringDestructor(&desc_str2);

    SFStringDestructor(&full_desc);
    SFStringDestructor(&spell_string);
    SFStringDestructor(&newline);
}

void prepareEffectString(AutoClass101 *_this, SF_CGdFigure *figures, uint16_t figure_id, uint16_t effect_id,
                         SF_String *out_string)
{
    SF_String desc_str1;
    SF_String desc_str2;
    SF_String description;
    SF_String newline;
    SF_String chance_string;

    SFStringConstructor(&chance_string);
    SFStringConstructor(&desc_str1);
    SFStringConstructor(&desc_str2);
    SFStringConstructor(&description);
    SFStringFromWchar(&newline, L'\n', 1);

    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, effect_id);
    SF_CGdResourceSpellLine spell_line_data;
    GetResourceSpellLine(_this->SF_CGdResource, spell_data.spell_line_id, &spell_line_data);
    if (spell_data.spell_line_id != 0 && spell_line_data.description_id != 0)
    {
        GetDescriptionText(_this->UiDbProxy, 6550, &desc_str1);
        SFprintf(out_string,L"%%c%x%s:%%d ",0xc0d0ff,desc_str1.raw_data);
        GetTextById(_this->UiDbProxy, spell_line_data.text_id, &description);
        GetColoredText(&description, 278);
        SFStringConcat(out_string, &description);
        enchant_handler_ptr handler = get_enchant_handler(spell_data.spell_line_id);
        uint16_t chance = handler(figures, figure_id);
        SFprintf(&chance_string,L" (%.1f%%%%)", (double)((float)chance / 100.0));
        SFStringConcat(out_string, &chance_string);

        GetDescriptionText(_this->UiDbProxy, spell_line_data.description_id, &desc_str2);
        SF_String temp_str;
        ExpandSpellDescription(_this, &temp_str, &desc_str2, &spell_data);
        SFStringConcat(&newline, &temp_str);
        SFStringConcat(out_string, &newline);

        SFStringDestructor(&temp_str);
    }
    SFStringDestructor(&desc_str1);
    SFStringDestructor(&desc_str2);

    SFStringDestructor(&description);
    SFStringDestructor(&chance_string);
    SFStringDestructor(&newline);
}

SF_String * __thiscall portrait_overlay_hook(AutoClass101 *_this, SF_String *param_1, uint16_t figure_id,
                                             uint32_t param3,
                                             uint32_t param4, uint32_t param5)
{
    SF_String figure_name;
    SF_String full_desc;
    SF_String description;

    SF_String result_string;
    SF_String newline;
    SF_String aspd_string;

    SFStringConstructor(&full_desc);
    SFStringConstructor(&description);
    SFStringConstructor(&aspd_string);
    SFStringConstructor(&figure_name);

    SFStringConstructor_char(&result_string, "");
    SFStringFromWchar(&newline, L'\n', 1);


    SF_CGdFigure *SF_Figures = getFigurePtr(_this->CGdMain);

    if (SF_Figures->figures[figure_id].owner != (uint16_t)(-1))
    {
        AC95_get_figure_name(_this->AC95, &figure_name, figure_id);
        GetColoredText(&figure_name, 282);
        GetDescriptionText(_this->UiDbProxy, 4425, &description);
        if (param3 == 0)
        {
            SFprintf(&full_desc, L"%s", figure_name.raw_data);
        }
        else
        {
            uint8_t level = SF_Figures->figures[figure_id].level;
            SFprintf(&full_desc, L"%s (%s %i)", figure_name.raw_data, description.raw_data, level);
        }
        SFStringConcat(&result_string, &full_desc);
        if (param3 != 0)
        {
            SFStringConcat(&result_string, &newline);             //concatenate newline
            SF_String hp_string;
            SFStringConstructor(&hp_string);
            prepareHPString(_this, SF_Figures, figure_id, &hp_string);
            SFStringConcat(&result_string, &hp_string);
            SFStringDestructor(&hp_string);
        }
        if (param4 != 0)
        {
            SF_String mp_string;
            SFStringConstructor(&mp_string);
            SFStringConcat(&result_string, &newline);              //concatenate newline
            prepareMPString(_this, SF_Figures, figure_id, &mp_string);
            SFStringConcat(&result_string, &mp_string);
            SFStringDestructor(&mp_string);
        }
        if (param5 != 0)
        {
            bool both_hands = true;
            uint32_t dual_wield = 0;
            CGdResourceWeaponData l_item_info;
            CGdResourceWeaponData r_item_info;
            CGdResourceWeaponData *active_hand_info = nullptr;
            SF_String desc_str1;
            SF_String desc_str2;

            SFStringConstructor(&desc_str1);
            SFStringConstructor(&desc_str2);

            uint8_t r_type;
            uint8_t l_type;
            uint16_t r_item_id = SF_Figures->figures[figure_id].equipment[1];
            GetItemInfo(_this->SF_CGdResource,&r_item_info, r_item_id);

            uint16_t l_item_id =  SF_Figures->figures[figure_id].equipment[3];
            GetItemInfo(_this->SF_CGdResource,&l_item_info, l_item_id);

            r_type = GetWeaponType(_this->SF_CGdResource, r_item_id);
            l_type = GetWeaponType(_this->SF_CGdResource, l_item_id);
            active_hand_info = &r_item_info;
            if ((l_item_info.min_dmg == 0) || (r_item_info.min_dmg == 0))
            {
                both_hands = false;
                if (l_item_info.min_dmg != 0)
                {
                    active_hand_info = &l_item_info;
                    r_type = l_type;
                    r_item_id = l_item_id;
                }
            }
            else
            {
                GetDescriptionText(_this->UiDbProxy, 4800, &desc_str1);
                GetDescriptionText(_this->UiDbProxy, 4801, &desc_str2);
                if ((r_type != 7) || (l_type != 7))
                {
                    dual_wield = 0;
                }
                else
                {
                    dual_wield = 1;
                }
            }
            if (active_hand_info != nullptr)
            {
                SF_String damage_string;
                SFStringConstructor(&damage_string);

                SFStringConcat(&result_string, &newline);
                SFStringSetLength(&full_desc, 0);
                if (both_hands)
                {
                    SFprintf(&full_desc, L"%s: ", desc_str2.raw_data);
                }
                prepareDamageString(_this, SF_Figures, figure_id, active_hand_info, &damage_string);
                GetColoredText(&damage_string, 280);
                SFStringConcat(&full_desc, &damage_string);
                SFStringDestructor(&damage_string);


                GetDescriptionText(_this->UiDbProxy, 4803, &aspd_string);
                SFprintf(&description, L"%s, ", aspd_string.raw_data);

                uint8_t race = SF_Figures->figures[figure_id].race;
                uint32_t aspd = figureAPI.getCurrentStat(SF_Figures, figure_id, FIGHT_SPEED);
                uint32_t *AC34 = (uint32_t *)CGdMainGetAC34(_this->CGdMain);
                aspd = AC34calculateWeaponSpeed(AC34, aspd, race, r_item_id, dual_wield, (r_type == 12));

                double disp_aspd = static_cast<double>(aspd) / 1000.0;
                SFprintf(&description, description.raw_data, disp_aspd);
                SFStringConcat(&full_desc, &description);

                GetDescriptionText(_this->UiDbProxy, 4532, &description);
                GetColoredText(&description, 280);

                if (active_hand_info->min_rng ==  active_hand_info->max_rng)
                {
                    SF_String temp_str;
                    SFStringConstructor(&temp_str);

                    if (active_hand_info->min_rng != 0)
                    {
                        getSpecialDesc(&figure_name, 280);
                        SFprintf(&temp_str, L"%s_%s%i%s%c ", description.raw_data, figure_name.raw_data,
                                 active_hand_info->min_rng, L"%d", L',');
                        SFStringDestructor(&figure_name);
                    }
                    SFStringConcat(&full_desc, &temp_str);
                    SFStringDestructor(&temp_str);
                }
                else
                {
                    getWeaponRangeDescription(&full_desc, active_hand_info->min_rng, active_hand_info->max_rng,
                                              &description, 0x01);
                }
                if (full_desc.str_length != 0)
                {
                    SFStringConcat(&result_string, &full_desc);
                }

                SFStringDestructor(&full_desc);
                SFStringDestructor(&description);

                SFStringConstructor(&full_desc);
                SFStringConstructor(&description);

                uint16_t effects[4];
                GetWeaponEffects(_this->SF_CGdResource, effects, r_item_id);
                for (int i = 1; i < 4; i++)
                {
                    SF_String effect_string;
                    SFStringConstructor(&effect_string);

                    if (effects[i] == 0)
                    {
                        break;
                    }
                    uint16_t effect_id = effects[i];
                    if ((SF_Figures->figures[figure_id].race != 0) && (SF_Figures->figures[figure_id].race < 7))
                    {
                        if (SF_Figures->figures[figure_id].owner != 0)
                        {
                            effect_id = spellAPI.getLeveledSpellID(_this->SF_CGdResource, effect_id,
                                                                   SF_Figures->figures[figure_id].level);
                        }
                    }
                    SFStringConcat(&full_desc, &newline);
                    prepareEffectString(_this, SF_Figures, figure_id, effect_id, &effect_string);
                    SFStringConcat(&full_desc, &effect_string);
                }
                if (full_desc.str_length != 0)
                {
                    SFStringConcat(&result_string, &full_desc);
                }
                if (both_hands)
                {
                    SF_String damage_string;
                    SFStringConstructor(&damage_string);

                    SFStringConcat(&result_string, &newline);
                    SFStringSetLength(&full_desc, 0);
                    if (both_hands)
                    {
                        SFprintf(&full_desc, L"%s: ", desc_str1.raw_data);
                    }
                    prepareDamageString(_this, SF_Figures, figure_id, &l_item_info, &damage_string);
                    GetColoredText(&damage_string, 280);
                    SFStringConcat(&full_desc, &damage_string);
                    SFStringDestructor(&damage_string);

                    GetDescriptionText(_this->UiDbProxy, 4803, &aspd_string);
                    SFprintf(&description, L"%s, ", aspd_string.raw_data);

                    uint8_t race = SF_Figures->figures[figure_id].race;
                    uint32_t aspd = figureAPI.getCurrentStat(SF_Figures, figure_id, FIGHT_SPEED);
                    uint32_t *AC34 = (uint32_t *)CGdMainGetAC34(_this->CGdMain);
                    aspd = AC34calculateWeaponSpeed(AC34, aspd, race, l_item_id, dual_wield, (r_type == 12));

                    double disp_aspd = static_cast<double>(aspd) / 1000.0;
                    SFprintf(&description, description.raw_data, disp_aspd);
                    SFStringConcat(&full_desc, &description);

                    GetDescriptionText(_this->UiDbProxy, 4532, &description);
                    GetColoredText(&description, 280);

                    if (l_item_info.min_rng == l_item_info.max_rng)
                    {
                        SF_String temp_str;
                        SFStringConstructor(&temp_str);

                        if (l_item_info.min_rng != 0)
                        {
                            getSpecialDesc(&figure_name, 280);
                            SFprintf(&temp_str, L"%s_%s%i%s%c ", description.raw_data, figure_name.raw_data,
                                     l_item_info.min_rng, L"%d", L',');
                            SFStringDestructor(&figure_name);
                        }
                        SFStringConcat(&full_desc, &temp_str);
                        SFStringDestructor(&temp_str);
                    }
                    else
                    {
                        getWeaponRangeDescription(&full_desc, l_item_info.min_rng, l_item_info.max_rng, &description,
                                                  0x01);
                    }
                    if (full_desc.str_length != 0)
                    {
                        SFStringConcat(&result_string, &full_desc);
                    }

                    SFStringDestructor(&full_desc);
                    SFStringDestructor(&description);

                    SFStringConstructor(&full_desc);
                    SFStringConstructor(&description);

                    uint16_t effects[4];
                    GetWeaponEffects(_this->SF_CGdResource, effects, l_item_id);
                    for (int i = 1; i < 4; i++)
                    {
                        SF_String effect_string;
                        SFStringConstructor(&effect_string);

                        if (effects[i] == 0)
                        {
                            break;
                        }
                        uint16_t effect_id = effects[i];
                        if ((SF_Figures->figures[figure_id].race != 0) && (SF_Figures->figures[figure_id].race < 7))
                        {
                            if (SF_Figures->figures[figure_id].owner != 0)
                            {
                                effect_id = spellAPI.getLeveledSpellID(_this->SF_CGdResource, effect_id,
                                                                       SF_Figures->figures[figure_id].level);
                            }
                        }
                        SFStringConcat(&full_desc, &newline);
                        prepareEffectString(_this, SF_Figures, figure_id, effect_id, &effect_string);
                        SFStringConcat(&full_desc, &effect_string);
                    }
                    if (full_desc.str_length != 0)
                    {
                        SFStringConcat(&result_string, &full_desc);
                    }
                }
            }
            uint8_t spells_count = 0;
            for (uint8_t i = 0; i < 15; i++)
            {
                SF_SGtFigureAction action;
                memset(&action, 0, sizeof(SF_SGtFigureAction));
                aiAPI.getFigureAction(SF_Figures, &action, figure_id, i);
                if (action.type == 0)
                {
                    continue;
                }
                if ((action.type < 9999) && (action.type != 241))
                {
                    spells_count++;
                }
            }
            if (spells_count != 0)
            {
                SF_String header_string;
                SFStringConstructor(&header_string);
                prepareSpellsHeaderString(_this, &header_string);
                for (int i = 0; i < 15; i++)
                {
                    SF_SGtFigureAction action;
                    memset(&action, 0, sizeof(SF_SGtFigureAction));
                    aiAPI.getFigureAction(SF_Figures, &action, figure_id, i);
                    if (action.type == 0)
                    {
                        continue;
                    }
                    if ((action.type < 9999) && (action.type != 241))
                    {
                        SF_String spell_string;
                        uint16_t spell_id = action.subtype;
                        if ((SF_Figures->figures[figure_id].race != 0) && (SF_Figures->figures[figure_id].race < 7))
                        {
                            if (SF_Figures->figures[figure_id].owner != 0)
                            {
                                spell_id = spellAPI.getLeveledSpellID(_this->SF_CGdResource, spell_id,
                                                                      SF_Figures->figures[figure_id].level);
                            }
                        }
                        SFStringConstructor(&spell_string);
                        prepareSpellString(_this, &spell_string, spell_id);
                        if (spell_string.str_length != 0)
                        {
                            SFStringConcat(&header_string, &spell_string);
                        }
                        SFStringDestructor(&spell_string);
                    }
                }
                SFStringConcat(&result_string, &header_string);
                SFStringDestructor(&header_string);
            }
        }
    }
    SFStringCopy(param_1, &result_string);
    SFStringDestructor(&result_string);
    SFStringDestructor(&figure_name);
    SFStringDestructor(&full_desc);
    SFStringDestructor(&description);
    SFStringDestructor(&newline);
    return param_1;
}


SF_String * __thiscall getSpecialFigureName(SF_CGdFigureToolbox *_this, SF_String *in_string, uint16_t figure_id)
{
    SF_String out_string;
    SFStringConstructor(&out_string);
    uint8_t figure_task = _this->CGdFigure->figures[figure_id].current_job.task;
    switch (figure_task)
    {
        case TASK_HERO:
        {
            uint16_t owner = _this->CGdFigure->figures[figure_id].owner;
            uint8_t hero_index = 0xff;
            for (int i = 0; i < 5; i++)
            {
                if (_this->CGdPlayer->players[owner].hero_figure_index[i] == figure_id)
                {
                    hero_index = i;
                    break;
                }
            }
            if (hero_index != 0xff)
            {
                SFStringCopy(&out_string, &_this->CGdPlayer->players[owner].hero_names[hero_index]);
            }
            break;
        }
        case TASK_MAINCHAR:
        {
            uint16_t owner = _this->CGdFigure->figures[figure_id].owner;
            uint16_t avatar_id = _this->CGdPlayer->players[owner].avatar_figure_index;
            if (avatar_id != 0)
            {
                SFStringCopy(&out_string, &_this->CGdPlayer->players[owner].avatar_name);
            }
            break;
        }
        default:
        {
            if ((_this->CGdFigure->figures[figure_id].flags & ILLUSION) != 0)
            {
                uint16_t master_id = _this->CGdFigure->figures[figure_id].master_figure;
                SF_String master_name;
                SFStringConstructor(&master_name);

                getSpecialFigureName(_this, &master_name, master_id);
                SFStringCopy(&out_string, &master_name);
                SFStringDestructor(&master_name);
            }
            else
            {
                uint16_t data_id = _this->CGdFigure->figures[figure_id].unit_data_id;
                if (data_id == 0)
                {
                    SF_String empty;
                    SFStringConstructor_char(&empty, "");
                    SFStringCopy(&out_string, &empty);
                    SFStringDestructor(&empty);
                }
                else
                {
                    uint16_t unit_str_id = getUnitStringID(_this->CGdResource, data_id);
                    char resource_str[512];
                    getTextString(_this->CGdResource, unit_str_id, resource_str);
                    SF_String sf_resource_str;
                    SFStringConstructor_char(&sf_resource_str, resource_str);
                    SFStringCopy(&out_string, &sf_resource_str);
                    SFStringDestructor(&sf_resource_str);
                }
            }
            break;
        }
    }

    SFStringCopy(in_string, &out_string);
    return in_string;
}

void initialize_vanilla_fix_hooks()
{
    add_item = (add_item_ptr)(ASI::AddrOf(0x260d70));

    AC95_get_figure_name = (AC95_get_figure_name_ptr)(ASI::AddrOf(0x567086));
    GetColoredText = (GetColoredText_ptr)(ASI::AddrOf(0x619670));
    GetDescriptionText = (GetDescriptionText_ptr)(ASI::AddrOf(0x575cd0));
    GetTextById = (GetTextById_ptr)(ASI::AddrOf(0x575740));
    ExpandSpellDescription = (ExpandSpellDescription_ptr)(ASI::AddrOf(0x5d7c20));
    SFprintf = (SFprintf_ptr)(ASI::AddrOf(0x384170));
    SFStringConcat = (SFStringConcat_ptr)(ASI::AddrOf(0x383d00));
    SFStringSetLength = (SFStringSetLength_ptr)(ASI::AddrOf(0x3846d0));
    SFStringFromWchar = (SFStringFromWchar_ptr)(ASI::AddrOf(0x383920));
    SFStringDestructor = (SFStringDestructor_ptr)(ASI::AddrOf(0x3839c0));
    SFStringCopy = (SFStringCopy_ptr)(ASI::AddrOf(0x383720));

    GetItemInfo = (GetItemInfo_ptr)(ASI::AddrOf(0x269460));
    GetWeaponType = (GetWeaponType_ptr)(ASI::AddrOf(0x269260));
    GetDamageMultiplierPerLevel = (GetDamageMultiplierPerLevel_ptr)(ASI::AddrOf(0x26f170));
    CGdMainGetAC34 = (CGdMainGetAC34_ptr)(ASI::AddrOf(0x277271));
    AC34calculateWeaponSpeed = (AC34calculateWeaponSpeed_ptr)(ASI::AddrOf(0x3175c0));
    getSpecialDesc = (getSpecialDesc_ptr)(ASI::AddrOf(0x619530));
    getWeaponRangeDescription = (getWeaponRangeDescription_ptr)(ASI::AddrOf(0x5cf200));
    GetWeaponEffects = (GetWeaponEffects_ptr)(ASI::AddrOf(0x2693b0));
    GetResourceSpellLine = (GetResourceSpellLine_ptr)(ASI::AddrOf(0x26e2d0));
    getUnitStringID = (getUnitStringID_ptr)(ASI::AddrOf(0x26e9f0));
    getTextString = (getTextString_ptr)(ASI::AddrOf(0x26e510));

    log_info("| - Vanilla Fix Hooks");
    figure_statistic_hook_current_ac();
    figure_statistic_hook_current_agi();
    figure_statistic_hook_current_cha();
    figure_statistic_hook_current_dex();
    figure_statistic_hook_current_int();
    figure_statistic_hook_current_hp();
    figure_statistic_hook_current_mp();
    figure_statistic_hook_current_sta();
    figure_statistic_hook_current_str();
    figure_statistic_hook_current_wis();
    figure_statistic_hook_current_fire_res();
    figure_statistic_hook_current_ice_res();
    figure_statistic_hook_current_mental_res();
    figure_statistic_hook_current_black_res();
    figure_statistic_hook_current_walk_spd();
    figure_statistic_hook_current_fight_spd();
    figure_statistic_hook_current_cast_spd();
    salvo_fix_hook();
    get_race_fix_hook();
    army_size_fix_hook();
}
