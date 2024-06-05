#include <windows.h>
#include "../api/sfsf.h"
#include "../api/sf_general_structures.h"
// NOTE sfsf.h includes the OTHER api files, the other files are still required

#include <cstdio>
#include <string.h>

SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;
SFLog *logger;
// This custom spell type and custom spell effect need to be setup manually in the GameData.cff file currently

// Spell index is the ID for the TYPE of spell being cast
// Spell Job is the ID for the LOGIC (effect) handler that the spell uses when being cast.
void __thiscall custom_spelltype_handler(SF_CGdSpell * _this, uint16_t spell_index) {
  //Effect ID is the spell_job (links to custom_spelleffect_handler)
	_this->active_spell_list[spell_index].spell_job = 0xf2; 

  // Required for the spell to be initialized as active
	spellAPI->initializeSpellData(_this, spell_index, SPELL_TICK_COUNT);

  logger->logInfo("Spell Handled");
  logger->logWarning("Test Warning");
}

void __thiscall custom_spellend_handler(SF_CGdSpell * _this, uint16_t spell_index){
  logger->logInfo("END EFFECT HANDLED");
  spellAPI->spellClearFigureFlag(_this, spell_index, UNFREEZE);
  spellAPI->removeDLLNode(_this, spell_index);
  spellAPI->setEffectDone(_this, spell_index, 0);
}

void __thiscall custom_spelleffect_handler(SF_CGdSpell * _this, uint16_t spell_index) {
  logger->logInfo("Custom Effect Handled");
  // Required for the spell to eventually become Inactive, without this and setEffectDone, you can't attack the same target again.
  logger->logInfo("Grab Spell from list");
  SF_GdSpell *spell = &_this->active_spell_list[spell_index];
  uint8_t xdata_key = spell->xdata_key;
  uint8_t tick_count = spellAPI->getXData(_this, xdata_key, SPELL_TICK_COUNT);

  spellAPI->addToXData(_this, spell_index, SPELL_TICK_COUNT, 100);

  uint32_t damage = 1;

  uint16_t target_index = spell->target.entity_index;
  uint16_t source_index = spell->source.entity_index;

  SF_SpellEffectInfo effect_info;
  effect_info.spell_id = spell->spell_id;
  effect_info.job_id = spell->spell_job;

  uint32_t resist_chance = spellAPI->getChanceToResistSpell(_this->unkn2, source_index, target_index, effect_info);

  uint16_t random_roll = spellAPI->getRandom(_this->OpaqueClass, 100);

  uint16_t isAlive = figureAPI->isAlive(_this->SF_CGdFigure, target_index);

  figureAPI->setWalkSpeed(_this->SF_CGdFigure, source_index, 200);

  figureAPI->addBonusMultToStatistic(_this->SF_CGdFigure, WISDOM, source_index, 2);


  if(resist_chance < random_roll) {
    toolboxAPI->dealDamage(_this->SF_CGdFigureToolBox, source_index, target_index, damage, 1, 0, 0);
    return;
  }

  // Last Param for spell effect done should always be 0?
  if(tick_count <= 1) 
  spellAPI->setEffectDone(_this, spell_index, 0);
}

extern "C" __declspec(dllexport) void InitModule(SpellforceSpellFramework* framework) {
	  sfsf = framework;
    spellAPI = sfsf->spellAPI;
    toolboxAPI = sfsf->toolboxAPI;
    figureAPI = sfsf->figureAPI;
    logger = sfsf->logAPI;

    // This will OVERWRITE existing entries, so you can fix or modify vanilla spelltypes and effects
    // 0xe and 0xeb are the Icestrike or Iceburst Spell
    sfsf->registerSpellTypeHandler(0xe, &custom_spelltype_handler);
    sfsf->registerSpellTypeHandler(0xeb, &custom_spelltype_handler);
    sfsf->registerEffectHandler(0xf2, &custom_spelleffect_handler);
    sfsf->registerSpellEndHandler(0xe, &custom_spellend_handler);
}

extern "C" __declspec(dllexport) SFMod RegisterMod(SpellforceSpellFramework* framework) {
    return framework->createModInfo("Test Mod", "1.0.0", "Muddykat, UnSchtalch", "A mod designed to test in development functions exposed through the SFSF API");
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