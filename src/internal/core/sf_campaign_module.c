/**
 * @addtogroup CampaignModule
 * @{
 */

#include <windows.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sf_campaign_module.h"
#include "sf_campaign_loader.h"
#include "sf_hooks.h"
#include "sf_wrappers.h"

#include "../registry/sf_error_registry.h"
#include "../registry/sf_registry.h"

SFMod *g_campaign_mod = NULL;

/**
 * @brief Asks the engine whether a map exists under a campaign's folder.
 */
static bool campaign_map_exists(const char *campaign_folder, const char *map_name)
{
    char map_path[MAX_PATH];
    snprintf(map_path, sizeof(map_path), "map\\%s\\%s.map", campaign_folder, map_name);

    SF_String path;
    uiAPI.SFStringConstructor_char(&path, map_path);
    bool exists = (checkFileExists(&path) != 0);
    uiAPI.SFStringDestructor(&path);

    if (!exists)
    {
        log_debug(DEBUG_MED, "| - Campaign map not found: %s", map_path);
    }

    return exists;
}

/* Save folders the engine creates for itself in checkDirs(). A campaign that
 * claimed one of these would share its saves with a vanilla campaign. */
static const char *k_reserved_folders[] = { "save", "temp", "char", "campaign2", "campaign3" };

static bool equals_ignore_case(const char *a, const char *b)
{
    while (*a != '\0' && *b != '\0')
    {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
        {
            return false;
        }
        a++;
        b++;
    }
    return *a == *b;
}

/**
 * @brief Rejects folder names that would escape the save root or collide with
 * the engine's own directories.
 */
static bool is_campaign_folder_safe(const char *folder, const char *source)
{
    if (strchr(folder, '\\') != NULL || strchr(folder, '/') != NULL ||
        strchr(folder, ':') != NULL || strstr(folder, "..") != NULL)
    {
        report_mod_error(g_campaign_mod,
                         "Campaign [%s]: campaign_folder \"%s\" must be a plain folder name (no slashes, colons or \"..\")",
                         source, folder);
        return false;
    }

    for (size_t i = 0; i < (sizeof(k_reserved_folders) / sizeof(k_reserved_folders[0])); i++)
    {
        if (equals_ignore_case(folder, k_reserved_folders[i]))
        {
            report_mod_error(g_campaign_mod,
                             "Campaign [%s]: campaign_folder \"%s\" is reserved by the engine",
                             source, folder);
            return false;
        }
    }

    return true;
}

/**
 * @brief Applies every rule a campaign must satisfy before registration.
 * @return false when the campaign must be skipped.
 */
static bool validate_campaign(CampaignJson *campaign, const char *source)
{
    SFSF_CampaignDef *def = &campaign->def;
    bool valid = true;

    if (!campaign->found_name || def->name[0] == '\0')
    {
        report_mod_error(g_campaign_mod, "Campaign [%s]: missing required field \"name\"", source);
        valid = false;
    }

    if (!campaign->found_start_map || def->start_map[0] == '\0')
    {
        report_mod_error(g_campaign_mod, "Campaign [%s]: missing required field \"start_map\"", source);
        valid = false;
    }

    if (!campaign->found_campaign_folder || def->campaign_folder[0] == '\0')
    {
        report_mod_error(g_campaign_mod, "Campaign [%s]: missing required field \"campaign_folder\"", source);
        valid = false;
    }
    else if (!is_campaign_folder_safe(def->campaign_folder, source))
    {
        valid = false;
    }

    if (campaign->found_avatar_type && def->avatar_type != -1 &&
        (def->avatar_type < SFSF_AVATAR_TYPE_BASE || def->avatar_type > SFSF_AVATAR_TYPE_MAX))
    {
        report_mod_error(g_campaign_mod,
                         "Campaign [%s]: avatar_type %u is outside the custom range %u-%u",
                         source, def->avatar_type,
                         (uint32_t)SFSF_AVATAR_TYPE_BASE, (uint32_t)SFSF_AVATAR_TYPE_MAX);
        valid = false;
    }

    if (!valid)
    {
        // The map checks below need a usable folder and map name to build a path.
        return false;
    }

    /* The fields are well formed - now make sure they point at maps that are
     * actually on disk, so a typo is caught here rather than at load time. */
    if (!campaign_map_exists(def->campaign_folder, def->start_map))
    {
        report_mod_error(g_campaign_mod,
                         "Campaign [%s]: start_map \"%s\" not found at map\\%s\\%s.map",
                         source, def->start_map, def->campaign_folder, def->start_map);
        valid = false;
    }

    if (def->tutorial_map[0] != '\0' &&
        !campaign_map_exists(def->campaign_folder, def->tutorial_map))
    {
        report_mod_error(g_campaign_mod,
                         "Campaign [%s]: tutorial_map \"%s\" not found at map\\%s\\%s.map",
                         source, def->tutorial_map, def->campaign_folder, def->tutorial_map);
        valid = false;
    }

    if (!valid)
    {
        return false;
    }

    /* Advisory only - the campaign still loads. */
    if (def->description[0] == '\0')
    {
        report_mod_warning(g_campaign_mod, "Campaign [%s]: no description set", source);
    }

    if (def->author[0] == '\0')
    {
        report_mod_warning(g_campaign_mod, "Campaign [%s]: no author set", source);
    }

    if (def->tutorial_map[0] == '\0')
    {
        log_debug(DEBUG_MED, "| - Campaign [%s]: no tutorial_map, the tutorial checkbox will be ignored", source);
    }

    return true;
}

