#pragma once
#include "../../../api/sfsf.h"

#include <vector>

typedef struct __attribute__((packed))
{
    uint16_t item_id;
    uint8_t item_type;
    uint8_t item_subtype;
    uint16_t name_text_id;
    uint16_t unit_stats_id;
    uint16_t army_stats_id_maybe;
    uint16_t building_id_maybe;
    uint8_t unk_0c;
    uint32_t sell_value_maybe;
    uint32_t buy_value_maybe;
    uint8_t set_type_maybe;
} ItemGeneralData;

typedef struct __attribute__((packed))
{
    uint16_t unit_stats_id;
    GdFigureAbility skills[10];
} UnitSkills;

typedef struct __attribute__((packed))
{
    uint16_t creo;
    uint8_t level;
    uint16_t head;
    wchar_t name[40];
    uint16_t strength, stamina, charisma, agility, dexterity, wisdom, intelligence;
    uint16_t res_fire, res_ice, res_black, res_mind;
    uint16_t walk_speed, fight_speed, cast_speed, scaling;
    GdFigureAbility skills[10];
    uint32_t is_female;
} CDbHeroRecord;

typedef struct __attribute__((packed))
{
    uint16_t item_id;
    uint16_t count;
    uint8_t flags;
} InvSlotEntry;

typedef struct __attribute__((packed))
{
    uint16_t slot;
    InvSlotEntry entry;
} InvSlotUpdate;

template <typename T> struct GameVector { T *begin, *end, *capacity_end; };   // game std::vector (game allocator!)

typedef struct __attribute__((packed))
{
    GameVector<InvSlotUpdate> updates;
    GtInvSlot container;
    uint8_t pad_0d;
    uint16_t first_slot;
    uint8_t slot_count;
    uint8_t is_slot_list;
} InvContainerUpdateMsg;

typedef struct __attribute__((packed))
{
    void *db;
    void *net;
    uint8_t send_buffer[0x7000];
    int send_buffer_size;
} AutoClass6;
