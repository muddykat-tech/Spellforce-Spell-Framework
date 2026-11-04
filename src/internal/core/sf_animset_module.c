/**
 * @addtogroup AnimsetModule
 * @{
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "sf_animset_module.h"
#include "sf_animset_loader.h"
#include "../registry/sf_animset_registry.h"
#include "../registry/sf_weapon_type_registry.h"
#include "hooks/sf_movie_lib_bridge.h" /* animsetLibCacheEvictionCount for the summary line */
#include "sf_hooks.h"
#include "sf_wrappers.h"

#include "../registry/sf_error_registry.h"
#include "../registry/sf_registry.h"

SFMod *g_animset_mod = NULL;

/* Whole-catalog name budget (TZ 4d, W2-3): the unit of account is one NAME
 * OCCURRENCE - every element of anims[]/aim[]/shoot[] in every section of
 * every file. Conservative on purpose: the bridge's probe cache is keyed by
 * name, so counting occurrences (not distinct names) bounds the worst case
 * without deduplication. */
#define SFSF_ANIMSET_CATALOG_NAME_LIMIT 230

/* Record accumulator cap; the records are far too large for the stack, the
 * array is heap-allocated like the per-file catalogs. */
#define SFSF_ANIMSET_CATALOG_MAX_RECORDS 256

/* Directory scan caps. */
#define SFSF_ANIMSET_MAX_FILES 256
#define SFSF_ANIMSET_MAX_PATH MAX_PATH

/* Sound field bounds, seconds. */
#define SFSF_ANIMSET_SOUND_MAX_SECONDS 60.0f

/** @brief True for a weapon type allowed in a shielded pair (TZ 4b):
 * {0,2,3,4,5,6,7,8,18} (vanilla 1H set + bare hand + Fist item) plus the two
 * custom segments. Type 19 (Claw) is deliberately absent (fixed owner list). */
static bool animsetShieldedWeaponAllowed(int32_t weapon)
{
    if (weapon == 0)
    {
        return true; /* bare hand + shield: the shield-bash scenario */
    }
    if (weapon == 2)
    {
        return true; /* equipped Fist item -> Punch */
    }
    if (weapon >= 3 && weapon <= 8)
    {
        return true; /* 1HDagger..1HStaff (3..8 includes the shield group 4..7) */
    }
    if (weapon == 18)
    {
        return true; /* 1HMaceBlunt, shield group member */
    }
    if (weapon >= 22 && weapon <= 99)
    {
        return true; /* custom weapon types, first segment */
    }
    if (weapon >= 108 && weapon <= 239)
    {
        return true; /* custom weapon types, second segment */
    }
    return false;
}

/** @brief True for a weapon type allowed in a solo set (TZ 4b): the vanilla
 * melee set {0..15, 18..21} (ranged 16/17 excluded, W1-5) plus the custom
 * segments - a catalog type with attack_style "ranged" is refused per-record
 * by the cross-check below, not by this set. */
static bool animsetSoloWeaponAllowed(int32_t weapon)
{
    if (weapon >= 0 && weapon <= 15)
    {
        return true;
    }
    if (weapon >= 18 && weapon <= 99)
    {
        return true; /* 18..21 vanilla tail plus the first custom segment */
    }
    if (weapon >= 108 && weapon <= 239)
    {
        return true;
    }
    return false;
}

/** @brief True for a weapon type allowed in a ranged set (TZ 4b): custom
 * segments only - the ranged phases are cross-checked against the
 * weapon_types catalog (attack_style "ranged") separately. */
static bool animsetRangedWeaponAllowed(int32_t weapon)
{
    return (weapon >= 22 && weapon <= 99) || (weapon >= 108 && weapon <= 239);
}

/** @brief True for [A-Za-z0-9_], the charset of animset and film names. */
static bool animsetNameCharsetOk(const char *name)
{
    for (const char *c = name; *c != '\0'; c++)
    {
        if (!isalnum((unsigned char)*c) && *c != '_')
        {
            return false;
        }
    }
    return true;
}

/* Section-validation outcome: record-local problems drop the record
 * (per-record skip, TZ 4d), structural/section problems kill the file. */
enum
{
    SET_OK = 0,
    SET_DROP_RECORD,
    SET_FILE_BROKEN
};

/** @brief Checks the sound object of one section record (shared by all three
 * sections; per-record for solo/ranged, per-file for shielded as v1). */
