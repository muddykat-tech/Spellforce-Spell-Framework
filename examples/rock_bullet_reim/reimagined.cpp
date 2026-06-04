#include "../../src/api/sfsf.h"


// NOTE sfsf.h includes the OTHER api files, but other libraries are still required
#include <windows.h>
#include <stdio.h>


SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;
EffectFunctions *effectAPI;
RegistrationFunctions *registrationAPI;
SFLog *logAPI;

//TODO: export this function somewhere?
uint32_t getDistance(SF_Coord *pointA, SF_Coord *pointB)
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

bool isSiegeUnit (SF_CGdFigure *_this, uint16_t figure_index)
{
    if ((_this->figures[figure_index].unit_data_id == 2236) ||
        (_this->figures[figure_index].unit_data_id == 2238) ||
        (_this->figures[figure_index].unit_data_id == 2239) ||
        (_this->figures[figure_index].unit_data_id == 2244) ||
        (_this->figures[figure_index].unit_data_id == 2245) ||
        (_this->figures[figure_index].unit_data_id == 2249) )
    {
        return true;
    }
    return false;
}
// Rock Bullet Effect Rework
void __thiscall rock_bullet_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    SF_CGdResourceSpell spell_data;
    SF_SpellEffectInfo effect_info;

    effect_info.spell_id = spell->spell_id;
    effect_info.job_id = spell->spell_job;
    spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);


    uint16_t target_index = spell->target.entity_index;
    uint16_t source_index = spell->source.entity_index;

    //target is figure, target index has been passed through
    //figure is owned by any clan present on the map
    //figure is targetable
    //figure is not protected by special flags
    if ((spell->target.entity_type == 1) && (target_index != 0) &&
        (_this->SF_CGdFigure->figures[target_index].owner != (uint16_t)(-1)) &&
        (toolboxAPI->isTargetable(_this->SF_CGdFigureToolBox, target_index)) &&
        ((*(uint8_t *)(&_this->SF_CGdFigure->figures[target_index].flags) & 0xa) == 0))
    {
        uint16_t damage = spell_data.params[0];
        SF_CGdTargetData source_data;
        SF_CGdTargetData target_data;

        target_data.entity_index = target_index;
        target_data.entity_type = 1;
        target_data.position.X = spell->target.position.X;
        target_data.position.Y = spell->target.position.Y;

        source_data.entity_index = source_index;
        source_data.entity_type = 1;
        source_data.position.X = spell->source.position.X;
        source_data.position.X = spell->source.position.Y;

        //Wave form handling
        uint16_t prev_figure_index = spellAPI->getXData(_this, spell_index, EFFECT_ENTITY_INDEX3);
        if ((prev_figure_index == 0) || (!figureAPI->isAlive(_this->SF_CGdFigure, prev_figure_index)))
        {
            prev_figure_index = source_index;
        }

        uint16_t resist_chance = spellAPI->getChanceToResistSpell(_this->AutoClass34, source_index, target_index,
                                                                  effect_info);

        uint16_t random = spellAPI->getRandom(_this->OpaqueClass, 100);

        if (resist_chance < random)
        {
            uint16_t distance = getDistance(&_this->SF_CGdFigure->figures[source_index].position,
                                            &_this->SF_CGdFigure->figures[target_index].position);

            distance = (distance * 1400) / 5000;
            if (distance == 0)
            {
                distance = 1; //we need it for non-zero flight time
            }
            if (!toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, kGdSpellLineDecay))
            {
                uint16_t reduction_percent = toolboxAPI->getPhysDamageReduction(_this->SF_CGdFigureToolBox,
                                                                                source_index,
                                                                                target_index, kGdSpellLineRockBullet);
                damage = ((uint32_t)(damage * reduction_percent + 5000)) / 10000;
            }
            //if caster is alive
            if (figureAPI->isAlive(_this->SF_CGdFigure, source_index))
            {
                //double damage magic set handling
                if ((_this->SF_CGdFigure->figures[source_index].set_type == 8) &&
                    (spellAPI->getRandom(_this->OpaqueClass, 100) < 26))
                {
                    damage *= 2;
                }
            }
            if (isSiegeUnit(_this->SF_CGdFigure, target_index))
            {
                damage = ((uint16_t)(damage * 100) + 50) / 1000;
            }

            if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, kGdSpellLinePetrify))
            {
                damage *= 3;
            }

            SF_Rectangle rect = {0, 0};
            target_data.entity_index = target_index;
            target_data.entity_type = 1;
            target_data.position.X = spell->target.position.X;
            target_data.position.Y = spell->target.position.Y;
            uint16_t effect_index = effectAPI->addEffect(_this->SF_CGdEffect, kGdEffectSpellHitTarget, &source_data,
                                                         &target_data, _this->OpaqueClass->current_step, distance,
                                                         &rect);
            effectAPI->setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_SPELL_INDEX, spell_index);
            effectAPI->setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_SPELL_ID,
                                      _this->active_spell_list[spell_index].spell_id);

            effectAPI->setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_PHYSICAL_DAMAGE, damage);
            effectAPI->setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_ADD_SUBSPELL, 1);
            effectAPI->setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_ENTITY_INDEX, source_index);
            effectAPI->setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_ENTITY_TYPE, 1);
            effectAPI->setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_ENTITY_INDEX2, target_index);
            effectAPI->setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_ENTITY_TYPE2, 1);
            effectAPI->setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_ENTITY_INDEX3, prev_figure_index);
        }
        else
        {
            spellAPI->figureAggro(_this, spell_index, target_index);
            SF_Rectangle rect = {0,0};

            spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellTargetResisted, &source_data, &target_data,
                                      _this->OpaqueClass->current_step, 40, &rect);
        }
    }
    spellAPI->setEffectDone(_this, spell_index, 0);
}
// Thorns Effect Rework
void __thiscall thorns_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    SF_CGdResourceSpell spell_data;
//get caster figure index
    uint16_t source_index = spell->source.entity_index;
