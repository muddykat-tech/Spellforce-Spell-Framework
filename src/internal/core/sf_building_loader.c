
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

int jsoneq(const char *json, jsmntok_t *tok, const char *s)
{
    if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0)
    {
        return 1;
    }
    return 0;
}

int jsonint(const char *json, jsmntok_t *tok)
{
    char buffer[32];
    int len = tok->end - tok->start;
    if (len >= sizeof(buffer))
        len = sizeof(buffer) - 1;
    strncpy(buffer, json + tok->start, len);
    buffer[len] = '\0';
    return atoi(buffer);
}

void jsonstr(const char *json, jsmntok_t *tok, char *buffer, int buffer_size)
{
    int len = tok->end - tok->start;
    if (len >= buffer_size)
        len = buffer_size - 1;
    strncpy(buffer, json + tok->start, len);
    buffer[len] = '\0';
}

int jsonbool(const char *json, jsmntok_t *tok)
{
    return (tok->type == JSMN_PRIMITIVE && json[tok->start] == 't');
}

int skip_token_tree(jsmntok_t *tokens, int token_count, int index)
{
    if (index >= token_count)
        return index;

    jsmntok_t *token = &tokens[index];
    int next_index = index + 1;

    // For primitives and strings, just move to next token
    if (token->type == JSMN_PRIMITIVE || token->type == JSMN_STRING)
    {
        return next_index;
    }

    // For arrays and objects, recursively skip all children
    if (token->type == JSMN_ARRAY)
    {
        for (int i = 0; i < token->size && next_index < token_count; i++)
        {
            next_index = skip_token_tree(tokens, token_count, next_index);
        }
    }
    else if (token->type == JSMN_OBJECT)
    {
        for (int i = 0; i < token->size && next_index < token_count; i++)
        {
            // Skip key
            next_index = skip_token_tree(tokens, token_count, next_index);
            // Skip value
            if (next_index < token_count)
            {
                next_index = skip_token_tree(tokens, token_count, next_index);
            }
        }
    }

    return next_index;
}


FieldKey parse_field_key(const char *json, jsmntok_t *tok)
{
    if (jsoneq(json, tok, "id"))
        return FIELD_ID;
    if (jsoneq(json, tok, "building_required"))
        return FIELD_BUILDING_REQUIRED;
    if (jsoneq(json, tok, "can_enter"))
        return FIELD_CAN_ENTER;
    if (jsoneq(json, tok, "center_x"))
        return FIELD_CENTER_X;
    if (jsoneq(json, tok, "center_y"))
        return FIELD_CENTER_Y;
    if (jsoneq(json, tok, "collision_count"))
        return FIELD_COLLISION_COUNT;
    if (jsoneq(json, tok, "description_id"))
        return FIELD_DESCRIPTION_ID;
    if (jsoneq(json, tok, "flags"))
        return FIELD_FLAGS;
    if (jsoneq(json, tok, "health"))
        return FIELD_HEALTH;
    if (jsoneq(json, tok, "name_id"))
        return FIELD_NAME_ID;
    if (jsoneq(json, tok, "race"))
        return FIELD_RACE;
    if (jsoneq(json, tok, "slot_count"))
        return FIELD_SLOT_COUNT;
    if (jsoneq(json, tok, "collisions"))
        return FIELD_COLLISIONS;
    if (jsoneq(json, tok, "resources"))
        return FIELD_RESOURCES;
    return FIELD_UNKNOWN;
}

void parse_point(const char *json, jsmntok_t *tokens, int token_count, int point_index, WorldCoord *point)
{
    if (tokens[point_index].type != JSMN_OBJECT)
        return;

    int current_index = point_index + 1;
    int pairs_to_process = tokens[point_index].size;

    for (int i = 0; i < pairs_to_process && current_index < token_count; i++)
    {
        if (jsoneq(json, &tokens[current_index], "x"))
        {
            point->X = jsonint(json, &tokens[current_index + 1]);
        }
        else if (jsoneq(json, &tokens[current_index], "y"))
        {
            point->Y = jsonint(json, &tokens[current_index + 1]);
        }

        // Skip key and value
        current_index = skip_token_tree(tokens, token_count, current_index);
        current_index = skip_token_tree(tokens, token_count, current_index);
    }
}

int find_key_in_object(const char *json, jsmntok_t *tokens, int obj_index, const char *key)
{
    if (tokens[obj_index].type != JSMN_OBJECT)
    {
        return -1;
    }

    int key_count = tokens[obj_index].size;
    int current_index = obj_index + 1;

    for (int i = 0; i < key_count; i++)
    {
        if (current_index >= obj_index + tokens[obj_index].size * 2 + 1)
            break;

        // Check if this is the key we're looking for
        if (jsoneq(json, &tokens[current_index], key))
        {
            return current_index + 1; // Return index of the value
        }

        // Skip this key-value pair
        current_index++; // Move past the key
        if (current_index < obj_index + tokens[obj_index].size * 2 + 1)
        {
            // Skip the value and all its children
            current_index += tokens[current_index].size + 1;
        }
    }

    return -1; // Key not found
}

