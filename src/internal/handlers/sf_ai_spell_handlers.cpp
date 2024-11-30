#include "sf_ai_spell_handlers.h"

uint32_t __thiscall target_healing_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    // TODO: Implement Me
    return 0;
}

uint32_t __thiscall cure_poison_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    if (!toolboxAPI.hasSpellOnIt((_this->BattleData).CGdFigureToolBox, target_index, kGdSpellLinePoison))
    {
        rank = 0;
    }
    return rank;
}

// Ice, Fire, Thorn shield, Conservation, Dexterity
uint32_t __thiscall shields_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    if (_this->BattleData.figures_maybe2.entityCount == 0)
    {
        rank = 0;
    }
    else
    {
        if (toolboxAPI.isUnitMelee(_this->BattleData.CGdFigureToolBox, target_index))
        {
            rank = rank << 2;
        }
        else
        {
            rank = rank << 1;
        }

        if (figureAPI.getCurrentHealth(_this->BattleData.CGdFigure, target_index) < figureAPI.getCurrentMaxHealth(_this->BattleData.CGdFigure, target_index))
        {
            rank = rank << 1;
        }
    }
    return rank;
}

uint32_t __thiscall cure_disease_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    if (!toolboxAPI.hasSpellOnIt((_this->BattleData).CGdFigureToolBox, target_index, kGdSpellLinePestilence))
    {
        rank = 0;
    }
    return rank;
}

uint32_t __thiscall death_grasp_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    uint16_t current_health = figureAPI.getCurrentHealth(_this->BattleData.CGdFigure, target_index);
    uint32_t max_health = figureAPI.getCurrentMaxHealth(_this->BattleData.CGdFigure, target_index);
    if (max_health != 0)
    {
        uint16_t percent = (current_health / max_health) * 100;
        if (percent < 50)
        {
            rank = rank * 10;
        }
        else
        {
            rank = 0;
        }
    }
    else
    {
        rank = 0;
    }
    return rank;
}