#include "../.../src/api/sfsf.h"

// NOTE sfsf.h includes the OTHER api files, but other libraries are still required
#include <windows.h>
#include <stdio.h>

#define TEMPLATE_LINE 0xf2
#define TEMPLATE_JOB 0xaa

SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;
RegistrationFunctions *registrationAPI;


void __thiscall template_type_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
  _this->active_spell_list[spell_index].spell_job = TEMPLATE_JOB;
}

void __thiscall template_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
  spellAPI->removeDLLNode(_this, spell_index);
  spellAPI->setEffectDone(_this, spell_index, 0);
}

void __thiscall template_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
  SF_GdSpell *spell = &_this->active_spell_list[spell_index];

  SF_CGdResourceSpell spell_data;
  spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);

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

  SFSpell *template_spell = registrationAPI->registerSpell(TEMPLATE_LINE);
  registrationAPI->linkTypeHandler(template_spell, &template_type_handler);
  registrationAPI->linkEffectHandler(template_spell, TEMPLATE_JOB, &template_effect_handler);
  registrationAPI->linkEndHandler(template_spell, &template_end_handler);
}


/***
 * This function MUST be present in your code with the exact declaration
 * otherwise the framework is unable to describe your mod in logs and mod info menu (menu not yet implemented)
 ***/

extern "C" __declspec(dllexport) SFMod *RegisterMod(SpellforceSpellFramework *framework)
{
  return framework->createModInfo("The custom spell template", "1.0.0", "Muddykat, UnSchtalch, Teekius", "This template provides the core functions and dependencies necessary to create a custom spell using the Spellforce Framework.");
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
