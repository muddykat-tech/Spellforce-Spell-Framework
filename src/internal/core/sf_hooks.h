#ifndef HOOKS_H
#define HOOKS_H

#include "../../api/sfsf.h"
#include "../../asi/sf_asi.h"
#include "../../api/sf_registration_functions.h"

#define DEFINE_FUNCTION(group, name, address)             \
    name##_ptr name = (name##_ptr)(ASI::AddrOf(address)); \
    group##API.name = name;

#define INCLUDE_FUNCTION(group, name, pointer) \
    group##API.name = pointer;

extern SpellFunctions spellAPI;
extern ToolboxFunctions toolboxAPI;
extern FigureFunctions figureAPI;
extern IteratorFunctions iteratorAPI;
extern RegistrationFunctions registrationAPI;
extern EffectFunctions effectAPI;
extern AiFunctions aiAPI;

extern SFSpell *__thiscall registerSpell(uint16_t spell_id);
extern void __thiscall applySpellTag(SFSpell *spell, SpellTag tag);

extern void __thiscall linkTypeHandler(SFSpell *spell, handler_ptr typeHandler);
extern void __thiscall linkEffectHandler(SFSpell *spell, uint16_t spell_effect_id, handler_ptr effectHandler);
extern void __thiscall linkEndHandler(SFSpell *spell, handler_ptr endHandler);
extern void __thiscall linkSubEffectHandler(SFSpell *spell, sub_effect_handler_ptr handler);
extern void __thiscall linkRefreshHandler(SFSpell *spell, refresh_handler_ptr handler);
extern void __thiscall linkDealDamageHandler(SFSpell *spell, damage_handler_ptr handler, SpellDamagePhase phase);
extern void __thiscall linkOnHitHandler(SFSpell *spell, onhit_handler_ptr handler, OnHitPhase phase);

extern void __thiscall linkSingleTargetAIHandler(SFSpell *spell, ai_single_handler_ptr handler);
extern void __thiscall linkAOEAIHandler(SFSpell *spell, ai_aoe_handler_ptr handler);
extern void __thiscall linkAvoidanceAIHandler(SFSpell *spell, ai_avoidance_handler_ptr handler);

extern SFMod *createModInfo(const char *mod_id, const char *mod_version, const char *mod_author, const char *mod_description);

void initialize_beta_hooks();
void initialize_data_hooks();

#endif