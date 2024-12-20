#include "sf_spellend_handlers.h"
#include "../core/sf_hooks.h"
#include "../core/sf_wrappers.h"

// default case
void __thiscall default_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    spellAPI.removeDLLNode(_this, spell_index);
    spellAPI.setEffectDone(_this, spell_index, 0);
}

// Stat changing handlers
void __thiscall slowness_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;

    if (toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
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

    if (toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
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

    if (toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
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

    if (toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
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

    if (toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
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

    if (toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
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

    if (toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
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
    if (toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
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
    if (toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        uint16_t max_health = figureAPI.getCurrentMaxHealth(_this->SF_CGdFigure, target_index);
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, STAMINA, target_index, spell_data.params[0]);
        toolboxAPI.rescaleLevelStats(_this->SF_CGdFigureToolBox, target_index);
        figureAPI.rescaleHealth(_this->SF_CGdFigure, target_index, max_health);
    }
    default_end_handler(_this, spell_index);
}

void __thiscall inablility_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;
    if (toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, DEXTERITY, target_index, spell_data.params[0]);
    }
    default_end_handler(_this, spell_index);
}

void __thiscall slow_fighting_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;
    if (toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, FIGHT_SPEED, target_index, spell_data.params[0]);
    }
    default_end_handler(_this, spell_index);
}

void __thiscall dexterity_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;
    if (toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        int8_t bonus = (int8_t)(-spellAPI.getXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER));
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, DEXTERITY, target_index, bonus);
    }
    default_end_handler(_this, spell_index);
}

void __thiscall edurance_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;
    if (toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        uint16_t max_health = figureAPI.getCurrentMaxHealth(_this->SF_CGdFigure, target_index);
        int8_t bonus = (int8_t)(-spellAPI.getXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER));
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, STAMINA, target_index, bonus);
        toolboxAPI.rescaleLevelStats(_this->SF_CGdFigureToolBox, target_index);
        figureAPI.rescaleHealth(_this->SF_CGdFigure, target_index, max_health);
    }
    default_end_handler(_this, spell_index);
}

void __thiscall fast_fighting_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;
    if (toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, FIGHT_SPEED, target_index, -spell_data.params[0]);
    }
    default_end_handler(_this, spell_index);
}

void __thiscall charisma_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;
    if (toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, CHARISMA, target_index, -spell_data.params[0]);
    }
    default_end_handler(_this, spell_index);
}

void __thiscall enlightenment_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;
    if (toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        uint16_t max_mana = figureAPI.getCurrentMaxMana(_this->SF_CGdFigure, target_index);
        int8_t bonus = (int8_t)(-spellAPI.getXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER));
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, INTELLIGENCE, target_index, bonus);
        bonus = (int8_t)(-spellAPI.getXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER2));
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, WISDOM, target_index, bonus);
        toolboxAPI.rescaleLevelStats(_this->SF_CGdFigureToolBox, target_index);
        figureAPI.rescaleMana(_this->SF_CGdFigure, target_index, max_mana);
    }
    default_end_handler(_this, spell_index);
}
void __thiscall melt_resistance_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;
    if (toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, RESISTANCE_ICE, target_index, spell_data.params[0]);
    }
    default_end_handler(_this, spell_index);
}

void __thiscall chill_resistance_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;
    if (toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, RESISTANCE_FIRE, target_index, spell_data.params[0]);
    }
    default_end_handler(_this, spell_index);
}

void __thiscall black_almightness_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;
    if (toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, FIGHT_SPEED, target_index, spell_data.params[3]);
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, AGILITY, target_index, spell_data.params[2]);
    }
    default_end_handler(_this, spell_index);
}

void __thiscall white_almightness_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;
    if (toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, FIGHT_SPEED, target_index, -spell_data.params[3]);
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, DEXTERITY, target_index, -spell_data.params[2]);
    }
    default_end_handler(_this, spell_index);
}

