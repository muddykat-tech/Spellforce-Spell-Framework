#ifndef HOOKS_H
#define HOOKS_H

#include "../../api/sfsf.h"
#include "../../asi/sf_asi.h"

#define DEFINE_FUNCTION(group, name, address) \
    name##_ptr name = (name##_ptr)(ASI::AddrOf(address)); \
    group##API.name = name;

#define INCLUDE_FUNCTION(group, name, pointer) \
    group##API.name = pointer;

extern SpellFunctions spellAPI;
extern ToolboxFunctions toolboxAPI;
extern FigureFunctions figureAPI;
extern IteratorFunctions iteratorAPI;
extern RegistrationFunctions registrationAPI;

extern void __thiscall setupFigureIterator(CGdFigureIterator *iterator, SF_CGdSpell *spell);
extern void __thiscall disposeFigureIterator(CGdFigureIterator iterator);
extern void __thiscall addBonusMultToStatistic(SF_CGdFigure *figure, StatisticDataKey key, uint16_t target, int8_t value);
extern void __thiscall spellClearFigureFlag(SF_CGdSpell *spell, uint16_t spell_index, SpellFlagKey key);

extern SFSpell* __thiscall registerSpell(uint16_t spell_id);
extern void __thiscall linkSpellTags(SFSpell* spell, SpellTag tags, ...);

typedef void (__thiscall *handler_ptr) (SF_CGdSpell *, uint16_t);
extern void __thiscall linkTypeHandler(SFSpell* spell, handler_ptr typeHandler); 
extern void __thiscall linkEffectHandler(SFSpell* spell, uint16_t spell_effect_id, handler_ptr effectHandler); 
extern void __thiscall linkEndHandler(SFSpell* spell, handler_ptr endHandler); 

extern SFMod* createModInfo(const char *mod_id, const char* mod_version, const char *mod_author, const char *mod_description);

extern FUN_0069eaf0_ptr FUN_0069eaf0;
extern fidfree_ptr fidFree;
extern menu_label_ptr initialize_menu_label;
extern menu_label_set_string_ptr menu_label_set_string;
extern message_box_ptr show_message_box;
extern original_menu_func_ptr original_menu_func;

void initialize_beta_hooks();
void initialize_data_hooks();

void console_log(const char*);

#endif 