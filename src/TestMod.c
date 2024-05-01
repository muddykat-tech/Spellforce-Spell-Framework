#include <string>
#include "SpellforceSpellFramework.h"

void new_spell_handler(SF_CGdSpell *_this, uint16_t spell_index) {
    // for now, do nothing?
    printf("Spell Handled?\n"); 
}

extern "C" __declspec(dllexport) void initializeModule(SpellforceSpellFramework* framework) {
    addSpellHandler(0xb, &new_spell_handler);
    printf("Module initialized successfully!\n");
}
