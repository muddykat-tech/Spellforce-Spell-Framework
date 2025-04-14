#ifndef MOD_REGISTRY_H
#define MOD_REGISTRY_H
#include "../../api/sfsf.h"
#include "../core/sf_modloader.h"
#include "../core/sf_wrappers.h"
#include "../core/sf_hooks.h"
#include <list>

extern std::list<SFSpell *> g_internal_spell_list;
extern void register_mod_spells();

// TODO Clean this up, move to spell API
uint16_t __thiscall getSpellTags(uint16_t spell_line_id);

#endif // MOD_REGISTRY_H
