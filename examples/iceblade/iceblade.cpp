#include "../../src/api/sfsf.h"
#include <windows.h>
#include <stdio.h>

// this example shows another way to implement the shieldwall spell
// it implements shieldwall via single aoe spell instead of combination of two spells (AoE and individual)
// such implementation is closer to vanilla implementation of AoE buffs

// We declare macros for Iceblade Spell Type and Spell Job
// Spell Type 0xf8 = 248, Spell Job 0xaf = 175

// The custom Spell Type also must be defined within GameData.cff
// and provided with at least one spell corresponding to it

#define ICEBLADE_LINE 0xf8
#define ICEBLADE_JOB 0xaf


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

uint16_t __thiscall iceblade_onhit_handler(SF_CGdFigureJobs *_this, uint16_t source_index, uint16_t target_index, uint16_t damage)
{
    // 0x7fff is an amount of damage which critical hits ability deals to as part of instant kill logic
    // instead of checking for spell effect, we can determine it with damage comparison
    // such implementation would also automatically handle other abilities which work as critical hits, in case we make any
    // if there was no damage or damage was supposed to insta-kill, we interrupt the handler, because further calculations will be made in vain

    if (damage == 0 || damage == 0x7fff)
    {
        return damage;
    }
    uint16_t spell_index = toolboxAPI->getSpellIndexOfType(_this->CGdFigureToolBox, source_index, ICEBLADE_LINE, 0);

    SF_GdSpell *spell = &_this->CGdSpell->active_spell_list[spell_index];

    char aliveInfo[256];
    sprintf(aliveInfo, "Full damage is: %hd \n", damage);
    logger->logInfo(aliveInfo);

    SF_SpellEffectInfo effect_info;
    SF_CGdResourceSpell spell_data;

    effect_info.spell_id = spell->spell_id;
    effect_info.job_id = spell->spell_job;

    spellAPI->getResourceSpellData(_this->CGdResource, &spell_data, effect_info.spell_id);

    SF_SGtFigureAction action;
    figureAPI->getTargetAction(_this->CGdFigure, &action, source_index);

    // Pass though is WIP, so we inline the function here
    bool isMeleeAttack = 0;
    if ((action.type == 10000) || (action.type == 0x2711))
    {
        isMeleeAttack = 1;
    }

    if (isMeleeAttack)
    {
        uint16_t frost_resistance = spellAPI->getChanceToResistSpell(_this->CGdSpell->unkn2, source_index, target_index, effect_info);

        uint16_t ice_damage = uint16_t((damage * spell_data.params[0]) / 100);

        if (damage >= ice_damage)
        {
            damage -= ice_damage;
        }
        else
        {
            damage = 0;
        }

        ice_damage -= uint16_t((ice_damage * frost_resistance) / 100);

        toolboxAPI->dealDamage(_this->CGdFigureToolBox, source_index, target_index, ice_damage, 1, 0, 0);
        /*
                char aliveInfo[256];
                sprintf(aliveInfo, "Physical damage is: %hd \n", damage);
                logger->logInfo(aliveInfo);

                sprintf(aliveInfo, "Ice damage is: %hd \n", ice_damage);
                logger->logInfo(aliveInfo);
        */
    }

    return damage;
}

void __thiscall iceblade_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // we pull the pointer for this instance of spell
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];

    // we initiate ticks counter which is necessary to emulate spell duration
    uint32_t current_tick = spellAPI->getXData(_this, spell_index, SPELL_TICK_COUNT_AUX);

    // we increase amount of ticks passed by 1
    spellAPI->addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1);

    if (current_tick == 0 && spellAPI->checkCanApply(_this, spell_index))
    {
        // we load the spell parameters from GameData.cff
        // we'll need them only in tick 0
        // we will access armor bonus directly with spell XData in tick 1
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

        // we apply the visual effect filling the area which we specified above
        spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitTarget, &unused, &relative_data, _this->OpaqueClass->current_step, 0, &aux_data);

        // we disable the spell effect from being triggered until specified amount of time passes
        uint16_t ticks_interval = spell_data.params[1];
        _this->active_spell_list[spell_index].to_do_count = (uint16_t)((ticks_interval * 10) / 1000);

        // we have to activate the flag F_CHECK_SPELLS_BEFORE_JOB in order to make it possible for Deal Damage handler to trigger when the damage is received
        _this->SF_CGdFigure->figures[source_index].flags |= F_CHECK_SPELLS_BEFORE_JOB;

        // we activate the flag F_CHECK_SPELLS_BEFORE_BATTLE for the sake of optimization
        _this->active_spell_list[spell_index].flags |= 2;
    }
    else
    // current_tick == 1 or spellCheckCanApply == 0, either way, the spell has come to end
    {
        // we finish the spell and remove it from the active spells list
        spellAPI->figTryClrCHkSPlBfrJob2(_this, spell_index);
        spellAPI->figClrChkSplBfrChkBattle(_this, spell_index, 0);
        spellAPI->removeDLLNode(_this, spell_index);
        spellAPI->setEffectDone(_this, spell_index, 0);
    }
}

int __thiscall iceblade_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index) // we casted shieldwall universal again before the previous expired
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    // we declare target index with value which we stored into ICEBLADE spell above
    uint16_t target_index = spell->target.entity_index;

    // we check whether the figure has the Iceblade applied to it already

    uint16_t spell_index_current = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox, target_index, ICEBLADE_LINE, spell_index);

    if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, ICEBLADE_LINE) && spell_index_current != 0)
    // the ICEBLADE spell already exists over the target and target isn't the spellcaster
    // we return 0, because the spell can't be applied to target at this moment
    {
        return 0;
    }
    else
    // the target isn't affected by the ICEBLADE spell, we allow to apply the spell effect to target
    {
        return 1;
    }
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
    registrationAPI->linkEndHandler(iceblade_spell, &iceblade_end_handler);
}

/***
 * This function MUST be present in your code with the exact declaration
 * otherwise the framework is unable to describe your mod in logs and mod info menu (menu not yet implemented)
 ***/
extern "C" __declspec(dllexport) SFMod *RegisterMod(SpellforceSpellFramework *framework)
{
    return framework->createModInfo("Iceblade", "1.0.0", "Teekius", "A mod designed to demonstrate On Hit Spell Handler. The Iceblade spell transforms a percentage of damage done with melee attack into frost damage.");
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
