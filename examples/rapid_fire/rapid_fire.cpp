#include "../../src/api/sfsf.h"
#include <stdint.h>

// ============================================================================
// rapid fire — ranged-buff example for the RANGED_SHOT on-hit phase
//
// While the buff is active on a figure, every ranged shot of that figure
// spawns extra clones of the donor projectile (data-driven cascade: the
// number comes from spell params[1], the buff duration in ms from params[0]).
// Clones launch as a batch across the shot-completion animation window
// (7 ticks ~ 700 ms, evenly spread) and still fly the donor flight length
// from their own launch, so they land on the same victim with the same
// spread, dealing the snapshot damage of the original shot. On top of that,
// every clone gets its OWN full vanilla roll
// of the shooter's weapon enchant pool (framework helper
// rollWeaponEnchantEffects); winning procs are scheduled with a future-start
// addSpell: the spell instance sits on the victim from the shot on and its
// effect lands at the clone's arrival tick (a pathological arrival delta
// >= 0x8000 is skipped with a warning - the short window guard).
//
// Data requirements (see README.md): SpellLine 251, Spell <SPELL_ID> —
// owner-confirmed free id (vanilla occupies 3555-3573, see README), SpellJob 177,
// params[0] = buff duration in ms, params[1] = extra arrows per shot (default
// 3 in the shipped gamedata; tunable without recompilation).
// ============================================================================

SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;
RegistrationFunctions *registrationAPI;
EffectFunctions *effectAPI;
SFLog *logger;

// ids confirmed by the owner; free of vanilla namespaces
// (GdSpellLine max 241, GdSpellJob max 167, spell ids max ~3554)
#define RAPID_FIRE_LINE 251
#define RAPID_FIRE_JOB 177

// we declare spell type handler for Rapid Fire, it must initialize ticks and
// associate the spell type with a spell job
void __thiscall rapid_fire_type_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    _this->active_spell_list[spell_index].spell_job = RAPID_FIRE_JOB;
    // AUX init for the effect handler phase split (tick 0 / tick 1) - mirror of
    // arrow_reflection.cpp:33; default-0 for an unset xdata key is unproven.
    // NOT the cancelled v1.4 cascade-budget counter.
    spellAPI->setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

// we declare spell end handler
// this handler would work in case a spell wasn't finished correctly
void __thiscall rapid_fire_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // the clear calls are safe even when the flags were never set
    spellAPI->figTryClrCHkSPlBfrJob2(_this, spell_index);
    spellAPI->figClrChkSplBfrChkBattle(_this, spell_index, 0);
    spellAPI->removeDLLNode(_this, spell_index);
    spellAPI->setEffectDone(_this, spell_index, 0);
}

// result of engine reverse-engineering
// that's how the game calculates the distance between two points on the map
// all credits to UnSchtalch/Muddykat for retrieving this function from the deeps of Spellforce engine
// we'll need this function later to adjust the extra arrows flight time
// (byte-exact copy of the internal helper, precedent arrow_reflection.cpp:50-74;
// toolboxAPI->getDistance is a typedef-only field - not visible to mods)
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

// we have to manually calculate the direction where the extra arrows would fly
// let's wrap all calculations in a function accepting position (X-Y) coordinates
// of two figures and returning the rectangle going from the first figure to the
// second (byte-exact mirror of the shot vector block; donor sf_onhit_hook.c:416-460,
// precedent arrow_reflection.cpp:78-132)
SF_Rectangle *get_spell_vector(SF_Coord *source_position, SF_Coord *target_position, SF_Rectangle *spell_vector)
{
    // let's build a real rectangle based on coordinates we got in arguments
    uint16_t maxY = (source_position->Y <= target_position->Y)
                        ? (target_position->Y)
                        : (source_position->Y);

    uint16_t maxX = (source_position->X <= target_position->X)
                        ? (target_position->X)
                        : (source_position->X);

    uint16_t minY = (source_position->Y > target_position->Y)
                        ? (target_position->Y)
                        : (source_position->Y);

    uint16_t minX = (source_position->X > target_position->X)
                        ? (target_position->X)
                        : (source_position->X);

    // SF_Rectangle is defined with only two points rather than four
    // meaningly upper-left and lower-right vertices
    // let's declare variables for them, p1 for upper-left and p2 for lower-right
    SF_Coord p1;
    SF_Coord p2;

    // let's establish upper-left vertex
    p1.X = maxX;
    if (minX < maxX)
    {
        p1.X = minX;
    }
    p1.Y = maxY;
    if (minY < maxY)
    {
        p1.Y = minY;
    }

    // now let's establish lower-right vertex
    p2.X = maxX;
    if (maxX <= minX)
    {
        p2.X = minX;
    }
    p2.Y = maxY;
    if (maxY <= minY)
    {
        p2.Y = minY;
    }

    // let's calculate the vector basing on the rectangle we've got
    spell_vector->partA = p1.Y << 0x10 | p1.X;
    spell_vector->partB = p2.Y << 0x10 | p2.X;

    return spell_vector;
}

