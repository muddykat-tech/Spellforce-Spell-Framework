
#include "sf_building_loader.h"
#include "sf_wrappers.h"
#include "sf_modloader.h"
#include "sf_hooks.h"

#define JSMN_PARENT_LINKS
#include "jsmn.h"

void init_building(Building *building)
{
    memset(building, 0, sizeof(building));
    building->id = -1;
    building->building_required = -1;
    building->can_enter = false;
    building->center_x = -1;
    building->center_y = -1;
    building->collision_count = -1;
    building->description_id = -1;
    building->flags = -1;
    building->health = -1;
    building->name_id = -1;
    building->race = -1;
    building->slot_count = -1;
    building->resource_count = 0;
}

// Compares a JSON token string with a given C string.
static bool json_token_streq(const char *json, const jsmntok_t *token, const char *s)
{
    return token->type == JSMN_STRING && (int)strlen(s) == token->end - token->start && strncmp(json + token->start, s,
                                                                                                token->end -
                                                                                                token->start) == 0;
}

// Converts a JSON token to an integer.
static int jsonint(const char *json, const jsmntok_t *token)
{
    char temp[32];
    int len = token->end - token->start;
    if (len >= 32)
        len = 31;
    strncpy(temp, json + token->start, len);
    temp[len] = '\0';
    return atoi(temp);
}

// Converts a JSON token to a boolean.
static bool jsonbool(const char *json, const jsmntok_t *token)
{
    if (json_token_streq(json, token, "true"))
    {
        return true;
    }
    return false;
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

// Maps a JSON key string to the FieldKey enum.
static FieldKey parse_field_key(const char *json, const jsmntok_t *token)
{
    if (token->type != JSMN_STRING)
    {
        return FIELD_UNKNOWN;
    }
    if (json_token_streq(json, token, "id"))
        return FIELD_ID;
    if (json_token_streq(json, token, "building_required"))
        return FIELD_BUILDING_REQUIRED;
    if (json_token_streq(json, token, "can_enter"))
        return FIELD_CAN_ENTER;
    if (json_token_streq(json, token, "center_x"))
        return FIELD_CENTER_X;
    if (json_token_streq(json, token, "center_y"))
        return FIELD_CENTER_Y;
    if (json_token_streq(json, token, "collision_count"))
        return FIELD_COLLISION_COUNT;
    if (json_token_streq(json, token, "collisions"))
        return FIELD_COLLISIONS;
    if (json_token_streq(json, token, "description_id"))
        return FIELD_DESCRIPTION_ID;
    if (json_token_streq(json, token, "flags"))
        return FIELD_FLAGS;
    if (json_token_streq(json, token, "health"))
        return FIELD_HEALTH;
    if (json_token_streq(json, token, "name_id"))
        return FIELD_NAME_ID;
    if (json_token_streq(json, token, "race"))
        return FIELD_RACE;
    if (json_token_streq(json, token, "resources"))
        return FIELD_RESOURCES;
    if (json_token_streq(json, token, "slot_count"))
        return FIELD_SLOT_COUNT;
    if (json_token_streq(json, token, "shadow"))
        return FIELD_SHADOW;
    if (json_token_streq(json, token, "points"))
        return FIELD_POINTS;
    if (json_token_streq(json, token, "amount"))
        return FIELD_AMOUNT;
    if (json_token_streq(json, token, "type"))
        return FIELD_TYPE;
    if (json_token_streq(json, token, "list"))
        return FIELD_LIST;
    if (json_token_streq(json, token, "number"))
        return FIELD_NUMBER;
    return FIELD_UNKNOWN;
}

// Forward declarations for parsing functions
bool parse_collision(const char *json, const jsmntok_t *tokens, int token_count, int start_index, Collision *collision);
bool parse_resource(const char *json, const jsmntok_t *tokens, int token_count, int start_index, Resource *resource);
bool parse_resources(const char *json, const jsmntok_t *tokens, int token_count, int start_index, Building *building);
bool parse_world_coord(const char *json, const jsmntok_t *tokens, int token_count, int start_index, WorldCoord *coord);

// Parses a single WorldCoord object.
bool parse_world_coord(const char *json, const jsmntok_t *tokens, int token_count, int start_index, WorldCoord *coord)
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
        FieldKey key = parse_field_key(json, &tokens[current_index]);
        if (key == FIELD_UNKNOWN)
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
            case FIELD_X:
                coord->X = jsonint(json, value_token);
                break;
            case FIELD_Y:
                coord->Y = jsonint(json, value_token);
                break;
            default:
                break;
        }
        current_index = skip_token_tree(tokens, token_count, current_index);
        current_index = skip_token_tree(tokens, token_count, current_index);
    }
    return true;
}

