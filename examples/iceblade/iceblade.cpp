#include "../../src/api/sfsf.h"
#include <windows.h>
#include <stdio.h>

// We declare macros for Iceblade Spell Type and Spell Job
// Spell Type 0xf7 = 247, Spell Job 0xae = 174

// The custom Spell Type also must be defined within GameData.cff
// and provided with at least one spell corresponding to it

#define ICEBLADE_LINE 0xf7
#define ICEBLADE_JOB 0xae

SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;
IteratorFunctions *iteratorAPI;
RegistrationFunctions *registrationAPI;
SFLog *logger;

// we declare spell type handler for Iceblade, it must initialize ticks and bonus modifier values

void __thiscall iceblade_type_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // we associate spell type with a spell job
    _this->active_spell_list[spell_index].spell_job = ICEBLADE_JOB;
    // we initialize the ticks counter
    spellAPI->setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

// we declare spell end handler
// this handler would work in case a spell wasn't finished correctly
void __thiscall iceblade_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    spellAPI->figTryClrCHkSPlBfrJob2(_this, spell_index);
    spellAPI->figClrChkSplBfrChkBattle(_this, spell_index, 0);
    spellAPI->removeDLLNode(_this, spell_index);
    spellAPI->setEffectDone(_this, spell_index, 0);
}

// we declare On Hit Handler
// this handler will be triggered whenever a spellcaster makes an attack with the weapon against an enemy
// the handler intercepts the damage dealt which will be dealt with an attack to the target and allows us to modify it according to our own rules
// the game engine will continue processing the value which we return to it from the point of interception
// we determine the moment when the spell will be intercept as part of mod initialization when we link the spell with the On Hit Handler

uint16_t __thiscall iceblade_onhit_handler(SF_CGdFigureJobs *_this, uint16_t source_index, uint16_t target_index, uint16_t damage)
{

    // first of all, we should check for dead-ends such as zero damage or automatical oneshot provided by Critical Hits spell

    // 0x7fff is an amount which Critical Hits uses to turn damage into oneshot
    // instead of checking for spell effect lingering over spellcaster, we can know that with doing damage comparison
    // also, in case there would be other oneshotting spells, our algorithm would automatically hook them on without the need to modify it
    if (damage == 0 || damage == 0x7fff)
    {
        return damage;
    }

    // unlike most of handlers, On_Hit handler uses Figure Jobs instead of Spell as global object
    // it means, we have to manually get spell index of Iceblade
    uint16_t spell_index = toolboxAPI->getSpellIndexOfType(_this->CGdFigureToolBox, source_index, ICEBLADE_LINE, 0);

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

    // we declare structure to save the spellcaster's action in it
    // we'll need it to determine whether the attack was ranged or melee
    SF_SGtFigureAction action;

    // we determine spellcaster's action which triggered the On Hit Handler with figureAPI function
    figureAPI->getTargetAction(_this->CGdFigure, &action, source_index);

    // Pass though is WIP, so we inline the function here
    // we initialize boolean value which would save the result of our check
    bool isMeleeAttack = 0;
    // numbers 10000 or 0x2711 stand for melee, if action type matches either of these numbers, it means that the On Hit Handler was triggered with a melee attack
    if ((action.type == 10000) || (action.type == 0x2711))
    {
        isMeleeAttack = 1;
    }

    if (isMeleeAttack)
    {
        // we learn target's resistance to Ice magic
        // getChanceToResistSpell automatically determines the type of magic using effect info as the reference
        uint16_t frost_resistance = spellAPI->getChanceToResistSpell(_this->CGdSpell->unkn2, source_index, target_index, effect_info);

        // we convert the specified percentage into ice damage
        uint16_t ice_damage = uint16_t(damage * spell_data.params[0] / 100);

        // we subtract the convereted damage from total damage
        // we will return the reduced physical damage in the end of the handler's logic, and game engine will process it according to usual rules
        // in case the percentage is higher than 100, we should make sure it won't reduce the physical damage to negative value
        if (damage >= ice_damage)
        {
            damage -= ice_damage;
        }
        else
        {
            damage = 0;
        }

        // we modify ice damage with target's ice resistance
        ice_damage -= uint16_t((ice_damage * frost_resistance) / 100);

        // we deal ice damage to a target directly within On Hit Handler, we can't deliver it in other way
        toolboxAPI->dealDamage(_this->CGdFigureToolBox, source_index, target_index, ice_damage, 1, 0, 0);
    }

    // we should return damage regardless of whether it was modified or not
    return damage;
}

