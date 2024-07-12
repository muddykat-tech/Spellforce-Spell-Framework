#include "sf_vanilla_registry.h"
#include "../core/sf_hooks.h"

void initialize_vanilla_spells()
{
    SFSpell *fireburst = registrationAPI->registerSpell(0x01);
    registrationAPI->linkTypeHandler(fireburst, &fireburst_handler);

    SFSpell *healing = registrationAPI->registerSpell(0x02);
    registrationAPI->linkTypeHandler(healing, &healing_handler);

    SFSpell *death = registrationAPI->registerSpell(0x03);
    registrationAPI->linkTypeHandler(death, &death_handler);

    SFSpell *slowness = registrationAPI->registerSpell(0x04);
    registrationAPI->linkTypeHandler(slowness, &slowness_handler);

    SFSpell *poison = registrationAPI->registerSpell(0x05);
    registrationAPI->linkTypeHandler(poison, &poison_handler);

    SFSpell *invulnerability = registrationAPI->registerSpell(0x06);
    registrationAPI->linkTypeHandler(invulnerability, &invulnerability_handler);

    SFSpell *cure_poison = registrationAPI->registerSpell(0x07);
    registrationAPI->linkTypeHandler(cure_poison, &cure_poison_handler);

    // Empty Spell Type here

    SFSpell *freeze = registrationAPI->registerSpell(0x09);
    registrationAPI->linkTypeHandler(freeze, &freeze_handler);

    SFSpell *fog = registrationAPI->registerSpell(0x0a);
    registrationAPI->linkTypeHandler(fog, &fog_handler);

    SFSpell *illuminate = registrationAPI->registerSpell(0x0b);
    registrationAPI->linkTypeHandler(illuminate, &illuminate_handler);

    SFSpell *fireshield = registrationAPI->registerSpell(0x0c);
    registrationAPI->linkTypeHandler(fireshield, &fireshield_handler);

    SFSpell *fireball = registrationAPI->registerSpell(0x0d);
    registrationAPI->linkTypeHandler(fireshield, &fireball_handler);

    SFSpell *icestrike = registrationAPI->registerSpell(0x0e);
    registrationAPI->linkTypeHandler(icestrike, &icestrike_handler);

    SFSpell *iceshield = registrationAPI->registerSpell(0x0f);
    registrationAPI->linkTypeHandler(iceshield, &iceshield_handler);

    // next block

    SFSpell *decay_1 = registrationAPI->registerSpell(0x10);
    registrationAPI->linkTypeHandler(decay_1, &decay1_handler);

    SFSpell *decay_2 = registrationAPI->registerSpell(0x11);
    registrationAPI->linkTypeHandler(decay_2, &decay2_handler);

    SFSpell *pain = registrationAPI->registerSpell(0x12);
    registrationAPI->linkTypeHandler(pain, &pain_handler);

    SFSpell *lifetap = registrationAPI->registerSpell(0x13);
    registrationAPI->linkTypeHandler(lifetap, &lifetap_handler);

    SFSpell *summon_1 = registrationAPI->registerSpell(0x14); // TODO Find spell name
    registrationAPI->linkTypeHandler(summon_1, &summons_handler);

    SFSpell *hypnotize = registrationAPI->registerSpell(0x15);
    registrationAPI->linkTypeHandler(hypnotize, &hypnotize_handler);

    SFSpell *iceshield2 = registrationAPI->registerSpell(0x16);
    registrationAPI->linkTypeHandler(iceshield2, &iceshield2_handler);

    SFSpell *pestilence = registrationAPI->registerSpell(0x17);
    registrationAPI->linkTypeHandler(pestilence, &pestilence_handler);

    SFSpell *cure_disease = registrationAPI->registerSpell(0x18);
    registrationAPI->linkTypeHandler(cure_disease, &cure_disease_handler);

    SFSpell *petrify = registrationAPI->registerSpell(0x19);
    registrationAPI->linkTypeHandler(petrify, &petrify_handler);

    // Two unused spells here

    SFSpell *area_pain = registrationAPI->registerSpell(0x1c);
    registrationAPI->linkTypeHandler(area_pain, &area_pain_handler);

    SFSpell *summon_2 = registrationAPI->registerSpell(0x1d); // TODO Find spell name
    registrationAPI->linkTypeHandler(summon_2, &summons_handler);

    SFSpell *raise_dead = registrationAPI->registerSpell(0x1e);
    registrationAPI->linkTypeHandler(raise_dead, &raise_dead_handler);

    SFSpell *summon_3 = registrationAPI->registerSpell(0x1f); // TODO Find spell name
    registrationAPI->linkTypeHandler(summon_3, &summons_handler);

    SFSpell *death_grasp = registrationAPI->registerSpell(0x20);
    registrationAPI->linkTypeHandler(death_grasp, &death_grasp_handler);

    SFSpell *summon_4 = registrationAPI->registerSpell(0x21); // TODO Find spell name
    registrationAPI->linkTypeHandler(summon_4, &summons_handler);

    SFSpell *inflexibility = registrationAPI->registerSpell(0x22);
    registrationAPI->linkTypeHandler(inflexibility, &inflexibility_handler);

    SFSpell *weaken = registrationAPI->registerSpell(0x23);
    registrationAPI->linkTypeHandler(weaken, &weaken_handler);

    SFSpell *dark_banishing = registrationAPI->registerSpell(0x24);
    registrationAPI->linkTypeHandler(dark_banishing, &dark_banishing_handler);

    SFSpell *area_slowness = registrationAPI->registerSpell(0x25);
    registrationAPI->linkTypeHandler(area_slowness, &area_slowness_handler);

    SFSpell *area_inflexibility = registrationAPI->registerSpell(0x26);
    registrationAPI->linkTypeHandler(area_inflexibility, &area_inflexibility_handler);

    SFSpell *area_weaken = registrationAPI->registerSpell(0x27);
    registrationAPI->linkTypeHandler(area_weaken, &area_weaken_handler);

    SFSpell *area_plauge = registrationAPI->registerSpell(0x28);
    registrationAPI->linkTypeHandler(area_plague, &area_plague_handler);

    SFSpell *remediless = registrationAPI->registerSpell(0x29);
    registrationAPI->linkTypeHandler(remediless, &remediless_handler);

    // Unused Spell here

    SFSpell *area_healing = registrationAPI->registerSpell(0x2b);
    registrationAPI->linkTypeHandler(area_healing, &area_healing_handler);

    SFSpell *sentine1_healing = registrationAPI->registerSpell(0x2c);
    registrationAPI->linkTypeHandler(sentine1_healing, &sentine1_healing_handler);

    SFSpell *healing2 = registrationAPI->registerSpell(0x2d);
    registrationAPI->linkTypeHandler(healing2, &healing_handler);

    SFSpell *charm_animal = registrationAPI->registerSpell(0x2e);
    registrationAPI->linkTypeHandler(charm_animal, &charm_animal_handler);

    SFSpell *thorn_shield = registrationAPI->registerSpell(0x2f);
    registrationAPI->linkTypeHandler(thorn_shield, &thorn_shield_handler);

    SFSpell *quickness = registrationAPI->registerSpell(0x30);
    registrationAPI->linkTypeHandler(quickness, &quickness_handler);

    SFSpell *area_quickness = registrationAPI->registerSpell(0x31);
    registrationAPI->linkTypeHandler(area_quickness, &area_quickness_handler);

    SFSpell *flexibility = registrationAPI->registerSpell(0x32);
    registrationAPI->linkTypeHandler(flexibility, &flexibility_handler);

    SFSpell *area_flexibility = registrationAPI->registerSpell(0x33);
    registrationAPI->linkTypeHandler(area_flexibility, &area_flexibility_handler);

    SFSpell *strength = registrationAPI->registerSpell(0x34);
    registrationAPI->linkTypeHandler(strength, &strength_handler);

    SFSpell *area_strength = registrationAPI->registerSpell(0x35);
    registrationAPI->linkTypeHandler(area_strength, &area_strength_handler);

    SFSpell *guard = registrationAPI->registerSpell(0x36);
    registrationAPI->linkTypeHandler(guard, &guard_handler);

    SFSpell *remove_curse = registrationAPI->registerSpell(0x37);
    registrationAPI->linkTypeHandler(remove_curse, &remove_curse_handler);

    SFSpell *regenerate = registrationAPI->registerSpell(0x38);
    registrationAPI->linkTypeHandler(regenerate, &regenerate_handler);

    SFSpell *holy_might = registrationAPI->registerSpell(0x39);
    registrationAPI->linkTypeHandler(holy_might, &default_handler);

    SFSpell *hallow = registrationAPI->registerSpell(0x3a);
    registrationAPI->linkTypeHandler(hallow, &hallow_handler);

    // Unused spell here

    SFSpell *fireshield2 = registrationAPI->registerSpell(0x3c);
    registrationAPI->linkTypeHandler(fireshield2, &fireshield_handler);

    SFSpell *thorn_shield2 = registrationAPI->registerSpell(0x3d);
    registrationAPI->linkTypeHandler(thorn_shield2, &thorn_shield_handler);

    SFSpell *forget = registrationAPI->registerSpell(0x3e);
    registrationAPI->linkTypeHandler(forget, &forget_handler);

    SFSpell *self_illusion = registrationAPI->registerSpell(0x3f);
    registrationAPI->linkTypeHandler(self_illusion, &self_illusion_handler);

    // Next Block

    SFSpell *retention = registrationAPI->registerSpell(0x40);
    registrationAPI->linkTypeHandler(retention, &retention_handler);

    SFSpell *brilliance = registrationAPI->registerSpell(0x41);
    registrationAPI->linkTypeHandler(brilliance, &brilliance_handler);

    SFSpell *sacrifice_mana = registrationAPI->registerSpell(0x42);
    registrationAPI->linkTypeHandler(sacrifice_mana, &sacrifice_mana_handler);

    SFSpell *manatap = registrationAPI->registerSpell(0x43);
    registrationAPI->linkTypeHandler(manatap, &manatap_handler);

    SFSpell *mana_drain = registrationAPI->registerSpell(0x44);
    registrationAPI->linkTypeHandler(mana_drain, &mana_drain_handler);

    SFSpell *shock = registrationAPI->registerSpell(0x45);
    registrationAPI->linkTypeHandler(shock, &shock_handler);

    SFSpell *distrupt = registrationAPI->registerSpell(0x46);
    registrationAPI->linkTypeHandler(distrupt, &distrupt_handler);

    // Unused Spell Here

    SFSpell *confuse = registrationAPI->registerSpell(0x48);
    registrationAPI->linkTypeHandler(confuse, &confuse_handler);

    SFSpell *rain_of_fire = registrationAPI->registerSpell(0x49);
    SFSpell *spell = registrationAPI->linkTypeHandler(rain_of_fire, &rain_of_fire_handler);

    SFSpell *blizzard = registrationAPI->registerSpell(0x4a);
    registrationAPI->linkTypeHandler(blizzard, &blizzard_handler);

    SFSpell *acid_cloud = registrationAPI->registerSpell(0x4b);
    registrationAPI->linkTypeHandler(acid_cloud, &acid_cloud_handler);

    SFSpell *stone_rain = registrationAPI->registerSpell(0x4c);
    registrationAPI->linkTypeHandler(stone_rain, &stone_rain_handler);

    // Unused Spell Here
    // Unused Spell Here

    SFSpell *amok = registrationAPI->registerSpell(0x4f);
    registrationAPI->linkTypeHandler(amok, &amok_handler);

    // Next Block

    // Unused Spell Here

    SFSpell *extinct = registrationAPI->registerSpell(0x51);
    registrationAPI->linkTypeHandler(extinct, &extinct_handler);
}