// Parses a single Collision object, including its nested points array.
bool parse_collision(const char *json, const jsmntok_t *tokens, int token_count, int start_index, Collision *collision)
{
    if (start_index >= token_count || tokens[start_index].type != JSMN_OBJECT)
    {
        return false;
    }
    collision->point_count = 0;
    int pair_count = tokens[start_index].size;
    int current_index = start_index + 1;
    for (int i = 0; i < pair_count; i++)
    {
        if (current_index >= token_count)
            return false;
        FieldKey key = parse_field_key(json, &tokens[current_index]);

        switch (key)
        {
            case FIELD_SHADOW:
                collision->shadow = jsonbool(json, &tokens[current_index + 1]);
                break;
            case FIELD_POINTS:
            {
                int points_array_index = current_index + 1;
                if (points_array_index >= token_count || tokens[points_array_index].type != JSMN_ARRAY)
                {
                    break;
                }
                int point_count = tokens[points_array_index].size;
                if (point_count > MAX_POINTS)
                    point_count = MAX_POINTS;

                int point_index = points_array_index + 1;
                for (int j = 0; j < point_count; j++)
                {
                    if (point_index >= token_count)
                        break;
                    if (parse_world_coord(json, tokens, token_count, point_index, &collision->points[j]))
                    {
                        collision->point_count++;
                    }
                    point_index = skip_token_tree(tokens, token_count, point_index);
                }
                break;
            }
            default:
                break;
        }
        current_index = skip_token_tree(tokens, token_count, current_index);
        current_index = skip_token_tree(tokens, token_count, current_index);
    }
    return true;
}

// Parses a single Resource object.
bool parse_resource(const char *json, const jsmntok_t *tokens, int token_count, int start_index, Resource *resource)
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
        FieldKey key = parse_field_key(json, &tokens[current_index]);

        switch (key)
        {
            case FIELD_AMOUNT:
                resource->amount = jsonint(json, &tokens[current_index + 1]);
                break;
            case FIELD_TYPE:
            {
                const jsmntok_t *type_token = &tokens[current_index + 1];
                int len = type_token->end - type_token->start;
                if (len >= 32)
                    len = 31;
                strncpy(resource->type, json + type_token->start, len);
                resource->type[len] = '\0';
            }
            break;
            default:
                break;
        }
        current_index = skip_token_tree(tokens, token_count, current_index);
        current_index = skip_token_tree(tokens, token_count, current_index);
    }
    return true;
}

// Parses the resources object and its nested list.
bool parse_resources(const char *json, const jsmntok_t *tokens, int token_count, int start_index, Building *building)
{
    if (start_index >= token_count || tokens[start_index].type != JSMN_OBJECT)
    {
        return false;
    }
    building->resource_count = 0;
    int pair_count = tokens[start_index].size;
    int current_index = start_index + 1;
    for (int i = 0; i < pair_count; i++)
    {
        if (current_index >= token_count)
            return false;
        FieldKey key = parse_field_key(json, &tokens[current_index]);

        switch (key)
        {
            case FIELD_NUMBER:
                // No need to parse this, as we'll count the actual items in the list.
                break;
            case FIELD_LIST:
            {
                int list_array_index = current_index + 1;
                if (list_array_index >= token_count || tokens[list_array_index].type != JSMN_ARRAY)
                {
                    break;
                }
                int list_count = tokens[list_array_index].size;
                if (list_count > MAX_RESOURCES)
                    list_count = MAX_RESOURCES;

                int list_item_index = list_array_index + 1;
                for (int j = 0; j < list_count; j++)
                {
                    if (list_item_index >= token_count)
                        break;
                    if (parse_resource(json, tokens, token_count, list_item_index, &building->resources[j]))
                    {
                        building->resource_count++;
                    }
                    list_item_index = skip_token_tree(tokens, token_count, list_item_index);
                }
                break;
            }
            default:
                break;
        }
        current_index = skip_token_tree(tokens, token_count, current_index);
        current_index = skip_token_tree(tokens, token_count, current_index);
    }
    return true;
}

