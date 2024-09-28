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
    uint16_t spell_id = spellAPI.getSpellID(_this->CGdSpell, spell_index_of_type);
    spellAPI.getResourceSpellData(_this->CGdResource, &spell_data, spell_id);
    uint16_t current_damage = (spell_data.params[1] * damage) / 100;
    return current_damage;
}