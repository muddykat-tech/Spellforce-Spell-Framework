#include "sf_ai_spell_handlers.h"
#include "../core/sf_wrappers.h"

#include <stdio.h>

uint32_t __thiscall cure_poison_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    if (!toolboxAPI.hasSpellOnIt((_this->battleData).CGdFigureToolBox, target_index, kGdSpellLinePoison))
    {
        rank = 0;
    }
    return rank;
}

// Ice, Fire, Thorn shield, Conservation, Dexterity
uint32_t __thiscall shields_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    if (_this->battleData.enemy_figures.entityCount == 0)
    {
        rank = 0;
    }
    else
    {
        if (toolboxAPI.isUnitMelee(_this->battleData.CGdFigureToolBox, target_index))
        {
            rank = rank << 2;
        }
        else
        {
            rank = rank << 1;
        }

        if (figureAPI.getCurrentHealth(_this->battleData.CGdFigure, target_index) < figureAPI.getCurrentMaxHealth(_this->battleData.CGdFigure, target_index))
        {
            rank = rank << 1;
        }
    }
    return rank;
}

uint32_t __thiscall cure_disease_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    if (!toolboxAPI.hasSpellOnIt((_this->battleData).CGdFigureToolBox, target_index, kGdSpellLinePestilence))
    {
        rank = 0;
    }
    return rank;
}

uint32_t __thiscall death_grasp_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    if (figureAPI.getCurrentHealthPercent(_this->battleData.CGdFigure, target_index) < 50)
    {
        rank = rank * 10;
    }
    else
    {
        rank = 0;
    }

    return rank;
}

// weakness, suffocation, lifetap
// slow fighting, inflexibility, slow walking
// inability, hypnotization, manatap
uint32_t __thiscall offensive_aura_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    if (_this->battleData.current_figure == target_index)
    {
        if (_this->battleData.enemy_figures.entityCount == 0)
        {
            rank = 0;
        }
        else
        {
            uint16_t manacost = (spell_data->mana_cost * 3) / 2;
            if (figureAPI.getCurrentMana(_this->battleData.CGdFigure, target_index) < manacost)
            {
                rank = 0;
            }
        }
    }
    else
    {
        rank = 0;
    }
    return rank;
}

// kGdSpellLineAuraStrength
// kGdSpellLineAuraEndurance
// kGdSpellLineAuraFastFighting
// kGdSpellLineAuraFlexibility
// kGdSpellLineAuraFastWalking
// kGdSpellLineAuraDexterity
// kGdSpellLineAuraBrilliance
uint32_t __thiscall defensive_aura_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    if (_this->battleData.current_figure == target_index)
    {
        if ((_this->battleData.enemy_figures.entityCount == 0) || (_this->battleData.ally_figures.entityCount == 0))
        {
            rank = 0;
        }
        else
        {
            uint16_t manacost = (spell_data->mana_cost * 3) / 2;
            if (figureAPI.getCurrentMana(_this->battleData.CGdFigure, target_index) < manacost)
            {
                rank = 0;
            }
        }
    }
    else
    {
        rank = 0;
    }
    return rank;
}

// Regeneration and Healing
uint32_t __thiscall healing_aura_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    if (_this->battleData.current_figure == target_index)
    {
        if ((_this->battleData.enemy_figures.entityCount == 0) || (_this->battleData.ally_figures.entityCount == 0))
        {
            rank = 0;
        }
        else
        {
            if (_this->battleData.figures_missing_hp == 0)
            {
                rank = 0;
            }
            else
            {
                uint16_t manacost = (spell_data->mana_cost * 3) / 2;
                if (figureAPI.getCurrentMana(_this->battleData.CGdFigure, target_index) < manacost)
                {
                    rank = 0;
                }
            }
        }
    }
    else
    {
        rank = 0;
    }
    return rank;
}

uint32_t __thiscall aura_light_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    if (_this->battleData.current_figure == target_index)
    {
        if (_this->battleData.enemy_figures.entityCount == 0)
        {
            rank = 0;
        }
        else
        {
            rank = 0;
            // Vanilla Bug Fix
            for (uint16_t i = 0; i < _this->battleData.enemy_figures.entityCount; i++)
            {
                uint16_t enemy_index = _this->battleData.enemy_figures.data[i].entity_index;
                if (_this->battleData.CGdFigure->figures[enemy_index].flags & UNDEAD)
                {
                    rank = 1;
                    break;
                }
            }
            if (rank)
            {
                uint16_t manacost = (spell_data->mana_cost * 3) / 2;
                if (figureAPI.getCurrentMana(_this->battleData.CGdFigure, target_index) < manacost)
                {
                    rank = 0;
                }
            }
        }
    }
    else
    {
        rank = 0;
    }
    return rank;
}