int skip_token_and_children(jsmntok_t *tokens, int token_count, int index)
{
    if (index >= token_count)
        return index;

    jsmntok_t *token = &tokens[index];
    int next_index = index + 1;

    // For arrays and objects, we need to skip all children
    if (token->type == JSMN_ARRAY || token->type == JSMN_OBJECT)
    {
        int children_to_skip = token->size;

        if (token->type == JSMN_OBJECT)
        {
            children_to_skip *= 2; // Objects have key-value pairs
        }

        for (int i = 0; i < children_to_skip && next_index < token_count; i++)
        {
            next_index = skip_token_and_children(tokens, token_count, next_index);
        }
    }

    return next_index;
}

const char *readfile(const char *path)
{
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        log_debug(DEBUG_MED, "Expected file \"%s\" not found", path);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(len + 1);
    if (buffer == NULL)
    {
        log_debug(DEBUG_MED, "Unable to allocate memory for file");
        fclose(file);
        return NULL;
    }
    fread(buffer, 1, len, file);
    buffer[len] = '\0';

    return (const char *)buffer;
}

const char *extractToken(const char *json_string, jsmntok_t *token)
{
    char *result = calloc(token->size+1, 1);
    strncpy(result, json_string+token->start, token->size);
    return result;
}

void parse_collision(const char *json, jsmntok_t *tokens, int token_count, int collision_index, Collision *collision)
{
    if (tokens[collision_index].type != JSMN_OBJECT)
        return;

    collision->point_count = 0;
    collision->shadow = false;

    int current_index = collision_index + 1;
    int pairs_to_process = tokens[collision_index].size;

    for (int i = 0; i < pairs_to_process && current_index < token_count; i++)
    {
        if (jsoneq(json, &tokens[current_index], "points"))
        {
            int points_array_index = current_index + 1;
            if (tokens[points_array_index].type == JSMN_ARRAY)
            {
                int point_count = tokens[points_array_index].size;
                if (point_count > MAX_POINTS)
                    point_count = MAX_POINTS;

                int point_index = points_array_index + 1;
                for (int j = 0; j < point_count && point_index < token_count; j++)
                {
                    parse_point(json, tokens, token_count, point_index, &collision->points[j]);
                    collision->point_count++;
                    point_index = skip_token_tree(tokens, token_count, point_index);
                }
            }
        }
        else if (jsoneq(json, &tokens[current_index], "shadow"))
        {
            collision->shadow = jsonbool(json, &tokens[current_index + 1]);
        }

        // Skip key and value
        current_index = skip_token_tree(tokens, token_count, current_index);
        current_index = skip_token_tree(tokens, token_count, current_index);
    }
}

void parse_resources(const char *json, jsmntok_t *tokens, int token_count, int resources_index, Building *building)
{
    if (tokens[resources_index].type != JSMN_OBJECT)
        return;

    int current_index = resources_index + 1;
    int pairs_to_process = tokens[resources_index].size;

    for (int i = 0; i < pairs_to_process && current_index < token_count; i++)
    {
        if (jsoneq(json, &tokens[current_index], "list"))
        {
            int list_array_index = current_index + 1;
            if (tokens[list_array_index].type == JSMN_ARRAY)
            {
                int resource_count = tokens[list_array_index].size;
                if (resource_count > MAX_RESOURCES)
                    resource_count = MAX_RESOURCES;

                int resource_index = list_array_index + 1;
                for (int j = 0; j < resource_count && resource_index < token_count; j++)
                {
                    if (tokens[resource_index].type == JSMN_OBJECT)
                    {
                        int res_current_index = resource_index + 1;
                        int res_pairs_to_process = tokens[resource_index].size;

                        for (int k = 0; k < res_pairs_to_process && res_current_index < token_count; k++)
                        {
                            if (jsoneq(json, &tokens[res_current_index], "amount"))
                            {
                                building->resources[building->resource_count].amount = jsonint(json,
                                                                                               &tokens[res_current_index
                                                                                                       + 1]);
                            }
                            else if (jsoneq(json, &tokens[res_current_index], "type"))
                            {
                                jsonstr(json, &tokens[res_current_index + 1],
                                        building->resources[building->resource_count].type,
                                        sizeof(building->resources[building->resource_count].type));
                            }

                            res_current_index = skip_token_tree(tokens, token_count, res_current_index);
                            res_current_index = skip_token_tree(tokens, token_count, res_current_index);
                        }
                        building->resource_count++;
                    }
                    resource_index = skip_token_tree(tokens, token_count, resource_index);
                }
            }
        }

        // Skip key and value
        current_index = skip_token_tree(tokens, token_count, current_index);
        current_index = skip_token_tree(tokens, token_count, current_index);
    }
}

