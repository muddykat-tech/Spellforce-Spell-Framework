#ifndef MOD_REGISTRY_H
#define MOD_REGISTRY_H
#include "../../api/sfsf.h"
#include "../core/sf_modloader.h"
#include "../core/sf_wrappers.h"
#include "../core/sf_hooks.h"
#include <list>

extern std::list<SFSpell *> g_internal_spell_list;
extern std::list<SFBuilding *> g_internal_building_list;
void register_mod_spells();
void register_mod_buildings();

// TODO Clean this up, move to spell API
uint16_t __thiscall getSpellTags(uint16_t spell_line_id);
uint32_t __thiscall getBuildingTags(uint8_t building_type);


#endif // MOD_REGISTRY_H
