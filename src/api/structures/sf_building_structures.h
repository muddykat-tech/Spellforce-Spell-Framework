#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "../sf_general_structures.h"

/* |-============= Buildings Structures ==========- |*/

typedef struct BuildingAuxEntry BuildingAuxEntry;

struct __attribute__((packed)) BuildingAuxEntry
{
    BuildingAuxEntry *first;
    BuildingAuxEntry *data;
    BuildingAuxEntry *postlast;
    uint8_t unknown[2];
    uint8_t id;
    uint32_t centerX;
    uint32_t centerY;
    uint8_t shadows[5];
    uint_list_node collisions[5];
    uint8_t poly_count;
    uint8_t resource_req_type[5];
    uint16_t resource_req_amount[5];
    uint8_t resource_req_num;
    uint8_t padding[3];
};

typedef struct __attribute__((packed))
{
    BuildingAuxEntry *data;
    uint8_t flag;

} BuildingAuxEntry_related;

typedef struct __attribute__((packed))
{
    SF_Coord position;
    uint16_t owner;
    uint8_t type;
    uint8_t race;
    uint8_t flags;
    uint16_t build_up;
    uint16_t build_max;
    uint16_t health_current;
    uint16_t life_current_max;
    uint16_t health_max;
    uint16_t DLLNode;
    uint16_t xdata_key;
    uint16_t angle;
    uint32_t unknown[5];
    uint8_t worker_count;
    uint8_t worker_count_max;
    uint8_t level;
    SF_Coord entry_point;
} GdBuilding;

struct __attribute__((packed)) SF_CGdBuilding
{
    AutoClass14 *autoClass14;
    GdBuilding buildings[1000];
    uint16_t max_used;
    void *astruct_31;
    uint32_t unknown1;
    void *astruct_39;
    uint32_t unknown2;
};

typedef struct astruct32 astruct32;
typedef struct BuildingResource BuildingResource;
struct __attribute__((packed)) BuildingResource
{
    uint16_t id;
    uint8_t race;
    uint8_t can_enter;
    uint8_t slot_count;
    uint16_t health;
    uint16_t name_id;
    uint16_t worker_cycle;
    uint16_t building_required;
    uint16_t initial_angle;
    uint16_t ext_description_id;
    uint8_t flags;
};

struct __attribute__((packed)) astruct32
{
    astruct32 *ref1;
    astruct32 *ref2;
    astruct32 *ref3;
    uint16_t padding_1;
    uint8_t id;
    BuildingResource building;
    uint8_t padding_2[3];
};

struct __attribute__((packed)) SF_CGdBuildingToolbox
{
    void *Decals;
    void *CGdAStar;
    AutoClass14 *OpaqueClass;
    SF_CGdBuilding *CGdBuilding;
    void *CGdDoubleLinkList;
    SF_CGdEffect *CGdEffect;
    void *AutoClass30;
    SF_CGdFigure *CGdFigure;
    SF_CGdFigureJobs *CGdFigureJobs;
    SF_CGdFigureToolbox *CGdFigureToolbox;
    void *AutoClass34;
    void *CGdObject;
    SF_CGdPlayer *CGdPlayer;
    void *AutoClass47;
    void *CGdResource;
    void *AutoClass22;
    SF_CGdWorld *CGdWorld;
    SF_CGdWorldToolBox *CGdWorldToolbox;
    void *CGdXDataList;
    uint32_t unknown1[3];
    uint8_t unknown2[800];
    uint8_t unknown3[800];
    uint8_t unknown4[14884];
    uint8_t unknown5[14884];
    uint8_t unknown6[3721];
};

struct __attribute__((packed)) SF_CGdResource
{
    astruct32 *building_map;
    void *unknown1;
    BuildingAuxEntry *building_aux_map;
    uint32_t not_annotated[46];
};

typedef void (__thiscall *addBuilding_ptr)(void *_this,void *struct1, void *struct2);
typedef void (__thiscall *addBuildingAuxData_ptr)(void *_this, BuildingAuxEntry_related *entry, uint8_t *building_id);
typedef void (__thiscall *setCollisionListSize_ptr)(uint_list_node *_this, uint8_t list_size);
