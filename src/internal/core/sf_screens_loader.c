/**
 * @addtogroup ScreensLoader
 * @{
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sf_screens_loader.h"
#include "sf_building_loader.h" /* readfile() */
#include "sf_wrappers.h"

#define JSMN_STATIC
#define JSMN_PARENT_LINKS
#include "jsmn.h"

static void str_to_lower(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

// Copies a JSON string token's contents into a bounded buffer.
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

bool parse_screens_json_file(const char *path, ScreenEntry *out_entries,
                             int max_entries, int *out_count)
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
        log_error("screens.json is not valid JSON: %s", path);
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
        log_error("screens.json failed to tokenize: %s", path);
        log_debug(DEBUG_HIGH, json_string);
    }
    else if (tokens[0].type != JSMN_OBJECT)
    {
        log_error("screens.json must be a single JSON object of map_name : msb_file pairs");
    }
    else
    {
        int pairs_to_process = tokens[0].size;
        int current_token_index = 1;

        for (int i = 0; i < pairs_to_process; i++)
        {
            if (current_token_index + 1 >= token_count)
            {
                break;
            }

            if (*out_count >= max_entries)
            {
                log_error("screens.json has more than %d entries, ignoring the rest", max_entries);
                break;
            }

            const jsmntok_t *key_token = &tokens[current_token_index];
            const jsmntok_t *value_token = &tokens[current_token_index + 1];

            if (key_token->type != JSMN_STRING || value_token->type != JSMN_STRING)
            {
                log_error("screens.json entry %d is not a string:string pair, skipping", i);
            }
            else
            {
                ScreenEntry *entry = &out_entries[*out_count];
                json_str(json_string, key_token, entry->map_name, sizeof(entry->map_name));
                json_str(json_string, value_token, entry->msb_file, sizeof(entry->msb_file));
                str_to_lower(entry->map_name);
                (*out_count)++;
            }

            current_token_index += 2;
        }

        success = true;
    }

    free(tokens);
    free(json_string);
    return success;
}

/** @} */
