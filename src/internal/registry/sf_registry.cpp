#include "../core/sf_modloader.h"
#include "../core/sf_wrappers.h"
#include "../core/sf_hooks.h"

#include "sf_registry.h"
#include "sf_spelltype_registry.h"
#include "sf_spelleffect_registry.h"
#include "sf_spellend_registry.h"
#include <windows.h>

// Exposed in sfsf.h
SpellforceSpellFramework frameworkAPI;

// We can include pointers to our own functions we define.
// Note these functions are ASSIGNED to a function group when we DEFINE said group, names are VERY important.

// exposed in sf_registry.h
void registerFrameworkAPI()
{
    frameworkAPI.figureAPI = &figureAPI;
    frameworkAPI.spellAPI = &spellAPI;
    frameworkAPI.toolboxAPI = &toolboxAPI;
    frameworkAPI.iteratorAPI = &iteratorAPI;
	frameworkAPI.registerSpellTypeHandler = &registerSpellTypeHandler;
    frameworkAPI.registerEffectHandler = &register_effect_handler;
    frameworkAPI.logWarning = &log_warning;
    frameworkAPI.logInfo = &log_info;
}

void initialize_framework() 
{
    log_info("Initializing Data Hooks");

	initialize_data_hooks();

    log_info("Linking API functions");

    // setup framework api structure references
    registerFrameworkAPI();
    
    log_info("Registration of Vanilla Spelltypes");

    // Setup Vanilla Spells -> see sf_spelltype_handler.h
    initSpellMap();

    log_info("Registration of Vanilla Effect Handlers");

    register_vanilla_effect_handlers();

    log_info("Registration of Vanilla Spell End Handlers");

    register_vanilla_spell_end_handlers();
    
    log_info("Initializing Mods");

    // Attempt to load all mods -> see sf_modloader.h
    initialize_mods();
}
