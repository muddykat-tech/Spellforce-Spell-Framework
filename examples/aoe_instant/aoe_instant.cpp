#include "../api/sfsf.h"
#include "../api/sf_data_utilities.h"
#include <windows.h>
#include <stdio.h>
//For convenience only; You can put headers just near the file and remove ../api/

//Another convenience: you can ommit functions you won't need, or call everything from the framework structure
SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;
IteratorFunctions *iteratorAPI;

void __thiscall aoe_lifetap_type_handler(SF_CGdSpell * _this, uint16_t spell_index) 
{
	_this->active_spell_list[spell_index].spell_job = 0xf2; 

}

typedef struct
{
    uint32_t partA;
    uint32_t partB;
} effect_rectangle;

void __thiscall aoe_lifetap_effect_handler(SF_CGdSpell * _this, uint16_t spell_index)
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    //Iterators are opaque from the user perspective. 
    //Just give enough memory and don't bother what's inside
    uint8_t iterator_memory[0x44];
    iteratorAPI->figureIteratorInit(&iterator_memory, 0x0, 0x0, 0x3ff, 0x3ff);
    iteratorAPI->figureIteratorSetPointers(&iterator_memory, _this->SF_CGdFigure, _this->unkn3, _this->SF_CGdWorld);
    SF_CGdResourceSpell spell_data;
    SF_SpellEffectInfo effect_info;
    effect_rectangle hit_area;
    spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, effect_info.spell_id);


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
    iteratorAPI = sfsf->apiIteratorFunctions;
    //Here comes the spell type registration. 
    //Note: you need to pass pointer to function here
    sfsf->registerSpellTypeHandler(242, &aoe_lifetap_type_handler);


    //Here comes the spell logic (effect) registration; f2 = 242
    sfsf->registerEffectHandler(0xf2, &aoe_lifetap_effect_handler);

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