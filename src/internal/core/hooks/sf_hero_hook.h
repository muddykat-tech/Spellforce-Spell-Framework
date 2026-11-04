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


typedef uint32_t (__thiscall *getItemGeneral_ptr)(void *CDbLocal, uint16_t item_id, ItemGeneralData *out);
typedef uint32_t (__thiscall *getUnitStats_ptr)(void *CDbLocal, uint16_t unit_stats_id,
                                                CGdResourceUnitStats *out);
typedef uint32_t (__thiscall *getUnitSkills_ptr)(void *CDbLocal, uint16_t item_id, UnitSkills *out);

typedef uint32_t (__thiscall *getText_ptr)(void *CDbLocal, uint16_t text_id, char *out,uint16_t max_len);
typedef uint32_t (__thiscall *setHeroRecord_ptr)(void *CDbLocal, uint32_t player_id, CDbHeroRecord *record,
                                                 uint16_t hero_slot);
typedef uint32_t (__thiscall *getUnitSpells_ptr)(void *CDbLocal,  uint16_t item_id, ushort_list_node *out);
typedef uint32_t (__thiscall *getSpellSlotItem_ptr)(void *CDbLocal,  uint16_t spell_id, uint16_t *out);
typedef uint32_t (__thiscall *setSlot_ptr)(void *CDbLocal, uint32_t player_id, uint8_t container, uint16_t slot,
                                           InvSlotEntry *out);
typedef uint32_t (__thiscall *setCharSlotData_ptr)(void *CDbLocal, uint32_t player_id, uint8_t char_index,
                                                   uint16_t slot, void *out);

typedef void (__thiscall *invVectorPushBack_ptr)(GameVector<InvSlotUpdate> *_this, InvSlotUpdate *entry);
typedef InvContainerUpdateMsg *(__thiscall *initInvContainerUpdateMsg_ptr)(InvContainerUpdateMsg *_this);
typedef void (__thiscall *invVectorAssign_ptr)(GameVector<InvSlotUpdate> *_this, GameVector<InvSlotUpdate> *source);
typedef uint32_t (__thiscall *serializeInvContainerUpdateMsg_ptr)(InvContainerUpdateMsg *_this, void *buffer,
                                                                  uint32_t size);
typedef void (__thiscall *disposeInvContainerUpdateMsg_ptr)(InvContainerUpdateMsg *_this);
typedef void (__thiscall *sendToPlayer_ptr)(void *_this, uint32_t param1, uint32_t param2, void *param3,
                                            uint32_t param4);
typedef void (__stdcall *dispose_ushort_vector_ptr)(void *_this, uint32_t size);
typedef void (__stdcall *dispose_UpdatesVector_ptr)(void *_this, uint32_t size);

void __thiscall createHeroFromRuneHook(AutoClass6 *_this, uint32_t player_id, uint16_t hero_slot, uint16_t container_id,
                                       uint16_t rune_item_id, uint32_t no_notify);
void init_hero_functions();
