#pragma once
#include "sf_general_structures.h"



DECLARE_FUNCTION(void, setEffectXData, void *CGDEffect, SpellDataKey key, uint32_t value);
DECLARE_FUNCTION(uint16_t, addEffect, void * _this,CGdEffectType effect_id, SF_CGdTargetData *source, 
                SF_CGdTargetData *target, uint32_t tick_start, uint16_t tick_count, SF_Rectangle *param_6);
DECLARE_FUNCTION_GROUP(Effect,
    setEffectXData_ptr setEffectXData;
    addEffect_ptr addEffect;
);