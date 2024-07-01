#include "../core/sf_hooks.h"
#include "../core/sf_wrappers.h"

#include "sf_spellrefresh_handlers.h"

int __thiscall first_block_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    log_info("first block refresh handler called");
    uint16_t spell_line = _this->active_spell_list[spell_index].spell_line;
    return 1;
}

int __thiscall second_block_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    log_info("second block refresh handler called");
    uint16_t spell_line = _this->active_spell_list[spell_index].spell_line;

    return 1;
}

int __thiscall case_da_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    log_info("case 0xda block refresh handler called");
    return 1;
}

int __thiscall case_17_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    log_info("case 0x17 block refresh handler called");
    int end_result = 1;

    uint16_t remove_id = spell_index;
    // A few checks here, this one can return 1 or 0
    uint16_t spell_line = _this->active_spell_list[spell_index].spell_line;
    uint16_t spell_id = _this->active_spell_list[spell_index].spell_id;

    SF_CGdResourceSpell spell_data;
    SF_CGdResourceSpell *ref = spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, spell_id);

    uint16_t spell_line_id = ref->spell_line_id;
    uint16_t entity_index = _this->active_spell_list[spell_index].target.entity_index;

    bool does_figure_have_spell_on_it = toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, entity_index, spell_line_id);

    if (does_figure_have_spell_on_it == 0)
    {
        end_result = 1;
        goto end;
    }

    spellAPI.removeDLLNode(_this, remove_id);
    spellAPI.onSpellRemove(_this, remove_id);
    spellAPI.setEffectDone(_this, remove_id, 0);

end:
    return end_result;
}