//get target figure index
    uint16_t target_index = spell->target.entity_index;

    if ((source_index != 0) && (target_index != 0))
    {
//mandatory check for target not being reserved or dead
        if ((_this->SF_CGdFigure->figures[target_index].owner != (uint16_t)(-1)) &&
            ((*(uint8_t *)(&_this->SF_CGdFigure->figures[target_index].flags) & 0xa) == 0))
        {
//get info from gamedata.cff
            spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);
            uint16_t base_damage = spell_data.params[0];
//armor reduction SCRAPPED!!!!
            uint32_t damage = base_damage;
//		uint32_t damage = toolboxAPI->getPhysDamageReduction(_this->SF_CGdFigureToolBox, source_index, target_index, kGdSpellLineThornShieldDamage);
            //if caster is alive
            if (figureAPI->isAlive(_this->SF_CGdFigure, source_index))
            {
                //double damage magic set handling
                if ((_this->SF_CGdFigure->figures[source_index].set_type == 8) &&
                    (spellAPI->getRandom(_this->OpaqueClass, 100) < 26))
                {
                    damage *= 2;
                }
            }
            if (isSiegeUnit(_this->SF_CGdFigure, target_index))
            {
                damage = ((uint16_t)(damage * 100) + 50) / 1000;
            }
            if ((toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, kGdSpellLineRoots)) ||
                (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, kGdSpellLineRootsArea)))
            {
                damage *= 2;
            }
//multiply by base spell damage and reduce by 10000, so we get sane numbers, scrapped, since no armour factor
//            damage = (damage * base_damage + 5000) / 10000;
//setup target_data structure for later use
            SF_CGdTargetData target_data;
            target_data.entity_index = target_index;
            target_data.entity_type = 1;
            target_data.position = {0,0};
            SF_Rectangle rect = {0,0};
            if (damage == 0)
            {
//show "resisted sparks" FX if damage is 0
                spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellTargetResisted, &rect, &target_data,
                                          _this->OpaqueClass->current_step, 10, &rect);
            }
            else
            {
//show thorns FX if damage is not 0
                spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitTarget, &rect, &target_data,
                                          _this->OpaqueClass->current_step, 10, &rect);
//dealing damage. last 3 numbers are flags: isSpellDamage, isRangedDamage, is(I can't remember, but is is 0 here)
                toolboxAPI->dealDamage(_this->SF_CGdFigureToolBox, source_index, target_index, damage, 1, 0, 0);
            }
        }

    }
//mark the spell as complete
    spellAPI->setEffectDone(_this, spell_index, 0);
}

/***
 * Applying different proc chance of different type of enchants
 * Tier 1: Basic spells. Have about 30% chance (3000), will scale up fast with attributes (on average 12-14 points per attribute)
 * Tier 2: Advanced spells. Have about 20% chance (2000), will scale up moderately with attributes (on average 9-12 points per attribute)
 * Tier 3: Elite spells. Have about 10% chance (1500), will scale up slow with attributes (on average 6-9 points per attribute)
 * Tier Special: Unique effects that will have special trigger conditions
 ***/

