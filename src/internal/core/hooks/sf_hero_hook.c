
#include "sf_hero_hook.h"
#include "../../../asi/sf_asi.h"
#include <stdlib.h>
#include <stdio.h>
#include "../../registry/sf_hero_registry.h"

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
typedef void (__thiscall *dispose_ushort_vector_ptr)(void *_this, uint32_t size);
typedef void (__thiscall *dispose_UpdatesVector_ptr)(void *_this, uint32_t size);

getItemGeneral_ptr getItemGeneral;      //0x2494e0
getUnitStats_ptr getUnitStats;          //0x244720
getUnitSkills_ptr getUnitSkills;        //0x2446b0
getText_ptr getText;                    //0x252800
setHeroRecord_ptr setHeroRecord;        //0x245580
getUnitSpells_ptr getUnitSpells;        //0x244820
getSpellSlotItem_ptr getSpellSlotItem;  //0x252530
setSlot_ptr setSlot;                    //0x248dd0
setCharSlotData_ptr setCharSlotData;    //0x2509f0

invVectorPushBack_ptr invVectorPushBack;//0x233080
invVectorAssign_ptr invVectorAssign;    //0x222e10

initInvContainerUpdateMsg_ptr initInvContainerUpdateMsg; //0x216c30
serializeInvContainerUpdateMsg_ptr serializeInvContainerUpdateMsg; //0x219630
disposeInvContainerUpdateMsg_ptr disposeInvContainerUpdateMsg; //0x222950

sendToPlayer_ptr sendToPlayer; //0x2613d0

dispose_ushort_vector_ptr dispose_ushort_vector;
dispose_UpdatesVector_ptr dispose_UpdatesVector;

void init_hero_functions()
{
    getItemGeneral = (getItemGeneral_ptr)(ASI::AddrOf(0x2494e0));
    getUnitStats = (getUnitStats_ptr)(ASI::AddrOf(0x244720));
    getUnitSkills = (getUnitSkills_ptr)(ASI::AddrOf(0x2446b0));
    getText = (getText_ptr)(ASI::AddrOf(0x252800));
    setHeroRecord = (setHeroRecord_ptr)(ASI::AddrOf(0x245580));
    getUnitSpells = (getUnitSpells_ptr)(ASI::AddrOf(0x244820));
    getSpellSlotItem = (getSpellSlotItem_ptr)(ASI::AddrOf(0x252530));
    setSlot = (setSlot_ptr)(ASI::AddrOf(0x248dd0));
    setCharSlotData = (setCharSlotData_ptr)(ASI::AddrOf(0x2509f0));

    invVectorPushBack = (invVectorPushBack_ptr)(ASI::AddrOf(0x233080));
    invVectorAssign = (invVectorAssign_ptr)(ASI::AddrOf(0x222e10));

    initInvContainerUpdateMsg = (initInvContainerUpdateMsg_ptr)(ASI::AddrOf(0x216c30));
    serializeInvContainerUpdateMsg = (serializeInvContainerUpdateMsg_ptr)(ASI::AddrOf(0x219630));
    disposeInvContainerUpdateMsg = (disposeInvContainerUpdateMsg_ptr)(ASI::AddrOf(0x222950));

    sendToPlayer = (sendToPlayer_ptr)(ASI::AddrOf(0x2613d0));

    dispose_ushort_vector = (dispose_ushort_vector_ptr)(ASI::AddrOf(0x220ed0));
    dispose_UpdatesVector = (dispose_UpdatesVector_ptr)(ASI::AddrOf(0x221170));
}

uint8_t InvSlotToCharIndex(uint8_t invSlot)
{
    switch(invSlot)
    {
        case 2:
        case 10:
            return 0;
        case 3:
        case 11:
            return 1;
        case 4:
        case 12:
            return 2;
        case 5:
        case 13:
            return 3;
        case 6:
        case 14:
            return 4;
        case 7:
        case 15:
            return 5;
        default:
            return 0xff;
    }
}

void copyCreoStats(CDbHeroRecord *hero, CGdResourceUnitStats *stats)
{
    hero->creo = stats->creo;
    hero->level = stats->level;
    hero->agility = stats->agility;
    hero->dexterity = stats->dexterity;
    hero->charisma = stats->charisma;
    hero->intelligence = stats->intelligence;
    hero->stamina = stats->stamina;
    hero->strength = stats->strength;
    hero->wisdom = stats->wisdom;
    hero->res_black = stats->res_black;
    hero->res_fire = stats->res_fire;
    hero->res_ice = stats->res_ice;
    hero->res_mind = stats->res_mind;
    hero->cast_speed = stats->cast_speed;
    hero->fight_speed = stats->fight_speed;
    hero->walk_speed = stats->walk_speed;
    hero->scaling = stats->scaling;
    hero->head = stats->head;
    hero->is_female = stats->unit_flags & 1;
}

void initSlotUpdate(InvSlotUpdate *slot_update, uint16_t item_id, uint8_t slot)
{
    slot_update->entry.count = 1;
    slot_update->entry.flags = 0;
    slot_update->entry.item_id = item_id;
    slot_update->slot = slot;
}

void initSlotEntry(InvSlotEntry *slot_entry, uint16_t item_id)
{
    slot_entry->count = 1;
    slot_entry->flags = 0;
    slot_entry->item_id = item_id;
}