// donor projectile flight time: the donor formula sf_onhit_hook.c:462-468
// ((distance & 0xffff) * 1400) / 3000 with the same 0 -> 1 clamp the shot path
// applies; uint32 math cannot overflow (flight <= 30583, i <= 65535)
uint32_t rapid_fire_flight_ticks(SF_Coord *source_position, SF_Coord *target_position)
{
    uint32_t distance = getDistance(source_position, target_position);
    distance = ((distance & 0xffff) * 1400) / 3000;
    return (distance != 0) ? distance : 1;
}

// Volley launch window, engine ticks: the shot-completion animation job
// kGdJobHitTargetRange2 (J74, jobdata.des 700 ms); engine conversion
// floor(ms * 10 / 1000) = 7 ticks @10 Hz.
#define RAPID_FIRE_WINDOW_TICKS 7

// Launch offset of clone i (1-based) within the volley window, engine ticks -
// even spread Delta_i = (WINDOW_TICKS * i) / (clones + 1). Integer math:
// K=1 -> 3; K=2 -> 2,4; K=3 -> 1,3,5; K=6 -> 1..6; K > 6 may repeat ticks
// (independent spell instances - no clamp needed).
static uint32_t rapid_fire_launch_delta(uint32_t clones, uint32_t i)
{
    return (RAPID_FIRE_WINDOW_TICKS * i) / (clones + 1);
}

// RANGED_SHOT phase handler: damage slot carries donor_effect_id (phase
// contract, OnHitPhase::RANGED_SHOT); the return value is ignored by the
// dispatcher - donor damage is already snapshotted into the donor xdata
// (sf_onhit_hook.c:476).
uint16_t __thiscall rapid_fire_onhit_handler(SF_CGdFigureJobs *_this,
                                             uint16_t source_index,
                                             uint16_t target_index,
                                             uint16_t damage)
{
    uint16_t donor_effect_id = damage;

    // Critical Strikes oneshot marker must never volley (precedent
    // arrow_reflection.cpp:145-148).
    if (effectAPI->getEffectXData(_this->CGdEffect, donor_effect_id,
                                  EFFECT_PHYSICAL_DAMAGE) == 0x7fff)
    {
        return donor_effect_id;
    }

    // Buff instance on the shooter (dispatcher gate hasSpellOnIt already passed;
    // index needed for the spell data - precedent sf_onhit_handlers.cpp:19-20).
    uint16_t spell_index = toolboxAPI->getSpellIndexOfType(
        _this->CGdFigureToolBox, source_index, RAPID_FIRE_LINE, 0);
    if (spell_index == 0)
    {
        return donor_effect_id;
    }
    SF_GdSpell *spell = &_this->CGdSpell->active_spell_list[spell_index];
    SF_CGdResourceSpell spell_data;
    spellAPI->getResourceSpellData(_this->CGdResource, &spell_data,
                                   spell->spell_id);

    // Data-driven volley: params[1] extra arrows per shot, default 3.
    // Engine precedent - Salvo reads its shot count from params[1].
    // params[0] holds the buff duration (ms), mirroring arrow_reflection.
    uint16_t volleys = spell_data.params[1];

    uint32_t flight_ticks = rapid_fire_flight_ticks(
        &_this->CGdFigure->figures[source_index].position,
        &_this->CGdFigure->figures[target_index].position);
    SF_Rectangle vector;
    get_spell_vector(&_this->CGdFigure->figures[source_index].position,
                     &_this->CGdFigure->figures[target_index].position,
                     &vector);
    uint32_t current_step = _this->OpaqueClass->current_step;

    // Wide loop counter: a 16-bit counter wraps at
    // params[1]=0xffff (i <= volleys stays true forever) and locks the
    // handler in an endless cascade; u16 volleys widens safely here.
    uint32_t n;
    for (n = 0; n < volleys; ++n)
    {
        SF_CGdTargetData clone_source = {1, source_index, {0, 0}};
        SF_CGdTargetData clone_target = {1, target_index, {0, 0}};

        // Even spread across the shot-completion animation window: clone n
        // launches (WINDOW_TICKS * (n+1)) / (volleys+1) ticks after the shot
        // and still flies the donor flight length (tick_count unchanged), so
        // arrivals keep the same spread.
        uint32_t delta = rapid_fire_launch_delta(volleys, n + 1);
        uint16_t clone_id = effectAPI->cloneEffect(
            _this->CGdEffect, donor_effect_id, &clone_source, &clone_target,
            current_step + delta, (uint16_t)flight_ticks, &vector);

        // The engine silently returns 0 when all 1999 effect slots are taken:
        // warn and stop the cascade; clones already spawned stay in flight.
        // (precedent arrow_reflection.cpp:211-215)
        if (clone_id == 0)
        {
            logger->logWarning("RAPID FIRE FAILED: no free effect slot for the extra arrow");
            return donor_effect_id;
        }

        logger->logInfo("RAPID FIRE: extra arrow spawned");

        // Per-arrow enchant procs: delta guard FIRST - the future-start tick
        // lives in a 16-bit to_do_count (short window [0..0x7FFF]); a delta
        // >= 0x8000 reads back negative and would fire next tick. Defensive
        // only: the launch spread is <= WINDOW_TICKS-1 and the flight
        // <= 30583, so the total arrival delta stays below 0x8000 for any
        // natural input. A fired guard skips the procs of THIS arrow (warn) -
        // the clone's physics is untouched.
        if (delta + flight_ticks >= 0x8000)
        {
            logger->logWarning("RAPID FIRE: enchant delta overflow, proc skipped");
            continue;
        }
        // Full vanilla roll per arrow: every non-empty slot rolled independently (count 0..3).
        // Ranged weapon slot is equipment[3] (hookA:531 - (action.type !=
        // 10000)*2+1 with 0x2712); the dispatcher only fires from the ranged
        // branch.
        uint16_t enchant_spell_ids[3];
        uint8_t proc_count = spellAPI->rollWeaponEnchantEffects(
            _this, source_index,
            _this->CGdFigure->figures[source_index].equipment[3],
            enchant_spell_ids);
        for (uint8_t j = 0; j < proc_count; ++j)
        {
            // Future-start addSpell: inside the spell instance
            // to_do_count = param_2 - (short)current_step = pure delta
            // countdown; the job handler fires at THIS clone's arrival tick.
            // Source = the shooter {1, source_index}, target = the victim.
            // The instance is observable on the target from the shot on:
            // death/dispel before arrival removes it - new
            // behaviour, owner-accepted.
            spellAPI->addSpell(_this->CGdSpell, enchant_spell_ids[j],
                               (short)(current_step + delta + flight_ticks), &clone_source,
                               &clone_target, 0);
        }
    }
    return donor_effect_id;
}

