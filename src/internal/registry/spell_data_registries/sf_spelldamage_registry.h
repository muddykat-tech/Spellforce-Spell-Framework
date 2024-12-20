#ifndef SF_SPELLDAMAGE_REGISTRY_H
#define SF_SPELLDAMAGE_REGISTRY_H

#include "../../../api/sfsf.h"

extern damage_handler_ptr get_spell_damage(const uint16_t spell_line_id, SpellDamagePhase phase);
void registerSpellDamageHandler(uint16_t spell_line_id, damage_handler_ptr handler, SpellDamagePhase phase);
void __thiscall register_vanilla_spell_damage_handlers();

#endif