#ifndef SF_SPELLTYPE_HANDLER_H
#define SF_SPELLTYPE_HANDLER_H

#include "../../api/sfsf.h"
#include "../../api/sf_general_structures.h"
#include "../../api/sf_spell_functions.h"

void __thiscall default_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall fireburst_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall icestrike_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall healing_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall death_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall slowness_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall poison_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall invulnerability_handler(SF_CGdSpell *_this,
                                        uint16_t spell_index);
void __thiscall cure_poison_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall freeze_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall fog_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall illuminate_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall fireshield_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall fireball_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall iceshield_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall decay1_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall decay2_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall pain_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall lifetap_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall summons_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall hypnotize_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall iceshield2_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall iceshield3_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall pestilence_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall cure_disease_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall petrify_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall area_pain_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall raise_dead_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall death_grasp_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall inflexibility_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall weaken_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall dark_banishing_handler(SF_CGdSpell *_this,
                                       uint16_t spell_index);
void __thiscall area_slowness_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall area_inflexibility_handler(SF_CGdSpell *_this,
                                           uint16_t spell_index);
void __thiscall area_weaken_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall area_plague_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall remediless_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall area_healing_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall sentinel_healing_handler(SF_CGdSpell *_this,
                                         uint16_t spell_index);
void __thiscall charm_animal_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall thorn_shield_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall quickness_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall area_quickness_handler(SF_CGdSpell *_this,
                                       uint16_t spell_index);
void __thiscall flexibility_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall area_flexibility_handler(SF_CGdSpell *_this,
                                         uint16_t spell_index);
void __thiscall strength_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall area_strength_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall guard_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall remove_curse_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall regenerate_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall hallow_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall fireshield2_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall thorn_shield2_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall forget_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall self_illusion_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall retention_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall brilliance_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall sacrifice_mana_handler(SF_CGdSpell *_this,
                                       uint16_t spell_index);
void __thiscall manatap_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall manadrain_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall shock_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall disrupt_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall confuse_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall rain_of_fire_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall blizzard_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall acid_cloud_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall stone_rain_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall amok_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall extinct_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall detect_metal_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall detect_magic_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall unkn_spell_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall invisibility_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall stone_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall aura_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall suicide_death_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall feign_death_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall dispel_white_aura_handler(SF_CGdSpell *_this,
                                          uint16_t spell_index);
void __thiscall suffocation_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall inablility_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall slow_fighting_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall suicide_heal_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall dominate_animal_handler(SF_CGdSpell *_this,
                                        uint16_t spell_index);
void __thiscall dispel_black_aura_handler(SF_CGdSpell *_this,
                                          uint16_t spell_index);
void __thiscall dexterity_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall endurance_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall fast_fighting_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall distract_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall dominate_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall charm_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall befriend_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall disenchant_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall charisma_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall shockwave_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall demoralization_handler(SF_CGdSpell *_this,
                                       uint16_t spell_index);
void __thiscall enlightenment_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall meditation_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall wave_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall melt_resistance_handler(SF_CGdSpell *_this,
                                        uint16_t spell_index);
void __thiscall chill_resistance_handler(SF_CGdSpell *_this,
                                         uint16_t spell_index);
void __thiscall rock_bullet_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall conservation_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall tower_arrow_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall tower_healing_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall tower_icestrike_handler(SF_CGdSpell *_this,
                                        uint16_t spell_index);
void __thiscall lifetap_aura_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall fireball2_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall ability_warcry_handler(SF_CGdSpell *_this,
                                       uint16_t spell_index);
void __thiscall ability_benefactions_handler(SF_CGdSpell *_this,
                                             uint16_t spell_index);
void __thiscall ability_patronize_handler(SF_CGdSpell *_this,
                                          uint16_t spell_index);
void __thiscall ability_endurance_handler(SF_CGdSpell *_this,
                                          uint16_t spell_index);
void __thiscall ability_berserk_handler(SF_CGdSpell *_this,
                                        uint16_t spell_index);
void __thiscall ability_boons_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall ability_shelter_handler(SF_CGdSpell *_this,
                                        uint16_t spell_index);
void __thiscall ability_durability_handler(SF_CGdSpell *_this,
                                           uint16_t spell_index);
void __thiscall ability_trueshot_handler(SF_CGdSpell *_this,
                                         uint16_t spell_index);
void __thiscall ability_steelskin_handler(SF_CGdSpell *_this,
                                          uint16_t spell_index);
void __thiscall ability_salvo_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall spark_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall tower_hypnotize_handler(SF_CGdSpell *_this,
                                        uint16_t spell_index);
void __thiscall tower_pain_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall tower_stone_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall cloak_of_nor_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall unkn_spell2_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall freeze2_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall freeze3_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall lava_bullet_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall tower_extinct_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall manatap_aura_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall firebane_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall black_essence_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall white_essence_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall chain_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall eternity_aura_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall elemental_essence_handler(SF_CGdSpell *_this,
                                          uint16_t spell_index);
void __thiscall mental_essence_handler(SF_CGdSpell *_this,
                                       uint16_t spell_index);
void __thiscall black_almightness_handler(SF_CGdSpell *_this,
                                          uint16_t spell_index);
void __thiscall white_almightness_handler(SF_CGdSpell *_this,
                                          uint16_t spell_index);
void __thiscall elemental_almightness_handler(SF_CGdSpell *_this,
                                              uint16_t spell_index);
void __thiscall mental_almightness_handler(SF_CGdSpell *_this,
                                           uint16_t spell_index);
void __thiscall elemental_almightness2_handler(SF_CGdSpell *_this,
                                               uint16_t spell_index);
void __thiscall elemental_essence2_handler(SF_CGdSpell *_this,
                                           uint16_t spell_index);
void __thiscall assistance_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall holy_touch_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall revenge_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall area_roots_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall roots_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall reinforcement_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall cannibalize_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall torture_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall dominate_undead_handler(SF_CGdSpell *_this,
                                        uint16_t spell_index);
void __thiscall mutation_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall area_darkness_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall area_freeze_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall clay_feet_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall mirage_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall feedback_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall area_hypnotize_handler(SF_CGdSpell *_this,
                                       uint16_t spell_index);
void __thiscall area_confuse_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall manashield_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall shift_mana_handler(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall ability_shift_life_handler(SF_CGdSpell *_this,
                                           uint16_t spell_index);
void __thiscall ability_riposte_handler(SF_CGdSpell *_this,
                                        uint16_t spell_index);
void __thiscall ability_critical_hits_handler(SF_CGdSpell *_this,
                                              uint16_t spell_index);
void __thiscall belial_effect_handler(SF_CGdSpell *_this, uint16_t spell_index);

#endif
