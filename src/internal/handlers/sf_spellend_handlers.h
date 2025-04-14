#pragma once

#include "../../api/sfsf.h"
#include "../../api/sf_general_structures.h"

void __thiscall default_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall command_handler_removespell(SF_CGdSpell *_this,
                                            uint16_t spell_index);
void __thiscall slowness_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall poison_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall common_handler_check_battle(SF_CGdSpell *_this,
                                            uint16_t spell_index);
void __thiscall cure_poison_end_handler(SF_CGdSpell *_this,
                                        uint16_t spell_index);
void __thiscall common_handler_unfreeze(SF_CGdSpell *_this,
                                        uint16_t spell_index);
void __thiscall common_handler_check_unfreeze(SF_CGdSpell *_this,
                                              uint16_t spell_index);                     // For Freeze and Petrify
void __thiscall common_handler_check_unfreeze2(SF_CGdSpell *_this,
                                               uint16_t spell_index);
void __thiscall common_handler_done(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall fog_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall common_handler_job_battle_check2(SF_CGdSpell *_this,
                                                 uint16_t spell_index);
void __thiscall common_handler_job_battle_check(SF_CGdSpell *_this,
                                                uint16_t spell_index);
void __thiscall fireball_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall iceshield_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall decay1_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall decay2_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall pain_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall lifetap_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall summons_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall hypnotize_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall iceshield2_end_handler(SF_CGdSpell *_this,
                                       uint16_t spell_index);
void __thiscall pestilence_end_handler(SF_CGdSpell *_this,
                                       uint16_t spell_index);
void __thiscall cure_disease_end_handler(SF_CGdSpell *_this,
                                         uint16_t spell_index);
void __thiscall area_pain_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall raise_dead_end_handler(SF_CGdSpell *_this,
                                       uint16_t spell_index);
void __thiscall death_grasp_end_handler(SF_CGdSpell *_this,
                                        uint16_t spell_index);
void __thiscall inflexibility_end_handler(SF_CGdSpell *_this,
                                          uint16_t spell_index);
void __thiscall weaken_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall dark_banishing_end_handler(SF_CGdSpell *_this,
                                           uint16_t spell_index);
void __thiscall area_slowness_end_handler(SF_CGdSpell *_this,
                                          uint16_t spell_index);
void __thiscall area_inflexibility_end_handler(SF_CGdSpell *_this,
                                               uint16_t spell_index);
void __thiscall area_weaken_end_handler(SF_CGdSpell *_this,
                                        uint16_t spell_index);
void __thiscall area_plague_end_handler(SF_CGdSpell *_this,
                                        uint16_t spell_index);
void __thiscall area_healing_end_handler(SF_CGdSpell *_this,
                                         uint16_t spell_index);
void __thiscall sentinel_healing_end_handler(SF_CGdSpell *_this,
                                             uint16_t spell_index);
void __thiscall charm_animal_end_handler(SF_CGdSpell *_this,
                                         uint16_t spell_index);
void __thiscall thorn_shield_end_handler(SF_CGdSpell *_this,
                                         uint16_t spell_index);
void __thiscall quickness_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall area_quickness_end_handler(SF_CGdSpell *_this,
                                           uint16_t spell_index);
void __thiscall flexibility_end_handler(SF_CGdSpell *_this,
                                        uint16_t spell_index);
void __thiscall area_flexibility_end_handler(SF_CGdSpell *_this,
                                             uint16_t spell_index);
void __thiscall strength_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall area_strength_end_handler(SF_CGdSpell *_this,
                                          uint16_t spell_index);
void __thiscall guard_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall remove_curse_end_handler(SF_CGdSpell *_this,
                                         uint16_t spell_index);
void __thiscall regenerate_end_handler(SF_CGdSpell *_this,
                                       uint16_t spell_index);
void __thiscall hallow_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall fireshield2_end_handler(SF_CGdSpell *_this,
                                        uint16_t spell_index);
void __thiscall thorn_shield2_end_handler(SF_CGdSpell *_this,
                                          uint16_t spell_index);
void __thiscall forget_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall self_illusion_end_handler(SF_CGdSpell *_this,
                                          uint16_t spell_index);
void __thiscall retention_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall brilliance_end_handler(SF_CGdSpell *_this,
                                       uint16_t spell_index);
void __thiscall sacrifice_mana_end_handler(SF_CGdSpell *_this,
                                           uint16_t spell_index);
void __thiscall manatap_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall manadrain_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall shock_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall disrupt_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall confuse_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall rain_of_fire_end_handler(SF_CGdSpell *_this,
                                         uint16_t spell_index);
void __thiscall blizzard_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall acid_cloud_end_handler(SF_CGdSpell *_this,
                                       uint16_t spell_index);
void __thiscall stone_rain_end_handler(SF_CGdSpell *_this,
                                       uint16_t spell_index);
void __thiscall amok_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall extinct_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall detect_metal_end_handler(SF_CGdSpell *_this,
                                         uint16_t spell_index);
void __thiscall detect_magic_end_handler(SF_CGdSpell *_this,
                                         uint16_t spell_index);
void __thiscall invisibility_end_handler(SF_CGdSpell *_this,
                                         uint16_t spell_index);
void __thiscall stone_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall aura_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall suicide_death_end_handler(SF_CGdSpell *_this,
                                          uint16_t spell_index);
void __thiscall dispel_white_aura_end_handler(SF_CGdSpell *_this,
                                              uint16_t spell_index);
void __thiscall suffocation_end_handler(SF_CGdSpell *_this,
                                        uint16_t spell_index);
void __thiscall inablility_end_handler(SF_CGdSpell *_this,
                                       uint16_t spell_index);
void __thiscall slow_fighting_end_handler(SF_CGdSpell *_this,
                                          uint16_t spell_index);
void __thiscall suicide_heal_end_handler(SF_CGdSpell *_this,
                                         uint16_t spell_index);
void __thiscall dominate_animal_end_handler(SF_CGdSpell *_this,
                                            uint16_t spell_index);
void __thiscall dispel_black_aura_end_handler(SF_CGdSpell *_this,
                                              uint16_t spell_index);
void __thiscall dexterity_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall edurance_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall fast_fighting_end_handler(SF_CGdSpell *_this,
                                          uint16_t spell_index);
void __thiscall distract_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall dominate_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall charm_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall befriend_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall disenchant_end_handler(SF_CGdSpell *_this,
                                       uint16_t spell_index);
void __thiscall charisma_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall shockwave_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall enlightenment_end_handler(SF_CGdSpell *_this,
                                          uint16_t spell_index);
void __thiscall meditation_end_handler(SF_CGdSpell *_this,
                                       uint16_t spell_index);
void __thiscall wave_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall melt_resistance_end_handler(SF_CGdSpell *_this,
                                            uint16_t spell_index);
void __thiscall chill_resistance_end_handler(SF_CGdSpell *_this,
                                             uint16_t spell_index);
void __thiscall rock_bullet_end_handler(SF_CGdSpell *_this,
                                        uint16_t spell_index);
void __thiscall conservation_end_handler(SF_CGdSpell *_this,
                                         uint16_t spell_index);
void __thiscall tower_arrow_end_handler(SF_CGdSpell *_this,
                                        uint16_t spell_index);
void __thiscall tower_healing_end_handler(SF_CGdSpell *_this,
                                          uint16_t spell_index);
void __thiscall tower_icestrike_end_handler(SF_CGdSpell *_this,
                                            uint16_t spell_index);
void __thiscall lifetap_aura_end_handler(SF_CGdSpell *_this,
                                         uint16_t spell_index);
void __thiscall fireball2_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall ability_warcry_end_handler(SF_CGdSpell *_this,
                                           uint16_t spell_index);
void __thiscall ability_benefactions_end_handler(SF_CGdSpell *_this,
                                                 uint16_t spell_index);
void __thiscall ability_patronize_end_handler(SF_CGdSpell *_this,
                                              uint16_t spell_index);
void __thiscall ability_endurance_end_handler(SF_CGdSpell *_this,
                                              uint16_t spell_index);
void __thiscall ability_berserk_end_handler(SF_CGdSpell *_this,
                                            uint16_t spell_index);
void __thiscall ability_boons_end_handler(SF_CGdSpell *_this,
                                          uint16_t spell_index);
void __thiscall ability_shelter_end_handler(SF_CGdSpell *_this,
                                            uint16_t spell_index);
void __thiscall ability_durability_end_handler(SF_CGdSpell *_this,
                                               uint16_t spell_index);
void __thiscall ability_trueshot_end_handler(SF_CGdSpell *_this,
                                             uint16_t spell_index);
void __thiscall ability_steelskin_end_handler(SF_CGdSpell *_this,
                                              uint16_t spell_index);
void __thiscall ability_salvo_end_handler(SF_CGdSpell *_this,
                                          uint16_t spell_index);
void __thiscall spark_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall tower_hypnotize_end_handler(SF_CGdSpell *_this,
                                            uint16_t spell_index);
void __thiscall tower_pain_end_handler(SF_CGdSpell *_this,
                                       uint16_t spell_index);
void __thiscall tower_stone_end_handler(SF_CGdSpell *_this,
                                        uint16_t spell_index);
void __thiscall cloak_of_nor_end_handler(SF_CGdSpell *_this,
                                         uint16_t spell_index);
void __thiscall unkn_spell2_end_handler(SF_CGdSpell *_this,
                                        uint16_t spell_index);
void __thiscall freeze2_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall freeze3_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall lava_bullet_end_handler(SF_CGdSpell *_this,
                                        uint16_t spell_index);
void __thiscall tower_extinct_end_handler(SF_CGdSpell *_this,
                                          uint16_t spell_index);
void __thiscall manatap_aura_end_handler(SF_CGdSpell *_this,
                                         uint16_t spell_index);
void __thiscall firebane_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall black_essence_end_handler(SF_CGdSpell *_this,
                                          uint16_t spell_index);
void __thiscall white_essence_end_handler(SF_CGdSpell *_this,
                                          uint16_t spell_index);
void __thiscall chain_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall eternity_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall elemental_essence_end_handler(SF_CGdSpell *_this,
                                              uint16_t spell_index);
void __thiscall mental_essence_end_handler(SF_CGdSpell *_this,
                                           uint16_t spell_index);
void __thiscall black_almightness_end_handler(SF_CGdSpell *_this,
                                              uint16_t spell_index);
void __thiscall white_almightness_end_handler(SF_CGdSpell *_this,
                                              uint16_t spell_index);
void __thiscall elemental_almightness_end_handler(SF_CGdSpell *_this,
                                                  uint16_t spell_index);
void __thiscall mental_almightness_end_handler(SF_CGdSpell *_this,
                                               uint16_t spell_index);
void __thiscall elemental_almightness2_end_handler(SF_CGdSpell *_this,
                                                   uint16_t spell_index);
void __thiscall elemental_essence2_end_handler(SF_CGdSpell *_this,
                                               uint16_t spell_index);
void __thiscall assistance_end_handler(SF_CGdSpell *_this,
                                       uint16_t spell_index);
void __thiscall holy_touch_end_handler(SF_CGdSpell *_this,
                                       uint16_t spell_index);
void __thiscall revenge_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall area_roots_end_handler(SF_CGdSpell *_this,
                                       uint16_t spell_index);
void __thiscall roots_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall reinforcement_end_handler(SF_CGdSpell *_this,
                                          uint16_t spell_index);
void __thiscall cannibalize_end_handler(SF_CGdSpell *_this,
                                        uint16_t spell_index);
void __thiscall torture_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall dominate_undead_end_handler(SF_CGdSpell *_this,
                                            uint16_t spell_index);
void __thiscall mutation_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall area_darkness_end_handler(SF_CGdSpell *_this,
                                          uint16_t spell_index);
void __thiscall area_freeze_end_handler(SF_CGdSpell *_this,
                                        uint16_t spell_index);
void __thiscall clay_feet_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall mirage_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall feedback_end_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall area_hypnotize_end_handler(SF_CGdSpell *_this,
                                           uint16_t spell_index);
void __thiscall area_confuse_end_handler(SF_CGdSpell *_this,
                                         uint16_t spell_index);
void __thiscall shift_mana_end_handler(SF_CGdSpell *_this,
                                       uint16_t spell_index);
void __thiscall ability_shift_life_end_handler(SF_CGdSpell *_this,
                                               uint16_t spell_index);
void __thiscall ability_riposte_end_handler(SF_CGdSpell *_this,
                                            uint16_t spell_index);
void __thiscall ability_critical_hits_end_handler(SF_CGdSpell *_this,
                                                  uint16_t spell_index);
void __thiscall belial_effect_end_handler(SF_CGdSpell *_this,
                                          uint16_t spell_index);