/* Rock Bullet                  Tier 1 */
uint16_t __thiscall rock_bullet_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t str_current = figureAPI->getCurrentStat(_this, figure_id, STRENGTH);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return str_current * 8 + int_current * 5 + 3000;
}
/* Decay                                Tier 1 */
uint16_t __thiscall decay_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t str_current = figureAPI->getCurrentStat(_this, figure_id, STRENGTH);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return str_current * 4 + int_current * 10 + 3000;
}
/* Petrify	                        Tier 2 */
uint16_t __thiscall petrify_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t str_current = figureAPI->getCurrentStat(_this, figure_id, STRENGTH);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return str_current * 4 + int_current * 8 + 2000;
}
/* Chain Rock Bullet    Tier 3 */
uint16_t __thiscall chain_rock_bullet_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t str_current = figureAPI->getCurrentStat(_this, figure_id, STRENGTH);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return str_current * 2 + int_current * 7 + 1500;
}
/* Fireburst                    Tier 1 */
uint16_t __thiscall fire_burst_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    uint16_t wis_current = figureAPI->getCurrentStat(_this, figure_id, WISDOM);
    return int_current * 7 + wis_current * 7 + 3000;
}
/* Fireball				Tier 2 */
uint16_t __thiscall fire_ball_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    uint16_t wis_current = figureAPI->getCurrentStat(_this, figure_id, WISDOM);
    return int_current * 5 + wis_current * 5 + 2000;
}
/* Melt Resistance		Tier SPECIAL */
uint16_t __thiscall melt_resistance_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    return 5000;
}
/* Chain Fireburst		Tier 2 */
uint16_t __thiscall chain_fire_burst_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    uint16_t wis_current = figureAPI->getCurrentStat(_this, figure_id, WISDOM);
    return int_current * 5 + wis_current * 5 + 2000;
}
/* Chain Fireball		Tier 3 */
uint16_t __thiscall chain_fire_ball_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    uint16_t wis_current = figureAPI->getCurrentStat(_this, figure_id, WISDOM);
    return int_current * 4 + wis_current * 4 + 1500;
}
/* Iceburst                     Tier 1 */
uint16_t __thiscall ice_burst_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    uint16_t wis_current = figureAPI->getCurrentStat(_this, figure_id, WISDOM);
    return int_current * 8 + wis_current * 6 + 3000;
}
/* Freeze                               Tier 1 */
uint16_t __thiscall freeze_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    uint16_t wis_current = figureAPI->getCurrentStat(_this, figure_id, WISDOM);
    return int_current * 8 + wis_current * 6 + 3000;
}
/* Chill Resistance		Tier SPECIAL */
uint16_t __thiscall chill_resistance_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    return 5000;
}
/* Chain Iceburst		Tier 2 */
uint16_t __thiscall chain_ice_burst_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    uint16_t wis_current = figureAPI->getCurrentStat(_this, figure_id, WISDOM);
    return int_current * 6 + wis_current * 5 + 2000;
}
/* Elemental Essence	Tier 2 */
uint16_t __thiscall ele_essence_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    uint16_t wis_current = figureAPI->getCurrentStat(_this, figure_id, WISDOM);
    return int_current * 6 + wis_current * 5 + 2000;
}
/* Elemental Almight	Tier 3 */
uint16_t __thiscall ele_almight_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    uint16_t wis_current = figureAPI->getCurrentStat(_this, figure_id, WISDOM);
    return int_current * 5 + wis_current * 4 + 1500;
}
/* Shared Destiny               Tier 2 */
uint16_t __thiscall shared_destiny_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t cha_current = figureAPI->getCurrentStat(_this, figure_id, CHARISMA);
    uint16_t wis_current = figureAPI->getCurrentStat(_this, figure_id, WISDOM);
    return cha_current * 6 + wis_current * 6 + 2000;
}
/* Thorns                               Tier 1 */
uint16_t __thiscall thorns_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t dex_current = figureAPI->getCurrentStat(_this, figure_id, DEXTERITY);
    uint16_t str_current = figureAPI->getCurrentStat(_this, figure_id, STRENGTH);
    return dex_current * 7 + str_current * 6 + 3000;
}
/* Dominate Animal              Tier 2 */
uint16_t __thiscall dominate_animal_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t cha_current = figureAPI->getCurrentStat(_this, figure_id, CHARISMA);
    uint16_t wis_current = figureAPI->getCurrentStat(_this, figure_id, WISDOM);
    return cha_current * 8 + wis_current * 5 + 2000;
}
/* Charm Animal                 Tier 2 */
uint16_t __thiscall charm_animal_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t cha_current = figureAPI->getCurrentStat(_this, figure_id, CHARISMA);
    uint16_t wis_current = figureAPI->getCurrentStat(_this, figure_id, WISDOM);
    return cha_current * 8 + wis_current * 5 + 2000;
}
/* Roots                                Tier 1 */
uint16_t __thiscall roots_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t cha_current = figureAPI->getCurrentStat(_this, figure_id, CHARISMA);
    uint16_t wis_current = figureAPI->getCurrentStat(_this, figure_id, WISDOM);
    return cha_current * 7 + wis_current * 6 + 3000;
}
/* Hallow                               Tier 1 */
uint16_t __thiscall hallow_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t cha_current = figureAPI->getCurrentStat(_this, figure_id, CHARISMA);
    uint16_t wis_current = figureAPI->getCurrentStat(_this, figure_id, WISDOM);
    return cha_current * 7 + wis_current * 6 + 3000;
}
/* Dispel Black Aura    Tier SPECIAL */
uint16_t __thiscall dispel_black_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    return 5000;
}
/* Chain Hallow                 Tier 3 */
uint16_t __thiscall chain_hallow_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t cha_current = figureAPI->getCurrentStat(_this, figure_id, CHARISMA);
    uint16_t wis_current = figureAPI->getCurrentStat(_this, figure_id, WISDOM);
    return cha_current * 5 + wis_current * 3 + 1500;
}
/* Shock                                Tier 1 */
uint16_t __thiscall shock_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t cha_current = figureAPI->getCurrentStat(_this, figure_id, CHARISMA);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return cha_current * 7 + int_current * 6 + 3000;
}
/* Confuse                              Tier 1 */
uint16_t __thiscall confusion_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t cha_current = figureAPI->getCurrentStat(_this, figure_id, CHARISMA);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return cha_current * 7 + int_current * 6 + 3000;
}
/* Hypnotization                Tier 2 */
uint16_t __thiscall hypnotize_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t cha_current = figureAPI->getCurrentStat(_this, figure_id, CHARISMA);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return cha_current * 6 + int_current * 5 + 2000;
}
/* Amok                                 Tier 2 */
uint16_t __thiscall amok_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t cha_current = figureAPI->getCurrentStat(_this, figure_id, CHARISMA);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return cha_current * 6 + int_current * 5 + 2000;
}
/* Disrupt                              Tier SPECIAL */
uint16_t __thiscall disrupt_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    return 7500;
}
/* Demoralization               Tier 2 */
uint16_t __thiscall demoralization_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t cha_current = figureAPI->getCurrentStat(_this, figure_id, CHARISMA);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return cha_current * 6 + int_current * 5 + 2000;
}
/* Chain Shock                  Tier 3 */
uint16_t __thiscall chain_shock_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t cha_current = figureAPI->getCurrentStat(_this, figure_id, CHARISMA);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return cha_current * 5 + int_current * 4 + 1500;
}
/* Manadrain                    Tier 1 */
uint16_t __thiscall mana_drain_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t cha_current = figureAPI->getCurrentStat(_this, figure_id, CHARISMA);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return cha_current * 7 + int_current * 6 + 3000;
}
/* Manatap                              Tier 2 */
uint16_t __thiscall mana_tap_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t cha_current = figureAPI->getCurrentStat(_this, figure_id, CHARISMA);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return cha_current * 6 + int_current * 5 + 2000;
}
/* Chain Manatap                Tier 3 */
uint16_t __thiscall chain_mana_tap_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t cha_current = figureAPI->getCurrentStat(_this, figure_id, CHARISMA);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return cha_current * 5 + int_current * 4 + 1500;
}
/* Dominate                     Tier 2 */
uint16_t __thiscall dominate_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t cha_current = figureAPI->getCurrentStat(_this, figure_id, CHARISMA);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return cha_current * 6 + int_current * 5 + 2000;
}
/* Charm                                Tier 2 */
uint16_t __thiscall charm_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t cha_current = figureAPI->getCurrentStat(_this, figure_id, CHARISMA);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return cha_current * 6 + int_current * 5 + 2000;
}
/* Befriend                     Tier 1 */
uint16_t __thiscall befriend_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t cha_current = figureAPI->getCurrentStat(_this, figure_id, CHARISMA);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return cha_current * 7 + int_current * 6 + 3000;
}
/* Disenchant                   Tier SPECIAL */
uint16_t __thiscall disenchant_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    return 6666;
}
/* Chain Charm                  Tier 3 */
uint16_t __thiscall chain_charm_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t cha_current = figureAPI->getCurrentStat(_this, figure_id, CHARISMA);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return cha_current * 5 + int_current * 4 + 1500;
}
/* Mental Essence               Tier 2 */
uint16_t __thiscall mental_essence_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t cha_current = figureAPI->getCurrentStat(_this, figure_id, CHARISMA);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return cha_current * 6 + int_current * 5 + 2000;
}
/* Mental Almightiness  Tier 3 */
uint16_t __thiscall mental_almight_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t cha_current = figureAPI->getCurrentStat(_this, figure_id, CHARISMA);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return cha_current * 5 + int_current * 4 + 1500;
}
/* Pain                                 Tier 1 */
uint16_t __thiscall pain_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t wis_current = figureAPI->getCurrentStat(_this, figure_id, WISDOM);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return wis_current * 6 + int_current * 7 + 3000;
}
/* Weaken                               Tier SPECIAL */
uint16_t __thiscall weaken_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    return 2500;
}
/* Death                                Tier 2 */
uint16_t __thiscall death_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t wis_current = figureAPI->getCurrentStat(_this, figure_id, WISDOM);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return wis_current * 4 + int_current * 6 + 2000;
}
/* Pestilence                   Tier 2 */
uint16_t __thiscall pestilence_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t wis_current = figureAPI->getCurrentStat(_this, figure_id, WISDOM);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return wis_current * 4 + int_current * 6 + 2000;
}
/* Suffocation                  Tier SPECIAL */
uint16_t __thiscall suffocate_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    return 2500;
}
/* Suicide Death                Tier 2 */
uint16_t __thiscall suicide_death_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t wis_current = figureAPI->getCurrentStat(_this, figure_id, WISDOM);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return wis_current * 4 + int_current * 6 + 2000;
}
/* Chain Pain                   Tier 3 */
uint16_t __thiscall chain_pain_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t wis_current = figureAPI->getCurrentStat(_this, figure_id, WISDOM);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return wis_current * 3 + int_current * 5 + 1500;
}
/* Lifetap                              Tier 1 */
uint16_t __thiscall life_tap_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t wis_current = figureAPI->getCurrentStat(_this, figure_id, WISDOM);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return wis_current * 6 + int_current * 7 + 3000;
}
/* Dominate Undead              Tier 2 */
uint16_t __thiscall dominate_undead_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t wis_current = figureAPI->getCurrentStat(_this, figure_id, WISDOM);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return wis_current * 4 + int_current * 6 + 2000;
}
/* Chain Lifetap                Tier 3 */
uint16_t __thiscall chain_life_tap_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t wis_current = figureAPI->getCurrentStat(_this, figure_id, WISDOM);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return wis_current * 3 + int_current * 5 + 1500;
}
/* Slow Fighting                Tier SPECIAL */
uint16_t __thiscall slow_fight_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    return 2500;
}
/* Poison                               Tier 1 */
uint16_t __thiscall poison_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t wis_current = figureAPI->getCurrentStat(_this, figure_id, WISDOM);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return wis_current * 6 + int_current * 7 + 3000;
}
/* Inflexibility                Tier SPECIAL */
uint16_t __thiscall inflexibility_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    return 2500;
}
/* Dispel White Aura    Tier SPECIAL */
uint16_t __thiscall dispel_white_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    return 5000;
}
/* Slow Walking                 Tier SPECIAL */
uint16_t __thiscall slow_walk_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    return 2500;
}
/* Dark Banishing               Tier SPECIAL */
uint16_t __thiscall dark_banishing_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    return 5000;
}
/* Inability                    Tier SPECIAL */
uint16_t __thiscall inability_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    return 2500;
}
/* Remediless                   Tier 1 */
uint16_t __thiscall remediless_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t wis_current = figureAPI->getCurrentStat(_this, figure_id, WISDOM);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return wis_current * 6 + int_current * 7 + 3000;
}
/* Mutation                     Tier 1 */
uint16_t __thiscall mutation_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t wis_current = figureAPI->getCurrentStat(_this, figure_id, WISDOM);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return wis_current * 6 + int_current * 7 + 3000;
}
/* Chain Mutation               Tier 2 */
uint16_t __thiscall chain_mutation_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t wis_current = figureAPI->getCurrentStat(_this, figure_id, WISDOM);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return wis_current * 4 + int_current * 6 + 2000;
}
/* Black Essence                Tier 2 */
uint16_t __thiscall black_essence_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t wis_current = figureAPI->getCurrentStat(_this, figure_id, WISDOM);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return wis_current * 4 + int_current * 6 + 2000;
}
/* Black Almightiness   Tier 3 */
uint16_t __thiscall black_almight_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t wis_current = figureAPI->getCurrentStat(_this, figure_id, WISDOM);
    uint16_t int_current = figureAPI->getCurrentStat(_this, figure_id, INTELLIGENCE);
    return wis_current * 3 + int_current * 5 + 1500;
}
/* Spark                                Tier 1 */
uint16_t __thiscall spark_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t str_current = figureAPI->getCurrentStat(_this, figure_id, STRENGTH);
    uint16_t dex_current = figureAPI->getCurrentStat(_this, figure_id, DEXTERITY);
    return str_current * 6 + dex_current * 6 + 3000;
}
/***
 * This function MUST be present in your code with the exact declaration
 * otherwise framework won't load your plugin
 ***/

