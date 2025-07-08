#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#define JSMN_PARENT_LINKS
#include "jsmn.h"
#include "../../api/structures/sf_building_structures.h"

#define MAX_POINTS 255
#define MAX_COLLISIONS 5
#define MAX_RESOURCES 5

typedef struct
{
    int32_t X;
    int32_t Y;
} WorldCoord;

typedef struct
{
    WorldCoord points[MAX_POINTS];
    int point_count;
    bool shadow;
} Collision;

typedef struct
{
    int amount;
    char type[32];
} Resource;

typedef struct
{
    int id;
    int building_required;
    bool can_enter;
    int center_x;
    int center_y;
    int collision_count;
    Collision collisions[MAX_COLLISIONS];
    int description_id;
    int flags;
    int health;
    int name_id;
    int race;
    Resource resources[MAX_RESOURCES];
    int resource_count;
    int slot_count;

    bool found_id;
    bool found_building_required;
    bool found_can_enter;
    bool found_center_x;
    bool found_center_y;
    bool found_collision_count;
    bool found_description_id;
    bool found_flags;
    bool found_health;
    bool found_name_id;
    bool found_race;
    bool found_slot_count;
} Building;

typedef enum
{
    FIELD_UNKNOWN,
    FIELD_ID,
    FIELD_BUILDING_REQUIRED,
    FIELD_CAN_ENTER,
    FIELD_CENTER_X,
    FIELD_CENTER_Y,
    FIELD_COLLISION_COUNT,
    FIELD_DESCRIPTION_ID,
    FIELD_FLAGS,
    FIELD_HEALTH,
    FIELD_NAME_ID,
    FIELD_RACE,
    FIELD_SLOT_COUNT,
    FIELD_COLLISIONS,
    FIELD_RESOURCES
} FieldKey;

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
        //FIXME: debug output
        fprintf(stderr, "Expected file \"%s\" not found", path);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    fseek(file, 0, SEEK_SET);

    //ACHTUNG -- memory allocation. Approach with care
    char *buffer = malloc(len + 1);
    if (buffer == NULL)
    {
        //FIXME: debug output
        fprintf(stderr, "Unable to allocate memory for file");
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

void parse_building(const char *json, jsmntok_t *tokens, int token_count, int building_index, Building *building)
{
    if (tokens[building_index].type != JSMN_OBJECT)
        return;

    init_building(building);

    int current_index = building_index + 1;
    int pairs_to_process = tokens[building_index].size;

    //printf("Parsing building object with %d key-value pairs\n", pairs_to_process);

    for (int i = 0; i < pairs_to_process && current_index < token_count; i++)
    {
        FieldKey key = parse_field_key(json, &tokens[current_index]);
        // printf("Processing key: %.*s\n",
        //        tokens[current_index].end - tokens[current_index].start,
        //        json + tokens[current_index].start);

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
}

// Print building information
void print_building(const Building *building)
{
    printf("\n=== BUILDING INFORMATION ===\n");

    if (building->found_id)
        printf("ID: %d\n", building->id);
    if (building->found_building_required)
        printf("Building Required: %d\n", building->building_required);
    if (building->found_can_enter)
        printf("Can Enter: %s\n", building->can_enter ? "true" : "false");
    if (building->found_center_x)
        printf("Center X: %d\n", building->center_x);
    if (building->found_center_y)
        printf("Center Y: %d\n", building->center_y);
    if (building->found_collision_count)
        printf("Collision Count: %d\n", building->collision_count);
    if (building->found_description_id)
        printf("Description ID: %d\n", building->description_id);
    if (building->found_flags)
        printf("Flags: %d\n", building->flags);
    if (building->found_health)
        printf("Health: %d\n", building->health);
    if (building->found_name_id)
        printf("Name ID: %d\n", building->name_id);
    if (building->found_race)
        printf("Race: %d\n", building->race);
    if (building->found_slot_count)
        printf("Slot Count: %d\n", building->slot_count);

    // Print collisions
    printf("Collisions:\n");
    for (int i = 0; i < building->collision_count && i < MAX_COLLISIONS; i++)
    {
        printf("  Collision %d (shadow: %s):\n", i, building->collisions[i].shadow ? "true" : "false");
        for (int j = 0; j < building->collisions[i].point_count; j++)
        {
            printf("    Point %d: (%d, %d)\n", j,
                   building->collisions[i].points[j].X,
                   building->collisions[i].points[j].Y);
        }
    }

    // Print resources
    if (building->resource_count > 0)
    {
        printf("Resources:\n");
        for (int i = 0; i < building->resource_count; i++)
        {
            printf("  Resource %d: %d %s\n", i,
                   building->resources[i].amount,
                   building->resources[i].type);
        }
    }

    printf("============================\n");
}


int main(void)
{
    printf("Loading Data.\n");
    const char *json_string = readfile("./building_data.json");
    jsmn_parser parser;
    jsmntok_t *tokens;

    if (json_string == NULL)
    {
        printf("No Data Exiting.\n");
        return -1;
    }

    printf("Parsing Data.\n");
    jsmn_init(&parser);
    uint32_t token_count = jsmn_parse(&parser, json_string, strlen(json_string), NULL, 0);
    jsmn_init(&parser);
    tokens = calloc(token_count, sizeof(jsmntok_t));
    jsmn_parse(&parser, json_string, strlen(json_string), tokens, token_count);

    printf("Token count: %u\n", token_count);
    printf("Trying to parse Buildings Data.\n");
    int buildings_index = -1;
    for (int i = 0; i < token_count; i++)
    {
        if (jsoneq(json_string, &tokens[i], "buildings"))
        {
            buildings_index = i + 1;
            printf("Found buildings array at index %d with %d buildings\n",
                   buildings_index, tokens[buildings_index].size);
            break;
        }
    }

    if (buildings_index == -1)
    {
        printf("No 'buildings' array found.\n");
        free((void *)json_string);
        free(tokens);
        return -1;
    }

    int building_count = tokens[buildings_index].size;
    int current_index = buildings_index + 1;

    for (int i = 0; i < building_count && current_index < token_count; i++)
    {
        printf("\n--- Processing building %d at token index %d ---\n", i, current_index);
        Building building;
        parse_building(json_string, tokens, token_count, current_index, &building);

        print_building(&building);

        current_index = skip_token_tree(tokens, token_count, current_index);
    }

    free((void *)json_string);
    free(tokens);
    return 0;
}
