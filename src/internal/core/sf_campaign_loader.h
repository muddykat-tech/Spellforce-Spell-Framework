/**
 * @defgroup CampaignLoader Custom Campaign JSON Loader
 * @ingroup Core
 * @brief Turns a campaign JSON file into an SFSF_CampaignDef.
 *
 * The loader only parses - it does not validate meaning or register anything.
 * Presence flags are reported back so the caller (sf_campaign_module.c) can
 * decide what is required and attribute the failures to a mod.
 *
 * @addtogroup CampaignLoader
 * @{
 */

#ifndef CAMPAIGN_LOADER_H
#define CAMPAIGN_LOADER_H

#include <stdbool.h>
#include <stdint.h>

#include "hooks/sf_campaign_hook.h"

/**
 * @brief Parse result: the definition plus which keys were actually present.
 *
 * Kept separate from SFSF_CampaignDef so the registered struct stays free of
 * loader bookkeeping, mirroring how Building relates to SFBuilding.
 */
typedef struct
{
    SFSF_CampaignDef def;

    bool found_name;
    bool found_campaign_name_id;
    bool found_description;
    bool found_author;
    bool found_start_map;
    bool found_tutorial_map;
    bool found_campaign_folder;
    bool found_starterkit;
    bool found_intro_video;
    bool found_avatar_type;
} CampaignJson;

typedef enum
{
    CAMPAIGN_FIELD_UNKNOWN = 0,
    CAMPAIGN_FIELD_NAME,
    CAMPAIGN_FIELD_CAMPAIGN_NAME_ID,
    CAMPAIGN_FIELD_DESCRIPTION,
    CAMPAIGN_FIELD_AUTHOR,
    CAMPAIGN_FIELD_START_MAP,
    CAMPAIGN_FIELD_TUTORIAL_MAP,
    CAMPAIGN_FIELD_CAMPAIGN_FOLDER,
    CAMPAIGN_FIELD_STARTERKIT,
    CAMPAIGN_FIELD_INTRO_VIDEO,
    CAMPAIGN_FIELD_AVATAR_TYPE,
} CampaignFieldKey;

/**
 * @brief Reads and parses a campaign JSON file.
 *
 * @param path         Full path to the .json file.
 * @param out_campaign Zeroed and filled on success.
 * @return false when the file is unreadable or is not a single JSON object.
 */
bool parse_campaign_json_file(const char *path, CampaignJson *out_campaign);

/** @} */
#endif // CAMPAIGN_LOADER_H
