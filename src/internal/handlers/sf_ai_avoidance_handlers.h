#pragma once

#include "../../api/sfsf.h"
#include "../../asi/sf_asi.h"

int __thiscall sf_ai_avoidance_hypnotize_handler(CGdAIBattleData *_this, uint16_t figure_index, uint16_t spell_index);
int __thiscall sf_ai_avoidance_freeze_handler(CGdAIBattleData *_this, uint16_t figure_index, uint16_t spell_index);
int __thiscall sf_ai_avoidance_invulnerability_handler(CGdAIBattleData *_this, uint16_t figure_index, uint16_t spell_index);
int __thiscall sf_ai_avoidance_shield_handler(CGdAIBattleData *_this, uint16_t figure_index, uint16_t spell_index);
int __thiscall sf_ai_avoidance_default_handler(CGdAIBattleData *_this, uint16_t figure_index, uint16_t spell_index);