/*  Summons fix  */

bool __thiscall isTower(SF_CGdFigure *_this, uint16_t figure_index)
{
    return _this->figures[figure_index].flags >> 11 & 1;
}

bool __thiscall isInMapBounds(SF_CGdWorld *_this, SF_Coord *position)
{
    if ((position->X < _this->map_size) && (position->Y < _this->map_size))
    {
        return 1;
    }
    return 0;
}

uint16_t __thiscall getSector(SF_CGdWorld *_this, SF_Coord *position)
{
    uint32_t index = position->X + position->Y * 0x400;
    return _this->cells[index].sector;
}

uint16_t __thiscall summonCreature(SF_CGdFigureToolbox *_this, uint16_t master_index, uint16_t creature_type)
{
    SF_Coord master_postion;
    SF_Coord summon_pos;
    uint16_t summon_index = 0;
    figureAPI->getPosition(_this->CGdFigure, &master_postion, master_index);
    summon_pos.X = master_postion.X;
    summon_pos.Y = master_postion.Y;
    if (isTower(_this->CGdFigure, master_index))
    {
        for (int i = 0; i < 9999; i++)
        {
            uint16_t someX = _this->CGdWorld->unknown1[i].uknwn1;
            uint16_t someY = _this->CGdWorld->unknown1[i].uknwn2;
            SF_Coord new_pos = {someX, someY};
            new_pos.X += master_postion.X;
            new_pos.Y += master_postion.Y;
            if (isInMapBounds(_this->CGdWorld, &new_pos) && getSector(_this->CGdWorld, &new_pos))
            {
                summon_pos.X = new_pos.X;
                summon_pos.Y = new_pos.Y;
                break;
            }
        }
    }
    uint16_t sector = getSector(_this->CGdWorld, &summon_pos);
    SF_Coord offset = {1, 4};
    SF_Coord real_pos = {0, 0};

    if (toolboxAPI->findClosestFreePosition(_this->CGdWorldToolBox, &summon_pos, &offset, sector, &real_pos))
    {
        uint16_t owner = _this->CGdFigure->figures[master_index].owner;
        summon_index = toolboxAPI->addUnit(_this, real_pos.X, real_pos.Y, owner, creature_type, 0x13, 0, 100, 0);
        if (summon_index != 0)
        {
            figureAPI->setTask(_this->CGdFigure, summon_index, TASK_PET);
            figureAPI->setJob(_this->CGdFigureJobs,summon_index, kGdJobPetIdle);
            _this->CGdFigure->figures[summon_index].master_figure = master_index;
            _this->CGdFigure->figures[summon_index].flags &= ~(FOLLOW_MODE);
        }
    }
    return summon_index;
}

