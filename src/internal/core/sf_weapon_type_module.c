/**
 * @addtogroup WeaponTypeModule
 * @{
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "sf_weapon_type_module.h"
#include "sf_weapon_type_loader.h"
#include "../registry/sf_weapon_type_registry.h"
#include "sf_hooks.h"
#include "sf_wrappers.h"

#include "../registry/sf_error_registry.h"
#include "../registry/sf_registry.h"

SFMod *g_weapon_type_mod = NULL;

/**
 * @brief Maps one attack_style json string to its enum value (TZ 6.1/6.3).
 * @return false for every string outside the five v2 style names ("melee"
 * included - it is a rejected alias of "strike", not a style of its own).
 */
static bool parse_weapon_attack_style(const char *text, WeaponAttackStyle *out_style)
{
    if (strcmp(text, "ranged") == 0)
    {
        *out_style = WEAPON_ATTACK_STYLE_RANGED;
        return true;
    }
    if (strcmp(text, "strike") == 0)
    {
        *out_style = WEAPON_ATTACK_STYLE_MELEE;
        return true;
    }
    if (strcmp(text, "stab") == 0)
    {
        *out_style = WEAPON_ATTACK_STYLE_STAB;
        return true;
    }
    if (strcmp(text, "punch") == 0)
    {
        *out_style = WEAPON_ATTACK_STYLE_PUNCH;
        return true;
    }
    if (strcmp(text, "staff") == 0)
    {
        *out_style = WEAPON_ATTACK_STYLE_STAFF;
        return true;
    }
    return false;
}

/**
 * @brief Checks one parsed record and converts it into a registry entry.
 * @return true when the record was accepted.
 */
static bool accept_weapon_type_record(const WeaponTypeJsonRecord *record, WeaponTypeEntry *entry,
                                      const char *source)
{
    if (!record->found_id || !record->found_name || !record->found_attack_style)
    {
        report_mod_error(g_weapon_type_mod,
                         "Weapon type [%s]: record is missing a required field (id, name, attack_style)",
                         source);
        return false;
    }

    /* ids up to 21 are vanilla (engine kDrwWt constants 1..21 mirror the
     * gamedata WeaponTypes table: 1 Default .. 17 2HCrossbow, 21 Throw).
     * 100-107 hold the kDrwAnim service subkeys of the melee cascade and
     * 240-255 the synthetic shield-pair subkeys of the animset bridge - the
     * two segments are reserved, every violation is a per-record skip. */
    if (record->id >= 100 && record->id <= 107)
    {
        report_mod_error(g_weapon_type_mod,
                         "Weapon type [%s]: id %d collides with kDrwAnim (100-107)", source,
                         record->id);
        return false;
    }
    if (record->id >= 240 && record->id <= 255)
    {
        report_mod_error(g_weapon_type_mod,
                         "Weapon type [%s]: id %d reserved for shield pairs (240-255)", source,
                         record->id);
        return false;
    }
    if (record->id < 22 || record->id > 239)
    {
        report_mod_error(g_weapon_type_mod, "Weapon type [%s]: id %d outside 22-99 and 108-239",
                         source, record->id);
        return false;
    }

    if (record->name[0] == '\0')
    {
        report_mod_error(g_weapon_type_mod, "Weapon type [%s]: name is empty", source);
        return false;
    }

    for (const char *c = record->name; *c != '\0'; c++)
    {
        if (!isalnum((unsigned char)*c))
        {
            report_mod_error(g_weapon_type_mod,
                             "Weapon type [%s]: name \"%s\" contains characters outside A-Za-z0-9",
                             source, record->name);
            return false;
        }
    }

    /* v2 style set (TZ 6.3): {"ranged", "strike", "stab", "punch", "staff"}.
     * "melee" is deliberately NOT accepted - "strike" is the one name of the
     * plain swing style, a second alias would split the catalog language. */
    WeaponAttackStyle style;
    if (!parse_weapon_attack_style(record->attack_style, &style))
    {
        report_mod_error(g_weapon_type_mod,
                          "Weapon type [%s]: attack_style \"%s\" is not one of "
                          "\"ranged\", \"strike\", \"stab\", \"punch\", \"staff\"",
                          source, record->attack_style);
        return false;
    }

    entry->id = (uint16_t)record->id;
    weaponTypeCopyBounded(entry->name, sizeof(entry->name), record->name);
    entry->attack_style = style;
    weaponTypeCopyBounded(entry->source, sizeof(entry->source), source);
    return true;
}

