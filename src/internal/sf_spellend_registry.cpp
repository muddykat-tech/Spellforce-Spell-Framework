#include "sf_spellend_registry.h"
#include "sf_hooks.h"

// default case
void __thiscall default_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    apiSpellFunctions.removeDLLNode(_this, spell_index);
    apiSpellFunctions.setEffectDone(_this, spell_index, 0);
}

// Stat changing handlers

void __thiscall slowness_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    apiSpellFunctions.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;

    if (apiToolboxFunctions.hasSpellOnHit(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        apiFigureFunctions.addBonusMultToStatistic(_this->SF_CGdFigure, WALK_SPEED, target_index, spell_data.params[0]);
    }
    // just don't wanna to repeat it every time
    default_end_handler(_this, spell_index);
}
void __thiscall decay1_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    apiSpellFunctions.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;

    if (!apiToolboxFunctions.hasSpellOnHit(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        apiFigureFunctions.addBonusMultToStatistic(_this->SF_CGdFigure, ARMOR, target_index, spell_data.params[2]);
    }
    default_end_handler(_this, spell_index);
}

void __thiscall inflexibility_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    apiSpellFunctions.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;

    if (!apiToolboxFunctions.hasSpellOnHit(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        apiFigureFunctions.addBonusMultToStatistic(_this->SF_CGdFigure, AGILITY, target_index, spell_data.params[0]);
    }
    default_end_handler(_this, spell_index);
}

void __thiscall weaken_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    apiSpellFunctions.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;

    if (!apiToolboxFunctions.hasSpellOnHit(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        apiFigureFunctions.addBonusMultToStatistic(_this->SF_CGdFigure, STRENGTH, target_index, spell_data.params[0]);
    }
    default_end_handler(_this, spell_index);
}

void __thiscall quickness_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    apiSpellFunctions.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;
    int8_t bonus = (int8_t)(-apiSpellFunctions.getXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER));

    if (!apiToolboxFunctions.hasSpellOnHit(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        apiFigureFunctions.addBonusMultToStatistic(_this->SF_CGdFigure, WALK_SPEED, target_index, bonus);
    }
    default_end_handler(_this, spell_index);
}

void __thiscall flexibility_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    apiSpellFunctions.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;
    int8_t bonus = (int8_t)(-apiSpellFunctions.getXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER));

    if (!apiToolboxFunctions.hasSpellOnHit(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        apiFigureFunctions.addBonusMultToStatistic(_this->SF_CGdFigure, AGILITY, target_index, bonus);
    }
    default_end_handler(_this, spell_index);
}

void __thiscall strength_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    apiSpellFunctions.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;
    int8_t bonus = (int8_t)(-apiSpellFunctions.getXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER));

    if (!apiToolboxFunctions.hasSpellOnHit(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        apiFigureFunctions.addBonusMultToStatistic(_this->SF_CGdFigure, STRENGTH, target_index, bonus);
    }
    default_end_handler(_this, spell_index);
}

void __thiscall retention_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall brilliance_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall suffocation_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
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
