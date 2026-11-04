/**
 * @addtogroup AbilityModule
 * @{
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sf_ability_module.h"
#include "sf_ability_loader.h"
#include "../registry/sf_level_up_registry.h"
#include "sf_hooks.h"
#include "sf_wrappers.h"

#include "../registry/sf_error_registry.h"
#include "../registry/sf_registry.h"

SFMod *g_ability_mod = NULL;

/**
 * @brief Checks one parsed record and registers it when sound.
 * @return true when the record was accepted.
 */
static bool accept_ability_record(const AbilityJsonRecord *record, const char *source)
{
    if (!record->found_skill || !record->found_spec || !record->found_level ||
        !record->found_ability_id)
    {
        report_mod_error(g_ability_mod,
                         "Ability [%s]: record is missing a required field (skill, spec, level, ability_id)",
                         source);
        return false;
    }

    if (record->skill < 0 || record->skill > 255 || record->spec < 0 || record->spec > 255)
    {
        report_mod_error(g_ability_mod,
                         "Ability [%s]: skill/spec %d/%d outside 0-255",
                         source, record->skill, record->spec);
        return false;
    }

    if (record->level < 1 || record->level > 255)
    {
        report_mod_error(g_ability_mod, "Ability [%s]: level %d outside 1-255",
                         source, record->level);
        return false;
    }

    if (record->ability_id < 0 || record->ability_id > 65535)
    {
        report_mod_error(g_ability_mod, "Ability [%s]: ability_id %d outside 0-65535",
                         source, record->ability_id);
        return false;
    }

    /* Advisory only - the record still loads. */
    if (record->skill == 0 && record->spec == 0)
    {
        if (record->level > 50)
        {
            report_mod_warning(g_ability_mod,
                               "Ability [%s]: avatar level-up record with level %d above the level cap 50",
                               source, record->level);
        }
        else
        {
            log_debug(DEBUG_MED, "| - Ability [%s]: avatar level-up record (level %d)",
                      source, record->level);
        }
    }

    registerLearnedAbilityFromSource((uint8_t)record->skill, (uint8_t)record->spec,
                                     (uint8_t)record->level, (uint16_t)record->ability_id, source);
    return true;
}

void initialize_ability_module()
{
    char version_tag_buffer[128];
    snprintf(version_tag_buffer, sizeof(version_tag_buffer), "%d.%d.%d-%s",
             SPELLFRAMEWORK_VERSION_MAJOR, SPELLFRAMEWORK_VERSION_MINOR,
             SPELLFRAMEWORK_VERSION_PATCH, SPELLFRAMEWORK_TAG);

    g_ability_mod = createModInfo("Ability Learn Module", version_tag_buffer,
                                  "Muddykat, UnSchtalch",
                                  "Loads learnable abilities defined by JSON files in the sfsf\\abilities folder.");
    clear_mod_errors(g_ability_mod);
    register_mod_for_listing(g_ability_mod, MOD_TYPE_CORE);

    uint32_t loaded = 0;
    uint32_t skipped = 0;

    char currentDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDir);

    char abilityDirectory[MAX_PATH];
    snprintf(abilityDirectory, sizeof(abilityDirectory), "%s\\sfsf\\abilities", currentDir);

    char searchPath[MAX_PATH];
    snprintf(searchPath, sizeof(searchPath), "%s\\*.json", abilityDirectory);

    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(searchPath, &findFileData);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if ((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
            {
                continue;
            }

            char abilityPath[MAX_PATH];
            snprintf(abilityPath, sizeof(abilityPath), "%s\\%s", abilityDirectory,
                     findFileData.cFileName);

            log_info("| - Reading ability definitions [%s]", findFileData.cFileName);

            AbilityJsonRecord records[SFSF_MAX_ABILITY_RECORDS];
            int record_count = 0;

            if (!parse_ability_json_file(abilityPath, records, SFSF_MAX_ABILITY_RECORDS,
                                         &record_count))
            {
                report_mod_error(g_ability_mod, "Ability [%s] could not be parsed",
                                 findFileData.cFileName);
                skipped++;
                continue;
            }

            if (record_count == 0)
            {
                report_mod_warning(g_ability_mod, "Ability [%s] contains no records",
                                   findFileData.cFileName);
            }

            for (int i = 0; i < record_count; i++)
            {
                if (accept_ability_record(&records[i], findFileData.cFileName))
                {
                    loaded++;
                }
                else
                {
                    skipped++;
                }
            }
        } while (FindNextFile(hFind, &findFileData) != 0);

        FindClose(hFind);
    }
    else
    {
        // No folder and no ability files is a perfectly normal install.
        log_info("| - No ability files found in %s (custom abilities disabled)", abilityDirectory);
    }

    /* The single registry sort, after every population is done - including the
     * vanilla entries registered in the configuration phase. */
    finalizeLearnedAbilities();

    log_info("| - %u ability record(s) registered, %u skipped", loaded, skipped);
}

/** @} */
