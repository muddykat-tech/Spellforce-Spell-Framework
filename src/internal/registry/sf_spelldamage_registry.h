#ifndef SF_SPELLDAMAGE_REGISTRY_H
#define SF_SPELLDAMAGE_REGISTRY_H

#include "../../api/sfsf.h"

extern damage_handler_ptr get_spell_damage(const uint16_t spell_job, SpellDamagePhase phase);
extern void registerSpellDamageHandler(uint16_t spell_job, handler_ptr handler, SpellDamagePhase phase);
void __thiscall register_vanilla_spell_damage_handlers();

#endif