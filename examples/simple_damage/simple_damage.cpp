#include "../api/sfsf.h"
#include "../api/sf_data_utilities.h"
#include <windows.h>
//For convenience only; You can put headers just near the file and remove ../api/

//Another convenience: you can ommit functions you won't need, or call everything from the framework structure
SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;


void __thiscall simple_damage_handler(SF_CGdSpell * _this, uint16_t spell_index) 
{
	_this->active_spell_list[spell_index].spell_job = 0xf2; 

}

void __thiscall custom_spelleffect_handler(SF_CGdSpell * _this, uint16_t spell_index) 
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    uint16_t target_index = spell->target.entity_index;
    uint16_t source_index = spell->source.entity_index;
    SF_SpellEffectInfo effect_info;
    SF_CGdResourceSpell spell_data;
    effect_info.spell_id = spell->spell_id;
    effect_info.job_id = spell->spell_job;
    //calculate properly, what are we feeding there?
    spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data,effect_info.spell_id);

    uint16_t random_roll = spellAPI->getRandom(_this->OpaqueClass, 100);
    uint32_t resist_chance = spellAPI->getChanceToResistSpell(_this->unkn2, source_index, target_index, effect_info);

    spellAPI->setEffectDone(_this, spell_index, 0);
}

/***
 * This function MUST be present in your code with the exact declaration
 * otherwise framework won't load your plugin
 ***/
extern "C" __declspec(dllexport) void InitModule(SpellforceSpellFramework* framework) 
{
	sfsf = framework;
    spellAPI = sfsf->apiSpellFunctions;
    toolboxAPI = sfsf->apiToolboxFunctions;
    figureAPI = sfsf->apiFigureFunctions;

    //Here comes the spell type registration. 
    //Note: you need to pass pointer to function here
    sfsf->registerSpellTypeHandler(242, &simple_damage_handler);


    //Here comes the spell logic (effect) registration

}


//Required to be present, not required for any functionality
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