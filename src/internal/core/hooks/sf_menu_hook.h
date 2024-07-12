#ifndef SF_MENU_HOOK_H
#define SF_MENU_HOOK_H

#include "../../../api/sfsf.h"
#include "../../../asi/sf_asi.h"

extern original_menu_func_ptr original_menu_func;
extern uint32_t menu_return_addr;
extern menu_label_ptr initialize_menu_label;
extern menu_label_set_string_ptr menu_label_set_string;
extern message_box_ptr show_message_box;
extern new_operator_ptr new_operator;
extern menu_label_constructor_ptr menu_label_constructor;
extern mnu_label_init_data_ptr init_menu_element;
extern menu_label_set_data_ptr menu_label_set_color;
extern get_smth_fonts_ptr get_smth_fonts;
extern menu_label_set_font_ptr menu_label_set_font;
extern get_font_ptr get_font;
extern container_add_control_ptr container_add_control;

void __thiscall sf_menu_hook(uint32_t _CAppMenu);
void initialize_menu_data_hooks();

#endif