// we declare effect handler
// triggered twice per buff window: tick 0 applies the visual effect and arms
// the timer, tick 1 finishes the instance
void __thiscall rapid_fire_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // we pull the pointer for this instance of spell
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];

    // we initiate ticks counter which is necessary to emulate spell duration
    uint32_t current_tick = spellAPI->getXData(_this, spell_index, SPELL_TICK_COUNT_AUX);

    // we increase amount of ticks passed by 1, ensuring that the next time
    // effect handler is triggered, it will start with tick 1
    spellAPI->addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1);

    // we check the current tick
    // during tick 0 we will initialize the spell main logic if it's allowed with refresh handler
    if (current_tick == 0)
    {
        // we trigger the refresh handler to know whether the spell could be applied
        // to the shooter or whether it would be blocked with other instances
        if (spellAPI->checkCanApply(_this, spell_index))
        {
            // we load the spell parameters from GameData.cff
            // we'll need them only in tick 0
            SF_CGdResourceSpell spell_data;
            spellAPI->getResourceSpellData(_this->CGdResource, &spell_data, spell->spell_id);

            // we store index of the shooter
            uint16_t source_index = _this->active_spell_list[spell_index].source.entity_index;

            // we declare structure for relative position of visual effect
            SF_CGdTargetData relative_data;
            figureAPI->getPosition(_this->CGdFigure, &relative_data.position, source_index);
            relative_data.entity_type = 1;
            relative_data.entity_index = 0;

            // we apply the visual effect to the target (paired with the lua
            // registration from README - without lua the mechanism works, the
            // buff just has no visual)
            spellAPI->addSpellVisualEffect(_this, spell_index, kGdEffectSpellHitTarget, &relative_data, 0, NULL);

            // we disable the spell effect from being triggered until specified
            // amount of time passes
            // param 0 = buff duration in ms; on/off mechanic without frequent ticks
            // (no F_CHECK_SPELLS_BEFORE_JOB / CHECK_SPELLS_BEFORE_JOB2 here:
            // the RANGED_SHOT dispatcher gates on buff presence alone)
            uint32_t ticks_interval = spell_data.params[0];
            _this->active_spell_list[spell_index].to_do_count = (uint16_t)((ticks_interval * 10) / 1000);
        }
        else
        // the refresh handler indicated that the current instance of rapid fire
        // can't be applied, because the target is already affected with the same spell
        {
            // in such case we finish the spell and remove it from the active spells list
            spellAPI->removeDLLNode(_this, spell_index);
            spellAPI->setEffectDone(_this, spell_index, 0);
        }
    }
    else
    // the current_tick is 1
    {
        // we clear the flags which we might have inherited (safe if unset)
        spellAPI->figTryClrCHkSPlBfrJob2(_this, spell_index);
        spellAPI->figClrChkSplBfrChkBattle(_this, spell_index, 0);
        // we finish the spell and remove it from the active spells list
        spellAPI->removeDLLNode(_this, spell_index);
        spellAPI->setEffectDone(_this, spell_index, 0);
    }
}

