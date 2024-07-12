
#include "../sf_wrappers.h"
#include "sf_menu_hook.h"
#include "../sf_hooks.h"
#include "../sf_modloader.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

menu_label_ptr initialize_menu_label;
initialize_menu_container_ptr initialize_menu_container;
original_menu_func_ptr original_menu_func;
construct_default_sf_string_ptr construct_default_sf_string;
uint32_t menu_return_addr;
new_operator_ptr new_operator;
message_box_ptr show_message_box;
menu_label_constructor_ptr menu_label_constructor;
mnu_label_init_data_ptr init_menu_element;
menu_label_set_data_ptr menu_label_set_color;
get_smth_fonts_ptr get_smth_fonts;
menu_label_set_font_ptr menu_label_set_font;
get_font_ptr get_font;
container_add_control_ptr container_add_control;
menu_label_set_string_ptr menu_label_set_string;

void initialize_menu_data_hooks()
{
    initialize_menu_label = (menu_label_ptr)(ASI::AddrOf(0x51a180));
    menu_label_set_string = (menu_label_set_string_ptr)(ASI::AddrOf(0x52fab0));

    initialize_menu_container = (initialize_menu_container_ptr)(ASI::AddrOf(0x505780));
    construct_default_sf_string = (construct_default_sf_string_ptr)(ASI::AddrOf(0x383900));

    original_menu_func = (original_menu_func_ptr)(ASI::AddrOf(0x197b10));
    menu_return_addr = (ASI::AddrOf(0x182799));
    show_message_box = (message_box_ptr)(ASI::AddrOf(0x198660));

    new_operator = (new_operator_ptr)(ASI::AddrOf(0x675A9D));
    menu_label_constructor = (menu_label_constructor_ptr)(ASI::AddrOf(0x51a180));
    init_menu_element = (mnu_label_init_data_ptr)(ASI::AddrOf(0x52cfe0));

    // This does seem to work, but has some strange behaviour, the color format is strange
    menu_label_set_color = (menu_label_set_data_ptr)(ASI::AddrOf(0x530330));

    get_smth_fonts = (get_smth_fonts_ptr)(ASI::AddrOf(0x5357b0));
    menu_label_set_font = (menu_label_set_font_ptr)(ASI::AddrOf(0x530e00));
    get_font = (get_font_ptr)(ASI::AddrOf(0x535180));
    container_add_control = (container_add_control_ptr)(ASI::AddrOf(0x506f30));
}

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