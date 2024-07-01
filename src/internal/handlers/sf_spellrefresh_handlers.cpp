#include "../core/sf_hooks.h"
#include "../core/sf_wrappers.h"

#include "sf_spellrefresh_handlers.h"

int __thiscall first_block_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{

    return 1;
}

int __thiscall second_block_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
}

int __thiscall case_17_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // A few checks here, this one can return 1 or 0
    SF_CGdResourceSpell spell_data;
    spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell_index);
    uint16_t spell_line_id = spellResource->spellline_id;
    uint16_t entity_index = _this->active_spell_list[spell_index].target.entity_index;

    bool does_figure_have_spell_on_it = toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, entity_index, spell_line_id);

    if (does_figure_have_spell_on_it == 0)
        return 1;

    return 0;
}