bool parse_building_from_tokens(const char *json, jsmntok_t *tokens, int token_count, Building *building);

bool parse_building_json(const char *json_string, Building *building)
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
    return parse_building_from_tokens(json_string, tokens, token_count, building);
}

bool parse_building_from_tokens(const char *json, jsmntok_t *tokens, int token_count, Building *building)
{
    // The root of the JSON must be an object.
    if (tokens[0].type != JSMN_OBJECT)
    {
        return false;
    }

    init_building(building);

    int pairs_to_process = tokens[0].size;
    // Start iterating from the first key, which is at index 1
    int current_token_index = 1;

    for (int i = 0; i < pairs_to_process; i++)
    {
        // Boundary check for the key
        if (current_token_index >= token_count)
            return false;

        FieldKey key = parse_field_key(json, &tokens[current_token_index]);
        int value_index = current_token_index + 1;

        // Boundary check for the value
        if (value_index >= token_count)
            return false;

        switch (key)
        {
            case FIELD_ID:
                building->id = jsonint(json, &tokens[value_index]);
                building->found_id = true;
                break;
            case FIELD_BUILDING_REQUIRED:
                building->building_required = jsonint(json, &tokens[value_index]);
                building->found_building_required = true;
                break;
            case FIELD_CAN_ENTER:
                building->can_enter = jsonbool(json, &tokens[value_index]);
                building->found_can_enter = true;
                break;
            case FIELD_CENTER_X:
                building->center_x = jsonint(json, &tokens[value_index]);
                building->found_center_x = true;
                break;
            case FIELD_CENTER_Y:
                building->center_y = jsonint(json, &tokens[value_index]);
                building->found_center_y = true;
                break;
            case FIELD_COLLISION_COUNT:
                building->collision_count = jsonint(json, &tokens[value_index]);
                building->found_collision_count = true;
                break;
            case FIELD_DESCRIPTION_ID:
                building->description_id = jsonint(json, &tokens[value_index]);
                building->found_description_id = true;
                break;
            case FIELD_FLAGS:
                building->flags = jsonint(json, &tokens[value_index]);
                building->found_flags = true;
                break;
            case FIELD_HEALTH:
                building->health = jsonint(json, &tokens[value_index]);
                building->found_health = true;
                break;
            case FIELD_NAME_ID:
                building->name_id = jsonint(json, &tokens[value_index]);
                building->found_name_id = true;
                break;
            case FIELD_RACE:
                building->race = jsonint(json, &tokens[value_index]);
                building->found_race = true;
                break;
            case FIELD_SLOT_COUNT:
                building->slot_count = jsonint(json, &tokens[value_index]);
                building->found_slot_count = true;
                break;
            case FIELD_COLLISIONS:
            {
                building->collision_count = 0;
                int collisions_array_index = value_index;
                if (tokens[collisions_array_index].type == JSMN_ARRAY)
                {
                    int collisions_to_parse = tokens[collisions_array_index].size;
                    if (collisions_to_parse > MAX_COLLISIONS)
                        collisions_to_parse = MAX_COLLISIONS;

                    int collision_token_index = collisions_array_index + 1;
                    for (int j = 0; j < collisions_to_parse; j++)
                    {
                        if (parse_collision(json, tokens, token_count, collision_token_index, &building->collisions[j]))
                        {
                            building->collision_count++;
                        }
                        collision_token_index = skip_token_tree(tokens, token_count, collision_token_index);
                    }
                }
                break;
            }
            case FIELD_RESOURCES:
                parse_resources(json, tokens, token_count, value_index, building);
                break;
            default:
                // Unknown key, just skip it
                break;
        }

        // Advance the current index past the key and its entire value tree
        current_token_index = skip_token_tree(tokens, token_count, current_token_index); // Skip key
        current_token_index = skip_token_tree(tokens, token_count, current_token_index); // Skip value
    }

    return true;
}

