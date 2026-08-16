/**
 * @addtogroup ShaderLoader
 * @{
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sf_shader_loader.h"
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

static bool json_key_is(const char *json, const jsmntok_t *token, const char *name)
{
    int len = token->end - token->start;
    return (token->type == JSMN_STRING &&
            (int)strlen(name) == len &&
            strncmp(json + token->start, name, len) == 0);
}

static bool json_bool(const char *json, const jsmntok_t *token, bool fallback)
{
    char buffer[16];
    json_str(json, token, buffer, sizeof(buffer));
    if (strcasecmp(buffer, "true") == 0)
    {
        return true;
    }
    if (strcasecmp(buffer, "false") == 0)
    {
        return false;
    }
    return fallback;
}

/**
 * @brief Fills one ShaderEntry from a JSMN_OBJECT token.
 * @return Token index just past the object.
 */
static int parse_shader_object(const char *json, const jsmntok_t *tokens,
                               int token_count, int object_index,
                               ShaderEntry *out_entry, int declaration_order)
{
    memset(out_entry, 0, sizeof(*out_entry));
    out_entry->enabled = true;
    out_entry->order = declaration_order;

    int pairs_to_process = tokens[object_index].size;
    int current_token_index = object_index + 1;

    for (int i = 0; i < pairs_to_process; i++)
    {
        if (current_token_index + 1 >= token_count)
        {
            break;
        }

        const jsmntok_t *key_token = &tokens[current_token_index];
        const jsmntok_t *value_token = &tokens[current_token_index + 1];

        if (json_key_is(json, key_token, "name"))
        {
            json_str(json, value_token, out_entry->name, sizeof(out_entry->name));
            str_to_lower(out_entry->name);
        }
        else if (json_key_is(json, key_token, "vertex"))
        {
            json_str(json, value_token, out_entry->vertex_file, sizeof(out_entry->vertex_file));
        }
        else if (json_key_is(json, key_token, "fragment"))
        {
            json_str(json, value_token, out_entry->fragment_file, sizeof(out_entry->fragment_file));
        }
        else if (json_key_is(json, key_token, "enabled"))
        {
            out_entry->enabled = json_bool(json, value_token, true);
        }
        else if (json_key_is(json, key_token, "order"))
        {
            char buffer[16];
            json_str(json, value_token, buffer, sizeof(buffer));
            out_entry->order = atoi(buffer);
        }
        else
        {
            char unknown[64];
            json_str(json, key_token, unknown, sizeof(unknown));
            log_debug(DEBUG_MED, "| - shaders.json: ignoring unknown key \"%s\"", unknown);
        }

        current_token_index = skip_token_tree(tokens, token_count, current_token_index); // key
        current_token_index = skip_token_tree(tokens, token_count, current_token_index); // value
    }

    return current_token_index;
}

bool parse_shaders_json_file(const char *path, ShaderEntry *out_entries,
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
        log_error("shaders.json is not valid JSON: %s", path);
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
        log_error("shaders.json failed to tokenize: %s", path);
        log_debug(DEBUG_HIGH, json_string);
    }
    else if (tokens[0].type != JSMN_OBJECT)
    {
        log_error("shaders.json must be a single JSON object containing a \"shaders\" array");
    }
    else
    {
        /* Find the "shaders" array at the top level. */
        int array_index = -1;
        int pairs_to_process = tokens[0].size;
        int current_token_index = 1;

        for (int i = 0; i < pairs_to_process; i++)
        {
            if (current_token_index + 1 >= token_count)
            {
                break;
            }
            if (json_key_is(json_string, &tokens[current_token_index], "shaders") &&
                tokens[current_token_index + 1].type == JSMN_ARRAY)
            {
                array_index = current_token_index + 1;
                break;
            }
            current_token_index = skip_token_tree(tokens, token_count, current_token_index);
            current_token_index = skip_token_tree(tokens, token_count, current_token_index);
        }

        if (array_index < 0)
        {
            log_error("shaders.json has no \"shaders\" array");
        }
        else
        {
            int entries_to_process = tokens[array_index].size;
            int entry_token_index = array_index + 1;

            for (int i = 0; i < entries_to_process; i++)
            {
                if (entry_token_index >= token_count)
                {
                    break;
                }

                if (*out_count >= max_entries)
                {
                    log_error("shaders.json has more than %d entries, ignoring the rest", max_entries);
                    break;
                }

                if (tokens[entry_token_index].type != JSMN_OBJECT)
                {
                    log_error("shaders.json entry %d is not an object, skipping", i);
                    entry_token_index = skip_token_tree(tokens, token_count, entry_token_index);
                    continue;
                }

                entry_token_index = parse_shader_object(json_string, tokens, token_count,
                                                        entry_token_index,
                                                        &out_entries[*out_count], i);
                (*out_count)++;
            }

            success = true;
        }
    }

    free(tokens);
    free(json_string);
    return success;
}

/** @} */
