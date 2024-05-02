#include "../api/sfsf.h"

// sfsf has this included
#include "../api/sf_spells"

#include "sf_registry.h"
#include "sf_spellhandler.h"
#include "sf_modloader.h"

// See sf_spells.h
cgdspellfunctions CGdSpellFunctions;

// Exposed in sfsf.h
static SpellforceSpellFramework frameworkAPI;

// exposed in sf_registry.h
void initFramework() {
    // setup framework api structure references
    registerFrameworkAPI();
    
    // Setup Vanilla Spells -> see sf_spellhandler.h
    initSpellMap();
    
    // Attempt to load all mods -> see sf_modloader.h
    initMods();
}

void registerFrameworkAPI(){     
	CGdSpellFunctions.setXData = (setXData_ptr) ASI::AddrOf(0x329C40);
	frameworkAPI.pCGdSpellFunctions = &CGdSpellFunctions;
	frameworkAPI.addSpellHandler = &addSpellHandler;
}