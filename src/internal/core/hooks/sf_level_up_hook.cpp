#include "sf_level_up_hook.h"
#include "../../../asi/sf_asi.h"

#include "../../registry/sf_level_up_registry.h"
#include "string.h"
#include "stdint.h"

extern initInvContainerUpdateMsg_ptr initInvContainerUpdateMsg; //0x216c30
extern serializeInvContainerUpdateMsg_ptr serializeInvContainerUpdateMsg; //0x219630
extern disposeInvContainerUpdateMsg_ptr disposeInvContainerUpdateMsg; //0x222950
extern sendToPlayer_ptr sendToPlayer; //0x2613d0
extern dispose_UpdatesVector_ptr dispose_UpdatesVector;
extern setSlot_ptr setSlot;                    //0x248dd0
extern invVectorPushBack_ptr invVectorPushBack;//0x233080


typedef uint32_t (__thiscall *findPlayerItem_ptr)(void *_this,uint32_t player_id, uint16_t item_id,
                                                  GtInvSlot *out_container,uint16_t *out_slot,InvSlotEntry *out_entry);
typedef uint16_t (__thiscall *findFreeSlot_ptr)(void *_this, uint32_t player_id, GtInvSlot container, uint32_t unused);

typedef uint32_t (__thiscall *serializeAvatarUpdate_ptr)(void *_this, void *param_1, uint32_t param_2);
typedef uint32_t (__thiscall *getAvatar_ptr)(void *_this, uint32_t player_id, GdAvatar *out);

findPlayerItem_ptr findPlayerItem; //0x2465a0
findFreeSlot_ptr findFreeSlot; //0x246210
serializeAvatarUpdate_ptr serializeAvatarUpdate; //0x218b10
getAvatar_ptr getAvatar; //0x24fcc0

void __thiscall learnSkillOnLevelUp_hook (AutoClass6 *_this, uint32_t player_id, GdFigureAbility *skill,
                                          uint32_t notify)
{
    uint16_t ability_id = 0;
    GdAvatar avatar;
    memset(&avatar, 0, sizeof(avatar));

    for (int i = 0; i < 10; i++)
    {
        avatar.internal.abilities[i].id = 0xff;
        avatar.internal.abilities[i].spec = 0xff;
        avatar.internal.abilities[i].level = 0xff;
    }
    if (getAvatar(_this->db, player_id, &avatar))
    {
        ability_id = getLearnedAbility(skill->id, skill->spec, skill->level);
    }
    if (ability_id != 0)
    {
        InvContainerUpdateMsg update_msg;
        initInvContainerUpdateMsg(&update_msg);

        InvSlotEntry found_entry;
        found_entry.item_id = 0;
        found_entry.count = 0;
        found_entry.flags = 0;

        GtInvSlot found_container;
        uint16_t slot;

        if (!findPlayerItem(_this->db, player_id, ability_id, &found_container, &slot, &found_entry))
        {
            slot = findFreeSlot(_this->db, player_id, kGtInvSlotSpellBook, 0);
            if (slot != (uint16_t)(-1))
            {
                InvSlotUpdate slot_update;

                slot_update.entry.count = 1;
                slot_update.entry.flags = 0;
                slot_update.entry.item_id = ability_id;
                setSlot(_this->db, player_id, kGtInvSlotSpellBook, slot, &slot_update.entry);
                if (notify != 0)
                {
                    update_msg.first_slot = slot;
                    update_msg.container = kGtInvSlotSpellBook;
                    update_msg.slot_count = 1;

                    slot_update.entry.flags = 0;
                    slot_update.slot = update_msg.first_slot;
                    slot_update.entry.count = 1;
                    slot_update.entry.item_id = ability_id;

                    invVectorPushBack(&update_msg.updates, &slot_update);
                    update_msg.is_slot_list = 1;
                    uint32_t total_size = serializeInvContainerUpdateMsg(&update_msg, _this->send_buffer,
                                                                         _this->send_buffer_size);
                    sendToPlayer(_this->net, player_id, 8009, _this->send_buffer, total_size);

                    total_size = serializeAvatarUpdate(&player_id, _this->send_buffer, _this->send_buffer_size);
                    sendToPlayer(_this->net, player_id, 8102, _this->send_buffer, total_size);
                }
            }
        }
        if ((uint32_t)update_msg.updates.begin != 0)
        {
            dispose_UpdatesVector(update_msg.updates.begin,
                                  ((uint32_t)update_msg.updates.capacity_end - (uint32_t)update_msg.updates.begin) / 7);
        }
    }
}

void init_lvl_up_functions()
{
    findPlayerItem = (findPlayerItem_ptr) (ASI::AddrOf(0x2465a0));
    findFreeSlot = (findFreeSlot_ptr) (ASI::AddrOf(0x246210));
    serializeAvatarUpdate = (serializeAvatarUpdate_ptr) (ASI::AddrOf(0x218b10));
    getAvatar = (getAvatar_ptr) (ASI::AddrOf(0x24fcc0));
}
