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

uint16_t __thiscall getSector(SF_CGdWorld *_this, SF_Coord *position)
{
    uint32_t index = position->X + position->Y * 0x400;
    return _this->cells[index].sector;
}

uint32_t GetEuclideanDistanceFromRing(int x1, int y1, int x2, int y2, int inner_radius, int outer_radius)
{
    int iVar1;

    iVar1 = (y1 - y2) * (y1 - y2) + (x1 - x2) * (x1 - x2);
    if (iVar1 < (inner_radius * inner_radius - 1))
    {
        return inner_radius * inner_radius - iVar1;
    }
    if ((outer_radius * outer_radius + 1) < iVar1)
    {
        return iVar1 - outer_radius * outer_radius;
    }
    return 0;
}

uint32_t signum(uint32_t param_1)
{
    return (param_1 ^ (int)param_1 >> 0x1f) - ((int)param_1 >> 0x1f);
}

void updateCurrentAction(SF_CGdBattleDevelopment *_this, SF_SGtFigureAction *action, SF_CGdTargetData *target, uint16_t minRange, uint16_t maxRange)
{
    _this->BattleData.current_target.entity_index = target->entity_index;
    _this->BattleData.current_target.entity_type = target->entity_type;
    _this->BattleData.current_target.position.X = target->position.X;
    _this->BattleData.current_target.position.Y = target->position.Y;

    _this->BattleData.current_action.type = action->type;
    _this->BattleData.current_action.subtype = action->subtype;
    _this->BattleData.current_action.unkn2 = action->unkn2;
    _this->BattleData.current_action.unkn3 = action->unkn3;
    _this->BattleData.current_action.unkn4 = action->unkn4;
    _this->BattleData.current_action.unkn5 = action->unkn5;

    _this->BattleData.current_action_min_rng = minRange;
    _this->BattleData.current_action_max_rng = maxRange;
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
                uint8_t cast_type = aiAPI.getCastType(battleData->CGdResource, current_action.subtype);
                uint16_t targets_length = 0;
                if (cast_type == 2)
                {
                    targets_length = aiAPI.getAIVectorLength(&battleData->some_figure_list[battleData->current_figure]);
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
                    targets_length = aiAPI.getAIVectorLength(&battleData->another_figure_list[battleData->current_figure]);
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
                if (getSector(battleData->CGdWorld, &caster_postion) == getSector(battleData->CGdWorld, &castCoord))
                {
                    uint32_t distance = GetEuclideanDistanceFromRing(battleData->current_figure_pos.X, battleData->current_figure_pos.Y,
                                                                     castCoord.X, castCoord.Y, minRange, maxRange);
                    distance = signum(distance);
                    action_rank = (distance + 1) * action_rank;
                    distance = getDistance(&caster_postion, &castCoord);
                    action_rank = (((uint16_t)distance) / 5 + 1) * action_rank;
                }
                if (cast_type == 2)
                {
                    action_rank *= 2; // FIXME, handle AOE support spells!
                }
                else
                {
                    action_rank *= 2; // FIXME TOO! Handle AOE offensive spells!
                }
                if ((action_rank < targets_length) && action_rank != 0)
                {
                    action_rank = 1;
                }
                else
                {
                    action_rank = action_rank / targets_length;
                }
                uint16_t rand_penalty = spellAPI.getRandom(_this->battleFactory, 5);
                action_rank = ((rand_penalty + 10) * action_rank) / 10;
                if (action_rank == 0)
                {
                    action_rank = 0xffffffff;
                }
                if (action_rank < aiAPI.getAICurrentActionRanking(_this))
                {
                    aiAPI.setAICurrentActionRanking(_this, action_rank);
                    SF_CGdTargetData target = {spell_data.cast_type2,
                                               0,
                                               {castCoord.X, castCoord.Y}};
                    updateCurrentAction(_this, &current_action, &target, minRange, maxRange);
                    battleData->something_related_to_ranking = 0;
                    battleData->current_figure_noaggroattack = no_aggro_attack;
                }
            }
        }

        
    }
}