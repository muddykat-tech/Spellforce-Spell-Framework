/**
 * @addtogroup WeaponTypeLoader
 * @{
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sf_weapon_type_loader.h"
#include "sf_building_loader.h" /* readfile() */
#include "sf_wrappers.h"

#define JSMN_STATIC
#define JSMN_PARENT_LINKS
#include "jsmn.h"

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

static bool json_token_streq(const char *json, const jsmntok_t *token, const char *s)
{
    return token->type == JSMN_STRING &&
           (int)strlen(s) == token->end - token->start &&
           strncmp(json + token->start, s, token->end - token->start) == 0;
}

/** @brief Returns the index just past @p index and everything nested under it. */
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
                next_index = skip_token_tree(tokens, token_count, next_index); // key
                next_index = skip_token_tree(tokens, token_count, next_index); // value
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

static void copy_json_string(char *dst, size_t dst_len, const char *json, const jsmntok_t *token)
{
    int len = token->end - token->start;
    if (len >= (int)dst_len)
    {
        len = (int)dst_len - 1;
    }
    strncpy(dst, json + token->start, len);
    dst[len] = '\0';
}

bool parse_weapon_type_json_file(const char *path, WeaponTypeJsonRecord *out_record)
{
    memset(out_record, 0, sizeof(*out_record));

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
        log_error("Weapon type JSON is not valid JSON: %s", path);
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
        log_error("Weapon type JSON failed to tokenize: %s", path);
        log_debug(DEBUG_HIGH, json_string);
    }
    else if (tokens[0].type != JSMN_OBJECT)
    {
        log_error("Weapon type JSON must be a single JSON object: %s", path);
    }
    else
    {
        bool record_broken = false;

        int pairs_to_process = tokens[0].size;
        int pair_index = 1;

        for (int j = 0; j < pairs_to_process; j++)
        {
            if (pair_index + 1 >= token_count)
            {
                record_broken = true;
                break;
            }

            const jsmntok_t *key_token = &tokens[pair_index];
            const jsmntok_t *value_token = &tokens[pair_index + 1];

            if (json_token_streq(json_string, key_token, "id"))
            {
                if (value_token->type != JSMN_PRIMITIVE)
                {
                    log_error("Weapon type JSON: \"id\" is not a number, file %s", path);
                    record_broken = true;
                }
                else
                {
                    out_record->id = jsonint(json_string, value_token);
                    out_record->found_id = true;
                }
            }
            else if (json_token_streq(json_string, key_token, "name"))
            {
                if (value_token->type != JSMN_STRING)
                {
                    log_error("Weapon type JSON: \"name\" is not a string, file %s", path);
                    record_broken = true;
                }
                else
                {
                    copy_json_string(out_record->name, sizeof(out_record->name), json_string,
                                     value_token);
                    out_record->found_name = true;
                }
            }
            else if (json_token_streq(json_string, key_token, "attack_style"))
            {
                if (value_token->type != JSMN_STRING)
                {
                    log_error("Weapon type JSON: \"attack_style\" is not a string, file %s", path);
                    record_broken = true;
                }
                else
                {
                    copy_json_string(out_record->attack_style, sizeof(out_record->attack_style),
                                     json_string, value_token);
                    out_record->found_attack_style = true;
                }
            }
            /* Unknown keys are skipped silently so future fields stay backwards compatible. */

            pair_index = skip_token_tree(tokens, token_count, pair_index);     // key
            pair_index = skip_token_tree(tokens, token_count, pair_index);     // value
        }

        success = !record_broken;
    }

    free(tokens);
    free(json_string);
    return success;
}

/** @} */