static int checkSetSound(const AnimsetSound *sound, char *out_name, size_t out_name_len,
                         float *out_offset, float *out_length, bool *out_has_sound,
                         const char *source)
{
    if (sound->found_name)
    {
        if (sound->name[0] == '\0')
        {
            report_mod_warning(g_animset_mod, "Animset [%s]: sound.name is empty", source);
            return SET_DROP_RECORD;
        }
        strncpy(out_name, sound->name, out_name_len - 1);
        out_name[out_name_len - 1] = '\0';
        *out_offset = sound->found_offset ? sound->offset : 0.0f;
        *out_length = sound->found_length ? sound->length : 0.0f;
        if (*out_offset < 0.0f || *out_offset > SFSF_ANIMSET_SOUND_MAX_SECONDS ||
            *out_length < 0.0f || *out_length > SFSF_ANIMSET_SOUND_MAX_SECONDS)
        {
            report_mod_warning(g_animset_mod,
                               "Animset [%s]: sound offset/length must be within 0.0-60.0 seconds",
                               source);
            return SET_DROP_RECORD;
        }
        *out_has_sound = true;
    }
    else if (sound->found_offset || sound->found_length)
    {
        report_mod_warning(g_animset_mod, "Animset [%s]: sound.offset/length without sound.name",
                           source);
        return SET_DROP_RECORD;
    }
    return SET_OK;
}

/** @brief Checks one shielded[] / solo[] record and fills the registry set.
 * @param strict_file true for the shielded section (missing-field problems
 * are file-level there, as v1); solo failures are record-local. */
static int acceptFilmSet(const AnimsetFilmSet *set, AnimsetFilmSetEntry *out_set,
                         bool (*allowed)(int32_t), const char *section, bool strict_file,
                         const char *source, uint32_t *dropped)
{
    if (!set->found_weapon || !set->found_anims)
    {
        report_mod_warning(g_animset_mod,
                           "Animset [%s]: %s record is missing a required field (weapon, anims)",
                           source, section);
        return strict_file ? SET_FILE_BROKEN : SET_DROP_RECORD;
    }
    if (!allowed(set->weapon))
    {
        report_mod_warning(g_animset_mod,
                           "Animset [%s]: weapon %d is not one of the allowed ids of the %s section",
                           source, set->weapon, section);
        (*dropped)++;
        return SET_DROP_RECORD;
    }
    if (set->anim_count < 1)
    {
        report_mod_warning(g_animset_mod, "Animset [%s]: %s record of weapon %d has no anims",
                           source, section, set->weapon);
        (*dropped)++;
        return SET_DROP_RECORD;
    }
    for (int j = 0; j < set->anim_count; j++)
    {
        if (set->anims[j][0] == '\0' || !animsetNameCharsetOk(set->anims[j]))
        {
            report_mod_warning(g_animset_mod,
                               "Animset [%s]: %s record of weapon %d has an empty or non A-Za-z0-9_ anim name",
                               source, section, set->weapon);
            (*dropped)++;
            return SET_DROP_RECORD;
        }
    }

    out_set->weapon = (uint16_t)set->weapon;
    out_set->anim_count = (uint8_t)set->anim_count;
    for (int j = 0; j < set->anim_count; j++)
    {
        strncpy(out_set->anims[j], set->anims[j], sizeof(out_set->anims[j]) - 1);
        out_set->anims[j][sizeof(out_set->anims[j]) - 1] = '\0';
    }
    int sound_status = checkSetSound(&set->sound, out_set->sound_name, sizeof(out_set->sound_name),
                                     &out_set->sound_offset, &out_set->sound_length,
                                     &out_set->has_sound, source);
    if (sound_status != SET_OK)
    {
        (*dropped)++;
        return sound_status;
    }
    return SET_OK;
}

/** @brief Checks one ranged[] record and fills the registry set. Both phases
 * are mandatory (owner decision 14.2) and the type must sit in the
 * weapon_types catalog with attack_style "ranged" (TZ 4b cross-check; the
 * registry is finalized earlier in the Core Module Phase). */
