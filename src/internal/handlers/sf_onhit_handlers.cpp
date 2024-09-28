#include "sf_onhit_handlers.h"
#include "../core/sf_wrappers.h"
#include "../core/sf_hooks.h"

uint16_t __thiscall trueshot_onhit_handler(SF_CGdFigureJobs *_this, uint16_t source_index, uint16_t target_index, uint16_t damage)
{
    log_info("True Shot Handler Called");
    SF_SGtFigureAction action;
    figureAPI.getTargetAction(_this->CGdFigure, &action, source_index);

    if (isActionMelee(&action))
    {
        return damage;
    }

    uint16_t spell_index_of_type = toolboxAPI.getSpellIndexOfType(_this->CGdFigureToolBox, source_index, kGdSpellLineAbilityTrueShot, 0);
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->CGdResource, &spell_data, spell_index_of_type);
    uint16_t current_damage = (spell_data.params[1] * damage) / 100;

    return current_damage;
}

uint16_t __thiscall riposte_onhit_handler(SF_CGdFigureJobs *_this, uint16_t source_index, uint16_t target_index, uint16_t damage)
{
    log_info("Riposte Handler Called");
    // Check if it is source_index passed to the function or target_index

    CGdResourceSpell spell_data;
    uint16_t start_node = figureAPI.getSpellJobStartNode(_this->CGdFigure, source_index);
    for (; start_node != 0; start_node = figureAPI.getSpellJobNextNode(_this->CGdFigure, start_node))
    {
        uint16_t spell_index = toolboxAPI.getSpellIndexFromDLL(_this->CGdDoubleLinkList, start_node);
        uint16_t spell_id = spellAPI.getSpellID(_this->CGdSpell, spell_index);
        uint16_t spell_line_id = spellAPI.getSpellLine(_this->CGdSpell, spell_id);
        if (spell_linde_id == kGdSpellLineAbilityRiposte)
        {
            spellAPI.getResourceSpellData(_this->CGdResource, &spell_data, spell_id);
        }
    }

    if (spell_data != nullptr)
    {
        uint16_t return_damage = (damage * spell_data.params[0]) / 100;
        // Lower the damage by the amount that will be reflected
        toolboxAPI.dealDamage(_this->CGdFigureToolBox, source_index, target_index, (damage - return_damage), 0, 0, 0);
    }

    return damage;
}