#pragma once
#include "sf_general_structures.h"

typedef void(__thiscall *sub_effect_handler_ptr)(SF_GdEffect *, uint16_t effect_index);

DECLARE_FUNCTION(void, setEffectXData, SF_CGDEffect *_this, SpellDataKey key, uint32_t value);
DECLARE_FUNCTION(uint32_t, getEffectXData, SF_CGDEffect *_this, uint16_t effect_index, SpellDataKey key);
DECLARE_FUNCTION(uint16_t, addEffect, SF_CGDEffect *_this, CGdEffectType effect_id, SF_CGdTargetData *source,
                 SF_CGdTargetData *target, uint32_t tick_start, uint16_t tick_count, SF_Rectangle *param_6);
DECLARE_FUNCTION_GROUP(Effect,
                       setEffectXData_ptr setEffectXData;
                       addEffect_ptr addEffect;
                       getEffectXData_ptr getEffectXData;);