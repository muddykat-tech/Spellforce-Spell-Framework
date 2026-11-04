/**
 * @addtogroup HeroModule
 * @{
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sf_hero_module.h"
#include "sf_hero_loader.h"
#include "../registry/sf_hero_registry.h"
#include "sf_hooks.h"
#include "sf_wrappers.h"

#include "../registry/sf_error_registry.h"
#include "../registry/sf_registry.h"

SFMod *g_hero_mod = NULL;

/**
 * @brief Validates the gear of a parsed hero and compacts it in place.
 * @return the number of valid gear entries.
 */
static uint8_t accept_hero_gear(CustomHero *hero, const char *source)
{
    uint8_t valid_count = 0;
    for (uint8_t i = 0; i < hero->item_count; i++)
    {
        if (hero->gear[i].slot > 6)
        {
            report_mod_error(g_hero_mod, "Hero [%s]: gear entry #%u has slot %u outside 0-6",
                             source, i, hero->gear[i].slot);
            continue;
        }
        if (hero->gear[i].item_id == 0)
        {
            report_mod_error(g_hero_mod, "Hero [%s]: gear entry #%u has item_id 0",
                             source, i);
            continue;
        }
        hero->gear[valid_count] = hero->gear[i];
        valid_count++;
    }
    return valid_count;
}

void initialize_hero_module()
{
    char version_tag_buffer[128];
    snprintf(version_tag_buffer, sizeof(version_tag_buffer), "%d.%d.%d-%s",
             SPELLFRAMEWORK_VERSION_MAJOR, SPELLFRAMEWORK_VERSION_MINOR,
             SPELLFRAMEWORK_VERSION_PATCH, SPELLFRAMEWORK_TAG);

    g_hero_mod = createModInfo("Custom Hero Module", version_tag_buffer,
                               "Muddykat, UnSchtalch",
                               "Loads custom heroes defined by JSON files in the sfsf\\heroes folder.");
    clear_mod_errors(g_hero_mod);
    register_mod_for_listing(g_hero_mod, MOD_TYPE_CORE);

    uint32_t loaded = 0;
    uint32_t skipped = 0;

    char currentDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDir);

    char heroDirectory[MAX_PATH];
    snprintf(heroDirectory, sizeof(heroDirectory), "%s\\sfsf\\heroes", currentDir);

    char searchPath[MAX_PATH];
    snprintf(searchPath, sizeof(searchPath), "%s\\*.json", heroDirectory);

    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(searchPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        // No folder and no heroes is a perfectly normal install.
        log_info("| - No hero files found in %s (custom heroes disabled)", heroDirectory);
        return;
    }

    do
    {
        if ((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
        {
            continue;
        }

        char heroPath[MAX_PATH];
        snprintf(heroPath, sizeof(heroPath), "%s\\%s", heroDirectory, findFileData.cFileName);

        log_info("| - Reading hero definition [%s]", findFileData.cFileName);

        CustomHero hero;
        if (!parse_hero_json_file(heroPath, &hero))
        {
            report_mod_error(g_hero_mod, "Hero [%s] could not be parsed",
                             findFileData.cFileName);
            skipped++;
            continue;
        }

        if (hero.creo_id == 0)
        {
            report_mod_error(g_hero_mod,
                             "Hero [%s]: missing or invalid \"creo_id\"", findFileData.cFileName);
            skipped++;
            continue;
        }

        uint8_t gear_count = accept_hero_gear(&hero, findFileData.cFileName);
        if (gear_count == 0)
        {
            /* Advisory only - an empty gear list is legal and clears the gear of
             * an already registered (e.g. vanilla) hero. */
            report_mod_warning(g_hero_mod, "Hero [%s] contains no gear entries",
                               findFileData.cFileName);
        }

        registerHeroGearFromSource(hero.creo_id, hero.gear, gear_count, findFileData.cFileName);
        loaded++;
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);

    log_info("| - %u hero(es) registered, %u skipped", loaded, skipped);
}

/** @} */
