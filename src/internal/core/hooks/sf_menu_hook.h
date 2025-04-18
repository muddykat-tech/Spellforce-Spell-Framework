#ifndef SF_MENU_HOOK_H
#define SF_MENU_HOOK_H

#include "../../../api/sfsf.h"
#include "../../../asi/sf_asi.h"

extern uint32_t g_menu_return_addr;
extern uint32_t g_ui_hook_fix_addr;
extern uint32_t g_ui_hook_fix_addr2;

extern menu_label_set_string_ptr g_menu_label_set_string;
extern new_operator_ptr g_new_operator;
extern menu_label_constructor_ptr g_menu_label_constructor;
extern set_label_flags_ptr g_set_label_flags;
extern mnu_label_init_data_ptr g_init_menu_element;
extern get_smth_fonts_ptr g_get_smth_fonts;
extern menu_label_set_font_ptr g_menu_label_set_font;
extern get_font_ptr g_get_font;
extern container_add_control_ptr g_container_add_control;
extern create_option_ptr f_create_menu_option;
extern cuiVideoSequence_constructor_ptr cuiVideoSequence_constructor;
extern CMnuScreen_attach_control_ptr CMnuScreen_attach_control;

extern CMnuLabel *sfsf_version_label;
void __thiscall sf_menu_hook(uint32_t _CAppMenu);
uint16_t __thiscall sf_ui_overlay_fix(SF_CGdFigure *_this, void *CGdResource, uint16_t spell_id, uint16_t figure_id);

void initialize_menu_data_hooks();
void __thiscall sf_click_vertical_button(SF_CUiMain *_this, uint16_t figure_id,
                                         uint8_t entity_type,
                                         uint16_t target_id,
                                         SF_UIElement *element);
void __thiscall sf_click_horizontal_button(SF_CUiMain *_this,
                                           uint_list_node *param1,
                                           SF_UIElement *param2);
void __thiscall sf_handle_button_flashing_maybe(SF_CUiMain *_this);
#endif