// kGdSpellLineAbilityWarCry
// kGdSpellLineAbilityPatronize
// kGdSpellLineAbilityEndurance
uint32_t __thiscall HCA_ability_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    if ((_this->battleData.current_figure != target_index) ||
        (_this->battleData.ally_figures.entityCount < 4) ||
        (_this->battleData.enemy_figures.entityCount == 0))
    {
        rank = 0;
    }
    else
    {
        uint8_t figure_count = 0;
        for (uint16_t i = 0; i < _this->battleData.ally_figures.entityCount; i++)
        {
            uint16_t ally_index = _this->battleData.ally_figures.data[i].entity_index;
            if ((figureAPI.isAlive(_this->battleData.CGdFigure, ally_index)) &&
                (figureAPI.isWarrior(_this->battleData.CGdFigure, ally_index)) &&
                (!toolboxAPI.hasSpellOnIt(_this->battleData.CGdFigureToolBox, ally_index, spell_line)))
            {
                figure_count++;
            }
            if (figure_count > 3)
            {
                break;
            }
        }
        if (figure_count < 3)
        {
            rank = 0;
        }
    }
    return rank;
}

// kGdSpellLineAbilityShelter
// kGdSpellLineAbilityTrueShot
// kGdSpellLineAbilitySteelSkin
// kGdSpellLineAbilitySalvo
uint32_t __thiscall RCA_ability_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    if ((_this->battleData.current_figure != target_index) ||
        (_this->battleData.enemy_figures.entityCount == 0))
    {
        rank = 0;
    }
    return rank;
}

// kGdSpellLineAbilityDurability
// kGdSpellLineAbilityRiposte
uint32_t __thiscall riposte_ability_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    if ((_this->battleData.current_figure != target_index) ||
        (_this->battleData.enemy_figures.entityCount == 0))
    {
        rank = 0;
    }
    else
    {
        if (figureAPI.getCurrentHealthPercent(_this->battleData.CGdFigure, target_index) > 80)
        {
            rank = 0;
        }
    }
    return rank;
}

// kGdSpellLineSummonGoblin
// kGdSpellLineSummonSkeleton
// kGdSpellLineSummonDemon
// kGdSpellLineSummonChanneler
// kGdSpellLineSummonSpectre
// kGdSpellLineSummonWolf
// kGdSpellLineSummonBear
// kGdSpellLineFireElemental
// kGdSpellLineIceElemental
// kGdSpellLineEarthElemental
// kGdSpellLineSummonTreeWraith
// kGdSpellLineSummonBlade
// kGdSpellLineSummonFireGolem
// kGdSpellLineSummonIceGolem
// kGdSpellLineSummonEarthGolem
// kGdSpellLineMirrorImage
uint32_t __thiscall summon_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    if ((_this->battleData.current_figure != target_index) ||
        (_this->battleData.enemy_figures.entityCount == 0))
    {
        rank = 0;
    }
    else
    {
        uint16_t manacost = (spell_data->mana_cost * 3) / 2;
        if (figureAPI.getCurrentMana(_this->battleData.CGdFigure, target_index) < manacost)
        {
            rank = 0;
        }
    }
    return rank;
}

// kGdSpellLineDistract
// kGdSpellLineShockwave
// kGdSpellLineWaveOfFire
// kGdSpellLineWaveOfIce
// kGdSpellLineWaveOfRocks
uint32_t __thiscall wave_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    if ((_this->battleData.current_figure != target_index) ||
        (_this->battleData.enemy_figures.entityCount == 0))
    {
        rank = 0;
    }
    else
    {
        uint8_t figure_count = 0;
        for (uint16_t i = 0; i < _this->battleData.enemy_figures.entityCount; i++)
        {
            uint16_t enemy_index = _this->battleData.enemy_figures.data[i].entity_index;
            if (figureAPI.isAlive(_this->battleData.CGdFigure, enemy_index))
            {
                SF_Coord enemy_pos = _this->battleData.CGdFigure->figures[enemy_index].position;
                SF_Coord caster_pos = _this->battleData.current_figure_pos;
                if (getDistance(&enemy_pos, &caster_pos) < 10)
                {
                    figure_count++;
                }
            }
            if (figure_count > 2)
            {
                break;
            }
        }
        if (figure_count < 2)
        {
            rank = 0;
        }
    }
    return rank;
}

