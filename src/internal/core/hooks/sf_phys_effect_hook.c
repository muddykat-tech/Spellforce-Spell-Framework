#include "sf_phys_effect_hook.h"
#include "sf_projectile_visual_hook.h"
#include "../../registry/spell_data_registries/sf_phys_effect_registry.h"
#include "../../registry/spell_data_registries/sf_onhit_registry.h"
#include <utility>
#include <list>

// Effect index of the projectile currently resolved by sf_phys_effect_hook.
// Set on hook entry, read by handlers through effectAPI.getCurrentEffectIndex().
static uint16_t g_current_phys_effect_index = 0;

uint16_t __thiscall getCurrentEffectIndex()
{
    return g_current_phys_effect_index;
}

uint16_t __thiscall cloneEffect(SF_CGdEffect *_this, uint16_t src_effect_id, SF_CGdTargetData *source,
                                SF_CGdTargetData *target, uint32_t tick_start, uint16_t tick_count,
                                SF_Rectangle *rect)
{
    uint16_t clone_id = effectAPI.addEffect(_this, kGdEffectProjectile, source, target,
                                            tick_start, tick_count, rect);
    if (clone_id == 0)
    {
        // No free effect slot was allocated (engine limit 1999, Add returns 0 silently).
        return 0;
    }

    /* Visual source anchor (probe-7/8 layout: +0x45 pair = SOURCE used by the
     * client template resolver 7e0aa0; +0x3E pair = TARGET/homing — never
     * touch). Honest clone: the template comes from the DONOR's figure
     * (weapon line), flight endpoints were snapshotted at Add and are not
     * affected. Record addressing: record base = _this + id * 0x2c + 0x3c;
     * type u8, index u16 (record layout canon: source pair at +0x09/+0x0A). */
    uint8_t *donor_rec = (uint8_t *)_this + (uint32_t)src_effect_id * 0x2c + 0x3c;
    uint8_t *clone_rec = (uint8_t *)_this + (uint32_t)clone_id * 0x2c + 0x3c;
    clone_rec[9] = donor_rec[9];                                   /* +0x45: source entity type */
    *(uint16_t *)(clone_rec + 10) = *(uint16_t *)(donor_rec + 10); /* +0x46: source entity index */

    // Engine dispatch branches on key PRESENCE, not value: copy donor xdata key by key,
    // only when the donor actually carries the key. An unconditional setEffectXData(key, 0)
    // would plant a false dispatch branch (e.g. rain for EFFECT_ADD_SUBSPELL == 0).
    static const SpellDataKey donor_keys[] = {
        EFFECT_SPELL_LINE,
        EFFECT_SUBSPELL_ID,
        EFFECT_ADD_SUBSPELL,
        EFFECT_MANA_COST,
        EFFECT_PHYSICAL_DAMAGE,
    };
    for (SpellDataKey key : donor_keys)
    {
        if (effectAPI.effectXDataExists(_this, src_effect_id, key))
        {
            effectAPI.setEffectXData(_this, clone_id, key,
                                     effectAPI.getEffectXData(_this, src_effect_id, key));
        }
    }

    /* Honest clone: EFFECT_ENTITY_INDEX3 travels with the donor when present
     * and is copied key-by-key (whitelist gate, same rule as donor_keys above)
     * — this is the key the engine's own kGdSpellLineRockBulletChained
     * override inside CDrwEffectObject::vfunction15 reads. The wave-12c
     * fallback marker (INDEX3 synthesized from the donor record's source
     * index) was removed together with the diagnostic visual-hook layer:
     * the K12 source-pair copy above already carries the donor anchor.
     * There is no EFFECT_ENTITY_TYPE3 in the SpellDataKey enum, so no type
     * companion key is written. */
    if (effectAPI.effectXDataExists(_this, src_effect_id, EFFECT_ENTITY_INDEX3))
    {
        effectAPI.setEffectXData(_this, clone_id, EFFECT_ENTITY_INDEX3,
                                 effectAPI.getEffectXData(_this, src_effect_id, EFFECT_ENTITY_INDEX3));
    }

    /* Anchor registration (waves 17/19): the arc is rendered per frame from
     * the anchors passed to the 8aa610 call site inside vf15, and the record
     * keeps the DONOR in its source pair (K12 copy above) so the client
     * template resolver uses the donor's weapon. The engine resolver is
     * stateless and re-reads the record keys EVERY frame, so the clone's
     * record id -> {donor, reflector} entry stays live for the whole flight:
     * expire = tick_start + tick_count covers exactly the rendered frames
     * cs in [start, start+length] (research O-F1/O-F5). Donor id: the
     * donor's EFFECT_ENTITY_INDEX3 when it carries one, else the donor
     * record's source index (+0x46). now_cs = engine current_step lets the
     * put side sweep finished flights and, on a full ring, drop the entry
     * with the earliest expire_cs (wave 21). */
    uint32_t anchor_donor = effectAPI.effectXDataExists(_this, src_effect_id, EFFECT_ENTITY_INDEX3)
                                ? effectAPI.getEffectXData(_this, src_effect_id, EFFECT_ENTITY_INDEX3)
                                : *(uint16_t *)(donor_rec + 10);
    pvfx_register_anchor(clone_id, (uint16_t)anchor_donor, source->entity_index,
                         tick_start + tick_count, _this->OpaqueClass->current_step);

    // Entity endpoints always come from the clone's own source/target pair.
    effectAPI.setEffectXData(_this, clone_id, EFFECT_ENTITY_TYPE, source->entity_type);
    effectAPI.setEffectXData(_this, clone_id, EFFECT_ENTITY_INDEX, source->entity_index);
    effectAPI.setEffectXData(_this, clone_id, EFFECT_ENTITY_TYPE2, target->entity_type);
    effectAPI.setEffectXData(_this, clone_id, EFFECT_ENTITY_INDEX2, target->entity_index);

    return clone_id;
}

