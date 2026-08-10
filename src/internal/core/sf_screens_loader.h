/**
 * @defgroup ScreensLoader Loading Screen JSON Loader
 * @ingroup Core
 * @brief Parses screens.json into ScreenEntry records.
 *
 * Parsing only - discovery, validation and lookup belong to the Loading Screen
 * Module (sf_screens_module.h), which owns the entries this fills in.
 *
 * @addtogroup ScreensLoader
 * @{
 */

#ifndef SCREENS_LOADER_H
#define SCREENS_LOADER_H

#include <stdbool.h>
#include <stdint.h>

#define SCREEN_MAP_NAME_LEN 128
#define SCREEN_MSB_NAME_LEN 128

/** @brief One "map name -> loading screen mesh" pair. */
typedef struct
{
    char map_name[SCREEN_MAP_NAME_LEN]; /**< Lower-cased map name, no path or extension. */
    char msb_file[SCREEN_MSB_NAME_LEN]; /**< Mesh file to show while that map loads. */
} ScreenEntry;

/**
 * @brief Reads a screens.json: a single object of map_name : msb_file pairs.
 *
 * @param path        Full path to the file.
 * @param out_entries Caller owned array, filled up to @p max_entries.
 * @param max_entries Capacity of @p out_entries.
 * @param out_count   Number of entries written.
 * @return false when the file is unreadable or is not a flat JSON object.
 */
bool parse_screens_json_file(const char *path, ScreenEntry *out_entries,
                             int max_entries, int *out_count);

/** @} */
#endif // SCREENS_LOADER_H
