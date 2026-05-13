#include "sf_spelleffect_handlers.h"
#include "../core/sf_wrappers.h"
#include "../registry/spell_data_registries/sf_spelleffect_registry.h"
#include <cstdio>
#include <vector>
#include <algorithm>

handler_ptr effect_ability_benefactions_handler;
handler_ptr effect_ability_berserk_handler;
handler_ptr effect_ability_boons_handler;
handler_ptr effect_ability_critical_hits_handler;
handler_ptr effect_ability_durability_handler;
handler_ptr effect_ability_endurance_handler;
handler_ptr effect_ability_patronize_handler;
handler_ptr effect_ability_riposte_handler;
handler_ptr effect_ability_salvo_handler;
handler_ptr effect_ability_shelter_handler;
handler_ptr effect_ability_shift_life_handler;
handler_ptr effect_ability_steelskin_handler;
handler_ptr effect_ability_trueshot_handler;
handler_ptr effect_ability_warcry_handler;
handler_ptr effect_acid_cloud_handler;
handler_ptr effect_almightiness_black_handler;
handler_ptr effect_almightiness_elemental_handler;
handler_ptr effect_almightiness_elemental2_handler;
handler_ptr effect_almightiness_mental_handler;
handler_ptr effect_almightiness_white_handler;
handler_ptr effect_amok_handler;
handler_ptr effect_tower_arrow_handler;
handler_ptr effect_assistance_handler;
handler_ptr effect_aura_handler;
handler_ptr effect_siege_aura_handler;
handler_ptr effect_befriend_handler;
handler_ptr effect_unknown1_handler;
handler_ptr effect_blizzard_handler;
handler_ptr effect_brilliance_handler;
handler_ptr effect_cannibalize_handler;
handler_ptr effect_chain_handler;
handler_ptr effect_charisma_handler;
handler_ptr effect_charm_handler;
handler_ptr effect_charm_animal_handler;
handler_ptr effect_chill_resistance_handler;
handler_ptr effect_confuse_handler;
handler_ptr effect_confuse_area_handler;
handler_ptr effect_conservation_handler;
handler_ptr effect_cure_disease_handler;
handler_ptr effect_cure_poison_handler;
handler_ptr effect_dark_banishing_handler;
handler_ptr effect_darkness_area_handler;
handler_ptr effect_death_handler;
handler_ptr effect_death_grasp_handler;
handler_ptr effect_decay_handler;
handler_ptr effect_decay2_handler;
handler_ptr effect_demoralization_handler;
handler_ptr effect_detect_magic_handler;
handler_ptr effect_detect_metal_handler;
handler_ptr effect_dexterity_handler;
handler_ptr effect_disenchant_handler;
handler_ptr effect_dispel_black_aura_handler;
handler_ptr effect_dispel_white_aura_handler;
handler_ptr effect_disrupt_handler;
handler_ptr effect_distract_handler;
handler_ptr effect_dominate_break_handler;
handler_ptr effect_dominate_handler;
handler_ptr effect_dominate_animal_handler;
handler_ptr effect_dominate_undead_handler;
handler_ptr effect_endurance_handler;
handler_ptr effect_enlightement_handler;
handler_ptr effect_essence_black_handler;
handler_ptr effect_essence_elemental_handler;
handler_ptr effect_essence_elemental2_handler;
handler_ptr effect_essence_mental_handler;
handler_ptr effect_essence_white_handler;
handler_ptr effect_eternity_handler;
handler_ptr effect_extinct_handler;
handler_ptr effect_tower_extinct_handler;
handler_ptr effect_fake_spell_one_figure_handler;
handler_ptr effect_fast_fighting_handler;
handler_ptr effect_feet_of_clay_handler;
handler_ptr effect_feign_death_handler;
handler_ptr effect_fireball1_handler;
handler_ptr effect_fireball2_handler;
handler_ptr effect_firebullet_handler;
handler_ptr effect_fireburst_handler;
handler_ptr effect_fire_resistance_handler;
handler_ptr effect_fireshield1_handler;
handler_ptr effect_fireshield2_handler;
handler_ptr effect_flexibility_handler;
handler_ptr effect_flexibility_area_handler;
handler_ptr effect_fog_handler;
handler_ptr effect_forget_handler;
handler_ptr effect_freeze_handler;
handler_ptr effect_freeze_area_handler;
handler_ptr effect_guard_handler;
handler_ptr effect_hallow_handler;
handler_ptr effect_healing_handler;
handler_ptr effect_healing_area_handler;
handler_ptr effect_unknown2_handler;
handler_ptr effect_healing_aura_handler;
handler_ptr effect_holy_touch_handler;
handler_ptr effect_iceshield_handler;
handler_ptr effect_hypnotize_area_handler;
handler_ptr effect_hypnotize_handler;
handler_ptr effect_ice1_handler;
handler_ptr effect_ice2_handler;
handler_ptr effect_icestrike1_handler;
handler_ptr effect_iceshield2_handler;
handler_ptr effect_iceshield3_handler;
handler_ptr effect_icestrike2_handler;
handler_ptr effect_illuminate_handler;
handler_ptr effect_inability_handler;
handler_ptr effect_inflexibility_handler;
handler_ptr effect_inflexibility_area_handler;
handler_ptr effect_invisibility_handler;
handler_ptr effect_invulnerability_handler;
handler_ptr effect_lifetap_handler;
handler_ptr effect_lifetap_aura_handler;
handler_ptr effect_manadrain_handler;
handler_ptr effect_manashield_handler;
handler_ptr effect_manatap_handler;
handler_ptr effect_manatap_aura_handler;
handler_ptr effect_meditation_handler;
handler_ptr effect_melt_resistance_handler;
handler_ptr effect_mirage_handler;
handler_ptr effect_mutation_handler;
handler_ptr effect_pain_handler;
handler_ptr effect_pain_area_handler;
handler_ptr effect_tower_pain_handler;
handler_ptr effect_pestilence_handler;
handler_ptr effect_petrify_handler;
handler_ptr effect_plague_area_handler;
handler_ptr effect_poison_handler;
handler_ptr effect_cloak_of_nor_handler;
handler_ptr effect_quickness_handler;
handler_ptr effect_quickness_area_handler;
handler_ptr effect_rain_of_fire_handler;
handler_ptr effect_raise_dead_handler;
handler_ptr effect_regenerate_handler;
handler_ptr effect_reinforcement_handler;
handler_ptr effect_remediless_handler;
handler_ptr effect_remove_curse_handler;
handler_ptr effect_retention_handler;
handler_ptr effect_revenge_handler;
handler_ptr effect_rock_bullet_handler;
handler_ptr effect_roots_handler;
handler_ptr effect_roots_area_handler;
handler_ptr effect_sacrifice_mana_handler;
handler_ptr effect_self_illusion_handler;
handler_ptr effect_sentinel_healing_handler;
handler_ptr effect_shift_mana_handler;
handler_ptr effect_shock_handler;
handler_ptr effect_shockwave_handler;
handler_ptr effect_slow_fighting_handler;
handler_ptr effect_slowness_handler;
handler_ptr effect_slowness_area_handler;
handler_ptr effect_spark_handler;
handler_ptr effect_stone_handler;
handler_ptr effect_rain_of_stone_handler;
handler_ptr effect_tower_stone_handler;
handler_ptr effect_strength_handler;
handler_ptr effect_strength_area_handler;
handler_ptr effect_suffocation_handler;
handler_ptr effect_suicide_death_handler;
handler_ptr effect_suicide_heal_handler;
handler_ptr effect_summon_handler;
handler_ptr effect_thornshield_handler;
handler_ptr effect_thorns_handler;
handler_ptr effect_torture_handler;
handler_ptr effect_feedback_handler;
handler_ptr effect_wave_handler;
handler_ptr effect_weaken_handler;
handler_ptr effect_weaken_area_handler;


