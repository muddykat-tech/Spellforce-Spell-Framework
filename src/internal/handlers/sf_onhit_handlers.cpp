#include "sf_onhit_handlers.h"
#include "../core/sf_wrappers.h"
#include "../core/sf_hooks.h"
#include <list>

uint16_t __thiscall trueshot_onhit_handler(SF_CGdFigureJobs *_this, uint16_t source_index, uint16_t target_index, uint16_t damage)
{    SF_SGtFigureAction action;
    figureAPI.getTargetAction(_this->CGdFigure, &action, source_index);

    if (isActionMelee(&action))
    {
        return damage;
    }

    uint16_t spell_index_of_type = toolboxAPI.getSpellIndexOfType(_this->CGdFigureToolBox, source_index, kGdSpellLineAbilityTrueShot, 0);
    SF_CGdResourceSpell spell_data;
    uint16_t spell_id = spellAPI.getSpellID(_this->CGdSpell, spell_index_of_type);
    spellAPI.getResourceSpellData(_this->CGdResource, &spell_data, spell_id);
    uint16_t current_damage = (spell_data.params[1] * damage) / 100;
    return current_damage;
}

uint16_t __thiscall riposte_onhit_handler(SF_CGdFigureJobs *_this, uint16_t source_index, uint16_t target_index, uint16_t damage)
{
    SF_SGtFigureAction action;
    figureAPI.getTargetAction(_this->CGdFigure, &action, source_index);
    bool isMeleeAttack = isActionMelee(&action);

    if (!isMeleeAttack)
    {
        return damage;
    }

    SF_CGdResourceSpell spell_data;
    uint16_t spell_index = toolboxAPI.getSpellIndexOfType(_this->CGdFigureToolBox, target_index, kGdSpellLineAbilityRiposte, 0);
    uint16_t spell_id = spellAPI.getSpellID(_this->CGdSpell, spell_index);
    spellAPI.getResourceSpellData(_this->CGdResource, &spell_data, spell_id);

    uint16_t return_damage = (damage * spell_data.params[0]) / 100;

    // Lower the damage by the amount that will be reflected
    uint16_t final_damage = (damage - return_damage);
    if (final_damage != 0)
    {
        toolboxAPI.dealDamage(_this->CGdFigureToolBox, target_index, source_index, return_damage, 0, 0, 0);
    }

    return final_damage;
}

uint16_t __thiscall durability_onhit_handler(SF_CGdFigureJobs *_this, uint16_t source_index, uint16_t target_index, uint16_t damage)
{
    SF_SGtFigureAction action;
    figureAPI.getTargetAction(_this->CGdFigure, &action, source_index);
    bool isMeleeAttack = isActionMelee(&action);

    if (!isMeleeAttack)
    {
        return damage;
    }

    SF_CGdResourceSpell spell_data;
    uint16_t spell_index = toolboxAPI.getSpellIndexOfType(_this->CGdFigureToolBox, source_index, kGdSpellLineAbilityDurability, 0);
    uint16_t spell_id = spellAPI.getSpellID(_this->CGdSpell, spell_index);
    spellAPI.getResourceSpellData(_this->CGdResource, &spell_data, spell_id);
    return (damage * spell_data.params[1]) / 100;
}

uint16_t __thiscall critical_hits_onhit_handler(SF_CGdFigureJobs *_this, uint16_t source_index, uint16_t target_index, uint16_t damage)
{
    // no need to calculate damage in this cases
    if (damage == 0)
    {
        return damage;
    }
    uint16_t target_level = _this->CGdFigure->figures[target_index].level;
    uint16_t source_level = _this->CGdFigure->figures[source_index].level;
    // not proccing on targets 5+ levels above source
    if (source_level + 5 <= target_level)
    {
        return damage;
    }
    // outright killing targets 15+ levels below
    if (source_level - 0xf > target_level)
    {
        return 0x7fff;
    }
    SF_CGdResourceSpell spell_data;
    uint16_t spell_index = toolboxAPI.getSpellIndexOfType(_this->CGdFigureToolBox, spell_index, kGdSpellLineAbilityCriticalHits, 0);
    uint16_t spell_id = spellAPI.getSpellID(_this->CGdSpell, spell_index);
    spellAPI.getResourceSpellData(_this->CGdResource, &spell_data, spell_id);

    uint16_t tmp_damage = damage * spell_data.params[1];
    uint16_t hp_left = figureAPI.getCurrentHealth(_this->CGdFigure, target_index);
    if (tmp_damage < hp_left)
    {
        return damage;
    }

    tmp_damage = ((tmp_damage - hp_left) * 100) / tmp_damage;
    uint16_t chance = 5 * tmp_damage * (5 + (int16_t)(source_level - target_level));
    if (spellAPI.getRandom(_this->OpaqueClass, 10000) > chance)
    {
        return damage;
    }
    return 0x7fff;
}

uint16_t __thiscall endurance_onhit_handler(SF_CGdFigureJobs *_this, uint16_t source_index, uint16_t target_index, uint16_t damage)
{
    SF_SGtFigureAction action;
    figureAPI.getTargetAction(_this->CGdFigure, &action, source_index);
    bool isMeleeAttack = isActionMelee(&action);

    if (!isMeleeAttack)
    {
        return damage;
    }
    if (toolboxAPI.hasSpellOnIt(_this->CGdFigureToolBox, source_index, kGdSpellLineAbilityDurability))
    {
        return damage;
    }

    SF_CGdResourceSpell spell_data;
    uint16_t spell_index = toolboxAPI.getSpellIndexOfType(_this->CGdFigureToolBox, source_index, kGdSpellLineAbilityEndurance, 0);
    uint16_t spell_id = spellAPI.getSpellID(_this->CGdSpell, spell_index);
    spellAPI.getResourceSpellData(_this->CGdResource, &spell_data, spell_id);
    return (damage * spell_data.params[1]) / 100;
}

