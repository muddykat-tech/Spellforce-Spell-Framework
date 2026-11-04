/**
 * @addtogroup AbilityLoader
 * @{
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sf_ability_loader.h"
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

static AbilityFieldKey parse_ability_field_key(const char *json, const jsmntok_t *token)
{
    if (token->type != JSMN_STRING)
    {
        return ABILITY_FIELD_UNKNOWN;
    }
    if (json_token_streq(json, token, "skill"))
        return ABILITY_FIELD_SKILL;
    if (json_token_streq(json, token, "spec"))
        return ABILITY_FIELD_SPEC;
    if (json_token_streq(json, token, "level"))
        return ABILITY_FIELD_LEVEL;
    if (json_token_streq(json, token, "ability_id"))
        return ABILITY_FIELD_ABILITY_ID;
    return ABILITY_FIELD_UNKNOWN;
}

bool parse_ability_json_file(const char *path, AbilityJsonRecord *out_records, int max_records,
                             int *out_count)
{
    *out_count = 0;

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
        log_error("Abilities JSON is not valid JSON: %s", path);
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
        log_error("Abilities JSON failed to tokenize: %s", path);
        log_debug(DEBUG_HIGH, json_string);
    }
    else if (tokens[0].type != JSMN_ARRAY)
    {
        log_error("Abilities JSON must be a single JSON array of records: %s", path);
    }
    else
    {
        int record_count = tokens[0].size;
        int current_token_index = 1;

        for (int i = 0; i < record_count; i++)
        {
            if (current_token_index >= token_count)
            {
                break;
            }

            if (*out_count >= max_records)
            {
                log_error("Abilities JSON has more than %d records, ignoring the rest", max_records);
                break;
            }

            if (tokens[current_token_index].type != JSMN_OBJECT)
            {
                log_error("Abilities JSON record #%d is not an object, skipping", i);
                current_token_index = skip_token_tree(tokens, token_count, current_token_index);
                continue;
            }

            AbilityJsonRecord record;
            memset(&record, 0, sizeof(record));
            bool record_broken = false;

            int pairs_to_process = tokens[current_token_index].size;
            int pair_index = current_token_index + 1;

            for (int j = 0; j < pairs_to_process; j++)
            {
                if (pair_index + 1 >= token_count)
                {
                    record_broken = true;
                    break;
                }

                AbilityFieldKey key = parse_ability_field_key(json_string, &tokens[pair_index]);
                const jsmntok_t *value_token = &tokens[pair_index + 1];

                if (key != ABILITY_FIELD_UNKNOWN)
                {
                    if (value_token->type != JSMN_PRIMITIVE)
                    {
                        log_error("Abilities JSON record #%d: field value is not a number, skipping record", i);
                        record_broken = true;
                    }
                    else
                    {
                        int value = jsonint(json_string, value_token);
                        switch (key)
                        {
                            case ABILITY_FIELD_SKILL:
                                record.skill = value;
                                record.found_skill = true;
                                break;
                            case ABILITY_FIELD_SPEC:
                                record.spec = value;
                                record.found_spec = true;
                                break;
                            case ABILITY_FIELD_LEVEL:
                                record.level = value;
                                record.found_level = true;
                                break;
                            case ABILITY_FIELD_ABILITY_ID:
                                record.ability_id = value;
                                record.found_ability_id = true;
                                break;
                            default:
                                break;
                        }
                    }
                }
                /* Unknown keys are skipped so future fields stay backwards compatible. */

                pair_index = skip_token_tree(tokens, token_count, pair_index);     // key
                pair_index = skip_token_tree(tokens, token_count, pair_index);     // value
            }

            if (!record_broken)
            {
                out_records[*out_count] = record;
                (*out_count)++;
            }

            current_token_index = skip_token_tree(tokens, token_count, current_token_index);
        }

        success = true;
    }

    free(tokens);
    free(json_string);
    return success;
}

/** @} */
