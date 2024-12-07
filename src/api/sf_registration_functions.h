#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "sf_general_structures.h"
#include "sf_effect_functions.h"
#include "sf_ai_functions.h"
typedef uint16_t(__thiscall *damage_handler_ptr)(SF_CGdFigureToolbox *_toolbox, uint16_t source, uint16_t target,
												 uint16_t current_damage, uint16_t is_spell_damage, uint32_t is_ranged_damage, uint16_t spell_id);

typedef void(__thiscall *handler_ptr)(SF_CGdSpell *, uint16_t);
typedef uint32_t(__thiscall *ai_single_hander_ptr)(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data);
typedef uint16_t(__thiscall *onhit_handler_ptr)(SF_CGdFigureJobs *, uint16_t source, uint16_t target, uint16_t damage);
typedef int(__thiscall *refresh_handler_ptr)(SF_CGdSpell *, uint16_t);

typedef struct __attribute__((packed))
{
	uint16_t spell_id;
	uint16_t spell_effect_id;
	uint16_t spell_tags;
	handler_ptr spell_type_handler;
	handler_ptr spell_effect_handler;
	handler_ptr spell_end_handler;
	onhit_handler_ptr spell_onhit_handler;
	refresh_handler_ptr spell_refresh_handler;
	sub_effect_handler_ptr sub_effect_handler;
	damage_handler_ptr deal_damage_handler;
	SpellDamagePhase damage_phase;
	OnHitPhase hit_phase;
	SFMod *parent_mod;
} SFSpell;

// Declare the function pointers for the RegistrationFunctions group
DECLARE_FUNCTION(SFSpell *, registerSpell, uint16_t spell_id);
DECLARE_FUNCTION(void, linkTypeHandler, SFSpell *spell, handler_ptr typeHandler);
DECLARE_FUNCTION(void, linkEffectHandler, SFSpell *spell, uint16_t spell_effect_id, handler_ptr effectHandler);
DECLARE_FUNCTION(void, linkEndHandler, SFSpell *spell, handler_ptr endHandler);
DECLARE_FUNCTION(void, linkOnHitHandler, SFSpell *spell, onhit_handler_ptr onhitHandler, OnHitPhase phase);
DECLARE_FUNCTION(void, applySpellTag, SFSpell *spell, SpellTag tag);
DECLARE_FUNCTION(void, linkSubEffectHandler, SFSpell *spell, sub_effect_handler_ptr handler);
DECLARE_FUNCTION(void, linkRefreshHandler, SFSpell *spell, refresh_handler_ptr handler);
DECLARE_FUNCTION(void, linkDealDamageHandler, SFSpell *spell, damage_handler_ptr handler, SpellDamagePhase phase);

DECLARE_FUNCTION_GROUP(Registration,
					   registerSpell_ptr registerSpell;
					   linkTypeHandler_ptr linkTypeHandler;
					   linkEffectHandler_ptr linkEffectHandler;
					   linkEndHandler_ptr linkEndHandler;
					   applySpellTag_ptr applySpellTag;
					   linkSubEffectHandler_ptr linkSubEffectHandler;
					   linkRefreshHandler_ptr linkRefreshHandler;
					   linkDealDamageHandler_ptr linkDealDamageHandler;
					   linkOnHitHandler_ptr linkOnHitHandler;);