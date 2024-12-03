#include "sf_ai_spell_handlers.h"

uint32_t __thiscall target_healing_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    // TODO: Implement Me
    return 0;
}

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
    uint16_t current_health = figureAPI.getCurrentHealth(_this->battleData.CGdFigure, target_index);
    uint32_t max_health = figureAPI.getCurrentMaxHealth(_this->battleData.CGdFigure, target_index);
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
            if (figure_count > 2)
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
        uint16_t current_health = figureAPI.getCurrentHealth(_this->battleData.CGdFigure, target_index);
        uint32_t max_health = figureAPI.getCurrentMaxHealth(_this->battleData.CGdFigure, target_index);
        if (max_health != 0)
        {
            uint16_t percent = (current_health / max_health) * 100;
            if (percent > 80)
            {
                rank = 0;
            }
        }
        else
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
            uint16_t current_health = figureAPI.getCurrentHealth(_this->battleData.CGdFigure, target_index);
            uint32_t max_health = figureAPI.getCurrentMaxHealth(_this->battleData.CGdFigure, target_index);
            if (max_health != 0)
            {
                uint16_t percent = (current_health / max_health) * 100;
                if (percent > 25)
                {
                    rank = 0;
                }
            }
            else
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
        uint16_t current_health = figureAPI.getCurrentHealth(_this->battleData.CGdFigure, target_index);
        uint32_t max_health = figureAPI.getCurrentMaxHealth(_this->battleData.CGdFigure, target_index);
        if (max_health != 0)
        {
            uint16_t percent = (current_health / max_health) * 100;
            if (percent > (100 - spell_data->params[0]))
            {
                rank = 0;
            }
        }
        else
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
                        uint16_t current_health = figureAPI.getCurrentHealth(_this->battleData.CGdFigure, ally_index);
                        uint32_t max_health = figureAPI.getCurrentMaxHealth(_this->battleData.CGdFigure, ally_index);
                        if (max_health != 0)
                        {
                            uint16_t percent = (current_health / max_health) * 100;
                            if (percent < (100 - spell_data->params[0]))
                            {
                                figures_count++;
                            }
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
        uint16_t current_health = figureAPI.getCurrentHealth(_this->battleData.CGdFigure, target_index);
        uint32_t max_health = figureAPI.getCurrentMaxHealth(_this->battleData.CGdFigure, target_index);
        if (max_health != 0)
        {
            uint16_t percent = (current_health / max_health) * 100;
            // hardcode value is 75% gamedata value is 50%, i go with gamedata value
            if (percent > (100 - spell_data->params[1]))
            {
                rank = 0;
            }
        }
        else
        {
            rank = 0;
        }
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
            rank = (current_health * 2 - max_health) / (max_health * max_health);
        }
    }
    else 
    {
        rank = 0;
    }
    return rank;
}