uint32_t __thiscall berserk_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    if ((_this->battleData.current_figure != target_index) ||
        (_this->battleData.enemy_figures.entityCount == 0))
    {
        rank = 0;
    }
    else
    {
        // Berserk level 101 is special case for trolls
        if (spell_data->skill_requirements[2] > 100)
        {
            if (figureAPI.getCurrentHealthPercent(_this->battleData.CGdFigure, target_index) > 25)
            {
                rank = 0;
            }
        }
    }
    return rank;
}

uint32_t __thiscall blessing_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    if ((_this->battleData.current_figure != target_index) ||
        (_this->battleData.enemy_figures.entityCount == 0))
    {
        rank = 0;
    }
    else
    {
        if (figureAPI.getCurrentHealthPercent(_this->battleData.CGdFigure, target_index) > (100 - spell_data->params[0]))
        {
            rank = 0;
        }
    }
    return rank;
}

uint32_t __thiscall benefactions_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    if ((_this->battleData.current_figure != target_index) ||
        (_this->battleData.enemy_figures.entityCount == 0) ||
        (_this->battleData.ally_figures.entityCount < 4) ||
        (_this->battleData.figures_missing_hp == 0))
    {
        rank = 0;
    }
    else
    {
        uint16_t figures_count = 0;
        for (uint16_t i = 0; i < _this->battleData.ally_figures.entityCount; i++)
        {
            uint16_t ally_index = _this->battleData.ally_figures.data[i].entity_index;
            if (figureAPI.isAlive(_this->battleData.CGdFigure, ally_index))
            {
                if (!toolboxAPI.hasSpellOnIt(_this->battleData.CGdFigureToolBox, ally_index, spell_line))
                {
                    if (figureAPI.isWarrior(_this->battleData.CGdFigure, ally_index))
                    {
                        if (figureAPI.getCurrentHealthPercent(_this->battleData.CGdFigure, ally_index) < (100 - spell_data->params[0]))
                        {
                            figures_count++;
                        }
                    }
                }
                else
                {
                    rank = 0;
                    break;
                }
            }
        }
        if (figures_count < 3)
        {
            rank = 0;
        }
    }
    return rank;
}

uint32_t __thiscall shift_life_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    if ((_this->battleData.current_figure != target_index) ||
        (_this->battleData.enemy_figures.entityCount == 0) ||
        (_this->battleData.ally_figures.entityCount == 0))
    {
        rank = 0;
    }
    else
    {
        uint16_t percent = figureAPI.getCurrentHealthPercent(_this->battleData.CGdFigure, target_index);
        // hardcode value is 75% gamedata value is 50%, i go with gamedata value
        if (percent > (100 - spell_data->params[1]))
        {
            rank = 0;
        }
    }
    return rank;
}

uint32_t __thiscall critical_hits_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    if ((_this->battleData.current_figure != target_index) ||
        (_this->battleData.enemy_figures.entityCount == 0))
    {
        rank = 0;
    }
    else
    {
        CGdFigureIterator iter;
        SF_Coord pos;
        iteratorAPI.figureIteratorInit(&iter, 0, 0, 0x3ff, 0x3ff);
        iteratorAPI.figureIteratorSetPointers(&iter,
                                              _this->battleData.CGdFigure,
                                              _this->battleData.autoclass22,
                                              _this->battleData.CGdWorld);
        figureAPI.getPosition(_this->battleData.CGdFigure, &pos, target_index);
        if (toolboxAPI.isUnitMelee(_this->battleData.CGdFigureToolBox, target_index))
        {
            iteratorAPI.iteratorSetArea(&iter, &pos, 2);
        }
        else
        {
            iteratorAPI.iteratorSetArea(&iter, &pos, 8);
        }
        bool should_use = false;
        uint16_t figure_id = iteratorAPI.getNextFigure(&iter);
        while (figure_id != 0)
        {
            if (_this->battleData.CGdFigure->figures[figure_id].owner != (uint16_t)-1)
            {
                if (toolboxAPI.figuresCheckHostile(_this->battleData.CGdFigureToolBox, target_index, figure_id))
                {
                    if (figureAPI.isAlive(_this->battleData.CGdFigure, figure_id))
                    {
                        if (figureAPI.getCurrentHealthPercent(_this->battleData.CGdFigure, figure_id) < 50)
                        {
                            should_use = true;
                            break;
                        }
                    }
                }
            }
            figure_id = iteratorAPI.getNextFigure(&iter);
        }
        if (!should_use)
        {
            rank = 0;
        }
        iteratorAPI.disposeFigureIterator(&iter);
    }
    return rank;
}

