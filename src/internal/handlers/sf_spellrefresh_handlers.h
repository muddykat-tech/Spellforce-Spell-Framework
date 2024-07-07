#pragma once

#include "../../api/sfsf.h"
#include "../../api/sf_general_structures.h"
#include "../../api/sf_spell_functions.h"

int __thiscall first_block_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index);

int __thiscall warcry_berserk_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall patronize_shelter_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall endurence_durability_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index);

int __thiscall domination_spell_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall case_da_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall pestilence_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index);