#include <string>
#include "SpellforceSpellFramework.h"
#include <windows.h>

void new_spell_handler(SF_CGdSpell *_this, uint16_t spell_index) {
    // for now, do nothing?
    OutputDebugStringA("Spell Handled?"); 
}

extern "C" __declspec(dllexport) void initializeModule(SpellforceSpellFramework* framework) {
    addSpellHandler(0xb, &new_spell_handler);
    OutputDebugStringA("Module initialized successfully!");
}
