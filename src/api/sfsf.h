// SpellforceSpellFramework.h
// DEPRECATED
#pragma once

#include "sf_spells.h"
#include "../asi/sf_asi.h"
#include <stdint.h>

typedef void (*InitModuleFunc)(void*);
typedef void (__thiscall *handler_ptr) (SF_CGdSpell *, uint16_t);
typedef void (*SpellHandlerFunc)(uint16_t spell_index, handler_ptr handler);

// Declare the structure
struct SpellforceSpellFramework {
    InitModuleFunc initializeModule;
    SpellHandlerFunc addSpellHandler;
    cgdspellfunctions *pCGdSpellFunctions;
};

typedef struct SpellforceSpellFramework FrameworkAPI;
typedef struct SpellforceSpellFramework *pFrameworkAPI;