// kGdSpellLineProtectionBlack
// kGdSpellLineFakeSpellOneFigure
uint32_t __thiscall do_not_cast_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    return 0;
}

uint32_t __thiscall default_support_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    return 2;
}

// Weird shit, might need fix?
uint32_t __thiscall sacrifice_mana_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 5;
    uint16_t current_mana = figureAPI.getCurrentMana(_this->battleData.CGdFigure, _this->battleData.current_figure);
    uint16_t max_mana = figureAPI.getCurrentMaxMana(_this->battleData.CGdFigure, _this->battleData.current_figure);
    uint16_t current_health = figureAPI.getCurrentHealth(_this->battleData.CGdFigure, target_index);
    uint16_t max_health = figureAPI.getCurrentMaxHealth(_this->battleData.CGdFigure, target_index);
    if ((max_mana / 2 < current_mana) && (current_health < max_health))
    {
        if (current_health == 0)
        {
            rank = 0;
        }
        else
        {
            if (max_health == 0)
            {
                rank = 0;
            }
            else
            {
                rank = (current_health * 2 - max_health) / (max_health * max_health);
            }
        }
    }
    else
    {
        rank = 0;
    }
    return rank;
}

uint32_t __thiscall tower_extinct_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    ushort_list_node node = _this->battleData.another_figure_list[_this->battleData.current_figure];
    uint16_t list_length = ((uint32_t)node.data - (uint32_t)node.first) >> 1;
    if ((_this->battleData.enemy_figures.entityCount == 0) || (list_length == 0))
    {
        rank = 0;
    }
    else
    {
        _this->battleData.current_source_maybe.entity_index = _this->battleData.current_figure;
        _this->battleData.current_source_maybe.entity_type = 1;
        _this->battleData.current_source_maybe.position.X = _this->battleData.current_figure_pos.X;
        _this->battleData.current_source_maybe.position.Y = _this->battleData.current_figure_pos.Y;
    }
    return rank;
}

uint32_t __thiscall extinct_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    if (target_index == _this->battleData.current_figure)
    {
        uint16_t threshold = spell_data->params[0];
        uint16_t count = 0;
        for (int i = 0; i < _this->battleData.enemy_figures.entityCount; i++)
        {
            uint16_t figure_index = _this->battleData.enemy_figures.data[i].entity_index;
            if (figureAPI.isAlive(_this->battleData.CGdFigure, figure_index))
            {
                if (figureAPI.getCurrentHealth(_this->battleData.CGdFigure, figure_index) < threshold)
                {
                    SF_Coord position;
                    figureAPI.getPosition(_this->battleData.CGdFigure, &position, figure_index);
                    uint16_t distance = getDistance(&position, &(_this->battleData.current_figure_pos));
                    if (distance < 10)
                    {
                        count++;
                        break;
                    }
                }
            }
        }
        if (!count)
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

uint32_t __thiscall healing_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    uint16_t current_health = figureAPI.getCurrentHealth(_this->battleData.CGdFigure, target_index);
    uint16_t max_health = figureAPI.getCurrentMaxHealth(_this->battleData.CGdFigure, target_index);
    uint16_t healing_amount = 0;
    struct
    {
        uint32_t *first;
        uint32_t *data;
        uint32_t *post_last;
    } some_list;
    uint32_t *list_first;
    uint32_t *list_current;
    figureAPI.getHealersList(_this->battleData.CGdFigure, &some_list, target_index);
    uint32_t list_length = ((uint32_t)some_list.data - (uint32_t)some_list.first) >> 2;
    if (list_length != 0)
    {
        list_first = some_list.first;
        list_current = some_list.data;
        while (list_current != list_first)
        {
            healing_amount += spell_data->params[0];
            list_first++;
        }
    }
    if (current_health + healing_amount < max_health)
    {
        if (current_health == 0)
        {
            rank = 0;
            figureAPI.disposeHealerList(&some_list);
            return rank;
        }
        if (((max_health * 9) / 10 < current_health + healing_amount) ||
            (current_health + healing_amount < max_health / 10))
        {
            rank = rank << 1;
        }
    }
    else
    {
        rank = 0;
    }
    return rank;
}

