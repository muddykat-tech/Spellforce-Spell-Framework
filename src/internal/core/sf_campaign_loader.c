/**
 * @addtogroup CampaignLoader
 * @{
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sf_campaign_loader.h"
#include "sf_building_loader.h" /* readfile() */
#include "sf_wrappers.h"

#define JSMN_STATIC
#define JSMN_PARENT_LINKS
#include "jsmn.h"

static bool json_token_streq(const char *json, const jsmntok_t *token, const char *s)
{
    return token->type == JSMN_STRING &&
           (int)strlen(s) == token->end - token->start &&
           strncmp(json + token->start, s, token->end - token->start) == 0;
}

static void json_str(const char *json, const jsmntok_t *token, char *out, size_t out_size)
{
    int len = token->end - token->start;
    if (len >= (int)out_size)
    {
        len = (int)out_size - 1;
    }
    strncpy(out, json + token->start, len);
    out[len] = '\0';
}

static int jsonint(const char *json, const jsmntok_t *token)
{
    char temp[32];
    int len = token->end - token->start;
    if (len >= (int)sizeof(temp))
    {
        len = (int)sizeof(temp) - 1;
    }
    strncpy(temp, json + token->start, len);
    temp[len] = '\0';
    return atoi(temp);
}

static int skip_token_tree(const jsmntok_t *tokens, int token_count, int index)
{
    if (index >= token_count)
    {
        return index;
    }

    if (tokens[index].type == JSMN_OBJECT || tokens[index].type == JSMN_ARRAY)
    {
        int children_count = tokens[index].size;
        int next_index = index + 1;
        for (int i = 0; i < children_count; i++)
        {
            if (next_index >= token_count)
            {
                return token_count;
            }
            if (tokens[index].type == JSMN_OBJECT)
            {
                next_index = skip_token_tree(tokens, token_count, next_index);
                next_index = skip_token_tree(tokens, token_count, next_index);
            }
            else
            {
                next_index = skip_token_tree(tokens, token_count, next_index);
            }
        }
        return next_index;
    }

    return index + 1;
}

static CampaignFieldKey parse_campaign_field_key(const char *json, const jsmntok_t *token)
{
    if (token->type != JSMN_STRING)
    {
        return CAMPAIGN_FIELD_UNKNOWN;
    }
    if (json_token_streq(json, token, "name"))
        return CAMPAIGN_FIELD_NAME;
    if (json_token_streq(json, token, "campaign_name_id"))
        return CAMPAIGN_FIELD_CAMPAIGN_NAME_ID;
    if (json_token_streq(json, token, "description"))
        return CAMPAIGN_FIELD_DESCRIPTION;
    if (json_token_streq(json, token, "author"))
        return CAMPAIGN_FIELD_AUTHOR;
    if (json_token_streq(json, token, "start_map"))
        return CAMPAIGN_FIELD_START_MAP;
    if (json_token_streq(json, token, "tutorial_map"))
        return CAMPAIGN_FIELD_TUTORIAL_MAP;
    if (json_token_streq(json, token, "campaign_folder"))
        return CAMPAIGN_FIELD_CAMPAIGN_FOLDER;
    if (json_token_streq(json, token, "starterkit"))
        return CAMPAIGN_FIELD_STARTERKIT;
    if (json_token_streq(json, token, "intro_video"))
        return CAMPAIGN_FIELD_INTRO_VIDEO;
    if (json_token_streq(json, token, "avatar_type"))
        return CAMPAIGN_FIELD_AVATAR_TYPE;
    return CAMPAIGN_FIELD_UNKNOWN;
}