void __thiscall summon_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    // we pull the pointer for this instance of spell
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    uint16_t source_index = spell->source.entity_index;
    spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);
    uint32_t tick_number = spellAPI->getXData(_this, spell_index, SPELL_TICK_COUNT_AUX);
    if (tick_number == 0)
    {
        uint16_t target_index = summonCreature(_this->SF_CGdFigureToolBox, source_index, spell_data.params[2]);
        if (target_index != 0)
        {
            toolboxAPI->addSpellToFigure(_this->SF_CGdFigureToolBox, target_index, spell_index);
            spellAPI->addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1);
            _this->active_spell_list[spell_index].to_do_count = (spell_data.params[0] * 10) / 1000;
            _this->active_spell_list[spell_index].target.entity_index = target_index;
            _this->active_spell_list[spell_index].target.entity_type = 1;
            _this->active_spell_list[spell_index].target.position.X = 0;
            _this->active_spell_list[spell_index].target.position.Y = 0;
            return;
        }
    }
    else
    {
        uint16_t required_mana =  spell_data.params[1];
        if ((_this->SF_CGdFigure->figures[source_index].set_type == 10))
        {
            required_mana = required_mana >> 1;
        }
        uint16_t current_mana = figureAPI->getCurrentStat(_this->SF_CGdFigure, source_index, MANA);
        if (current_mana >= required_mana)
        {
            _this->active_spell_list[spell_index].to_do_count = (spell_data.params[0] * 10) / 1000;
            figureAPI->subMana(_this->SF_CGdFigure, source_index, required_mana);
            return;
        }
        else
        {
            SF_Rectangle some_rect = {0, 0};
            SF_CGdTargetData target;
            uint16_t target_index = spell->target.entity_index;
            target.entity_index = target_index;
            target.position = {0, 0};
            target.entity_type = 1;
            uint32_t unused;
            spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitTarget, &unused, &target,
                                      _this->OpaqueClass->current_step, 0x19, &some_rect);
            if ((figureAPI->isAlive(_this->SF_CGdFigure, target.entity_index)) &&
                (_this->SF_CGdFigure->figures[target_index].owner != (uint16_t)(-1)) &&
                (_this->SF_CGdFigure->figures[source_index].owner != (uint16_t)(-1)))
            {
                toolboxAPI->dealDamage(_this->SF_CGdFigureToolBox, 0, target_index, 0x7FFF, 1, 0, 0);
            }
            else
            {
                toolboxAPI->removeSpellFromList(_this->SF_CGdFigureToolBox, source_index, spell_index);
                toolboxAPI->removeSpellFromList(_this->SF_CGdFigureToolBox, target_index, spell_index);
            }
            if (_this->SF_CGdFigure->figures[target.entity_index].owner != (uint16_t)(-1))
            {
                logAPI->logDebug(DEBUG_HIGH,"Broken figure, index [%d] spell [%d] type [%d]",
                                 target.entity_index, spell_index, spell->spell_line);
            }

        }
    }
    spellAPI->setEffectDone(_this, spell_index, 0);
}


