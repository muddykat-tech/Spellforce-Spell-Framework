#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "sf_screens_loader.h"
#include "sf_building_loader.h"
#include "sf_wrappers.h"
#include "sf_hooks.h"

// Prevent duplicate link error
#define JSMN_STATIC
#define JSMN_PARENT_LINKS
#include "jsmn.h"

static ScreenEntry g_screen_entries[MAX_SCREEN_ENTRIES];
static int g_screen_entry_count = 0;

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
        len = (int)out_size - 1;
    strncpy(out, json + token->start, len);
    out[len] = '\0';
}

static bool parse_screens_json(const char *json_string)
{
    jsmn_parser parser;
    jsmntok_t *tokens;

    jsmn_init(&parser);
    int token_count = jsmn_parse(&parser, json_string, strlen(json_string), NULL, 0);
    tokens = (jsmntok_t *)calloc(token_count, sizeof(jsmntok_t));
    jsmn_init(&parser);
    token_count = jsmn_parse(&parser, json_string, strlen(json_string), tokens, token_count);

    if (token_count < 1)
    {
        log_error("Failed to parse screens.json (Enable DEBUG HIGH to view raw JSON string.)");
        log_debug(DEBUG_HIGH, json_string);
        free(tokens);
        return false;
    }

    if (tokens[0].type != JSMN_OBJECT)
    {
        log_error("screens.json must be a single JSON object of map_name : msb_file pairs");
        free(tokens);
        return false;
    }

    int pairs_to_process = tokens[0].size;
    int current_token_index = 1;

    for (int i = 0; i < pairs_to_process; i++)
    {
        if (g_screen_entry_count >= MAX_SCREEN_ENTRIES)
        {
            log_error("screens.json has more than %d entries, ignoring the rest", MAX_SCREEN_ENTRIES);
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
            ScreenEntry *entry = &g_screen_entries[g_screen_entry_count];
            json_str(json_string, key_token, entry->map_name, sizeof(entry->map_name));
            json_str(json_string, value_token, entry->msb_file, sizeof(entry->msb_file));
            str_to_lower(entry->map_name);
            g_screen_entry_count++;
        }

        current_token_index += 2;
    }

    free(tokens);
    return true;
}

void load_screens_json()
{
    g_screen_entry_count = 0;

    char currentDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDir);
    char path[MAX_PATH];
    snprintf(path, sizeof(path), "%s\\sfsf\\screens.json", currentDir);

    char *json_str_data = readfile(path);
    if (!json_str_data)
    {
        log_info("No screens.json found at: %s (custom loading screens disabled)", path);
        return;
    }

    if (parse_screens_json(json_str_data))
    {
        log_info("Loaded %d custom loading screen(s) from screens.json", g_screen_entry_count);
    }

    free(json_str_data);
}

const char *find_screen_for_map(const char *map_name)
{
    if (!map_name)
        return NULL;

    char lowered[SCREEN_MAP_NAME_LEN];
    size_t len = strlen(map_name);
    if (len >= sizeof(lowered))
        len = sizeof(lowered) - 1;
    strncpy(lowered, map_name, len);
    lowered[len] = '\0';
    str_to_lower(lowered);

    for (int i = 0; i < g_screen_entry_count; i++)
    {
        if (strcmp(g_screen_entries[i].map_name, lowered) == 0)
        {
            return g_screen_entries[i].msb_file;
        }
    }

    return NULL;
}
