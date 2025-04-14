#pragma once

#include "../../../api/sfsf.h"
#include "../../../asi/sf_asi.h"
#include "../../../api/sf_ai_functions.h"
#include "../../../api/sf_general_structures.h"
#include "../sf_hooks.h"
#include "../sf_wrappers.h"


void __thiscall is_combat_ability(SF_CGdSpell *_this, GdSpellLine spell_line);

void __thiscall is_domination_spellline(SF_CGdSpell *_this,
                                        GdSpellLine spell_line);

void __thiscall is_domination_spell(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall is_summon_spell(SF_CGdSpell *_this, GdSpellLine spell_line);
void __thiscall is_white_aura_spell(SF_CGdSpell *_this, GdSpellLine spell_line);
void __thiscall is_black_aura_spell(SF_CGdSpell *_this, GdSpellLine spell_line);
void __thiscall is_aoe_spell(SF_CGdSpell *_this, GdSpellLine spell_line);
void __thiscall is_aura_spell(SF_CGdSpell *_this, GdSpellLine spell_line);