uint32_t __thiscall amok_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 2;
    if ((_this->battleData.enemy_figures.entityCount < 2) ||
        (spell_data->params[2] < _this->battleData.current_target_level_possibly))
    {
        rank = 0;
    }
    return rank;
}

// kGdSpellLineHypnotizeTower
// kGdSpellLineHypnotizeTwo
// kGdSpellLineHypnotizeArea
// kGdSpellLineHypnotize
uint32_t __thiscall hypnotize_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 2;

    if (spell_data->params[2] < _this->battleData.current_target_level_possibly)
    {
        rank = 0;
    }
    else
    {
        if (toolboxAPI.isUnitMelee(_this->battleData.CGdFigureToolBox, target_index))
        {
            rank = 4;
        }
    }
    return rank;
}

uint32_t __thiscall freeze_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 2;
    uint16_t max_health = figureAPI.getCurrentMaxHealth(_this->battleData.CGdFigure, target_index);
    uint16_t current_health = figureAPI.getCurrentHealth(_this->battleData.CGdFigure, target_index);
    if (current_health < max_health)
    {
        rank = 4;
    }
    return rank;
}

// Stackable:
//  kGdSpellLineFireBurst
//  Icestrike
// Non-stackable:
//  kGdSpellLinePoison
//  kGdSpellLinePestilence
uint32_t __thiscall fireburst_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 2;
    if (_this->battleData.current_target_level_possibly < 3)
    {
        rank = 4;
    }
    return rank;
}

// Death and Pain
uint32_t __thiscall death_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 2;
    if (_this->battleData.current_target_level_possibly != 0)
    {
        rank = ((uint16_t)_this->battleData.current_target_level_possibly / 5 + 1) * 2;
    }
    return rank;
}

uint32_t __thiscall lifetap_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 2;
    uint16_t percent = figureAPI.getCurrentHealthPercent(_this->battleData.CGdFigure, _this->battleData.current_figure);
    if (percent > 95)
    {
        rank = 0;
    }
    percent = figureAPI.getCurrentHealthPercent(_this->battleData.CGdFigure, target_index);
    if (percent > 10)
    {
        rank = 0;
    }
    return rank;
}

uint32_t __thiscall petrify_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 2;

    if (spell_data->params[2] < _this->battleData.current_target_level_possibly)
    {
        rank = 0;
    }
    return rank;
}

// charm
// charm animal
uint32_t __thiscall charm_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 2;
    if ((figureAPI.isFlagSet(_this->battleData.CGdFigure, target_index, UNKILLABLE)) ||
        (spell_data->params[6] < _this->battleData.current_target_level_possibly))
    {
        rank = 0;
    }
    return rank;
}

uint32_t __thiscall hallow_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 2;
    if ((_this->battleData.CGdFigure->figures[target_index].flags & GdFigureFlags::UNDEAD) == 0)
    {
        rank = 0;
    }
    return rank;
}

uint32_t __thiscall manatap_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 2;
    uint16_t current_mana = figureAPI.getCurrentMana(_this->battleData.CGdFigure, _this->battleData.current_figure);
    uint16_t max_mana = figureAPI.getCurrentMaxMana(_this->battleData.CGdFigure, _this->battleData.current_figure);
    if (current_mana <= (uint16_t)((max_mana * 95) / 100))
    {
        uint16_t target_mana = figureAPI.getCurrentMana(_this->battleData.CGdFigure, target_index);
        uint16_t target_max_mana = figureAPI.getCurrentMaxMana(_this->battleData.CGdFigure, target_index);
        if ((target_max_mana / 10) > target_mana)
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

uint32_t __thiscall confuse_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 2;
    if (!toolboxAPI.isUnitMelee(_this->battleData.CGdFigureToolBox, target_index))
    {
        rank = 0;
    }
    return rank;
}

uint32_t __thiscall demoralization_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 2;
    if (spell_data->params[1] < _this->battleData.current_target_level_possibly)
    {
        rank = 0;
    }
    return rank;
}

// befriend
// disenchant
uint32_t __thiscall befriend_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 2;
    if (spell_data->params[0] < _this->battleData.current_target_level_possibly)
    {
        rank = 0;
    }
    return rank;
}

