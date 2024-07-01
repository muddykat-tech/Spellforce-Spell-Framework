#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "sf_general_structures.h"
#include "sf_effect_functions.h"
typedef void(__thiscall *handler_ptr)(SF_CGdSpell *, uint16_t);
typedef int(__thiscall *refresh_handler_ptr)(SF_CGdSpell *, uint16_t);

typedef enum
{
	IsSummonSpellLine,
	SPELL_TAG_COUNT
} SpellTag;

typedef struct __attribute__((packed))
{
	uint16_t spell_id;
	uint16_t spell_effect_id;
	bool spell_tags[SPELL_TAG_COUNT];
	handler_ptr spell_type_handler;
	handler_ptr spell_effect_handler;
	handler_ptr spell_end_handler;
	sub_effect_handler_ptr sub_effect_handler;
	SFMod *parent_mod;
} SFSpell;

// Declare the function pointers for the RegistrationFunctions group
DECLARE_FUNCTION(SFSpell *, registerSpell, uint16_t spell_id);
DECLARE_FUNCTION(void, linkTypeHandler, SFSpell *spell, handler_ptr typeHandler);
DECLARE_FUNCTION(void, linkEffectHandler, SFSpell *spell, uint16_t spell_effect_id, handler_ptr effectHandler);
DECLARE_FUNCTION(void, linkEndHandler, SFSpell *spell, handler_ptr endHandler);
DECLARE_FUNCTION(void, linkSpellTags, SFSpell *spell, SpellTag tags, ...);
DECLARE_FUNCTION(void, linkSubEffectHandler, SFSpell *spell, sub_effect_handler_ptr handler);
DECLARE_FUNCTION_GROUP(Registration,
					   registerSpell_ptr registerSpell;
					   linkTypeHandler_ptr linkTypeHandler;
					   linkEffectHandler_ptr linkEffectHandler;
					   linkEndHandler_ptr linkEndHandler;
					   linkSpellTags_ptr linkSpellTags;
					   linkSubEffectHandler_ptr linkSubEffectHandler;);