extern SpellFunctions spellAPI;
extern EffectFunctions effectAPI;
extern FigureFunctions figureAPI;
extern ToolboxFunctions toolboxAPI;
extern IteratorFunctions iteratorAPI;
extern BuildingFunctions buildingAPI;

void __thiscall apply_aura_effect(SF_CGdSpell *_this, uint16_t spell_index, uint16_t sub_spell_index,
                                  uint16_t source_index,
                                  uint16_t target_index)
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    SF_Rectangle rect;
    SF_Coord center = {0,0};
    spellAPI.getTargetsRectangle(_this, &rect, spell_index, 0, &center);
    SF_CGdTargetData target_data;
    target_data.entity_index = target_index;
    target_data.entity_type = 1;
    target_data.position = {0,0};

    SF_CGdTargetData source_data;
    source_data.entity_index = source_index;
    source_data.entity_type = 1;
    source_data.position = {0,0};
    SF_Coord caster_pos = _this->SF_CGdFigure->figures[source_index].position;
    SF_Coord target_pos = _this->SF_CGdFigure->figures[target_index].position;
    uint32_t distance = toolboxAPI.getDistance(&caster_pos, &target_pos);
    distance = (distance * 1400) / 4000;
    if (distance == 0)
    {
        distance = 1;
    }
    uint16_t effect_index = effectAPI.addEffect(_this->SF_CGdEffect, kGdEffectAuraResolve,
                                                &source_data, &target_data,
                                                _this->OpaqueClass->current_step, distance,
                                                &rect);
    effectAPI.setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_SPELL_INDEX,
                             spell_index);
    effectAPI.setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_SPELL_ID,
                             spell->spell_id);
    effectAPI.setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_SUBSPELL_ID,
                             sub_spell_index);
    effectAPI.setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_ENTITY_TYPE, 1);
    effectAPI.setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_ENTITY_INDEX,
                             source_index);
    effectAPI.setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_ENTITY_TYPE2, 1);
    effectAPI.setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_ENTITY_INDEX2,
                             target_index);
}

void __thiscall effect_conservation(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    uint16_t source_index = spell->source.entity_index;
    uint16_t target_index = spell->target.entity_index;


    if ((_this->SF_CGdFigure->figures[target_index].owner != (uint16_t)(-1)) &&
        ((_this->SF_CGdFigure->figures[target_index].flags & (IS_DEAD|RESESRVED_ONLY)) == 0))
    {
        if (spellAPI.addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1) == 1)
        {
            if (spellAPI.checkCanApply(_this, spell_index))
            {
                SF_CGdResourceSpell spell_data;
                spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);
                spellAPI.setXData(_this, spell_index, SPELL_CONSERVATION_SHIELD, spell_data.params[0]);
                _this->SF_CGdFigure->figures[target_index].flags |= F_CHECK_SPELLS_BEFORE_CHECK_BATTLE;
                _this->SF_CGdFigure->figures[target_index].flags |= F_CHECK_SPELLS_BEFORE_JOB;
                spell->flags |= CHECK_SPELLS_BEFORE_CHECK_BATTLE;
                spell->flags |= CHECK_SPELLS_BEFORE_JOB2;
                spell->to_do_count = 20;
                uint32_t unused;
                SF_Rectangle rect = {0,0};
                SF_CGdTargetData target;
                target.position = {0,0};
                target.entity_index = target_index;
                target.entity_type = 1;
                spellAPI.addVisualEffect(_this, spell_index, kGdEffectSpellHitTarget, &unused, &target,
                                         _this->OpaqueClass->current_step, 0, &rect);
                return;
            }
        }
        else
        {
            uint32_t shield = spellAPI.getXData(_this, spell_index, SPELL_CONSERVATION_SHIELD);
            if (shield != 0)
            {
                SF_CGdResourceSpell spell_data;
                spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);
                uint32_t total_ticks = (spell_data.params[1] * 10) / 20000 - 1;
                if (spellAPI.getXData(_this, spell_index, SPELL_TICK_COUNT_AUX) <= total_ticks)
                {
                    spell->to_do_count = 20;
                    return;
                }
            }
            spellAPI.figTryClrCHkSPlBfrJob2(_this, spell_index);
            spellAPI.figClrChkSplBfrChkBattle(_this, spell_index, 0);
            spellAPI.removeDLLNode(_this, spell_index);
        }
    }
    spellAPI.setEffectDone(_this, spell_index, 0);
}

