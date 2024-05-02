// SpellforceSpellFramework.h
#pragma once

#include "api/SpellforceSpells.h"
#include "asi/sf_asi.h"
// Define the function pointer type without initialization
typedef void (*InitModuleFunc)(void*);
typedef void (__thiscall *handler_ptr) (SF_CGdSpell *, uint16_t);
typedef void (*SpellHandlerFunc)(uint16_t spell_index, handler_ptr handler);

// Declare the structure
typedef struct SpellforceSpellFramework {
    InitModuleFunc initializeModule;
    SpellHandlerFunc addSpellHandler;
    cgdspellfunctions * pCGdSpellFunctions;
} SpellforceSpellFramework;