#ifndef MOD_REGISTRY_H
#define MOD_REGISTRY_H
#include "../../api/sfsf.h"
#include "../core/sf_modloader.h"
#include "../core/sf_wrappers.h"
#include "../core/sf_hooks.h"
#include <list>

extern std::list<SFSpell *> g_internal_spell_list;
extern void register_mod_spells();

#endif // MOD_REGISTRY_H