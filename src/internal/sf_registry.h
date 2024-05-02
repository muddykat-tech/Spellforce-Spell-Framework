#ifndef REGISTRY_H
#define REGISTRY_H

#include <stdint.h>
#include "../api/sfsf.h"


// Function prototypes for mod loader.
extern void initFramework();

extern handler_ptr get_spell_handler (const uint16_t key);

extern void addSpellHandler(uint16_t spell_index, handler_ptr handler);

#endif // REGISTRY_H