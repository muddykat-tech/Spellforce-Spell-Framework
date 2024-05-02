#ifndef SPELLFORCE_SPELL_HANDLER_H
#define SPELLFORCE_SPELL_HANDLER_H

__declspec(dllexport) void __thiscall initSpellMap();

// We may need to export individual handlers? but perhaps not?
// __declspec(dllexport) void __thiscall fireburst_handler(SF_CGdSpell* _this, uint16_t spell_index);

#endif 