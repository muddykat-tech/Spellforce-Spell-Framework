#include "../core/sf_modloader.h"
#include "../core/sf_wrappers.h"
#include "../core/sf_hooks.h"

#include "sf_registry.h"
#include "sf_spelltype_registry.h"
#include "sf_spelleffect_registry.h"
#include "sf_spellend_registry.h"
#include "sf_spellrefresh_registry.h"
#include "sf_mod_registry.h"
#include "sf_subeffect_registry.h"
#include "sf_vanilla_registry.h"
#include "sf_spelldamage_registry.h"

#include <windows.h>
#include <iostream>

// Exposed in sfsf.
SpellforceSpellFramework frameworkAPI;
SFMod *g_current_mod;
SFMod *g_framework_mod;

// We can include pointers to our own functions we define.
// Note these functions are ASSIGNED to a function group when we DEFINE said group, names are VERY important.

// exposed in sf_registry.h
void registerFrameworkAPI()
{
    log_info("| - Loading framework with API Addresses");
    frameworkAPI.figureAPI = &figureAPI;
    frameworkAPI.spellAPI = &spellAPI;
    frameworkAPI.toolboxAPI = &toolboxAPI;
    frameworkAPI.iteratorAPI = &iteratorAPI;
    frameworkAPI.registrationAPI = &registrationAPI;
    frameworkAPI.logAPI = setup_logger();
    log_info("| - Loading framework with create_mod_info Address");
    frameworkAPI.createModInfo = &createModInfo;
    frameworkAPI.effectAPI = &effectAPI;

    log_info("| - Loading Default Mod Information");
    g_framework_mod = createModInfo("SFSF", "v1.0.2-beta", "Muddykat, UnSchtalch and shovel_knight", "The Spellforce Spell Framework");
    g_current_mod = g_framework_mod;
}

void initialize_framework()
{

    log_info("|=====| Spellforce Spell Framework Configuration Phase Start |=====|");

    log_info("| - Initializing Data Hooks");

    initialize_data_hooks();

    log_info("| - Linking API functions");

    // setup framework api structure references
    registerFrameworkAPI();

    log_info("| - Initialization of Vanilla Spells");

    initialize_vanilla_spells();

    log_info("| - Effect Handlers");

    register_vanilla_effect_handlers();

    log_info("| - Registration of Vanilla Spell End Handlers");

    register_vanilla_spell_end_handlers();

    log_info("| - Registration of Vanilla Sub Effect Handlers");

    register_vanilla_sub_effect_handlers();

    log_info("| - Registration of Vanilla Spell Refresh Handlers");

    register_vanilla_spell_refresh_handlers();

    log_info("| - Registration of Vanilla Spell Deal Damage Handlers");

    register_vanilla_spell_damage_handlers();

    log_info("|======| Spellforce Spell Framework Configuration Phase End |======|");
    log_info("|+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.|");
    log_info("|====== ====== ======| Mod Loading Phase Start |====== ====== =====|");

    // Attempt to load all mods -> see sf_modloader.h
    initialize_mods();

    log_info("|====== ====== ======| Mod Loading Phase End |====== ====== ======-|");
    log_info("|+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.|");
    log_info("|====== === ======| Mod Registration Phase Start |====== === ======|");

    // Now try and register these spells
    register_mod_spells();

    log_info("|====== ==== ======| Mod Registration Phase End |====== ==== ======|");
    log_info("|+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.|");
    log_info("|=== === ===| Spellforce Spell Framework Working (^_^) |=== === ===|");
    log_info("|======================| Injecting Assembly |======================|");
}
