#include "../handlers/sf_spelltype_handlers.h"
#include "../core/sf_hooks.h"
#include "../core/sf_wrappers.h"

#include "sf_spelltype_registry.h"
#include "sf_registry.h"
#include <map>
#include <cstdio>

void __thiscall initializeSpellData(SF_CGdSpell* _this, uint16_t spell_id, SpellDataKey key) {
	spellAPI.setXData(_this, spell_id, key, 0);
}

std::map<uint16_t, handler_ptr> handler_map;

handler_ptr get_spell_handler (const uint16_t key)
{
	auto it = handler_map.find(key);
    if (it == handler_map.end()) {
        // Element doesn't exist, insert the default value
        it = handler_map.emplace(key, &default_handler).first;
    }
    return it->second;
}

// Exposed in sf_registry.h
void registerSpellTypeHandler(uint16_t spell_index, handler_ptr handler) {
    auto check = handler_map.find(spell_index);
    if (check != handler_map.end()){ 
        if(check->second != &default_handler) {
            char message[256]; // Assuming a maximum message length of 255 characters
            sprintf(message, "A non-default handler has been replaced! [%d] (Was this on purpose?)", spell_index);
            log_warning(message);
        }
    }

	handler_map[spell_index] = handler;
}

void initSpellMap()
{
	// Setup Vanilla Spell Handling!
	registerSpellTypeHandler(0x01, &fireburst_handler);
	registerSpellTypeHandler(0x02, &healing_handler);
	registerSpellTypeHandler(0x03, &death_handler);
	registerSpellTypeHandler(0x04, &slowness_handler);
	registerSpellTypeHandler(0x05, &poison_handler);
	registerSpellTypeHandler(0x06, &invulnerability_handler);
	registerSpellTypeHandler(0x07, &cure_poison_handler);
	registerSpellTypeHandler(0x08, &default_handler); //empty spell line slot
	registerSpellTypeHandler(0x09, &freeze_handler);
	registerSpellTypeHandler(0x0a, &fog_handler);
	registerSpellTypeHandler(0x0b, &illuminate_handler);
	registerSpellTypeHandler(0x0c, &fireshield_handler);
	registerSpellTypeHandler(0x0d, &fireball_handler);
	registerSpellTypeHandler(0x0e, &icestrike_handler);
	registerSpellTypeHandler(0x0f, &iceshield_handler);

	registerSpellTypeHandler(0x10, &decay1_handler);
	registerSpellTypeHandler(0x11, &decay2_handler);
	registerSpellTypeHandler(0x12, &pain_handler);
	registerSpellTypeHandler(0x13, &lifetap_handler);
	registerSpellTypeHandler(0x14, &summons_handler);
	registerSpellTypeHandler(0x15, &hypnotize_handler);
	registerSpellTypeHandler(0x16, &iceshield2_handler);
	registerSpellTypeHandler(0x17, &pestilence_handler);
	registerSpellTypeHandler(0x18, &cure_disease_handler);
	registerSpellTypeHandler(0x19, &petrify_handler);
	registerSpellTypeHandler(0x1a, &default_handler); //empty spell line slot
	registerSpellTypeHandler(0x1b, &default_handler); //empty spell line slot
	registerSpellTypeHandler(0x1c, &area_pain_handler);
	registerSpellTypeHandler(0x1d, &summons_handler);
	registerSpellTypeHandler(0x1e, &raise_dead_handler);
	registerSpellTypeHandler(0x1f, &summons_handler);

	registerSpellTypeHandler(0x20, &death_grasp_handler);
	registerSpellTypeHandler(0x21, &summons_handler);
	registerSpellTypeHandler(0x22, &inflexibility_handler);
	registerSpellTypeHandler(0x23, &weaken_handler);
	registerSpellTypeHandler(0x24, &dark_banishing_handler);
	registerSpellTypeHandler(0x25, &area_slowness_handler);
	registerSpellTypeHandler(0x26, &area_inflexibility_handler);
	registerSpellTypeHandler(0x27, &area_weaken_handler);
	registerSpellTypeHandler(0x28, &area_plague_handler);
	registerSpellTypeHandler(0x29, &remediless_handler);
	registerSpellTypeHandler(0x2a, &default_handler); //dark might
	registerSpellTypeHandler(0x2b, &area_healing_handler);
	registerSpellTypeHandler(0x2c, &sentinel_healing_handler);
	registerSpellTypeHandler(0x2d, &healing_handler);
	registerSpellTypeHandler(0x2e, &charm_animal_handler);
	registerSpellTypeHandler(0x2f, &thorn_shield_handler);

	registerSpellTypeHandler(0x30, &quickness_handler);
	registerSpellTypeHandler(0x31, &area_quickness_handler);
	registerSpellTypeHandler(0x32, &flexibility_handler);
	registerSpellTypeHandler(0x33, &area_flexibility_handler);
	registerSpellTypeHandler(0x34, &strength_handler);
	registerSpellTypeHandler(0x35, &area_strength_handler);
	registerSpellTypeHandler(0x36, &guard_handler);
	registerSpellTypeHandler(0x37, &remove_curse_handler);
	registerSpellTypeHandler(0x38, &regenerate_handler);
	registerSpellTypeHandler(0x39, &default_handler); //holy might
	registerSpellTypeHandler(0x3a, &hallow_handler);
	registerSpellTypeHandler(0x3b, &default_handler); //empty slot
	registerSpellTypeHandler(0x3c, &fireshield2_handler); 
	registerSpellTypeHandler(0x3d, &thorn_shield2_handler);
	registerSpellTypeHandler(0x3e, &forget_handler); 
	registerSpellTypeHandler(0x3f, &self_illusion_handler); 

	registerSpellTypeHandler(0x40, &retention_handler);
	registerSpellTypeHandler(0x41, &brilliance_handler);
	registerSpellTypeHandler(0x42, &sacrifice_mana_handler);
	registerSpellTypeHandler(0x43, &manatap_handler);
	registerSpellTypeHandler(0x44, &manadrain_handler);
	registerSpellTypeHandler(0x45, &shock_handler);
	registerSpellTypeHandler(0x46, &disrupt_handler);
	registerSpellTypeHandler(0x47, &default_handler); //empty slot
	registerSpellTypeHandler(0x48, &confuse_handler);
	registerSpellTypeHandler(0x49, &rain_of_fire_handler);
	registerSpellTypeHandler(0x4a, &blizzard_handler);
	registerSpellTypeHandler(0x4b, &acid_cloud_handler);
	registerSpellTypeHandler(0x4c, &stone_rain_handler);
	registerSpellTypeHandler(0x4d, &default_handler); //empty slot
	registerSpellTypeHandler(0x4e, &default_handler); //empty slot
	registerSpellTypeHandler(0x4f, &amok_handler);

	registerSpellTypeHandler(0x50, &default_handler); //empty spell line slot
	registerSpellTypeHandler(0x51, &extinct_handler);
	registerSpellTypeHandler(0x52, &detect_metal_handler);
	registerSpellTypeHandler(0x53, &detect_magic_handler);
	registerSpellTypeHandler(0x54, &default_handler); //unnamed spell 84
	registerSpellTypeHandler(0x55, &unkn_spell_handler); //unnamed spell 85
	registerSpellTypeHandler(0x56, &invisibility_handler);
	registerSpellTypeHandler(0x57, &stone_handler);
	registerSpellTypeHandler(0x58, &aura_handler);
	registerSpellTypeHandler(0x59, &aura_handler);
	registerSpellTypeHandler(0x5a, &suicide_death_handler);
	registerSpellTypeHandler(0x5b, &aura_handler);
	registerSpellTypeHandler(0x5c, &summons_handler);
	registerSpellTypeHandler(0x5d, &feign_death_handler);
	registerSpellTypeHandler(0x5e, &aura_handler);
	registerSpellTypeHandler(0x5f, &aura_handler);
	
	registerSpellTypeHandler(0x60, &dispel_white_aura_handler);
	registerSpellTypeHandler(0x61, &aura_handler);
	registerSpellTypeHandler(0x62, &aura_handler);
	registerSpellTypeHandler(0x63, &suffocation_handler);
	registerSpellTypeHandler(0x64, &inablility_handler);
	registerSpellTypeHandler(0x65, &slow_fighting_handler);
	registerSpellTypeHandler(0x66, &aura_handler);
	registerSpellTypeHandler(0x67, &aura_handler);
	registerSpellTypeHandler(0x68, &aura_handler);
	registerSpellTypeHandler(0x69, &suicide_heal_handler);
	registerSpellTypeHandler(0x6a, &summons_handler);
	registerSpellTypeHandler(0x6b, &aura_handler);
	registerSpellTypeHandler(0x6c, &dominate_animal_handler);
	registerSpellTypeHandler(0x6d, &summons_handler);
	registerSpellTypeHandler(0x6e, &aura_handler);
	registerSpellTypeHandler(0x6f, &aura_handler);


	registerSpellTypeHandler(0x70, &dispel_black_aura_handler);
	registerSpellTypeHandler(0x71, &aura_handler);
	registerSpellTypeHandler(0x72, &aura_handler);
	registerSpellTypeHandler(0x73, &aura_handler);
	registerSpellTypeHandler(0x74, &dexterity_handler);
	registerSpellTypeHandler(0x75, &edurance_handler);
	registerSpellTypeHandler(0x76, &fast_fighting_handler);
	registerSpellTypeHandler(0x77, &distract_handler);
	registerSpellTypeHandler(0x78, &dominate_handler);
	registerSpellTypeHandler(0x79, &default_handler); //empty spell line slot
	registerSpellTypeHandler(0x7a, &charm_handler);
	registerSpellTypeHandler(0x7b, &befriend_handler);
	registerSpellTypeHandler(0x7c, &disenchant_handler);
	registerSpellTypeHandler(0x7d, &charisma_handler);
	registerSpellTypeHandler(0x7e, &shockwave_handler);
	registerSpellTypeHandler(0x7f, &aura_handler);

	registerSpellTypeHandler(0x80, &demoralization_handler);
	registerSpellTypeHandler(0x81, &aura_handler);
	registerSpellTypeHandler(0x82, &enlightenment_handler);
	registerSpellTypeHandler(0x83, &aura_handler);
	registerSpellTypeHandler(0x84, &meditation_handler);
	registerSpellTypeHandler(0x85, &summons_handler);
	registerSpellTypeHandler(0x86, &wave_handler);
	registerSpellTypeHandler(0x87, &melt_resistance_handler);
	registerSpellTypeHandler(0x88, &summons_handler);
	registerSpellTypeHandler(0x89, &wave_handler);
	registerSpellTypeHandler(0x8a, &chill_resistance_handler);
	registerSpellTypeHandler(0x8b, &rock_bullet_handler);
	registerSpellTypeHandler(0x8c, &conservation_handler);
	registerSpellTypeHandler(0x8d, &summons_handler);
	registerSpellTypeHandler(0x8e, &wave_handler);
	registerSpellTypeHandler(0x8f, &tower_arrow_handler);

	registerSpellTypeHandler(0x90, &tower_healing_handler);
	registerSpellTypeHandler(0x91, &tower_icestrike_handler);
	registerSpellTypeHandler(0x92, &lifetap_aura_handler);
	registerSpellTypeHandler(0x93, &fireball2_handler);
	registerSpellTypeHandler(0x94, &ability_warcry_handler);
	registerSpellTypeHandler(0x95, &ability_benefactions_handler);
	registerSpellTypeHandler(0x96, &ability_patronize_handler);
	registerSpellTypeHandler(0x97, &ability_endurance_handler);
	registerSpellTypeHandler(0x98, &ability_berserk_handler);
	registerSpellTypeHandler(0x99, &ability_boons_handler);
	registerSpellTypeHandler(0x9a, &ability_shelter_handler);
	registerSpellTypeHandler(0x9b, &ability_durability_handler);
	registerSpellTypeHandler(0x9c, &ability_trueshot_handler);
	registerSpellTypeHandler(0x9d, &ability_steelskin_handler);
	registerSpellTypeHandler(0x9e, &ability_salvo_handler);
	registerSpellTypeHandler(0x9f, &fireburst_handler);

	registerSpellTypeHandler(0xa0, &spark_handler);
	registerSpellTypeHandler(0xa1, &tower_hypnotize_handler);
	registerSpellTypeHandler(0xa2, &tower_pain_handler);
	registerSpellTypeHandler(0xa3, &tower_stone_handler);
	registerSpellTypeHandler(0xa4, &cloak_of_nor_handler);
	registerSpellTypeHandler(0xa5, &unkn_spell2_handler);
	registerSpellTypeHandler(0xa6, &healing_handler);
	registerSpellTypeHandler(0xa7, &hypnotize_handler);
	registerSpellTypeHandler(0xa8, &freeze2_handler);
	registerSpellTypeHandler(0xa9, &freeze3_handler);
	registerSpellTypeHandler(0xaa, &lava_bullet_handler);
	registerSpellTypeHandler(0xab, &tower_extinct_handler);
	registerSpellTypeHandler(0xac, &manatap_aura_handler);
	registerSpellTypeHandler(0xad, &firebane_handler);
	registerSpellTypeHandler(0xae, &black_essence_handler);
	registerSpellTypeHandler(0xaf, &white_essence_handler);

	registerSpellTypeHandler(0xb0, &elemental_essence_handler);
	registerSpellTypeHandler(0xb1, &mental_essence_handler);
	registerSpellTypeHandler(0xb2, &black_almightness_handler);
	registerSpellTypeHandler(0xb3, &white_almightness_handler);
	registerSpellTypeHandler(0xb4, &elemental_almightness_handler);
	registerSpellTypeHandler(0xb5, &mental_almightness_handler);
	registerSpellTypeHandler(0xb6, &elemental_almightness2_handler);
	registerSpellTypeHandler(0xb7, &elemental_essence2_handler);
	registerSpellTypeHandler(0xb8, &assistance_handler);
	registerSpellTypeHandler(0xb9, &holy_touch_handler);
	registerSpellTypeHandler(0xba, &revenge_handler);
	registerSpellTypeHandler(0xbb, &area_roots_handler);
    registerSpellTypeHandler(0xbc, &summons_handler);
	registerSpellTypeHandler(0xbd, &roots_handler);
	registerSpellTypeHandler(0xbe, &chain_handler);
	registerSpellTypeHandler(0xbf, &reinforcement_handler);

	registerSpellTypeHandler(0xc0, &aura_handler);
	registerSpellTypeHandler(0xc1, &chain_handler);
	registerSpellTypeHandler(0xc2, &cannibalize_handler);
	registerSpellTypeHandler(0xc3, &torture_handler);
	registerSpellTypeHandler(0xc4, &chain_handler);
    registerSpellTypeHandler(0xc5, &dominate_undead_handler);
    registerSpellTypeHandler(0xc6, &summons_handler);
    registerSpellTypeHandler(0xc7, &mutation_handler);
    registerSpellTypeHandler(0xc8, &area_darkness_handler);
    registerSpellTypeHandler(0xc9, &chain_handler);
    registerSpellTypeHandler(0xca, &chain_handler);
    registerSpellTypeHandler(0xcb, &summons_handler);
    registerSpellTypeHandler(0xcc, &chain_handler);
    registerSpellTypeHandler(0xcd, &chain_handler);
    registerSpellTypeHandler(0xce, &summons_handler);
    registerSpellTypeHandler(0xcf, &area_freeze_handler);

    registerSpellTypeHandler(0xd0, &chain_handler);
    registerSpellTypeHandler(0xd1, &summons_handler);
    registerSpellTypeHandler(0xd2, &clay_feet_handler);
    registerSpellTypeHandler(0xd3, &mirage_handler);
    registerSpellTypeHandler(0xd4, &chain_handler);
    registerSpellTypeHandler(0xd5, &feedback_handler);
    registerSpellTypeHandler(0xd6, &chain_handler);
    registerSpellTypeHandler(0xd7, &area_hypnotize_handler);
    registerSpellTypeHandler(0xd8, &area_confuse_handler);
    registerSpellTypeHandler(0xd9, &chain_handler);
    registerSpellTypeHandler(0xda, &manashield_handler);
	registerSpellTypeHandler(0xdb, &shift_mana_handler);
	registerSpellTypeHandler(0xdc, &ability_shift_life_handler);
	registerSpellTypeHandler(0xdd, &ability_riposte_handler);
	registerSpellTypeHandler(0xde, &ability_critical_hits_handler);
	registerSpellTypeHandler(0xdf, &aura_handler);

	registerSpellTypeHandler(0xe0, &default_handler); //empty spell line slot
	registerSpellTypeHandler(0xe1, &aura_handler);
	registerSpellTypeHandler(0xe2, &aura_handler);
	registerSpellTypeHandler(0xe3, &aura_handler);
	registerSpellTypeHandler(0xe4, &aura_handler);
 	registerSpellTypeHandler(0xe5, &eternity_aura_handler);
	registerSpellTypeHandler(0xe6, &hallow_handler);
    registerSpellTypeHandler(0xe7, &lifetap_handler);
	registerSpellTypeHandler(0xe8, &manatap_handler);
	registerSpellTypeHandler(0xe9, &mutation_handler);
    registerSpellTypeHandler(0xea, &fireburst_handler);
    registerSpellTypeHandler(0xeb, &icestrike_handler);
    registerSpellTypeHandler(0xec, &rock_bullet_handler);
    registerSpellTypeHandler(0xed, &charm_handler);
	registerSpellTypeHandler(0xee, &shock_handler);
    registerSpellTypeHandler(0xef, &fireball_handler);

    registerSpellTypeHandler(0xf0, &pain_handler);
    registerSpellTypeHandler(0xf1, &belial_effect_handler);
}