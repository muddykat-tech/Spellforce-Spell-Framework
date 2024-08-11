#include "sf_spelleffect_handlers.h"
#include "../core/sf_wrappers.h"
#include "sf_spelldamage_handlers.h"
#include "../core/sf_hooks.h"

uint16_t __thiscall steelskin_dmg_handler(SF_CGdFigureToolbox *_this, uint16_t source, uint16_t target,
                                          uint16_t current_damage, uint16_t is_spell_damage, uint32_t is_ranged_damage, uint16_t spell_id)
{
    if (is_ranged_damage)
    {
        return 0;
    }
    return current_damage;
}

// same for endurance ability to a T
uint16_t __thiscall durability_dmg_handler(SF_CGdFigureToolbox *_this, uint16_t source, uint16_t target,
                                           uint16_t current_damage, uint16_t is_spell_damage, uint32_t is_ranged_damage, uint16_t spell_id)
{
    if ((!is_ranged_damage) && (!is_spell_damage))
    {
        SF_CGdResourceSpell spell_data;
        spellAPI.getResourceSpellData(_this->CGdResource, &spell_data, spell_id);
        current_damage = (uint16_t)((current_damage * spell_data.params[1]) / 100);
        return current_damage;
    }
    return current_damage;
}

uint16_t invulnerability_dmg_handler(SF_CGdFigureToolbox *_this, uint16_t source, uint16_t target,
                                     uint16_t current_damage, uint16_t is_spell_damage, uint32_t is_ranged_damage, uint16_t spell_id)
{
    return 0;
}

// fire shield, ice shield, thorn shield, white essence and allmight
//  0xc, 0xf, 0x2f, 0xaf, 0xb3
uint16_t __thiscall dmg_shield_dmg_handler(SF_CGdFigureToolbox *_this, uint16_t source, uint16_t target,
                                           uint16_t current_damage, uint16_t is_spell_damage, uint32_t is_ranged_damage, uint16_t spell_id)
{
    if ((!is_ranged_damage) && (!is_spell_damage) && (source != 0))
    {
        SF_CGdResourceSpell spell_data;
        spellAPI.getResourceSpellData(_this->CGdResource, &spell_data, spell_id);
        SF_CGdTargetData source_data = {1, source, {0, 0}};
        SF_CGdTargetData target_data = {1, target, {0, 0}};
        // Attention -- source and target ARE reversed here
        spellAPI.addSpell(_this->CGdSpell, spell_data.params[1], _this->maybe_random->current_step, &target_data, &source_data, 0);
    }
    return current_damage;
}

uint16_t __thiscall conservation_dmg_handler(SF_CGdFigureToolbox *_this, uint16_t source, uint16_t target,
                                             uint16_t current_damage, uint16_t is_spell_damage, uint32_t is_ranged_damage, uint16_t spell_id)
{
    if (!is_spell_damage)
    {
        uint16_t spell_index = toolboxAPI.getSpellIndexOfType(_this, target, 0x8c, 0);
        uint16_t shield_value = (uint16_t)spellAPI.getXData(_this->CGdSpell, spell_index, SPELL_CONSERVATION_SHIELD);
        if (shield_value < current_damage)
        {
            current_damage = current_damage - shield_value;
            shield_value = 0;
        }
        else
        {
            shield_value = shield_value - current_damage;
            current_damage = 0;
        }
        spellAPI.setXData(_this->CGdSpell, spell_index, SPELL_CONSERVATION_SHIELD, shield_value);
        if (shield_value == 0)
        {
            spellAPI.figTryClrCHkSPlBfrJob2(_this->CGdSpell, spell_index);
            spellAPI.figClrChkSplBfrChkBattle(_this->CGdSpell, spell_index, 0);
            spellAPI.removeDLLNode(_this->CGdSpell, spell_index);
            spellAPI.setEffectDone(_this->CGdSpell, spell_index, 0);
        }
    }
    return current_damage;
}

uint16_t __thiscall feign_death_dmg_handler(SF_CGdFigureToolbox *_this, uint16_t source, uint16_t target,
                                            uint16_t current_damage, uint16_t is_spell_damage, uint32_t is_ranged_damage, uint16_t spell_id)
{
    toolboxAPI.figureSetNewJob(_this->CGdFigureJobs, target, 3, 1, 0, 0);
    return current_damage;
}

