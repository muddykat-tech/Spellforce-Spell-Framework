// Already included in sf_registry, may be good to remove.
#include "../api/sfsf.h"
#include "../api/sf_spells.h"
//

#include "sf_registry.h"
#include "sf_spellhandler.h"
#include "sf_modloader.h"
#include <stdint.h>
#include "../api/sfsf.h"
#include <map>

// See sf_spells.h
cgdspellfunctions CGdSpellFunctions;

// Exposed in sfsf.h
SpellforceSpellFramework frameworkAPI;

static std::map<uint16_t, handler_ptr> handler_map;

handler_ptr get_or_default (std::map<uint16_t, handler_ptr> m, const uint16_t key)
{
	auto it = m.find(key);
    if (it == m.end()) {
        // Element doesn't exist, insert the default value
        it = m.emplace(key, &default_handler).first;
    }
    return it->second;
}

// Exposed in sf_registry.h
handler_ptr get_spell_handler (const uint16_t key) {
	return get_or_default(handler_map, key);
}

// Exposed in sf_registry.h
void addSpellHandler(uint16_t spell_index, handler_ptr handler) {
	handler_map[spell_index] = handler;
}

// exposed in sf_registry.h
void registerFrameworkAPI(){     
	CGdSpellFunctions.setXData = (setXData_ptr) ASI::AddrOf(0x329C40);
	frameworkAPI.pCGdSpellFunctions = &CGdSpellFunctions;
	frameworkAPI.addSpellHandler = &addSpellHandler;
}

void initFramework() {
    // setup framework api structure references
    registerFrameworkAPI();
    
    // Setup Vanilla Spells -> see sf_spellhandler.h
    initSpellMap();
    
    // Attempt to load all mods -> see sf_modloader.h
    initMods();
}
