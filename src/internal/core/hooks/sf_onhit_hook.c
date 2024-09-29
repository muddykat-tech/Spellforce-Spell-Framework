#include "sf_onhit_hook.h"
#include "../sf_wrappers.h"
#include "../sf_hooks.h"
#include "../sf_modloader.h"
#include "../../registry/sf_onhit_registry.h"
#include "../../registry/sf_mod_registry.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <list>
#include <utility>

typedef uint16_t(__thiscall *get_reduced_damage_ptr)(void *AutoClass34, uint16_t source_index, uint16_t target_index, uint16_t unkn);
typedef uint16_t(__thiscall *get_reduced_building_damage_ptr)(void *AutoClass32, uint16_t source_index, uint16_t target_index, uint16_t damage);
typedef uint16_t(__thiscall *get_hit_chance_ptr)(void *AutoClass34, uint16_t source_index, uint16_t target_index);
typedef uint16_t(__thiscall *get_leveled_spell_ptr)(void *CGdResource, uint16_t source_spell_id, uint16_t spell_level);
typedef void(__thiscall *FUN_006c3a60_ptr)(void *AutoClass30, uint16_t source_index, uint16_t target_index, uint8_t unkn, uint32_t unkn2);
typedef uint32_t(__thiscall *FUN_0071d7b0_ptr)(void *CGdObject, uint16_t object_index);
typedef uint32_t(__thiscall *FUN_00755180_ptr)(uint32_t param1);
typedef uint32_t(__thiscall *objectDealDamage_ptr)(void *CGdObjectToolBox, uint16_t source_index, uint16_t target_index, uint16_t damage, uint32_t unknown);
typedef uint32_t(__thiscall *getWeaponEffects_ptr)(void *CGdResource, uint32_t *param1, uint16_t weapon_id);

get_reduced_damage_ptr g_get_reduced_damage;
get_hit_chance_ptr g_get_hit_chance;
get_leveled_spell_ptr g_get_leveled_spell;
FUN_006c3a60_ptr g_FUN_006c3a60;
get_reduced_building_damage_ptr g_get_reduced_building_damage;
FUN_0071d7b0_ptr g_FUN_0071d7b0;
FUN_00755180_ptr g_FUN_00755180;
objectDealDamage_ptr g_objectDealDamage;
getWeaponEffects_ptr g_getWeaponEffects;

// TODO PassThrough GET CURRENT STAT

uint16_t __thiscall getCurrentDex(SF_CGdFigure *_this, uint16_t figure_index)
{
    uint16_t base_val = _this->figures[figure_index].dexterity.base_val;
    uint16_t bonus_val = _this->figures[figure_index].dexterity.bonus_val;
    uint16_t multiplier = _this->figures[figure_index].dexterity.bonus_multiplier + 100;
    if (base_val + bonus_val > 0)
    {
        return ((base_val + bonus_val) * multiplier) / 100;
    }
    return 0;
}

uint16_t __thiscall getCurrentInt(SF_CGdFigure *_this, uint16_t figure_index)
{
    uint16_t base_val = _this->figures[figure_index].intelligence.base_val;
    uint16_t bonus_val = _this->figures[figure_index].intelligence.bonus_val;
    uint16_t multiplier = _this->figures[figure_index].intelligence.bonus_multiplier + 100;
    if (base_val + bonus_val > 0)
    {
        return ((base_val + bonus_val) * multiplier) / 100;
    }
    return 0;
}
// End TODO