static bool parse_campaign_from_tokens(const char *json, const jsmntok_t *tokens,
                                       int token_count, CampaignJson *out_campaign)
{
    if (token_count < 1 || tokens[0].type != JSMN_OBJECT)
    {
        return false;
    }

    memset(out_campaign, 0, sizeof(CampaignJson));

    /* -1 means "unset" to every consumer of avatar_type. */
    out_campaign->def.avatar_type = -1;

    int pairs_to_process = tokens[0].size;
    int current_token_index = 1;

    for (int i = 0; i < pairs_to_process; i++)
    {
        if (current_token_index + 1 >= token_count)
        {
            return false;
        }

        CampaignFieldKey key = parse_campaign_field_key(json, &tokens[current_token_index]);
        const jsmntok_t *value_token = &tokens[current_token_index + 1];
        SFSF_CampaignDef *def = &out_campaign->def;

        switch (key)
        {
            case CAMPAIGN_FIELD_NAME:
                json_str(json, value_token, def->name, sizeof(def->name));
                out_campaign->found_name = true;
                break;
            case CAMPAIGN_FIELD_CAMPAIGN_NAME_ID:
            {
                int campaign_name_id = jsonint(json, value_token);
                def->campaign_name_id = campaign_name_id;
                out_campaign->found_campaign_name_id = true;
                break;
            }
            case CAMPAIGN_FIELD_DESCRIPTION:
                json_str(json, value_token, def->description, sizeof(def->description));
                out_campaign->found_description = true;
                break;
            case CAMPAIGN_FIELD_AUTHOR:
                json_str(json, value_token, def->author, sizeof(def->author));
                out_campaign->found_author = true;
                break;
            case CAMPAIGN_FIELD_START_MAP:
                json_str(json, value_token, def->start_map, sizeof(def->start_map));
                out_campaign->found_start_map = true;
                break;
            case CAMPAIGN_FIELD_TUTORIAL_MAP:
                json_str(json, value_token, def->tutorial_map, sizeof(def->tutorial_map));
                out_campaign->found_tutorial_map = true;
                break;
            case CAMPAIGN_FIELD_CAMPAIGN_FOLDER:
                json_str(json, value_token, def->campaign_folder, sizeof(def->campaign_folder));
                out_campaign->found_campaign_folder = true;
                break;
            case CAMPAIGN_FIELD_STARTERKIT:
                json_str(json, value_token, def->starterkit, sizeof(def->starterkit));
                out_campaign->found_starterkit = true;
                break;
            case CAMPAIGN_FIELD_INTRO_VIDEO:
                json_str(json, value_token, def->intro_video, sizeof(def->intro_video));
                out_campaign->found_intro_video = true;
                break;
            case CAMPAIGN_FIELD_AVATAR_TYPE:
            {
                int avatar_type = jsonint(json, value_token);
                def->avatar_type = avatar_type;
                out_campaign->found_avatar_type = true;
                break;
            }
            default:
                // Unknown key - skipped so future fields stay backwards compatible.
                break;
        }

        current_token_index = skip_token_tree(tokens, token_count, current_token_index); // key
        current_token_index = skip_token_tree(tokens, token_count, current_token_index); // value
    }

    return true;
}

bool parse_campaign_json_file(const char *path, CampaignJson *out_campaign)
{
    char *json_string = readfile(path);
    if (json_string == NULL)
    {
        // readfile has already logged the reason.
        return false;
    }

    jsmn_parser parser;
    jsmn_init(&parser);

    size_t json_length = strlen(json_string);
    int token_count = jsmn_parse(&parser, json_string, json_length, NULL, 0);
    if (token_count < 1)
    {
        log_error("Campaign JSON is not valid JSON: %s", path);
        log_debug(DEBUG_HIGH, json_string);
        free(json_string);
        return false;
    }

    jsmntok_t *tokens = (jsmntok_t *)calloc(token_count, sizeof(jsmntok_t));
    if (tokens == NULL)
    {
        log_error("Unable to allocate %d JSON tokens for: %s", token_count, path);
        free(json_string);
        return false;
    }

    jsmn_init(&parser);
    token_count = jsmn_parse(&parser, json_string, json_length, tokens, token_count);

    bool success = false;
    if (token_count < 1)
    {
        log_error("Campaign JSON failed to tokenize: %s", path);
        log_debug(DEBUG_HIGH, json_string);
    }
    else if (tokens[0].type != JSMN_OBJECT)
    {
        log_error("Campaign JSON must be a single JSON object: %s", path);
    }
    else
    {
        success = parse_campaign_from_tokens(json_string, tokens, token_count, out_campaign);
        if (!success)
        {
            log_error("Campaign JSON structure invalid: %s", path);
        }
    }

    free(tokens);
    free(json_string);
    return success;
}

/** @} */
