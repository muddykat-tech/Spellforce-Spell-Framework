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

//same for endurance ability to a T
uint16_t __thiscall durability_dmg_handler(SF_CGdFigureToolbox *_this, uint16_t source, uint16_t target,
                    uint16_t current_damage, uint16_t is_spell_damage, uint32_t is_ranged_damage, uint16_t spell_id)
{
    if ((!is_ranged_damage) &&(!is_spell_damage))
    {
        SF_CGdResourceSpell spell_data;
        spellAPI.getResourceSpellData(_this->CGdResource,&spell_data,spell_id);
        current_damage = (uint16_t)((current_damage*spell_data.params[1])/100);
        return current_damage;
    }
    return current_damage;
}

uint16_t invulnerability_dmg_handler(SF_CGdFigureToolbox *_this, uint16_t source, uint16_t target,
                    uint16_t current_damage, uint16_t is_spell_damage, uint32_t is_ranged_damage, uint16_t spell_id)
{
    return 0;
}

//fire shield, ice shield, thorn shield, white essence and allmight
// 0xc, 0xf, 0x2f, 0xaf, 0xb3
uint16_t dmg_shield_dmg_handler(SF_CGdFigureToolbox *_this, uint16_t source, uint16_t target,
                    uint16_t current_damage, uint16_t is_spell_damage, uint32_t is_ranged_damage, uint16_t spell_id)
{
    if ((!is_ranged_damage) && (!is_spell_damage) && (source !=0))
    {
        SF_CGdResourceSpell spell_data;
        spellAPI.getResourceSpellData(_this->CGdResource,&spell_data,spell_id);
        SF_CGdTargetData source_data = {1, source, {0,0}};
        SF_CGdTargetData target_data = {1, target, {0,0}};
        //Attention -- source and target ARE reversed here
        spellAPI.addSpell(_this->CGdSpell, spell_data.params[1], _this->maybe_random->current_step, &target_data, &source_data, 0);
    }
    return current_damage;
}

uint16_t conservation_dmg_handler(SF_CGdFigureToolbox *_this, uint16_t source, uint16_t target,
                    uint16_t current_damage, uint16_t is_spell_damage, uint32_t is_ranged_damage, uint16_t spell_id)
{
    if (!is_spell_damage)
    {
        uint16_t spell_index = toolboxAPI.getSpellIndexOfType(_this, target, 0x8c, 0);
        uint32_t shield_value = spellAPI.getXData(_this->CGdSpell, spell_index, SPELL_CONSERVATION_SHIELD);
        if (shield_value < current_damage)
        {
            current_damage = current_damage - shield_value;
            shield_value = 0;
        }
        else
        {
            current_damage = 0;
            shield_value = shield_value - current_damage;
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

uint16_t feign_death_dmg_handler(SF_CGdFigureToolbox *_this, uint16_t source, uint16_t target,
                    uint16_t current_damage, uint16_t is_spell_damage, uint32_t is_ranged_damage, uint16_t spell_id)
{
    //need FigureSetNewJob passed through here
    return current_damage;
}

uint16_t feedback_dmg_handler(SF_CGdFigureToolbox *_this, uint16_t source, uint16_t target,
                    uint16_t current_damage, uint16_t is_spell_damage, uint32_t is_ranged_damage, uint16_t spell_id)
{
    return current_damage;
}