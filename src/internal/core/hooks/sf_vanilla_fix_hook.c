#include "../sf_wrappers.h"
#include "../../../asi/sf_asi.h"
#include "sf_vanilla_fix_hook.h"


uint16_t __thiscall get_figure_statistic_max_hp(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].health.base_val;
    int32_t bonus_val = _this->figures[figure_id].health.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 0;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].health.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    return result;
}

uint16_t __thiscall get_figure_statistic_max_mp(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].mana.base_val;
    int32_t bonus_val = _this->figures[figure_id].mana.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 0;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].mana.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    return result;
}

uint16_t __thiscall get_figure_statistic_max_sta(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].stamina.base_val;
    int32_t bonus_val = _this->figures[figure_id].stamina.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 0;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].stamina.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    return result;
}

uint16_t __thiscall get_figure_statistic_current_ac(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].armor.base_val;
    int32_t bonus_val = _this->figures[figure_id].armor.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 0;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].armor.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    return result;
}

uint16_t __thiscall get_figure_statistic_current_agi(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].agility.base_val;
    int32_t bonus_val = _this->figures[figure_id].agility.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 0;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].agility.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    return result;
}

uint16_t __thiscall get_figure_statistic_current_cha(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].charisma.base_val;
    int32_t bonus_val = _this->figures[figure_id].charisma.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 0;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].charisma.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    return result;
}

uint16_t __thiscall get_figure_statistic_current_dex(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].dexterity.base_val;
    int32_t bonus_val = _this->figures[figure_id].dexterity.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 0;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].dexterity.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
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
    uint16_t result = 0;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].intelligence.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
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
    uint16_t result = 0;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].stamina.bonus_multiplier + 100;
        uint16_t missing_val = _this->figures[figure_id].stamina.missing_val;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
        (result - missing_val < 0) ? (result = 0) : (result -= missing_val);
    }
    return result;
}

uint16_t __thiscall get_figure_statistic_current_str(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].strength.base_val;
    int32_t bonus_val = _this->figures[figure_id].strength.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 0;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].strength.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    return result;
}

uint16_t __thiscall get_figure_statistic_current_wis(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].wisdom.base_val;
    int32_t bonus_val = _this->figures[figure_id].wisdom.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 0;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].wisdom.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    return result;
}

uint16_t __thiscall get_figure_statistic_current_fire_res(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].resistance_fire.base_val;
    int32_t bonus_val = _this->figures[figure_id].resistance_fire.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 0;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].resistance_fire.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    return result;
}

uint16_t __thiscall get_figure_statistic_current_ice_res(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].resistance_ice.base_val;
    int32_t bonus_val = _this->figures[figure_id].resistance_ice.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 0;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].resistance_ice.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    return result;
}

uint16_t __thiscall get_figure_statistic_current_mental_res(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].resistance_mental.base_val;
    int32_t bonus_val = _this->figures[figure_id].resistance_mental.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 0;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].resistance_mental.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    return result;
}

uint16_t __thiscall get_figure_statistic_current_black_res(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].resistance_black.base_val;
    int32_t bonus_val = _this->figures[figure_id].resistance_black.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 0;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].resistance_black.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    return result;
}

uint16_t __thiscall get_figure_statistic_current_walk_spd(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].walk_speed.base_val;
    int32_t bonus_val = _this->figures[figure_id].walk_speed.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 0;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].walk_speed.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    return result;
}

uint16_t __thiscall get_figure_statistic_current_fight_spd(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].fight_speed.base_val;
    int32_t bonus_val = _this->figures[figure_id].fight_speed.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 0;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].fight_speed.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    return result;
}

uint16_t __thiscall get_figure_statistic_current_cast_spd(SF_CGdFigure *_this, uint16_t figure_id)
{
    int32_t base_val = _this->figures[figure_id].cast_speed.base_val;
    int32_t bonus_val = _this->figures[figure_id].cast_speed.bonus_val;
    int32_t total_flat = base_val+bonus_val;
    uint16_t result = 0;
    if (total_flat >= 0)
    {
        int16_t multiplier = _this->figures[figure_id].cast_speed.bonus_multiplier + 100;
        (multiplier < 0) ? (multiplier = 1) : (multiplier);
        result = (uint16_t)((total_flat * multiplier) / 100);
    }
    return result;
}