static int acceptRangedSet(const AnimsetRangedSet *set, AnimsetRangedEntry *out_set,
                           const char *source, uint32_t *dropped)
{
    if (!set->found_weapon || !set->found_aim || !set->found_shoot)
    {
        report_mod_warning(g_animset_mod,
                           "Animset [%s]: ranged record is missing a required field "
                           "(weapon, aim and shoot are both required)",
                           source);
        (*dropped)++;
        return SET_DROP_RECORD;
    }
    if (!animsetRangedWeaponAllowed(set->weapon))
    {
        report_mod_warning(g_animset_mod,
                           "Animset [%s]: weapon %d is not one of the allowed ids of the ranged section",
                           source, set->weapon);
        (*dropped)++;
        return SET_DROP_RECORD;
    }
    if (!isOverridedRangedWeaponType((uint16_t)set->weapon))
    {
        report_mod_warning(g_animset_mod,
                           "Animset [%s]: ranged set for weapon %d refused: the type is not in the "
                           "weapon_types catalog with attack_style \"ranged\"",
                           source, set->weapon);
        (*dropped)++;
        return SET_DROP_RECORD;
    }
    if (set->aim_count < 1 || set->shoot_count < 1)
    {
        report_mod_warning(g_animset_mod,
                           "Animset [%s]: ranged record of weapon %d has an empty aim or shoot list",
                           source, set->weapon);
        (*dropped)++;
        return SET_DROP_RECORD;
    }
    for (int j = 0; j < set->aim_count; j++)
    {
        if (set->aim[j][0] == '\0' || !animsetNameCharsetOk(set->aim[j]))
        {
            report_mod_warning(g_animset_mod, "Animset [%s]: ranged aim name of weapon %d is invalid",
                               source, set->weapon);
            (*dropped)++;
            return SET_DROP_RECORD;
        }
    }
    for (int j = 0; j < set->shoot_count; j++)
    {
        if (set->shoot[j][0] == '\0' || !animsetNameCharsetOk(set->shoot[j]))
        {
            report_mod_warning(g_animset_mod, "Animset [%s]: ranged shoot name of weapon %d is invalid",
                               source, set->weapon);
            (*dropped)++;
            return SET_DROP_RECORD;
        }
    }

    out_set->weapon = (uint16_t)set->weapon;
    out_set->aim_count = (uint8_t)set->aim_count;
    out_set->shoot_count = (uint8_t)set->shoot_count;
    for (int j = 0; j < set->aim_count; j++)
    {
        strncpy(out_set->aim[j], set->aim[j], sizeof(out_set->aim[j]) - 1);
        out_set->aim[j][sizeof(out_set->aim[j]) - 1] = '\0';
    }
    for (int j = 0; j < set->shoot_count; j++)
    {
        strncpy(out_set->shoot[j], set->shoot[j], sizeof(out_set->shoot[j]) - 1);
        out_set->shoot[j][sizeof(out_set->shoot[j]) - 1] = '\0';
    }
    int sound_status = checkSetSound(&set->sound, out_set->sound_name, sizeof(out_set->sound_name),
                                     &out_set->sound_offset, &out_set->sound_length,
                                     &out_set->has_sound, source);
    if (sound_status != SET_OK)
    {
        (*dropped)++;
        return sound_status;
    }
    return SET_OK;
}

/**
 * @brief Checks one parsed entry and converts it into a registry record.
 * @param dropped per-record skip counter of the current file (solo/ranged
 * granularity and loader drops, TZ 4d).
 * @return false for per-file class problems (the file dies whole).
 */
