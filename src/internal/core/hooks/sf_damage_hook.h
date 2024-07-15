#ifndef SF_DAMAGE_HOOK_H
#define SF_DAMAGE_HOOK_H

#include "../../../api/sfsf.h"
#include "../../../asi/sf_asi.h"

extern uint32_t g_damage_return_addr;
void __declspec(naked) sf_damage_hook();

#endif