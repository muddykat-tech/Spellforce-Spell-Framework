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
void initialize_beta_hooks();
void initialize_data_hooks();

void console_log(const char*);

#endif 