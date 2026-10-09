#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sf_hero_loader.h"
#include "sf_wrappers.h"


#define JSMN_STATIC
#define JSMN_PARENT_LINKS
#include "jsmn.h"

extern "C" char *readfile(const char *path);


void init_hero(CustomHero *hero)
{
    memset(hero, 0, sizeof(CustomHero));
}

static bool json_token_streq(const char *json, const jsmntok_t *token, const char *s)
{
    return token->type == JSMN_STRING &&
           (int)strlen(s) == token->end - token->start &&
           strncmp(json + token->start, s, token->end - token->start) == 0;
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

// Skips a token and all of its children, returning the next index to parse.
static int skip_token_tree(const jsmntok_t *tokens, int token_count, int index)
{
    if (index >= token_count)
    {
        return index;
    }

    if (tokens[index].type == JSMN_PRIMITIVE || tokens[index].type == JSMN_STRING)
    {
        return index + 1;
    }
    else if (tokens[index].type == JSMN_OBJECT || tokens[index].type == JSMN_ARRAY)
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
                // For objects, we have a key-value pair, so we skip two children at a time.
                next_index = skip_token_tree(tokens, token_count, next_index); // Skip key
                next_index = skip_token_tree(tokens, token_count, next_index); // Skip value
            }
            else
            {
                // For arrays, we skip one child at a time.
                next_index = skip_token_tree(tokens, token_count, next_index);
            }
        }
        return next_index;
    }
    return index + 1;
}

static HeroFieldKey parse_hero_field_key(const char *json, const jsmntok_t *token)
{
    if (token->type != JSMN_STRING)
    {
        return HERO_FIELD_UNKNOWN;
    }
    if (json_token_streq(json, token, "creo_id"))
        return HERO_FIELD_CREO;
    if (json_token_streq(json, token, "slot"))
        return HERO_FIELD_SLOT;
    if (json_token_streq(json, token, "item_id"))
        return HERO_FIELD_ITEM_ID;
    if (json_token_streq(json, token, "gear"))
        return HERO_FIELD_GEAR;
    return HERO_FIELD_UNKNOWN;
}

bool parse_gear_entry(const char *json, const jsmntok_t *tokens, int token_count, int start_index,
                      HeroGearEntry *entry);

bool parse_gear_entry(const char *json, const jsmntok_t *tokens, int token_count, int start_index, HeroGearEntry *entry)
{
    if (start_index >= token_count || tokens[start_index].type != JSMN_OBJECT)
    {
        return false;
    }
    int pair_count = tokens[start_index].size;
    int current_index = start_index + 1;
    for (int i = 0; i < pair_count; i++)
    {
        if (current_index >= token_count)
            return false;
        HeroFieldKey key = parse_hero_field_key(json, &tokens[current_index]);
        if (key == HERO_FIELD_UNKNOWN)
        {
            current_index = skip_token_tree(tokens, token_count, current_index);
            current_index = skip_token_tree(tokens, token_count, current_index);
            continue;
        }

        const jsmntok_t *value_token = &tokens[current_index + 1];
        if (value_token->type != JSMN_PRIMITIVE)
        {
            current_index = skip_token_tree(tokens, token_count, current_index);
            current_index = skip_token_tree(tokens, token_count, current_index);
            continue;
        }

        switch (key)
        {
            case HERO_FIELD_SLOT: entry->slot = jsonint(json, value_token); break;
            case HERO_FIELD_ITEM_ID: entry->item_id = jsonint(json, value_token); break;
            default: break;
        }
        current_index = skip_token_tree(tokens, token_count, current_index);
        current_index = skip_token_tree(tokens, token_count, current_index);
    }
    return true;
}

bool parse_hero_from_tokens(const char *json, jsmntok_t *tokens, int token_count, CustomHero *hero);

bool parse_hero_json(const char *json_string, CustomHero *hero)
{

    jsmn_parser parser;
    jsmntok_t *tokens;

    jsmn_init(&parser);
    int token_count = jsmn_parse(&parser, json_string, strlen(json_string), NULL, 0);
    tokens = (jsmntok_t *)calloc(token_count, sizeof (jsmntok_t));
    jsmn_init(&parser);
    token_count = jsmn_parse(&parser, json_string, strlen(json_string), tokens, token_count);

    // jsmn_parse returns a negative value on error
    if (token_count < 0)
    {
        log_info("Failed to parse due to Negative token count (Enable DEBUG HIGH to view raw JSON string.)");
        log_debug(DEBUG_HIGH, json_string);
        return false;
    }

    // A valid building object must have at least one token (the object itself)
    if (token_count < 1)
    {
        log_info("No object found in JSON file. (Enable DEBUG HIGH to view raw JSON string.)");
        log_debug(DEBUG_HIGH, json_string);
        return false;
    }

    // Delegate the actual struct population to the helper function
    return parse_hero_from_tokens(json_string, tokens, token_count, hero);
}

bool parse_hero_from_tokens(const char *json, jsmntok_t *tokens, int token_count, CustomHero *hero)
{
    // The root of the JSON must be an object.
    if (tokens[0].type != JSMN_OBJECT)
    {
        return false;
    }

    init_hero(hero);

    int pairs_to_process = tokens[0].size;
    // Start iterating from the first key, which is at index 1
    int current_token_index = 1;

    for (int i = 0; i < pairs_to_process; i++)
    {
        // Boundary check for the key
        if (current_token_index >= token_count)
            return false;

        HeroFieldKey key = parse_hero_field_key(json, &tokens[current_token_index]);
        int value_index = current_token_index + 1;

        // Boundary check for the value
        if (value_index >= token_count)
            return false;

        switch (key)
        {
            case HERO_FIELD_CREO:
            {
                hero->creo_id = jsonint(json, &tokens[current_token_index + 1]);
                break;
            }
            case HERO_FIELD_GEAR:
            {
                int points_array_index = current_token_index + 1;
                if (points_array_index >= token_count || tokens[points_array_index].type != JSMN_ARRAY)
                {
                    break;
                }
                int item_count = tokens[points_array_index].size;
                if (item_count > MAX_SLOTS)
                {
                    item_count = MAX_SLOTS;
                }
                int point_index = points_array_index + 1;
                for (int j = 0; j < item_count; j++)
                {
                    if (point_index >= token_count)
                        break;
                    if (parse_gear_entry(json, tokens, token_count, point_index, &hero->gear[j]))
                    {
                        hero->item_count++;
                    }
                    point_index = skip_token_tree(tokens, token_count, point_index);
                }
                break;
            }
            default: break;
        }

        // Advance the current index past the key and its entire value tree
        current_token_index = skip_token_tree(tokens, token_count, current_token_index); // Skip key
        current_token_index = skip_token_tree(tokens, token_count, current_token_index); // Skip value
    }
    return true;
}


bool parse_hero_json_entrypoint(const char *hero_json_name, const char *mod_id, CustomHero *out_hero)
{
    char currentDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDir);
    char path[MAX_PATH];
    snprintf(path, sizeof(path), "%s\\sfsf\\%s\\heroes\\%s.json", currentDir, mod_id, hero_json_name);

    char *json_str = readfile(path);
    if (!json_str)
    {
        log_error("Failed to read: %s", path);
        return false;
    }

    memset(out_hero, 0, sizeof(CustomHero));
    bool success = parse_hero_json(json_str, out_hero);
    if (!success)
    {
        log_error("Custom Hero JSON structure invalid: %s", hero_json_name);
    }

    free(json_str);
    return success;
}
