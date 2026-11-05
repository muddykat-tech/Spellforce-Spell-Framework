#include "../../src/api/sfsf.h"
#include <windows.h>

// Arrow Reflections is a spell which reflects any ranged attacks which hit the spellcaster back towards an attacker

// We declare macros for Arrows Reflection Spell Type and Spell Job
// Spell Type 247, Spell Job 0xaf = 175

// The custom Spell Type also must be defined within GameData.cff
// and provided with at least one spell corresponding to it

// line 247 in working gamedata (spell 3554 overwritten for testing), owner directive 2026-11-04
#define ARROWS_REFLECTION_LINE 247
#define ARROWS_REFLECTION_JOB 0xaf

SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;
IteratorFunctions *iteratorAPI;
RegistrationFunctions *registrationAPI;
EffectFunctions *effectAPI;
AiFunctions *aiAPI;
SFLog *logger;

// we declare spell type handler for Arrows Reflection, it must initialize ticks and bonus modifier values

void __thiscall arrows_reflection_type_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // we associate spell type with a spell job
    _this->active_spell_list[spell_index].spell_job = ARROWS_REFLECTION_JOB;
    // we initialize the ticks counter
    spellAPI->setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

// we declare spell end handler
// this handler would work in case a spell wasn't finished correctly
void __thiscall arrows_reflection_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    spellAPI->figTryClrCHkSPlBfrJob2(_this, spell_index);
    spellAPI->figClrChkSplBfrChkBattle(_this, spell_index, 0);
    spellAPI->removeDLLNode(_this, spell_index);
    spellAPI->setEffectDone(_this, spell_index, 0);
}

// result of engine reverse-engineering
// that's how the game calculates the distance between two points on the map
// all credits to UnSchtalch/Muddykat for retrieving this function from the deeps of Spellforce engine
// we'll need this function later to adjust reflected projectile flight time
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

// we have to manually calculate the direction where the reflected projectile would fly
// let's wrap all calculations in a function accepting position (X-Y) coordinates of two figures and returning vector going from the first figure to the second
SF_Rectangle* get_spell_vector(SF_Coord* source_position, SF_Coord* target_position, SF_Rectangle* spell_vector)
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

// we declare Phys Effect Handler
// this handler is triggered whenever a projectile (ranged attack) resolves against the spellcaster
// the handler intercepts the physical damage the projectile was about to deal and allows us to modify it according to our own rules
// the phys effect hook will continue processing the value which we return to it from the point of interception
// we determine the moment when the handler is triggered as part of mod initialization when we link the spell with the Phys Effect Handler

