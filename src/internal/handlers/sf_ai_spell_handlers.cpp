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

uint32_t __thiscall shields_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    if (_this->BattleData.figures_maybe2.entityCount == 0)
    {
        rank = 0;
    }
    else
    {
        //pass through isUnitMelee
    }
    return rank;
}