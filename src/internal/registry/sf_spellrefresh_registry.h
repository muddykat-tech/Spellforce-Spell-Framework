#ifndef SF_SPELLREFRESH_REGISTRY_H
#define SF_SPELLREFRESH_REGISTRY_H

#include "../../api/sfsf.h"

extern handler_ptr get_spell_refresh(const uint16_t spell_job);
extern void registerSpellRefreshHandler(uint16_t spell_job, handler_ptr handler);
void __thiscall register_vanilla_spell_refresh_handlers();

#endif