extern "C" __declspec(dllexport) void InitModule(SpellforceSpellFramework *framework)
{
    sfsf = framework;
    spellAPI = sfsf->spellAPI;
    toolboxAPI = sfsf->toolboxAPI;
    figureAPI = sfsf->figureAPI;
    registrationAPI = sfsf->registrationAPI;
    effectAPI = sfsf->effectAPI;
    logAPI = sfsf->logAPI;

    SFSpell *summon_spell = registrationAPI->registerSpell(kGdSpellLineSummonBear);              // just pull any summoning spell
    registrationAPI->linkEffectHandler(summon_spell, kGdSpellJobSummon, &summon_effect_handler); // and overwrite it's effect handler which is common

/* EXAMPLE
        SFSpell *fire_burst = registrationAPI->registerSpell(ORIGINALSPELLID);
        registrationAPI->linkEnchantChanceHandler(fire_burst, &SCRIPTID);
 */

/* Rock Bullet */
    SFSpell *rock_bullet = registrationAPI->registerSpell(kGdSpellLineRockBullet);
    registrationAPI->linkEnchantChanceHandler(rock_bullet, &rock_bullet_enchant_handler);
    registrationAPI->linkEffectHandler(rock_bullet, kGdSpellJobRockBullet, &rock_bullet_effect_handler);
/* Decay */
    SFSpell *decay = registrationAPI->registerSpell(kGdSpellLineDecay);
    registrationAPI->linkEnchantChanceHandler(decay, &decay_enchant_handler);
/* Petrify */
    SFSpell *petrify = registrationAPI->registerSpell(kGdSpellLinePetrify);
    registrationAPI->linkEnchantChanceHandler(petrify, &petrify_enchant_handler);
/* Chain Rock Bullet */
    SFSpell *chain_rock_bullet = registrationAPI->registerSpell(kGdSpellLineChainRockBullet);
    registrationAPI->linkEnchantChanceHandler(chain_rock_bullet, &chain_rock_bullet_enchant_handler);
/* Fireburst */
    SFSpell *fire_burst = registrationAPI->registerSpell(kGdSpellLineFireBurst);
    registrationAPI->linkEnchantChanceHandler(fire_burst, &fire_burst_enchant_handler);
/* Fireball */
    SFSpell *fire_ball = registrationAPI->registerSpell(kGdSpellLineFireBall);
    registrationAPI->linkEnchantChanceHandler(fire_ball, &fire_ball_enchant_handler);
/* Melt Resistance */
    SFSpell *melt_resistance = registrationAPI->registerSpell(kGdSpellLineMeltResistance);
    registrationAPI->linkEnchantChanceHandler(melt_resistance, &melt_resistance_enchant_handler);
/* Chain Fireburst */
    SFSpell *chain_fire_burst = registrationAPI->registerSpell(kGdSpellLineChainFireburst);
    registrationAPI->linkEnchantChanceHandler(chain_fire_burst, &chain_fire_burst_enchant_handler);
/* Chain Fireball */
    SFSpell *chain_fire_ball = registrationAPI->registerSpell(kGdSpellLineChainFireball);
    registrationAPI->linkEnchantChanceHandler(chain_fire_ball, &chain_fire_ball_enchant_handler);
/* Iceburst */
    SFSpell *ice_burst = registrationAPI->registerSpell(kGdSpellLineIceBurst);
    registrationAPI->linkEnchantChanceHandler(ice_burst, &ice_burst_enchant_handler);
/* Freeze */
    SFSpell *freeze = registrationAPI->registerSpell(kGdSpellLineFreeze);
    registrationAPI->linkEnchantChanceHandler(freeze, &freeze_enchant_handler);
/* Chill Resistance */
    SFSpell *chill_resistance = registrationAPI->registerSpell(kGdSpellLineChillResistance);
    registrationAPI->linkEnchantChanceHandler(chill_resistance, &chill_resistance_enchant_handler);
/* Chain Iceburst */
    SFSpell *chain_ice_burst = registrationAPI->registerSpell(kGdSpellLineChainIceburst);
    registrationAPI->linkEnchantChanceHandler(chain_ice_burst, &chain_ice_burst_enchant_handler);
/* Elemental Essence */
    SFSpell *ele_essence = registrationAPI->registerSpell(kGdSpellLineEssenceElemental);
    registrationAPI->linkEnchantChanceHandler(ele_essence, &ele_essence_enchant_handler);
/* Elemental Almightiness */
    SFSpell *ele_almight = registrationAPI->registerSpell(kGdSpellLineAlmightinessElemental);
    registrationAPI->linkEnchantChanceHandler(ele_almight, &ele_almight_enchant_handler);
/* Shared Destiny */
    SFSpell *shared_destiny = registrationAPI->registerSpell(kGdSpellLineAssistance);
    registrationAPI->linkEnchantChanceHandler(shared_destiny, &shared_destiny_enchant_handler);
/* Thorns */
    SFSpell *thorns = registrationAPI->registerSpell(kGdSpellLineThornShieldDamage);
    registrationAPI->linkEnchantChanceHandler(thorns, &thorns_enchant_handler);
    registrationAPI->linkEffectHandler(thorns, kGdSpellJobThorns, &thorns_effect_handler);
/* Dominate Animal */
    SFSpell *dominate_animal = registrationAPI->registerSpell(kGdSpellLineDominateAnimal);
    registrationAPI->linkEnchantChanceHandler(dominate_animal, &dominate_animal_enchant_handler);
/* Charm Animal */
    SFSpell *charm_animal = registrationAPI->registerSpell(kGdSpellLineCharmAnimal);
    registrationAPI->linkEnchantChanceHandler(charm_animal, &charm_animal_enchant_handler);
/* Roots */
    SFSpell *roots = registrationAPI->registerSpell(kGdSpellLineRoots);
    registrationAPI->linkEnchantChanceHandler(roots, &roots_enchant_handler);
/* Hallow */
    SFSpell *hallow = registrationAPI->registerSpell(kGdSpellLineHallow);
    registrationAPI->linkEnchantChanceHandler(hallow, &hallow_enchant_handler);
/* Dispel Black Aura */
    SFSpell *dispel_black = registrationAPI->registerSpell(kGdSpellLineDispelBlackAura);
    registrationAPI->linkEnchantChanceHandler(dispel_black, &dispel_black_enchant_handler);
/* Chain Hallow */
    SFSpell *chain_hallow = registrationAPI->registerSpell(kGdSpellLineChainHallow);
    registrationAPI->linkEnchantChanceHandler(chain_hallow, &chain_hallow_enchant_handler);
/* Shock */
    SFSpell *shock = registrationAPI->registerSpell(kGdSpellLineShock);
    registrationAPI->linkEnchantChanceHandler(shock, &shock_enchant_handler);
/* Confuse */
    SFSpell *confusion = registrationAPI->registerSpell(kGdSpellLineConfuse);
    registrationAPI->linkEnchantChanceHandler(confusion, &confusion_enchant_handler);
/* Hypnotization */
    SFSpell *hypnotize = registrationAPI->registerSpell(kGdSpellLineHypnotize);
    registrationAPI->linkEnchantChanceHandler(hypnotize, &hypnotize_enchant_handler);
/* Amok */
    SFSpell *amok = registrationAPI->registerSpell(kGdSpellLineAmok);
    registrationAPI->linkEnchantChanceHandler(amok, &amok_enchant_handler);
/* Disrupt */
    SFSpell *disrupt = registrationAPI->registerSpell(kGdSpellLineDisrupt);
    registrationAPI->linkEnchantChanceHandler(disrupt, &disrupt_enchant_handler);
/* Demoralization */
    SFSpell *demoralization = registrationAPI->registerSpell(kGdSpellLineDemoralization);
    registrationAPI->linkEnchantChanceHandler(demoralization, &demoralization_enchant_handler);
/* Chain Shock */
    SFSpell *chain_shock = registrationAPI->registerSpell(kGdSpellLineChainShock);
    registrationAPI->linkEnchantChanceHandler(chain_shock, &chain_shock_enchant_handler);
/* Manadrain */
    SFSpell *mana_drain = registrationAPI->registerSpell(kGdSpellLineManaDrain);
    registrationAPI->linkEnchantChanceHandler(mana_drain, &mana_drain_enchant_handler);
/* Manatap */
    SFSpell *mana_tap = registrationAPI->registerSpell(kGdSpellLineManaTap);
    registrationAPI->linkEnchantChanceHandler(mana_tap, &mana_tap_enchant_handler);
/* Chain Manatap */
    SFSpell *chain_mana_tap = registrationAPI->registerSpell(kGdSpellLineChainManatap);
    registrationAPI->linkEnchantChanceHandler(chain_mana_tap, &chain_mana_tap_enchant_handler);
/* Dominate */
    SFSpell *dominate = registrationAPI->registerSpell(kGdSpellLineDominate);
    registrationAPI->linkEnchantChanceHandler(dominate, &dominate_enchant_handler);
/* Charm */
    SFSpell *charm = registrationAPI->registerSpell(kGdSpellLineCharm);
    registrationAPI->linkEnchantChanceHandler(charm, &charm_enchant_handler);
/* Befriend */
    SFSpell *befriend = registrationAPI->registerSpell(kGdSpellLineBefriend);
    registrationAPI->linkEnchantChanceHandler(befriend, &befriend_enchant_handler);
/* Disenchant */
    SFSpell *disenchant = registrationAPI->registerSpell(kGdSpellLineDisenchant);
    registrationAPI->linkEnchantChanceHandler(disenchant, &disenchant_enchant_handler);
/* Chain Charm */
    SFSpell *chain_charm = registrationAPI->registerSpell(kGdSpellLineChainCharm);
    registrationAPI->linkEnchantChanceHandler(chain_charm, &chain_charm_enchant_handler);
/* Mental Essence */
    SFSpell *mental_essence = registrationAPI->registerSpell(kGdSpellLineEssenceMental);
    registrationAPI->linkEnchantChanceHandler(mental_essence, &mental_essence_enchant_handler);
/* Mental Almightiness */
    SFSpell *mental_almight = registrationAPI->registerSpell(kGdSpellLineAlmightinessMental);
    registrationAPI->linkEnchantChanceHandler(mental_almight, &mental_almight_enchant_handler);
/* Pain */
    SFSpell *pain = registrationAPI->registerSpell(kGdSpellLinePain);
    registrationAPI->linkEnchantChanceHandler(pain, &pain_enchant_handler);
/* Weaken */
    SFSpell *weaken = registrationAPI->registerSpell(kGdSpellLineWeaken);
    registrationAPI->linkEnchantChanceHandler(weaken, &weaken_enchant_handler);
/* Death */
    SFSpell *death = registrationAPI->registerSpell(kGdSpellLineDeath);
    registrationAPI->linkEnchantChanceHandler(death, &death_enchant_handler);
/* Pestilence */
    SFSpell *pestilence = registrationAPI->registerSpell(kGdSpellLinePestilence);
    registrationAPI->linkEnchantChanceHandler(pestilence, &pestilence_enchant_handler);
/* Suffocation */
    SFSpell *suffocate = registrationAPI->registerSpell(kGdSpellLineSuffocation);
    registrationAPI->linkEnchantChanceHandler(suffocate, &suffocate_enchant_handler);
/* Suicide Death */
    SFSpell *suicide_death = registrationAPI->registerSpell(kGdSpellLineSuicideDeath);
    registrationAPI->linkEnchantChanceHandler(suicide_death, &suicide_death_enchant_handler);
/* Chain Pain */
    SFSpell *chain_pain = registrationAPI->registerSpell(kGdSpellLineChainPain);
    registrationAPI->linkEnchantChanceHandler(chain_pain, &chain_pain_enchant_handler);
/* Lifetap */
    SFSpell *life_tap = registrationAPI->registerSpell(kGdSpellLineLifeTap);
    registrationAPI->linkEnchantChanceHandler(life_tap, &life_tap_enchant_handler);
/* Dominate Undead */
    SFSpell *dominate_undead = registrationAPI->registerSpell(kGdSpellLineDominateUndead);
    registrationAPI->linkEnchantChanceHandler(dominate_undead, &dominate_undead_enchant_handler);
/* Chain Lifetap */
    SFSpell *chain_life_tap = registrationAPI->registerSpell(kGdSpellLineChainLifetap);
    registrationAPI->linkEnchantChanceHandler(chain_life_tap, &chain_life_tap_enchant_handler);
/* Slow Fighting */
    SFSpell *slow_fight = registrationAPI->registerSpell(kGdSpellLineSlowFighting);
    registrationAPI->linkEnchantChanceHandler(slow_fight, &slow_fight_enchant_handler);
/* Poison */
    SFSpell *poison = registrationAPI->registerSpell(kGdSpellLinePoison);
    registrationAPI->linkEnchantChanceHandler(poison, &poison_enchant_handler);
/* Inflexibility */
    SFSpell *inflexibility = registrationAPI->registerSpell(kGdSpellLineInflexibility);
    registrationAPI->linkEnchantChanceHandler(inflexibility, &inflexibility_enchant_handler);
/* Dispel White Aura */
    SFSpell *dispel_white = registrationAPI->registerSpell(kGdSpellLineDispelWhiteAura);
    registrationAPI->linkEnchantChanceHandler(dispel_white, &dispel_white_enchant_handler);
/* Slow Walking */
    SFSpell *slow_walk = registrationAPI->registerSpell(kGdSpellLineSlowness);
    registrationAPI->linkEnchantChanceHandler(slow_walk, &slow_walk_enchant_handler);
/* Dark Banishing */
    SFSpell *dark_banishing = registrationAPI->registerSpell(kGdSpellLineDarkBanishing);
    registrationAPI->linkEnchantChanceHandler(dark_banishing, &dark_banishing_enchant_handler);
/* Inability */
    SFSpell *inability = registrationAPI->registerSpell(kGdSpellLineInability);
    registrationAPI->linkEnchantChanceHandler(inability, &inability_enchant_handler);
/* Remediless */
    SFSpell *remediless = registrationAPI->registerSpell(kGdSpellLineRemediless);
    registrationAPI->linkEnchantChanceHandler(remediless, &remediless_enchant_handler);
/* Mutation */
    SFSpell *mutation = registrationAPI->registerSpell(kGdSpellLineMutation);
    registrationAPI->linkEnchantChanceHandler(mutation, &mutation_enchant_handler);
/* Chain Mutation */
    SFSpell *chain_mutation = registrationAPI->registerSpell(kGdSpellLineChainMutation);
    registrationAPI->linkEnchantChanceHandler(chain_mutation, &chain_mutation_enchant_handler);
/* Black Essence */
    SFSpell *black_essence = registrationAPI->registerSpell(kGdSpellLineEssenceBlack);
    registrationAPI->linkEnchantChanceHandler(black_essence, &black_essence_enchant_handler);
/* Black Almightiness */
    SFSpell *black_almight = registrationAPI->registerSpell(kGdSpellLineAlmightinessBlack);
    registrationAPI->linkEnchantChanceHandler(black_almight, &black_almight_enchant_handler);
/* Spark */
    SFSpell *spark = registrationAPI->registerSpell(kGdSpellLineSpark);
    registrationAPI->linkEnchantChanceHandler(spark, &spark_enchant_handler);
}



/***
 * This function MUST be present in your code with the exact declaration
 * otherwise the framework is unable to describe your mod in logs and mod info menu (menu not yet implemented)
 ***/

extern "C" __declspec(dllexport) SFMod *RegisterMod(SpellforceSpellFramework *framework)
{
    return framework->createModInfo("Reimagined Framework File", "v1", "DarkRuler & UnSchtalch",
                                    "A necessary sub-mod for SpellForce:Reimagined to work with SpellForce:Framework.");
}


// Required to be present by OS, not required for any functionality

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            /* Code path executed when DLL is loaded into a process's address space. */
            break;

        case DLL_THREAD_ATTACH:
            /* Code path executed when a new thread is created within the process. */
            break;

        case DLL_THREAD_DETACH:
            /* Code path executed when a thread within the process has exited *cleanly*. */
            break;

        case DLL_PROCESS_DETACH:
            /* Code path executed when DLL is unloaded from a process's address space. */
            break;
    }

    return TRUE;
}
