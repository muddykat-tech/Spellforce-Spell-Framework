#include <windows.h>
#include "../api/sf_data_utilities.h"
#include "../api/sfsf.h"

#include <cstdio>

SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;

// This custom spell type and custom spell effect need to be setup manually in the GameData.cff file currently

// Spell index is the ID for the TYPE of spell being cast
// Spell Job is the ID for the LOGIC (effect) handler that the spell uses when being cast.
void __thiscall custom_spelltype_handler(SF_CGdSpell * _this, uint16_t spell_index) {
  //Effect ID is the spell_job (links to custom_spelleffect_handler)
	_this->active_spell_list[spell_index].spell_job = 0xf2; 

  // Required for the spell to be initialized as active
	spellAPI->initializeSpellData(_this, spell_index, SPELL_TICK_COUNT);

  sfsf->logInfo("Spell Handled");
}

void __thiscall custom_spelleffect_handler(SF_CGdSpell * _this, uint16_t spell_index) {
  sfsf->logInfo("Custom Effect Handled");
  // Required for the spell to eventually become Inactive, without this and setEffectDone, you can't attack the same target again.
  spellAPI->addToXDataList(_this->SF_CGdXDataList, spell_index, SPELL_TICK_COUNT, 1);

  uint32_t damage = 20;
  sfsf->logInfo("Grab Spell from list");
  SF_GdSpell *spell = &_this->active_spell_list[spell_index];
  
  sfsf->logInfo("Setup Spell Target Info");
  uint16_t target_index = spell->target.entity_index;
  uint16_t source_index = spell->source.entity_index;
  
  sfsf->logInfo("Setup Spell Effect Info");
  SF_SpellEffectInfo effect_info;
  effect_info.spell_id = spell->spell_id;
  effect_info.job_id = spell->spell_job;

  sfsf->logInfo("Get Resist");
  uint32_t resist_chance = spellAPI->getChanceToResistSpell(_this->unkn2, source_index, target_index, effect_info);
  char resInfo[256];
  sprintf(resInfo, "Resist:  %d\n", resist_chance);
  sfsf->logInfo(resInfo);

  sfsf->logInfo("Get Random");
  uint16_t random_roll = spellAPI->getRandom(_this->OpaqueClass, 100);

  char ranInfo[256];
  sprintf(ranInfo, "Random:  %d\n", random_roll);
  sfsf->logInfo(ranInfo);

  sfsf->logInfo("Get isAlive Target");
  uint16_t isAlive = figureAPI->isAlive(_this->SF_CGdFigure, target_index);

  char aliveInfo[256];
  sprintf(aliveInfo, "isAlive:  %d\n", isAlive);
  sfsf->logInfo(aliveInfo);

  sfsf->logInfo("Change Walk Speed of Caster to 200");
  figureAPI->setWalkSpeed(_this->SF_CGdFigure, source_index, 200);

  sfsf->logInfo("Added to the Wisdom Bonus Mult by 2"); // Unconfirmed if working as in tended, does NOT cause a crash though...
  figureAPI->addBonusMultToStatistic(_this->SF_CGdFigure, WISDOM, source_index, 2);


  if(resist_chance < random_roll) {
    sfsf->logInfo("Deal Damage");
    toolboxAPI->dealDamage(_this->SF_CGdFigureToolBox, source_index, target_index, damage, 1, 0, 0);
  }

  // Last Param for spell effect done should always be 0?
  spellAPI->setEffectDone(_this, spell_index, 0);
}

extern "C" __declspec(dllexport) void InitModule(SpellforceSpellFramework* framework) {
	  sfsf = framework;
    spellAPI = sfsf->apiSpellFunctions;
    toolboxAPI = sfsf->apiToolboxFunctions;
    figureAPI = sfsf->apiFigureFunctions;

    // This will OVERWRITE existing entries, so you can fix or modify vanilla spelltypes and effects
    // 0xe and 0xeb are the Icestrike or Iceburst Spell
    sfsf->registerSpellTypeHandler(0xe, &custom_spelltype_handler);
    sfsf->registerSpellTypeHandler(0xeb, &custom_spelltype_handler);
    sfsf->registerEffectHandler(0xf2, &custom_spelleffect_handler);
}


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