#ifndef REGISTRY_H
#define REGISTRY_H

#include <stdint.h>
#include "../api/sfsf.h"


// Function prototypes for mod loader.
extern "C" __declspec(dllexport) void initFramework();
extern "C" __declspec(dllexport) handler_ptr get_spell_handler (const uint16_t key);

extern "C" __declspec(dllexport) void addSpellHandler(uint16_t spell_index, handler_ptr handler);
extern "C" __declspec(dllexport) void default_handler(SF_CGdSpell * spell, uint16_t spell_index);

#endif // REGISTRY_H