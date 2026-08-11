/**
 * @defgroup ScreensModule SFSF Loading Screen Module
 * @ingroup Core
 * @brief Core mod that maps custom maps to custom loading screen meshes.
 *
 * Reads sfsf\screens.json, checks that every mesh it names is actually present
 * under mesh\, and answers lookups from the loading screen hook. Owning its
 * own SFMod means a mistyped mesh name shows up in the in-game mod list rather
 * than silently falling back to the vanilla screen.
 *
 * @addtogroup ScreensModule
 * @{
 */

#ifndef SCREENS_MODULE_H
#define SCREENS_MODULE_H

#include "../../api/sfsf.h"

/** Upper bound on custom loading screens. */
#define MAX_SCREEN_ENTRIES 128

/** The core mod that owns every JSON-defined loading screen. */
extern SFMod *g_screens_mod;

/**
 * @brief Loads and validates sfsf\screens.json.
 *
 * Safe to call when the file is absent - custom loading screens are simply
 * disabled. Must run before the first map load.
 */
void initialize_screens_module();

/**
 * @brief Finds the loading screen mesh for a map, or NULL for the default.
 *
 * @param map_name Map path as the engine has it, e.g. "map\\Campaign\\000_Greyfell.map".
 *                 Path, extension and any leading "NNN_" prefix are stripped
 *                 before matching, so "000_Greyfell.map" matches "greyfell".
 */
const char *find_screen_for_map(const char *map_name);

/** @} */
#endif // SCREENS_MODULE_H
