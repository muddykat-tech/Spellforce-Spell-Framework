#ifndef MODLOADER_H
#define MODLOADER_H

#include "../../api/sfsf.h"
// Function prototypes for mod loader.
extern "C" __declspec(dllexport) void initialize_mods();
extern int mod_count; 

#endif // MODLOADER_H