/**
 * @brief Checks a validated campaign against everything already registered.
 * @return false when the campaign must be skipped.
 */
static bool check_campaign_conflicts(const CampaignJson *campaign, const char *source)
{
    const SFSF_CampaignDef *def = &campaign->def;
    bool conflict_free = true;

    if (!claim_string_id(CONFLICT_CAMPAIGN_FOLDER, def->campaign_folder, g_campaign_mod,
                         "Campaign folder"))
    {
        // Two campaigns sharing a save folder would overwrite each other's saves.
        log_error("| - Campaign [%s] cannot share campaign_folder \"%s\"", source, def->campaign_folder);
        conflict_free = false;
    }

    if (!claim_string_id(CONFLICT_CAMPAIGN_NAME, def->name, g_campaign_mod, "Campaign name"))
    {
        // Not fatal on its own, but the menu buttons become indistinguishable.
        log_warning("| - Campaign [%s] reuses the name \"%s\"", source, def->name);
    }

    if (def->avatar_type != -1 &&
        !claim_numeric_id(CONFLICT_CAMPAIGN_AVATAR_TYPE, def->avatar_type, g_campaign_mod,
                          0, "Campaign avatar type"))
    {
        log_error("| - Campaign [%s] cannot share avatar_type %u", source, def->avatar_type);
        conflict_free = false;
    }

    return conflict_free;
}

/** @brief Loads one campaign file. Returns true when it reached the registry. */
static bool load_campaign_file(const char *path, const char *filename)
{
    if (g_campaign_count >= SFSF_MAX_CAMPAIGNS)
    {
        report_mod_error(g_campaign_mod,
                         "Campaign [%s] skipped: the registry is full (%d campaigns max)",
                         filename, SFSF_MAX_CAMPAIGNS);
        return false;
    }

    CampaignJson campaign;
    if (!parse_campaign_json_file(path, &campaign))
    {
        report_mod_error(g_campaign_mod, "Campaign [%s] could not be parsed", filename);
        return false;
    }

    if (!validate_campaign(&campaign, filename))
    {
        return false;
    }

    if (!check_campaign_conflicts(&campaign, filename))
    {
        return false;
    }

    int32_t index = register_campaign(&campaign.def);
    if (index < 0)
    {
        report_mod_error(g_campaign_mod, "Campaign [%s] was rejected by the registry", filename);
        return false;
    }

    if (campaign.def.avatar_type == -1)
    {
        claim_numeric_id(CONFLICT_CAMPAIGN_AVATAR_TYPE, g_campaigns[index].avatar_type,
                         g_campaign_mod, 0, "Campaign avatar type");
    }

    return true;
}

void initialize_campaign_module()
{
    char version_tag_buffer[128];
    snprintf(version_tag_buffer, sizeof(version_tag_buffer), "%d.%d.%d-%s",
             SPELLFRAMEWORK_VERSION_MAJOR, SPELLFRAMEWORK_VERSION_MINOR,
             SPELLFRAMEWORK_VERSION_PATCH, SPELLFRAMEWORK_TAG);

    g_campaign_mod = createModInfo("Custom Campaign Module", version_tag_buffer,
                                   "Muddykat, UnSchtalch",
                                   "Loads custom campaigns defined by JSON files in the sfsf\\campaigns folder.");
    clear_mod_errors(g_campaign_mod);
    register_mod_for_listing(g_campaign_mod, MOD_TYPE_CORE);

    char currentDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDir);

    char campaignDirectory[MAX_PATH];
    snprintf(campaignDirectory, sizeof(campaignDirectory), "%s\\sfsf\\campaigns", currentDir);

    char searchPath[MAX_PATH];
    snprintf(searchPath, sizeof(searchPath), "%s\\*.json", campaignDirectory);

    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(searchPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        // No folder and no campaigns is a perfectly normal install.
        log_info("| - No campaigns found in %s (custom campaigns disabled)", campaignDirectory);
        return;
    }

    uint32_t loaded = 0;
    uint32_t skipped = 0;
    do
    {
        if ((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
        {
            continue;
        }

        char campaignPath[MAX_PATH];
        snprintf(campaignPath, sizeof(campaignPath), "%s\\%s", campaignDirectory, findFileData.cFileName);

        log_info("| - Reading campaign definition [%s]", findFileData.cFileName);

        if (load_campaign_file(campaignPath, findFileData.cFileName))
        {
            loaded++;
        }
        else
        {
            skipped++;
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);

    log_info("| - %u campaign(s) registered, %u skipped", loaded, skipped);
}

/** @} */
