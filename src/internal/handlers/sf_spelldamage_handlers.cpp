#include "sf_spelleffect_handlers.h"
#include "../core/sf_wrappers.h"
#include "sf_spelldamage_handlers.h"
#include "../core/sf_hooks.h"

uint16_t __thiscall steelskin_dmg_handler(SF_CGdFigureToolbox *_toolbox, uint16_t source, uint16_t target,
                    uint16_t current_damage, uint16_t is_spell_damage, uint32_t is_ranged_damage, uint16_t spell_id)
{
    if (is_ranged_damage)
    {
        return 0;
    }
    return current_damage;
}

//same for endurance ability to a T
uint16_t __thiscall durability_dmg_handler(SF_CGdFigureToolbox *_toolbox, uint16_t source, uint16_t target,
                    uint16_t current_damage, uint16_t is_spell_damage, uint32_t is_ranged_damage, uint16_t spell_id)
{
    if ((!is_ranged_damage) &&(!is_spell_damage))
    {
        SF_CGdResourceSpell spell_data;
        spellAPI.getResourceSpellData(_toolbox->CGdResource,&spell_data,spell_id);
        current_damage = (uint16_t)((current_damage*spell_data.params[1])/100);
        return current_damage;
    }
    return current_damage;
}

uint16_t invulnerability_dmg_handler(SF_CGdFigureToolbox *_toolbox, uint16_t source, uint16_t target,
                    uint16_t current_damage, uint16_t is_spell_damage, uint32_t is_ranged_damage, uint16_t spell_id)
{
    return 0;
}

