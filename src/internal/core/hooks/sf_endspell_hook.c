/**
 * @defgroup EndSpellHook End Spell Hook
 * @ingroup Hooks
 * @brief Used to inject 'endspell' handlers registered by mods and sfsf into spellforce
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

void __thiscall tryClearCheckSpellsBeforeJob(SF_CGdSpell *_this, uint16_t spell_index, uint16_t figure_index)
{
    uint16_t spell_node = figureAPI.getSpellJobStartNode(_this->SF_CGdFigure, figure_index);
    while (spell_node != 0)
    {
        uint16_t spell_id = toolboxAPI.getSpellIndexFromDLL(_this->SF_CGdDoubleLinkedList, spell_node);
        if ((spell_index != spell_id) && ((_this->active_spell_list[spell_id].flags & 0x2) != 0))
        {
            return;
        }
        spell_node = toolboxAPI.getNextNode(_this->SF_CGdDoubleLinkedList, spell_node);
    }
    _this->SF_CGdFigure->figures[figure_index].flags &= ~(F_CHECK_SPELLS_BEFORE_JOB);

}
void __thiscall sf_figure_end_spells_hook(SF_CGdFigureToolbox *_this, uint16_t figure_id)
{
    for (uint16_t spell_node = figureAPI.getSpellJobStartNode(_this->CGdFigure, figure_id);
         spell_node != 0; spell_node =  figureAPI.getSpellJobStartNode(_this->CGdFigure, figure_id))
    {
        uint16_t spell_index = toolboxAPI.getSpellIndexFromDLL(_this->CGdDoubleLinkedList, spell_node);
        uint16_t spell_line = _this->CGdSpell->active_spell_list[spell_index].spell_line;
        if (!spellAPI.hasSpellTag(spell_line, SpellTag::AOE_SPELL))
        {
            sf_endspell_hook(_this->CGdSpell, spell_index);
        }
        else
        {
            if (toolboxAPI.removeSpellFromList(_this, figure_id, spell_index))
            {
                uint16_t spell_line = spellAPI.getSpellLine(_this->CGdSpell, spell_index);
                if (spell_line == kGdSpellLineFreezeArea)
                {
                    spellAPI.figTryUnfreeze(_this->CGdSpell, spell_index, figure_id);
                    spellAPI.figClrChkSplBfrChkBattle(_this->CGdSpell, spell_index, figure_id);
                }
                if (spell_line == kGdSpellLineHypnotizeArea)
                {
                    spellAPI.figTryUnfreeze(_this->CGdSpell, spell_index, figure_id);
                    tryClearCheckSpellsBeforeJob(_this->CGdSpell, spell_index, figure_id);
                    spellAPI.figClrChkSplBfrChkBattle(_this->CGdSpell, spell_index, figure_id);
                }
            }
        }
        if ((toolboxAPI.hasSpellOnIt(_this, figure_id, spell_line)) &&
            (_this->CGdSpell->active_spell_list[spell_index].target.entity_index == 0) &&
            (_this->CGdSpell->active_spell_list[spell_index].source.entity_index == 0))
        {
            log_debug(DEBUG_HIGH, "Broken spell [%d] on figure [%d] spell type [%d]",
                      spell_index, figure_id, spell_line);
            toolboxAPI.removeSpellFromList(_this, figure_id, spell_index);
        }
    }
}
