// SpellforceSpellFramework.h
// DEPRECATED
#pragma once

#include "sf_general_structures.h"
#include "sf_figure_functions.h"
#include "sf_iterator_functions.h"
#include "sf_spell_functions.h"
#include "sf_toolbox_functions.h"
#include "sf_registration_functions.h"

#include <stdint.h>

typedef void (*initializeModule_ptr)(void*);
typedef SFMod *(*registerMod_ptr)(void*);
typedef SFMod *(*createModInfo_ptr)(const char *mod_id, const char* mod_version, const char *mod_author, const char *mod_description);

typedef void (*logInfoFunc)(const char*);
typedef void (__thiscall *handler_ptr) (SF_CGdSpell *, uint16_t);
typedef void (*register_spelltype_handler_ptr)(uint16_t spell_index, handler_ptr handler);
typedef void (*register_effect_handler_ptr)(uint16_t spell_job, handler_ptr handler);
typedef void (*register_spell_end_handler_ptr)(uint16_t spell_index, handler_ptr handler);

// Declare the structure
struct SpellforceSpellFramework {
    register_spelltype_handler_ptr registerSpellTypeHandler;
    register_effect_handler_ptr registerEffectHandler;
    register_spell_end_handler_ptr registerSpellEndHandler;
    createModInfo_ptr createModInfo;

    SpellFunctions *spellAPI;
    ToolboxFunctions *toolboxAPI;
    FigureFunctions *figureAPI; 
    IteratorFunctions *iteratorAPI;
    RegistrationFunctions *registrationAPI;
    
    SFLog *logAPI;
};

typedef struct SpellforceSpellFramework FrameworkAPI;
typedef struct SpellforceSpellFramework *pFrameworkAPI;