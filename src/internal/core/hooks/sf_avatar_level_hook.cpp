#include "sf_avatar_level_hook.h"
#include "../../../asi/sf_asi.h"
#include "../sf_wrappers.h"
#include "../../registry/sf_level_up_registry.h"
#include "string.h"
#include "stdint.h"

extern initInvContainerUpdateMsg_ptr initInvContainerUpdateMsg; //0x216c30
extern serializeInvContainerUpdateMsg_ptr serializeInvContainerUpdateMsg; //0x219630
extern disposeInvContainerUpdateMsg_ptr disposeInvContainerUpdateMsg; //0x222950
extern sendToPlayer_ptr sendToPlayer; //0x2613d0
extern setSlot_ptr setSlot; //0x248dd0
extern invVectorPushBack_ptr invVectorPushBack; //0x233080
extern dispose_UpdatesVector_ptr dispose_UpdatesVector; //0x221170

typedef uint32_t (__thiscall *findPlayerItem_ptr)(void *_this, uint32_t player_id, uint16_t item_id,
                                                  GtInvSlot *out_container, uint16_t *out_slot,
                                                  InvSlotEntry *out_entry);
typedef uint16_t (__thiscall *findFreeSlot_ptr)(void *_this, uint32_t player_id, GtInvSlot container,
                                                uint32_t unused);
typedef uint32_t (__thiscall *serializeAvatarUpdate_ptr)(void *_this, void *param_1, uint32_t param_2);
typedef uint32_t (__thiscall *getAvatar_ptr)(void *_this, uint32_t player_id, GdAvatar *out);

extern findPlayerItem_ptr findPlayerItem;       //0x2465a0 (sf_level_up_hook.cpp)
extern findFreeSlot_ptr findFreeSlot;           //0x246210 (sf_level_up_hook.cpp)
extern serializeAvatarUpdate_ptr serializeAvatarUpdate; //0x218b10 (sf_level_up_hook.cpp)
extern getAvatar_ptr getAvatar;                 //0x24fcc0 (sf_level_up_hook.cpp)

/* AutoClass6::HandleXpAward_maybe address range and the serializer it calls in the
 * avatar level-up branch (Serialize InvContainer-style message for packet 0xFBC). */
#define AVATAR_LEVEL_HOOK_HANDLE_XP_AWARD 0x225ca0
#define AVATAR_LEVEL_HOOK_HANDLE_XP_AWARD_END 0x226400
#define AVATAR_LEVEL_HOOK_SERIALIZE_LEVELUP 0x2a7f00

static uint32_t g_original_serialize_levelup = 0;
static AvatarLevelUpMsg *g_level_up_msg = NULL; /* ECX save slot, game logic is single threaded */

/**
 * Grants the ability registered for the avatar's new level into the player's
 * spell book and notifies the client. Mirrors learnSkillOnLevelUp_hook and
 * shares its registry: records with skill == 0 && spec == 0 are looked up by
 * the avatar level. Runs inside the level-up branch of
 * AutoClass6::HandleXpAward, before the original 0xFBC serialization, so each
 * send_buffer use stays sequential.
 */
void avatar_level_up_hook_body(AutoClass6 *self, AvatarLevelUpMsg *msg)
{
    if (self == NULL || msg == NULL || self->db == NULL)
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
    if (!getAvatar(self->db, msg->player_id, &avatar))
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

    if (!findPlayerItem(self->db, msg->player_id, ability_id, &found_container, &slot, &found_entry))
    {
        slot = findFreeSlot(self->db, msg->player_id, kGtInvSlotSpellBook, 0);
        if (slot != (uint16_t)-1)
        {
            InvSlotUpdate slot_update;

            slot_update.entry.count = 1;
            slot_update.entry.flags = 0;
            slot_update.entry.item_id = ability_id;
            setSlot(self->db, msg->player_id, kGtInvSlotSpellBook, slot, &slot_update.entry);

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
            sendToPlayer(self->net, msg->player_id, 8009, self->send_buffer, total_size);

            total_size = serializeAvatarUpdate(&msg->player_id, self->send_buffer, self->send_buffer_size);
            sendToPlayer(self->net, msg->player_id, 8102, self->send_buffer, total_size);
        }
        else
        {
            log_warning("AvatarLevelHook: spell book full, player %u", msg->player_id);
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
 * stack afterwards. Entry state (verified on the beta build): ECX points to
 * AvatarLevelUpMsg, ESI holds AutoClass6*, stack holds [ret][buffer][size].
 * The original serializer is a __thiscall function that pops its two stack
 * arguments itself, so the stub tail-calls it with the caller's stack intact
 * and returns its result (EAX is consumed by the following broadcast call).
 */
static void __declspec(naked) avatar_level_up_hook_stub()
{
    asm("mov %%ecx, %0        \n\t" /* save msg pointer across the body call */
        "push %%esi           \n\t" /* arg 1: AutoClass6* */
        "push %%ecx           \n\t" /* arg 2: AvatarLevelUpMsg* */
        "call %P1             \n\t" /* C body (cdecl) */
        "add $8, %%esp        \n\t"
        "mov %0, %%ecx        \n\t" /* restore this for the original call */
        "pop %%edx            \n\t" /* original return address */
        "call *%2             \n\t" /* original serializer, pops buffer+size */
        "push %%edx           \n\t"
        "ret                  \n\t"
        :
        : "m"(g_level_up_msg), "i"(avatar_level_up_hook_body), "m"(g_original_serialize_levelup));
}

void init_avatar_level_hook()
{
    g_original_serialize_levelup = ASI::AddrOf(AVATAR_LEVEL_HOOK_SERIALIZE_LEVELUP);

    /* Resolve the level-up call site instead of pinning it: scan the body of
     * AutoClass6::HandleXpAward_maybe for the unique E8 call to the level-up
     * serializer. Anything but exactly one match means an unexpected build
     * layout, so we refuse to patch. */
    uint32_t start = ASI::AddrOf(AVATAR_LEVEL_HOOK_HANDLE_XP_AWARD);
    uint32_t end = ASI::AddrOf(AVATAR_LEVEL_HOOK_HANDLE_XP_AWARD_END);
    uint32_t call_site = 0;
    int found = 0;
    for (uint32_t addr = start; addr < end; addr++)
    {
        if (*(uint8_t *)addr == 0xE8 && addr + 5 + *(int *)(addr + 1) == g_original_serialize_levelup)
        {
            call_site = addr;
            found++;
        }
    }
    if (found != 1)
    {
        log_error("AvatarLevelHook: expected 1 level-up call site, found %d - hook disabled", found);
        return;
    }

    ASI::MemoryRegion levelup_mreg(call_site, 5);
    ASI::BeginRewrite(levelup_mreg);
    *(uint8_t *)(call_site) = 0xE8; /* CALL instruction */
    *(int *)(call_site + 1) = (int)(&avatar_level_up_hook_stub) - (int)(call_site + 5);
    ASI::EndRewrite(levelup_mreg);
    log_info("AvatarLevelHook: level-up call site hooked at 0x%x", call_site);
}
