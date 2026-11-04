#include "sf_level_up_hook.h"
#include "../../../asi/sf_asi.h"

#include "../../registry/sf_level_up_registry.h"
#include "../sf_wrappers.h"
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

static findPlayerItem_ptr findPlayerItem; //0x2465a0
static findFreeSlot_ptr findFreeSlot;     //0x246210
static serializeAvatarUpdate_ptr serializeAvatarUpdate; //0x218b10
static getAvatar_ptr getAvatar;           //0x24fcc0

/**
 * @brief Every game function both level-up hooks rely on must be resolved before use.
 * @return false (with a warning) when initialization has not happened yet, so a hook
 * firing too early degrades into a skipped grant instead of an access violation.
 */
static bool ability_functions_ready()
{
    if (getAvatar == NULL || findPlayerItem == NULL || findFreeSlot == NULL || setSlot == NULL ||
        initInvContainerUpdateMsg == NULL || serializeInvContainerUpdateMsg == NULL ||
        sendToPlayer == NULL || serializeAvatarUpdate == NULL || invVectorPushBack == NULL ||
        dispose_UpdatesVector == NULL)
    {
        log_warning("LevelUpHook: game functions are not initialized yet, ability grant skipped");
        return false;
    }
    return true;
}

void __thiscall learnSkillOnLevelUp_hook (AutoClass6 *_this, uint32_t player_id, GdFigureAbility *skill,
                                          uint32_t notify)
{
    if (!ability_functions_ready())
    {
        return;
    }

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

/**
 * Grants the ability registered for the avatar's new level into the player's
 * spell book and notifies the client. Shares the registry and the grant flow
 * with learnSkillOnLevelUp_hook. Runs before the game's level-up branch: the
 * 0xFBC serialization and all packets of the branch stay game-owned, so each
 * send_buffer use stays sequential.
 */
void __thiscall avatar_level_up_hook_body(AutoClass6 *self, uint32_t leveled_up, uint32_t player_id)
{
    if (leveled_up == 0 || self == NULL || self->db == NULL)
    {
        return;
    }
    if (!ability_functions_ready())
    {
        return;
    }

    GdAvatar avatar;
    memset(&avatar, 0, sizeof(avatar));
    for (int i = 0; i < 10; i++)
    {
        avatar.internal.abilities[i].id = 0xff;
        avatar.internal.abilities[i].spec = 0xff;
        avatar.internal.abilities[i].level = 0xff;
    }
    if (!getAvatar(self->db, player_id, &avatar))
    {
        return;
    }

    uint16_t ability_id = getLearnedAbility(0, 0, avatar.internal.level);
    if (ability_id == 0)
    {
        return;
    }

    InvContainerUpdateMsg update_msg;
    initInvContainerUpdateMsg(&update_msg);

    InvSlotEntry found_entry;
    found_entry.item_id = 0;
    found_entry.count = 0;
    found_entry.flags = 0;

    GtInvSlot found_container;
    uint16_t slot;

    if (!findPlayerItem(self->db, player_id, ability_id, &found_container, &slot, &found_entry))
    {
        slot = findFreeSlot(self->db, player_id, kGtInvSlotSpellBook, 0);
        if (slot != (uint16_t)-1)
        {
            InvSlotUpdate slot_update;

            slot_update.entry.count = 1;
            slot_update.entry.flags = 0;
            slot_update.entry.item_id = ability_id;
            setSlot(self->db, player_id, kGtInvSlotSpellBook, slot, &slot_update.entry);

            update_msg.first_slot = slot;
            update_msg.container = kGtInvSlotSpellBook;
            update_msg.slot_count = 1;

            slot_update.entry.flags = 0;
            slot_update.slot = update_msg.first_slot;
            slot_update.entry.count = 1;
            slot_update.entry.item_id = ability_id;

            invVectorPushBack(&update_msg.updates, &slot_update);
            update_msg.is_slot_list = 1;
            uint32_t total_size = serializeInvContainerUpdateMsg(&update_msg, self->send_buffer,
                                                                 self->send_buffer_size);
            sendToPlayer(self->net, player_id, 8009, self->send_buffer, total_size);

            total_size = serializeAvatarUpdate(&player_id, self->send_buffer, self->send_buffer_size);
            sendToPlayer(self->net, player_id, 8102, self->send_buffer, total_size);
        }
        else
        {
            log_warning("AvatarLevelHook: spell book full, player %u", player_id);
        }
    }

    if ((uint32_t)update_msg.updates.begin != 0)
    {
        dispose_UpdatesVector(update_msg.updates.begin,
                              ((uint32_t)update_msg.updates.capacity_end -
                               (uint32_t)update_msg.updates.begin) / 7);
    }
}

/**
 * Asm part is limited to preparing the C/C++ hook body call and restoring the
 * stack afterwards. The stub replaces the guard comparison of the level-up
 * branch (cmp dword [ebp-0xc4], 0 @ 0x626071, 7 bytes patched as
 * call + 2x nop). Entry state: ESI holds AutoClass6*, EBX the party member
 * index, EBP the game frame. The member id is read from the party array and
 * passed to the __thiscall body together with the leveled-up flag (the body
 * pops both arguments itself via ret 8). Afterwards the stub re-executes the
 * overwritten comparison so the intact jz takes the same path as vanilla, and
 * returns into the two padding nops. ESP is net-zero by construction.
 */
static void __declspec(naked) avatar_level_up_hook_stub()
{
    asm("mov  -0x98(%%ebp),%%eax  \n\t" /* eax = party member array */
        "mov  (%%eax,%%ebx,4),%%eax \n\t" /* eax = member player id   */
        "push %%eax               \n\t" /* arg 2: player_id         */
        "pushl -0xc4(%%ebp)       \n\t" /* arg 1: leveled_up flag   */
        "mov  %%esi,%%ecx         \n\t" /* this: AutoClass6*        */
        "call %P0                 \n\t" /* C body (__thiscall, ret 8) */
        "cmpl $0x0,-0xc4(%%ebp)   \n\t" /* re-execute the overwritten cmp */
        "ret                      \n\t"
        :
        : "i"(avatar_level_up_hook_body));
}

void init_avatar_level_hook()
{
    /* Resolve the guard comparison instead of pinning it: scan the body of
     * AutoClass6::HandleXpAward_maybe (0x625ca0) for the unique pattern of the
     * level-up branch guard - cmp dword [ebp-0xc4], 0 ; jz - verified at
     * 0x626071 on the beta build. Anything but exactly one match means an
     * unexpected build layout, so we refuse to patch. */
    static const uint8_t guard_pattern[9] = {
        0x83, 0xbd, 0x3c, 0xff, 0xff, 0xff, 0x00, /* cmp dword [ebp-0xc4], 0 */
        0x0f, 0x84                                /* jz (left intact)       */
    };

    uint32_t start = ASI::AddrOf(0x225ca0);
    uint32_t end = ASI::AddrOf(0x226400);
    uint32_t guard_site = 0;
    int found = 0;
    for (uint32_t addr = start; addr + sizeof(guard_pattern) <= end; addr++)
    {
        if (memcmp((const void *)addr, guard_pattern, sizeof(guard_pattern)) == 0)
        {
            guard_site = addr;
            found++;
        }
    }
    if (found != 1)
    {
        log_error("AvatarLevelHook: expected 1 level-up guard site, found %d - hook disabled", found);
        return;
    }

    ASI::MemoryRegion levelup_mreg(guard_site, 7);
    ASI::BeginRewrite(levelup_mreg);
    *(uint8_t *)(guard_site) = 0xE8; /* CALL instruction */
    *(int *)(guard_site + 1) = (int)(&avatar_level_up_hook_stub) - (int)(guard_site + 5);
    *(uint8_t *)(guard_site + 5) = 0x90; /* nop padding */
    *(uint8_t *)(guard_site + 6) = 0x90;
    ASI::EndRewrite(levelup_mreg);
    log_info("AvatarLevelHook: level-up guard hooked at 0x%x", guard_site);
}

void init_lvl_up_functions()
{
    findPlayerItem = (findPlayerItem_ptr) (ASI::AddrOf(0x2465a0));
    findFreeSlot = (findFreeSlot_ptr) (ASI::AddrOf(0x246210));
    serializeAvatarUpdate = (serializeAvatarUpdate_ptr) (ASI::AddrOf(0x218b10));
    getAvatar = (getAvatar_ptr) (ASI::AddrOf(0x24fcc0));

    if (!ability_functions_ready())
    {
        log_error("LevelUpHook: game function initialization failed");
    }
}