void __thiscall createHeroFromRuneHook(AutoClass6 *_this, uint32_t player_id, uint16_t hero_slot,
                                       uint16_t container_id, uint16_t rune_item_id, uint32_t no_notify)
{
    UnitSkills unit_skills;
    CDbHeroRecord hero;
    ItemGeneralData rune_item;
    CGdResourceUnitStats unit_stats;
    ushort_list_node unit_spells;
    char hero_name[40];

    GameVector<InvSlotUpdate> spell_updates;

    unit_spells.first = 0;
    unit_spells.data = 0;
    unit_spells.post_last = 0;

    spell_updates.begin = 0;
    spell_updates.end = 0;
    spell_updates.capacity_end = 0;

    for (int i = 0; i < 10; i++)
    {
        unit_skills.skills[i].id = 0xff;
        unit_skills.skills[i].spec = 0xff;
        unit_skills.skills[i].level = 0xff;
    }
    memset(&hero, 0, 0x95);

    rune_item.item_type = 0;
    rune_item.item_subtype = 0;

    if ((!getItemGeneral(_this->db, rune_item_id, &rune_item)) ||
        (!rune_item.unit_stats_id) ||
        (!getUnitStats(_this->db, rune_item.unit_stats_id, &unit_stats)))
    {
        return;
    }

    getUnitSkills(_this->db, unit_stats.creo, &unit_skills);
    copyCreoStats(&hero, &unit_stats);
    getText(_this->db, rune_item.name_text_id, hero_name, 40);
    mbstowcs(hero.name, hero_name, 40);
    for (int i = 0; i< 10; i++)
    {
        hero.skills[i].id = unit_skills.skills[i].id;
        hero.skills[i].level = unit_skills.skills[i].level;
        hero.skills[i].spec = unit_skills.skills[i].spec;
    }
    setHeroRecord(_this->db, player_id, &hero, hero_slot & 0xff);

    getUnitSpells(_this->db, hero.creo, &unit_spells);
    if ((unit_spells.data - unit_spells.first) >> 1 > 0)
    {
        uint32_t spell_count = 0;
        for (int index = 0; index < (unit_spells.data - unit_spells.first) >> 1; index++)
        {
            uint16_t spell_item_id = 0;
            InvSlotEntry slot_entry;
            if(getSpellSlotItem(_this->db, unit_spells.first[index], &spell_item_id))
            {
                slot_entry.count = 1;
                slot_entry.item_id = spell_item_id;
                slot_entry.flags = 0;
                setSlot(_this->db, player_id, (uint8_t)hero_slot + kGtInvSlotSpellMemoryChar1, index, &slot_entry);

                uint32_t slot_data_zero[2];
                slot_data_zero[0] = 0;
                slot_data_zero[1] = 0;

                uint8_t char_index = InvSlotToCharIndex(container_id);
                setCharSlotData(_this->db, player_id, char_index, hero_slot, &slot_data_zero);

                InvSlotUpdate slot_update;
                slot_update.entry.item_id = spell_item_id;
                slot_update.slot = index;
                slot_update.entry.count = 1;
                slot_update.entry.flags = 0;
                invVectorPushBack(&spell_updates, &slot_update);
            }
            spell_count++;
        }

        if ((spell_count != 0) && (no_notify == 0))
        {
            InvContainerUpdateMsg update_msg;
            initInvContainerUpdateMsg(&update_msg);
            update_msg.container = hero_slot + kGtInvSlotSpellMemoryChar1;
            update_msg.first_slot = 0;
            update_msg.slot_count = spell_count;
            invVectorAssign(&update_msg.updates, &spell_updates);
            update_msg.is_slot_list = 1;
            uint32_t msg_size = serializeInvContainerUpdateMsg(&update_msg, _this->send_buffer,
                                                               _this->send_buffer_size);
            sendToPlayer(_this->net, player_id, 0x1f49, _this->send_buffer, msg_size);
            disposeInvContainerUpdateMsg(&update_msg);
        }
    }
    InvContainerUpdateMsg update_msg;
    initInvContainerUpdateMsg(&update_msg);
    uint8_t slot_index = hero_slot + kGtInvSlotEquipmentChar1;
    update_msg.container = slot_index;
    update_msg.first_slot = 0;
    update_msg.slot_count = 0;
    update_msg.is_slot_list = 1;

    bool has_update = false;

    HeroGearEntry custom_gear[7];
    for (int i = 0; i < 7; i++)
    {
        custom_gear[i].slot = 0xff;
        custom_gear[i].item_id = 0;
    }
    if (getHeroGear(hero.creo, custom_gear))
    {
        has_update = true;
        for (int i = 0; i < 7; i++)
        {
            if (custom_gear[i].slot == 0xff)
            {
                break;
            }
            InvSlotEntry slot_entry;
            InvSlotUpdate slot_update;

            initSlotEntry(&slot_entry, custom_gear[i].item_id);
            setSlot(_this->db, player_id, slot_index, custom_gear[i].slot, &slot_entry);

            initSlotUpdate(&slot_update,  custom_gear[i].item_id, custom_gear[i].slot);
            invVectorPushBack(&update_msg.updates, &slot_update);
        }
    }

    if ((has_update) && (no_notify == 0))
    {
        uint32_t msg_size = serializeInvContainerUpdateMsg(&update_msg, _this->send_buffer, _this->send_buffer_size);
        sendToPlayer(_this->net, player_id, 0x1f49, _this->send_buffer, msg_size);
    }

    disposeInvContainerUpdateMsg(&update_msg);

    if (unit_spells.first !=0)
    {
        dispose_ushort_vector(unit_spells.first, ((uint32_t)unit_spells.post_last - (uint32_t)unit_spells.first) >> 1);
    }
    if (spell_updates.begin != 0)
    {
        dispose_UpdatesVector(spell_updates.begin,
                              ((uint32_t)spell_updates.capacity_end - (uint32_t)spell_updates.begin) / 7);
    }
}