uint32_t __thiscall dispel_white_aura_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 2;
    bool hasAura = false;
    if (toolboxAPI.hasAuraActive(_this->battleData.CGdFigureToolBox, target_index))
    {
        for (uint16_t node_id = figureAPI.getSpellJobStartNode(_this->battleData.CGdFigure, target_index);
             node_id != 0; node_id = toolboxAPI.getNextNode(_this->battleData.CGdDoubleLinkList, node_id))
        {
            uint16_t spell_index = toolboxAPI.getSpellIndexFromDLL(_this->battleData.CGdDoubleLinkList, node_id);
            uint16_t spell_line = spellAPI.getSpellLine(_this->battleData.CGdSpell, spell_index);
            if (spellAPI.hasSpellTag(spell_line, SpellTag::WHITE_AURA_SPELL))
            {
                hasAura = true;
                break;
            }
        }
    }
    if (!hasAura)
    {
        rank = 0;
    }
    return rank;
}

uint32_t __thiscall dispel_black_aura_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 2;
    bool hasAura = false;
    if (toolboxAPI.hasAuraActive(_this->battleData.CGdFigureToolBox, target_index))
    {
        for (uint16_t node_id = figureAPI.getSpellJobStartNode(_this->battleData.CGdFigure, target_index);
             node_id != 0; node_id = toolboxAPI.getNextNode(_this->battleData.CGdDoubleLinkList, node_id))
        {
            uint16_t spell_index = toolboxAPI.getSpellIndexFromDLL(_this->battleData.CGdDoubleLinkList, node_id);
            uint16_t spell_line = spellAPI.getSpellLine(_this->battleData.CGdSpell, spell_index);
            if (spellAPI.hasSpellTag(spell_line, SpellTag::BLACK_AURA_SPELL))
            {
                hasAura = true;
                break;
            }
        }
    }
    if (!hasAura)
    {
        rank = 0;
    }
    return rank;
}

uint32_t __thiscall default_offensive_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    return 4;
}

uint32_t __thiscall dominate_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 2;
    if ((spell_data->params[2] < _this->battleData.current_target_level_possibly) ||
        (figureAPI.isFlagSet(_this->battleData.CGdFigure, target_index, UNKILLABLE)))
    {
        rank = 0;
    }
    return rank;
}

uint32_t __thiscall raise_dead_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    CGdFigureIterator iter;
    iteratorAPI.figureIteratorInit(&iter, 0, 0, 0x3ff, 0x3ff);
    iteratorAPI.figureIteratorSetPointers(&iter, _this->battleData.CGdFigure,
                                          _this->battleData.autoclass22, _this->battleData.CGdWorld);

    iteratorAPI.iteratorSetArea(&iter, cast_position, spell_data->params[2]);
    uint16_t count = 0;
    uint32_t x_sum = 0;
    uint32_t y_sum = 0;
    SF_CGdFigure *sf_figures = _this->battleData.CGdFigure;

    uint16_t figure_index = iteratorAPI.getNextFigure(&iter);
    while (figure_index != 0)
    {
        if (sf_figures->figures[figure_index].owner != (uint16_t)(-1))
        {
            if (sf_figures->figures[figure_index].owner == sf_figures->figures[_this->battleData.current_figure].owner)
            {
                if ((sf_figures->figures[figure_index].flags & GdFigureFlags::IS_DEAD) != 0)
                {
                    if ((sf_figures->figures[figure_index].debug_flags & 0x2) == 0)
                    {
                        x_sum += sf_figures->figures[figure_index].position.X;
                        y_sum += sf_figures->figures[figure_index].position.Y;
                        count++;
                    }
                }
            }
        }
        figure_index = iteratorAPI.getNextFigure(&iter);
    }

    if (count < 3)
    {
        rank = 0;
    }
    else
    {
        x_sum = x_sum / count;
        y_sum = y_sum / count;
        cast_position->X = x_sum;
        cast_position->Y = y_sum;
    }
    iteratorAPI.disposeFigureIterator(&iter);
    return rank;
}

uint32_t __thiscall area_heal_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    if (((_this->battleData.figures_missing_hp * 100) / _this->battleData.figures_max_hp) >= 4)
    {
        rank = 0;
    }
    return rank;
}