// the Rapid Fire is freely refreshable
// it means that when it's casted on a shooter again before its duration expired,
// it would remove the old instance and replace it with a new one, this would update
// the spell duration; clones already in flight from the old window keep flying
// (supersession) and consume the budget of the new window - accepted behavior
int __thiscall rapid_fire_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // we store spell pointer and source index for our convenience
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    uint16_t source_index = spell->source.entity_index;

    // we should check for whether the shooter is affected with more than a single
    // instance of Rapid Fire
    uint16_t pruned_spell_index = toolboxAPI->getSpellIndexOfType(_this->CGdFigureToolBox, source_index, RAPID_FIRE_LINE, spell_index);
    // because we passed spell_index as last argument of the function, this index will be ignored
    // the function will return the spell index of another Rapid Fire instance, if there is any
    // if there is no other instance of Rapid Fire than the current, the function will return 0

    // if the function returned any spell index, we must prune that instance to make a space for the new one
    if (pruned_spell_index != 0)
    {
        logger->logInfo("REMOVE OLD RAPID FIRE");
        // first we should clear the flags which the old instance set up
        spellAPI->figTryClrCHkSPlBfrJob2(_this, pruned_spell_index);
        spellAPI->figClrChkSplBfrChkBattle(_this, pruned_spell_index, 0);
        // then we should end the previous instance
        spellAPI->removeDLLNode(_this, pruned_spell_index);
        spellAPI->setEffectDone(_this, pruned_spell_index, 0);
    }

    // we return true showing that the new instance can be applied to the target
    return 1;
}

/***
 * This function MUST be present in your code with the exact declaration
 * otherwise framework won't load your plugin
 ***/
extern "C" __declspec(dllexport) void InitModule(SpellforceSpellFramework *framework)
{
    // we pull pointers to framework methods and structures
    sfsf = framework;
    spellAPI = sfsf->spellAPI;
    toolboxAPI = sfsf->toolboxAPI;
    figureAPI = sfsf->figureAPI;
    registrationAPI = sfsf->registrationAPI;
    effectAPI = sfsf->effectAPI;
    logger = sfsf->logAPI;

    // we register handlers for the custom spell
    // linkOnHitHandler with RANGED_SHOT arms the per-shot cascade (the phase is
    // dispatched once per ranged shot from the ranged branch of sf_onhit_hook)
    SFSpell *rapid_fire_spell = registrationAPI->registerSpell(RAPID_FIRE_LINE);
    registrationAPI->linkTypeHandler(rapid_fire_spell, &rapid_fire_type_handler);
    registrationAPI->linkEffectHandler(rapid_fire_spell, RAPID_FIRE_JOB, &rapid_fire_effect_handler);
    registrationAPI->linkRefreshHandler(rapid_fire_spell, &rapid_fire_refresh_handler);
    registrationAPI->linkEndHandler(rapid_fire_spell, &rapid_fire_end_handler);
    registrationAPI->linkOnHitHandler(rapid_fire_spell, &rapid_fire_onhit_handler, RANGED_SHOT);
}

/***
 * This function MUST be present in your code with the exact declaration
 * otherwise the framework is unable to describe your mod in logs and mod info menu (menu not yet implemented)
 ***/
extern "C" __declspec(dllexport) SFMod *RegisterMod(SpellforceSpellFramework *framework)
{
    return framework->createModInfo("Rapid Fire", "2.0.0", "SFSF", "Example of the RANGED_SHOT on-hit phase: every ranged shot of a buffed figure spawns extra clones of the donor projectile (count = spell params[1]), each clone rolls its own full weapon-enchant roll with procs scheduled to start at the clone's arrival tick (future-start addSpell).");
}
