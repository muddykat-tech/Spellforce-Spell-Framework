#ifndef MODLOADER_H
#define MODLOADER_H

#include "../../api/sfsf.h"
// Function prototypes for mod loader.
extern "C" __declspec(dllexport) void initialize_mods();
extern int g_mod_count;
extern int g_error_count;
extern SFMod *g_current_mod;
extern SFMod *g_framework_mod;

#endif // MODLOADER_H