uint32_t __thiscall revenge_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    if (((_this->battleData.figures_missing_hp * 100) / _this->battleData.figures_max_hp) < 5)
    {
        return 0;
    }
    CGdFigureIterator iter;
    iteratorAPI.figureIteratorInit(&iter, 0, 0, 0x3ff, 0x3ff);
    iteratorAPI.figureIteratorSetPointers(&iter, _this->battleData.CGdFigure,
                                          _this->battleData.autoclass22, _this->battleData.CGdWorld);

    iteratorAPI.iteratorSetArea(&iter, cast_position, spell_data->params[2]);
    uint16_t count = 0;
    SF_CGdFigure *sf_figures = _this->battleData.CGdFigure;
    uint16_t figure_index = iteratorAPI.getNextFigure(&iter);
    while (figure_index != 0)
    {
        if (sf_figures->figures[figure_index].owner != (uint16_t)(-1))
        {
            if (sf_figures->figures[figure_index].owner == sf_figures->figures[_this->battleData.current_figure].owner)
            {
                if ((sf_figures->figures[figure_index].flags & GdFigureFlags::IS_DEAD) != 0)
                {
                    if ((sf_figures->figures[figure_index].flags & GdFigureFlags::USED_FOR_REVENGE) != 0)
                    {
                        count++;
                    }
                }
            }
            if (count >= 5)
            {
                break;
            }
        }
        figure_index = iteratorAPI.getNextFigure(&iter);
    }

    if (count < 5)
    {
        rank = 0;
    }
    iteratorAPI.disposeFigureIterator(&iter);
    return rank;
}

uint32_t __thiscall torture_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    CGdFigureIterator iter;
    iteratorAPI.figureIteratorInit(&iter, 0, 0, 0x3ff, 0x3ff);
    iteratorAPI.figureIteratorSetPointers(&iter, _this->battleData.CGdFigure,
                                          _this->battleData.autoclass22, _this->battleData.CGdWorld);

    iteratorAPI.iteratorSetArea(&iter, cast_position, spell_data->params[2]);
    uint16_t count = 0;
    SF_CGdFigure *sf_figures = _this->battleData.CGdFigure;
    uint16_t figure_index = iteratorAPI.getNextFigure(&iter);
    while (figure_index != 0)
    {
        if (sf_figures->figures[figure_index].owner != (uint16_t)(-1))
        {
            if (sf_figures->figures[figure_index].owner != sf_figures->figures[_this->battleData.current_figure].owner)
            {
                if ((sf_figures->figures[figure_index].flags & GdFigureFlags::IS_DEAD) != 0)
                {
                    if ((sf_figures->figures[figure_index].flags & GdFigureFlags::USED_FOR_REVENGE) != 0)
                    {
                        count++;
                    }
                }
            }
            if (count >= 5)
            {
                break;
            }
        }
        figure_index = iteratorAPI.getNextFigure(&iter);
    }

    if (count < 5)
    {
        rank = 0;
    }
    iteratorAPI.disposeFigureIterator(&iter);
    return rank;
}

uint32_t __thiscall fog_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    uint16_t count = 0;
    ushort_list_node *enemy_list = &(_this->battleData.another_figure_list[_this->battleData.current_figure]);
    uint16_t length = ((uint32_t)enemy_list->data - (uint32_t)(enemy_list->first)) >> 1;

    for (uint16_t i = 0; i < length; i++)
    {
        uint16_t target_index = enemy_list->first[i];
        SF_Coord target_pos;
        target_pos.X = _this->battleData.CGdFigure->figures[target_index].position.X;
        target_pos.Y = _this->battleData.CGdFigure->figures[target_index].position.Y;
        uint16_t distance = getDistance(cast_position, &target_pos);
        if (figureAPI.getSpellJobStartNode(_this->battleData.CGdFigure, target_index))
        {
            if (spell_data->params[0] >= distance)
            {
                count++;
            }
        }
        else
        {
            if (!toolboxAPI.hasSpellOnIt(_this->battleData.CGdFigureToolBox, target_index, spell_line))
            {
                if (spell_data->params[0] >= distance)
                {
                    count++;
                }
            }
        }
        if (count > 2)
        {
            break;
        }
    }

    if (count < 3)
    {
        rank = 0;
    }

    return rank;
}

uint32_t __thiscall area_pain_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    return 1;
}