uint16_t __thiscall arrows_reflection_phys_handler(SF_CGdEffect *_this, uint16_t source, uint16_t target,
                                                   bool *isSpellDamage, uint16_t damage)
{
    // first of all, we should check for dead-ends such as zero damage or automatical oneshot provided by Critical Strikes spell

    // 0x7fff is an amount which Critical Strikes uses to turn damage into oneshot
    // instead of checking for spell effect lingering over spellcaster, we can know that with doing damage comparison
    // also, in case there would be other oneshotting spells, our algorithm would automatically hook them on without the need to modify it
    if (damage == 0 || damage == 0x7fff)
    {
        logger->logInfo("ARROW REFLECTION INTERRUPTED");
        return damage;
    }

    // a self-shot (source == target) has no vector to mirror: the clone would
    // land on the same figure with the reflection buff still active and chain
    // clones forever, so the damage simply passes through
    if (source == target)
    {
        return damage;
    }

    // the handler's source is the attacker and the handler's target is the spellcaster
    // (the spell is tagged as TARGET_ONHIT_SPELL, so the phys effect hook dispatches target-side)
    // we clone the incoming projectile and reverse its endpoints, so the clone flies
    // from the spellcaster back to the attacker

    // first we have to calculate the vector between the spellcaster and the attacker
    // the vector can be stored as variable of SF_Rectangle type
    SF_Rectangle spell_vector;
    get_spell_vector(&_this->CGdFigure->figures[source].position, &_this->CGdFigure->figures[target].position, &spell_vector);

    // we should also calculate the distance between the spellcaster and the attacker to know how fast the projectile should fly
    // we wrapped up distance calculations in its own function as well
    uint32_t distance = getDistance(&_this->CGdFigure->figures[source].position, &_this->CGdFigure->figures[target].position);

    // Flight timing restored verbatim from the original example (HEAD): owner
    // directive 2026-11-05 — donor-length reuse collapsed the flight (donor
    // finishes before reflection fires).

    // we don't need the distance itself, let's translate it into flight time with some numbers

    distance = ((distance & 0xffff) * 0x578) / 3000;

    uint16_t rfx_donor = effectAPI->getCurrentEffectIndex();

    // source and target have to be manually reversed in this case, because to make the new projectile damage the attacker
    // it should be sourced from the spellcaster and have attacker as its target
    // basically, it means that the clone's source is the spellcaster (handler's target)
    // and the clone's target is the attacker (handler's source)
    // flight start decoupled from the visual anchor: world-point source gives
    // Add an explicit start position (the reflector), while K12 in cloneEffect
    // rewrites the record's source pair to the DONOR figure for the client
    // template resolver (owner-approved mechanism, wave 16)
    SF_CGdTargetData clone_source;
    figureAPI->getPosition(_this->CGdFigure, &clone_source.position, target); // live position of the reflector (handler's target)
    clone_source.entity_type = 4; // world point: Add writes the position verbatim (flight start = reflector)
    // reflector figure id carrier — world-point index is ignored by Add, read by cloneEffect for the anchor hook
    clone_source.entity_index = target;
    SF_CGdTargetData clone_target = {1, source, {0, 0}};

    // we clone the incoming projectile via the framework cloneEffect helper
    // it copies the donor projectile's xdata (spell line, subspell, mana cost and so on),
    // so special arrows (like the fire arrow) keep their visuals and subspells in the clone
    auto rfx_tick_start = _this->OpaqueClass->current_step;
    uint32_t rfx_tick_count = ((distance != 0) ? distance : 1);
    uint16_t clone_id = effectAPI->cloneEffect(_this, rfx_donor, &clone_source, &clone_target,
                                               rfx_tick_start, rfx_tick_count, &spell_vector);

    // the engine silently refuses to allocate an effect slot when all 1999 slots are taken
    // in that case the reflection didn't happen and the victim keeps the incoming damage
    if (clone_id == 0)
    {
        logger->logWarning("ARROW REFLECTION FAILED: no free effect slot for the clone");
        return damage;
    }

    // the cloned projectile carries the full damage the attacker was going to deal
    effectAPI->setEffectXData(_this, clone_id, EFFECT_PHYSICAL_DAMAGE, damage);

    // Damage attribution (owner decision): the clone's landing damage is dealt by the attacker
    // (self-hit) — the server-side hook reads source from xdata ENTITY_INDEX. The clone's VISUAL
    // is independent of these keys: it comes from the record's source pair copied by cloneEffect
    // (K12), so the client template resolver uses the donor's weapon.
    // type/index mirror the attacker's pair used for clone_target above (entity_type 1, handler's source)
    effectAPI->setEffectXData(_this, clone_id, EFFECT_ENTITY_TYPE, 1);
    effectAPI->setEffectXData(_this, clone_id, EFFECT_ENTITY_INDEX, source);

    logger->logInfo("PROJECTILE IS MIRRORED");

    // the original projectile is negated, so the spellcaster takes no damage
    return 0;
}

