#include "../handlers/sf_spelltype_handlers.h"
#include "../core/sf_hooks.h"
#include "../core/sf_wrappers.h"
#include "../core/sf_modloader.h"

#include "sf_spelltype_registry.h"
#include "sf_registry.h"
#include <map>
#include <cstdio>

void __thiscall initializeSpellData(SF_CGdSpell *_this, uint16_t spell_id, SpellDataKey key)
{
	spellAPI.setXData(_this, spell_id, key, 0);
}

std::map<uint16_t, handler_ptr> handler_map;

handler_ptr get_spell_handler(const uint16_t key)
{
	auto it = handler_map.find(key);
	if (it == handler_map.end())
	{
		// Element doesn't exist, insert the default value
		it = handler_map.emplace(key, &default_handler).first;
	}
	return it->second;
}

// Exposed in sf_registry.h
void registerSpellTypeHandler(uint16_t spell_index, handler_ptr handler)
{
	auto check = handler_map.find(spell_index);
	if (check != handler_map.end())
	{
		if (check->second != &default_handler)
		{
			char message[256]; // Assuming a maximum message length of 255 characters
			sprintf(message, "%s (v%s) has replaced a Spelltype Handler! [%d] (Was this on purpose?)", g_current_mod->mod_id, g_current_mod->mod_version, spell_index);
			log_warning(message);
		}
	}

	handler_map[spell_index] = handler;
}