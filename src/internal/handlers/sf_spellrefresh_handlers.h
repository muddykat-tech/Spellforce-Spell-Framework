#pragma once

#include "../../api/sfsf.h"
#include "../../api/sf_general_structures.h"
#include "../../api/sf_spell_functions.h"

int __thiscall first_block_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall second_block_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall case_da_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall case_17_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index);