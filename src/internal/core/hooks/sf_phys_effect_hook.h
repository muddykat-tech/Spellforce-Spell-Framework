#pragma once

#include "../../../api/sfsf.h"
#include "../../../asi/sf_asi.h"
#include "../sf_wrappers.h"
#include "../sf_hooks.h"
#include "../sf_modloader.h"


void __thiscall sf_phys_effect_hook(SF_CGdEffect *_this, uint16_t effect_id);

// Index of the effect currently resolved by sf_phys_effect_hook. Valid only
// inside a phys effect handler call; the effect Trigger loop is single-threaded
// and effects resolve on later ticks, so reentrancy is not required.
uint16_t __thiscall getCurrentEffectIndex();

uint16_t __thiscall cloneEffect(SF_CGdEffect *_this, uint16_t src_effect_id, SF_CGdTargetData *source,
                                SF_CGdTargetData *target, uint32_t tick_start, uint16_t tick_count,
                                SF_Rectangle *rect);
