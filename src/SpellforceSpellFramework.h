// SpellforceSpellFramework.h
#ifndef SPELLFORCE_SPELL_FRAMEWORK_H
#define SPELLFORCE_SPELL_FRAMEWORK_H

// Define the function pointer type without initialization
typedef void (*InitModuleFunc)(void*);

// Declare the structure
typedef struct SpellforceSpellFramework {
    InitModuleFunc initializeModule;
} SpellforceSpellFramework;

#endif /* SPELLFORCE_SPELL_FRAMEWORK_H */
