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

uint16_t __thiscall riposte_onhit_handler(SF_CGdFigureJobs *_this, uint16_t source_index, uint16_t target_index, uint16_t damage)
{
    log_info("Riposte Handler Called");
    // Check if it is source_index passed to the function or target_index
    SF_SGtFigureAction action;
    figureAPI.getTargetAction(_this->CGdFigure, &action, source_index);
    bool isMeleeAttack = isActionMelee(&action);

    if (!isMeleeAttack)
    {
        return damage;
    }

    SF_CGdResourceSpell spell_data;
    uint16_t spell_index = toolboxAPI.getSpellIndexOfType(_this->CGdFigureToolBox, target_index, kGdSpellLineAbilityRiposte, 0);
    uint16_t spell_id = spellAPI.getSpellID(_this->CGdSpell, spell_index);
    spellAPI.getResourceSpellData(_this->CGdResource, &spell_data, spell_id);

    uint16_t return_damage = (damage * spell_data.params[0]) / 100;

    // Lower the damage by the amount that will be reflected
    uint16_t final_damage = (damage - return_damage);
    if (final_damage != 0)
    {
        toolboxAPI.dealDamage(_this->CGdFigureToolBox, target_index, source_index, return_damage, 0, 0, 0);
    }

    return final_damage;
}