char *readfile(const char *path)
{
    FILE *file = fopen(path, "rb");
    if(file == NULL)
    {
        log_error("Expected file \"%s\" not found\n", path);
        return NULL;
    }

    fseek(file,0,SEEK_END);
    long len = ftell(file);
    fseek(file,0,SEEK_SET);

    if(len <= 0)
    {
        log_error("Empty or unreadable file: \"%s\"\n",path);
        fclose(file);
        return NULL;
    }

    // To ensure C++ doesn't warn us here
    char *buffer = (char *) malloc(len+1);
    if(buffer == NULL)
    {
        log_error("Unable to allocate memory for file: \"%s\"\n",path);
        fclose(file);
        return NULL;
    }

    size_t read_len = fread(buffer, 1, len, file);
    fclose(file);

    if(read_len != (size_t)len)
    {
        log_error("File Read Incomplete: \"%s\" (Expected %ld bytes, got %zu bytes)\n", path, len, read_len);
        free(buffer);
        return NULL;
    }

    buffer[read_len] = '\0';

    char *last_brace = strrchr(buffer, '}');
    if(last_brace != NULL)
    {
        last_brace[1] = '\0';
    }
    else
    {
        log_error("Invalid JSON: No closing brace found in \"%s\"\n", path);
        free(buffer);
        return NULL;
    }

    return buffer;
}

bool parse_building_json_entrypoint(const char *building_json_name, const char *mod_id, Building *out_building)
{
    char currentDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDir);
    char path[MAX_PATH];
    snprintf(path, sizeof(path), "%s\\sfsf\\%s\\buildings\\%s.json", currentDir, mod_id, building_json_name);

    char *json_str = readfile(path);
    if (!json_str)
    {
        log_error("Failed to read: %s", path);
        return false;
    }

    memset(out_building, 0, sizeof(Building));
    bool success = parse_building_json(json_str, out_building);
    if (!success)
    {
        log_error("Building JSON structure invalid: %s", building_json_name);
    }

    free(json_str);
    return success;
}

// Print building information
void print_building(const Building *building)
{
    log_debug(DEBUG_MED,"\n=== BUILDING INFORMATION ===\n");

    if (building->found_id)
        log_debug(DEBUG_MED,"ID: %d\n", building->id);
    if (building->found_building_required)
        log_debug(DEBUG_MED,"Building Required: %d\n", building->building_required);
    if (building->found_can_enter)
        log_debug(DEBUG_MED,"Can Enter: %s\n", building->can_enter ? "true" : "false");
    if (building->found_center_x)
        log_debug(DEBUG_MED,"Center X: %d\n", building->center_x);
    if (building->found_center_y)
        log_debug(DEBUG_MED,"Center Y: %d\n", building->center_y);
    if (building->found_collision_count)
        log_debug(DEBUG_MED,"Collision Count: %d\n", building->collision_count);
    if (building->found_description_id)
        log_debug(DEBUG_MED,"Description ID: %d\n", building->description_id);
    if (building->found_flags)
        log_debug(DEBUG_MED,"Flags: %d\n", building->flags);
    if (building->found_health)
        log_debug(DEBUG_MED,"Health: %d\n", building->health);
    if (building->found_name_id)
        log_debug(DEBUG_MED,"Name ID: %d\n", building->name_id);
    if (building->found_race)
        log_debug(DEBUG_MED,"Race: %d\n", building->race);
    if (building->found_slot_count)
        log_debug(DEBUG_MED,"Slot Count: %d\n", building->slot_count);

    // Print collisions
    log_debug(DEBUG_MED,"Collisions:\n");
    for (int i = 0; i < building->collision_count && i < MAX_COLLISIONS; i++)
    {
        log_debug(DEBUG_MED,"  Collision %d (shadow: %s):\n", i, building->collisions[i].shadow ? "true" : "false");
        for (int j = 0; j < building->collisions[i].point_count; j++)
        {
            log_debug(DEBUG_MED,"    Point %d: (%d, %d)\n", j,
                      building->collisions[i].points[j].X,
                      building->collisions[i].points[j].Y);
        }
    }

    // Print resources
    if (building->resource_count > 0)
    {
        log_debug(DEBUG_MED,"Resources:\n");
        for (int i = 0; i < building->resource_count; i++)
        {
            log_debug(DEBUG_MED,"  Resource %d: %d %s\n", i,
                      building->resources[i].amount,
                      building->resources[i].type);
        }
    }

    log_debug(DEBUG_MED,"============================\n");
}
