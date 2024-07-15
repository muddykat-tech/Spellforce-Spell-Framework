#pragma once

#include "../../api/sfsf.h"
#include "../../api/sf_general_structures.h"
#include "../../api/sf_spell_functions.h"

int __thiscall first_block_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index);

int __thiscall slowness_refresh_handler (SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall decay_refresh_handler (SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall inflexibility_refresh_handler (SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall weaken_refresh_handler (SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall quickness_refresh_handler (SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall flexibility_refresh_handler (SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall strength_refresh_handler (SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall brilliance_refresh_handler (SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall brilliance_refresh_handler (SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall suffocation_refresh_handler (SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall inablility_refresh_handler (SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall slow_fighting_refresh_handler (SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall dexterity_refresh_handler (SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall endurance_refresh_handler (SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall fast_fighting_refresh_handler (SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall charisma_refresh_handler (SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall enlightenment_refresh_handler (SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall melt_resistance_refresh_handler (SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall chill_resistance_refresh_handler (SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall white_almightness_refresh_handler (SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall black_almightness_refresh_handler (SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall mutation_refresh_handler (SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall eternity_refresh_handler (SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall mutation_refresh_handler (SF_CGdSpell *_this, uint16_t spell_index);

int __thiscall warcry_berserk_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall patronize_shelter_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall endurance_durability_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index);

int __thiscall domination_spell_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall case_da_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index);
int __thiscall pestilence_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index);