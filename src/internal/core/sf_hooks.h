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

extern void __thiscall setupFigureIterator(CGdFigureIterator *iterator, SF_CGdSpell *spell);
extern void __thiscall disposeFigureIterator(CGdFigureIterator iterator);
extern void __thiscall addBonusMultToStatistic(SF_CGdFigure *figure, StatisticDataKey key, uint16_t target, int8_t value);
extern FUN_0069eaf0_ptr FUN_0069eaf0;
extern fidfree_ptr fidFree;
extern menu_label_ptr initialize_menu_label;
extern menu_label_set_string_ptr menu_label_set_string;

void initialize_beta_hooks();
void initialize_data_hooks();

void console_log(const char*);

#endif 