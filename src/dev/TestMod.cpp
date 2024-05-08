#include <windows.h>
#include "../api/sf_data_utilities.h"
#include "../api/sfsf.h"

SpellforceSpellFramework *sfsf;
cgdspellfunctions *spellAPI;

// Spell index is the ID for the TYPE of spell being cast
// Spell Job is the ID for the LOGIC (effect) handler that the spell uses when being cast.

void __thiscall icestrike_handler(SF_CGdSpell * _this, uint16_t spell_index) {
	_this->active_spell_list[spell_index].spell_job = 0xa7;

  
	spellAPI->initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	spellAPI->initializeSpellData(_this, spell_index, SPELL_TICK_COUNT);
	spellAPI->initializeSpellData(_this, spell_index, SPELL_DOUBLE_DAMAGE);

  sfsf->logInfo("Spell Handled");
}

void __thiscall custom_effect(SF_CGdSpell * _this, uint16_t spell_index) {
  sfsf->logInfo("Custom Effect Handled");
  spellAPI->addToXDataList(_this->SF_CGdXDataList, spell_index, SPELL_TICK_COUNT, 1);
  // uint32_t damage = 2;
  // SF_GdSpell spell = _this->active_spell_list[(spell_index + -1)]; // May need to decrement by one?
  // uint8_t *datapointer = &_this->active_spell_list[(int)(spell_index + -1)].xdata_key;
  // 0x33 is TARGET Data Key from the looks of it
  // SF_CGdTargetData target = spell.target;//datapointer[SPELL_TARGET];
  // SF_CGdTargetData source = spell.source;// local_bc + 0x5f; // Tied to Random for some reason in effect_fireball2? And in effect_firebullet is using current calc

  // spellAPI->dealDamage(_this->SF_CGdFigureToolBox, source, target, damage, 1, 0, 0);
  // Last Param for effect done should always be 0?
  spellAPI->setEffectDone(_this, spell_index, 0);
}

extern "C" __declspec(dllexport) void InitModule(SpellforceSpellFramework* framework) {
	  sfsf = framework;
    spellAPI = sfsf->pCGdSpellFunctions;

    sfsf->registerSpellTypeHandler(0xe, &icestrike_handler);
    sfsf->registerSpellTypeHandler(0xeb, &icestrike_handler);
    sfsf->registerEffectHandler(0xa7, &custom_effect);
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