static bool accept_animset_entry(const AnimsetEntry *entry, AnimsetEntryRecord *record,
                                 const char *source, uint32_t *dropped)
{
    if (!entry->found_figure || !entry->found_base_name)
    {
        report_mod_warning(g_animset_mod,
                           "Animset [%s]: record is missing a required field (figure, base_name)",
                           source);
        return false;
    }
    if (entry->figure[0] == '\0' || !animsetNameCharsetOk(entry->figure))
    {
        report_mod_warning(g_animset_mod,
                           "Animset [%s]: figure \"%s\" is empty or contains characters outside A-Za-z0-9_",
                           source, entry->figure);
        return false;
    }
    if (entry->base_name[0] == '\0')
    {
        report_mod_warning(g_animset_mod, "Animset [%s]: base_name is empty", source);
        return false;
    }
    if (entry->shielded_count == 0 && entry->solo_count == 0 && entry->ranged_count == 0)
    {
        report_mod_warning(g_animset_mod,
                           "Animset [%s]: record carries no shielded/solo/ranged section", source);
        return false;
    }

    /* shielded: v1 granularity for its own errors (missing fields), per-record
     * for the general TZ 4d rows (allowed ids, empty anims). */
    for (int i = 0; i < entry->shielded_count; i++)
    {
        int status = acceptFilmSet(&entry->shielded[i], &record->shielded[record->shielded_count],
                                   animsetShieldedWeaponAllowed, "shielded", true, source, dropped);
        if (status == SET_FILE_BROKEN)
        {
            return false;
        }
        if (status == SET_OK)
        {
            record->shielded_count++;
        }
    }

    for (int i = 0; i < record->shielded_count; i++)
    {
        for (int j = i + 1; j < record->shielded_count; j++)
        {
            if (record->shielded[i].weapon == record->shielded[j].weapon)
            {
                report_mod_warning(g_animset_mod,
                                   "Animset [%s]: weapon %d appears twice in the shielded section",
                                   source, record->shielded[i].weapon);
                return false;
            }
        }
    }

    for (int i = 0; i < entry->solo_count; i++)
    {
        int status = acceptFilmSet(&entry->solo[i], &record->solo[record->solo_count],
                                   animsetSoloWeaponAllowed, "solo", false, source, dropped);
        if (status == SET_OK)
        {
            if (isOverridedRangedWeaponType(record->solo[record->solo_count].weapon))
            {
                report_mod_warning(g_animset_mod,
                                   "Animset [%s]: solo set for weapon %d refused: weapon type has "
                                   "style ranged, use the ranged section",
                                   source, record->solo[record->solo_count].weapon);
                (*dropped)++;
            }
            else
            {
                if (record->solo[record->solo_count].weapon <= 1)
                {
                    log_warning("Animset [%s]: solo set for weapon %d rides the shared bare-hand "
                                "(Strike, 0/1) key of the vanilla cascade",
                                source, record->solo[record->solo_count].weapon);
                }
                record->solo_count++;
            }
        }
        else if (status == SET_FILE_BROKEN)
        {
            return false;
        }
    }
    /* Duplicate solo weapons duplicate the key (jobMap(w), w): per-file (TZ 4d). */
    for (int i = 0; i < record->solo_count; i++)
    {
        for (int j = i + 1; j < record->solo_count; j++)
        {
            if (record->solo[i].weapon == record->solo[j].weapon)
            {
                report_mod_warning(g_animset_mod,
                                   "Animset [%s]: weapon %d appears twice in the solo section",
                                   source, record->solo[i].weapon);
                return false;
            }
        }
    }

    for (int i = 0; i < entry->ranged_count; i++)
    {
        int status = acceptRangedSet(&entry->ranged[i], &record->ranged[record->ranged_count],
                                     source, dropped);
        if (status == SET_OK)
        {
            record->ranged_count++;
        }
    }
    /* Duplicate ranged weapons duplicate (71, w) and (74, w): per-file. */
    for (int i = 0; i < record->ranged_count; i++)
    {
        for (int j = i + 1; j < record->ranged_count; j++)
        {
            if (record->ranged[i].weapon == record->ranged[j].weapon)
            {
                report_mod_warning(g_animset_mod,
                                   "Animset [%s]: weapon %d appears twice in the ranged section",
                                   source, record->ranged[i].weapon);
                return false;
            }
        }
    }

    strncpy(record->figure, entry->figure, sizeof(record->figure) - 1);
    record->figure[sizeof(record->figure) - 1] = '\0';
    strncpy(record->base_name, entry->base_name, sizeof(record->base_name) - 1);
    record->base_name[sizeof(record->base_name) - 1] = '\0';
    strncpy(record->source, source, sizeof(record->source) - 1);
    record->source[sizeof(record->source) - 1] = '\0';
    return true;
}

/** @brief Name occurrences of one parsed file - the TZ 4d unit of account
 * (W2-3): every anims[]/aim[]/shoot[] element, counted at parse level before
 * validation (conservative: dropped records still bound the probe budget). */
static uint32_t fileNameOccurrences(const AnimsetCatalogFile *catalog)
{
    uint32_t names = 0;
    for (int i = 0; i < catalog->entry_count; i++)
    {
        const AnimsetEntry *entry = &catalog->entries[i];
        for (int j = 0; j < entry->shielded_count; j++)
        {
            names += (uint32_t)entry->shielded[j].anim_count;
        }
        for (int j = 0; j < entry->solo_count; j++)
        {
            names += (uint32_t)entry->solo[j].anim_count;
        }
        for (int j = 0; j < entry->ranged_count; j++)
        {
            names += (uint32_t)entry->ranged[j].aim_count;
            names += (uint32_t)entry->ranged[j].shoot_count;
        }
    }
    return names;
}

