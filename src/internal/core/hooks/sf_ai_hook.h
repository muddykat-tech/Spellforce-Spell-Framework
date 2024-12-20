#pragma once

#include "../../../api/sfsf.h"
#include "../../../asi/sf_asi.h"
#include "../../../api/sf_ai_functions.h"
#include "../../../api/sf_general_structures.h"
#include "../sf_hooks.h"
#include "../sf_wrappers.h"
#include "../../registry/ai_data_registries/sf_ai_single_target_registry.h"


uint32_t __thiscall rank_support_spell_hook(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data);
uint32_t __thiscall rank_offensive_spell_hook(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data);
uint32_t __thiscall avoidance_penalty_hook(SF_CGdBattleDevelopment *_this, uint16_t figure_index);