uint16_t get_effect_chance(uint16_t dex_val, uint16_t int_val, uint16_t spell_line)
{
    if (spell_line != kGdSpellLinePoison)
    {
        return (dex_val + int_val + 100) * 5;
    }
    return dex_val * 8 + int_val * 5 + 0x578;
}
void initialize_onhit_data_hooks()
{
    g_get_reduced_damage = (get_reduced_damage_ptr)(ASI::AddrOf(0x3177d0));
    g_get_hit_chance = (get_hit_chance_ptr)(ASI::AddrOf(0x317860));
    g_get_leveled_spell = (get_leveled_spell_ptr)(ASI::AddrOf(0x26de20));
    g_FUN_006c3a60 = (FUN_006c3a60_ptr)(ASI::AddrOf(0x2c3a60));
    g_get_reduced_building_damage = (get_reduced_building_damage_ptr)(ASI::AddrOf(0x317740));
    g_FUN_0071d7b0 = (FUN_0071d7b0_ptr)(ASI::AddrOf(0x31d7b0));
    g_FUN_00755180 = (FUN_00755180_ptr)(ASI::AddrOf(0x355180));
    g_objectDealDamage = (objectDealDamage_ptr)(ASI::AddrOf(0x2b7d70));
    g_getWeaponEffects = (getWeaponEffects_ptr)(ASI::AddrOf(0x2693b0));
}

void __thiscall getTargetData(AutoClass24 *_this, SF_CGdTargetData *target)
{
    target->entity_type = _this->target.entity_type;
    target->entity_index = _this->target.entity_index;
    target->position = _this->target.position;
}

uint32_t getDistange(SF_Coord *pointA, SF_Coord *pointB)
{

    uint32_t delta;
    uint32_t uVar1;
    uint32_t uVar2;
    uint32_t uVar3;
    uint32_t uVar4;

    delta = (uint32_t)(uint16_t)pointA->X - (uint32_t)(uint16_t)pointB->X;
    uVar2 = (int)delta >> 0x1f;
    uVar2 = (delta ^ uVar2) - uVar2;
    uVar4 = uVar2 & 0xffff;
    delta = (uint32_t)(uint16_t)pointA->Y - (uint32_t)(uint16_t)pointB->Y;
    uVar3 = (int)delta >> 0x1f;
    uVar3 = (delta ^ uVar3) - uVar3;
    uVar1 = uVar3 & 0xffff;
    delta = uVar1;
    if ((uint16_t)uVar2 < (uint16_t)uVar3)
    {
        delta = uVar4;
        uVar4 = uVar1;
    }
    return ((delta * 0xd) >> 5) + uVar4;
}

bool isJobDoNothing(uint16_t job_id)
{
    if ((job_id == kGdJobGroupNothing) || (job_id == kGdJobWarriorNothing) ||
        (job_id == kGdJobCheckBattleSleep) || (job_id == kGdJobPetIdle))
    {
        return 1;
    }
    return 0;
}

bool __thiscall isFigureJobSpell(SF_CGdFigureJobs *_this, uint16_t figure_id)
{
    uint16_t job_id = figureAPI.getJob(_this->CGdFigure, figure_id);
    if ((job_id == kGdJobCast) || (job_id == kGdJobPreCast) || (job_id == kGdJobCastResolve) || (job_id == kGdJobCastPreResolve))
    {
        return 1;
    }
    if (job_id == kGdJobWalkToTarget)
    {
        SF_SGtFigureAction action;
        figureAPI.getTargetAction(_this->CGdFigure, &action, figure_id);
        if ((action.type != 0) && (action.type < 10000))
        {
            return 1;
        }
    }
    return 0;
}

bool __thiscall canJobBeInterrupted(FigureJobs job_id)

{
    if ((((job_id != kGdJobCastPreResolve) && (job_id != kGdJobCastResolve)) &&
         (job_id != kGdJobHitTargetRange2)) &&
        (((job_id != kGdJobEnterBuilding && (job_id != kGdJobExitBuilding)) &&
          ((job_id != kGdJobMeleeAbility && (job_id != kGdJobStartWorkAtBuilding))))))
    {
        return 1;
    }
    return 0;
}

uint16_t __thiscall getAttackInterruptionChance(SF_CGdFigure *_this, uint16_t source_index, uint16_t target_index)
{
    uint8_t source_level = _this->figures[source_index].level;
    uint8_t target_level = _this->figures[target_index].level;
    if (target_level < source_level)
    {
        uint16_t precalc_chance = (source_level - target_level) * 5 + 0xf;
        if (precalc_chance > 100)
        {
            return 100;
        }
        return precalc_chance;
    }
    if (target_level == source_level)
    {
        return 0xf;
    }
    if (0xf < ((target_level - source_level) * 2))
    {
        return 0;
    }
    return (target_index - source_index) * -2 + 0xf;
}

