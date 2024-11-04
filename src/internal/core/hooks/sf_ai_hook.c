#include "sf_ai_hook.h"

void clearAction(SF_SGtFigureAction *_this)
{
    _this->type = 0xffff;
    _this->subtype = 0;
    _this->unkn2 = 0;
    _this->unkn3 = 0;
    _this->unkn4 = 0;
    _this->unkn5 = 0;
}
// TODO MOVE ME

bool isSpellAction(SF_SGtFigureAction *_this)
{
    if ((_this->type != 0) && (_this->type < 10000))
    {
        return true;
    }
    return false;
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
        if (!aiAPI.isAIVectorEmpty(&battleData->another_figure_list[battleData->current_figure]))
        {
            uint16_t *first_figure;
            aiAPI.getAIVectorFirstElement(&battleData->another_figure_list[battleData->current_figure], &first_figure);
            while (true)
            {
                uint16_t *current_figure;
                aiAPI.getAIVectorGetCurrent(&battleData->another_figure_list[battleData->current_figure], &current_figure);
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
        SF_CGdResourceSpell spell_data;
        aiAPI.getFigureAction(battleData->CGdFigure, &current_action, battleData->current_figure, action_index);
        if (current_action.type == (uint16_t)(-1))
            break; // no more actions available

        uint16_t spell_line_id = current_action.type;
        if (spellAPI.hasSpellTag(spell_line_id, SpellTag::SEIGE_AURA_SPELL))
        {
            battleData->action_is_siege_aura = 1;
        }
        if (!aiAPI.canFigureDoAction(_this, &current_action))
        {
            continue;
        }
        uint16_t minRange = 1;
        uint16_t maxRange = 1;
        aiAPI.getActionStats(_this, &minRange, &maxRange, &spell_data);
        if ((!battleData->action_is_siege_aura) && (isSpellAction(&current_action)))
        {
            if ((spellAPI.hasSpellTag(spell_line_id, SpellTag::AOE_SPELL)) &&
                (spellAPI.hasSpellTag(spell_line_id, SpellTag::COMBAT_ABILITY_SPELL)))
            {
                action_rank = 1;
                SF_Coord castCoord = {0, 0};
                if (aiAPI.getCastType(battleData->CGdResource, current_action.subtype) == 2)
                {
                    uint16_t targets_length = aiAPI.getAIVectorLength(&battleData->some_figure_list[battleData->current_figure]);
                    if (targets_length > 4)
                    {
                        action_rank *= 5;
                        aiAPI.getPositionToCastAlly(_this, &castCoord, &battleData->some_figure_list[battleData->current_figure]);
                    }
                    else
                    {
                        continue;
                    }
                }
                else
                {
                    uint16_t targets_length = aiAPI.getAIVectorLength(&battleData->another_figure_list[battleData->current_figure]);
                    if (targets_length > 4)
                    {
                        action_rank *= 10;
                        aiAPI.getPositionToCastEnemy(_this, &castCoord, &battleData->another_figure_list[battleData->current_figure]);
                    }
                    else
                    {
                        continue;
                    }
                }
                SF_Coord caster_postion = {battleData->current_figure_pos.X, battleData->current_figure_pos.Y};

            }
        }
    }
}