/* Figure Statistics */
static void figure_statistic_hook_current_ac()
{
    uint32_t addrbase = 0x2b18e0;
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(addrbase), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(++addrbase)) = (int)(&get_figure_statistic_current_ac)-
                                        ASI::AddrOf((addrbase + 4));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_agi()
{
    uint32_t addrbase = 0x2b1af0;
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(addrbase), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(++addrbase)) = (int)(&get_figure_statistic_current_agi)-
                                        ASI::AddrOf((addrbase + 4));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_cha()
{
    uint32_t addrbase = 0x2b1c30;
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(addrbase), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(++addrbase)) = (int)(&get_figure_statistic_current_cha)-
                                        ASI::AddrOf((addrbase + 4));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_dex()
{
    uint32_t addrbase = 0x2b2600;
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(addrbase), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(++addrbase)) = (int)(&get_figure_statistic_current_dex)-
                                        ASI::AddrOf((addrbase + 4));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_int()
{
    uint32_t addrbase = 0x2b28a0;
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(addrbase), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(++addrbase)) = (int)(&get_figure_statistic_current_int)-
                                        ASI::AddrOf((addrbase + 4));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_hp()
{
    uint32_t addrbase = 0x279350;
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(addrbase), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(++addrbase)) = (int)(&get_figure_statistic_current_hp)-
                                        ASI::AddrOf((addrbase + 4));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_mp()
{
    uint32_t addrbase = 0x2b29c0;
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(addrbase), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(++addrbase)) = (int)(&get_figure_statistic_current_mp)-
                                        ASI::AddrOf((addrbase + 4));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_sta()
{
    uint32_t addrbase = 0x2b2e00;
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(addrbase), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(++addrbase)) = (int)(&get_figure_statistic_current_sta)-
                                        ASI::AddrOf((addrbase + 4));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_str()
{
    uint32_t addrbase = 0x2b2eb0;
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(addrbase), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(++addrbase)) = (int)(&get_figure_statistic_current_str)-
                                        ASI::AddrOf((addrbase + 4));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_wis()
{
    uint32_t addrbase = 0x2b3160;
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(addrbase), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(++addrbase)) = (int)(&get_figure_statistic_current_wis)-
                                        ASI::AddrOf((addrbase + 4));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_fire_res()
{
    uint32_t addrbase = 0x2b2c00;
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(addrbase), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(++addrbase)) = (int)(&get_figure_statistic_current_fire_res)-
                                        ASI::AddrOf((addrbase + 4));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_ice_res()
{
    uint32_t addrbase = 0x2b2ca0;
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(addrbase), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(++addrbase)) = (int)(&get_figure_statistic_current_ice_res)-
                                        ASI::AddrOf((addrbase + 4));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_mental_res()
{
    uint32_t addrbase = 0x2b2d40;
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(addrbase), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(++addrbase)) = (int)(&get_figure_statistic_current_mental_res)-
                                        ASI::AddrOf((addrbase + 4));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_black_res()
{
    uint32_t addrbase = 0x2b2b60;
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(addrbase), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(++addrbase)) = (int)(&get_figure_statistic_current_black_res)-
                                        ASI::AddrOf((addrbase + 4));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_walk_spd()
{
    uint32_t addrbase = 0x2b2fb0;
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(addrbase), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(++addrbase)) = (int)(&get_figure_statistic_current_walk_spd)-
                                        ASI::AddrOf((addrbase + 4));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_fight_spd()
{
    uint32_t addrbase = 0x2b26d0;
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(addrbase), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(++addrbase)) = (int)(&get_figure_statistic_current_fight_spd)-
                                        ASI::AddrOf((addrbase + 4));
    ASI::EndRewrite(ac_mreg);
}

static void figure_statistic_hook_current_cast_spd()
{
    uint32_t addrbase = 0x2b1b90;
    ASI::MemoryRegion ac_mreg (ASI::AddrOf(addrbase), 7);
    ASI::BeginRewrite(ac_mreg);
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0x90; // nop
    *(unsigned char *)(ASI::AddrOf(++addrbase)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(++addrbase)) = (int)(&get_figure_statistic_current_cast_spd)-
                                        ASI::AddrOf((addrbase + 4));
    ASI::EndRewrite(ac_mreg);
}

void initialize_vanilla_fix_hooks()
{
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
}