void __thiscall effect_ability_warcry(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    uint16_t source_index = spell->source.entity_index;

    if (spell->target.entity_type == 1)
    {
        uint32_t tick_count =  spellAPI.addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1);
        if (tick_count == 1)
        {
            SF_CGdResourceSpell spell_data;
            spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);

            _this->active_spell_list[spell_index].to_do_count = (spell_data.params[0] * 10) / 1000;

            SF_Rectangle rect = {0,0};
            SF_Coord caster_pos = _this->SF_CGdFigure->figures[source_index].position;
            //spellAPI.getTargetsRectangle(_this, &rect, spell_index, spell_data.params[3], &caster_pos);

            SF_CGdTargetData source_data;
            source_data.entity_index = source_index;
            source_data.entity_type = 1;
            source_data.position = {0,0};

            spellAPI.addVisualEffect(_this, spell_index, kGdEffectSpellHitTarget, &rect, &source_data,
                                     _this->OpaqueClass->current_step, 100, &rect);

            _this->SF_CGdFigure->figures[source_index].flags |=  F_CHECK_SPELLS_BEFORE_JOB;
            spell->flags |= CHECK_SPELLS_BEFORE_JOB2;

            CGdFigureIterator iter;
            iteratorAPI.setupFigureIterator(&iter, _this);
            iteratorAPI.iteratorSetArea(&iter,  &caster_pos, spell_data.params[3]);
            std::vector<uint16_t> targets;
            std::vector<uint16_t> meh_targets;
            uint16_t next_figure = iteratorAPI.getNextFigure(&iter);
            while (next_figure != 0)
            {
                if ((_this->SF_CGdFigure->figures[next_figure].owner != (uint16_t)(-1)) &&
                    ((_this->SF_CGdFigure->figures[next_figure].flags & (IS_DEAD|RESESRVED_ONLY)) == 0))
                {
                    if ((next_figure != source_index) &&
                        (toolboxAPI.figuresCheckFriendly(_this->SF_CGdFigureToolBox, source_index, next_figure)) &&
                        (toolboxAPI.isTargetable(_this->SF_CGdFigureToolBox, next_figure)) &&
                        (!toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, next_figure, kGdSpellLineAbilityWarCry)))
                    {
                        if (toolboxAPI.isUnitMelee(_this->SF_CGdFigureToolBox, next_figure))
                        {
                            targets.push_back(next_figure);
                        }
                        else
                        {
                            meh_targets.push_back(next_figure);
                        }

                    }
                }
                if (targets.size() < spell_data.params[2])
                {
                    next_figure = iteratorAPI.getNextFigure(&iter);
                }
                else
                {
                    break;
                }
            }
            iteratorAPI.disposeFigureIterator(&iter);
            if (targets.size() < spell_data.params[2])
            {
                uint16_t count = spell_data.params[2] - targets.size();
                while (meh_targets.size() > 0 && count > 0)
                {
                    uint16_t add_target = meh_targets.back();
                    targets.push_back(add_target);
                    meh_targets.pop_back();
                    count--;
                }
            }
            while (targets.size() > 0)
            {
                uint16_t target_index = targets.back();

                _this->SF_CGdFigure->figures[target_index].flags |=  F_CHECK_SPELLS_BEFORE_JOB;

                SF_CGdTargetData target_data;
                target_data.entity_index = target_index;
                target_data.entity_type = 1;
                target_data.position = {0,0};
                spellAPI.addVisualEffect(_this, spell_index, kGdEffectSpellHitTarget, &rect, &target_data,
                                         _this->OpaqueClass->current_step, 100, &rect);
                toolboxAPI.addSpellToFigure(_this->SF_CGdFigureToolBox, target_index, spell_index);
                targets.pop_back();
            }
            return;
        }
        for (int i = 0; i<_this->SF_CGdFigure->max_used; i++)
        {
            if ((_this->SF_CGdFigure->figures[i].owner != (uint16_t)(-1)) &&
                ((_this->SF_CGdFigure->figures[i].flags & (IS_DEAD|RESESRVED_ONLY)) == 0))
            {
                if (toolboxAPI.removeSpellFromList(_this->SF_CGdFigureToolBox, i, spell_index))
                {
                    spellAPI.figTryClrCHkSPlBfrJob2(_this, spell_index);
                }
            }
        }
        spellAPI.setEffectDone(_this, spell_index, 0);
    }

}

void __thiscall effect_aura (SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    SF_CGdResourceSpell spell_data;
    uint16_t source_index = spell->source.entity_index;
    spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);
    uint16_t sub_effect_id = spell_data.params[6];
    SF_CGdResourceSpell sub_spell_data;
    spellAPI.getResourceSpellData(_this->SF_CGdResource, &sub_spell_data, sub_effect_id);
    _this->active_spell_list[spell_index].to_do_count = (spell_data.params[0] * 10) / 1000;

    SF_CGdTargetData source_data;
    source_data.entity_index = source_index;
    source_data.entity_type = 1;
    source_data.position = {0,0};
    if (spell->source.entity_type == 1)
    {
        if ((_this->SF_CGdFigure->figures[source_index].owner != (uint16_t)(-1)) &&
            ((*(uint8_t *)(&_this->SF_CGdFigure->figures[source_index].flags) & 0xa) == 0))
        {

            uint32_t tick = spellAPI.addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1);
            if (tick == 1)
            {
                _this->SF_CGdFigure->figures[source_index].flags |= AURA_RUNNING;
                SF_Rectangle rect = {0, 0};
                uint16_t effect_index = effectAPI.addEffect(_this->SF_CGdEffect, kGdEffectSpellHitTarget, &source_data,
                                                            &source_data,
                                                            _this->OpaqueClass->current_step, 0, &rect);

                spellAPI.setXData(_this, spell_index, EFFECT_EFFECT_INDEX, effect_index);
                effectAPI.setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_SPELL_INDEX, spell_index);
                effectAPI.setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_SPELL_ID, spell->spell_id);
            }
        }
    }
    uint16_t current_mp = figureAPI.getCurrentStat(_this->SF_CGdFigure, source_index, MANA);
    uint16_t aura_target_type = spell_data.params[5];
    uint16_t manacost = spell_data.params[8];
    if (_this->SF_CGdFigure->figures[source_index].set_type == 7)
    {
        manacost -= ((manacost * 33) / 100);
    }
    if (current_mp >= manacost)
    {
        SF_Coord caster_pos = _this->SF_CGdFigure->figures[source_index].position;
        CGdFigureIterator iter;
        iteratorAPI.setupFigureIterator(&iter, _this);
        iteratorAPI.iteratorSetArea(&iter,  &caster_pos, spell_data.params[2]);
        std::vector<uint16_t> targets;
        uint16_t target_index = iteratorAPI.getNextFigure(&iter);
        while (target_index != 0)
        {
            if ((_this->SF_CGdFigure->figures[target_index].owner != (uint16_t)(-1)) &&
                ((*(uint8_t *)(&_this->SF_CGdFigure->figures[target_index].flags) & 0xa) == 0))
            {
                //allied auras
                if (aura_target_type == 1)
                {
                    if (!toolboxAPI.figuresCheckHostile(_this->SF_CGdFigureToolBox, source_index, target_index))
                    {
                        if (_this->SF_CGdFigure->figures[target_index].owner ==
                            _this->SF_CGdFigure->figures[source_index].owner)
                        {
                            //special logic here
                            if (spell->spell_line == kGdSpellLineAuraHealing)
                            {
                                uint16_t missing_hp = _this->SF_CGdFigure->figures[target_index].health.missing_val;
                                if ((missing_hp >sub_spell_data.params[0]) &&
                                    (!toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index,
                                                              kGdSpellLineRemediless)))
                                {
                                    targets.push_back(target_index);
                                }
                                target_index = iteratorAPI.getNextFigure(&iter);
                                continue;
                            }

                            if (!toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index,
                                                         sub_spell_data.spell_line_id))
                            {
                                if (figureAPI.isWarrior(_this->SF_CGdFigure, target_index))
                                {
                                    targets.push_back(target_index);
                                }
                            }
                        }
                    }
                }
                //hostile auras
                if (aura_target_type == 2)
                {
                    if (toolboxAPI.figuresCheckHostile(_this->SF_CGdFigureToolBox, source_index, target_index))
                    {
                        if (toolboxAPI.isTargetable(_this->SF_CGdFigureToolBox, target_index))
                        {
                            if (!toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index,
                                                         sub_spell_data.spell_line_id))
                            {
                                targets.push_back(target_index);
                            }
                        }
                    }
                }

            }
            target_index = iteratorAPI.getNextFigure(&iter);
        }
        if (!targets.empty())
        {
            uint16_t aura_target = 0;
            uint16_t prio = 0;
            if (spell->spell_line == kGdSpellLineAuraHealing)
            {
                for (uint32_t j = 0; j < targets.size(); j++)
                {
                    target_index = targets.at(j);
                    SF_Coord target_pos = _this->SF_CGdFigure->figures[target_index].position;
                    uint16_t dx = 7;
                    uint16_t sec_prio = 0;
                    for (int i = 8; i> 0; i--)
                    {
                        //some serious bitfuckery
                        uint16_t near_x = *(uint16_t *)((uint32_t)&_this->SF_CGdWorld->unknown1[0].uknwn1 + dx) +
                                          target_pos.X;
                        uint16_t near_y = *(uint16_t *)((uint32_t)&_this->SF_CGdWorld->unknown1[0].uknwn2 + dx) +
                                          target_pos.Y;
                        if (((*(uint8_t *)&_this->SF_CGdWorld->cells[near_y*0x400 + near_x].world_cell_flags) & 0x10) !=
                            0)
                        {
                            uint16_t sec_target = toolboxAPI.getFigureFromWorld(_this->SF_CGdWorldToolBox, near_x,
                                                                                near_y, 1);

                            if ((sec_target != 0) && (toolboxAPI.figuresCheckHostile(_this->SF_CGdFigureToolBox,
                                                                                     target_index, sec_target)))
                            {
                                sec_prio++;
                            }
                        }
                        dx += 7;
                    }
                    sec_prio *= (100 - figureAPI.getCurrentHealthPercent(_this->SF_CGdFigure, target_index));
                    if (sec_prio > prio)
                    {
                        prio = sec_prio;
                        aura_target = targets[j];
                    }
                }
            }
            if (aura_target == 0)
            {
                uint16_t value = spellAPI.getRandom(_this->OpaqueClass, targets.size()-1);
                aura_target = targets.at(value);
            }
            apply_aura_effect(_this, spell_index, sub_effect_id, source_index, aura_target);
            figureAPI.subMana(_this->SF_CGdFigure, source_index, manacost);
        }
        iteratorAPI.disposeFigureIterator(&iter);
        return;
    }
    if (source_index != 0)
    {
        _this->SF_CGdFigure->figures[source_index].flags &= ~AURA_RUNNING;
    }
    uint16_t effect_index = spellAPI.getXData(_this, spell_index, EFFECT_EFFECT_INDEX);
    effectAPI.tryEndEffect(_this->SF_CGdEffect, effect_index);
    spellAPI.setXData(_this, spell_index, EFFECT_EFFECT_INDEX, 0);
    spellAPI.setEffectDone(_this, spell_index, 0);
    return;
}

