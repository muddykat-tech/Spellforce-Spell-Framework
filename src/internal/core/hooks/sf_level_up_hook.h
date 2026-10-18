#pragma once
#include "sf_hero_hook.h"

void __thiscall learnSkillOnLevelUp_hook (AutoClass6 *_this, uint32_t player_id, GdFigureAbility *skill,
                                          uint32_t notify);
void init_lvl_up_functions();
