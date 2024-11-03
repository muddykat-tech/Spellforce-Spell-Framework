#include "sf_ai_hook.h"

void clearAction(SF_SGtFigureAction *_this)
{
    _this->type = 0xffff;
    _this->unkn1 = 0;
    _this->unkn2 = 0;
    _this->unkn3 = 0;
    _this->unkn4 = 0;
    _this->unkn5 = 0;
}

bool isSiegeAura(SF_SGtFigureAction *action)
{
    uint16_t type = action->type;
    if (type == kGdSpellLineAuraSiegeDarkElf)
    {
        return 1;
    }
    if (type == kGdSpellLineAuraSiegeHuman)
    {
        return 1;
    }
    if (type == kGdSpellLineAuraSiegeElf)
    {
        return 1;
    }
    if (type == kGdSpellLineAuraSiegeTroll)
    {
        return 1;
    }
    if (type == kGdSpellLineAuraSiegeOrc)
    {
        return 1;
    }
    return 0;
}

void __thiscall ai_spell_hook(SF_CGdBattleDevelopment *_this)
{
    SF_SGtFigureAction current_action;
    uint32_t action_rank = 10;
    uint32_t no_aggro_attack = 0;
    clearAction(&current_action);
    CGdAIBattleData *battleData = &(_this->BattleData);
    if (battleData->current_figure_has_owner != 0 && (battleData->figure_maybe2).entityCount == 0)
    {
        if (!aiAPI.isAIVectorEmpty(battleData->another_figure_list[battleData->current_figure]))
        {
            uint16_t *first_figure;
            aiAPI.getAIVectorFirstElement(battleData->another_figure_list[battleData->current_figure], &first_figure);
            while (true)
            {
                uint16_t *current_figure;
                aiAPI.getAIVectorGetCurrent(battleData->another_figure_list[battleData->current_figure], &current_figure);
                if (current_figure == first_figure)
                    break;
                aiAPI.AC60AddOrGetEntity(&(battleData->figure_maybe2), *first_figure);
                first_figure++;
            }
            no_aggro_attack = 1;
        }
    }
    for (uint8_t action_index = 0; action_index < 0xf; action_index++)
    {
        aiAPI.getFigureAction(battleData->CGdFigure, &current_action, battleData->current_figure, action_index);
        if (current_action.type == (uint16_t)(-1))
            break; // no more actions available
        if (isSiegeAura(&current_action))
        {
            battleData->action_is_siege_aura = 1;
        }
    }
}