//fix for aura set
void __thiscall effect_siege_aura (SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    SF_CGdResourceSpell spell_data;
    uint16_t source_index = spell->source.entity_index;
    spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);
    SF_CGdTargetData source_data;
    source_data.entity_index = source_index;
    source_data.entity_type = 1;
    source_data.position = {0,0};
    _this->active_spell_list[spell_index].to_do_count = (spell_data.params[0] * 10) / 1000;
    if (spell->source.entity_type == 1)
    {
        if ((_this->SF_CGdFigure->figures[source_index].owner != (uint16_t)(-1)) &&
            ((*(uint8_t *)(&_this->SF_CGdFigure->figures[source_index].flags) & 0xa) == 0))
        {
            uint32_t tick = spellAPI.addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1);
            if (tick == 1)
            {
                _this->SF_CGdFigure->figures[source_index].flags |= AURA_RUNNING;
                SF_Rectangle rect = {0, 0};
                uint16_t effect_index = effectAPI.addEffect(_this->SF_CGdEffect, kGdEffectSpellHitTarget, &source_data,
                                                            &source_data,
                                                            _this->OpaqueClass->current_step, 0, &rect);

                spellAPI.setXData(_this, spell_index, EFFECT_EFFECT_INDEX, effect_index);
                effectAPI.setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_SPELL_INDEX, spell_index);
                effectAPI.setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_SPELL_ID, spell->spell_id);
            }

        }
    }
    uint16_t current_mp = figureAPI.getCurrentStat(_this->SF_CGdFigure, source_index, MANA);
    uint16_t aura_target_type = spell_data.params[5];
    //Aura target type is either 1 or 2 for valid auras! IDK wtf is 3, but that's valid too? ~UnSchtalch
    if ((aura_target_type > 0) && (aura_target_type <= 3) && (current_mp >= spell_data.params[8]))
    {
        SF_Coord caster_pos;
        caster_pos.X = _this->SF_CGdFigure->figures[source_index].position.X;
        caster_pos.Y = _this->SF_CGdFigure->figures[source_index].position.Y;
        CGdBuildingIterator iter;
        iteratorAPI.buildingIteratorInit(&iter, 0, 0, 0x3ff, 0x3ff);
        iteratorAPI.buildingIteratorSetPointers(&iter, _this->CGdBuilding, _this->unkn3, _this->SF_CGdWorld);
        //Types are similar, to a point, but I do need to cast it

        iteratorAPI.iteratorSetArea((CGdFigureIterator *)&iter, &caster_pos, spell_data.params[2]+10);
        uint16_t building_index = iteratorAPI.getNextBuilding(&iter);
        uint16_t min_distance = 0xffff;
        uint16_t target_building = 0;
        while (building_index != 0)
        {
            //Since it's siege auras, they won't have allied buildings as targets. So I DO skip section with 1
            if (_this->CGdBuilding->buildings[building_index].owner !=
                _this->SF_CGdFigure->figures[source_index].owner)
            {
                if (buildingAPI.buildingCheckHostile(_this->SF_CGdBuildingToolbox, source_index, building_index))
                {
                    if (_this->CGdBuilding->buildings[building_index].health_current != 0)
                    {
                        SF_Rectangle building_rect;
                        buildingAPI.getBuildingClosestVertex(_this->SF_CGdBuildingToolbox, (SF_Coord *) &building_rect,
                                                             building_index,
                                                             source_index, 1);
                        SF_Coord building_pos = *(SF_Coord *)&building_rect.partA;
                        uint16_t current_distance = toolboxAPI.getDistance(&caster_pos,&building_pos);
                        if ((current_distance < min_distance) && (current_distance <= spell_data.params[2]))
                        {
                            min_distance = current_distance;
                            target_building = building_index;
                        }
                    }
                }
            }
            building_index = iteratorAPI.getNextBuilding(&iter);
        }
        if (target_building != 0)
        {
            SF_CGdTargetData building_data;
            building_data.entity_index = target_building;
            building_data.entity_type = 2;
            building_data.position = {0,0};
            SF_Rectangle rect = {0,0};
            uint16_t effect_index = effectAPI.addEffect(_this->SF_CGdEffect, kGdEffectSpellHitTarget,
                                                        &source_data, &building_data,
                                                        _this->OpaqueClass->current_step, 0x14, &rect);
            effectAPI.setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_SPELL_INDEX, spell_index);
            effectAPI.setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_SPELL_ID, spell->spell_id);
            effectAPI.setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_PHYSICAL_DAMAGE,
                                     spell_data.params[7]);
            effectAPI.setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_ADD_SUBSPELL, 1);
            effectAPI.setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_ENTITY_TYPE, 1);
            effectAPI.setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_ENTITY_INDEX, source_index);
            effectAPI.setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_ENTITY_TYPE2, 2);
            effectAPI.setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_ENTITY_INDEX2, target_building);
        }
        iteratorAPI.disposeFigureIterator((CGdFigureIterator *)&iter);
        return;
    }
    if (source_index != 0)
    {
        _this->SF_CGdFigure->figures[source_index].flags &= ~AURA_RUNNING;
    }
    uint16_t effect_index = spellAPI.getXData(_this, spell_index, EFFECT_EFFECT_INDEX);
    effectAPI.tryEndEffect(_this->SF_CGdEffect, effect_index);
    spellAPI.setXData(_this, spell_index, EFFECT_EFFECT_INDEX, 0);
    spellAPI.setEffectDone(_this, spell_index, 0);
    return;
}

