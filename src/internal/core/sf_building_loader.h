#ifndef BUILDING_LOADER_H
#define BUILDING_LOADER_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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

extern "C" const char *readfile(const char *path);
bool parse_building_json_entrypoint(const char *building_json_name, Building *out_building);

#endif
