#ifndef SF_SPELLEND_REGISTRY_H
#define SF_SPELLEND_REGISTRY_H

#include "../../../api/sfsf.h"

extern handler_ptr get_spell_end(const uint16_t spell_job);
extern void registerSpellEndHandler(uint16_t spell_job, handler_ptr handler);
void __thiscall register_vanilla_spell_end_handlers();

#endif
