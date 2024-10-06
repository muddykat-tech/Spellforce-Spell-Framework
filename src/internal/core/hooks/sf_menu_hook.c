
#include "../sf_wrappers.h"
#include "sf_menu_hook.h"
#include "../sf_hooks.h"
#include "../sf_modloader.h"
#include "../../registry/sf_mod_registry.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static menu_label_ptr s_initialize_menu_label;
static initialize_menu_container_ptr s_initialize_menu_container;
static original_menu_func_ptr s_menu_func;
static construct_default_sf_string_ptr s_construct_default_sf_string;
static message_box_ptr s_show_message_box;
static menu_label_set_data_ptr s_menu_label_set_color;

container_add_control_ptr g_container_add_control;
uint32_t g_menu_return_addr;
new_operator_ptr g_new_operator;
menu_label_constructor_ptr g_menu_label_constructor;
set_label_flags_ptr g_set_label_flags;
mnu_label_init_data_ptr g_init_menu_element;
get_smth_fonts_ptr g_get_smth_fonts;
menu_label_set_font_ptr g_menu_label_set_font;
get_font_ptr g_get_font;
menu_label_set_string_ptr g_menu_label_set_string;

void initialize_menu_data_hooks()
{
    // Retrieve function pointers using the Memory Address of Intercepted Function
    // These addresses are found using a Reverse Engineering program called Ghidra

    s_initialize_menu_label = (menu_label_ptr)(ASI::AddrOf(0x51a180));
    s_initialize_menu_container = (initialize_menu_container_ptr)(ASI::AddrOf(0x505780));
    s_construct_default_sf_string = (construct_default_sf_string_ptr)(ASI::AddrOf(0x383900));
    s_menu_func = (original_menu_func_ptr)(ASI::AddrOf(0x197b10));
    s_menu_label_set_color = (menu_label_set_data_ptr)(ASI::AddrOf(0x530330));
    s_show_message_box = (message_box_ptr)(ASI::AddrOf(0x198660));

    g_set_label_flags = (set_label_flags_ptr)(ASI::AddrOf(0x52f1d0));
    g_menu_label_set_string = (menu_label_set_string_ptr)(ASI::AddrOf(0x52fab0));
    g_menu_return_addr = (ASI::AddrOf(0x182799));
    g_new_operator = (new_operator_ptr)(ASI::AddrOf(0x675A9D));
    g_menu_label_constructor = (menu_label_constructor_ptr)(ASI::AddrOf(0x51a180));
    g_init_menu_element = (mnu_label_init_data_ptr)(ASI::AddrOf(0x52cfe0));
    g_get_smth_fonts = (get_smth_fonts_ptr)(ASI::AddrOf(0x5357b0));
    g_menu_label_set_font = (menu_label_set_font_ptr)(ASI::AddrOf(0x530e00));
    g_get_font = (get_font_ptr)(ASI::AddrOf(0x535180));
    g_container_add_control = (container_add_control_ptr)(ASI::AddrOf(0x506f30));
}

void __attribute__((no_caller_saved_registers, thiscall)) sf_menu_hook(uint32_t _CAppMenu)
{
    // String to display in the new label we're attaching to the menu
    char sfsf_info[256];
    sprintf(sfsf_info, "Spell Framework %s\n%d Mod(s) Loaded with %d Error(s)", g_framework_mod->mod_version, g_mod_count, g_error_count);

    // Manually move the pointer in order to access the CMNuContainer
    uint32_t CAppMenu_data = *(uint32_t *)(_CAppMenu + 0x4);
    uint32_t CMnuScreen_ptr = *(uint32_t *)(CAppMenu_data + 0x68);
    uint32_t container_hack_ptr = *(uint32_t *)(_CAppMenu + 0x58);
    CMnuContainer *container_hack = (CMnuContainer *)container_hack_ptr;
    uint32_t screen_vftable_ptr = CMnuScreen_ptr;

    attach_new_label(container_hack, sfsf_info, 6, 10, 729, 100, 100);

    log_info("-==== Mod Information Start ====-");
    SFMod *old_parent;
    for (SFSpell *spell_data : g_internal_spell_list)
    {
        // Let's add mod information into the console
        SFMod *parent_mod = spell_data->parent_mod;
        if (old_parent != parent_mod)
        {
            log_info(parent_mod->mod_id);
            log_info(parent_mod->mod_version);
            log_info(parent_mod->mod_author);
            log_info(parent_mod->mod_description);
            old_parent = parent_mod;
        }
    }
    log_info("-==== Mod Information End ====-");
    // Call original menu function to show the menu
    s_menu_func(_CAppMenu);
}