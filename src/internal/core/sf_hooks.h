#ifndef HOOKS_H
#define HOOKS_H

#include "../../api/sfsf.h"
#include "../../asi/sf_asi.h"

#define DEFINE_FUNCTION(group, name, address) \
    name##_ptr name = (name##_ptr)(ASI::AddrOf(address)); \
    api_##group##_functions.name = name;

#define INCLUDE_FUNCTION(group, name, pointer) \
    api_##group##_functions.name = pointer;

extern SpellFunctions api_spell_functions;
extern ToolboxFunctions api_toolbox_functions;
extern FigureFunctions api_figure_functions;
extern IteratorFunctions api_iterator_functions;
void initialize_beta_hooks();
void initialize_data_hooks();

void console_log(const char*);

#endif 