#include ".../.../api/sfsf.h"
#include <windows.h>
#include <stdio.h>

// We declare macros for Arrows reflection Spell Type and Spell Job
// Spell Type 0xf8 = 248, Spell Job 0xaf = 175

// The custom Spell Type also must be defined within GameData.cff
// and provided with at least one spell corresponding to it

#define ARROWS_REFLECTION_LINE 0xf8
#define ARROWS_REFLECTION_JOB 0xaf

SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;
IteratorFunctions *iteratorAPI;
RegistrationFunctions *registrationAPI;
EffectFunctions *effectAPI;
SFLog *logger;

// we declare spell type handler for Arrows reflection, it must initialize ticks and bonus modifier values

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

SF_Rectangle get_spell_vector (SF_Coord* source_position, SF_Coord* target_position, SF_Rectangle* spell_vector)
{
                /*uint16_t maxY = (_this->CGdFigure->figures[source_index].position.Y <= _this->CGdFigure->figures[target.entity_index].position.Y)
                                    ? (_this->CGdFigure->figures[target.entity_index].position.Y)
                                    : (_this->CGdFigure->figures[source_index].position.Y);

                uint16_t maxX = (_this->CGdFigure->figures[source_index].position.X <= _this->CGdFigure->figures[target.entity_index].position.X)
                                    ? (_this->CGdFigure->figures[target.entity_index].position.X)
                                    : (_this->CGdFigure->figures[source_index].position.X);

                uint16_t minY = (_this->CGdFigure->figures[source_index].position.Y > _this->CGdFigure->figures[target.entity_index].position.Y)
                                    ? (_this->CGdFigure->figures[target.entity_index].position.Y)
                                    : (_this->CGdFigure->figures[source_index].position.Y);

                uint16_t minX = (_this->CGdFigure->figures[source_index].position.X > _this->CGdFigure->figures[target.entity_index].position.X)
                                    ? (_this->CGdFigure->figures[target.entity_index].position.X)
                                    : (_this->CGdFigure->figures[source_index].position.X);*/

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

                SF_Coord p1;
                SF_Coord p2;
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
                spell_vector->partA = p1.Y << 0x10 | p1.X;
                spell_vector->partB = p2.Y << 0x10 | p2.X;

                return *spell_vector;
}


// result of engine reverse-engineering, all credits to UnSchtalch/Muddykat for bringing this function to life
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

// we declare On Hit Handler
// this handler will be triggered whenever a spellcaster makes an attack with the weapon against an enemy
// the handler intercepts the damage dealt which will be dealt with an attack to the target and allows us to modify it according to our own rules
// the game engine will continue processing the value which we return to it from the point of interception
// we determine the moment when the spell will be intercept as part of mod initialization when we link the spell with the On Hit Handler

uint16_t __thiscall arrows_reflection_onhit_handler(SF_CGdFigureJobs *_this, uint16_t source_index, uint16_t target_index, uint16_t damage)
{
    logger->logInfo("ARROW REFLECTION WAS TRIGGERED");

    // first of all, we should check for dead-ends such as zero damage or automatical oneshot provided by Critical Strikes spell

    // 0x7fff is an amount which Critical Strikes uses to turn damage into oneshot
    // instead of checking for spell effect lingering over spellcaster, we can know that with doing damage comparison
    // also, in case there would be other oneshotting spells, our algorithm would automatically hook them on without the need to modify it
    if (damage == 0 || damage == 0x7fff)
    {
        return damage;
    }

    // we declare structure to save the spellcaster's action in it
    // we'll need it to determine whether the attack was ranged or melee
    SF_SGtFigureAction action;

    // we determine spellcaster's action which triggered the On Hit Handler with figureAPI function
    figureAPI->getTargetAction(_this->CGdFigure, &action, source_index);

    // Pass though is WIP, so we inline the function here
    // we initialize boolean value which would save the result of our check
    bool isRangedAttack = 0;

    if ((action.type == 0x2712)) // have to fix == into !=
    // number 0x2712 stands for ranged attack, if the attack wasn't ranged, we interrupt the function
    {
        return damage;
    }
    else
    // if the attack was ranged attack, we reflected the projectile into the attacker and completely negate damage
    {
    // unlike most of handlers, On_Hit handler uses Figure Jobs instead of Spell as global object
    // it means, we have to manually get spell index of Arrows reflection
    uint16_t spell_index = toolboxAPI->getSpellIndexOfType(_this->CGdFigureToolBox, source_index, ARROWS_REFLECTION_LINE, 0);

    // we retrieve the spell which exists behind given On Hit handler
    SF_GdSpell *spell = &_this->CGdSpell->active_spell_list[spell_index];

    // we declare effect_info structure, because we'll need it later for function which retrieves target's resistance to ice
    SF_SpellEffectInfo effect_info;

    // we store spell id and spell job into effect_info structure
    effect_info.spell_id = spell->spell_id;
    effect_info.job_id = spell->spell_job;

    // we declare structure for spell data and load it from GameData.cff
    SF_CGdResourceSpell spell_data;
    spellAPI->getResourceSpellData(_this->CGdResource, &spell_data, effect_info.spell_id);

    SF_Rectangle spell_vector;
    get_spell_vector(&_this->CGdFigure->figures[source_index].position, &_this->CGdFigure->figures[target_index].position, &spell_vector);

    uint32_t distance = getDistance(&_this->CGdFigure->figures[source_index].position, &_this->CGdFigure->figures[target_index].position);
    distance = ((distance & 0xffff) * 0x578) / 3000;
    SF_CGdTargetData source = {1, source_index, {0, 0}};
    SF_CGdTargetData target = {1, target_index, {0, 0}};
    uint16_t effect_id = effectAPI->addEffect(_this->CGdEffect, kGdEffectProjectile, &source, &target,
                                                         _this->OpaqueClass->current_step, ((distance != 0) ? distance : 1), &spell_vector);
    effectAPI->setEffectXData(_this->CGdEffect, effect_id, EFFECT_ENTITY_TYPE, 1);
    effectAPI->setEffectXData(_this->CGdEffect, effect_id, EFFECT_ENTITY_TYPE2, 1);
    effectAPI->setEffectXData(_this->CGdEffect, effect_id, EFFECT_ENTITY_INDEX, source_index);
    effectAPI->setEffectXData(_this->CGdEffect, effect_id, EFFECT_ENTITY_INDEX2, target.entity_index);
    effectAPI->setEffectXData(_this->CGdEffect, effect_id, EFFECT_DO_NOT_ADD_SUBSPELL, 1);
    effectAPI->setEffectXData(_this->CGdEffect, effect_id, EFFECT_PHYSICAL_DAMAGE, damage);

    logger->logInfo("ATTACK WAS REFLECTED");
    // we negate damage to the spellcaster
    return 0;
    }

}

