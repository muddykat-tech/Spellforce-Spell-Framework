#include "sf_spelleffect_handlers.h"
#include "sf_utility.h"

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
handler_ptr effect_ability_warcy_handler;
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

void initEffectHandlers()
{
    effect_ability_benefactions_handler = (handler_ptr)(ASI::AddrOf(0x32c090));
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
    effect_ability_warcy_handler = (handler_ptr)(ASI::AddrOf(0x32df90));
    effect_acid_cloud_handler = (handler_ptr)(ASI::AddrOf(0x32e370));
    effect_almightiness_black_handler = (handler_ptr)(ASI::AddrOf(0x32e730));
    effect_almightiness_elemental_handler = (handler_ptr)(ASI::AddrOf(0x32e9d0));
    effect_almightiness_elemental2_handler = (handler_ptr)(ASI::AddrOf(0x32eca0));
    effect_almightiness_mental_handler = (handler_ptr)(ASI::AddrOf(0x32f050));
    effect_almightiness_white_handler = (handler_ptr)(ASI::AddrOf(0x32f330));
    effect_amok_handler = (handler_ptr)(ASI::AddrOf(0x32f590));
    effect_tower_arrow_handler = (handler_ptr)(ASI::AddrOf(0x32f840));
    effect_assistance_handler = (handler_ptr)(ASI::AddrOf(0x32fbc0));
    effect_aura_handler = (handler_ptr)(ASI::AddrOf(0x32fd40));
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
    effect_conservation_handler = (handler_ptr)(ASI::AddrOf(0x333130));
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
    effect_fire_resistance_handler = (handler_ptr)(ASI::AddrOf(0x33a3e0));
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

handler_ptr __thiscall get_spell_effect(uint16_t spell_job) {
    switch(spell_job) {
        case 1:
            return effect_fireburst_handler;
            
        case 2:
            return effect_healing_handler;
            
        case 3:
            return effect_death_handler;
            
        case 4:
            return effect_poison_handler;
            
        case 5:
            return effect_cure_poison_handler;
            
        case 6:
            return effect_invulnerability_handler;
            
        case 7:
            return effect_unknown1_handler;
            
        case 8:
            return effect_icestrike1_handler;
            
        case 9:
            return effect_fog_handler;
            
        case 10:
            return effect_stone_handler;
            
        case 0xb:
            return effect_slowness_handler;
            
        case 0xc:
            return effect_freeze_handler;
            
        case 0xd:
            return effect_decay_handler;
            
        case 0xe:
            return effect_pain_handler;
            
        case 0xf:
            return effect_iceshield_handler;
            
        case 0x10:
            return effect_illuminate_handler;
            
        case 0x11:
            return effect_fireball1_handler;
            
        case 0x12:
            return effect_fireshield1_handler;
            
        case 0x13:
            return effect_iceshield2_handler;
            
        case 0x14:
            return effect_lifetap_handler;
            
        case 0x15:
            return effect_decay2_handler;
            
        case 0x16:
            return effect_iceshield3_handler;
            
        case 0x17:
            return effect_summon_handler;
            
        case 0x18:
            return effect_petrify_handler;
            
        case 0x19:
            return effect_pestilence_handler;
            
        case 0x1a:
            return effect_cure_disease_handler;
            
        case 0x1b:
            return effect_pain_area_handler;
            
        case 0x1c:
            return effect_death_grasp_handler;
            
        case 0x1d:
            return effect_inflexibility_handler;
            
        case 0x1e:
            return effect_detect_magic_handler;
            
        case 0x1f:
            return effect_weaken_handler;
            
        case 0x20:
            return effect_dark_banishing_handler;
            
        case 0x21:
            return effect_slowness_area_handler;
            
        case 0x22:
            return effect_inflexibility_area_handler;
            
        case 0x23:
            return effect_weaken_area_handler;
            
        case 0x24:
            return effect_plague_area_handler;
            
        case 0x25:
            return effect_remediless_handler;
            
        case 0x26:
            return effect_healing_area_handler;
            
        case 0x27:
            return effect_sentinel_healing_handler;
            
        case 0x28:
            return effect_quickness_handler;
            
        case 0x29:
            return effect_quickness_area_handler;
            
        case 0x2a:
            return effect_thornshield_handler;
            
        case 0x2b:
            return effect_flexibility_handler;
            
        case 0x2c:
            return effect_flexibility_area_handler;
            
        case 0x2d:
            return effect_strength_handler;
            
        case 0x2e:
            return effect_strength_area_handler;
            
        case 0x2f:
            return effect_guard_handler;
            
        case 0x30:
            return effect_regenerate_handler;
            
        case 0x31:
            return effect_hallow_handler;
            
        case 0x32:
            return effect_fireshield2_handler;
            
        case 0x33:
            return effect_thorns_handler;
            
        case 0x34:
            return effect_raise_dead_handler;
            
        case 0x37:
            return effect_forget_handler;
            
        case 0x38:
            return effect_retention_handler;
            
        case 0x39:
            return effect_brilliance_handler;
            
        case 0x3a:
            return effect_sacrifice_mana_handler;
            
        case 0x3b:
            return effect_manatap_handler;
            
        case 0x3c:
            return effect_manadrain_handler;
            
        case 0x3d:
            return effect_amok_handler;
            
        case 0x3e:
            return effect_confuse_handler;
            
        case 0x3f:
            return effect_disrupt_handler;
            
        case 0x40:
            return effect_acid_cloud_handler;
            
        case 0x41:
            return effect_rain_of_fire_handler;
            
        case 0x42:
            return effect_shock_handler;
            
        case 0x43:
            return effect_blizzard_handler;
            
        case 0x44:
            return effect_rain_of_stone_handler;
            
        case 0x45:
            return effect_charm_animal_handler;
            
        case 0x46:
            return effect_invisibility_handler;
            
        case 0x47:
            return effect_extinct_handler;
            
        case 0x48:
            return effect_remove_curse_handler;
            
        case 0x49:
            return effect_aura_handler;
            
        case 0x4a:
            return effect_suffocation_handler;
            
        case 0x4b:
            return effect_slow_fighting_handler;
            
        case 0x4c:
            return effect_inability_handler;
            
        case 0x4d:
            return effect_suicide_death_handler;
            
        case 0x4e:
            return effect_endurance_handler;
            
        case 0x4f:
            return effect_fast_fighting_handler;
            
        case 0x50:
            return effect_dexterity_handler;
            
        case 0x51:
            return effect_suicide_heal_handler;
            
        case 0x52:
            return effect_dispel_white_aura_handler;
            
        case 0x53:
            return effect_dispel_black_aura_handler;
            
        case 0x54:
            return effect_dominate_animal_handler;
            
        case 0x55:
            return effect_wave_handler;
            
        case 0x56:
            return effect_rock_bullet_handler;
            
        case 0x57:
            return effect_melt_resistance_handler;
            
        case 0x58:
            return effect_chill_resistance_handler;
            
        case 0x59:
            return effect_charisma_handler;
            
        case 0x5a:
            return effect_enlightement_handler;
            
        case 0x5b:
            return effect_meditation_handler;
            
        case 0x5c:
            return effect_shockwave_handler;
            
        case 0x5d:
            return effect_befriend_handler;
            
        case 0x5e:
            return effect_dominate_handler;
            
        case 0x5f:
            return effect_charm_handler;
            
        case 0x60:
            return effect_tower_arrow_handler;
            
        case 0x61:
            return effect_healing_aura_handler;
            
        case 0x62:
            return effect_icestrike2_handler;
            
        case 99:
            return effect_distract_handler;
            
        case 100:
            return effect_detect_metal_handler;
            
        case 0x65:
            return effect_conservation_handler;
            
        case 0x66:
            return effect_lifetap_aura_handler;
            
        case 0x67:
            return effect_fireball2_handler;
            
        case 0x68:
            return effect_self_illusion_handler;
            
        case 0x69:
            return effect_ability_warcy_handler;
            
        case 0x6a:
            return effect_ability_benefactions_handler;
            
        case 0x6b:
            return effect_ability_patronize_handler;
            
        case 0x6c:
            return effect_ability_endurance_handler;
            
        case 0x6d:
            return effect_ability_berserk_handler;
            
        case 0x6e:
            return effect_ability_boons_handler;
            
        case 0x6f:
            return effect_ability_shelter_handler;
            
        case 0x70:
            return effect_ability_durability_handler;
            
        case 0x71:
            return effect_ability_trueshot_handler;
            
        case 0x72:
            return effect_ability_steelskin_handler;
            
        case 0x73:
            return effect_ability_salvo_handler;
            
        case 0x74:
            return effect_spark_handler;
            
        case 0x75:
            return effect_demoralization_handler;
            
        case 0x76:
            return effect_hypnotize_handler;
            
        case 0x77:
            return effect_tower_pain_handler;
            
        case 0x78:
            return effect_tower_stone_handler;
            
        case 0x79:
            return effect_cloak_of_nor_handler;
            
        case 0x7a:
            return effect_feign_death_handler;
            
        case 0x7b:
            return effect_dominate_break_handler;
            
        case 0x7c:
            return effect_disenchant_handler;
            
        case 0x7d:
            return effect_unknown2_handler;
            
        case 0x7e:
            return effect_ice1_handler;
            
        case 0x7f:
            return effect_ice2_handler;
            
        case 0x80:
            return effect_firebullet_handler;
            
        case 0x81:
            return effect_tower_extinct_handler;
            
        case 0x82:
            return effect_manatap_aura_handler;
            
        case 0x83:
            return effect_fire_resistance_handler;
            
        case 0x84:
            return effect_essence_black_handler;
            
        case 0x85:
            return effect_almightiness_black_handler;
            
        case 0x86:
            return effect_essence_white_handler;
            
        case 0x87:
            return effect_almightiness_white_handler;
            
        case 0x88:
            return effect_essence_elemental_handler;
            
        case 0x89:
            return effect_almightiness_elemental_handler;
            
        case 0x8a:
            return effect_essence_mental_handler;
            
        case 0x8b:
            return effect_almightiness_mental_handler;
            
        case 0x8c:
            return effect_essence_elemental2_handler;
            
        case 0x8d:
            return effect_almightiness_elemental2_handler;
            
        case 0x8e:
            return effect_assistance_handler;
            
        case 0x8f:
            return effect_holy_touch_handler;
            
        case 0x90:
            return effect_roots_area_handler;
            
        case 0x91:
            return effect_eternity_handler;
            
        case 0x92:
            return effect_cannibalize_handler;
            
        case 0x93:
            return effect_dominate_undead_handler;
            
        case 0x94:
            return effect_darkness_area_handler;
            
        case 0x95:
            return effect_freeze_area_handler;
            
        case 0x96:
            return effect_shift_mana_handler;
            
        case 0x97:
            return effect_revenge_handler;
            
        case 0x98:
            return effect_torture_handler;
            
        case 0x99:
            return effect_reinforcement_handler;
            
        case 0x9a:
            return effect_hypnotize_area_handler;
            
        case 0x9b:
            return effect_feet_of_clay_handler;
            
        case 0x9c:
            return effect_confuse_area_handler;
            
        case 0x9d:
            return effect_mutation_handler;
            
        case 0x9e:
            return effect_ability_shift_life_handler;
            
        case 0x9f:
            return effect_ability_riposte_handler;
            
        case 0xa0:
            return effect_ability_critical_hits_handler;
            
        case 0xa1:
            return effect_manashield_handler;
            
        case 0xa2:
            return effect_chain_handler;
            
        case 0xa3:
            return effect_roots_handler;
            
        case 0xa4:
            return effect_mirage_handler;
            
        case 0xa5:
            return effect_fake_spell_one_figure_handler;
            
        case 0xa6:
            return effect_feedback_handler;
    }
    logError("Unknown Job ID, No effect handler registered.");
    return NULL;
}