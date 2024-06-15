#include "sf_sub_effect_handlers.h"

void __thiscall elemental_sub_effect_handler(SF_CGDEffect *_this, uint16_t effect_index)
{
    uint16_t spell_id = effectAPI.getEffectXData(_this, effect_id, EFFECT_SUBSPELL_ID);
    uint16_t figure_index1 = effectAPI.getEffectXData(_this, effect_id, EFFECT_ENTITY_INDEX);
    uint8_t figure_type1;
    if (figure_index1)
    {
        figure_type1 = effectAPI.getEffectXData(_this, effect_id, EFFECT_ENTITY_TYPE);
    }
    uint16_t sub_spell_flag = effectAPI.getEffectXData(_this, effect_id, EFFECT_DO_NOT_ADD_SUBSPELL);
    if (sub_spell_flag != 1)
    {
        
    }
    else
    {
        uint16_t figure_index2 = effectAPI.getEffectXData(_this, effect_id, EFFECT_ENTITY_INDEX2);
        uint8_t figure_type2 = 0;
        if (figure_index2)
        {
            figure_type2 = effectAPI.getEffectXData(_this, effect_id, EFFECT_ENTITY_TYPE2);
        }
        if (!figure_type2 || !figure_index2)
        {
            return;
        }
        SF_CGdTargetData source = {figure_type1, figure_index1, {0, 0}};
        SF_CGdTargetData target = {figure_type2, figure_index2, {0, 0}};
        spellAPI.addSpell(_this->CGdSpell, spell_id, _this->OpaqueClass->current_step, source, target, 0);
    }
}