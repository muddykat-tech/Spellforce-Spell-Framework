/**
 * @addtogroup ScreensModule
 * @{
 */

#include <windows.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sf_screens_module.h"
#include "sf_screens_loader.h"
#include "sf_hooks.h"
#include "sf_wrappers.h"

#include "hooks/sf_campaign_hook.h" /* checkFileExists */

#include "../registry/sf_error_registry.h"
#include "../registry/sf_registry.h"

SFMod *g_screens_mod = NULL;

static ScreenEntry s_screen_entries[MAX_SCREEN_ENTRIES];
static int s_screen_entry_count = 0;

static void str_to_lower(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

static bool has_msb_extension(const char *name)
{
    size_t len = strlen(name);
    if (len < 4)
    {
        return false;
    }

    const char *ext = name + (len - 4);
    return (tolower((unsigned char)ext[0]) == '.' &&
            tolower((unsigned char)ext[1]) == 'm' &&
            tolower((unsigned char)ext[2]) == 's' &&
            tolower((unsigned char)ext[3]) == 'b');
}

/** @brief Asks the engine whether a loading screen mesh is present. */
static bool screen_mesh_exists(const char *msb_file)
{
    char mesh_path[MAX_PATH];
    snprintf(mesh_path, sizeof(mesh_path), "mesh\\%s", msb_file);

    SF_String path;
    uiAPI.SFStringConstructor_char(&path, mesh_path);
    bool exists = (checkFileExists(&path) != 0);
    uiAPI.SFStringDestructor(&path);

    if (!exists)
    {
        log_debug(DEBUG_MED, "| - Loading screen mesh not found: %s", mesh_path);
    }

    return exists;
}

/**
 * @brief Checks one parsed entry and copies it into the live table when sound.
 * @return true when the entry was accepted.
 */
static bool accept_screen_entry(const ScreenEntry *entry)
{
    if (entry->map_name[0] == '\0' || entry->msb_file[0] == '\0')
    {
        report_mod_error(g_screens_mod, "screens.json: entry with an empty map name or mesh name");
        return false;
    }

    if (!has_msb_extension(entry->msb_file))
    {
        report_mod_error(g_screens_mod,
                         "screens.json: \"%s\" maps to \"%s\", which is not a .msb mesh",
                         entry->map_name, entry->msb_file);
        return false;
    }

    for (int i = 0; i < s_screen_entry_count; i++)
    {
        if (strcmp(s_screen_entries[i].map_name, entry->map_name) == 0)
        {
            report_mod_error(g_screens_mod,
                             "screens.json: map \"%s\" is listed twice, keeping \"%s\"",
                             entry->map_name, s_screen_entries[i].msb_file);
            return false;
        }
    }

    if (!screen_mesh_exists(entry->msb_file))
    {
        report_mod_error(g_screens_mod,
                         "screens.json: map \"%s\" needs \"%s\", which is missing from mesh\\",
                         entry->map_name, entry->msb_file);
        return false;
    }

    s_screen_entries[s_screen_entry_count] = *entry;
    s_screen_entry_count++;
    return true;
}

void initialize_screens_module()
{
    char version_tag_buffer[128];
    snprintf(version_tag_buffer, sizeof(version_tag_buffer), "%d.%d.%d-%s",
             SPELLFRAMEWORK_VERSION_MAJOR, SPELLFRAMEWORK_VERSION_MINOR,
             SPELLFRAMEWORK_VERSION_PATCH, SPELLFRAMEWORK_TAG);

    g_screens_mod = createModInfo("Loading Screen Module", version_tag_buffer,
                                  "Muddykat, UnSchtalch",
                                  "Swaps in custom loading screens listed by the sfsf\\screens.json file.");
    clear_mod_errors(g_screens_mod);
    register_mod_for_listing(g_screens_mod, MOD_TYPE_CORE);

    s_screen_entry_count = 0;

    char currentDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDir);

    char path[MAX_PATH];
    snprintf(path, sizeof(path), "%s\\sfsf\\screens.json", currentDir);

    ScreenEntry parsed[MAX_SCREEN_ENTRIES];
    int parsed_count = 0;

    if (!parse_screens_json_file(path, parsed, MAX_SCREEN_ENTRIES, &parsed_count))
    {
        // A missing screens.json is a normal install, not a mod error.
        log_info("| - No screens.json found at %s (custom loading screens disabled)", path);
        return;
    }

    int rejected = 0;
    for (int i = 0; i < parsed_count; i++)
    {
        if (!accept_screen_entry(&parsed[i]))
        {
            rejected++;
        }
    }

    log_info("| - %d loading screen(s) registered, %d rejected", s_screen_entry_count, rejected);
}

const char *find_screen_for_map(const char *map_name)
{
    if (map_name == NULL || s_screen_entry_count == 0)
    {
        return NULL;
    }

    char lowered[SCREEN_MAP_NAME_LEN];
    size_t len = strlen(map_name);
    if (len >= sizeof(lowered))
    {
        len = sizeof(lowered) - 1;
    }
    memcpy(lowered, map_name, len);
    lowered[len] = '\0';
    str_to_lower(lowered);

    /* Strip the directory, then the extension. */
    const char *last_separator = strrchr(lowered, '\\');
    char *filename = (char *)(last_separator ? last_separator + 1 : lowered);

    char *dot = strrchr(filename, '.');
    if (dot != NULL)
    {
        *dot = '\0';
    }

    /* Vanilla maps are prefixed, e.g. "000_greyfell" or "p101_mirraw_thur".
     * Match on the part after the first underscore when there is one, so
     * screens.json can key on the readable name. */
    const char *underscore = strchr(filename, '_');
    const char *extracted = (underscore != NULL) ? underscore + 1 : filename;

    if (extracted[0] == '\0')
    {
        return NULL;
    }

    for (int i = 0; i < s_screen_entry_count; i++)
    {
        if (strcmp(s_screen_entries[i].map_name, extracted) == 0)
        {
            return s_screen_entries[i].msb_file;
        }
    }

    return NULL;
}

/** @} */
