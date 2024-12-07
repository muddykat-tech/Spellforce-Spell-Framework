/** 
 * @defgroup EndSpellHook End Spell Hook
 * @ingroup Hooks
 * Used to inject 'endspell' handlers registered by mods and sfsf into spellforce
 */

#include "../sf_wrappers.h"
#include "../sf_hooks.h"
#include "../../registry/spell_data_registries/sf_spellend_registry.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sf_endspell_hook.h"


/**
 * @ingroup EndSpellHook
 */
void __thiscall sf_endspell_hook(SF_CGdSpell *_this, uint16_t spell_index)
{
    // We need a map of end spell handlers?
    // And a default handler that does nothing
    // handler takes (SF_CGdSpell *_this, uint16_t spell_index) as params
    uint16_t spell_line = _this->active_spell_list[spell_index].spell_line;
    handler_ptr spellend_handler = get_spell_end(spell_line);
    if (spellend_handler != NULL)
    {
        spellend_handler(_this, spell_index);
    }
}