void __thiscall effect_self_illusion(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    if (spell->target.entity_type == 1)
    {
        SF_CGdResourceSpell spell_data;
        CGdResourceUnitStats stats;
        memset(&stats, 0, sizeof(CGdResourceUnitStats));
        spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);

        if (spellAPI.addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1) == 1)
        {

            uint16_t source_index = spell->source.entity_index;
            uint16_t scale_factor = spell_data.params[1];
            uint16_t summon_count = spell_data.params[0];
            stats.level = _this->SF_CGdFigure->figures[source_index].level;
            stats.race = _this->SF_CGdFigure->figures[source_index].race;
            stats.head = _this->SF_CGdFigure->figures[source_index].head;

            for (int i = 0; i< 10; i++)
            {
                stats.abilities[i].id = 0xff;
                stats.abilities[i].spec = 0xff;
                stats.abilities[i].level = 0xff;
            }
            if ((_this->SF_CGdFigure->figures[source_index].flags & FEMALE) != 0)
            {
                stats.unit_flags |= 1;
            }

            stats.agility = _this->SF_CGdFigure->figures[source_index].agility.base_val;
            stats.dexterity = _this->SF_CGdFigure->figures[source_index].dexterity.base_val;
            stats.charisma = _this->SF_CGdFigure->figures[source_index].charisma.base_val;
            stats.strength = _this->SF_CGdFigure->figures[source_index].strength.base_val;
            stats.stamina = (_this->SF_CGdFigure->figures[source_index].stamina.base_val * scale_factor) / 100;
            stats.intelligence = _this->SF_CGdFigure->figures[source_index].intelligence.base_val;
            stats.wisdom = _this->SF_CGdFigure->figures[source_index].wisdom.base_val;

            stats.ires = _this->SF_CGdFigure->figures[source_index].resistance_ice.base_val;
            stats.fres = _this->SF_CGdFigure->figures[source_index].resistance_fire.base_val;
            stats.bres = _this->SF_CGdFigure->figures[source_index].resistance_black.base_val;
            stats.mres = _this->SF_CGdFigure->figures[source_index].resistance_mental.base_val;

            stats.fspeed = _this->SF_CGdFigure->figures[source_index].fight_speed.base_val;
            stats.wspeed = _this->SF_CGdFigure->figures[source_index].walk_speed.base_val;
            stats.cspeed = _this->SF_CGdFigure->figures[source_index].cast_speed.base_val;
        }
    }
}

typedef struct
{
    uint16_t figure_id;
    uint32_t missing_hp;
} healing_entry;

void __thiscall effect_ability_benefactions(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    uint16_t target_index = spell->target.entity_index;
    if ((spell->target.entity_type == 1) && (target_index != 0))
    {
        SF_CGdResourceSpell spell_data;
        spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);
        uint16_t heal = spell_data.params[0];
        uint16_t unit_count = spell_data.params[1];
        uint16_t radius = spell_data.params[2];

        if (!toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, kGdSpellLineRemediless))
        {
            uint16_t current_hp = figureAPI.getCurrentStat(_this->SF_CGdFigure, target_index, HEALTH);
            uint16_t max_hp = figureAPI.getMaxStat(_this->SF_CGdFigure, target_index, HEALTH);

            if (current_hp <= max_hp)
            {
                int16_t heal_amount = (heal * max_hp) / 100;
                if (max_hp - current_hp < heal_amount)
                {
                    heal_amount = max_hp - current_hp;
                }
                SF_CGdTargetData target_data;
                target_data.entity_index = target_index;
                target_data.entity_type = 1;
                target_data.position = {0, 0};
                SF_Rectangle rect = {0,0};
                uint32_t unused;
                spellAPI.addVisualEffect(_this, spell_index, kGdEffectSpellHitTarget, &unused, &target_data,
                                         _this->OpaqueClass->current_step, 10, &rect);
                figureAPI.subHealth(_this->SF_CGdFigure, target_index, -heal_amount);
            }
        }

        CGdFigureIterator iter;
        iteratorAPI.figureIteratorInit(&iter, 0, 0, 0x3ff,0x3ff );
        iteratorAPI.figureIteratorSetPointers(&iter, _this->SF_CGdFigure, _this->unkn3, _this->SF_CGdWorld);

        iteratorAPI.iteratorSetArea(&iter, &_this->SF_CGdFigure->figures[target_index].position, radius);

        std::vector<healing_entry> healing_targets;
        uint16_t next_figure = iteratorAPI.getNextFigure(&iter);
        while (next_figure != 0)
        {
            if ((figureAPI.isAlive(_this->SF_CGdFigure, next_figure)) &&
                (toolboxAPI.figuresCheckFriendly(_this->SF_CGdFigureToolBox, target_index, next_figure)))
            {
                uint16_t current_hp = figureAPI.getCurrentStat(_this->SF_CGdFigure, next_figure, HEALTH);
                uint16_t max_hp = figureAPI.getMaxStat(_this->SF_CGdFigure, next_figure, HEALTH);
                if (current_hp < max_hp)
                {
                    healing_entry entry = {next_figure, max_hp - current_hp};
                    healing_targets.push_back(entry);
                }
            }
            next_figure = iteratorAPI.getNextFigure(&iter);
        }
        std::sort(healing_targets.begin(), healing_targets.end(), [](const healing_entry& a, const healing_entry& b)
        {
            return a.missing_hp > b.missing_hp;
        });
        iteratorAPI.disposeFigureIterator(&iter);
        for (int i = 0; i < healing_targets.size(); i++)
        {
            if (i > unit_count)
            {
                break;
            }
            if (toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox,  healing_targets[i].figure_id,
                                        kGdSpellLineRemediless))
            {
                continue;
            }
            uint16_t current_hp = figureAPI.getCurrentStat(_this->SF_CGdFigure, healing_targets[i].figure_id, HEALTH);
            uint16_t max_hp = figureAPI.getMaxStat(_this->SF_CGdFigure, healing_targets[i].figure_id, HEALTH);

            uint16_t heal_amount = (heal * max_hp) / 100;
            if (max_hp - current_hp < heal_amount)
            {
                heal_amount = max_hp - current_hp;
            }

            SF_CGdTargetData target_data;
            target_data.entity_index = healing_targets[i].figure_id;
            target_data.entity_type = 1;
            target_data.position = {0, 0};
            SF_Rectangle rect = {0,0};
            uint32_t unused;
            spellAPI.addVisualEffect(_this, spell_index, kGdEffectSpellHitTarget, &unused, &target_data,
                                     _this->OpaqueClass->current_step, 10, &rect);
            figureAPI.subHealth(_this->SF_CGdFigure, healing_targets[i].figure_id, -heal_amount);
        }
    }
    spellAPI.setEffectDone(_this, spell_index, 0);
}

