#ifndef MODLOADER_H
#define MODLOADER_H

#include "../../api/sfsf.h"
// Function prototypes for mod loader.
extern "C" __declspec(dllexport) void initialize_mods();
extern int mod_count; 
extern int error_count;
extern SFMod current_mod;

#endif // MODLOADER_H