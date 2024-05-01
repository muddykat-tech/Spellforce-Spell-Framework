// SpellforceSpellFramework.h
#ifndef SPELLFORCE_SPELL_FRAMEWORK_H
#define SPELLFORCE_SPELL_FRAMEWORK_H

#include "api/sf_spell.h"
#include "asi/sf_asi.h"
// Define the function pointer type without initialization
typedef void (*InitModuleFunc)(void*);

typedef uint16_t (__thiscall *get_spell_spell_line) (void *, uint16_t);
typedef uint32_t (__thiscall *figure_toolbox_get_unkn)(void *, uint16_t);
typedef void (__thiscall *figure_toolbox_add_spell)(void *, uint16_t, uint16_t);
typedef void (__thiscall *setXData)(SF_CGdSpell *, uint16_t, uint8_t, uint32_t);
typedef void (__thiscall *handler_ptr) (SF_CGdSpell *, uint16_t);
typedef void (*SpellHandlerFunc)(uint16_t spell_index, handler_ptr handler);

// Declare the structure
typedef struct SpellforceSpellFramework {
    InitModuleFunc initializeModule;
    SpellHandlerFunc addSpellHandler;
} SpellforceSpellFramework;


#endif /* SPELLFORCE_SPELL_FRAMEWORK_H */