void __thiscall mutation_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;
    if (toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        int8_t bonus = spellAPI.getXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER);
        uint16_t stat_value = _this->SF_CGdFigure->figures[target_index].resistance_fire.base_val;
        int8_t recalc_value = stat_value - ((bonus * stat_value) / 100);
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, RESISTANCE_FIRE, target_index, recalc_value);

        stat_value = _this->SF_CGdFigure->figures[target_index].fight_speed.base_val;
        recalc_value = stat_value - ((bonus * stat_value) / 100);
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, FIGHT_SPEED, target_index, recalc_value);

        stat_value = _this->SF_CGdFigure->figures[target_index].intelligence.base_val;
        recalc_value = stat_value - ((bonus * stat_value) / 100);
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, INTELLIGENCE, target_index, recalc_value);

        stat_value = _this->SF_CGdFigure->figures[target_index].stamina.base_val;
        recalc_value = stat_value - ((bonus * stat_value) / 100);
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, STAMINA, target_index, recalc_value);

        bonus = spellAPI.getXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER2);
        stat_value = _this->SF_CGdFigure->figures[target_index].resistance_ice.base_val;
        recalc_value = stat_value - ((bonus * stat_value) / 100);
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, RESISTANCE_ICE, target_index, recalc_value);

        stat_value = _this->SF_CGdFigure->figures[target_index].cast_speed.base_val;
        recalc_value = stat_value - ((bonus * stat_value) / 100);
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, CAST_SPEED, target_index, recalc_value);

        stat_value = _this->SF_CGdFigure->figures[target_index].wisdom.base_val;
        recalc_value = stat_value - ((bonus * stat_value) / 100);
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, WISDOM, target_index, recalc_value);

        bonus = spellAPI.getXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER3);
        stat_value = _this->SF_CGdFigure->figures[target_index].resistance_black.base_val;
        recalc_value = stat_value - ((bonus * stat_value) / 100);
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, RESISTANCE_BLACK, target_index, recalc_value);

        stat_value = _this->SF_CGdFigure->figures[target_index].walk_speed.base_val;
        recalc_value = stat_value - ((bonus * stat_value) / 100);
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, WALK_SPEED, target_index, recalc_value);

        stat_value = _this->SF_CGdFigure->figures[target_index].charisma.base_val;
        recalc_value = stat_value - ((bonus * stat_value) / 100);
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, CHARISMA, target_index, recalc_value);

        stat_value = _this->SF_CGdFigure->figures[target_index].dexterity.base_val;
        recalc_value = stat_value - ((bonus * stat_value) / 100);
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, DEXTERITY, target_index, recalc_value);

        bonus = spellAPI.getXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER4);
        stat_value = _this->SF_CGdFigure->figures[target_index].resistance_mental.base_val;
        recalc_value = stat_value - ((bonus * stat_value) / 100);
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, RESISTANCE_MENTAL, target_index, recalc_value);

        stat_value = _this->SF_CGdFigure->figures[target_index].strength.base_val;
        recalc_value = stat_value - ((bonus * stat_value) / 100);
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, STRENGTH, target_index, recalc_value);

        stat_value = _this->SF_CGdFigure->figures[target_index].agility.base_val;
        recalc_value = stat_value - ((bonus * stat_value) / 100);
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, AGILITY, target_index, recalc_value);
    }
    default_end_handler(_this, spell_index);
}

void __thiscall eternity_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;
    if (toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        int8_t bonus = (int8_t)(-spellAPI.getXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER));
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, DEXTERITY, target_index, bonus);
        bonus = (int8_t)(-spellAPI.getXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER2));
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, AGILITY, target_index, bonus);
        bonus = (int8_t)(-spellAPI.getXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER3));
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, FIGHT_SPEED, target_index, bonus);
        bonus = (int8_t)(-spellAPI.getXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER4));
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, WALK_SPEED, target_index, bonus);
    }
    default_end_handler(_this, spell_index);
}

// Muddykat Section:

// FIRST BLOCK (has a goto, to FigureClearCheckSpellsBeforeCheckBattle; -> break;)
void __thiscall common_handler_check_battle(SF_CGdSpell *_this, uint16_t spell_index)
{
    spellClearFigureFlag(_this, spell_index, CHECK_SPELLS_BEFORE_CHECK_BATTLE);
    default_end_handler(_this, spell_index);
}

