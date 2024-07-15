#ifndef SF_SPELLTYPE_REGISTRY_H
#define SF_SPELLTYPE_REGISTRY_H

#include <stdint.h>
#include "../../api/sfsf.h"

extern void __thiscall initializeSpellData(SF_CGdSpell *_this, uint16_t spell_id, SpellDataKey key);
extern handler_ptr get_spell_handler(const uint16_t key);
extern void registerSpellTypeHandler(uint16_t spell_index, handler_ptr handler);

#endif