// Already included in sf_registry, may be good to remove.
#include "../api/sfsf.h"
#include "../api/sf_data_utilities.h"

#include "sf_spelleffect_handlers.h"
#include "sf_registry.h"
#include "sf_spelltype_registry.h"
#include "sf_spelleffect_registry.h"
#include "sf_hooks.h"

#include "sf_modloader.h"
#include "sf_utility.h"
#include "sf_hooks.h"
#include <windows.h>

// Exposed in sfsf.h
SpellforceSpellFramework frameworkAPI;

// We can include pointers to our own functions we define.
// Note these functions are ASSIGNED to a function group when we DEFINE said group, names are VERY important.

// exposed in sf_registry.h
void registerFrameworkAPI(){
    frameworkAPI.apiFigureFunctions = &apiFigureFunctions;
    frameworkAPI.apiSpellFunctions = &apiSpellFunctions;
    frameworkAPI.apiToolboxFunctions = &apiToolboxFunctions;
    
	frameworkAPI.registerSpellTypeHandler = &registerSpellTypeHandler;
    frameworkAPI.registerEffectHandler = &registerEffectHandler;
    frameworkAPI.logWarning = &logWarning;
    frameworkAPI.logInfo = &logInfo;
}

void initFramework() {
    
	initDataHooks();

    // setup framework api structure references
    registerFrameworkAPI();
    
    // Setup Vanilla Spells -> see sf_spelltype_handler.h
    initSpellMap();

    // Setup effect job id registration -> see sf_spelleffect_handlers.h (This will MOVE to sf_spelleffect_registry.h)
    initEffectHandlers();

    // Attempt to load all mods -> see sf_modloader.h
    initMods();
}
