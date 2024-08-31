#include "../../api/sfsf.h"
#include <windows.h>
#include <stdio.h>

#define SUMMON_SPELL_LINE 0x14
#define SUMMON_SPELL_JOB 0x17

SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;
IteratorFunctions *iteratorAPI;
RegistrationFunctions *registrationAPI;
SFLog *logger;

uint16_t summonCreature(SF_CGdFigureToolbox _this, uint16_t master_index, uint16_t creature_type)
{
    
}

void __thiscall summon_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    // we pull the pointer for this instance of spell
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);
    uint32_t tick_number = spellAPI->getXData(_this, spell_index, SPELL_TICK_COUNT_AUX);
    if (tick_number == 0)
    {
        uint16_t target_index = summonCreature(_this->SF_CGdFigureToolBox, spell->source.entity_index, spell_data.params[2]);
    }
    else
    {

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

    SFSpell *summon_spell = registrationAPI->registerSpell(SUMMON_SPELL_LINE);                  // just pull any summoning spell
    registrationAPI->linkEffectHandler(summon_spell, SUMMON_SPELL_JOB, &summon_effect_handler); // and overwrite it's effect handler which is common
}

/***
 * This function MUST be present in your code with the exact declaration
 * otherwise the framework is unable to describe your mod in logs and mod info menu (menu not yet implemented)
 ***/
extern "C" __declspec(dllexport) SFMod *RegisterMod(SpellforceSpellFramework *framework)
{
    return framework->createModInfo("Controlled Summons mod", "1.0.0", "Un'Schtalch", "Done specifically for General Kong");
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
