#ifndef SPELLFORCE_SPELL_HANDLER_H
#define SPELLFORCE_SPELL_HANDLER_H

#include "../api/sfsf.h"

extern "C" __declspec(dllexport) void initSpellMap();
extern "C" __declspec(dllexport) void addSpellHandler(uint16_t spell_index, handler_ptr handler);
extern "C" __delcspec(dllexport) handler_ptr get_spell_handler (const uint16_t key);

// We may need to export individual handlers? but perhaps not?
// __declspec(dllexport) void __thiscall fireburst_handler(SF_CGdSpell* _this, uint16_t spell_index);

#endif 