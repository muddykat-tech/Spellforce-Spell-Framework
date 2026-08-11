/**
 * @defgroup EffectHook Effect Hook
 * @ingroup Hooks
 * @brief Used to inject Spell Effect Handlers into Spellforce.
 * @addtogroup EffectHook
 * @{
 */

#include "../sf_wrappers.h"
#include "../sf_hooks.h"
#include "../../registry/spell_data_registries/sf_spelleffect_registry.h"
#include "sf_spelleffect_hook.h"

void __thiscall sf_spelleffect_hook(SF_CGdSpell *_this)
{
    uint16_t spell_index;
    for (spell_index = 1; spell_index <= _this->max_used; ++spell_index)
    {
        if (_this->active_spell_list[spell_index].spell_id != 0)
        {
            short to_do_count =
                _this->active_spell_list[spell_index].to_do_count;
            if (to_do_count <= 0)
            {
                to_do_count = 0;
            }
            else
            {
                to_do_count = to_do_count + -1;
            }
            _this->active_spell_list[spell_index].to_do_count = to_do_count;
            if (to_do_count == 0)
            {
                uint16_t spell_job =
                    _this->active_spell_list[spell_index].spell_job;
                handler_ptr func = get_spell_effect(spell_job);
                if (func != NULL)
                {
                    func(_this, spell_index);
                }
            }
        }
    }
}

/** @} */