bool parse_building_json(const char *json, jsmntok_t *tokens, int token_count, int building_index, Building *building)
{
    if (tokens[building_index].type != JSMN_OBJECT)
        return false;

    init_building(building);

    int current_index = building_index + 1;
    int pairs_to_process = tokens[building_index].size;

    for (int i = 0; i < pairs_to_process && current_index < token_count; i++)
    {
        FieldKey key = parse_field_key(json, &tokens[current_index]);

        switch (key)
        {
            case FIELD_ID:
                building->id = jsonint(json, &tokens[current_index + 1]);
                building->found_id = true;
                break;
            case FIELD_BUILDING_REQUIRED:
                building->building_required = jsonint(json, &tokens[current_index + 1]);
                building->found_building_required = true;
                break;
            case FIELD_CAN_ENTER:
                building->can_enter = jsonbool(json, &tokens[current_index + 1]);
                building->found_can_enter = true;
                break;
            case FIELD_CENTER_X:
                building->center_x = jsonint(json, &tokens[current_index + 1]);
                building->found_center_x = true;
                break;
            case FIELD_CENTER_Y:
                building->center_y = jsonint(json, &tokens[current_index + 1]);
                building->found_center_y = true;
                break;
            case FIELD_COLLISION_COUNT:
                building->collision_count = jsonint(json, &tokens[current_index + 1]);
                building->found_collision_count = true;
                break;
            case FIELD_DESCRIPTION_ID:
                building->description_id = jsonint(json, &tokens[current_index + 1]);
                building->found_description_id = true;
                break;
            case FIELD_FLAGS:
                building->flags = jsonint(json, &tokens[current_index + 1]);
                building->found_flags = true;
                break;
            case FIELD_HEALTH:
                building->health = jsonint(json, &tokens[current_index + 1]);
                building->found_health = true;
                break;
            case FIELD_NAME_ID:
                building->name_id = jsonint(json, &tokens[current_index + 1]);
                building->found_name_id = true;
                break;
            case FIELD_RACE:
                building->race = jsonint(json, &tokens[current_index + 1]);
                building->found_race = true;
                break;
            case FIELD_SLOT_COUNT:
                building->slot_count = jsonint(json, &tokens[current_index + 1]);
                building->found_slot_count = true;
                break;
            case FIELD_COLLISIONS:
            {
                int collisions_array_index = current_index + 1;
                if (tokens[collisions_array_index].type == JSMN_ARRAY)
                {
                    int collision_count = tokens[collisions_array_index].size;
                    if (collision_count > MAX_COLLISIONS)
                        collision_count = MAX_COLLISIONS;

                    int collision_index = collisions_array_index + 1;
                    for (int j = 0; j < collision_count && collision_index < token_count; j++)
                    {
                        parse_collision(json, tokens, token_count, collision_index, &building->collisions[j]);
                        collision_index = skip_token_tree(tokens, token_count, collision_index);
                    }
                }
                break;
            }
            case FIELD_RESOURCES:
                parse_resources(json, tokens, token_count, current_index + 1, building);
                break;

            default:
                // Unknown key — optionally log or skip
                break;
        }

        current_index = skip_token_tree(tokens, token_count, current_index);
        current_index = skip_token_tree(tokens, token_count, current_index);
    }
    return true;
}

bool parse_building_json_entrypoint(const char *building_json_name, Building *out_building)
{
    char currentDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDir);
    char path[MAX_PATH];
    snprintf(path, sizeof(path), "%s\\buildings\\%s.json", currentDir, building_json_name);

    const char *json_str = readfile(path);
    if (!json_str)
    {
        log_error("Failed to read: %s", path);
        return false;
    }

    jsmn_parser parser;
    jsmntok_t *tokens = (jsmntok_t *)malloc(sizeof(jsmntok_t) * 512);
    if (!tokens)
    {
        log_error("Memory allocation failed for tokens");
        free((void *)json_str);
        return false;
    }

    jsmn_init(&parser);
    int count = jsmn_parse(&parser, json_str, strlen(json_str), tokens, 512);

    if (count < 0)
    {
        log_error("JSON parse failed for: %s", building_json_name);
        free(tokens);
        free((void *)json_str);
        return false;
    }

    memset(out_building, 0, sizeof(Building));
    bool success = parse_building_json(json_str, tokens, count, 0, out_building);
    if (!success)
    {
        log_error("Building JSON structure invalid: %s", building_json_name);
    }

    free(tokens);
    free((void *)json_str);
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