void __thiscall iceblade_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
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

            // we have to activate the flag F_CHECK_SPELLS_BEFORE_JOB in order to make it possible for On Hit handler to trigger when the spellcaster makes an attack
            _this->SF_CGdFigure->figures[source_index].flags |= F_CHECK_SPELLS_BEFORE_JOB;

            // we activate the flag CHECK_SPELLS_BEFORE_JOB2 for the sake of optimization
            _this->active_spell_list[spell_index].flags |= CHECK_SPELLS_BEFORE_JOB2;
            
            // we disable the spell effect from being triggered until specified amount of time passes
            uint16_t ticks_interval = spell_data.params[1];
            _this->active_spell_list[spell_index].to_do_count = (uint16_t)((ticks_interval * 10) / 1000);
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

// the Iceblade is freely refreshable
// it means that when it's casted on a spellcaster again before its duration expired,
// it would remove the old instance and replace it with a new one, this would update the spell duration

int __thiscall iceblade_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // we store spell pointer and source index for our convenience
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    uint16_t source_index = spell->source.entity_index;

    // we should check for whether the spellcaster is affected with more than a single instance of Iceblade
    uint16_t pruned_spell_index = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox, source_index, ICEBLADE_LINE, spell_index);
    // because we passed spell_index as last argument of the function, this index will be ignored
    // the function will return the spell index of another Iceblade instance, if there is any
    // if there is no other instance of Iceblade than the current, the function will return 0

    // if the function returned any spell index, we must prune that instance to make a space for the new one
    if (pruned_spell_index != 0)
    {
        // first we should clear the flags which the old instance of Iceblade set up 
        spellAPI->figTryClrCHkSPlBfrJob2(_this, pruned_spell_index);
        spellAPI->figClrChkSplBfrChkBattle(_this, pruned_spell_index, 0);
        // then we should end the previous instance
        spellAPI->removeDLLNode(_this, pruned_spell_index);
        spellAPI->setEffectDone(_this, pruned_spell_index, 0);
    }

    // we return true indicating that the new instance can be applied to the target
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
    logger = sfsf->logAPI;

    // we register handlers for custom spell
    SFSpell *iceblade_spell = registrationAPI->registerSpell(ICEBLADE_LINE);
    registrationAPI->linkTypeHandler(iceblade_spell, &iceblade_type_handler);
    registrationAPI->linkEffectHandler(iceblade_spell, ICEBLADE_JOB, &iceblade_effect_handler);
    registrationAPI->linkRefreshHandler(iceblade_spell, &iceblade_refresh_handler);
    registrationAPI->linkOnHitHandler(iceblade_spell, &iceblade_onhit_handler, PHASE_4);
    // the last argument can take values from PHASE_0 to PHASE_5
    // sadly the algorithm is too big to be explained in commentaries, but take my word that Phases 4 and 5 will fit the spell logic the best
    registrationAPI->linkEndHandler(iceblade_spell, &iceblade_end_handler);
}

/***
 * This function MUST be present in your code with the exact declaration
 * otherwise the framework is unable to describe your mod in logs and mod info menu (menu not yet implemented)
 ***/
extern "C" __declspec(dllexport) SFMod *RegisterMod(SpellforceSpellFramework *framework)
{
    return framework->createModInfo("Iceblade", "1.0.0", "Teekius", "A mod designed to demonstrate On Hit spell handler. The Iceblade spell converts a percentage of damage dealt with melee attack into ice damage.");
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
