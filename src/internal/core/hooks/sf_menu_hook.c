
#include "../sf_wrappers.h"
#include "sf_menu_hook.h"
#include "../sf_hooks.h"
#include "../sf_modloader.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

original_menu_func_ptr original_menu_func;

void __thiscall sf_menu_hook(uint32_t _CAppMenu)
{
    // Messy and ugly Code below, you have been warned. ~Muddykat
    char sfsf_info[256];
    sprintf(sfsf_info, "Spell Framework %s\n%d Mod(s) Loaded with %d Error(s)", framework_mod->mod_version, mod_count, error_count);
    uint32_t CAppMenu_data = *(uint32_t *)(_CAppMenu + 0x4);
    uint32_t CMnuScreen_ptr = *(uint32_t *)(CAppMenu_data + 0x68);
    uint32_t container_hack_ptr = *(uint32_t *)(_CAppMenu + 0x58);
    CMnuContainer *container_hack = (CMnuContainer *)container_hack_ptr;
    uint32_t screen_vftable_ptr = CMnuScreen_ptr;

    attach_new_label(container_hack, sfsf_info, 6, 10, 729, 100, 100);

    // Call original menu func
    original_menu_func(_CAppMenu);
}