uint32_t __thiscall rain_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    uint16_t count = 0;
    ushort_list_node *enemy_list = &(_this->battleData.another_figure_list[_this->battleData.current_figure]);
    uint16_t length = ((uint32_t)enemy_list->data - (uint32_t)(enemy_list->first)) >> 1;
    uint16_t limit = (length + 2) / 3;
    for (uint16_t i = 0; i < length; i++)
    {
        uint16_t target_index = enemy_list->first[i];
        SF_Coord target_pos;
        target_pos.X = _this->battleData.CGdFigure->figures[target_index].position.X;
        target_pos.Y = _this->battleData.CGdFigure->figures[target_index].position.Y;
        uint16_t distance = getDistance(cast_position, &target_pos);
        if (spell_data->params[2] >= distance)
        {
            count++;
        }
        if (count >= limit)
        {
            break;
        }
    }
    if (count < limit)
    {
        rank = 0;
    }
    return rank;
}

uint32_t __thiscall freeze_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    uint16_t count = 0;
    ushort_list_node *enemy_list = &(_this->battleData.another_figure_list[_this->battleData.current_figure]);
    uint16_t length = ((uint32_t)enemy_list->data - (uint32_t)(enemy_list->first)) >> 1;

    for (uint16_t i = 0; i < length; i++)
    {
        uint16_t target_index = enemy_list->first[i];
        SF_Coord target_pos;
        target_pos.X = _this->battleData.CGdFigure->figures[target_index].position.X;
        target_pos.Y = _this->battleData.CGdFigure->figures[target_index].position.Y;
        uint16_t distance = getDistance(cast_position, &target_pos);
        if (figureAPI.getSpellJobStartNode(_this->battleData.CGdFigure, target_index))
        {
            if (spell_data->params[1] >= distance)
            {
                count++;
            }
        }
        else
        {
            if (!toolboxAPI.hasSpellOnIt(_this->battleData.CGdFigureToolBox, target_index, spell_line))
            {
                if (spell_data->params[1] >= distance)
                {
                    count++;
                }
            }
        }
        if (count > 2)
        {
            break;
        }
    }

    if (count < 3)
    {
        rank = 0;
    }

    return rank;
}

uint32_t __thiscall hypnotize_area_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    uint16_t count = 0;
    ushort_list_node *enemy_list = &(_this->battleData.another_figure_list[_this->battleData.current_figure]);
    uint16_t length = ((uint32_t)enemy_list->data - (uint32_t)(enemy_list->first)) >> 1;

    for (uint16_t i = 0; i < length; i++)
    {
        uint16_t target_index = enemy_list->first[i];
        SF_Coord target_pos;
        target_pos.X = _this->battleData.CGdFigure->figures[target_index].position.X;
        target_pos.Y = _this->battleData.CGdFigure->figures[target_index].position.Y;
        uint16_t distance = getDistance(cast_position, &target_pos);
        if (figureAPI.getSpellJobStartNode(_this->battleData.CGdFigure, target_index))
        {
            if (spell_data->params[3] >= distance)
            {
                count++;
            }
        }
        else
        {
            if (!toolboxAPI.hasSpellOnIt(_this->battleData.CGdFigureToolBox, target_index, spell_line))
            {
                if (spell_data->params[3] >= distance)
                {
                    count++;
                }
            }
        }
        if (count > 2)
        {
            break;
        }
    }

    if (count < 3)
    {
        rank = 0;
    }

    return rank;
}

uint32_t __thiscall default_aoe_offensive_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    uint16_t count = 0;
    ushort_list_node *enemy_list = &(_this->battleData.another_figure_list[_this->battleData.current_figure]);
    uint16_t length = ((uint32_t)enemy_list->data - (uint32_t)(enemy_list->first)) >> 1;

    for (uint16_t i = 0; i < length; i++)
    {
        uint16_t target_index = enemy_list->first[i];
        SF_Coord target_pos;
        target_pos.X = _this->battleData.CGdFigure->figures[target_index].position.X;
        target_pos.Y = _this->battleData.CGdFigure->figures[target_index].position.Y;
        uint16_t distance = getDistance(cast_position, &target_pos);
        if (figureAPI.getSpellJobStartNode(_this->battleData.CGdFigure, target_index))
        {
            if (spell_data->params[2] >= distance)
            {
                count++;
            }
        }
        else
        {
            if (!toolboxAPI.hasSpellOnIt(_this->battleData.CGdFigureToolBox, target_index, spell_line))
            {
                if (spell_data->params[2] >= distance)
                {
                    count++;
                }
            }
        }
        if (count > 2)
        {
            break;
        }
    }

    if (count < 3)
    {
        rank = 0;
    }

    return rank;
}