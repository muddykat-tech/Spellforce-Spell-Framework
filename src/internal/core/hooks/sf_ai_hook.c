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


void __thiscall ai_spell_hook(SF_CGdBattleDevelopment *_this)
{
    SF_SGtFigureAction current_action;
    uint32_t action_rank = 10;

    clearAction(&current_action);
    CGdAIBattleData *battleData = &(_this->BattleData);
    if (battleData->current_figure_has_owner!=0 && (battleData->figure_maybe2).entityCount == 0)
    {
        if (!aiAPI.isAIVectorEmpty(battleData->another_figure_list[battleData->current_figure]))
        {
            uint16_t *first_figure;
            aiAPI.getAIVectorFirstElement(battleData->another_figure_list[battleData->current_figure], &first_figure);
            while (true)
            {
                uint16_t *current_figure;
                aiAPI.getAIVectorGetCurrent(battleData->another_figure_list[battleData->current_figure], &current_figure);
                if (current_figure == first_figure) break;
                aiAPI.AC60AddOrGetEntity(&(battleData->figure_maybe2), *first_figure);
                first_figure++;
            }
        }
    }
}