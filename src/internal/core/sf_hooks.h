#ifndef HOOKS_H
#define HOOKS_H

#include "../../api/sfsf.h"
#include "../../asi/sf_asi.h"

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

extern SFSpell *__thiscall registerSpell(uint16_t spell_id);
extern void __thiscall applySpellTag(SFSpell *spell, SpellTag tag);

typedef void(__thiscall *handler_ptr)(SF_CGdSpell *, uint16_t);
extern void __thiscall linkTypeHandler(SFSpell *spell, handler_ptr typeHandler);
extern void __thiscall linkEffectHandler(SFSpell *spell, uint16_t spell_effect_id, handler_ptr effectHandler);
extern void __thiscall linkEndHandler(SFSpell *spell, handler_ptr endHandler);
extern void __thiscall linkSubEffectHandler(SFSpell *spell, sub_effect_handler_ptr handler);
extern void __thiscall linkRefreshHandler(SFSpell *spell, refresh_handler_ptr handler);

extern SFMod *createModInfo(const char *mod_id, const char *mod_version, const char *mod_author, const char *mod_description);

void initialize_beta_hooks();
void initialize_data_hooks();

#endif