//hotfix for firebane
//TODO: fix some stuff about hasSpellOnIt
void __thiscall effect_fire_resistance (SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);
    uint16_t source_index = spell->source.entity_index;
    if (spell->target.entity_type == 5)
    {
        uint32_t tick = spellAPI.getXData(_this, spell_index, SPELL_TICK_COUNT_AUX);

        if (tick == 0)
        {
            _this->active_spell_list[spell_index].to_do_count = (spell_data.params[1] * 10) / 1000;
            spellAPI.addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1);
            int16_t multiplier = spell_data.params[0];
            (multiplier >= 127) ? (multiplier = 127) : (multiplier);
            for (int i = 1; i <= _this->SF_CGdFigure->max_used; i++)
            {
                if ((_this->SF_CGdFigure->figures[i].owner != (uint16_t)(-1)) &&
                    (_this->SF_CGdFigure->figures[i].owner == _this->SF_CGdFigure->figures[source_index].owner) &&
                    ((*(uint8_t *)(&_this->SF_CGdFigure->figures[i].flags) & 0xa) == 0))
                {
                    SF_CGdTargetData target_data;
                    target_data.entity_index = i;
                    target_data.entity_type = 1;
                    target_data.position = {0,0};
                    SF_Rectangle rect = {0,0};
                    figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, RESISTANCE_FIRE, i, multiplier);
                    spellAPI.addVisualEffect(_this, spell_index, kGdEffectSpellHitTarget, 0, &target_data,
                                             _this->OpaqueClass->current_step,
                                             0x14, &rect);
                }
            }
            return;
        }
        else
        {
            int16_t multiplier = spell_data.params[0];
            (multiplier >= 127) ? (multiplier = 127) : (multiplier);
            for (int i = 1; i <= _this->SF_CGdFigure->max_used; i++)
            {
                if ((_this->SF_CGdFigure->figures[i].owner != (uint16_t)(-1)) &&
                    (_this->SF_CGdFigure->figures[i].owner == _this->SF_CGdFigure->figures[source_index].owner) &&
                    ((*(uint8_t *)(&_this->SF_CGdFigure->figures[i].flags) & 0xa) == 0))
                {
                    figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, RESISTANCE_FIRE, i, -multiplier);
                }
            }
        }
    }
    spellAPI.setEffectDone(_this, spell_index, 0);
    return;

}
/**
 * Initializes all the vanilla effect handlers with their respective addresses.
 *
 * This function assigns the addresses of various effect handlers to their respective variables.
 * The addresses are obtained using Ghidra Reverse Engineering.
 */
