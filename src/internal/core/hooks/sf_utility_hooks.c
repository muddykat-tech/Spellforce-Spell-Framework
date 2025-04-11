
#include "../sf_wrappers.h"
#include "../sf_hooks.h"

/**
 * @defgroup UtilHooks Utility Hooks
 * @ingroup Hooks
 * @brief Hooks for Utility functions not directly related to each other; but not large enough to support their own hook type.
 * @addtogroup UtilHooks
 * @{
 */

bool __thiscall is_combat_ability(SF_CGdSpell *_this, GdSpellLine spell_line)
{
    return hasSpellTag(spell_line, SpellTag::COMBAT_ABILITY_SPELL);
}

bool __thiscall is_domination_spell(SF_CGdSpell *_this, GdSpellLine spell_line)
{
    return hasSpellTag(spell_line, SpellTag::DOMINATION_SPELL);
}

bool __thiscall is_domination_spell(SF_CGdSpell *_this, uint16_t spell_index)
{
    uint16_t spell_line = spellAPI.getSpellLine(_this, spell_index);
    return hasSpellTag(spell_line, SpellTag::DOMINATION_SPELL);
}

bool __thiscall is_domination_spellline(SF_CGdSpell *_this,
                                        GdSpellLine spell_line)
{
    return hasSpellTag(spell_line, SpellTag::DOMINATION_SPELL);
}


bool __thiscall is_summon_spell(SF_CGdSpell *_this, GdSpellLine spell_line)
{
    return hasSpellTag(spell_line, SpellTag::SUMMON_SPELL);
}

bool __thiscall is_white_aura_spell(SF_CGdSpell *_this, GdSpellLine spell_line)
{
    return hasSpellTag(spell_line, SpellTag::WHITE_AURA_SPELL);
}

bool __thiscall is_black_aura_spell(SF_CGdSpell *_this, GdSpellLine spell_line)
{
    return hasSpellTag(spell_line, SpellTag::BLACK_AURA_SPELL);
}

bool __thiscall is_aoe_spell(SF_CGdSpell *_this, GdSpellLine spell_line)
{
    return hasSpellTag(spell_line, SpellTag::AOE_SPELL);
}

bool __thiscall is_aura_spell(SF_CGdSpell *_this, GdSpellLine spell_line)
{
    return hasSpellTag(spell_line, SpellTag::AURA_SPELL);
}


/**
 * @}
 */
