#include "../sf_wrappers.h"
#include "../../../asi/sf_asi.h"
#include "sf_vanilla_fix_hook.h"
#define _USE_MATH_DEFINES
#include <math.h>

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

void initialize_vanilla_fix_hooks()
{
    add_item = (add_item_ptr)(ASI::AddrOf(0x260d70));

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
}
