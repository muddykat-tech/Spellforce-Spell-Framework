
#include "../sf_wrappers.h"
#include "../sf_hooks.h"


/** 
 * @defgroup UtilHooks Utility Hooks
 * @ingroup Hooks
 * @brief Hooks for Utility functions not directly related to each other; but not large enough to support their own hook type.
 * @addtogroup UtilHooks
 * @{
 */

/*
undefined4 __thiscall CGdSpell::IsAbilitySpellLine(CGdSpell *this,GdSpellLine param_1)  0072afb0
undefined4 __thiscall CGdSpell::IsAOESpellLine(CGdSpell *this,GdSpellLine param_1) 0072ac90
undefined4 __thiscall CGdSpell::IsAuraSpellLine(CGdSpell *this,GdSpellLine param_1) 0072ae20
undefined4 __thiscall CGdSpell::IsBlackAuraSpellLine(CGdSpell *this,GdSpellLine param_1) 0072aec0
undefined4 __thiscall CGdSpell::IsDominationSpell(CGdSpell *this,ushort param_1)  0072ac20
undefined4 __thiscall CGdSpell::IsDominationSpellLine(CGdSpell *this,GdSpellLine param_1) 0072af60
undefined4 __thiscall CGdSpell::IsSummonSpellLine(CGdSpell *this,GdSpellLine param_1) 0072b060
undefined4 __thiscall CGdSpell::IsWhiteAuraSpellLine(CGdSpell *this,GdSpellLine param_1) 0072af00
*/

//IsAbilitySpellLine
bool __thiscall is_combat_ability(SF_CGdSpell *_this, GdSpellLine spell_line)
{
    return hasSpellTag(spell_line, SpellTag::COMBAT_ABILITY_SPELL);
}


/**
 * @}
 */