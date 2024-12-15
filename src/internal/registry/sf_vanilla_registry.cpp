#include "sf_vanilla_registry.h"
#include "../core/sf_hooks.h"
#include "../handlers/sf_spelleffect_handlers.h"
#include "../handlers/sf_onhit_handlers.h"
#include "../handlers/sf_ai_spell_handlers.h"

void initialize_vanilla_spells()
{
    initialize_vanilla_effect_handler_hooks();

    SFSpell *fireburst = registrationAPI.registerSpell(kGdSpellLineFireBurst);
    registrationAPI.linkTypeHandler(fireburst, &fireburst_handler);
    registrationAPI.linkSingleTargetAIHandler(fireburst, &fireburst_ai_handler);
    registrationAPI.applySpellTag(fireburst, SpellTag::STACKABLE_SPELL);

    SFSpell *healing = registrationAPI.registerSpell(kGdSpellLineHealing);
    registrationAPI.linkTypeHandler(healing, &healing_handler);
    registrationAPI.applySpellTag(healing, STACKABLE_SPELL);
    registrationAPI.linkSingleTargetAIHandler(healing, &healing_ai_handler);

    SFSpell *death = registrationAPI.registerSpell(kGdSpellLineDeath);
    registrationAPI.linkTypeHandler(death, &death_handler);
    registrationAPI.linkSingleTargetAIHandler(death, &death_ai_handler);

    SFSpell *slowness = registrationAPI.registerSpell(kGdSpellLineSlowness);
    registrationAPI.linkTypeHandler(slowness, &slowness_handler);

    SFSpell *poison = registrationAPI.registerSpell(kGdSpellLinePoison);
    registrationAPI.linkTypeHandler(poison, &poison_handler);
    registrationAPI.linkSingleTargetAIHandler(poison, &fireburst_ai_handler);

    SFSpell *invulnerability = registrationAPI.registerSpell(kGdSpellLineInvulnerability);
    registrationAPI.linkTypeHandler(invulnerability, &invulnerability_handler);

    SFSpell *cure_poison = registrationAPI.registerSpell(kGdSpellLineCurePoison);
    registrationAPI.linkTypeHandler(cure_poison, &cure_poison_handler);
    registrationAPI.linkSingleTargetAIHandler(cure_poison, &cure_poison_ai_handler);

    // Empty Spell Type here

    SFSpell *freeze = registrationAPI.registerSpell(kGdSpellLineFreeze);
    registrationAPI.linkTypeHandler(freeze, &freeze_handler);
    registrationAPI.linkSingleTargetAIHandler(freeze, &freeze_ai_handler);

    SFSpell *fog = registrationAPI.registerSpell(kGdSpellLineFog);
    registrationAPI.linkTypeHandler(fog, &fog_handler);
    registrationAPI.applySpellTag(fog, SpellTag::AOE_SPELL);

    SFSpell *illuminate = registrationAPI.registerSpell(kGdSpellLineIlluminate);
    registrationAPI.linkTypeHandler(illuminate, &illuminate_handler);
    registrationAPI.applySpellTag(illuminate, SpellTag::AOE_SPELL);

    SFSpell *fireshield = registrationAPI.registerSpell(kGdSpellLineFireShield);
    registrationAPI.linkTypeHandler(fireshield, &fireshield_handler);
    registrationAPI.linkSingleTargetAIHandler(fireshield, &shields_ai_handler);

    SFSpell *fireball = registrationAPI.registerSpell(kGdSpellLineFireBall);
    registrationAPI.linkTypeHandler(fireball, &fireball_handler);
    registrationAPI.linkSingleTargetAIHandler(fireball, &fireburst_ai_handler);
    registrationAPI.applySpellTag(fireball, SpellTag::STACKABLE_SPELL);

    SFSpell *icestrike = registrationAPI.registerSpell(kGdSpellLineIceBurst);
    registrationAPI.linkTypeHandler(icestrike, &icestrike_handler);
    registrationAPI.linkSingleTargetAIHandler(icestrike, &fireburst_ai_handler);
    registrationAPI.applySpellTag(icestrike, SpellTag::STACKABLE_SPELL);

    SFSpell *iceshield = registrationAPI.registerSpell(kGdSpellLineIceShield);
    registrationAPI.linkTypeHandler(iceshield, &iceshield_handler);
    registrationAPI.linkSingleTargetAIHandler(iceshield, &shields_ai_handler);

    // next block

    SFSpell *decay_1 = registrationAPI.registerSpell(kGdSpellLineDecay);
    registrationAPI.linkTypeHandler(decay_1, &decay1_handler);

    SFSpell *decay_2 = registrationAPI.registerSpell(kGdSpellLineDecayArea);
    registrationAPI.linkTypeHandler(decay_2, &decay2_handler);

    SFSpell *pain = registrationAPI.registerSpell(kGdSpellLinePain);
    registrationAPI.linkTypeHandler(pain, &pain_handler);
    registrationAPI.linkSingleTargetAIHandler(pain, &death_ai_handler);

    SFSpell *lifetap = registrationAPI.registerSpell(kGdSpellLineLifeTap);
    registrationAPI.linkTypeHandler(lifetap, &lifetap_handler);
    registrationAPI.linkSingleTargetAIHandler(lifetap, &lifetap_ai_handler);

    SFSpell *summon_1 = registrationAPI.registerSpell(kGdSpellLineSummonGoblin);
    registrationAPI.linkTypeHandler(summon_1, &summons_handler);
    registrationAPI.applySpellTag(summon_1, SpellTag::SUMMON_SPELL);
    registrationAPI.applySpellTag(summon_1, SpellTag::STACKABLE_SPELL);
    registrationAPI.linkSingleTargetAIHandler(summon_1, &summon_ai_handler);

    SFSpell *hypnotize = registrationAPI.registerSpell(kGdSpellLineHypnotize);
    registrationAPI.linkTypeHandler(hypnotize, &hypnotize_handler);
    registrationAPI.linkSingleTargetAIHandler(hypnotize, &hypnotize_ai_handler);

    SFSpell *iceshield2 = registrationAPI.registerSpell(kGdSpellLineIceShieldStun);
    registrationAPI.linkTypeHandler(iceshield2, &iceshield2_handler);

    SFSpell *pestilence = registrationAPI.registerSpell(kGdSpellLinePestilence);
    registrationAPI.linkTypeHandler(pestilence, &pestilence_handler);
    registrationAPI.linkSingleTargetAIHandler(pestilence, &fireburst_ai_handler);

    SFSpell *cure_disease = registrationAPI.registerSpell(kGdSpellLineCureDisease);
    registrationAPI.linkTypeHandler(cure_disease, &cure_disease_handler);
    registrationAPI.linkSingleTargetAIHandler(cure_disease, &cure_disease_ai_handler);

    SFSpell *petrify = registrationAPI.registerSpell(kGdSpellLinePetrify);
    registrationAPI.linkTypeHandler(petrify, &petrify_handler);
    registrationAPI.linkSingleTargetAIHandler(petrify, &petrify_ai_handler);

    // Two unused spells here

    SFSpell *area_pain = registrationAPI.registerSpell(kGdSpellLinePainArea);
    registrationAPI.linkTypeHandler(area_pain, &area_pain_handler);
    registrationAPI.applySpellTag(area_pain, SpellTag::AOE_SPELL);

    SFSpell *summon_2 = registrationAPI.registerSpell(kGdSpellLineSummonSkeleton);
    registrationAPI.linkTypeHandler(summon_2, &summons_handler);
    registrationAPI.applySpellTag(summon_2, SpellTag::SUMMON_SPELL);
    registrationAPI.applySpellTag(summon_2, SpellTag::STACKABLE_SPELL);
    registrationAPI.linkSingleTargetAIHandler(summon_2, &summon_ai_handler);

    SFSpell *raise_dead = registrationAPI.registerSpell(kGdSpellLineRaiseDead);
    registrationAPI.linkTypeHandler(raise_dead, &raise_dead_handler);
    registrationAPI.applySpellTag(raise_dead, SpellTag::AOE_SPELL);

    SFSpell *summon_3 = registrationAPI.registerSpell(kGdSpellLineSummonDemon);
    registrationAPI.linkTypeHandler(summon_3, &summons_handler);
    registrationAPI.applySpellTag(summon_3, SpellTag::SUMMON_SPELL);
    registrationAPI.applySpellTag(summon_3, SpellTag::STACKABLE_SPELL);
    registrationAPI.linkSingleTargetAIHandler(summon_3, &summon_ai_handler);

    SFSpell *death_grasp = registrationAPI.registerSpell(kGdSpellLineDeathGrasp);
    registrationAPI.linkTypeHandler(death_grasp, &death_grasp_handler);
    registrationAPI.linkSingleTargetAIHandler(death_grasp, &death_grasp_ai_handler);

    SFSpell *summon_4 = registrationAPI.registerSpell(kGdSpellLineSummonChanneler);
    registrationAPI.linkTypeHandler(summon_4, &summons_handler);
    registrationAPI.applySpellTag(summon_4, SpellTag::SUMMON_SPELL);
    registrationAPI.applySpellTag(summon_4, SpellTag::STACKABLE_SPELL);
    registrationAPI.linkSingleTargetAIHandler(summon_4, &summon_ai_handler);

    SFSpell *inflexibility = registrationAPI.registerSpell(kGdSpellLineInflexibility);
    registrationAPI.linkTypeHandler(inflexibility, &inflexibility_handler);

    SFSpell *weaken = registrationAPI.registerSpell(kGdSpellLineWeaken);
    registrationAPI.linkTypeHandler(weaken, &weaken_handler);

    SFSpell *dark_banishing = registrationAPI.registerSpell(kGdSpellLineDarkBanishing);
    registrationAPI.linkTypeHandler(dark_banishing, &dark_banishing_handler);

    SFSpell *area_slowness = registrationAPI.registerSpell(kGdSpellLineSlownessArea);
    registrationAPI.linkTypeHandler(area_slowness, &area_slowness_handler);

    SFSpell *area_inflexibility = registrationAPI.registerSpell(kGdSpellLineInflexibilityArea);
    registrationAPI.linkTypeHandler(area_inflexibility, &area_inflexibility_handler);

    SFSpell *area_weaken = registrationAPI.registerSpell(kGdSpellLineWeakenArea);
    registrationAPI.linkTypeHandler(area_weaken, &area_weaken_handler);

    SFSpell *area_plauge = registrationAPI.registerSpell(kGdSpellLinePlagueArea);
    registrationAPI.linkTypeHandler(area_plauge, &area_plague_handler);

    SFSpell *remediless = registrationAPI.registerSpell(kGdSpellLineRemediless);
    registrationAPI.linkTypeHandler(remediless, &remediless_handler);

    // Unused Spell here

    SFSpell *area_healing = registrationAPI.registerSpell(kGdSpellLineHealingArea);
    registrationAPI.linkTypeHandler(area_healing, &area_healing_handler);
    registrationAPI.applySpellTag(area_healing, SpellTag::AOE_SPELL);

    SFSpell *sentinel_healing = registrationAPI.registerSpell(kGdSpellLineSentinelHealing);
    registrationAPI.linkTypeHandler(sentinel_healing, &sentinel_healing_handler);
    registrationAPI.applySpellTag(sentinel_healing, SpellTag::AOE_SPELL);

    SFSpell *healing2 = registrationAPI.registerSpell(kGdSpellLineGreaterHealing);
    registrationAPI.linkTypeHandler(healing2, &healing_handler);
    registrationAPI.applySpellTag(healing2, STACKABLE_SPELL);
    registrationAPI.linkSingleTargetAIHandler(healing2, &healing_ai_handler);

    SFSpell *charm_animal = registrationAPI.registerSpell(kGdSpellLineCharmAnimal);
    registrationAPI.linkTypeHandler(charm_animal, &charm_animal_handler);
    registrationAPI.applySpellTag(charm_animal, SpellTag::DOMINATION_SPELL);
    registrationAPI.linkSingleTargetAIHandler(charm_animal, &charm_ai_handler);

    SFSpell *thorn_shield = registrationAPI.registerSpell(kGdSpellLineThornShield);
    registrationAPI.linkTypeHandler(thorn_shield, &thorn_shield_handler);
    registrationAPI.linkSingleTargetAIHandler(thorn_shield, &shields_ai_handler);

    SFSpell *quickness = registrationAPI.registerSpell(kGdSpellLineQuickness);
    registrationAPI.linkTypeHandler(quickness, &quickness_handler);

    SFSpell *area_quickness = registrationAPI.registerSpell(kGdSpellLineQuicknessArea);
    registrationAPI.linkTypeHandler(area_quickness, &area_quickness_handler);

    SFSpell *flexibility = registrationAPI.registerSpell(kGdSpellLineFlexibility);
    registrationAPI.linkTypeHandler(flexibility, &flexibility_handler);

    SFSpell *area_flexibility = registrationAPI.registerSpell(kGdSpellLineFlexibilityArea);
    registrationAPI.linkTypeHandler(area_flexibility, &area_flexibility_handler);

    SFSpell *strength = registrationAPI.registerSpell(kGdSpellLineStrengthen);
    registrationAPI.linkTypeHandler(strength, &strength_handler);

    SFSpell *area_strength = registrationAPI.registerSpell(kGdSpellLineStrengthenArea);
    registrationAPI.linkTypeHandler(area_strength, &area_strength_handler);

    SFSpell *guard = registrationAPI.registerSpell(kGdSpellLineGuard);
    registrationAPI.linkTypeHandler(guard, &guard_handler);

    SFSpell *remove_curse = registrationAPI.registerSpell(kGdSpellLineRemoveCurse);
    registrationAPI.linkTypeHandler(remove_curse, &remove_curse_handler);

    SFSpell *regenerate = registrationAPI.registerSpell(kGdSpellLineRegenerate);
    registrationAPI.linkTypeHandler(regenerate, &regenerate_handler);

    SFSpell *holy_might = registrationAPI.registerSpell(kGdSpellLineHolyMight);
    registrationAPI.linkTypeHandler(holy_might, &default_handler);

    SFSpell *hallow = registrationAPI.registerSpell(kGdSpellLineHallow);
    registrationAPI.linkTypeHandler(hallow, &hallow_handler);
    registrationAPI.linkSingleTargetAIHandler(hallow, &hallow_ai_handler);
    registrationAPI.applySpellTag(hallow, SpellTag::STACKABLE_SPELL);

    // Unused spell here

    SFSpell *fireshield2 = registrationAPI.registerSpell(kGdSpellLineFireShieldDamage);
    registrationAPI.linkTypeHandler(fireshield2, &fireshield2_handler);

    SFSpell *thorn_shield2 = registrationAPI.registerSpell(kGdSpellLineThornShieldDamage);
    registrationAPI.linkTypeHandler(thorn_shield2, &thorn_shield2_handler);

    SFSpell *forget = registrationAPI.registerSpell(kGdSpellLineForget);
    registrationAPI.linkTypeHandler(forget, &forget_handler);

    SFSpell *self_illusion = registrationAPI.registerSpell(kGdSpellLineSelfIllusion);
    registrationAPI.linkTypeHandler(self_illusion, &self_illusion_handler);

    // Next Block

    SFSpell *retention = registrationAPI.registerSpell(kGdSpellLineRetention);
    registrationAPI.linkTypeHandler(retention, &retention_handler);

    SFSpell *brilliance = registrationAPI.registerSpell(kGdSpellLineBrilliance);
    registrationAPI.linkTypeHandler(brilliance, &brilliance_handler);

    SFSpell *sacrifice_mana = registrationAPI.registerSpell(kGdSpellLineSacrificeMana);
    registrationAPI.linkTypeHandler(sacrifice_mana, &sacrifice_mana_handler);
    registrationAPI.linkSingleTargetAIHandler(sacrifice_mana, &sacrifice_mana_ai_handler);

    SFSpell *manatap = registrationAPI.registerSpell(kGdSpellLineManaTap);
    registrationAPI.linkTypeHandler(manatap, &manatap_handler);
    registrationAPI.linkSingleTargetAIHandler(manatap, &manatap_ai_handler);

    SFSpell *mana_drain = registrationAPI.registerSpell(kGdSpellLineManaDrain);
    registrationAPI.linkTypeHandler(mana_drain, &manadrain_handler);

    SFSpell *shock = registrationAPI.registerSpell(kGdSpellLineShock);
    registrationAPI.linkTypeHandler(shock, &shock_handler);

    SFSpell *disrupt = registrationAPI.registerSpell(kGdSpellLineDisrupt);
    registrationAPI.linkTypeHandler(disrupt, &disrupt_handler);

    // Unused Spell Here

    SFSpell *confuse = registrationAPI.registerSpell(kGdSpellLineConfuse);
    registrationAPI.linkTypeHandler(confuse, &confuse_handler);
    registrationAPI.linkSingleTargetAIHandler(confuse, &confuse_ai_handler);

    SFSpell *rain_of_fire = registrationAPI.registerSpell(kGdSpellLineRainOfFire);
    registrationAPI.linkTypeHandler(rain_of_fire, &rain_of_fire_handler);
    registrationAPI.applySpellTag(rain_of_fire, SpellTag::AOE_SPELL);

    SFSpell *blizzard = registrationAPI.registerSpell(kGdSpellLineBlizzard);
    registrationAPI.linkTypeHandler(blizzard, &blizzard_handler);
    registrationAPI.applySpellTag(blizzard, SpellTag::AOE_SPELL);

    SFSpell *acid_cloud = registrationAPI.registerSpell(kGdSpellLineAcidCloud);
    registrationAPI.linkTypeHandler(acid_cloud, &acid_cloud_handler);
    registrationAPI.applySpellTag(acid_cloud, SpellTag::AOE_SPELL);

    SFSpell *stone_rain = registrationAPI.registerSpell(kGdSpellLineStoneRain);
    registrationAPI.linkTypeHandler(stone_rain, &stone_rain_handler);
    registrationAPI.applySpellTag(stone_rain, SpellTag::AOE_SPELL);

    // Unused Spell Here
    // Unused Spell Here

    SFSpell *amok = registrationAPI.registerSpell(kGdSpellLineAmok);
    registrationAPI.linkTypeHandler(amok, &amok_handler);
    registrationAPI.linkSingleTargetAIHandler(amok, &amok_ai_handler);

    // Next Block

    // Unused Spell Here

    SFSpell *extinct = registrationAPI.registerSpell(kGdSpellLineExtinct);
    registrationAPI.linkTypeHandler(extinct, &extinct_handler);
    registrationAPI.linkSingleTargetAIHandler(extinct, &extinct_ai_handler);

    SFSpell *detect_metal = registrationAPI.registerSpell(kGdSpellLineDetectMetal);
    registrationAPI.linkTypeHandler(detect_metal, &detect_metal_handler);

    SFSpell *detect_magic = registrationAPI.registerSpell(kGdSpellLineDetectMagic);
    registrationAPI.linkTypeHandler(detect_magic, &detect_magic_handler);

    // Unused Spell Here
    // Unused Spell Here

    SFSpell *invisibility = registrationAPI.registerSpell(kGdSpellLineInvisibility);
    registrationAPI.linkTypeHandler(invisibility, &invisibility_handler);

    SFSpell *stone = registrationAPI.registerSpell(kGdSpellLineStone);
    registrationAPI.linkTypeHandler(stone, &stone_handler);

    SFSpell *aura1 = registrationAPI.registerSpell(kGdSpellLineAuraWeakness);
    registrationAPI.linkTypeHandler(aura1, &aura_handler);
    registrationAPI.linkSingleTargetAIHandler(aura1, &offensive_aura_ai_handler);
    registrationAPI.applySpellTag(aura1, SpellTag::AURA_SPELL);
    registrationAPI.applySpellTag(aura1, SpellTag::BLACK_AURA_SPELL);

    SFSpell *aura2 = registrationAPI.registerSpell(kGdSpellLineAuraSuffocation);
    registrationAPI.linkTypeHandler(aura2, &aura_handler);
    registrationAPI.linkSingleTargetAIHandler(aura2, &offensive_aura_ai_handler);
    registrationAPI.applySpellTag(aura2, SpellTag::AURA_SPELL);
    registrationAPI.applySpellTag(aura2, SpellTag::BLACK_AURA_SPELL);

    SFSpell *suicide_death = registrationAPI.registerSpell(kGdSpellLineSuicideDeath);
    registrationAPI.linkTypeHandler(suicide_death, &suicide_death_handler);

    SFSpell *aura3 = registrationAPI.registerSpell(kGdSpellLineAuraLifeTap);
    registrationAPI.linkTypeHandler(aura3, &aura_handler);
    registrationAPI.linkSingleTargetAIHandler(aura3, &offensive_aura_ai_handler);
    registrationAPI.applySpellTag(aura3, SpellTag::AURA_SPELL);
    registrationAPI.applySpellTag(aura3, SpellTag::BLACK_AURA_SPELL);

    SFSpell *summon_5 = registrationAPI.registerSpell(kGdSpellLineSummonSpectre);
    registrationAPI.linkTypeHandler(summon_5, &summons_handler);
    registrationAPI.applySpellTag(summon_5, SpellTag::SUMMON_SPELL);
    registrationAPI.applySpellTag(summon_5, SpellTag::STACKABLE_SPELL);
    registrationAPI.linkSingleTargetAIHandler(summon_5, &summon_ai_handler);

    SFSpell *feign_death = registrationAPI.registerSpell(kGdSpellLineFeignDeath);
    registrationAPI.linkTypeHandler(feign_death, &feign_death_handler);

    SFSpell *aura4 = registrationAPI.registerSpell(kGdSpellLineAuraSlowFighting);
    registrationAPI.linkTypeHandler(aura4, &aura_handler);
    registrationAPI.linkSingleTargetAIHandler(aura4, &offensive_aura_ai_handler);
    registrationAPI.applySpellTag(aura4, SpellTag::AURA_SPELL);
    registrationAPI.applySpellTag(aura4, SpellTag::BLACK_AURA_SPELL);

    SFSpell *aura5 = registrationAPI.registerSpell(kGdSpellLineAuraInflexibility);
    registrationAPI.linkTypeHandler(aura5, &aura_handler);
    registrationAPI.linkSingleTargetAIHandler(aura5, &offensive_aura_ai_handler);
    registrationAPI.applySpellTag(aura5, SpellTag::AURA_SPELL);
    registrationAPI.applySpellTag(aura5, SpellTag::BLACK_AURA_SPELL);
    // Next Block

    SFSpell *dispel_white_aura = registrationAPI.registerSpell(kGdSpellLineDispelWhiteAura);
    registrationAPI.linkTypeHandler(dispel_white_aura, &dispel_white_aura_handler);
    registrationAPI.linkSingleTargetAIHandler(dispel_white_aura, &dispel_white_aura_ai_handler);

    SFSpell *aura6 = registrationAPI.registerSpell(kGdSpellLineAuraSlowWalking);
    registrationAPI.linkTypeHandler(aura6, &aura_handler);
    registrationAPI.linkSingleTargetAIHandler(aura6, &offensive_aura_ai_handler);
    registrationAPI.applySpellTag(aura6, SpellTag::AURA_SPELL);
    registrationAPI.applySpellTag(aura6, SpellTag::BLACK_AURA_SPELL);

    SFSpell *aura7 = registrationAPI.registerSpell(kGdSpellLineAuraInability);
    registrationAPI.linkTypeHandler(aura7, &aura_handler);
    registrationAPI.linkSingleTargetAIHandler(aura7, &offensive_aura_ai_handler);
    registrationAPI.applySpellTag(aura7, SpellTag::AURA_SPELL);
    registrationAPI.applySpellTag(aura7, SpellTag::BLACK_AURA_SPELL);

    SFSpell *suffocation = registrationAPI.registerSpell(kGdSpellLineSuffocation);
    registrationAPI.linkTypeHandler(suffocation, &suffocation_handler);

    SFSpell *inablility = registrationAPI.registerSpell(kGdSpellLineInability);
    registrationAPI.linkTypeHandler(inablility, &inablility_handler);

    SFSpell *slow_fighting = registrationAPI.registerSpell(kGdSpellLineSlowFighting);
    registrationAPI.linkTypeHandler(slow_fighting, &slow_fighting_handler);

    SFSpell *aura8 = registrationAPI.registerSpell(kGdSpellLineAuraStrength);
    registrationAPI.linkTypeHandler(aura8, &aura_handler);
    registrationAPI.linkSingleTargetAIHandler(aura8, &defensive_aura_ai_handler);
    registrationAPI.applySpellTag(aura8, SpellTag::AURA_SPELL);
    registrationAPI.applySpellTag(aura8, SpellTag::WHITE_AURA_SPELL);

    SFSpell *aura9 = registrationAPI.registerSpell(kGdSpellLineAuraHealing);
    registrationAPI.linkTypeHandler(aura9, &aura_handler);
    registrationAPI.linkSingleTargetAIHandler(aura9, &healing_aura_ai_handler);
    registrationAPI.applySpellTag(aura9, SpellTag::AURA_SPELL);
    registrationAPI.applySpellTag(aura9, SpellTag::WHITE_AURA_SPELL);

    SFSpell *aura10 = registrationAPI.registerSpell(kGdSpellLineAuraEndurance);
    registrationAPI.linkTypeHandler(aura10, &aura_handler);
    registrationAPI.linkSingleTargetAIHandler(aura10, &defensive_aura_ai_handler);
    registrationAPI.applySpellTag(aura10, SpellTag::AURA_SPELL);
    registrationAPI.applySpellTag(aura10, SpellTag::WHITE_AURA_SPELL);

    SFSpell *suicide_heal = registrationAPI.registerSpell(kGdSpellLineSuicideHeal);
    registrationAPI.linkTypeHandler(suicide_heal, &suicide_heal_handler);

    SFSpell *summon_6 = registrationAPI.registerSpell(kGdSpellLineSummonWolf);
    registrationAPI.linkTypeHandler(summon_6, &summons_handler);
    registrationAPI.applySpellTag(summon_6, SpellTag::SUMMON_SPELL);
    registrationAPI.applySpellTag(summon_6, SpellTag::STACKABLE_SPELL);
    registrationAPI.linkSingleTargetAIHandler(summon_6, &summon_ai_handler);

    SFSpell *aura11 = registrationAPI.registerSpell(kGdSpellLineAuraRegeneration);
    registrationAPI.linkTypeHandler(aura11, &aura_handler);
    registrationAPI.linkSingleTargetAIHandler(aura11, &healing_aura_ai_handler);
    registrationAPI.applySpellTag(aura11, SpellTag::AURA_SPELL);
    registrationAPI.applySpellTag(aura11, SpellTag::WHITE_AURA_SPELL);

    SFSpell *dominate_animal = registrationAPI.registerSpell(kGdSpellLineDominateAnimal);
    registrationAPI.linkTypeHandler(dominate_animal, &dominate_animal_handler);
    registrationAPI.applySpellTag(dominate_animal, SpellTag::DOMINATION_SPELL);
    registrationAPI.linkSingleTargetAIHandler(dominate_animal, &dominate_ai_handler);

    // Would this defeat Jack Slash?
    SFSpell *summon_7 = registrationAPI.registerSpell(kGdSpellLineSummonBear);
    registrationAPI.linkTypeHandler(summon_7, &summons_handler);
    registrationAPI.applySpellTag(summon_7, SpellTag::SUMMON_SPELL);
    registrationAPI.applySpellTag(summon_7, SpellTag::STACKABLE_SPELL);
    registrationAPI.linkSingleTargetAIHandler(summon_7, &summon_ai_handler);

    SFSpell *aura12 = registrationAPI.registerSpell(kGdSpellLineAuraFastFighting);
    registrationAPI.linkTypeHandler(aura12, &aura_handler);
    registrationAPI.linkSingleTargetAIHandler(aura12, &defensive_aura_ai_handler);
    registrationAPI.applySpellTag(aura12, SpellTag::AURA_SPELL);
    registrationAPI.applySpellTag(aura12, SpellTag::WHITE_AURA_SPELL);

    SFSpell *aura13 = registrationAPI.registerSpell(kGdSpellLineAuraFlexibility);
    registrationAPI.linkTypeHandler(aura13, &aura_handler);
    registrationAPI.linkSingleTargetAIHandler(aura13, &defensive_aura_ai_handler);
    registrationAPI.applySpellTag(aura13, SpellTag::AURA_SPELL);
    registrationAPI.applySpellTag(aura13, SpellTag::WHITE_AURA_SPELL);

    SFSpell *dispel_black_aura = registrationAPI.registerSpell(kGdSpellLineDispelBlackAura);
    registrationAPI.linkTypeHandler(dispel_black_aura, &dispel_black_aura_handler);

    SFSpell *aura14 = registrationAPI.registerSpell(kGdSpellLineAuraFastWalking);
    registrationAPI.linkTypeHandler(aura14, &aura_handler);
    registrationAPI.linkSingleTargetAIHandler(aura14, &defensive_aura_ai_handler);
    registrationAPI.applySpellTag(aura14, SpellTag::AURA_SPELL);
    registrationAPI.applySpellTag(aura14, SpellTag::WHITE_AURA_SPELL);

    SFSpell *aura15 = registrationAPI.registerSpell(kGdSpellLineAuraLight);
    registrationAPI.linkTypeHandler(aura15, &aura_handler);
    registrationAPI.linkSingleTargetAIHandler(aura15, &aura_light_ai_handler);
    registrationAPI.applySpellTag(aura15, SpellTag::AURA_SPELL);
    registrationAPI.applySpellTag(aura15, SpellTag::WHITE_AURA_SPELL);

    SFSpell *aura16 = registrationAPI.registerSpell(kGdSpellLineAuraDexterity);
    registrationAPI.linkTypeHandler(aura16, &aura_handler);
    registrationAPI.linkSingleTargetAIHandler(aura16, &defensive_aura_ai_handler);
    registrationAPI.applySpellTag(aura16, SpellTag::AURA_SPELL);
    registrationAPI.applySpellTag(aura16, SpellTag::WHITE_AURA_SPELL);

    SFSpell *dexterity = registrationAPI.registerSpell(kGdSpellLineDexterity);
    registrationAPI.linkTypeHandler(dexterity, &dexterity_handler);
    registrationAPI.linkSingleTargetAIHandler(dexterity, &shields_ai_handler);

    SFSpell *endurance = registrationAPI.registerSpell(kGdSpellLineEndurance);
    registrationAPI.linkTypeHandler(endurance, &endurance_handler);

    SFSpell *fast_fighting = registrationAPI.registerSpell(kGdSpellLineFastFighting);
    registrationAPI.linkTypeHandler(fast_fighting, &fast_fighting_handler);

    SFSpell *distract = registrationAPI.registerSpell(kGdSpellLineDistract);
    registrationAPI.linkTypeHandler(distract, &distract_handler);
    registrationAPI.linkSingleTargetAIHandler(distract, &wave_ai_handler);

    SFSpell *dominate = registrationAPI.registerSpell(kGdSpellLineDominate);
    registrationAPI.linkTypeHandler(dominate, &dominate_handler);
    registrationAPI.applySpellTag(dominate, SpellTag::DOMINATION_SPELL);
    registrationAPI.linkSingleTargetAIHandler(dominate, &dominate_ai_handler);

    // Unused Spell Here

    SFSpell *charm = registrationAPI.registerSpell(kGdSpellLineCharm);
    registrationAPI.linkTypeHandler(charm, &charm_handler);
    registrationAPI.applySpellTag(charm, SpellTag::DOMINATION_SPELL);
    registrationAPI.linkSingleTargetAIHandler(charm, &charm_ai_handler);

    SFSpell *befriend = registrationAPI.registerSpell(kGdSpellLineBefriend);
    registrationAPI.linkTypeHandler(befriend, &befriend_handler);
    registrationAPI.linkSingleTargetAIHandler(befriend, &befriend_ai_handler);

    SFSpell *disenchant = registrationAPI.registerSpell(kGdSpellLineDisenchant);
    registrationAPI.linkTypeHandler(disenchant, &disenchant_handler);
    registrationAPI.linkSingleTargetAIHandler(disenchant, &befriend_ai_handler);

    SFSpell *charisma = registrationAPI.registerSpell(kGdSpellLineCharisma);
    registrationAPI.linkTypeHandler(charisma, &charisma_handler);

    SFSpell *shockwave = registrationAPI.registerSpell(kGdSpellLineShockwave);
    registrationAPI.linkTypeHandler(shockwave, &shockwave_handler);
    registrationAPI.linkSingleTargetAIHandler(shockwave, &wave_ai_handler);

    SFSpell *aura17 = registrationAPI.registerSpell(kGdSpellLineAuraHypnotization);
    registrationAPI.linkTypeHandler(aura17, &aura_handler);
    registrationAPI.linkSingleTargetAIHandler(aura17, &offensive_aura_ai_handler);
    registrationAPI.applySpellTag(aura17, SpellTag::AURA_SPELL);

    // Next Spell Block

    SFSpell *demoralization = registrationAPI.registerSpell(kGdSpellLineDemoralization);
    registrationAPI.linkTypeHandler(demoralization, &demoralization_handler);
    registrationAPI.linkSingleTargetAIHandler(demoralization, &demoralization_ai_handler);

    SFSpell *aura18 = registrationAPI.registerSpell(kGdSpellLineAuraBrilliance);
    registrationAPI.linkTypeHandler(aura18, &aura_handler);
    registrationAPI.linkSingleTargetAIHandler(aura18, &defensive_aura_ai_handler);
    registrationAPI.applySpellTag(aura18, SpellTag::AURA_SPELL);

    SFSpell *enlightenment = registrationAPI.registerSpell(kGdSpellLineEnlightenment);
    registrationAPI.linkTypeHandler(enlightenment, &enlightenment_handler);

    SFSpell *aura19 = registrationAPI.registerSpell(kGdSpellLineAuraManaTap);
    registrationAPI.linkTypeHandler(aura19, &aura_handler);
    registrationAPI.linkSingleTargetAIHandler(aura19, &offensive_aura_ai_handler);
    registrationAPI.applySpellTag(aura19, SpellTag::AURA_SPELL);

    SFSpell *meditation = registrationAPI.registerSpell(kGdSpellLineMeditation);
    registrationAPI.linkTypeHandler(meditation, &meditation_handler);

    SFSpell *summon_8 = registrationAPI.registerSpell(kGdSpellLineFireElemental);
    registrationAPI.linkTypeHandler(summon_8, &summons_handler);
    registrationAPI.applySpellTag(summon_8, SpellTag::SUMMON_SPELL);
    registrationAPI.applySpellTag(summon_8, SpellTag::STACKABLE_SPELL);
    registrationAPI.linkSingleTargetAIHandler(summon_8, &summon_ai_handler);

    SFSpell *wave = registrationAPI.registerSpell(kGdSpellLineWaveOfFire);
    registrationAPI.linkTypeHandler(wave, &wave_handler);
    registrationAPI.linkSingleTargetAIHandler(wave, &wave_ai_handler);

    SFSpell *melt_resistance = registrationAPI.registerSpell(kGdSpellLineMeltResistance);
    registrationAPI.linkTypeHandler(melt_resistance, &melt_resistance_handler);

    SFSpell *summon_9 = registrationAPI.registerSpell(kGdSpellLineIceElemental);
    registrationAPI.linkTypeHandler(summon_9, &summons_handler);
    registrationAPI.applySpellTag(summon_9, SpellTag::SUMMON_SPELL);
    registrationAPI.applySpellTag(summon_9, SpellTag::STACKABLE_SPELL);
    registrationAPI.linkSingleTargetAIHandler(summon_9, &summon_ai_handler);

    SFSpell *wave2 = registrationAPI.registerSpell(kGdSpellLineWaveOfIce);
    registrationAPI.linkTypeHandler(wave2, &wave_handler);
    registrationAPI.linkSingleTargetAIHandler(wave2, &wave_ai_handler);

    SFSpell *chill_resistance = registrationAPI.registerSpell(kGdSpellLineChillResistance);
    registrationAPI.linkTypeHandler(chill_resistance, &chill_resistance_handler);

    SFSpell *rock_bullet = registrationAPI.registerSpell(kGdSpellLineRockBullet);
    registrationAPI.linkTypeHandler(rock_bullet, &rock_bullet_handler);

    SFSpell *conservation = registrationAPI.registerSpell(kGdSpellLineConservation);
    registrationAPI.linkTypeHandler(conservation, &conservation_handler);
    registrationAPI.linkSingleTargetAIHandler(conservation, &shields_ai_handler);

    SFSpell *summon_10 = registrationAPI.registerSpell(kGdSpellLineEarthElemental);
    registrationAPI.linkTypeHandler(summon_10, &summons_handler);
    registrationAPI.applySpellTag(summon_10, SpellTag::SUMMON_SPELL);
    registrationAPI.applySpellTag(summon_10, SpellTag::STACKABLE_SPELL);
    registrationAPI.linkSingleTargetAIHandler(summon_10, &summon_ai_handler);

    SFSpell *wave3 = registrationAPI.registerSpell(kGdSpellLineWaveOfRocks);
    registrationAPI.linkTypeHandler(wave3, &wave_handler);
    registrationAPI.linkSingleTargetAIHandler(wave3, &wave_ai_handler);

    SFSpell *tower_arrow = registrationAPI.registerSpell(kGdSpellLineArrowTower);
    registrationAPI.linkTypeHandler(tower_arrow, &tower_arrow_handler);

    // Next Spell Block

    SFSpell *tower_healing = registrationAPI.registerSpell(kGdSpellLineHealingTower);
    registrationAPI.linkTypeHandler(tower_healing, &tower_healing_handler);
    registrationAPI.applySpellTag(tower_healing, STACKABLE_SPELL);
    registrationAPI.linkSingleTargetAIHandler(tower_healing, &healing_ai_handler);

    SFSpell *tower_icestrike = registrationAPI.registerSpell(kGdSpellLineIceburstTower);
    registrationAPI.linkTypeHandler(tower_icestrike, &tower_icestrike_handler);
    registrationAPI.applySpellTag(tower_icestrike, SpellTag::STACKABLE_SPELL);
    registrationAPI.linkSingleTargetAIHandler(tower_icestrike, &fireburst_ai_handler);

    SFSpell *lifetap_aura = registrationAPI.registerSpell(kGdSpellLineLifeTapAura);
    registrationAPI.linkTypeHandler(lifetap_aura, &lifetap_aura_handler);

    SFSpell *fireball2 = registrationAPI.registerSpell(kGdSpellLineFireBallEffect);
    registrationAPI.linkTypeHandler(fireball2, &fireball2_handler);

    SFSpell *ability_warcry = registrationAPI.registerSpell(kGdSpellLineAbilityWarCry);
    registrationAPI.linkOnHitHandler(ability_warcry, &warcry_onhit_handler, PHASE_0);
    registrationAPI.linkTypeHandler(ability_warcry, &ability_warcry_handler);
    registrationAPI.applySpellTag(ability_warcry, STACKABLE_SPELL);
    registrationAPI.linkSingleTargetAIHandler(ability_warcry, &HCA_ability_ai_handler);
    registrationAPI.applySpellTag(ability_warcry, SpellTag::COMBAT_ABILITY_SPELL);
    registrationAPI.applySpellTag(ability_warcry, SpellTag::AOE_SPELL);

    SFSpell *ability_benefactions = registrationAPI.registerSpell(kGdSpellLineAbilityBenefactions);
    registrationAPI.linkTypeHandler(ability_benefactions, &ability_benefactions_handler);
    registrationAPI.applySpellTag(ability_benefactions, STACKABLE_SPELL);
    registrationAPI.linkSingleTargetAIHandler(ability_benefactions, &benefactions_ai_handler);
    registrationAPI.applySpellTag(ability_benefactions, SpellTag::COMBAT_ABILITY_SPELL);
    registrationAPI.applySpellTag(ability_benefactions, SpellTag::AOE_SPELL);

    SFSpell *ability_patronize = registrationAPI.registerSpell(kGdSpellLineAbilityPatronize);
    registrationAPI.linkTypeHandler(ability_patronize, &ability_patronize_handler);
    registrationAPI.applySpellTag(ability_patronize, STACKABLE_SPELL);
    registrationAPI.linkSingleTargetAIHandler(ability_patronize, &HCA_ability_ai_handler);
    registrationAPI.applySpellTag(ability_patronize, SpellTag::COMBAT_ABILITY_SPELL);
    registrationAPI.applySpellTag(ability_patronize, SpellTag::AOE_SPELL);

    SFSpell *ability_endurance = registrationAPI.registerSpell(kGdSpellLineAbilityEndurance);
    registrationAPI.linkTypeHandler(ability_endurance, &ability_endurance_handler);
    registrationAPI.linkOnHitHandler(ability_endurance, &endurance_onhit_handler, PHASE_0);
    registrationAPI.applySpellTag(ability_endurance, STACKABLE_SPELL);
    registrationAPI.linkSingleTargetAIHandler(ability_endurance, &HCA_ability_ai_handler);
    registrationAPI.applySpellTag(ability_endurance, SpellTag::COMBAT_ABILITY_SPELL);
    registrationAPI.applySpellTag(ability_endurance, SpellTag::AOE_SPELL);

    SFSpell *ability_berserk = registrationAPI.registerSpell(kGdSpellLineAbilityBerserk);
    registrationAPI.linkOnHitHandler(ability_berserk, &berserk_onhit_handler, PHASE_0);
    registrationAPI.linkTypeHandler(ability_berserk, &ability_berserk_handler);
    registrationAPI.linkSingleTargetAIHandler(ability_berserk, &berserk_ai_handler);
    registrationAPI.applySpellTag(ability_berserk, SpellTag::COMBAT_ABILITY_SPELL);

    SFSpell *ability_boons = registrationAPI.registerSpell(kGdSpellLineAbilityBlessing);
    registrationAPI.linkTypeHandler(ability_boons, &ability_boons_handler);
    registrationAPI.linkSingleTargetAIHandler(ability_boons, &blessing_ai_handler);
    registrationAPI.applySpellTag(ability_boons, SpellTag::COMBAT_ABILITY_SPELL);

    SFSpell *ability_shelter = registrationAPI.registerSpell(kGdSpellLineAbilityShelter);
    registrationAPI.linkTypeHandler(ability_shelter, &ability_shelter_handler);
    registrationAPI.linkSingleTargetAIHandler(ability_shelter, &RCA_ability_ai_handler);
    registrationAPI.applySpellTag(ability_shelter, SpellTag::COMBAT_ABILITY_SPELL);

    SFSpell *ability_durability = registrationAPI.registerSpell(kGdSpellLineAbilityDurability);
    registrationAPI.linkTypeHandler(ability_durability, &ability_durability_handler);
    registrationAPI.linkOnHitHandler(ability_durability, &durability_onhit_handler, PHASE_0);
    registrationAPI.linkSingleTargetAIHandler(ability_durability, &riposte_ability_ai_handler);
    registrationAPI.applySpellTag(ability_durability, SpellTag::COMBAT_ABILITY_SPELL);

    SFSpell *ability_trueshot = registrationAPI.registerSpell(kGdSpellLineAbilityTrueShot);
    registrationAPI.linkTypeHandler(ability_trueshot, &ability_trueshot_handler);
    registrationAPI.linkOnHitHandler(ability_trueshot, &trueshot_onhit_handler, PHASE_0);
    registrationAPI.linkSingleTargetAIHandler(ability_trueshot, &RCA_ability_ai_handler);
    registrationAPI.applySpellTag(ability_trueshot, SpellTag::COMBAT_ABILITY_SPELL);

    SFSpell *ability_steelskin = registrationAPI.registerSpell(kGdSpellLineAbilitySteelSkin);
    registrationAPI.linkTypeHandler(ability_steelskin, &ability_steelskin_handler);
    registrationAPI.linkSingleTargetAIHandler(ability_steelskin, &RCA_ability_ai_handler);
    registrationAPI.applySpellTag(ability_steelskin, SpellTag::COMBAT_ABILITY_SPELL);

    SFSpell *ability_salvo = registrationAPI.registerSpell(kGdSpellLineAbilitySalvo);
    registrationAPI.linkTypeHandler(ability_salvo, &ability_salvo_handler);
    registrationAPI.linkSingleTargetAIHandler(ability_salvo, &RCA_ability_ai_handler);
    registrationAPI.applySpellTag(ability_salvo, SpellTag::COMBAT_ABILITY_SPELL);

    SFSpell *fireburst2 = registrationAPI.registerSpell(kGdSpellLineFireBurstTower);
    registrationAPI.linkTypeHandler(fireburst2, &fireburst_handler);
    registrationAPI.applySpellTag(fireburst2, SpellTag::STACKABLE_SPELL);
    registrationAPI.linkSingleTargetAIHandler(fireburst2, &fireburst_ai_handler);

    // Next Spell Block

    SFSpell *spark = registrationAPI.registerSpell(kGdSpellLineSpark);
    registrationAPI.linkTypeHandler(spark, &spark_handler);

    SFSpell *tower_hypnotize = registrationAPI.registerSpell(kGdSpellLineHypnotizeTower);
    registrationAPI.linkTypeHandler(tower_hypnotize, &tower_hypnotize_handler);
    registrationAPI.linkSingleTargetAIHandler(tower_hypnotize, &hypnotize_ai_handler);

    SFSpell *tower_pain = registrationAPI.registerSpell(kGdSpellLinePainTower);
    registrationAPI.linkTypeHandler(tower_pain, &tower_pain_handler);

    SFSpell *tower_stone = registrationAPI.registerSpell(kGdSpellLineStoneTower);
    registrationAPI.linkTypeHandler(tower_stone, &tower_stone_handler);

    SFSpell *cloak_of_nor = registrationAPI.registerSpell(kGdSpellLineProtectionBlack);
    registrationAPI.linkTypeHandler(cloak_of_nor, &cloak_of_nor_handler);
    registrationAPI.linkSingleTargetAIHandler(cloak_of_nor, &do_not_cast_ai_handler);

    SFSpell *unkn_spell2 = registrationAPI.registerSpell(kGdSpellLineDoNotTouchMe);
    registrationAPI.linkTypeHandler(unkn_spell2, &unkn_spell2_handler);

    SFSpell *healing3 = registrationAPI.registerSpell(kGdSpellLineHealingAura);
    registrationAPI.linkTypeHandler(healing3, &healing_handler);

    SFSpell *hypnotize2 = registrationAPI.registerSpell(kGdSpellLineHypnotizeTwo);
    registrationAPI.linkTypeHandler(hypnotize2, &hypnotize_handler);
    registrationAPI.linkSingleTargetAIHandler(hypnotize2, &hypnotize_ai_handler);

    SFSpell *freeze2 = registrationAPI.registerSpell(kGdSpellLineIceArrowEffect);
    registrationAPI.linkTypeHandler(freeze2, &freeze2_handler);

    SFSpell *freeze3 = registrationAPI.registerSpell(kGdSpellLineIceBlockEffect);
    registrationAPI.linkTypeHandler(freeze3, &freeze3_handler);

    SFSpell *lava_bullet = registrationAPI.registerSpell(kGdSpellLineFireBlockEffect);
    registrationAPI.linkTypeHandler(lava_bullet, &lava_bullet_handler);

    SFSpell *tower_extinct = registrationAPI.registerSpell(kGdSpellLineExtinctTower);
    registrationAPI.linkTypeHandler(tower_extinct, &tower_extinct_handler);
    registrationAPI.linkSingleTargetAIHandler(tower_extinct, &tower_extinct_ai_handler);

    SFSpell *manatap_aura = registrationAPI.registerSpell(kGdSpellLineManaTapAura);
    registrationAPI.linkTypeHandler(manatap_aura, &manatap_aura_handler);

    SFSpell *firebane = registrationAPI.registerSpell(kGdSpellLineFireResistance);
    registrationAPI.linkTypeHandler(firebane, &firebane_handler);

    SFSpell *black_essence = registrationAPI.registerSpell(kGdSpellLineEssenceBlack);
    registrationAPI.linkTypeHandler(black_essence, &black_essence_handler);

    SFSpell *white_essence = registrationAPI.registerSpell(kGdSpellLineEssenceWhite);
    registrationAPI.linkTypeHandler(white_essence, &white_essence_handler);

    // Next Spell Block

    SFSpell *elemental_essence = registrationAPI.registerSpell(kGdSpellLineEssenceElemental);
    registrationAPI.linkTypeHandler(elemental_essence, &elemental_essence_handler);

    SFSpell *mental_essence = registrationAPI.registerSpell(kGdSpellLineEssenceMental);
    registrationAPI.linkTypeHandler(mental_essence, &mental_essence_handler);

    SFSpell *black_almightness = registrationAPI.registerSpell(kGdSpellLineAlmightinessBlack);
    registrationAPI.linkTypeHandler(black_almightness, &black_almightness_handler);

    SFSpell *white_almightness = registrationAPI.registerSpell(kGdSpellLineAlmightinessWhite);
    registrationAPI.linkTypeHandler(white_almightness, &white_almightness_handler);

    SFSpell *elemental_almightness = registrationAPI.registerSpell(kGdSpellLineAlmightinessElemental);
    registrationAPI.linkTypeHandler(elemental_almightness, &elemental_almightness_handler);

    SFSpell *mental_almightness = registrationAPI.registerSpell(kGdSpellLineAlmightinessMental);
    registrationAPI.linkTypeHandler(mental_almightness, &mental_almightness_handler);

    SFSpell *elemental_almightness2 = registrationAPI.registerSpell(kGdSpellLineAlmightinessElementalEffect);
    registrationAPI.linkTypeHandler(elemental_almightness2, &elemental_almightness2_handler);

    SFSpell *elemental_essence2 = registrationAPI.registerSpell(kGdSpellLineEssenceElementalEffect);
    registrationAPI.linkTypeHandler(elemental_essence2, &elemental_essence2_handler);

    SFSpell *assistance = registrationAPI.registerSpell(kGdSpellLineAssistance);
    registrationAPI.linkOnHitHandler(assistance, &assistance_onhit_handler, PHASE_2);
    registrationAPI.applySpellTag(assistance, TARGET_ONHIT_SPELL);
    registrationAPI.linkTypeHandler(assistance, &assistance_handler);

    SFSpell *holy_touch = registrationAPI.registerSpell(kGdSpellLineHolyTouch);
    registrationAPI.linkTypeHandler(holy_touch, &holy_touch_handler);

    SFSpell *revenge = registrationAPI.registerSpell(kGdSpellLineRevenge);
    registrationAPI.linkTypeHandler(revenge, &revenge_handler);
    registrationAPI.applySpellTag(revenge, SpellTag::AOE_SPELL);

    SFSpell *area_roots = registrationAPI.registerSpell(kGdSpellLineRootsArea);
    registrationAPI.linkTypeHandler(area_roots, &area_roots_handler);
    registrationAPI.applySpellTag(area_roots, SpellTag::AOE_SPELL);

    SFSpell *summon_11 = registrationAPI.registerSpell(kGdSpellLineSummonTreeWraith);
    registrationAPI.linkTypeHandler(summon_11, &summons_handler);
    registrationAPI.applySpellTag(summon_11, SpellTag::SUMMON_SPELL);
    registrationAPI.applySpellTag(summon_11, SpellTag::STACKABLE_SPELL);
    registrationAPI.linkSingleTargetAIHandler(summon_11, &summon_ai_handler);

    SFSpell *roots = registrationAPI.registerSpell(kGdSpellLineRoots);
    registrationAPI.linkTypeHandler(roots, &roots_handler);

    SFSpell *chain = registrationAPI.registerSpell(kGdSpellLineChainHallow);
    registrationAPI.linkTypeHandler(chain, &chain_handler);

    SFSpell *reinforcement = registrationAPI.registerSpell(kGdSpellLineReinforcement);
    registrationAPI.linkTypeHandler(reinforcement, &reinforcement_handler);
    registrationAPI.applySpellTag(reinforcement, SpellTag::AOE_SPELL);

    // Next Spell Block

    SFSpell *aura20 = registrationAPI.registerSpell(kGdSpellLineAuraEternity);
    registrationAPI.linkTypeHandler(aura20, &aura_handler);
    registrationAPI.linkSingleTargetAIHandler(aura20, &defensive_aura_ai_handler);
    registrationAPI.applySpellTag(aura20, SpellTag::AURA_SPELL);
    registrationAPI.applySpellTag(aura1, SpellTag::WHITE_AURA_SPELL);

    SFSpell *chain2 = registrationAPI.registerSpell(kGdSpellLineChainPain);
    registrationAPI.linkTypeHandler(chain2, &chain_handler);

    SFSpell *cannibalize = registrationAPI.registerSpell(kGdSpellLineCannibalize);
    registrationAPI.linkTypeHandler(cannibalize, &cannibalize_handler);

    SFSpell *torture = registrationAPI.registerSpell(kGdSpellLineTorture);
    registrationAPI.linkTypeHandler(torture, &torture_handler);
    registrationAPI.applySpellTag(torture, SpellTag::AOE_SPELL);

    SFSpell *chain3 = registrationAPI.registerSpell(kGdSpellLineChainLifetap);
    registrationAPI.linkTypeHandler(chain3, &chain_handler);

    SFSpell *dominate_undead = registrationAPI.registerSpell(kGdSpellLineDominateUndead);
    registrationAPI.linkTypeHandler(dominate_undead, &dominate_undead_handler);
    registrationAPI.applySpellTag(dominate_undead, SpellTag::DOMINATION_SPELL);

    SFSpell *summon_12 = registrationAPI.registerSpell(kGdSpellLineSummonBlade);
    registrationAPI.linkTypeHandler(summon_12, &summons_handler);
    registrationAPI.applySpellTag(summon_12, SpellTag::SUMMON_SPELL);
    registrationAPI.applySpellTag(summon_12, SpellTag::STACKABLE_SPELL);
    registrationAPI.linkSingleTargetAIHandler(summon_12, &summon_ai_handler);

    SFSpell *mutation = registrationAPI.registerSpell(kGdSpellLineMutation);
    registrationAPI.linkTypeHandler(mutation, &mutation_handler);

    SFSpell *area_darkness = registrationAPI.registerSpell(kGdSpellLineDarknessArea);
    registrationAPI.linkTypeHandler(area_darkness, &area_darkness_handler);
    registrationAPI.applySpellTag(area_darkness, SpellTag::AOE_SPELL);

    SFSpell *chain4 = registrationAPI.registerSpell(kGdSpellLineChainMutation);
    registrationAPI.linkTypeHandler(chain4, &chain_handler);

    SFSpell *chain5 = registrationAPI.registerSpell(kGdSpellLineChainFireburst);
    registrationAPI.linkTypeHandler(chain5, &chain_handler);

    SFSpell *summon_13 = registrationAPI.registerSpell(kGdSpellLineSummonFireGolem);
    registrationAPI.linkTypeHandler(summon_13, &summons_handler);
    registrationAPI.applySpellTag(summon_13, SpellTag::SUMMON_SPELL);
    registrationAPI.applySpellTag(summon_13, SpellTag::STACKABLE_SPELL);
    registrationAPI.linkSingleTargetAIHandler(summon_13, &summon_ai_handler);

    SFSpell *chain6 = registrationAPI.registerSpell(kGdSpellLineChainFireball);
    registrationAPI.linkTypeHandler(chain6, &chain_handler);

    SFSpell *chain7 = registrationAPI.registerSpell(kGdSpellLineChainIceburst);
    registrationAPI.linkTypeHandler(chain7, &chain_handler);

    SFSpell *summon_14 = registrationAPI.registerSpell(kGdSpellLineSummonIceGolem);
    registrationAPI.linkTypeHandler(summon_14, &summons_handler);
    registrationAPI.applySpellTag(summon_14, SpellTag::SUMMON_SPELL);
    registrationAPI.applySpellTag(summon_14, SpellTag::STACKABLE_SPELL);
    registrationAPI.linkSingleTargetAIHandler(summon_14, &summon_ai_handler);

    SFSpell *area_freeze = registrationAPI.registerSpell(kGdSpellLineFreezeArea);
    registrationAPI.linkTypeHandler(area_freeze, &area_freeze_handler);
    registrationAPI.applySpellTag(area_freeze, SpellTag::AOE_SPELL);

    // Next Spell Block

    SFSpell *chain8 = registrationAPI.registerSpell(kGdSpellLineChainRockBullet);
    registrationAPI.linkTypeHandler(chain8, &chain_handler);

    SFSpell *summon_15 = registrationAPI.registerSpell(kGdSpellLineSummonEarthGolem);
    registrationAPI.linkTypeHandler(summon_15, &summons_handler);
    registrationAPI.applySpellTag(summon_15, SpellTag::SUMMON_SPELL);
    registrationAPI.applySpellTag(summon_15, SpellTag::STACKABLE_SPELL);
    registrationAPI.linkSingleTargetAIHandler(summon_15, &summon_ai_handler);

    SFSpell *clay_feet = registrationAPI.registerSpell(kGdSpellLineFeetClay);
    registrationAPI.linkTypeHandler(clay_feet, &clay_feet_handler);
    registrationAPI.applySpellTag(clay_feet, SpellTag::AOE_SPELL);

    SFSpell *mirage = registrationAPI.registerSpell(kGdSpellLineMirrorImage);
    registrationAPI.linkTypeHandler(mirage, &mirage_handler);

    SFSpell *chain9 = registrationAPI.registerSpell(kGdSpellLineChainCharm);
    registrationAPI.linkTypeHandler(chain9, &chain_handler);

    SFSpell *feedback = registrationAPI.registerSpell(kGdSpellLineVoodoo);
    registrationAPI.linkTypeHandler(feedback, &feedback_handler);

    SFSpell *chain10 = registrationAPI.registerSpell(kGdSpellLineChainShock);
    registrationAPI.linkTypeHandler(chain10, &chain_handler);

    SFSpell *area_hypnotize = registrationAPI.registerSpell(kGdSpellLineHypnotizeArea);
    registrationAPI.linkTypeHandler(area_hypnotize, &area_hypnotize_handler);
    registrationAPI.linkSingleTargetAIHandler(area_hypnotize, &hypnotize_ai_handler);
    registrationAPI.applySpellTag(area_hypnotize, SpellTag::AOE_SPELL);

    SFSpell *area_confuse = registrationAPI.registerSpell(kGdSpellLineConfuseArea);
    registrationAPI.linkTypeHandler(area_confuse, &area_confuse_handler);
    registrationAPI.applySpellTag(area_confuse, SpellTag::AOE_SPELL);

    SFSpell *chain11 = registrationAPI.registerSpell(kGdSpellLineChainManatap);
    registrationAPI.linkTypeHandler(chain11, &chain_handler);

    SFSpell *manashield = registrationAPI.registerSpell(kGdSpellLineManaShield);
    registrationAPI.linkTypeHandler(manashield, &manashield_handler);

    SFSpell *shift_mana = registrationAPI.registerSpell(kGdSpellLineShiftMana);
    registrationAPI.linkTypeHandler(shift_mana, &shift_mana_handler);

    SFSpell *ability_shift_life = registrationAPI.registerSpell(kGdSpellLineAbilityShiftLife);
    registrationAPI.linkTypeHandler(ability_shift_life, &ability_shift_life_handler);
    registrationAPI.linkSingleTargetAIHandler(ability_shift_life, &shift_life_ai_handler);
    registrationAPI.applySpellTag(ability_shift_life, SpellTag::COMBAT_ABILITY_SPELL);

    SFSpell *ability_riposte = registrationAPI.registerSpell(kGdSpellLineAbilityRiposte);
    registrationAPI.applySpellTag(ability_riposte, SpellTag::TARGET_ONHIT_SPELL);
    registrationAPI.linkTypeHandler(ability_riposte, &ability_riposte_handler);
    registrationAPI.linkOnHitHandler(ability_riposte, &riposte_onhit_handler, PHASE_3);
    registrationAPI.linkSingleTargetAIHandler(ability_riposte, &riposte_ability_ai_handler);
    registrationAPI.applySpellTag(ability_riposte, SpellTag::COMBAT_ABILITY_SPELL);

    SFSpell *ability_critical_hits = registrationAPI.registerSpell(kGdSpellLineAbilityCriticalHits);
    registrationAPI.linkTypeHandler(ability_critical_hits, &ability_critical_hits_handler);
    registrationAPI.linkOnHitHandler(ability_critical_hits, &critical_hits_onhit_handler, PHASE_1);
    registrationAPI.linkSingleTargetAIHandler(ability_riposte, &critical_hits_ai_handler);
    registrationAPI.applySpellTag(ability_riposte, SpellTag::COMBAT_ABILITY_SPELL);

    SFSpell *aura21 = registrationAPI.registerSpell(kGdSpellLineAuraSiegeHuman);
    registrationAPI.linkTypeHandler(aura21, &aura_handler);
    registrationAPI.applySpellTag(aura21, SpellTag::SEIGE_AURA_SPELL);

    // Next Spell Block

    // Unused Spell Here

    SFSpell *aura22 = registrationAPI.registerSpell(kGdSpellLineAuraSiegeElf); // TODO Find Spell Name
    registrationAPI.linkTypeHandler(aura22, &aura_handler);
    registrationAPI.applySpellTag(aura22, SpellTag::SEIGE_AURA_SPELL);

    SFSpell *aura23 = registrationAPI.registerSpell(kGdSpellLineAuraSiegeOrc); // TODO Find Spell Name
    registrationAPI.linkTypeHandler(aura23, &aura_handler);
    registrationAPI.applySpellTag(aura23, SpellTag::SEIGE_AURA_SPELL);

    SFSpell *aura24 = registrationAPI.registerSpell(kGdSpellLineAuraSiegeTroll); // TODO Find Spell Name
    registrationAPI.linkTypeHandler(aura24, &aura_handler);
    registrationAPI.applySpellTag(aura24, SpellTag::SEIGE_AURA_SPELL);

    SFSpell *aura25 = registrationAPI.registerSpell(kGdSpellLineAuraSiegeDarkElf); // TODO Find Spell Name
    registrationAPI.linkTypeHandler(aura25, &aura_handler);
    registrationAPI.applySpellTag(aura25, SpellTag::SEIGE_AURA_SPELL);

    SFSpell *eternity_aura = registrationAPI.registerSpell(kGdSpellLineEternity);
    registrationAPI.linkTypeHandler(eternity_aura, &eternity_aura_handler);

    SFSpell *hallow2 = registrationAPI.registerSpell(kGdSpellLineHallowChained);
    registrationAPI.linkTypeHandler(hallow2, &hallow_handler);

    SFSpell *lifetap2 = registrationAPI.registerSpell(kGdSpellLineLifeTapChained);
    registrationAPI.linkTypeHandler(lifetap2, &lifetap_handler);

    SFSpell *manatap2 = registrationAPI.registerSpell(kGdSpellLineManaTapChained);
    registrationAPI.linkTypeHandler(manatap2, &manatap_handler);

    SFSpell *mutation2 = registrationAPI.registerSpell(kGdSpellLineMutationChained);
    registrationAPI.linkTypeHandler(mutation2, &mutation_handler);

    SFSpell *fireburst3 = registrationAPI.registerSpell(kGdSpellLineFireBurstChained);
    registrationAPI.linkTypeHandler(fireburst3, &fireburst_handler);

    SFSpell *icestrike2 = registrationAPI.registerSpell(kGdSpellLineIceBurstChained);
    registrationAPI.linkTypeHandler(icestrike2, &icestrike_handler);

    SFSpell *rock_bullet2 = registrationAPI.registerSpell(kGdSpellLineRockBulletChained);
    registrationAPI.linkTypeHandler(rock_bullet2, &rock_bullet_handler);

    SFSpell *charm2 = registrationAPI.registerSpell(kGdSpellLineCharmChained);
    registrationAPI.linkTypeHandler(charm2, &charm_handler);
    registrationAPI.applySpellTag(charm2, SpellTag::DOMINATION_SPELL);

    SFSpell *shock2 = registrationAPI.registerSpell(kGdSpellLineShockChained);
    registrationAPI.linkTypeHandler(shock2, &shock_handler);

    SFSpell *fireball3 = registrationAPI.registerSpell(kGdSpellLineFireBallChained);
    registrationAPI.linkTypeHandler(fireball3, &fireball_handler);

    // Next Spell Block

    SFSpell *pain2 = registrationAPI.registerSpell(kGdSpellLinePainChained);
    registrationAPI.linkTypeHandler(pain2, &pain_handler);

    SFSpell *belial_effect = registrationAPI.registerSpell(kGdSpellLineFakeSpellOneFigure);
    registrationAPI.linkTypeHandler(belial_effect, &belial_effect_handler);
    registrationAPI.linkSingleTargetAIHandler(belial_effect, &do_not_cast_ai_handler);
}