#include "../../src/api/sfsf.h"

// NOTE sfsf.h includes the OTHER api files, but other libraries are still required
#include <windows.h>
#include <stdio.h>

#define rapid_fire_LINE 0xf2
#define RAPID_FIRE_JOB 0xaa

SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;
RegistrationFunctions *registrationAPI;
SFLog *logAPI;


void __thiscall rapid_fire_type_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    _this->active_spell_list[spell_index].spell_job = RAPID_FIRE_JOB;
    spellAPI->setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
    spellAPI->setXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER, 0);
}

void __thiscall rapid_fire_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    spellAPI->removeDLLNode(_this, spell_index);
    spellAPI->setEffectDone(_this, spell_index, 0);
}

void __thiscall rapid_fire_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    uint16_t source_index = spell->source.entity_index;
    SF_CGdResourceSpell spell_data;
    spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);
    spell->to_do_count = (spell_data.params[0] * 10) / 1000;
    uint32_t tick = spellAPI->addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1);
    if (tick == 1)
    {
        if (_this->SF_CGdFigure->figures[source_index].weapon_stats[1].max_rng > 2)
        {
            uint32_t wspeed = _this->SF_CGdFigure->figures[source_index].weapon_stats[1].wpn_spd;
            spellAPI->setXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER, wspeed);
            wspeed = (wspeed * (100 + spell_data.params[1])) / 100; //percentage increase
            _this->SF_CGdFigure->figures[source_index].weapon_stats[1].wpn_spd = wspeed;
        }
        return;
    }
    if (_this->SF_CGdFigure->figures[source_index].weapon_stats[1].max_rng > 2)
    {
        _this->SF_CGdFigure->figures[source_index].weapon_stats[1].wpn_spd = spellAPI->getXData(_this, spell_index,
                                                                                                SPELL_STAT_MUL_MODIFIER);
    }
    spellAPI->removeDLLNode(_this, spell_index);
    spellAPI->setEffectDone(_this, spell_index, 0);
}


/***
 * This function MUST be present in your code with the exact declaration
 * otherwise framework won't load your plugin
 ***/

extern "C" __declspec(dllexport) void InitModule(SpellforceSpellFramework *framework)
{
    sfsf = framework;
    spellAPI = sfsf->spellAPI;
    toolboxAPI = sfsf->toolboxAPI;
    figureAPI = sfsf->figureAPI;
    registrationAPI = sfsf->registrationAPI;
    logAPI = sfsf->logAPI;

    SFSpell *rapid_fire_spell = registrationAPI->registerSpell(rapid_fire_LINE);
    registrationAPI->linkTypeHandler(rapid_fire_spell, &rapid_fire_type_handler);
    registrationAPI->linkEffectHandler(rapid_fire_spell, RAPID_FIRE_JOB, &rapid_fire_effect_handler);
    registrationAPI->linkEndHandler(rapid_fire_spell, &rapid_fire_end_handler);
}


/***
 * This function MUST be present in your code with the exact declaration
 * otherwise the framework is unable to describe your mod in logs and mod info menu (menu not yet implemented)
 ***/

extern "C" __declspec(dllexport) SFMod *RegisterMod(SpellforceSpellFramework *framework)
{
    return framework->createModInfo("The custom spell rapid_fire", "1.0.0", "Muddykat, UnSchtalch, Teekius",
                                    "Weapon stat modification test");
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
