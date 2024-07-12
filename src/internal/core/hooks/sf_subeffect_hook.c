#include "../../registry/sf_subeffect_registry.h"
#include "../sf_wrappers.h"
#include "../sf_hooks.h"

#include "sf_subeffect_hook.h"

void __thiscall sf_subeffect_hook(SF_CGDEffect *_this, uint16_t effect_id)
{
    uint16_t spell_id = effectAPI.getEffectXData(_this, effect_id, EFFECT_SUBSPELL_ID);
    log_info("Sub Effect hook triggered");
    if (spell_id)
    {
        SF_CGdResourceSpell spell_data;
        spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, spell_id);
        sub_effect_handler_ptr handler = get_sub_effect_handler(spell_data.spell_line_id);
        if (handler != NULL)
        {
            log_info("Sub Effect handler found");
            handler(_this, effect_id);
        }
    }
}