void __thiscall sf_phys_effect_hook(SF_CGdEffect *_this, uint16_t effect_id)
{
    uint32_t source_id = effectAPI.getEffectXData(_this, effect_id, EFFECT_ENTITY_INDEX);
    uint32_t source_type = effectAPI.getEffectXData(_this, effect_id, EFFECT_ENTITY_TYPE);
    uint32_t target_id = effectAPI.getEffectXData(_this, effect_id, EFFECT_ENTITY_INDEX2);
    uint32_t target_type = effectAPI.getEffectXData(_this, effect_id, EFFECT_ENTITY_TYPE2);
    uint32_t damage = effectAPI.getEffectXData(_this, effect_id, EFFECT_PHYSICAL_DAMAGE);

    //rain handlers
    if ((!effectAPI.getEffectXData(_this, effect_id, EFFECT_ADD_SUBSPELL)) &&
        (effectAPI.effectXDataExists(_this, effect_id, EFFECT_ADD_SUBSPELL)))
    {
        uint16_t spell_index = effectAPI.getEffectXData(_this, effect_id, EFFECT_SPELL_INDEX);
        uint16_t spell_line = spellAPI.getSpellLine(_this->CGdSpell, spell_index);
        sub_effect_handler_ptr rain_handler = get_rain_handler(spell_line);
        rain_handler(_this, effect_id);
    }
    else
    {
        bool isMagicDamage = 0;
        if (target_type == 1)
        {
            if ((figureAPI.isAlive(_this->CGdFigure, target_id))
                && (toolboxAPI.isTargetable(_this->CGdFigureToolBox, target_id)))
            {
                g_current_phys_effect_index = effect_id;

                uint16_t effect_spell_line = effectAPI.getEffectXData(_this, effect_id, EFFECT_SPELL_LINE);
                for (int i = PHASE_0; i < OnHitEnd; ++i)
                {
                    std::list<std::pair<uint16_t, phys_effect_handler_ptr>> phys_list =
                        get_phys_effect_phase(static_cast<OnHitPhase>(i));
                    for (auto it = phys_list.crbegin(); it != phys_list.crend(); ++it)
                    {
                        std::pair<uint16_t, phys_effect_handler_ptr> entry = *it;
                        uint16_t spell_line_id = entry.first;

                        if (spellAPI.hasSpellTag(spell_line_id, TARGET_ONHIT_SPELL))
                        {
                            // Target-side dispatch only: for ranged attacks the source-side
                            // handlers already ran in the on-hit hook when the shot was fired.
                            if ((_this->CGdFigure->figures[target_id].flags & F_CHECK_SPELLS_BEFORE_JOB) != 0)
                            {
                                if (toolboxAPI.hasSpellOnIt(_this->CGdFigureToolBox, target_id,
                                                            spell_line_id))
                                {
                                    phys_effect_handler_ptr phys_func = entry.second;
                                    damage = phys_func(_this, source_id, target_id, &isMagicDamage,
                                                       damage);
                                }
                            }
                        }
                        else
                        {
                            // Untagged (vanilla) lines match the projectile's spell line;
                            // PHASE_5 entries (e.g. spark) still run before the assistance block.
                            if ((effect_spell_line != 0) && (spell_line_id == effect_spell_line))
                            {
                                phys_effect_handler_ptr phys_func = entry.second;
                                damage = phys_func(_this, source_id, target_id, &isMagicDamage,
                                                   damage);
                            }
                        }
                    }
                }

                if (toolboxAPI.hasSpellOnIt(_this->CGdFigureToolBox, target_id, kGdSpellLineAssistance) &&
                    (damage != 0x7fff))
                {
                    std::list<std::pair<uint16_t, onhit_handler_ptr>> onhit_list = get_onhit_phase(OnHitPhase::PHASE_2);
                    for (auto it = onhit_list.crbegin(); it != onhit_list.crend(); ++it)
                    {
                        std::pair<uint16_t, onhit_handler_ptr> entry = *it;
                        uint16_t spell_line_id = entry.first;
                        if (spell_line_id == kGdSpellLineAssistance)
                        {
                            onhit_handler_ptr onhit_func = entry.second;
                            damage = onhit_func(_this->CGdFigureToolBox->CGdFigureJobs, source_id, target_id,
                                                damage);
                        }
                    }

                }
                // Deliberate behaviour change for zero-damage projectiles: DealDamage(0) is
                // skipped here, while HEAD and the engine original called it unconditionally.
                if (damage != 0)
                {
                    toolboxAPI.dealDamage(_this->CGdFigureToolBox, source_id, target_id, damage, isMagicDamage, 1, 0);
                }
                //Monument protection handler
                uint32_t mana_cost = effectAPI.getEffectXData(_this, effect_id, EFFECT_MANA_COST);
                if (mana_cost != 0)
                {
                    SF_CGdTargetData source;
                    source.position = {0,0};
                    source.entity_index = source_id;
                    source.entity_type = source_type;

                    SF_CGdTargetData target;
                    target.position = {0,0};
                    target.entity_index = target_id;
                    target.entity_type = target_type;

                    SF_Rectangle rect = {0,0};

                    effectAPI.addEffect(_this, kGdEffectMonumentHitFigure, &source, &target,
                                        _this->OpaqueClass->current_step, 0x14, &rect);
                    if (figureAPI.isAlive(_this->CGdFigure, target_id))
                    {
                        figureAPI.subMana(_this->CGdFigure, target_id, mana_cost);
                    }
                }
            }
        }
        //Siege Aura Handler
        else
        {

            if (_this->CGdBuilding->buildings[target_id].health_current != 0)
            {
                buildingAPI.buildingDealDamage(_this->CGdBuildingToolbox, source_id, target_id, damage, 0);
            }
        }
    }
}
