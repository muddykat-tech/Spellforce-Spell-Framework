#pragma once
#include "sf_hero_hook.h"

void __thiscall learnSkillOnLevelUp_hook (AutoClass6 *_this, uint32_t player_id, GdFigureAbility *skill,
                                          uint32_t notify);
void init_lvl_up_functions();

/**
 * Avatar level-up hook. Replaces the guard comparison of the level-up branch in
 * AutoClass6::HandleXpAward (cmp [ebp-0xc4], 0 @ 0x626071): when the avatar
 * leveled up, the ability registered for the new level is granted before the
 * game runs the branch itself (the 0xFBC serialization stays game-owned).
 */
void __thiscall avatar_level_up_hook_body(AutoClass6 *self, uint32_t leveled_up, uint32_t player_id);
void init_avatar_level_hook();