uint16_t __thiscall handle_riposte_set(SF_CGdFigureJobs *_this, uint16_t source_index, uint16_t target_index, uint16_t weapon_damage)
{
    bool apply_set = false;
    if (_this->CGdFigure->figures[target_index].set_type == 0x03)
    {
        uint16_t counter = spellAPI.getRandom(_this->OpaqueClass, 100);
        apply_set = (counter < 0x0b);
    }
    if (apply_set)
    {
        uint16_t damage = g_get_reduced_damage(_this->AutoClass34, source_index, source_index, weapon_damage);
        toolboxAPI.dealDamage(_this->CGdFigureToolBox, source_index, source_index, damage, 0, 0, 0);
        return 0;
    }
    // TODO: fix enchatments on weapon applying with reflected hit
    return weapon_damage;
}

uint16_t __thiscall handle_berserk_set(SF_CGdFigureJobs *_this, uint16_t source_index, uint16_t target_index, uint16_t weapon_damage)
{
    bool apply_set = false;
    if (_this->CGdFigure->figures[source_index].set_type == 0x04)
    {
        uint16_t counter = spellAPI.getRandom(_this->OpaqueClass, 100);
        apply_set = (counter < 0x0b);
    }
    // 7FFF is one-shot magic number
    if ((apply_set) && (weapon_damage != 0x7FFF))
    {
        uint16_t damage = weapon_damage * 3;
        return damage;
    }
    return weapon_damage;
}

uint16_t __thiscall handle_trueshot_set(SF_CGdFigureJobs *_this, uint16_t source_index, uint16_t target_index, uint16_t weapon_damage)
{
    bool apply_set = false;
    if (_this->CGdFigure->figures[source_index].set_type == 0x05)
    {
        uint16_t counter = spellAPI.getRandom(_this->OpaqueClass, 100);
        apply_set = (counter < 0x0b);
    }
    // 7FFF is one-shot magic number
    if ((apply_set) && (weapon_damage != 0x7FFF))
    {
        uint16_t damage = weapon_damage * 4;
        return damage;
    }
    return weapon_damage;
}

