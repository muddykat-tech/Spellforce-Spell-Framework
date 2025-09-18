#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "sf_general_structures.h"
#include "sf_effect_functions.h"
#include "sf_ai_functions.h"
#include "./structures/sf_registration_structures.h"

// Declare the function pointers for the RegistrationFunctions group
DECLARE_FUNCTION(SFSpell *, registerSpell, uint16_t spell_id);
DECLARE_FUNCTION(void, linkTypeHandler, SFSpell *spell,
                 handler_ptr typeHandler);
DECLARE_FUNCTION(void, linkEffectHandler, SFSpell *spell,
                 uint16_t spell_effect_id, handler_ptr effectHandler);
DECLARE_FUNCTION(void, linkEndHandler, SFSpell *spell, handler_ptr endHandler);
DECLARE_FUNCTION(void, linkOnHitHandler, SFSpell *spell,
                 onhit_handler_ptr onhitHandler, OnHitPhase phase);
DECLARE_FUNCTION(void, applySpellTag, SFSpell *spell, SpellTag tag);
DECLARE_FUNCTION(void, linkSubEffectHandler, SFSpell *spell,
                 sub_effect_handler_ptr handler);
DECLARE_FUNCTION(void, linkRefreshHandler, SFSpell *spell,
                 refresh_handler_ptr handler);
DECLARE_FUNCTION(void, linkDealDamageHandler, SFSpell *spell,
                 damage_handler_ptr handler, SpellDamagePhase phase);

DECLARE_FUNCTION(void, linkSingleTargetAIHandler, SFSpell *spell,
                 ai_single_handler_ptr handler);
DECLARE_FUNCTION(void, linkAOEAIHandler, SFSpell *spell,
                 ai_aoe_handler_ptr handler);
DECLARE_FUNCTION(void, linkAvoidanceAIHandler, SFSpell *spell,
                 ai_avoidance_handler_ptr handler);


DECLARE_FUNCTION(SFBuilding *, registerBuilding, uint8_t building_type);
DECLARE_FUNCTION(void, linkBuildingDoneHandler, SFBuilding *building, building_done_handler_ptr handler);
DECLARE_FUNCTION(void, linkBuildingJSON, SFBuilding *building, const char *building_json_name);

/**
 * @ingroup API
 * @brief A structure dedicated to the registration of spells
 * This structure holds functions that are used to create a representation of a spell within the framework.
 * We then translate this representation into a viable Spellforce Spell
 * @see sf_mod_registry.cpp register_mod_spells()
 */
typedef struct
{
    registerSpell_ptr registerSpell;
    linkTypeHandler_ptr linkTypeHandler;
    linkEffectHandler_ptr linkEffectHandler;
    linkEndHandler_ptr linkEndHandler;
    applySpellTag_ptr applySpellTag;
    linkSubEffectHandler_ptr linkSubEffectHandler;
    linkRefreshHandler_ptr linkRefreshHandler;
    linkDealDamageHandler_ptr linkDealDamageHandler;
    linkOnHitHandler_ptr linkOnHitHandler;
    linkAvoidanceAIHandler_ptr linkAvoidanceAIHandler;
    linkSingleTargetAIHandler_ptr linkSingleTargetAIHandler;
    linkAOEAIHandler_ptr linkAOEAIHandler;

    registerBuilding_ptr registerBuilding;
    linkBuildingDoneHandler_ptr linkBuildingDoneHandler;
    linkBuildingJSON_ptr linkBuildingJSON;
} RegistrationFunctions;