uint16_t __thiscall feedback_dmg_handler(SF_CGdFigureToolbox *_this, uint16_t source, uint16_t target,
                                         uint16_t current_damage, uint16_t is_spell_damage, uint32_t is_ranged_damage, uint16_t spell_id)
{
    if (is_spell_damage)
    {
        SF_CGdResourceSpell spell_data;
        spellAPI.getResourceSpellData(_this->CGdResource, &spell_data, spell_id);
        uint16_t reflect_amount = (uint16_t)((current_damage * spell_data.params[0]) / 100);
        if (current_damage < reflect_amount)
        {
            current_damage = 0;
            reflect_amount = current_damage;
        }
        else
        {
            current_damage = current_damage - reflect_amount;
        }
        toolboxAPI.dealDamage(_this, target, source, reflect_amount, 1, 0, 0);
        SF_CGdTargetData source_data = {1, source, {0, 0}};
        SF_CGdTargetData target_data = {1, target, {0, 0}};
        SF_Rectangle rect = {0, 0};
        effectAPI.addEffect(_this->CGdEffect, kGdEffectSpellVoodooHitFigure,
                            &source_data, &target_data, _this->maybe_random->current_step, 10, &rect);
    }
    return current_damage;
}

uint16_t __thiscall death_grasp_dmg_handler(SF_CGdFigureToolbox *_this, uint16_t source, uint16_t target,
                                            uint16_t current_damage, uint16_t is_spell_damage, uint32_t is_ranged_damage, uint16_t spell_id)
{
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->CGdResource, &spell_data, spell_id);
    uint16_t roll_value = spellAPI.getRandom(_this->maybe_random, 100);
    uint16_t current_health = figureAPI.getCurrentHealth(_this->CGdFigure, target);
    if (current_health <= current_damage)
    {
        if (roll_value <= spell_data.params[2])
        {
            current_damage = current_health - 1;
        }
    }
    return current_damage;
}

uint16_t __thiscall hypnotize_dmg_handler(SF_CGdFigureToolbox *_this, uint16_t source, uint16_t target,
                                          uint16_t current_damage, uint16_t is_spell_damage, uint32_t is_ranged_damage, uint16_t spell_id)
{
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->CGdResource, &spell_data, spell_id);
    uint16_t spell_index = toolboxAPI.getSpellIndexOfType(_this, target, spell_data.spell_line_id, 0);
    while (spell_index != 0)
    {
        spellAPI.figTryUnfreeze(_this->CGdSpell, spell_index, 0);
        spellAPI.figTryClrCHkSPlBfrJob2(_this->CGdSpell, spell_index);
        spellAPI.figClrChkSplBfrChkBattle(_this->CGdSpell, spell_index, 0);
        spellAPI.removeDLLNode(_this->CGdSpell, spell_index);
        spellAPI.setEffectDone(_this->CGdSpell, spell_index, 0);
        spell_index = toolboxAPI.getSpellIndexOfType(_this, target, spell_data.spell_line_id, 0);
    }
    return current_damage;
}

// Need passthough for figureAPI GetCurrentMana, SubMana here.
uint16_t __thiscall mana_shield_dmg_handler(SF_CGdFigureToolbox *_this, uint16_t source, uint16_t target,
                                            uint16_t current_damage, uint16_t is_spell_damage, uint32_t is_ranged_damage, uint16_t spell_id)
{
    uint16_t mana_left = figureAPI.getManaCurrent(_this->CGdFigure, target);
    if (mana_left < current_damage)
    {
        current_damage = current_damage - mana_left;
        figureAPI.subMana(_this->CGdFigure, target, mana_left);
    }
    else
    {
        figureAPI.subMana(_this->CGdFigure, target, current_damage);
        current_damage = 0;
    }
    SF_CGdTargetData source_data = {0, 0, {0, 0}};
    SF_CGdTargetData target_data = {1, target, {0, 0}};
    SF_Rectangle rect = {0, 0};
    effectAPI.addEffect(_this->CGdEffect, kGdEffectSpellManaShieldHitFigure,
                        &source_data, &target_data, _this->maybe_random->current_step, 0x14, &rect);
    return current_damage;
}
