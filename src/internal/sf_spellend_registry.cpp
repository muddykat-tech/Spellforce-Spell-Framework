#include "sf_spellend_registry.h"
#include "sf_hooks.h"

//default case
void __thiscall default_end_handler(SF_CGdSpell* _this, uint16_t spell_index)
{
    apiSpellFunctions.removeDLLNode(_this, spell_index);
    apiSpellFunctions.setEffectDone(_this, spell_index, 0);
}

//Stat changing handlers

void __thiscall slowness_end_handler(SF_CGdSpell* _this, uint16_t spell_index)
{
    
}
void __thiscall decay1_end_handler (SF_CGdSpell* _this, uint16_t spell_index);
void __thiscall inflexibility_end_handler (SF_CGdSpell* _this, uint16_t spell_index);
void __thiscall weaken_end_handler (SF_CGdSpell* _this, uint16_t spell_index);
void __thiscall quickness_end_handler (SF_CGdSpell* _this, uint16_t spell_index);
void __thiscall flexibility_end_handler (SF_CGdSpell* _this, uint16_t spell_index);
void __thiscall strength_end_handler (SF_CGdSpell* _this, uint16_t spell_index);
void __thiscall retention_end_handler (SF_CGdSpell* _this, uint16_t spell_index);
void __thiscall brilliance_end_handler (SF_CGdSpell* _this, uint16_t spell_index);
void __thiscall suffocation_end_handler (SF_CGdSpell* _this, uint16_t spell_index);
void __thiscall inablility_end_handler (SF_CGdSpell* _this, uint16_t spell_index);
void __thiscall slow_fighting_end_handler (SF_CGdSpell* _this, uint16_t spell_index);
void __thiscall dexterity_end_handler (SF_CGdSpell* _this, uint16_t spell_index);
void __thiscall edurance_end_handler (SF_CGdSpell* _this, uint16_t spell_index);
void __thiscall fast_fighting_end_handler (SF_CGdSpell* _this, uint16_t spell_index);
void __thiscall charisma_end_handler (SF_CGdSpell* _this, uint16_t spell_index);
void __thiscall enlightenment_end_handler (SF_CGdSpell* _this, uint16_t spell_index);
void __thiscall melt_resistance_end_handler (SF_CGdSpell* _this, uint16_t spell_index);
void __thiscall chill_resistance_end_handler (SF_CGdSpell* _this, uint16_t spell_index);
void __thiscall firebane_end_handler (SF_CGdSpell* _this, uint16_t spell_index);
void __thiscall black_almightness_end_handler (SF_CGdSpell* _this, uint16_t spell_index);
void __thiscall mutation_end_handler (SF_CGdSpell* _this, uint16_t spell_index);
void __thiscall eternity_end_handler (SF_CGdSpell* _this, uint16_t spell_index);

