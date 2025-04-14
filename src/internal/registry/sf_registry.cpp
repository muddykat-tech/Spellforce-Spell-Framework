/**
 * @file sf_registry.cpp
 * @brief Initializes and configures the Spellforce Spell Framework (SFSF).
 *
 * This file handles the initialization of various systems required by the
 * Spellforce Spell Framework (SFSF), including setting up low-level hooks,
 * linking APIs, and loading and registering mods. It serves as the core point
 * for the framework to interact with the game, ensuring compatibility and
 * enabling various mod functionalities.
 *
 * @details
 * The initialization process includes several phases:
 * - **Framework Configuration**: Includes setting up API structures and initializing vanilla spells and handlers.
 * - **Mod Loading**: Loads all compatible mods into the framework.
 * - **Mod Registration**: Registers spells and functions from the loaded mods.
 *
 * @note Ensure that mods are compliant with the framework's API for compatibility.
 */

#include "../core/sf_modloader.h"
#include "../core/sf_wrappers.h"
#include "../core/sf_hooks.h"

#include "sf_registry.h"
#include "sf_vanilla_registry.h"
#include "sf_mod_registry.h"

#include "spell_data_registries/sf_spelltype_registry.h"
#include "spell_data_registries/sf_spelleffect_registry.h"
#include "spell_data_registries/sf_spellend_registry.h"
#include "spell_data_registries/sf_spellrefresh_registry.h"
#include "spell_data_registries/sf_subeffect_registry.h"
#include "spell_data_registries/sf_spelldamage_registry.h"

#include <windows.h>
#include <iostream>

// Exposed in sfsf.h
SpellforceSpellFramework frameworkAPI;
SFMod *g_current_mod;
SFMod *g_framework_mod;

/**
 * @brief Registers the framework API addresses.
 *
 * This function loads the framework's API structure and assigns function pointers
 * for various subsystems, including the figure, spell, toolbox, iterator, registration,
 * and effect APIs. It also sets up logging functionality.
 *
 * @see figureAPI, spellAPI, toolboxAPI, iteratorAPI, registrationAPI, effectAPI
 */
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
    g_framework_mod = createModInfo("Spellforce Spell Framework", "3.0.0-beta",
                                    "Muddykat, UnSchtalch and shovel_knight",
                                    "A Modding Framework to ease the creation of new Spells in the game Spellforce Platinum Edition.");
    g_current_mod = g_framework_mod;
}

/**
 * @brief Initializes the Spellforce Spell Framework (SFSF).
 *
 * This function initializes the Spellforce Spell Framework (SFSF), setting up hooks, APIs,
 * vanilla spells, and loading/registering mods.
 *
 * @details
 * The initialization process includes the following phases:
 *
 * - **Framework Configuration Phase**:
 *   - **Data Hook Initialization**: Sets up low-level hooks to the game's memory and functions.
 *   - **API Function Linking**: Links framework API structures.
 *   - **Vanilla Spell Initialization**: Registers vanilla spell types and effect handlers.
 *
 * - **Mod Loading Phase**:
 *   - Loads all mods compatible with the framework.
 *
 * - **Mod Registration Phase**:
 *   - Registers spells and functions from loaded mods.
 * @warning
 * Incorrect setup of hooks or APIs can lead to crashes.
 * Mods must adhere to the framework's API for compatibility.
 * @see registerFrameworkAPI(), initialize_data_hooks(), initialize_vanilla_spells(),
 * register_vanilla_effect_handlers(), register_vanilla_spell_end_handlers(),
 * register_vanilla_sub_effect_handlers(), register_vanilla_spell_refresh_handlers(),
 * register_vanilla_spell_damage_handlers(), initialize_mods(), register_mod_spells()
 */
void initialize_framework()
{

    log_info(
        "|=====| Spellforce Spell Framework Configuration Phase Start |=====|");

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

    log_info(
        "|======| Spellforce Spell Framework Configuration Phase End |======|");
    log_info(
        "|+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.|");
    log_info(
        "|====== ====== ======| Mod Loading Phase Start |====== ====== =====|");

    // Attempt to load all mods -> see sf_modloader.h
    initialize_mods();

    log_info(
        "|====== ====== ======| Mod Loading Phase End |====== ====== ======-|");
    log_info(
        "|+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.|");
    log_info(
        "|====== === ======| Mod Registration Phase Start |====== === ======|");

    // Now try and register these spells
    register_mod_spells();

    log_info(
        "|====== ==== ======| Mod Registration Phase End |====== ==== ======|");
    log_info(
        "|+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.|");
    log_info(
        "|=== === ===| Spellforce Spell Framework Working (^_^) |=== === ===|");
    log_info(
        "|======================| Injecting Assembly |======================|");
}