uint16_t __thiscall berserk_onhit_handler(SF_CGdFigureJobs *_this, uint16_t source_index, uint16_t target_index, uint16_t damage)
{
    SF_SGtFigureAction action;
    figureAPI.getTargetAction(_this->CGdFigure, &action, source_index);
    bool isMeleeAttack = isActionMelee(&action);

    if (!isMeleeAttack)
    {
        return damage;
    }
    if (toolboxAPI.hasSpellOnIt(_this->CGdFigureToolBox, source_index, kGdSpellLineAbilityEndurance))
    {
        return damage;
    }
    if (toolboxAPI.hasSpellOnIt(_this->CGdFigureToolBox, source_index, kGdSpellLineAbilityDurability))
    {
        return damage;
    }
    SF_CGdResourceSpell spell_data;
    uint16_t spell_index = toolboxAPI.getSpellIndexOfType(_this->CGdFigureToolBox, source_index, kGdSpellLineAbilityBerserk, 0);
    uint16_t spell_id = spellAPI.getSpellID(_this->CGdSpell, spell_index);
    spellAPI.getResourceSpellData(_this->CGdResource, &spell_data, spell_id);
    return (damage * spell_data.params[1]) / 100;
}

uint16_t __thiscall warcry_onhit_handler(SF_CGdFigureJobs *_this, uint16_t source_index, uint16_t target_index, uint16_t damage)
{
    SF_SGtFigureAction action;
    figureAPI.getTargetAction(_this->CGdFigure, &action, source_index);
    bool isMeleeAttack = isActionMelee(&action);

    if (!isMeleeAttack)
    {
        return damage;
    }
    if (toolboxAPI.hasSpellOnIt(_this->CGdFigureToolBox, source_index, kGdSpellLineAbilityEndurance))
    {
        return damage;
    }
    if (toolboxAPI.hasSpellOnIt(_this->CGdFigureToolBox, source_index, kGdSpellLineAbilityDurability))
    {
        return damage;
    }
    if (toolboxAPI.hasSpellOnIt(_this->CGdFigureToolBox, source_index, kGdSpellLineAbilityBerserk))
    {
        return damage;
    }
    SF_CGdResourceSpell spell_data;
    uint16_t spell_index = toolboxAPI.getSpellIndexOfType(_this->CGdFigureToolBox, source_index, kGdSpellLineAbilityWarCry, 0);
    uint16_t spell_id = spellAPI.getSpellID(_this->CGdSpell, spell_index);
    spellAPI.getResourceSpellData(_this->CGdResource, &spell_data, spell_id);
    return (damage * spell_data.params[1]) / 100;
}

uint16_t __thiscall assistance_onhit_handler(SF_CGdFigureJobs *_this, uint16_t source_index, uint16_t target_index, uint16_t damage)
{
    // oneshot or zero damage
    if (damage == 0x7fff || damage == 0)
    {
        return damage;
    }
    SF_CGdResourceSpell spell_data;
    uint16_t spell_index = toolboxAPI.getSpellIndexOfType(_this->CGdFigureToolBox, target_index, kGdSpellLineAssistance, 0);
    uint16_t spell_id = spellAPI.getSpellID(_this->CGdSpell, spell_index);
    spellAPI.getResourceSpellData(_this->CGdResource, &spell_data, spell_id);
    CGdFigureIterator iter;
    iteratorAPI.figureIteratorInit(&iter, 0, 0, 0x3ff, 0x3ff);
    iteratorAPI.figureIteratorSetPointers(&iter, _this->CGdFigure, _this->AutoClass22, _this->CGdWorld);
    iteratorAPI.iteratorSetArea(&iter, &_this->CGdFigure->figures->position, spell_data.params[0]);
    std::list<uint16_t> affected_figures;
    uint16_t figure_id = iteratorAPI.getNextFigure(&iter);
    uint16_t element_count = 0;
    while (figure_id != 0)
    {
        if (toolboxAPI.figuresCheckFriendly(_this->CGdFigureToolBox, target_index, figure_id))
        {
            if (figure_id != target_index)
            {
                affected_figures.push_back(figure_id);
                element_count++;
            }
            if (element_count == spell_data.params[1])
            {
                break;
            }
        }
        figure_id = iteratorAPI.getNextFigure(&iter);
    }
    if (element_count != 0)
    {
        uint16_t shared_damage = (damage / (element_count + 1));
        uint16_t remainder = (damage % (element_count + 1));
        SF_Coord source_pos = _this->CGdFigure->figures[target_index].position;
        SF_CGdTargetData source_data;
        source_data.entity_index = target_index;
        source_data.entity_type = 1;
        source_data.position = source_pos;
        for (auto t : affected_figures)
        {
            SF_Coord friend_pos = _this->CGdFigure->figures[t].position;
            uint32_t distance = getDistance(&source_pos, &friend_pos);
            distance = (distance * 0x578) / 1000;
            if (distance == 0)
            {
                distance = 1;
            }
            SF_CGdTargetData friend_data;
            friend_data.entity_type = 1;
            friend_data.entity_index = t;
            friend_pos = friend_pos;
            SF_Rectangle unused = {0, 0};
            effectAPI.addEffect(_this->CGdEffect, kGdEffectSpellAssistanceHitFigure, &source_data, &friend_data,
                                _this->OpaqueClass->current_step, distance, &unused);
            toolboxAPI.dealDamage(_this->CGdFigureToolBox, source_index, t, shared_damage, 0, 0, 0);
        }
        damage = shared_damage + remainder;
    }
    iteratorAPI.disposeFigureIterator(iter);
    return damage;
}