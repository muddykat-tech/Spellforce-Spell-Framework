#ifndef HOOKS_H
#define HOOKS_H

#include "../api/sfsf.h"
#include "../api/sf_data_utilities.h"
#include "../asi/sf_asi.h"

#define DEFINE_FUNCTION(group, name, address) \
    name##_ptr name = (name##_ptr)(ASI::AddrOf(address)); \
    api##group##Functions.name = name;

#define INCLUDE_FUNCTION(group, name, pointer) \
    api##group##Functions.name = pointer;


extern SpellFunctions apiSpellFunctions;
extern ToolboxFunctions apiToolboxFunctions;
extern FigureFunctions apiFigureFunctions;
void initBetaHooks();
void initDataHooks();

void ConsoleLog(char*);

#endif 