#pragma once

#include "../../api/sfsf.h"
#include "../../asi/sf_asi.h"
#include "../core/sf_hooks.h"

uint32_t __thiscall sf_ai_avoidance_hypnotize_handler(CGdAIBattleData *_this,
                                                      uint16_t figure_index,
                                                      uint16_t spell_index);
uint32_t __thiscall sf_ai_avoidance_freeze_handler(CGdAIBattleData *_this,
                                                   uint16_t figure_index,
                                                   uint16_t spell_index);
uint32_t __thiscall sf_ai_avoidance_invulnerability_handler(
    CGdAIBattleData *_this, uint16_t figure_index, uint16_t spell_index);
uint32_t __thiscall sf_ai_avoidance_shield_handler(CGdAIBattleData *_this,
                                                   uint16_t figure_index,
                                                   uint16_t spell_index);
uint32_t __thiscall sf_ai_avoidance_default_handler(CGdAIBattleData *_this,
                                                    uint16_t figure_index,
                                                    uint16_t spell_index);