void __thiscall arrows_reflection_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    logger->logInfo("ARROWS REFLECTION EFFECT HANDLER INTIALIZED");
    // we pull the pointer for this instance of spell
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];

    // we initiate ticks counter which is necessary to emulate spell duration
    uint32_t current_tick = spellAPI->getXData(_this, spell_index, SPELL_TICK_COUNT_AUX);

    // we increase amount of ticks passed by 1, ensuring that the next time effect handler is triggered, it will start with tick 1
    spellAPI->addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1);

    // we check the current tick
    // during tick 0 we will initialize the spell main logic if it's allowed with refresh handler
    if (current_tick == 0)
    {
        // we trigger the refresh handler to know whether the spell could be applied to the spellcaster
        // or whether it would be blocked with other instances of that
        if (spellAPI->checkCanApply(_this, spell_index))
        {
            // we load the spell parameters from GameData.cff
            // we'll need them only in tick 0
            SF_CGdResourceSpell spell_data;
            spellAPI->getResourceSpellData(_this->CGdResource, &spell_data, spell->spell_id);

            // we store index of the spellcaster
            uint16_t source_index = _this->active_spell_list[spell_index].source.entity_index;

            // we declare structure for relative position of visual effect
            SF_CGdTargetData relative_data;
            figureAPI->getPosition(_this->CGdFigure, &relative_data.position, source_index);
            relative_data.entity_type = 1;
            relative_data.entity_index = 0;
            uint32_t unused;

            SF_Rectangle aux_data;
            aux_data.partA = 0;
            aux_data.partB = 0;

            // we apply the visual effect to the target (requires editing lua scripts to make the continous visual effect as long as the spell is active)
            spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitTarget, &unused, &relative_data, _this->OpaqueClass->current_step, 0, &aux_data);

            // we disable the spell effect from being triggered until specified amount of time passes
            // param 0 = buff duration in ms; on/off mechanic without frequent ticks
            uint32_t ticks_interval = spell_data.params[0];
            _this->active_spell_list[spell_index].to_do_count = (uint16_t)((ticks_interval * 10) / 1000);

            // we have to activate the flag F_CHECK_SPELLS_BEFORE_JOB in order to make it possible for On Hit handler to trigger when the spellcaster makes an attack
            _this->CGdFigure->figures[source_index].flags |= F_CHECK_SPELLS_BEFORE_JOB;

            // we activate the flag CHECK_SPELLS_BEFORE_JOB2 for the sake of optimization
            _this->active_spell_list[spell_index].flags |= CHECK_SPELLS_BEFORE_JOB2;
        }
        else
        // the refresh handler indicated that the current instance of arrows_reflection can't be applied, because the target is already affected with the same spell
        {
            // in such case we finish the spell and remove it from the active spells list
            spellAPI->removeDLLNode(_this, spell_index);
            spellAPI->setEffectDone(_this, spell_index, 0);
        }
    }
    else
    // the current_tick is 1
    {
        // we clear the flags which we set up in tick 0
        spellAPI->figTryClrCHkSPlBfrJob2(_this, spell_index);
        spellAPI->figClrChkSplBfrChkBattle(_this, spell_index, 0);
        // we finish the spell and remove it from the active spells list
        spellAPI->removeDLLNode(_this, spell_index);
        spellAPI->setEffectDone(_this, spell_index, 0);
    }
}

// the Arrows Reflection is freely refreshable
// it means that when it's casted on a spellcaster again before its duration expired,
// it would remove the old instance and replace it with a new one, this would update the spell duration

int __thiscall arrows_reflection_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // we store spell pointer and source index for our convenience
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    uint16_t source_index = spell->source.entity_index;

    // we should check for whether the spellcaster is affected with more than a single instance of Arrows Reflection
    uint16_t pruned_spell_index = toolboxAPI->getSpellIndexOfType(_this->CGdFigureToolBox, source_index, ARROWS_REFLECTION_LINE, spell_index);
    // because we passed spell_index as last argument of the function, this index will be ignored
    // the function will return the spell index of another Arrows Reflection instance, if there is any
    // if there is no other instance of Arrows Reflection than the current, the function will return 0

    // if the function returned any spell index, we must prune that instance to make a space for the new one
    if (pruned_spell_index != 0)
    {
        logger->logInfo("REMOVE OLD ARROW REFLECTION");
        // first we should clear the flags which the old instance of Arrows Reflection set up
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
    iteratorAPI = sfsf->iteratorAPI;
    registrationAPI = sfsf->registrationAPI;
    effectAPI = sfsf->effectAPI;
    aiAPI = sfsf->aiAPI;
    logger = sfsf->logAPI;

    // we register handlers for custom spell
    SFSpell *arrows_reflection_spell = registrationAPI->registerSpell(ARROWS_REFLECTION_LINE);
    registrationAPI->linkTypeHandler(arrows_reflection_spell, &arrows_reflection_type_handler);
    registrationAPI->linkEffectHandler(arrows_reflection_spell, ARROWS_REFLECTION_JOB, &arrows_reflection_effect_handler);
    registrationAPI->linkRefreshHandler(arrows_reflection_spell, &arrows_reflection_refresh_handler);
    registrationAPI->linkPhysEffectHandler(arrows_reflection_spell, &arrows_reflection_phys_handler, PHASE_2);
    // to make the Phys Effect handler be triggered when the caster is hit, we also have to tag the spell as TARGET_ONHIT_SPELL
    registrationAPI->applySpellTag(arrows_reflection_spell, TARGET_ONHIT_SPELL);
    registrationAPI->linkEndHandler(arrows_reflection_spell, &arrows_reflection_end_handler);
}

/***
 * This function MUST be present in your code with the exact declaration
 * otherwise the framework is unable to describe your mod in logs and mod info menu (menu not yet implemented)
 ***/
extern "C" __declspec(dllexport) SFMod *RegisterMod(SpellforceSpellFramework *framework)
{
    return framework->createModInfo("Arrows Reflection", "1.1.0", "Teekius", "A mod designed to demonstrate defensive using of the On Hit spell handler. The spell will reflect all ranged attacks which hit the spellcaster back towards the attackers.");
}

// Required to be present by, not required for any functionality
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
