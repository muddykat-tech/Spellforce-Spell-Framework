// Already included in sf_registry, may be good to remove.
#include "../api/sfsf.h"
#include "../api/sf_data_utilities.h"

#include "sf_registry.h"
#include "sf_spelltype_registry.h"
#include "sf_modloader.h"
#include "sf_utility.h"
#include <windows.h>

// See sf_data_utilities.h
cgdspellfunctions CGdSpellFunctions;

// Exposed in sfsf.h
SpellforceSpellFramework frameworkAPI;

// exposed in sf_registry.h
void registerFrameworkAPI(){     
	CGdSpellFunctions.setXData = (setXData_ptr) ASI::AddrOf(0x329C40);
    CGdSpellFunctions.initializeSpellData = &initializeSpellData;
	frameworkAPI.pCGdSpellFunctions = &CGdSpellFunctions;
	frameworkAPI.registerSpellTypeHandler = &registerSpellTypeHandler;
    frameworkAPI.logWarning = &logWarning;
    frameworkAPI.logInfo = &logInfo;
}

void initFramework() {

    // setup framework api structure references
    registerFrameworkAPI();
    
    // Setup Vanilla Spells -> see sf_spellhandler.h
    initSpellMap();
    
    // Attempt to load all mods -> see sf_modloader.h
    initMods();

    
}
