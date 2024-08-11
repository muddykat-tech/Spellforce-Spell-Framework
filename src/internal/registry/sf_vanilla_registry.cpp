#include "sf_vanilla_registry.h"
#include "../core/sf_hooks.h"
#include "../handlers/sf_spelleffect_handlers.h"

void initialize_vanilla_spells()
{
    initialize_vanilla_effect_handler_hooks();

    SFSpell *fireburst = registrationAPI.registerSpell(0x01);
    registrationAPI.linkTypeHandler(fireburst, &fireburst_handler);

    SFSpell *healing = registrationAPI.registerSpell(0x02);
    registrationAPI.linkTypeHandler(healing, &healing_handler);

    SFSpell *death = registrationAPI.registerSpell(0x03);
    registrationAPI.linkTypeHandler(death, &death_handler);

    SFSpell *slowness = registrationAPI.registerSpell(0x04);
    registrationAPI.linkTypeHandler(slowness, &slowness_handler);

    SFSpell *poison = registrationAPI.registerSpell(0x05);
    registrationAPI.linkTypeHandler(poison, &poison_handler);

    SFSpell *invulnerability = registrationAPI.registerSpell(0x06);
    registrationAPI.linkTypeHandler(invulnerability, &invulnerability_handler);

    SFSpell *cure_poison = registrationAPI.registerSpell(0x07);
    registrationAPI.linkTypeHandler(cure_poison, &cure_poison_handler);

    // Empty Spell Type here

    SFSpell *freeze = registrationAPI.registerSpell(0x09);
    registrationAPI.linkTypeHandler(freeze, &freeze_handler);

    SFSpell *fog = registrationAPI.registerSpell(0x0a);
    registrationAPI.linkTypeHandler(fog, &fog_handler);

    SFSpell *illuminate = registrationAPI.registerSpell(0x0b);
    registrationAPI.linkTypeHandler(illuminate, &illuminate_handler);

    SFSpell *fireshield = registrationAPI.registerSpell(0x0c);
    registrationAPI.linkTypeHandler(fireshield, &fireshield_handler);

    SFSpell *fireball = registrationAPI.registerSpell(0x0d);
    registrationAPI.linkTypeHandler(fireball, &fireball_handler);

    SFSpell *icestrike = registrationAPI.registerSpell(0x0e);
    registrationAPI.linkTypeHandler(icestrike, &icestrike_handler);

    SFSpell *iceshield = registrationAPI.registerSpell(0x0f);
    registrationAPI.linkTypeHandler(iceshield, &iceshield_handler);

    // next block

    SFSpell *decay_1 = registrationAPI.registerSpell(0x10);
    registrationAPI.linkTypeHandler(decay_1, &decay1_handler);

    SFSpell *decay_2 = registrationAPI.registerSpell(0x11);
    registrationAPI.linkTypeHandler(decay_2, &decay2_handler);

    SFSpell *pain = registrationAPI.registerSpell(0x12);
    registrationAPI.linkTypeHandler(pain, &pain_handler);

    SFSpell *lifetap = registrationAPI.registerSpell(0x13);
    registrationAPI.linkTypeHandler(lifetap, &lifetap_handler);

    SFSpell *summon_1 = registrationAPI.registerSpell(0x14); // TODO Find spell name
    registrationAPI.linkTypeHandler(summon_1, &summons_handler);
    registrationAPI.applySpellTag(summon_1, SpellTag::SUMMON_SPELL);

    SFSpell *hypnotize = registrationAPI.registerSpell(0x15);
    registrationAPI.linkTypeHandler(hypnotize, &hypnotize_handler);

    SFSpell *iceshield2 = registrationAPI.registerSpell(0x16);
    registrationAPI.linkTypeHandler(iceshield2, &iceshield2_handler);

    SFSpell *pestilence = registrationAPI.registerSpell(0x17);
    registrationAPI.linkTypeHandler(pestilence, &pestilence_handler);

    SFSpell *cure_disease = registrationAPI.registerSpell(0x18);
    registrationAPI.linkTypeHandler(cure_disease, &cure_disease_handler);

    SFSpell *petrify = registrationAPI.registerSpell(0x19);
    registrationAPI.linkTypeHandler(petrify, &petrify_handler);

    // Two unused spells here

    SFSpell *area_pain = registrationAPI.registerSpell(0x1c);
    registrationAPI.linkTypeHandler(area_pain, &area_pain_handler);

    SFSpell *summon_2 = registrationAPI.registerSpell(0x1d); // TODO Find spell name
    registrationAPI.linkTypeHandler(summon_2, &summons_handler);
    registrationAPI.applySpellTag(summon_2, SpellTag::SUMMON_SPELL);

    SFSpell *raise_dead = registrationAPI.registerSpell(0x1e);
    registrationAPI.linkTypeHandler(raise_dead, &raise_dead_handler);

    SFSpell *summon_3 = registrationAPI.registerSpell(0x1f); // TODO Find spell name
    registrationAPI.linkTypeHandler(summon_3, &summons_handler);
    registrationAPI.applySpellTag(summon_3, SpellTag::SUMMON_SPELL);

    SFSpell *death_grasp = registrationAPI.registerSpell(0x20);
    registrationAPI.linkTypeHandler(death_grasp, &death_grasp_handler);

    SFSpell *summon_4 = registrationAPI.registerSpell(0x21); // TODO Find spell name
    registrationAPI.linkTypeHandler(summon_4, &summons_handler);
    registrationAPI.applySpellTag(summon_4, SpellTag::SUMMON_SPELL);

    SFSpell *inflexibility = registrationAPI.registerSpell(0x22);
    registrationAPI.linkTypeHandler(inflexibility, &inflexibility_handler);

    SFSpell *weaken = registrationAPI.registerSpell(0x23);
    registrationAPI.linkTypeHandler(weaken, &weaken_handler);

    SFSpell *dark_banishing = registrationAPI.registerSpell(0x24);
    registrationAPI.linkTypeHandler(dark_banishing, &dark_banishing_handler);

    SFSpell *area_slowness = registrationAPI.registerSpell(0x25);
    registrationAPI.linkTypeHandler(area_slowness, &area_slowness_handler);

    SFSpell *area_inflexibility = registrationAPI.registerSpell(0x26);
    registrationAPI.linkTypeHandler(area_inflexibility, &area_inflexibility_handler);

    SFSpell *area_weaken = registrationAPI.registerSpell(0x27);
    registrationAPI.linkTypeHandler(area_weaken, &area_weaken_handler);

    SFSpell *area_plauge = registrationAPI.registerSpell(0x28);
    registrationAPI.linkTypeHandler(area_plauge, &area_plague_handler);

    SFSpell *remediless = registrationAPI.registerSpell(0x29);
    registrationAPI.linkTypeHandler(remediless, &remediless_handler);

    // Unused Spell here

    SFSpell *area_healing = registrationAPI.registerSpell(0x2b);
    registrationAPI.linkTypeHandler(area_healing, &area_healing_handler);

    SFSpell *sentinel_healing = registrationAPI.registerSpell(0x2c);
    registrationAPI.linkTypeHandler(sentinel_healing, &sentinel_healing_handler);

    SFSpell *healing2 = registrationAPI.registerSpell(0x2d);
    registrationAPI.linkTypeHandler(healing2, &healing_handler);

    SFSpell *charm_animal = registrationAPI.registerSpell(0x2e);
    registrationAPI.linkTypeHandler(charm_animal, &charm_animal_handler);
    registrationAPI.applySpellTag(charm_animal, SpellTag::DOMINATION_SPELL);

    SFSpell *thorn_shield = registrationAPI.registerSpell(0x2f);
    registrationAPI.linkTypeHandler(thorn_shield, &thorn_shield_handler);

    SFSpell *quickness = registrationAPI.registerSpell(0x30);
    registrationAPI.linkTypeHandler(quickness, &quickness_handler);

    SFSpell *area_quickness = registrationAPI.registerSpell(0x31);
    registrationAPI.linkTypeHandler(area_quickness, &area_quickness_handler);

    SFSpell *flexibility = registrationAPI.registerSpell(0x32);
    registrationAPI.linkTypeHandler(flexibility, &flexibility_handler);

    SFSpell *area_flexibility = registrationAPI.registerSpell(0x33);
    registrationAPI.linkTypeHandler(area_flexibility, &area_flexibility_handler);

    SFSpell *strength = registrationAPI.registerSpell(0x34);
    registrationAPI.linkTypeHandler(strength, &strength_handler);

    SFSpell *area_strength = registrationAPI.registerSpell(0x35);
    registrationAPI.linkTypeHandler(area_strength, &area_strength_handler);

    SFSpell *guard = registrationAPI.registerSpell(0x36);
    registrationAPI.linkTypeHandler(guard, &guard_handler);

    SFSpell *remove_curse = registrationAPI.registerSpell(0x37);
    registrationAPI.linkTypeHandler(remove_curse, &remove_curse_handler);

    SFSpell *regenerate = registrationAPI.registerSpell(0x38);
    registrationAPI.linkTypeHandler(regenerate, &regenerate_handler);

    SFSpell *holy_might = registrationAPI.registerSpell(0x39);
    registrationAPI.linkTypeHandler(holy_might, &default_handler);

    SFSpell *hallow = registrationAPI.registerSpell(0x3a);
    registrationAPI.linkTypeHandler(hallow, &hallow_handler);

    // Unused spell here

    SFSpell *fireshield2 = registrationAPI.registerSpell(0x3c);
    registrationAPI.linkTypeHandler(fireshield2, &fireshield2_handler);

    SFSpell *thorn_shield2 = registrationAPI.registerSpell(0x3d);
    registrationAPI.linkTypeHandler(thorn_shield2, &thorn_shield_handler);

    SFSpell *forget = registrationAPI.registerSpell(0x3e);
    registrationAPI.linkTypeHandler(forget, &forget_handler);

    SFSpell *self_illusion = registrationAPI.registerSpell(0x3f);
    registrationAPI.linkTypeHandler(self_illusion, &self_illusion_handler);

    // Next Block

    SFSpell *retention = registrationAPI.registerSpell(0x40);
    registrationAPI.linkTypeHandler(retention, &retention_handler);

    SFSpell *brilliance = registrationAPI.registerSpell(0x41);
    registrationAPI.linkTypeHandler(brilliance, &brilliance_handler);

    SFSpell *sacrifice_mana = registrationAPI.registerSpell(0x42);
    registrationAPI.linkTypeHandler(sacrifice_mana, &sacrifice_mana_handler);

    SFSpell *manatap = registrationAPI.registerSpell(0x43);
    registrationAPI.linkTypeHandler(manatap, &manatap_handler);

    SFSpell *mana_drain = registrationAPI.registerSpell(0x44);
    registrationAPI.linkTypeHandler(mana_drain, &manadrain_handler);

    SFSpell *shock = registrationAPI.registerSpell(0x45);
    registrationAPI.linkTypeHandler(shock, &shock_handler);

    SFSpell *disrupt = registrationAPI.registerSpell(0x46);
    registrationAPI.linkTypeHandler(disrupt, &disrupt_handler);

    // Unused Spell Here

    SFSpell *confuse = registrationAPI.registerSpell(0x48);
    registrationAPI.linkTypeHandler(confuse, &confuse_handler);

    SFSpell *rain_of_fire = registrationAPI.registerSpell(0x49);
    registrationAPI.linkTypeHandler(rain_of_fire, &rain_of_fire_handler);

    SFSpell *blizzard = registrationAPI.registerSpell(0x4a);
    registrationAPI.linkTypeHandler(blizzard, &blizzard_handler);

    SFSpell *acid_cloud = registrationAPI.registerSpell(0x4b);
    registrationAPI.linkTypeHandler(acid_cloud, &acid_cloud_handler);

    SFSpell *stone_rain = registrationAPI.registerSpell(0x4c);
    registrationAPI.linkTypeHandler(stone_rain, &stone_rain_handler);

    // Unused Spell Here
    // Unused Spell Here

    SFSpell *amok = registrationAPI.registerSpell(0x4f);
    registrationAPI.linkTypeHandler(amok, &amok_handler);

    // Next Block

    // Unused Spell Here

    SFSpell *extinct = registrationAPI.registerSpell(0x51);
    registrationAPI.linkTypeHandler(extinct, &extinct_handler);

    SFSpell *detect_metal = registrationAPI.registerSpell(0x52);
    registrationAPI.linkTypeHandler(detect_metal, &detect_metal_handler);

    SFSpell *detect_magic = registrationAPI.registerSpell(0x53);
    registrationAPI.linkTypeHandler(detect_magic, &detect_magic_handler);

    // Unused Spell Here
    // Unused Spell Here

    SFSpell *invisibility = registrationAPI.registerSpell(0x56);
    registrationAPI.linkTypeHandler(invisibility, &invisibility_handler);

    SFSpell *stone = registrationAPI.registerSpell(0x57);
    registrationAPI.linkTypeHandler(stone, &stone_handler);

    SFSpell *aura1 = registrationAPI.registerSpell(0x58); // TODO Find spell name
    registrationAPI.linkTypeHandler(aura1, &aura_handler);

    SFSpell *aura2 = registrationAPI.registerSpell(0x59); // TODO Find spell name
    registrationAPI.linkTypeHandler(aura2, &aura_handler);

    SFSpell *suicide_death = registrationAPI.registerSpell(0x5a);
    registrationAPI.linkTypeHandler(suicide_death, &suicide_death_handler);

    SFSpell *aura3 = registrationAPI.registerSpell(0x5b); // TODO Find spell name
    registrationAPI.linkTypeHandler(aura3, &aura_handler);

    SFSpell *summon_5 = registrationAPI.registerSpell(0x5c); // TODO Find Spell name
    registrationAPI.linkTypeHandler(summon_5, &summons_handler);

    SFSpell *feign_death = registrationAPI.registerSpell(0x5d);
    registrationAPI.linkTypeHandler(feign_death, &feign_death_handler);

    SFSpell *aura4 = registrationAPI.registerSpell(0x5e); // TODO Find spell name
    registrationAPI.linkTypeHandler(aura4, &aura_handler);

    SFSpell *aura5 = registrationAPI.registerSpell(0x5f); // TODO Find spell name
    registrationAPI.linkTypeHandler(aura5, &aura_handler);

    // Next Block

    SFSpell *dispel_white_aura = registrationAPI.registerSpell(0x60);
    registrationAPI.linkTypeHandler(dispel_white_aura, &dispel_white_aura_handler);

    SFSpell *aura6 = registrationAPI.registerSpell(0x61); // TODO Find spell name
    registrationAPI.linkTypeHandler(aura6, &aura_handler);

    SFSpell *aura7 = registrationAPI.registerSpell(0x62); // TODO Find spell name
    registrationAPI.linkTypeHandler(aura7, &aura_handler);

    SFSpell *suffocation = registrationAPI.registerSpell(0x63);
    registrationAPI.linkTypeHandler(suffocation, &suffocation_handler);

    SFSpell *inablility = registrationAPI.registerSpell(0x64);
    registrationAPI.linkTypeHandler(inablility, &inablility_handler);

    SFSpell *slow_fighting = registrationAPI.registerSpell(0x65);
    registrationAPI.linkTypeHandler(slow_fighting, &slow_fighting_handler);

    SFSpell *aura8 = registrationAPI.registerSpell(0x66); // TODO Find spell name
    registrationAPI.linkTypeHandler(aura8, &aura_handler);

    SFSpell *aura9 = registrationAPI.registerSpell(0x67); // TODO Find spell name
    registrationAPI.linkTypeHandler(aura9, &aura_handler);

    SFSpell *aura10 = registrationAPI.registerSpell(0x68); // TODO Find spell name
    registrationAPI.linkTypeHandler(aura10, &aura_handler);

    SFSpell *suicide_heal = registrationAPI.registerSpell(0x69);
    registrationAPI.linkTypeHandler(suicide_heal, &suicide_heal_handler);

    SFSpell *summon_6 = registrationAPI.registerSpell(0x6a); // TODO Find spell name
    registrationAPI.linkTypeHandler(summon_6, &summons_handler);
    registrationAPI.applySpellTag(summon_6, SpellTag::SUMMON_SPELL);

    SFSpell *aura11 = registrationAPI.registerSpell(0x6b); // TODO Find spell name
    registrationAPI.linkTypeHandler(aura11, &aura_handler);

    SFSpell *dominate_animal = registrationAPI.registerSpell(0x6c);
    registrationAPI.linkTypeHandler(dominate_animal, &dominate_animal_handler);

    SFSpell *summon_7 = registrationAPI.registerSpell(0x6d); // TODO Find spell name
    registrationAPI.linkTypeHandler(summon_7, &summons_handler);
    registrationAPI.applySpellTag(summon_7, SpellTag::SUMMON_SPELL);

    SFSpell *aura12 = registrationAPI.registerSpell(0x6e); // TODO Find spell name
    registrationAPI.linkTypeHandler(aura12, &aura_handler);

    SFSpell *aura13 = registrationAPI.registerSpell(0x6f); // TODO Find spell name
    registrationAPI.linkTypeHandler(aura13, &aura_handler);

    SFSpell *dispel_black_aura = registrationAPI.registerSpell(0x70);
    registrationAPI.linkTypeHandler(dispel_black_aura, &dispel_black_aura_handler);

    SFSpell *aura14 = registrationAPI.registerSpell(0x71); // TODO Find spell name
    registrationAPI.linkTypeHandler(aura14, &aura_handler);

    SFSpell *aura15 = registrationAPI.registerSpell(0x72); // TODO Find spell name
    registrationAPI.linkTypeHandler(aura15, &aura_handler);

    SFSpell *aura16 = registrationAPI.registerSpell(0x73); // TODO Find spell name
    registrationAPI.linkTypeHandler(aura16, &aura_handler);

    SFSpell *dexterity = registrationAPI.registerSpell(0x74);
    registrationAPI.linkTypeHandler(dexterity, &dexterity_handler);

    SFSpell *endurance = registrationAPI.registerSpell(0x75);
    registrationAPI.linkTypeHandler(endurance, &endurance_handler);

    SFSpell *fast_fighting = registrationAPI.registerSpell(0x76);
    registrationAPI.linkTypeHandler(fast_fighting, &fast_fighting_handler);

    SFSpell *distract = registrationAPI.registerSpell(0x77);
    registrationAPI.linkTypeHandler(distract, &distract_handler);

    SFSpell *dominate = registrationAPI.registerSpell(0x78);
    registrationAPI.linkTypeHandler(dominate, &dominate_handler);
    registrationAPI.applySpellTag(dominate, SpellTag::DOMINATION_SPELL);
    // Unused Spell Here

    SFSpell *charm = registrationAPI.registerSpell(0x7a);
    registrationAPI.linkTypeHandler(charm, &charm_handler);
    registrationAPI.applySpellTag(charm, SpellTag::DOMINATION_SPELL);

    SFSpell *befriend = registrationAPI.registerSpell(0x7b);
    registrationAPI.linkTypeHandler(befriend, &befriend_handler);

    SFSpell *disenchant = registrationAPI.registerSpell(0x7c);
    registrationAPI.linkTypeHandler(disenchant, &disenchant_handler);

    SFSpell *charisma = registrationAPI.registerSpell(0x7d);
    registrationAPI.linkTypeHandler(charisma, &charisma_handler);

    SFSpell *shockwave = registrationAPI.registerSpell(0x7e);
    registrationAPI.linkTypeHandler(shockwave, &shockwave_handler);

    SFSpell *aura17 = registrationAPI.registerSpell(0x7f); // TODO Find spell name
    registrationAPI.linkTypeHandler(aura17, &aura_handler);

    // Next Spell Block

    SFSpell *demoralization = registrationAPI.registerSpell(0x80);
    registrationAPI.linkTypeHandler(demoralization, &demoralization_handler);

    SFSpell *aura18 = registrationAPI.registerSpell(0x81); // TODO Find spell name
    registrationAPI.linkTypeHandler(aura18, &aura_handler);

    SFSpell *enlightenment = registrationAPI.registerSpell(0x82);
    registrationAPI.linkTypeHandler(enlightenment, &enlightenment_handler);

    SFSpell *aura19 = registrationAPI.registerSpell(0x83); // TODO Find spell name
    registrationAPI.linkTypeHandler(aura19, &aura_handler);

    SFSpell *meditation = registrationAPI.registerSpell(0x84);
    registrationAPI.linkTypeHandler(meditation, &meditation_handler);

    SFSpell *summon_8 = registrationAPI.registerSpell(0x85); // TODO Find spell name
    registrationAPI.linkTypeHandler(summon_8, &summons_handler);
    registrationAPI.applySpellTag(summon_8, SpellTag::SUMMON_SPELL);

    SFSpell *wave = registrationAPI.registerSpell(0x86);
    registrationAPI.linkTypeHandler(wave, &wave_handler);

    SFSpell *melt_resistance = registrationAPI.registerSpell(0x87);
    registrationAPI.linkTypeHandler(melt_resistance, &melt_resistance_handler);

    SFSpell *summon_9 = registrationAPI.registerSpell(0x88); // TODO Find spell name
    registrationAPI.linkTypeHandler(summon_9, &summons_handler);
    registrationAPI.applySpellTag(summon_9, SpellTag::SUMMON_SPELL);

    SFSpell *wave2 = registrationAPI.registerSpell(0x89);
    registrationAPI.linkTypeHandler(wave2, &wave_handler);

    SFSpell *chill_resistance = registrationAPI.registerSpell(0x8a);
    registrationAPI.linkTypeHandler(chill_resistance, &chill_resistance_handler);

    SFSpell *rock_bullet = registrationAPI.registerSpell(0x8b);
    registrationAPI.linkTypeHandler(rock_bullet, &rock_bullet_handler);

    SFSpell *conservation = registrationAPI.registerSpell(0x8c);
    registrationAPI.linkTypeHandler(conservation, &conservation_handler);

    SFSpell *summon_10 = registrationAPI.registerSpell(0x8d); // TODO Find spell name
    registrationAPI.linkTypeHandler(summon_10, &summons_handler);
    registrationAPI.applySpellTag(summon_10, SpellTag::SUMMON_SPELL);

    SFSpell *wave3 = registrationAPI.registerSpell(0x8e);
    registrationAPI.linkTypeHandler(wave3, &wave_handler);

    SFSpell *tower_arrow = registrationAPI.registerSpell(0x8f);
    registrationAPI.linkTypeHandler(tower_arrow, &tower_arrow_handler);

    // Next Spell Block

    SFSpell *tower_healing = registrationAPI.registerSpell(0x90);
    registrationAPI.linkTypeHandler(tower_healing, &tower_healing_handler);

    SFSpell *tower_icestrike = registrationAPI.registerSpell(0x91);
    registrationAPI.linkTypeHandler(tower_icestrike, &tower_icestrike_handler);

    SFSpell *lifetap_aura = registrationAPI.registerSpell(0x92);
    registrationAPI.linkTypeHandler(lifetap_aura, &lifetap_aura_handler);

    SFSpell *fireball2 = registrationAPI.registerSpell(0x93);
    registrationAPI.linkTypeHandler(fireball2, &fireball2_handler);

    SFSpell *ability_warcry = registrationAPI.registerSpell(0x94);
    registrationAPI.linkTypeHandler(ability_warcry, &ability_warcry_handler);

    SFSpell *ability_benefactions = registrationAPI.registerSpell(0x95);
    registrationAPI.linkTypeHandler(ability_benefactions, &ability_benefactions_handler);

    SFSpell *ability_patronize = registrationAPI.registerSpell(0x96);
    registrationAPI.linkTypeHandler(ability_patronize, &ability_patronize_handler);

    SFSpell *ability_endurance = registrationAPI.registerSpell(0x97);
    registrationAPI.linkTypeHandler(ability_endurance, &ability_endurance_handler);

    SFSpell *ability_berserk = registrationAPI.registerSpell(0x98);
    registrationAPI.linkTypeHandler(ability_berserk, &ability_berserk_handler);

    SFSpell *ability_boons = registrationAPI.registerSpell(0x99);
    registrationAPI.linkTypeHandler(ability_boons, &ability_boons_handler);

    SFSpell *ability_shelter = registrationAPI.registerSpell(0x9a);
    registrationAPI.linkTypeHandler(ability_shelter, &ability_shelter_handler);

    SFSpell *ability_durability = registrationAPI.registerSpell(0x9b);
    registrationAPI.linkTypeHandler(ability_durability, &ability_durability_handler);

    SFSpell *ability_trueshot = registrationAPI.registerSpell(0x9c);
    registrationAPI.linkTypeHandler(ability_trueshot, &ability_trueshot_handler);

    SFSpell *ability_steelskin = registrationAPI.registerSpell(0x9d);
    registrationAPI.linkTypeHandler(ability_steelskin, &ability_steelskin_handler);

    SFSpell *ability_salvo = registrationAPI.registerSpell(0x9e);
    registrationAPI.linkTypeHandler(ability_salvo, &ability_salvo_handler);

    SFSpell *fireburst2 = registrationAPI.registerSpell(0x9f);
    registrationAPI.linkTypeHandler(fireburst2, &fireburst_handler);

    // Next Spell Block

    SFSpell *spark = registrationAPI.registerSpell(0xa0);
    registrationAPI.linkTypeHandler(spark, &spark_handler);

    SFSpell *tower_hypnotize = registrationAPI.registerSpell(0xa1);
    registrationAPI.linkTypeHandler(tower_hypnotize, &tower_hypnotize_handler);

    SFSpell *tower_pain = registrationAPI.registerSpell(0xa2);
    registrationAPI.linkTypeHandler(tower_pain, &tower_pain_handler);

    SFSpell *tower_stone = registrationAPI.registerSpell(0xa3);
    registrationAPI.linkTypeHandler(tower_stone, &tower_stone_handler);

    SFSpell *cloak_of_nor = registrationAPI.registerSpell(0xa4);
    registrationAPI.linkTypeHandler(cloak_of_nor, &cloak_of_nor_handler);

    SFSpell *unkn_spell2 = registrationAPI.registerSpell(0xa5);
    registrationAPI.linkTypeHandler(unkn_spell2, &unkn_spell2_handler);

    SFSpell *healing3 = registrationAPI.registerSpell(0xa6);
    registrationAPI.linkTypeHandler(healing3, &healing_handler);

    SFSpell *hypnotize2 = registrationAPI.registerSpell(0xa7);
    registrationAPI.linkTypeHandler(hypnotize2, &hypnotize_handler);
    registrationAPI.applySpellTag(hypnotize2, SpellTag::DOMINATION_SPELL);

    SFSpell *freeze2 = registrationAPI.registerSpell(0xa8);
    registrationAPI.linkTypeHandler(freeze2, &freeze2_handler);

    SFSpell *freeze3 = registrationAPI.registerSpell(0xa9);
    registrationAPI.linkTypeHandler(freeze3, &freeze3_handler);

    SFSpell *lava_bullet = registrationAPI.registerSpell(0xaa);
    registrationAPI.linkTypeHandler(lava_bullet, &lava_bullet_handler);

    SFSpell *tower_extinct = registrationAPI.registerSpell(0xab);
    registrationAPI.linkTypeHandler(tower_extinct, &tower_extinct_handler);

    SFSpell *manatap_aura = registrationAPI.registerSpell(0xac);
    registrationAPI.linkTypeHandler(manatap_aura, &manatap_aura_handler);

    SFSpell *firebane = registrationAPI.registerSpell(0xad);
    registrationAPI.linkTypeHandler(firebane, &firebane_handler);

    SFSpell *black_essence = registrationAPI.registerSpell(0xae);
    registrationAPI.linkTypeHandler(black_essence, &black_essence_handler);

    SFSpell *white_essence = registrationAPI.registerSpell(0xaf);
    registrationAPI.linkTypeHandler(white_essence, &white_essence_handler);

    // Next Spell Block

    SFSpell *elemental_essence = registrationAPI.registerSpell(0xb0);
    registrationAPI.linkTypeHandler(elemental_essence, &elemental_essence_handler);

    SFSpell *mental_essence = registrationAPI.registerSpell(0xb1);
    registrationAPI.linkTypeHandler(mental_essence, &mental_essence_handler);

    SFSpell *black_almightness = registrationAPI.registerSpell(0xb2);
    registrationAPI.linkTypeHandler(black_almightness, &black_almightness_handler);

    SFSpell *white_almightness = registrationAPI.registerSpell(0xb3);
    registrationAPI.linkTypeHandler(white_almightness, &white_almightness_handler);

    SFSpell *elemental_almightness = registrationAPI.registerSpell(0xb4);
    registrationAPI.linkTypeHandler(elemental_almightness, &elemental_almightness_handler);

    SFSpell *mental_almightness = registrationAPI.registerSpell(0xb5);
    registrationAPI.linkTypeHandler(mental_almightness, &mental_almightness_handler);

    SFSpell *elemental_almightness2 = registrationAPI.registerSpell(0xb6);
    registrationAPI.linkTypeHandler(elemental_almightness2, &elemental_almightness2_handler);

    SFSpell *elemental_essence2 = registrationAPI.registerSpell(0xb7);
    registrationAPI.linkTypeHandler(elemental_essence2, &elemental_essence2_handler);

    SFSpell *assistance = registrationAPI.registerSpell(0xb8);
    registrationAPI.linkTypeHandler(assistance, &assistance_handler);

    SFSpell *holy_touch = registrationAPI.registerSpell(0xb9);
    registrationAPI.linkTypeHandler(holy_touch, &holy_touch_handler);

    SFSpell *revenge = registrationAPI.registerSpell(0xba);
    registrationAPI.linkTypeHandler(revenge, &revenge_handler);

    SFSpell *area_roots = registrationAPI.registerSpell(0xbb);
    registrationAPI.linkTypeHandler(area_roots, &area_roots_handler);

    SFSpell *summon_11 = registrationAPI.registerSpell(0xbc); // TODO Find Spell Name
    registrationAPI.linkTypeHandler(summon_11, &summons_handler);

    SFSpell *roots = registrationAPI.registerSpell(0xbd);
    registrationAPI.linkTypeHandler(roots, &roots_handler);

    SFSpell *chain = registrationAPI.registerSpell(0xbe); // TODO Find spell name
    registrationAPI.linkTypeHandler(chain, &chain_handler);

    SFSpell *reinforcement = registrationAPI.registerSpell(0xbf);
    registrationAPI.linkTypeHandler(reinforcement, &reinforcement_handler);

    // Next Spell Block

    SFSpell *aura20 = registrationAPI.registerSpell(0xc0);
    registrationAPI.linkTypeHandler(aura20, &aura_handler);

    SFSpell *chain2 = registrationAPI.registerSpell(0xc1); // TODO Find Spell Name
    registrationAPI.linkTypeHandler(chain2, &chain_handler);

    SFSpell *cannibalize = registrationAPI.registerSpell(0xc2);
    registrationAPI.linkTypeHandler(cannibalize, &cannibalize_handler);

    SFSpell *torture = registrationAPI.registerSpell(0xc3);
    registrationAPI.linkTypeHandler(torture, &torture_handler);

    SFSpell *chain3 = registrationAPI.registerSpell(0xc4); // TODO Find Spell Name
    registrationAPI.linkTypeHandler(chain3, &chain_handler);

    SFSpell *dominate_undead = registrationAPI.registerSpell(0xc5);
    registrationAPI.linkTypeHandler(dominate_undead, &dominate_undead_handler);

    SFSpell *summon_12 = registrationAPI.registerSpell(0xc6); // TODO Find Spell Name
    registrationAPI.linkTypeHandler(summon_12, &summons_handler);

    SFSpell *mutation = registrationAPI.registerSpell(0xc7);
    registrationAPI.linkTypeHandler(mutation, &mutation_handler);

    SFSpell *area_darkness = registrationAPI.registerSpell(0xc8);
    registrationAPI.linkTypeHandler(area_darkness, &area_darkness_handler);

    SFSpell *chain4 = registrationAPI.registerSpell(0xc9); // TODO Find Spell Name
    registrationAPI.linkTypeHandler(chain4, &chain_handler);

    SFSpell *chain5 = registrationAPI.registerSpell(0xca); // TODO Find Spell Name
    registrationAPI.linkTypeHandler(chain5, &chain_handler);

    SFSpell *summon_13 = registrationAPI.registerSpell(0xcb); // TODO Find Spell Name
    registrationAPI.linkTypeHandler(summon_13, &summons_handler);

    SFSpell *chain6 = registrationAPI.registerSpell(0xcc); // TODO Find Spell Name
    registrationAPI.linkTypeHandler(chain6, &chain_handler);

    SFSpell *chain7 = registrationAPI.registerSpell(0xcd); // TODO Find Spell Name
    registrationAPI.linkTypeHandler(chain7, &chain_handler);

    SFSpell *summon_14 = registrationAPI.registerSpell(0xce); // TODO Find Spell Name
    registrationAPI.linkTypeHandler(summon_14, &summons_handler);

    SFSpell *area_freeze = registrationAPI.registerSpell(0xcf);
    registrationAPI.linkTypeHandler(area_freeze, &area_freeze_handler);

    // Next Spell Block

    SFSpell *chain8 = registrationAPI.registerSpell(0xd0); // TODO Find Spell Name
    registrationAPI.linkTypeHandler(chain8, &chain_handler);

    SFSpell *summon_15 = registrationAPI.registerSpell(0xd1); // TODO Find Spell Name
    registrationAPI.linkTypeHandler(summon_15, &summons_handler);

    SFSpell *clay_feet = registrationAPI.registerSpell(0xd2);
    registrationAPI.linkTypeHandler(clay_feet, &clay_feet_handler);

    SFSpell *mirage = registrationAPI.registerSpell(0xd3);
    registrationAPI.linkTypeHandler(mirage, &mirage_handler);

    SFSpell *chain9 = registrationAPI.registerSpell(0xd4); // TODO Find Spell Name
    registrationAPI.linkTypeHandler(chain9, &chain_handler);

    SFSpell *feedback = registrationAPI.registerSpell(0xd5);
    registrationAPI.linkTypeHandler(feedback, &feedback_handler);

    SFSpell *chain10 = registrationAPI.registerSpell(0xd6); // TODO Find Spell Name
    registrationAPI.linkTypeHandler(chain10, &chain_handler);

    SFSpell *area_hypnotize = registrationAPI.registerSpell(0xd7);
    registrationAPI.linkTypeHandler(area_hypnotize, &area_hypnotize_handler);

    SFSpell *area_confuse = registrationAPI.registerSpell(0xd8);
    registrationAPI.linkTypeHandler(area_confuse, &area_confuse_handler);

    SFSpell *chain11 = registrationAPI.registerSpell(0xd9); // TODO Find Spell Name
    registrationAPI.linkTypeHandler(chain11, &chain_handler);

    SFSpell *manashield = registrationAPI.registerSpell(0xda);
    registrationAPI.linkTypeHandler(manashield, &manashield_handler);

    SFSpell *shift_mana = registrationAPI.registerSpell(0xdb);
    registrationAPI.linkTypeHandler(shift_mana, &shift_mana_handler);

    SFSpell *ability_shift_life = registrationAPI.registerSpell(0xdc);
    registrationAPI.linkTypeHandler(ability_shift_life, &ability_shift_life_handler);

    SFSpell *ability_riposte = registrationAPI.registerSpell(0xdd);
    registrationAPI.linkTypeHandler(ability_riposte, &ability_riposte_handler);

    SFSpell *ability_critical_hits = registrationAPI.registerSpell(0xde);
    registrationAPI.linkTypeHandler(ability_critical_hits, &ability_critical_hits_handler);

    SFSpell *aura21 = registrationAPI.registerSpell(0xdf); // TODO Find Spell Name
    registrationAPI.linkTypeHandler(aura21, &aura_handler);

    // Next Spell Block

    // Unused Spell Here

    SFSpell *aura22 = registrationAPI.registerSpell(0xe1); // TODO Find Spell Name
    registrationAPI.linkTypeHandler(aura22, &aura_handler);

    SFSpell *aura23 = registrationAPI.registerSpell(0xe2); // TODO Find Spell Name
    registrationAPI.linkTypeHandler(aura23, &aura_handler);

    SFSpell *aura24 = registrationAPI.registerSpell(0xe3); // TODO Find Spell Name
    registrationAPI.linkTypeHandler(aura24, &aura_handler);

    SFSpell *aura25 = registrationAPI.registerSpell(0xe4); // TODO Find Spell Name
    registrationAPI.linkTypeHandler(aura25, &aura_handler);

    SFSpell *eternity_aura = registrationAPI.registerSpell(0xe5);
    registrationAPI.linkTypeHandler(eternity_aura, &eternity_aura_handler);

    SFSpell *hallow2 = registrationAPI.registerSpell(0xe6);
    registrationAPI.linkTypeHandler(hallow2, &hallow_handler);

    SFSpell *lifetap2 = registrationAPI.registerSpell(0xe7);
    registrationAPI.linkTypeHandler(lifetap2, &lifetap_handler);

    SFSpell *manatap2 = registrationAPI.registerSpell(0xe8);
    registrationAPI.linkTypeHandler(manatap2, &manatap_handler);

    SFSpell *mutation2 = registrationAPI.registerSpell(0xe9);
    registrationAPI.linkTypeHandler(mutation2, &mutation_handler);

    SFSpell *fireburst3 = registrationAPI.registerSpell(0xea);
    registrationAPI.linkTypeHandler(fireburst3, &fireburst_handler);

    SFSpell *icestrike2 = registrationAPI.registerSpell(0xeb);
    registrationAPI.linkTypeHandler(icestrike2, &icestrike_handler);

    SFSpell *rock_bullet2 = registrationAPI.registerSpell(0xec);
    registrationAPI.linkTypeHandler(rock_bullet2, &rock_bullet_handler);

    SFSpell *charm2 = registrationAPI.registerSpell(0xed);
    registrationAPI.linkTypeHandler(charm2, &charm_handler);

    SFSpell *shock2 = registrationAPI.registerSpell(0xee);
    registrationAPI.linkTypeHandler(shock2, &shock_handler);

    SFSpell *fireball3 = registrationAPI.registerSpell(0xef);
    registrationAPI.linkTypeHandler(fireball3, &fireball_handler);

    // Next Spell Block

    SFSpell *pain2 = registrationAPI.registerSpell(0xf0);
    registrationAPI.linkTypeHandler(pain2, &pain_handler);

    SFSpell *belial_effect = registrationAPI.registerSpell(0xf1);
    registrationAPI.linkTypeHandler(belial_effect, &belial_effect_handler);
}