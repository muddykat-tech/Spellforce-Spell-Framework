#include <string>
#include <windows.h>
#include "api/sf_spell.h"
#include "SpellforceSpellFramework.h"

void __thiscall new_spell_handler(SF_CGdSpell * spell, uint16_t spell_index) {
    // for now, do nothing?
    OutputDebugStringA("Spell Handled?"); 
}

extern "C" __declspec(dllexport) void initializeModule(SpellforceSpellFramework* framework) {
    framework->addSpellHandler(0xb, &new_spell_handler);
    OutputDebugStringA("Module initialized successfully!");
}