void __thiscall common_handler_unfreeze(SF_CGdSpell *_this, uint16_t spell_index)
{
    spellClearFigureFlag(_this, spell_index, UNFREEZE);
    default_end_handler(_this, spell_index);
}

void __thiscall common_handler_check_unfreeze(SF_CGdSpell *_this, uint16_t spell_index)
{
    spellClearFigureFlag(_this, spell_index, CHECK_SPELLS_BEFORE_CHECK_BATTLE);
    spellClearFigureFlag(_this, spell_index, UNFREEZE);
    default_end_handler(_this, spell_index);
}

void __thiscall common_handler_check_unfreeze2(SF_CGdSpell *_this, uint16_t spell_index)
{
    spellClearFigureFlag(_this, spell_index, UNFREEZE);
    spellClearFigureFlag(_this, spell_index, CHECK_SPELLS_BEFORE_CHECK_BATTLE);
    default_end_handler(_this, spell_index);
}

// THIRD BLOCK FigureClearCheckSpellsBeforeCheckBattle -> Done -> Return;
void __thiscall fog_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    spellClearFigureFlag(_this, spell_index, CHECK_SPELLS_BEFORE_CHECK_BATTLE);
    spellAPI.setEffectDone(_this, spell_index, 0);
};

// FOURTH BLOCK FigureTryClearCheckSpellsBeforeJob2 -> goto -> FigureClearCheckSpellsBeforeCheckBattle -> break;
void __thiscall common_handler_job_battle_check(SF_CGdSpell *_this, uint16_t spell_index)
{
    spellClearFigureFlag(_this, spell_index, CHECK_SPELLS_BEFORE_JOB2);
    spellClearFigureFlag(_this, spell_index, CHECK_SPELLS_BEFORE_CHECK_BATTLE);
    default_end_handler(_this, spell_index);
}

void __thiscall common_handler_done(SF_CGdSpell *_this, uint16_t spell_index)
{
    spellAPI.setEffectDone(_this, spell_index, 1);
}

// Not 100% sure if this is neccessary, however the order of operations may be important
void __thiscall common_handler_job_battle_check2(SF_CGdSpell *_this, uint16_t spell_index)
{
    spellClearFigureFlag(_this, spell_index, CHECK_SPELLS_BEFORE_CHECK_BATTLE);
    spellClearFigureFlag(_this, spell_index, CHECK_SPELLS_BEFORE_JOB2);
    default_end_handler(_this, spell_index);
}

void __thiscall clay_feet_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index].spell_id);
    uint16_t target_index = _this->active_spell_list[spell_index].target.entity_index;
    if (toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, spell_data.spell_line_id))
    {
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, FIGHT_SPEED, target_index, spell_data.params[1]);
        figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, CAST_SPEED, target_index, spell_data.params[1]);
    }
    spellClearFigureFlag(_this, spell_index, CHECK_SPELLS_BEFORE_JOB2);
    spellClearFigureFlag(_this, spell_index, CHECK_SPELLS_BEFORE_CHECK_BATTLE);
    spellAPI.setEffectDone(_this, spell_index, 0);
}

void __thiscall aura_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdFigure *sf_figures = _this->SF_CGdFigure;
    uint16_t source_index = _this->active_spell_list[spell_index].source.entity_index;
    if ((sf_figures->figures[source_index].owner != (uint16_t)(-1)) &&
        ((sf_figures->figures[source_index].flags & GdFigureFlags::REDO) == 0))
    {
        sf_figures->figures[source_index].flags = static_cast<GdFigureFlags>(sf_figures->figures[source_index].flags & (~static_cast<unsigned int>(GdFigureFlags::AURA_RUNNING)));
    }
    uint16_t effect_index = spellAPI.getXData(_this, spell_index, EFFECT_EFFECT_INDEX);
    if (effect_index != 0)
    {
        effectAPI.tryEndEffect(_this->SF_CGdEffect, effect_index);
    }
    default_end_handler(_this, spell_index);
}
