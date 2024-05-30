#include "sf_spelleffect_registry.h"
#include "../core/sf_wrappers.h"
#include "../handlers/sf_spelleffect_handlers.h"

#include <iostream>
#include <map>
#include <cstdint>

std::map<uint16_t, handler_ptr> effect_handler_map;

void register_effect_handler(uint16_t spell_job, handler_ptr handler) 
{
    auto check = effect_handler_map.find(spell_job);
    if (check != effect_handler_map.end()){ 
        char message[256];
        sprintf(message, "An Effect Handler has been replaced! [%d] (Was this on purpose?)", spell_job);
        log_warning(message);
    }

    effect_handler_map[spell_job] = handler;
}

handler_ptr get_spell_effect(uint16_t spell_job) 
{
    auto it = effect_handler_map.find(spell_job);
    if (it == effect_handler_map.end()) {
        logError("Unknown Job ID, No effect handler registered.");
        return NULL;
    }
    return it->second;
}

void register_vanilla_effect_handlers()
{
    // initialize the functions see sf_spelleffect_handlers
    initialize_vanilla_effect_handlers();

    register_effect_handler(1, effect_fireburst_handler);
    register_effect_handler(2, effect_healing_handler);
    register_effect_handler(3, effect_death_handler);
    register_effect_handler(4, effect_poison_handler);
    register_effect_handler(5, effect_cure_poison_handler);
    register_effect_handler(6, effect_invulnerability_handler);
    register_effect_handler(7, effect_unknown1_handler);
    register_effect_handler(8, effect_icestrike1_handler);
    register_effect_handler(9, effect_fog_handler);
    register_effect_handler(10, effect_stone_handler);
    register_effect_handler(0xb, effect_slowness_handler);
    register_effect_handler(0xc, effect_freeze_handler);
    register_effect_handler(0xd, effect_decay_handler);
    register_effect_handler(0xe, effect_pain_handler);
    register_effect_handler(0xf, effect_iceshield_handler);
    register_effect_handler(0x10, effect_illuminate_handler);
    register_effect_handler(0x11, effect_fireball1_handler);
    register_effect_handler(0x12, effect_fireshield1_handler);
    register_effect_handler(0x13, effect_iceshield2_handler);
    register_effect_handler(0x14, effect_lifetap_handler);
    register_effect_handler(0x15, effect_decay2_handler);
    register_effect_handler(0x16, effect_iceshield3_handler);
    register_effect_handler(0x17, effect_summon_handler);
    register_effect_handler(0x18, effect_petrify_handler);
    register_effect_handler(0x19, effect_pestilence_handler);
    register_effect_handler(0x1a, effect_cure_disease_handler);
    register_effect_handler(0x1b, effect_pain_area_handler);
    register_effect_handler(0x1c, effect_death_grasp_handler);
    register_effect_handler(0x1d, effect_inflexibility_handler);
    register_effect_handler(0x1e, effect_detect_magic_handler);
    register_effect_handler(0x1f, effect_weaken_handler);
    register_effect_handler(0x20, effect_dark_banishing_handler);
    register_effect_handler(0x21, effect_slowness_area_handler);
    register_effect_handler(0x22, effect_inflexibility_area_handler);
    register_effect_handler(0x23, effect_weaken_area_handler);
    register_effect_handler(0x24, effect_plague_area_handler);
    register_effect_handler(0x25, effect_remediless_handler);
    register_effect_handler(0x26, effect_healing_area_handler);
    register_effect_handler(0x27, effect_sentinel_healing_handler);
    register_effect_handler(0x28, effect_quickness_handler);
    register_effect_handler(0x29, effect_quickness_area_handler);
    register_effect_handler(0x2a, effect_thornshield_handler);
    register_effect_handler(0x2b, effect_flexibility_handler);
    register_effect_handler(0x2c, effect_flexibility_area_handler);
    register_effect_handler(0x2d, effect_strength_handler);
    register_effect_handler(0x2e, effect_strength_area_handler);
    register_effect_handler(0x2f, effect_guard_handler);
    register_effect_handler(0x30, effect_regenerate_handler);
    register_effect_handler(0x31, effect_hallow_handler);
    register_effect_handler(0x32, effect_fireshield2_handler);
    register_effect_handler(0x33, effect_thorns_handler);
    register_effect_handler(0x34, effect_raise_dead_handler);
    register_effect_handler(0x37, effect_forget_handler);
    register_effect_handler(0x38, effect_retention_handler);
    register_effect_handler(0x39, effect_brilliance_handler);
    register_effect_handler(0x3a, effect_sacrifice_mana_handler);
    register_effect_handler(0x3b, effect_manatap_handler);
    register_effect_handler(0x3c, effect_manadrain_handler);
    register_effect_handler(0x3d, effect_amok_handler);
    register_effect_handler(0x3e, effect_confuse_handler);
    register_effect_handler(0x3f, effect_disrupt_handler);
    register_effect_handler(0x40, effect_acid_cloud_handler);
    register_effect_handler(0x41, effect_rain_of_fire_handler);
    register_effect_handler(0x42, effect_shock_handler);
    register_effect_handler(0x43, effect_blizzard_handler);
    register_effect_handler(0x44, effect_rain_of_stone_handler);
    register_effect_handler(0x45, effect_charm_animal_handler);
    register_effect_handler(0x46, effect_invisibility_handler);
    register_effect_handler(0x47, effect_extinct_handler);
    register_effect_handler(0x48, effect_remove_curse_handler);
    register_effect_handler(0x49, effect_aura_handler);
    register_effect_handler(0x4a, effect_suffocation_handler);
    register_effect_handler(0x4b, effect_slow_fighting_handler);
    register_effect_handler(0x4c, effect_inability_handler);
    register_effect_handler(0x4d, effect_suicide_death_handler);
    register_effect_handler(0x4e, effect_endurance_handler);
    register_effect_handler(0x4f, effect_fast_fighting_handler);
    register_effect_handler(0x50, effect_dexterity_handler);
    register_effect_handler(0x51, effect_suicide_heal_handler);
    register_effect_handler(0x52, effect_dispel_white_aura_handler);
    register_effect_handler(0x53, effect_dispel_black_aura_handler);
    register_effect_handler(0x54, effect_dominate_animal_handler);
    register_effect_handler(0x55, effect_wave_handler);
    register_effect_handler(0x56, effect_rock_bullet_handler);
    register_effect_handler(0x57, effect_melt_resistance_handler);
    register_effect_handler(0x58, effect_chill_resistance_handler);
    register_effect_handler(0x59, effect_charisma_handler);
    register_effect_handler(0x5a, effect_enlightement_handler);
    register_effect_handler(0x5b, effect_meditation_handler);
    register_effect_handler(0x5c, effect_shockwave_handler);
    register_effect_handler(0x5d, effect_befriend_handler);
    register_effect_handler(0x5e, effect_dominate_handler);
    register_effect_handler(0x5f, effect_charm_handler);
    register_effect_handler(0x60, effect_tower_arrow_handler);
    register_effect_handler(0x61, effect_healing_aura_handler);
    register_effect_handler(0x62, effect_icestrike2_handler);
    register_effect_handler(99, effect_distract_handler);
    register_effect_handler(100, effect_detect_metal_handler);
    register_effect_handler(0x65, effect_conservation_handler);
    register_effect_handler(0x66, effect_lifetap_aura_handler);
    register_effect_handler(0x67, effect_fireball2_handler);
    register_effect_handler(0x68, effect_self_illusion_handler);
    register_effect_handler(0x69, effect_ability_warcy_handler);
    register_effect_handler(0x6a, effect_ability_benefactions_handler);
    register_effect_handler(0x6b, effect_ability_patronize_handler);
    register_effect_handler(0x6c, effect_ability_endurance_handler);
    register_effect_handler(0x6d, effect_ability_berserk_handler);
    register_effect_handler(0x6e, effect_ability_boons_handler);
    register_effect_handler(0x6f, effect_ability_shelter_handler);
    register_effect_handler(0x70, effect_ability_durability_handler);
    register_effect_handler(0x71, effect_ability_trueshot_handler);
    register_effect_handler(0x72, effect_ability_steelskin_handler);
    register_effect_handler(0x73, effect_ability_salvo_handler);
    register_effect_handler(0x74, effect_spark_handler);
    register_effect_handler(0x75, effect_demoralization_handler);
    register_effect_handler(0x76, effect_hypnotize_handler);
    register_effect_handler(0x77, effect_tower_pain_handler);
    register_effect_handler(0x78, effect_tower_stone_handler);
    register_effect_handler(0x79, effect_cloak_of_nor_handler);
    register_effect_handler(0x7a, effect_feign_death_handler);
    register_effect_handler(0x7b, effect_dominate_break_handler);
    register_effect_handler(0x7c, effect_disenchant_handler);
    register_effect_handler(0x7d, effect_unknown2_handler);
    register_effect_handler(0x7e, effect_ice1_handler);
    register_effect_handler(0x7f, effect_ice2_handler);
    register_effect_handler(0x80, effect_firebullet_handler);
    register_effect_handler(0x81, effect_tower_extinct_handler);
    register_effect_handler(0x82, effect_manatap_aura_handler);
    register_effect_handler(0x83, effect_fire_resistance_handler);
    register_effect_handler(0x84, effect_essence_black_handler);
    register_effect_handler(0x85, effect_almightiness_black_handler);
    register_effect_handler(0x86, effect_essence_white_handler);
    register_effect_handler(0x87, effect_almightiness_white_handler);
    register_effect_handler(0x88, effect_essence_elemental_handler);
    register_effect_handler(0x89, effect_almightiness_elemental_handler);
    register_effect_handler(0x8a, effect_essence_mental_handler);
    register_effect_handler(0x8b, effect_almightiness_mental_handler);
    register_effect_handler(0x8c, effect_essence_elemental2_handler);
    register_effect_handler(0x8d, effect_almightiness_elemental2_handler);
    register_effect_handler(0x8e, effect_assistance_handler);
    register_effect_handler(0x8f, effect_holy_touch_handler);
    register_effect_handler(0x90, effect_roots_area_handler);
    register_effect_handler(0x91, effect_eternity_handler);
    register_effect_handler(0x92, effect_cannibalize_handler);
    register_effect_handler(0x93, effect_dominate_undead_handler);
    register_effect_handler(0x94, effect_darkness_area_handler);
    register_effect_handler(0x95, effect_freeze_area_handler);
    register_effect_handler(0x96, effect_shift_mana_handler);
    register_effect_handler(0x97, effect_revenge_handler);
    register_effect_handler(0x98, effect_torture_handler);
    register_effect_handler(0x99, effect_reinforcement_handler);
    register_effect_handler(0x9a, effect_hypnotize_area_handler);
    register_effect_handler(0x9b, effect_feet_of_clay_handler);
    register_effect_handler(0x9c, effect_confuse_area_handler);
    register_effect_handler(0x9d, effect_mutation_handler);
    register_effect_handler(0x9e, effect_ability_shift_life_handler);
    register_effect_handler(0x9f, effect_ability_riposte_handler);
    register_effect_handler(0xa0, effect_ability_critical_hits_handler);
    register_effect_handler(0xa1, effect_manashield_handler);
    register_effect_handler(0xa2, effect_chain_handler);
    register_effect_handler(0xa3, effect_roots_handler);
    register_effect_handler(0xa4, effect_mirage_handler);
    register_effect_handler(0xa5, effect_fake_spell_one_figure_handler);
    register_effect_handler(0xa6, effect_feedback_handler);
}