#include "sf_effect_hook.h"
#include "../../../asi/sf_asi.h"

#include "sf_subeffect_hook.h"
#include "sf_phys_effect_hook.h"

DECLARE_FUNCTION(void, removeFigureEffect, SF_CGdEffect *_this, uint16_t effect_id);
DECLARE_FUNCTION(void, updateStepCount, SF_CGdEffect *_this, uint16_t effect_id);
DECLARE_FUNCTION(uint32_t, tryRemoveEffect, SF_CGdEffect *_this, uint16_t effect_id);
DECLARE_FUNCTION(void, addEffectFromEffect, SF_CGdEffect *_this, uint16_t effect_id);


tryRemoveEffect_ptr tryRemoveEffect;
removeFigureEffect_ptr removeFigureEffect;
updateStepCount_ptr updateStepCount;
addEffectFromEffect_ptr addEffectFromEffect;



void __thiscall effect_trigger_hook(SF_CGdEffect *_this)
{
    log_debug (DEBUG_HIGH, "Max used %d", _this->max_used);

    for (uint16_t effect_id = 1; (_this->max_used != 0) && (effect_id <= _this->max_used); effect_id++)
    {
        //0 - succes, 1 - need work
        if (tryRemoveEffect(_this, effect_id))
        {
            if (_this->effects[effect_id].type != kGdEffectNone)
            {
                updateStepCount(_this, effect_id);
                uint16_t effect_len = _this->effects[effect_id].length;
                if ((effect_len != 0) &&
                    (effect_len + _this->effects[effect_id].start_step < _this->OpaqueClass->current_step))
                {
                    if (effectAPI.effectXDataExists(_this->SF_CGdXDataList, effect_id, EFFECT_SUBSPELL_ID))
                    {
                        sf_subeffect_hook(_this, effect_id);
                    }
                    if (effectAPI.effectXDataExists(_this->SF_CGdXDataList, effect_id, EFFECT_EFFECT_TYPE))
                    {
                        addEffectFromEffect(_this, effect_id);
                    }
                    if (effectAPI.effectXDataExists(_this->SF_CGdXDataList, effect_id, EFFECT_PHYSICAL_DAMAGE))
                    {
                        sf_phys_effect_hook(_this, effect_id);
                    }
                    removeFigureEffect(_this, effect_id);
                }
            }
        }
    }
}

void init_effect_hooks()
{
    removeFigureEffect =  (removeFigureEffect_ptr) ASI::AddrOf(0x2dcae0);
    updateStepCount = (updateStepCount_ptr) ASI::AddrOf(0x2de230);
    tryRemoveEffect = (tryRemoveEffect_ptr) ASI::AddrOf(0x2de410);
    addEffectFromEffect = (addEffectFromEffect_ptr) ASI::AddrOf(0x2ddc70);

    ASI::MemoryRegion effect_trigger_mreg (ASI::AddrOf(0x278789), 5);
    ASI::BeginRewrite(effect_trigger_mreg);
    *(unsigned char *)(ASI::AddrOf(0x278789)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x27878a)) = (int)(&effect_trigger_hook) - ASI::AddrOf(0x27878e);
    ASI::EndRewrite(effect_trigger_mreg);
}