void initialize_vanilla_effect_handler_hooks()
{
    effect_ability_benefactions_handler = &effect_ability_benefactions;// (handler_ptr)(ASI::AddrOf(0x32c090));
    effect_ability_berserk_handler = (handler_ptr)(ASI::AddrOf(0x32c4a0));
    effect_ability_boons_handler = (handler_ptr)(ASI::AddrOf(0x32c670));
    effect_ability_critical_hits_handler = (handler_ptr)(ASI::AddrOf(0x32c810));
    effect_ability_durability_handler = (handler_ptr)(ASI::AddrOf(0x32c9c0));
    effect_ability_endurance_handler = (handler_ptr)(ASI::AddrOf(0x32cb90));
    effect_ability_patronize_handler = (handler_ptr)(ASI::AddrOf(0x32cf60));
    effect_ability_riposte_handler = (handler_ptr)(ASI::AddrOf(0x32d330));
    effect_ability_salvo_handler = (handler_ptr)(ASI::AddrOf(0x32d4e0));
    effect_ability_shelter_handler = (handler_ptr)(ASI::AddrOf(0x32d6b0));
    effect_ability_shift_life_handler = (handler_ptr)(ASI::AddrOf(0x32d880));
    effect_ability_steelskin_handler = (handler_ptr)(ASI::AddrOf(0x32dbf0));
    effect_ability_trueshot_handler = (handler_ptr)(ASI::AddrOf(0x32ddc0));
    effect_ability_warcry_handler = &effect_ability_warcry;// (handler_ptr)(ASI::AddrOf(0x32df90));
    effect_acid_cloud_handler = (handler_ptr)(ASI::AddrOf(0x32e370));
    effect_almightiness_black_handler = (handler_ptr)(ASI::AddrOf(0x32e730));
    effect_almightiness_elemental_handler = (handler_ptr)(ASI::AddrOf(0x32e9d0));
    effect_almightiness_elemental2_handler = (handler_ptr)(ASI::AddrOf(0x32eca0));
    effect_almightiness_mental_handler = (handler_ptr)(ASI::AddrOf(0x32f050));
    effect_almightiness_white_handler = (handler_ptr)(ASI::AddrOf(0x32f330));
    effect_amok_handler = (handler_ptr)(ASI::AddrOf(0x32f590));
    effect_tower_arrow_handler = (handler_ptr)(ASI::AddrOf(0x32f840));
    effect_assistance_handler = (handler_ptr)(ASI::AddrOf(0x32fbc0));
    //effect_aura_handler = (handler_ptr)(ASI::AddrOf(0x32fd40));
    effect_aura_handler = &effect_aura; // hotfix lives here
    effect_siege_aura_handler = &effect_siege_aura;
    effect_befriend_handler = (handler_ptr)(ASI::AddrOf(0x3309b0));
    effect_unknown1_handler = (handler_ptr)(ASI::AddrOf(0x330bc0));
    effect_blizzard_handler = (handler_ptr)(ASI::AddrOf(0x330e00));
    effect_brilliance_handler = (handler_ptr)(ASI::AddrOf(0x331190));
    effect_cannibalize_handler = (handler_ptr)(ASI::AddrOf(0x3313a0));
    effect_chain_handler = (handler_ptr)(ASI::AddrOf(0x331510));
    effect_charisma_handler = (handler_ptr)(ASI::AddrOf(0x331bf0));
    effect_charm_handler = (handler_ptr)(ASI::AddrOf(0x331dc0));
    effect_charm_animal_handler = (handler_ptr)(ASI::AddrOf(0x332280));
    effect_chill_resistance_handler = (handler_ptr)(ASI::AddrOf(0x332750));
    effect_confuse_handler = (handler_ptr)(ASI::AddrOf(0x3329a0));
    effect_confuse_area_handler = (handler_ptr)(ASI::AddrOf(0x332c30));
    effect_conservation_handler = &effect_conservation;// (handler_ptr)(ASI::AddrOf(0x333130));
    effect_cure_disease_handler = (handler_ptr)(ASI::AddrOf(0x333360));
    effect_cure_poison_handler = (handler_ptr)(ASI::AddrOf(0x333560));
    effect_dark_banishing_handler = (handler_ptr)(ASI::AddrOf(0x333760));
    effect_darkness_area_handler = (handler_ptr)(ASI::AddrOf(0x3339b0));
    effect_death_handler = (handler_ptr)(ASI::AddrOf(0x333db0));
    effect_death_grasp_handler = (handler_ptr)(ASI::AddrOf(0x333fc0));
    effect_decay_handler = (handler_ptr)(ASI::AddrOf(0x334150));
    effect_decay2_handler = (handler_ptr)(ASI::AddrOf(0x334390));
    effect_demoralization_handler = (handler_ptr)(ASI::AddrOf(0x334760));
    effect_detect_magic_handler = (handler_ptr)(ASI::AddrOf(0x334a20));
    effect_detect_metal_handler = (handler_ptr)(ASI::AddrOf(0x334cb0));
    effect_dexterity_handler = (handler_ptr)(ASI::AddrOf(0x334f30));
    effect_disenchant_handler = (handler_ptr)(ASI::AddrOf(0x335180));
    effect_dispel_black_aura_handler = (handler_ptr)(ASI::AddrOf(0x3353d0));
    effect_dispel_white_aura_handler = (handler_ptr)(ASI::AddrOf(0x335680));
    effect_disrupt_handler = (handler_ptr)(ASI::AddrOf(0x335930));
    effect_distract_handler = (handler_ptr)(ASI::AddrOf(0x335c30));
    effect_dominate_break_handler = (handler_ptr)(ASI::AddrOf(0x335f40));
    effect_dominate_handler = (handler_ptr)(ASI::AddrOf(0x336060));
    effect_dominate_animal_handler = (handler_ptr)(ASI::AddrOf(0x336500));
    effect_dominate_undead_handler = (handler_ptr)(ASI::AddrOf(0x336a10));
    effect_endurance_handler = (handler_ptr)(ASI::AddrOf(0x336e40));
    effect_enlightement_handler = (handler_ptr)(ASI::AddrOf(0x3370f0));
    effect_essence_black_handler = (handler_ptr)(ASI::AddrOf(0x337450));
    effect_essence_elemental_handler = (handler_ptr)(ASI::AddrOf(0x3376f0));
    effect_essence_elemental2_handler = (handler_ptr)(ASI::AddrOf(0x3379c0));
    effect_essence_mental_handler = (handler_ptr)(ASI::AddrOf(0x337d20));
    effect_essence_white_handler = (handler_ptr)(ASI::AddrOf(0x338000));
    effect_eternity_handler = (handler_ptr)(ASI::AddrOf(0x338260));
    effect_extinct_handler = (handler_ptr)(ASI::AddrOf(0x338670));
    effect_tower_extinct_handler = (handler_ptr)(ASI::AddrOf(0x338a50));
    effect_fake_spell_one_figure_handler = (handler_ptr)(ASI::AddrOf(0x338e00));
    effect_fast_fighting_handler = (handler_ptr)(ASI::AddrOf(0x338e90));
    effect_feet_of_clay_handler = (handler_ptr)(ASI::AddrOf(0x3390e0));
    effect_feign_death_handler = (handler_ptr)(ASI::AddrOf(0x339590));
    effect_fireball1_handler = (handler_ptr)(ASI::AddrOf(0x339750));
    effect_fireball2_handler = (handler_ptr)(ASI::AddrOf(0x339a20));
    effect_firebullet_handler = (handler_ptr)(ASI::AddrOf(0x339e00));
    effect_fireburst_handler = (handler_ptr)(ASI::AddrOf(0x339fc0));
    effect_fire_resistance_handler = &effect_fire_resistance;
    effect_fireshield1_handler = (handler_ptr)(ASI::AddrOf(0x33a610));
    effect_fireshield2_handler = (handler_ptr)(ASI::AddrOf(0x33a7b0));
    effect_flexibility_handler = (handler_ptr)(ASI::AddrOf(0x33a970));
    effect_flexibility_area_handler = (handler_ptr)(ASI::AddrOf(0x33abc0));
    effect_fog_handler = (handler_ptr)(ASI::AddrOf(0x33aef0));
    effect_forget_handler = (handler_ptr)(ASI::AddrOf(0x33b240));
    effect_freeze_handler = (handler_ptr)(ASI::AddrOf(0x33b4b0));
    effect_freeze_area_handler = (handler_ptr)(ASI::AddrOf(0x33b740));
    effect_guard_handler = (handler_ptr)(ASI::AddrOf(0x33bad0));
    effect_hallow_handler = (handler_ptr)(ASI::AddrOf(0x33bc60));
    effect_healing_handler = (handler_ptr)(ASI::AddrOf(0x33c070));
    effect_healing_area_handler = (handler_ptr)(ASI::AddrOf(0x33c250));
    effect_unknown2_handler = (handler_ptr)(ASI::AddrOf(0x33c5f0));
    effect_healing_aura_handler = (handler_ptr)(ASI::AddrOf(0x33c770));
    effect_holy_touch_handler = (handler_ptr)(ASI::AddrOf(0x33c8c0));
    effect_iceshield_handler = (handler_ptr)(ASI::AddrOf(0x33c980));
    effect_hypnotize_area_handler = (handler_ptr)(ASI::AddrOf(0x33cd90));
    effect_hypnotize_handler = (handler_ptr)(ASI::AddrOf(0x33d3f0));
    effect_ice1_handler = (handler_ptr)(ASI::AddrOf(0x33d7d0));
    effect_ice2_handler = (handler_ptr)(ASI::AddrOf(0x33da90));
    effect_icestrike1_handler = (handler_ptr)(ASI::AddrOf(0x33dd50));
    effect_iceshield2_handler = (handler_ptr)(ASI::AddrOf(0x33e210));
    effect_iceshield3_handler = (handler_ptr)(ASI::AddrOf(0x33e3b0));
    effect_icestrike2_handler = (handler_ptr)(ASI::AddrOf(0x33e620));
    effect_illuminate_handler = (handler_ptr)(ASI::AddrOf(0x33ea50));
    effect_inability_handler = (handler_ptr)(ASI::AddrOf(0x33ed50));
    effect_inflexibility_handler = (handler_ptr)(ASI::AddrOf(0x33ef50));
    effect_inflexibility_area_handler = (handler_ptr)(ASI::AddrOf(0x33f150));
    effect_invisibility_handler = (handler_ptr)(ASI::AddrOf(0x33f530));
    effect_invulnerability_handler = (handler_ptr)(ASI::AddrOf(0x33f7d0));
    effect_lifetap_handler = (handler_ptr)(ASI::AddrOf(0x33f990));
    effect_lifetap_aura_handler = (handler_ptr)(ASI::AddrOf(0x33fbf0));
    effect_manadrain_handler = (handler_ptr)(ASI::AddrOf(0x33fdf0));
    effect_manashield_handler = (handler_ptr)(ASI::AddrOf(0x340000));
    effect_manatap_handler = (handler_ptr)(ASI::AddrOf(0x3401a0));
    effect_manatap_aura_handler = (handler_ptr)(ASI::AddrOf(0x3403d0));
    effect_meditation_handler = (handler_ptr)(ASI::AddrOf(0x340600));
    effect_melt_resistance_handler = (handler_ptr)(ASI::AddrOf(0x3406d0));
    effect_mirage_handler = (handler_ptr)(ASI::AddrOf(0x340920));
    effect_mutation_handler = (handler_ptr)(ASI::AddrOf(0x340e60));
    effect_pain_handler = (handler_ptr)(ASI::AddrOf(0x3418c0));
    effect_pain_area_handler = (handler_ptr)(ASI::AddrOf(0x341af0));
    effect_tower_pain_handler = (handler_ptr)(ASI::AddrOf(0x341e90));
    effect_pestilence_handler = (handler_ptr)(ASI::AddrOf(0x342060));
    effect_petrify_handler = (handler_ptr)(ASI::AddrOf(0x342440));
    effect_plague_area_handler = (handler_ptr)(ASI::AddrOf(0x342700));
    effect_poison_handler = (handler_ptr)(ASI::AddrOf(0x342be0));
    effect_cloak_of_nor_handler = (handler_ptr)(ASI::AddrOf(0x343020));
    effect_quickness_handler = (handler_ptr)(ASI::AddrOf(0x3431b0));
    effect_quickness_area_handler = (handler_ptr)(ASI::AddrOf(0x343400));
    effect_rain_of_fire_handler = (handler_ptr)(ASI::AddrOf(0x343730));
    effect_raise_dead_handler = (handler_ptr)(ASI::AddrOf(0x343ac0));
    effect_regenerate_handler = (handler_ptr)(ASI::AddrOf(0x343f60));
    effect_reinforcement_handler = (handler_ptr)(ASI::AddrOf(0x344170));
    effect_remediless_handler = (handler_ptr)(ASI::AddrOf(0x344750));
    effect_remove_curse_handler = (handler_ptr)(ASI::AddrOf(0x3449a0));
    effect_retention_handler = (handler_ptr)(ASI::AddrOf(0x344bb0));
    effect_revenge_handler = (handler_ptr)(ASI::AddrOf(0x344dc0));
    effect_rock_bullet_handler = (handler_ptr)(ASI::AddrOf(0x3453f0));
    effect_roots_handler = (handler_ptr)(ASI::AddrOf(0x345880));
    effect_roots_area_handler = (handler_ptr)(ASI::AddrOf(0x345c60));
    effect_sacrifice_mana_handler = (handler_ptr)(ASI::AddrOf(0x346240));
    effect_self_illusion_handler = (handler_ptr)(ASI::AddrOf(0x3463c0));
    effect_sentinel_healing_handler = (handler_ptr)(ASI::AddrOf(0x346ab0));
    effect_shift_mana_handler = (handler_ptr)(ASI::AddrOf(0x346f70));
    effect_shock_handler = (handler_ptr)(ASI::AddrOf(0x347310));
    effect_shockwave_handler = (handler_ptr)(ASI::AddrOf(0x347580));
    effect_slow_fighting_handler = (handler_ptr)(ASI::AddrOf(0x347980));
    effect_slowness_handler = (handler_ptr)(ASI::AddrOf(0x347b80));
    effect_slowness_area_handler = (handler_ptr)(ASI::AddrOf(0x347dd0));
    effect_spark_handler = (handler_ptr)(ASI::AddrOf(0x3481a0));
    effect_stone_handler = (handler_ptr)(ASI::AddrOf(0x348530));
    effect_rain_of_stone_handler = (handler_ptr)(ASI::AddrOf(0x348640));
    effect_tower_stone_handler = (handler_ptr)(ASI::AddrOf(0x3489d0));
    effect_strength_handler = (handler_ptr)(ASI::AddrOf(0x348db0));
    effect_strength_area_handler = (handler_ptr)(ASI::AddrOf(0x349000));
    effect_suffocation_handler = (handler_ptr)(ASI::AddrOf(0x349330));
    effect_suicide_death_handler = (handler_ptr)(ASI::AddrOf(0x349570));
    effect_suicide_heal_handler = (handler_ptr)(ASI::AddrOf(0x3497a0));
    effect_summon_handler = (handler_ptr)(ASI::AddrOf(0x3499b0));
    effect_thornshield_handler = (handler_ptr)(ASI::AddrOf(0x349bb0));
    effect_thorns_handler = (handler_ptr)(ASI::AddrOf(0x349d70));
    effect_torture_handler = (handler_ptr)(ASI::AddrOf(0x349f40));
    effect_feedback_handler = (handler_ptr)(ASI::AddrOf(0x34a3f0));
    effect_wave_handler = (handler_ptr)(ASI::AddrOf(0x34a5a0));
    effect_weaken_handler = (handler_ptr)(ASI::AddrOf(0x34aa10));
    effect_weaken_area_handler = (handler_ptr)(ASI::AddrOf(0x34ac60));
}
