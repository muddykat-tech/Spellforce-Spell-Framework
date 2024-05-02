#ifndef HOOKS_H
#define HOOKS_H

#include "../api/SpellforceSpellFramework.h"

extern "C" __declspec(dllexport) void addSpellHandler(uint16_t spell_index, handler_ptr handler);
extern "C" __declspec(dllexport) void hookBetaVersion();

#endif 