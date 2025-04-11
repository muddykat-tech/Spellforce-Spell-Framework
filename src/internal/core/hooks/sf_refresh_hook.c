/**
 * @defgroup RefreshHook Refresh Hook
 * @ingroup Hooks
 * @brief Used to inject Spell Refresh Handlers into Spellforce.
 * @addtogroup RefreshHook
 * @{
 */


#include "../sf_wrappers.h"
#include "../../registry/spell_data_registries/sf_spellrefresh_registry.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sf_refresh_hook.h"

int __thiscall sf_refresh_hook(SF_CGdSpell *_this, uint16_t spell_index)
{
    // We need a map of refresh handlers?
    uint16_t spell_line = _this->active_spell_list[spell_index].spell_line;
    refresh_handler_ptr spellrefresh_handler = get_spell_refresh(spell_line);
    /*  char message[256];
       sprintf(message, "Spell Refresh Hook: [%d]", spell_index);
       log_info(message);
     */
    if (spellrefresh_handler != NULL)
    {
        return spellrefresh_handler(_this, spell_index);
    }
    // Default return is 1, it can be 0
    return 1;
}

/** @} */
