#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "sf_general_structures.h"
#include "sf_effect_functions.h"
typedef void(__thiscall *handler_ptr)(SF_CGdSpell *, uint16_t);
typedef int(__thiscall *refresh_handler_ptr)(SF_CGdSpell *, uint16_t);

typedef enum
{
	SUMMON_SPELL,
	DOMINATION_SPELL,
	CHAIN_SPELL,
	WHITE_AURA_SPELL, // In Future versions, we may be able to refactor AURA tags to be more dynamic
	BLACK_AURA_SPELL,
	SPELL_TAG_COUNT
} SpellTag;

typedef struct __attribute__((packed))
{
	uint16_t spell_id;
	uint16_t spell_effect_id;
	SpellTag spell_tag;
	handler_ptr spell_type_handler;
	handler_ptr spell_effect_handler;
	handler_ptr spell_end_handler;
	refresh_handler_ptr spell_refresh_handler;
	sub_effect_handler_ptr sub_effect_handler;
	SFMod *parent_mod;
} SFSpell;

// Declare the function pointers for the RegistrationFunctions group
DECLARE_FUNCTION(SFSpell *, registerSpell, uint16_t spell_id);
DECLARE_FUNCTION(void, linkTypeHandler, SFSpell *spell, handler_ptr typeHandler);
DECLARE_FUNCTION(void, linkEffectHandler, SFSpell *spell, uint16_t spell_effect_id, handler_ptr effectHandler);
DECLARE_FUNCTION(void, linkEndHandler, SFSpell *spell, handler_ptr endHandler);
DECLARE_FUNCTION(void, applySpellTag, SFSpell *spell, SpellTag tag);
DECLARE_FUNCTION(void, linkSubEffectHandler, SFSpell *spell, sub_effect_handler_ptr handler);
DECLARE_FUNCTION(void, linkRefreshHandler, SFSpell *spell, refresh_handler_ptr handler);
DECLARE_FUNCTION_GROUP(Registration,
					   registerSpell_ptr registerSpell;
					   linkTypeHandler_ptr linkTypeHandler;
					   linkEffectHandler_ptr linkEffectHandler;
					   linkEndHandler_ptr linkEndHandler;
					   applySpellTag_ptr applySpellTag;
					   linkSubEffectHandler_ptr linkSubEffectHandler;
					   linkRefreshHandler_ptr linkRefreshHandler;);