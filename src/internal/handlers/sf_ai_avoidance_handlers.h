#pragma once
#include "../../api/sf_ai_functions.h"

int __thiscall sf_ai_avoidance_hypnotize_handler(SF_CGdAiBattleDevelopement *_this, uint16_t figure_index, uint16_t spell_index);
int __thiscall sf_ai_avoidance_freeze_handler(SF_CGdAiBattleDevelopement *_this, uint16_t figure_index, uint16_t spell_index);
int __thiscall sf_ai_avoidance_invulnerability_handler(SF_CGdAiBattleDevelopement *_this, uint16_t figure_index, uint16_t spell_index);
int __thiscall sf_ai_avoidance_shield_handler(SF_CGdAiBattleDevelopement *_this, uint16_t figure_index, uint16_t spell_index);
int __thiscall sf_ai_avoidance_default_handler(SF_CGdAiBattleDevelopement *_this, uint16_t figure_index, uint16_t spell_index);