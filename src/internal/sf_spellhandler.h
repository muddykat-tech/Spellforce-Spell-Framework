#ifndef SPELLFORCE_SPELL_HANDLER_H
#define SPELLFORCE_SPELL_HANDLER_H

#include "../api/sfsf.h"
#include "../api/sf_spells.h"

extern void initSpellMap();
extern void __thiscall default_handler(SF_CGdSpell* spell, uint16_t spell_index);
extern void __thiscall initializeSpellData(SF_CGdSpell* _this, uint16_t spell_id, SpellDataKey key);

#endif 