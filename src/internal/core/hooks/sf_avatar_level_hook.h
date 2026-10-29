#pragma once
#include "sf_level_up_hook.h"

/**
 * Message serialized by the game before broadcasting a level-up (packet 0xFBC).
 * Layout verified against the beta build: [ebp-0xe0] in AutoClass6::HandleXpAward.
 */
typedef struct __attribute__((packed))
{
    uint32_t player_id;   /* party member avatar that leveled up */
    uint8_t leveled_up;   /* flag copied from AddAvatarXp */
} AvatarLevelUpMsg;

void avatar_level_up_hook_body(AutoClass6 *self, AvatarLevelUpMsg *msg);
void init_avatar_level_hook();
