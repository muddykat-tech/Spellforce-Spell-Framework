// SpellforceSpellFramework.h
#ifndef SPELLFORCE_SPELL_FRAMEWORK_H
#define SPELLFORCE_SPELL_FRAMEWORK_H

// Define the function pointer type without initialization
typedef void (*InitModuleFunc)(void*);

// Declare the structure
typedef struct SpellforceSpellFramework {
    InitModuleFunc initializeModule;
} SpellforceSpellFramework;

// Probably unsafe to do this, but will do for now
extern std::map<uint16_t, handler_ptr> handler_map;
void addSpellHandler(uint16_t spell_index, handler_ptr handler);

#endif /* SPELLFORCE_SPELL_FRAMEWORK_H */