static int compareFilenames(const void *a, const void *b)
{
    return _stricmp((const char *)a, (const char *)b);
}

void initialize_animset_module()
{
    char version_tag_buffer[128];
    snprintf(version_tag_buffer, sizeof(version_tag_buffer), "%d.%d.%d-%s",
             SPELLFRAMEWORK_VERSION_MAJOR, SPELLFRAMEWORK_VERSION_MINOR,
             SPELLFRAMEWORK_VERSION_PATCH, SPELLFRAMEWORK_TAG);

    g_animset_mod = createModInfo("Custom Animset Module", version_tag_buffer,
                                   "Muddykat, UnSchtalch",
                                   "Loads custom animsets (shielded/solo/ranged) defined by JSON files in the sfsf\\animsets folder.");
    clear_mod_errors(g_animset_mod);
    register_mod_for_listing(g_animset_mod, MOD_TYPE_CORE);

    uint32_t loaded = 0;
    uint32_t skipped = 0;
    uint32_t shielded_total = 0;
    uint32_t solo_total = 0;
    uint32_t ranged_total = 0;
    uint32_t name_total = 0;

    char currentDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDir);

    char animsetDirectory[MAX_PATH];
    snprintf(animsetDirectory, sizeof(animsetDirectory), "%s\\sfsf\\animsets", currentDir);

    char searchPath[MAX_PATH];
    snprintf(searchPath, sizeof(searchPath), "%s\\*.json", animsetDirectory);

    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(searchPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        // No folder and no animset files is a perfectly normal install.
        log_info("| - No animset files found in %s (custom animsets disabled)",
                 animsetDirectory);
        return;
    }

    /* Deterministic 230 accounting (TZ 4d, W1-9): collect the file names
     * first, sort them, then load in that order - the file that crosses the
     * limit is then independent of the FindFirstFile order. */
    static char fileNames[SFSF_ANIMSET_MAX_FILES][SFSF_ANIMSET_MAX_PATH];
    int fileCount = 0;
    do
    {
        if ((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
        {
            continue;
        }
        if (fileCount >= SFSF_ANIMSET_MAX_FILES)
        {
            report_mod_warning(g_animset_mod,
                               "Animset catalog has more than %d files, ignoring the rest",
                               SFSF_ANIMSET_MAX_FILES);
            break;
        }
        strncpy(fileNames[fileCount], findFileData.cFileName, SFSF_ANIMSET_MAX_PATH - 1);
        fileNames[fileCount][SFSF_ANIMSET_MAX_PATH - 1] = '\0';
        fileCount++;
    } while (FindNextFile(hFind, &findFileData) != 0);
    FindClose(hFind);

    qsort(fileNames, fileCount, SFSF_ANIMSET_MAX_PATH, compareFilenames);

    /* Allocated only once the folder exists, so the normal no-folder install
     * never allocates (and never leaks) the record accumulator. */
    AnimsetEntryRecord *records =
        (AnimsetEntryRecord *)calloc(SFSF_ANIMSET_CATALOG_MAX_RECORDS, sizeof(AnimsetEntryRecord));
    size_t record_count = 0;
    if (records == NULL)
    {
        log_error("| - Unable to allocate the animset catalog (custom animsets disabled)");
        return;
    }

    for (int fileIndex = 0; fileIndex < fileCount; fileIndex++)
    {
        const char *fileName = fileNames[fileIndex];

        char animsetPath[MAX_PATH];
        snprintf(animsetPath, sizeof(animsetPath), "%s\\%s", animsetDirectory, fileName);

        log_info("Reading animset definitions [%s]", fileName);

        AnimsetCatalogFile *catalog = (AnimsetCatalogFile *)calloc(1, sizeof(AnimsetCatalogFile));
        if (catalog == NULL)
        {
            report_mod_error(g_animset_mod, "Animset file [%s] could not be read (out of memory)",
                             fileName);
            skipped++;
            continue;
        }

        if (!parse_animset_json_file(animsetPath, catalog))
        {
            if (catalog->rejected_v1_format)
            {
                report_mod_warning(g_animset_mod,
                                   "Animset file [%s] could not be loaded: "
                                   "unsupported v1 format (pairs), see v2 schema (solo/shielded/ranged)",
                                   fileName);
            }
            else
            {
                report_mod_warning(g_animset_mod, "Animset file [%s] could not be parsed", fileName);
            }
            skipped++;
            free(catalog);
            continue;
        }

        /* 230 limit (TZ 4d): counted per file in name-sorted order; the
         * violating file dies whole, its names never enter the counter and
         * the following files keep loading. */
        uint32_t file_names = fileNameOccurrences(catalog);
        if (name_total + file_names > SFSF_ANIMSET_CATALOG_NAME_LIMIT)
        {
            report_mod_warning(g_animset_mod,
                               "Animset catalog name limit exceeded (%d names), file [%s] skipped",
                               SFSF_ANIMSET_CATALOG_NAME_LIMIT, fileName);
            skipped += (uint32_t)catalog->entry_count;
            free(catalog);
            continue;
        }

        uint32_t dropped = (uint32_t)catalog->dropped_record_count;
        size_t file_first_record = record_count;
        bool file_ok = true;

        for (int i = 0; i < catalog->entry_count; i++)
        {
            if (record_count >= SFSF_ANIMSET_CATALOG_MAX_RECORDS)
            {
                report_mod_warning(g_animset_mod,
                                   "Animset catalog has more than %d records, ignoring the rest",
                                   SFSF_ANIMSET_CATALOG_MAX_RECORDS);
                file_ok = false;
                break;
            }
            AnimsetEntryRecord record;
            memset(&record, 0, sizeof(record));
            if (!accept_animset_entry(&catalog->entries[i], &record, fileName, &dropped))
            {
                file_ok = false;
                break;
            }
            records[record_count++] = record;
        }

        if (file_ok)
        {
            skipped += dropped;
            name_total += file_names;
        }
        else
        {
            /* Roll the file's partial contributions back: the file dies whole. */
            record_count = file_first_record;
            skipped += (uint32_t)catalog->entry_count;
        }

        free(catalog);
    }

    /* Base_name conflicts are decided per file as well (unchanged policy): a
     * duplicate inside one file skips that file, the same base_name in two
     * files skips both; everything else registers. */
    AnimsetSkipReport reports[SFSF_ANIMSET_CATALOG_MAX_RECORDS];
    size_t committed = 0;
    size_t commit_skipped = 0;
    size_t report_count = 0;
    commitAnimsetCatalog(records, record_count, &committed, &commit_skipped, reports,
                         SFSF_ANIMSET_CATALOG_MAX_RECORDS, &report_count);
    for (size_t i = 0; i < report_count; i++)
    {
        if (reports[i].reason == ANIMSET_SKIP_INTERNAL_DUPLICATE)
        {
            report_mod_warning(g_animset_mod,
                               "Animset file [%s] has a duplicate base_name inside it, file skipped",
                               reports[i].source);
        }
        else
        {
            report_mod_warning(g_animset_mod,
                               "Animset file [%s] shares a base_name with [%s], both files skipped",
                               reports[i].source, reports[i].other_source);
        }
    }
    skipped += (uint32_t)commit_skipped;
    loaded = (uint32_t)committed;

    /* Section sums follow the COMMITTED records (TZ 11.5): base_name
     * conflicts remove whole files (commit policy is per source), so their
     * records are excluded from the sums by source. */
    shielded_total = 0;
    solo_total = 0;
    ranged_total = 0;
    for (size_t r = 0; r < record_count; r++)
    {
        bool source_skipped = false;
        for (size_t i = 0; i < report_count; i++)
        {
            if (strcmp(records[r].source, reports[i].source) == 0)
            {
                source_skipped = true;
                break;
            }
        }
        if (source_skipped)
        {
            continue;
        }
        shielded_total += records[r].shielded_count;
        solo_total += records[r].solo_count;
        ranged_total += records[r].ranged_count;
    }

    /* The single registry sort, after every population is done. */
    finalizeAnimsets();

    free(records);

    log_info("| - %u animset record(s) registered (shielded %u, solo %u, ranged %u), "
             "%u skipped, lib cache evictions: %u",
             loaded, shielded_total, solo_total, ranged_total, skipped,
             animsetLibCacheEvictionCount());
}

/** @} */