void __thiscall sf_onhit_hook(SF_CGdFigureJobs *_this, uint16_t source_index, uint32_t param_2, uint8_t param_3)
{
    log_info("Called Into On Hit");
    if ((_this->CGdFigure->figures[source_index].owner == -1) || (_this->CGdFigure->figures[source_index].flags & 10))
    {
        return;
    }

    SF_CGdFigureWeaponStats weapon_stats;
    SF_CGdTargetData target;
    SF_SGtFigureAction action;
    figureAPI.getWeaponStats(_this->CGdFigure, &weapon_stats, source_index);
    uint16_t weapon_damage = spellAPI.getRandom(_this->OpaqueClass, weapon_stats.max_dmg - weapon_stats.min_rng);
    weapon_damage += weapon_stats.min_dmg;
    getTargetData(&_this->CGdFigure->figures[source_index].ac_1, &target);
    figureAPI.getTargetAction(_this->CGdFigure, &action, source_index);

    if (target.entity_type == 1)
    {
        if ((_this->CGdFigure->figures[target.entity_index].owner == -1) ||
            (_this->CGdFigure->figures[target.entity_index].flags & 10 != 0) ||
            (!toolboxAPI.isTargetable(_this->CGdFigureToolBox, target.entity_index)))
        {
            return;
        }

        uint16_t hit_chance = g_get_hit_chance(_this->AutoClass34, source_index, target.entity_index);

        // miss handling
        if ((hit_chance < spellAPI.getRandom(_this->OpaqueClass, 100)) && (param_2 == 0))
        {
            g_FUN_006c3a60(_this->AutoClass30, source_index, target.entity_index, 1, 0);
            uint16_t aggro = figureAPI.getAggroValue(_this->CGdFigure, target.entity_index, source_index);
            if (aggro < 10000)
            {
                figureAPI.setAggroValue(_this->CGdFigure, target.entity_index, source_index, 12000, 0);
            }
            uint16_t job_id = figureAPI.getJob(_this->CGdFigure, target.entity_index);

            if (isJobDoNothing(job_id))
            {
                _this->CGdFigure->figures[target.entity_index].to_do_count = 1;
                return;
            }
        }
        uint16_t damage = g_get_reduced_damage(_this->AutoClass34, source_index, target.entity_index, weapon_damage);
        damage = ((damage * param_3 + 0x32) / 100) & 0xffff;
        // glanced hit
        if (damage == 0)
        {
            uint16_t aggro = figureAPI.getAggroValue(_this->CGdFigure, target.entity_index, source_index);
            if (aggro < 10000)
            {
                figureAPI.setAggroValue(_this->CGdFigure, target.entity_index, source_index, 12000, 0);
            }
        }
        else
        {
            if (isFigureJobSpell(_this, target.entity_index))
            {
                uint16_t job_id = figureAPI.getJob(_this->CGdFigure, target.entity_index);
                if (canJobBeInterrupted(job_id))
                {
                    uint16_t chance = getAttackInterruptionChance(_this->CGdFigure, source_index, target.entity_index);
                    if (spellAPI.getRandom(_this->OpaqueClass, 100) <= chance)
                    {
                        toolboxAPI.figureSetNewJob(_this, target.entity_index, kGdJobOfferMe, 0, 0, 0);
                    }
                }
            }

            char ogdamage_info[128];
            snprintf(ogdamage_info, sizeof(ogdamage_info), "Before OnHit: %d", damage);
            log_info(ogdamage_info);

            for (int i = PHASE_0; i < OnHitEnd; ++i)
            {
                std::list<std::pair<uint16_t, onhit_handler_ptr>> onhit_list = get_onhit_phase(static_cast<OnHitPhase>(i));

                uint16_t list_size = onhit_list.size();

                int return_damage = weapon_damage;

                for (auto it = onhit_list.crbegin(); it != onhit_list.crend(); ++it)
                {
                    std::pair<uint16_t, onhit_handler_ptr> entry = *it;

                    uint16_t spell_line_id = entry.first;
                    SpellTag spell_tag = static_cast<SpellTag>(getSpellTag(spell_line_id));

                    if (spell_tag == TARGET_ONHIT_SPELL)
                    {
                        if ((_this->CGdFigure->figures[target.entity_index].flags & F_CHECK_SPELLS_BEFORE_JOB) != 0)
                        {
                            if (toolboxAPI.hasSpellOnIt(_this->CGdFigureToolBox, target.entity_index, spell_line_id))
                            {
                                log_info("Target On Hit spell");
                                onhit_handler_ptr onhit_func = entry.second;
                                damage = onhit_func(_this, source_index, target.entity_index, damage);
                            }
                        }
                    }
                    else
                    {
                        if ((_this->CGdFigure->figures[source_index].flags & F_CHECK_SPELLS_BEFORE_JOB) != 0)
                        {
                            if (toolboxAPI.hasSpellOnIt(_this->CGdFigureToolBox, source_index, spell_line_id))
                            {
                                log_info("Source on hit spell");
                                onhit_handler_ptr onhit_func = entry.second;
                                damage = onhit_func(_this, source_index, target.entity_index, damage);
                            }
                        }
                    }
                }
            }
            // donnoe wtf is it, looks like shooting?

            // logic here:
            //  calculate modification from spells that increase damage

            // apply set changes
            // check critical hits and riposte
            // apply
            if (action.type == 0x2712)
            {
                uint16_t maxY = (_this->CGdFigure->figures[source_index].position.Y <= _this->CGdFigure->figures[target.entity_index].position.Y)
                                    ? (_this->CGdFigure->figures[target.entity_index].position.Y)
                                    : (_this->CGdFigure->figures[source_index].position.Y);

                uint16_t maxX = (_this->CGdFigure->figures[source_index].position.X <= _this->CGdFigure->figures[target.entity_index].position.X)
                                    ? (_this->CGdFigure->figures[target.entity_index].position.X)
                                    : (_this->CGdFigure->figures[source_index].position.X);

                uint16_t minY = (_this->CGdFigure->figures[source_index].position.Y > _this->CGdFigure->figures[target.entity_index].position.Y)
                                    ? (_this->CGdFigure->figures[target.entity_index].position.Y)
                                    : (_this->CGdFigure->figures[source_index].position.Y);

                uint16_t minX = (_this->CGdFigure->figures[source_index].position.X > _this->CGdFigure->figures[target.entity_index].position.X)
                                    ? (_this->CGdFigure->figures[target.entity_index].position.X)
                                    : (_this->CGdFigure->figures[source_index].position.X);
                SF_Coord p1;
                SF_Coord p2;
                SF_Rectangle vector;
                p1.X = maxX;
                if (minX < maxX)
                {
                    p1.X = minX;
                }
                p1.Y = maxY;
                if (minY < maxY)
                {
                    p1.Y = minY;
                }

                p2.X = maxX;
                if (maxX <= minX)
                {
                    p2.X = minX;
                }
                p2.Y = maxY;
                if (maxY <= minY)
                {
                    p2.Y = minY;
                }
                vector.partA = p1.Y << 0x10 | p1.X;
                vector.partB = p2.Y << 0x10 | p2.X;

                uint32_t distance = getDistange(&_this->CGdFigure->figures[source_index].position, &_this->CGdFigure->figures[target.entity_index].position);
                distance = ((distance & 0xffff) * 0x578) / 3000;
                SF_CGdTargetData source = {1, source_index, {0, 0}};
                uint16_t effect_id = effectAPI.addEffect(_this->CGdEffect, kGdEffectProjectile, &source, &target,
                                                         _this->OpaqueClass->current_step, ((distance != 0) ? distance : 1), &vector);
                effectAPI.setEffectXData(_this->CGdEffect, effect_id, EFFECT_ENTITY_TYPE, 1);
                effectAPI.setEffectXData(_this->CGdEffect, effect_id, EFFECT_ENTITY_TYPE2, 1);
                effectAPI.setEffectXData(_this->CGdEffect, effect_id, EFFECT_ENTITY_INDEX, source_index);
                effectAPI.setEffectXData(_this->CGdEffect, effect_id, EFFECT_ENTITY_INDEX2, target.entity_index);
                effectAPI.setEffectXData(_this->CGdEffect, effect_id, EFFECT_DO_NOT_ADD_SUBSPELL, 1);
                effectAPI.setEffectXData(_this->CGdEffect, effect_id, EFFECT_PHYSICAL_DAMAGE, damage);

                uint16_t subspell_id = 0;
                // troll fire thrower
                if (_this->CGdFigure->figures[source_index].unit_data_id == 0x508)
                {
                    subspell_id = 0x6de;
                }
                // troll ice thrower
                if (_this->CGdFigure->figures[source_index].unit_data_id == 0x505)
                {
                    subspell_id = 0x6ea;
                }
                // windarcher
                if (_this->CGdFigure->figures[source_index].unit_data_id == 0x4cd)
                {
                    subspell_id = 0x6d2;
                }
                if (subspell_id != 0)
                {
                    uint16_t spell_lvl = _this->CGdFigure->figures[source_index].level;
                    uint16_t spell_id = g_get_leveled_spell(_this->CGdResource, subspell_id, spell_lvl);
                    if (spell_id != 0)
                    {
                        effectAPI.setEffectXData(_this->CGdEffect, effect_id, EFFECT_SUBSPELL_ID, spell_id);
                    }
                }
            }
            else
            {
                uint16_t subspell_id = 0;
                // Havoc & DeathKnight upgrade
                if ((_this->CGdFigure->figures[target.entity_index].unit_data_id == 0x510) || (_this->CGdFigure->figures[target.entity_index].unit_data_id == 0x513))
                {
                    // NOT a bug, but feature. The higher the level of enemy, the stronger curse gets.
                    uint16_t spell_id = g_get_leveled_spell(_this->CGdResource, 0x167, _this->CGdFigure->figures[source_index].level);
                    if (spell_id != 0)
                    {
                        SF_CGdTargetData t_data;
                        t_data.entity_index = source_index;
                        t_data.entity_type = 1;
                        t_data.position = {0, 0};
                        spellAPI.addSpell(_this->CGdSpell, spell_id, _this->OpaqueClass->current_step, &target, &t_data, 0);
                    }
                }
                toolboxAPI.dealDamage(_this->CGdFigureToolBox, source_index, target.entity_index, damage, 0, 0, 0);
            }
            if (figureAPI.isAlive(_this->CGdFigure, target.entity_index))
            {
                uint16_t weapon_id = _this->CGdFigure->figures[source_index].equipment[(action.type != 10000) * 2 + 1];
                // can't make heads or tails here

                uint32_t puVar15[2];
                g_getWeaponEffects(_this->CGdResource, puVar15, weapon_id);
                char damage_info[128];
                snprintf(damage_info, sizeof(damage_info), "Enchant info: %d %d %d", weapon_id, puVar15[0], puVar15[1]);
                log_info(damage_info);
                if (puVar15[0] != 0)
                {
                    for (int i = 0; i < 3; i++)
                    {
                        uint16_t enchant_id = *(uint16_t *)(&puVar15[0] + i * 2 + 2);

                        if (enchant_id == 0)
                            break;
                        SF_CGdResourceSpell spell_data;
                        spellAPI.getResourceSpellData(_this->CGdResource, &spell_data, enchant_id);
                        uint16_t curr_dex = getCurrentDex(_this->CGdFigure, source_index);
                        uint16_t curr_int = getCurrentInt(_this->CGdFigure, source_index);
                        snprintf(damage_info, sizeof(damage_info), "Enchant Spell Line: %d", spell_data.spell_line_id);
                        log_info(damage_info);
                        uint16_t chance = get_effect_chance(curr_dex, curr_int, spell_data.spell_line_id);
                        if (spellAPI.getRandom(_this->OpaqueClass, 10000) < chance)
                        {
                            if ((_this->CGdFigure->figures[source_index].race != 0) && (_this->CGdFigure->figures[source_index].race < 7))
                            {
                                if (_this->CGdFigure->figures[source_index].owner != 0)
                                {
                                    enchant_id = g_get_leveled_spell(_this->CGdResource, enchant_id, _this->CGdFigure->figures[source_index].level);
                                }
                            }
                            if (enchant_id != 0)
                            {
                                // FIXME reflect enchants as well in riposte set handler;
                                SF_CGdTargetData source;
                                source.entity_type = 1;
                                source.entity_index = source_index;
                                source.position = {0, 0};
                                spellAPI.addSpell(_this->CGdSpell, enchant_id, _this->OpaqueClass->current_step, &source, &target, 0);
                            }
                        }
                    }
                }
            }
        }
    }
    if (target.entity_type == 2)
    {
        uint16_t damage = g_get_reduced_building_damage(_this->AutoClass34, source_index, target.entity_index, weapon_damage);
        if ((target.entity_index != 0) && (_this->CGdBuilding->buildings[target.entity_index].health_current != 0))
        {
            toolboxAPI.buildingDealDamage(_this->CGdBuildingToolBox, source_index, target.entity_index, damage, 0);
        }
    }
    if (target.entity_type == 3)
    {
        if (target.entity_index != 0)
        {
            uint32_t value_1 = g_FUN_0071d7b0(_this->CGdObject, target.entity_index);
            if ((value_1 != 0) && (g_FUN_00755180(value_1) != 0))
            {
                g_objectDealDamage(_this->CGdObjectToolBox, source_index, target.entity_index, weapon_damage, 0);
            }
        }
    }
}