void initialize_weapon_type_module()
{
    char version_tag_buffer[128];
    snprintf(version_tag_buffer, sizeof(version_tag_buffer), "%d.%d.%d-%s",
             SPELLFRAMEWORK_VERSION_MAJOR, SPELLFRAMEWORK_VERSION_MINOR,
             SPELLFRAMEWORK_VERSION_PATCH, SPELLFRAMEWORK_TAG);

    g_weapon_type_mod = createModInfo("Custom Weapon Type Module", version_tag_buffer,
                                      "Muddykat, UnSchtalch",
                                      "Loads custom weapon types defined by JSON files in the sfsf\\weapon_types folder.");
    clear_mod_errors(g_weapon_type_mod);
    register_mod_for_listing(g_weapon_type_mod, MOD_TYPE_CORE);

    uint32_t loaded = 0;
    uint32_t skipped = 0;

    WeaponTypeEntry entries[SFSF_MAX_WEAPON_TYPE_RECORDS];
    size_t entry_count = 0;

    char currentDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDir);

    char weaponTypeDirectory[MAX_PATH];
    snprintf(weaponTypeDirectory, sizeof(weaponTypeDirectory), "%s\\sfsf\\weapon_types", currentDir);

    char searchPath[MAX_PATH];
    snprintf(searchPath, sizeof(searchPath), "%s\\*.json", weaponTypeDirectory);

    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(searchPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        // No folder and no weapon type files is a perfectly normal install.
        log_info("| - No weapon type files found in %s (custom weapon types disabled)",
                 weaponTypeDirectory);
        return;
    }

    do
    {
        if ((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
        {
            continue;
        }

        char weaponTypePath[MAX_PATH];
        snprintf(weaponTypePath, sizeof(weaponTypePath), "%s\\%s", weaponTypeDirectory,
                 findFileData.cFileName);

        log_info("Reading weapon type definitions [%s]", findFileData.cFileName);

        WeaponTypeJsonRecord record;
        if (!parse_weapon_type_json_file(weaponTypePath, &record))
        {
            report_mod_error(g_weapon_type_mod, "Weapon type [%s] could not be parsed",
                             findFileData.cFileName);
            skipped++;
            continue;
        }

        if (entry_count >= SFSF_MAX_WEAPON_TYPE_RECORDS)
        {
            report_mod_error(g_weapon_type_mod,
                             "Weapon type catalog has more than %d records, ignoring the rest",
                             SFSF_MAX_WEAPON_TYPE_RECORDS);
            break;
        }

        WeaponTypeEntry entry;
        if (accept_weapon_type_record(&record, &entry, findFileData.cFileName))
        {
            entries[entry_count++] = entry;
        }
        else
        {
            skipped++;
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);

    /* Strict conflict policy, intentionally different from the ability modules
     * (last-wins there): a weapon id lands in gamedata and savegames, so a
     * duplicate id or name between records deactivates the WHOLE catalog. */
    size_t committed = 0;
    size_t commit_skipped = 0;
    char first_source[64];
    char second_source[64];
    WeaponTypeCommitStatus status = commitWeaponTypeCatalog(entries, entry_count, &committed,
                                                            &commit_skipped, first_source,
                                                            sizeof(first_source), second_source,
                                                            sizeof(second_source));
    if (status != WT_COMMIT_OK)
    {
        report_mod_error(g_weapon_type_mod,
                         "Weapon type duplicate %s between [%s] and [%s], the whole catalog is deactivated",
                         (status == WT_COMMIT_DUPLICATE_ID) ? "id" : "name",
                         first_source, second_source);
        skipped = (uint32_t)commit_skipped;
    }

    loaded = (uint32_t)committed;

    /* The single registry sort, after every population is done. */
    finalizeWeaponTypes();

    log_info("| - %u weapon type record(s) registered, %u skipped", loaded, skipped);
}

/** @} */
