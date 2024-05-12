// SpellforceSpellFramework.h
// DEPRECATED
#pragma once

#include "sf_data_utilities.h"
#include <stdint.h>

typedef void (*InitModuleFunc)(void*);
typedef void (*logWarningFunc)(const char*);
typedef void (*logInfoFunc)(const char*);
typedef void (__thiscall *handler_ptr) (SF_CGdSpell *, uint16_t);
typedef void (*SpellHandlerFunc)(uint16_t spell_index, handler_ptr handler);
typedef void (*EffectHandlerFunc)(uint16_t spell_job, handler_ptr handler);

// Declare the structure
struct SpellforceSpellFramework {
    InitModuleFunc initializeModule;
    SpellHandlerFunc registerSpellTypeHandler;
    EffectHandlerFunc registerEffectHandler;

    SpellFunctions *apiSpellFunctions;
    ToolboxFunctions *apiToolboxFunctions;
    FigureFunctions *apiFigureFunctions;

    // Perhaps create a struct for a proper logger? 
    logWarningFunc logWarning;
    logInfoFunc logInfo;
};

typedef struct SpellforceSpellFramework FrameworkAPI;
typedef struct SpellforceSpellFramework *pFrameworkAPI;