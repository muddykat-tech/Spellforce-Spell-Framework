#include "sf_spellend_handlers.h"
#include "../core/sf_hooks.h"
#include "../core/sf_wrappers.h"

// default case
void __thiscall default_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    log_info("Did we get it ?");
    spellAPI.removeDLLNode(_this, spell_index);
    spellAPI.setEffectDone(_this, spell_index, 0);
}

// Stat changing handlers

void __thiscall slowness_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;

    if (toolboxAPI.hasSpellOnHit(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, WALK_SPEED, target_index, spell_data.params[0]);
    }
    // just don't wanna to repeat it every time
    default_end_handler(_this, spell_index);
}
void __thiscall decay1_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;

    if (toolboxAPI.hasSpellOnHit(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, ARMOR, target_index, spell_data.params[2]);
    }
    default_end_handler(_this, spell_index);
}

void __thiscall inflexibility_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;

    if (toolboxAPI.hasSpellOnHit(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, AGILITY, target_index, spell_data.params[0]);
    }
    default_end_handler(_this, spell_index);
}

void __thiscall weaken_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;

    if (toolboxAPI.hasSpellOnHit(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, STRENGTH, target_index, spell_data.params[0]);
    }
    default_end_handler(_this, spell_index);
}

void __thiscall quickness_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;
    int8_t bonus = (int8_t)(-spellAPI.getXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER));

    if (toolboxAPI.hasSpellOnHit(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, WALK_SPEED, target_index, bonus);
    }
    default_end_handler(_this, spell_index);
}

void __thiscall flexibility_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;
    int8_t bonus = (int8_t)(-spellAPI.getXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER));

    if (toolboxAPI.hasSpellOnHit(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, AGILITY, target_index, bonus);
    }
    default_end_handler(_this, spell_index);
}

void __thiscall strength_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;
    int8_t bonus = (int8_t)(-spellAPI.getXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER));

    if (toolboxAPI.hasSpellOnHit(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, STRENGTH, target_index, bonus);
    }
    default_end_handler(_this, spell_index);
}

// same handler for both brilliance and retention
void __thiscall brilliance_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;
    if (toolboxAPI.hasSpellOnHit(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        uint16_t max_mana = figureAPI.getCurrentMaxMana(_this->SF_CGdFigure, target_index);
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, WISDOM, target_index, -spell_data.params[0]);
        toolboxAPI.rescaleLevelStats(_this->SF_CGdFigureToolBox, target_index);
        figureAPI.rescaleMana(_this->SF_CGdFigure, target_index, max_mana);
    }
    default_end_handler(_this, spell_index);
}

void __thiscall suffocation_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;
    if (toolboxAPI.hasSpellOnHit(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        uint16_t max_health = figureAPI.getCurrentMaxHealth(_this->SF_CGdFigure, target_index);
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, STAMINA, target_index, spell_data.params[0]);
        toolboxAPI.rescaleLevelStats(_this->SF_CGdFigureToolBox, target_index);
        figureAPI.rescaleHealth(_this->SF_CGdFigure, target_index, max_health);
    }
    default_end_handler(_this, spell_index);
}

void __thiscall inablility_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall slow_fighting_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall dexterity_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall edurance_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall fast_fighting_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall charisma_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall enlightenment_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall melt_resistance_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall chill_resistance_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall firebane_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall black_almightness_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall mutation_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall eternity_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