void __thiscall arrows_reflection_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
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
            spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);

            // we store index of the spellcaster
            uint16_t source_index = _this->active_spell_list[spell_index].source.entity_index;

            // we declare structure for relative position of visual effect
            SF_CGdTargetData relative_data;
            figureAPI->getPosition(_this->SF_CGdFigure, &relative_data.position, source_index);
            relative_data.entity_type = 4;
            relative_data.entity_index = 0;
            uint32_t unused;

            SF_Rectangle aux_data;
            aux_data.partA = 0;
            aux_data.partB = 0;

            // we apply the visual effect to the target (requires editing lua scripts to make the continous visual effect as long as the spell is active)
            spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitTarget, &unused, &relative_data, _this->OpaqueClass->current_step, 0, &aux_data);

            // we disable the spell effect from being triggered until specified amount of time passes
            uint16_t ticks_interval = spell_data.params[1];
            _this->active_spell_list[spell_index].to_do_count = (uint16_t)((ticks_interval * 10) / 1000);

            // we have to activate the flag F_CHECK_SPELLS_BEFORE_JOB in order to make it possible for On Hit handler to trigger when the spellcaster makes an attack
            _this->SF_CGdFigure->figures[source_index].flags |= F_CHECK_SPELLS_BEFORE_JOB;

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

// the Arrows reflection is freely refreshable
// it means that when it's casted on a spellcaster again before its duration expired,
// it would remove the old instance and replace it with a new one, this would update the spell duration

int __thiscall arrows_reflection_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // we store spell pointer and source index for our convenience
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    uint16_t source_index = spell->source.entity_index;

    // we should check for whether the spellcaster is affected with more than a single instance of Arrows reflection
    uint16_t pruned_spell_index = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox, source_index, ARROWS_REFLECTION_LINE, spell_index);
    // because we passed spell_index as last argument of the function, this index will be ignored
    // the function will return the spell index of another Arrows reflection instance, if there is any
    // if there is no other instance of Arrows reflection than the current, the function will return 0

    // if the function returned any spell index, we must prune that instance to make a space for the new one
    if (pruned_spell_index != 0)
    {
        // first we should clear the flags which the old instance of Arrows reflection set up
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
    logger = sfsf->logAPI;

    // we register handlers for custom spell
    SFSpell *arrows_reflection_spell = registrationAPI->registerSpell(ARROWS_REFLECTION_LINE);
    registrationAPI->linkTypeHandler(arrows_reflection_spell, &arrows_reflection_type_handler);
    registrationAPI->linkEffectHandler(arrows_reflection_spell, ARROWS_REFLECTION_JOB, &arrows_reflection_effect_handler);
    registrationAPI->linkRefreshHandler(arrows_reflection_spell, &arrows_reflection_refresh_handler);
    registrationAPI->linkOnHitHandler(arrows_reflection_spell, &arrows_reflection_onhit_handler, PHASE_4);
    // the last argument can take values from PHASE_0 to PHASE_5
    // sadly the algorithm is too big to be explained in commentaries, but take my word that Phases 4 and 5 will fit the spell logic the best
    registrationAPI->applySpellTag(arrows_reflection_spell, TARGET_ONHIT_SPELL);
    registrationAPI->linkEndHandler(arrows_reflection_spell, &arrows_reflection_end_handler);
}

/***
 * This function MUST be present in your code with the exact declaration
 * otherwise the framework is unable to describe your mod in logs and mod info menu (menu not yet implemented)
 ***/
extern "C" __declspec(dllexport) SFMod *RegisterMod(SpellforceSpellFramework *framework)
{
    return framework->createModInfo("Arrows reflection", "1.0.0", "Teekius", "A mod designed to demonstrate defensive using of the On Hit spell handler. The spell will reflect all ranged attacks which hit the spellcaster back towards the attackers.");
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
