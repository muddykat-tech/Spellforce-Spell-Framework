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
    _this->BattleData.current_source_maybe.entity_type = 1;
}