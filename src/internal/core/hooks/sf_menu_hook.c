/**
 * @defgroup MenuHook Menu Hook
 * @ingroup Hooks
 * @brief Only Used internally to inject text onto the Main Menu of Spellforce
 * @note The menu functions and general UI elements of Spellforce are poorly understood and may have unknown consequences
 * @addtogroup MenuHook
 * @{
 */

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

cuiVideoSequence_constructor_ptr cuiVideoSequence_constructor;
CMnuScreen_attach_control_ptr CMnuScreen_attach_control;
create_option_ptr f_create_menu_option;
container_add_control_ptr g_container_add_control;
uint32_t g_menu_return_addr;
uint32_t g_ui_hook_fix_addr;
uint32_t g_ui_hook_fix_addr2;
new_operator_ptr g_new_operator;
menu_label_constructor_ptr g_menu_label_constructor;
set_label_flags_ptr g_set_label_flags;
mnu_label_init_data_ptr g_init_menu_element;
get_smth_fonts_ptr g_get_smth_fonts;
menu_label_set_font_ptr g_menu_label_set_font;
get_font_ptr g_get_font;
menu_label_set_string_ptr g_menu_label_set_string;

autoclass113_fun_00a27530_ptr fun_00a27530;
fun_0086dd60_ptr fun_0086dd60;
autoclass113_fun_00a278c0_ptr fun_00a278c0;
fun_00a2a1d0_ptr fun_00a2ald0;
fun_006a0140_ptr fun_006a0140;
fun_009a2790_ptr fun_009a2790;
fun_0069f8d0_ptr fun_0069f8d0;
fun_0069fb90_ptr fun_0069fb90;
fun_00a49b10_ptr fun_00a49b10;
fun_00a28d60_ptr fun_00a28d60;
fun_009a4020_ptr fun_009a4020;
vfun164_ptr vfun164;
vfun163_ptr vfun163;
vfun41_ptr vfun41;
getSpellLineIsTargetSelf_ptr getSpellLineIsTargetSelf;
fun_009a0750_ptr fun_009a0750;
fun_009de190_ptr fun_009de190;
fun_0099f610_ptr fun_0099f610;

fun_009cd1f0_ptr fun_009cd1f0;
fun_009a1fd0_ptr fun_009a1fd0;
fun_006f8c06_ptr fun_006f8c06;
fun_00910de0_ptr fun_00910de0;

void initialize_menu_data_hooks()
{
    // Retrieve function pointers using the Memory Address of Intercepted Function
    // These addresses are found using a Reverse Engineering program called Ghidra

    s_initialize_menu_label = (menu_label_ptr)(ASI::AddrOf(0x51a180));
    s_initialize_menu_container =
        (initialize_menu_container_ptr)(ASI::AddrOf(0x505780));
    s_construct_default_sf_string =
        (construct_default_sf_string_ptr)(ASI::AddrOf(0x383900));
    s_menu_func = (original_menu_func_ptr)(ASI::AddrOf(0x197b10));
    s_menu_label_set_color = (menu_label_set_data_ptr)(ASI::AddrOf(0x530330));
    s_show_message_box = (message_box_ptr)(ASI::AddrOf(0x198660));

    g_set_label_flags = (set_label_flags_ptr)(ASI::AddrOf(0x52f1d0));
    g_menu_label_set_string =
        (menu_label_set_string_ptr)(ASI::AddrOf(0x52fab0));
    g_menu_return_addr = (ASI::AddrOf(0x182799));
    g_ui_hook_fix_addr = (ASI::AddrOf(0x5D119E));
    g_ui_hook_fix_addr2 = (ASI::AddrOf(0x5d0a7e));
    g_new_operator = (new_operator_ptr)(ASI::AddrOf(0x675A9D));
    g_menu_label_constructor =
        (menu_label_constructor_ptr)(ASI::AddrOf(0x51a180));
    g_init_menu_element = (mnu_label_init_data_ptr)(ASI::AddrOf(0x52cfe0));
    g_get_smth_fonts = (get_smth_fonts_ptr)(ASI::AddrOf(0x5357b0));
    g_menu_label_set_font = (menu_label_set_font_ptr)(ASI::AddrOf(0x530e00));
    g_get_font = (get_font_ptr)(ASI::AddrOf(0x535180));
    g_container_add_control =
        (container_add_control_ptr)(ASI::AddrOf(0x506f30));

    f_create_menu_option = (create_option_ptr)(ASI::AddrOf(0x61CF80));

    fun_00a27530 = (autoclass113_fun_00a27530_ptr)(ASI::AddrOf(0x627530));
    fun_0086dd60 = (fun_0086dd60_ptr)(ASI::AddrOf(0x46DD60));
    fun_00a278c0 = (autoclass113_fun_00a278c0_ptr)(ASI::AddrOf(0x6278C0));
    fun_00a2ald0 = (fun_00a2a1d0_ptr)(ASI::AddrOf(0x62A1D0));
    fun_006a0140 = (fun_006a0140_ptr)(ASI::AddrOf(0x2A0140));
    fun_009a2790 = (fun_009a2790_ptr)(ASI::AddrOf(0x5A2790));
    fun_0069f8d0 = (fun_0069f8d0_ptr)(ASI::AddrOf(0x29F8D0));
    fun_0069fb90 = (fun_0069fb90_ptr)(ASI::AddrOf(0x29FB90));
    fun_00a49b10 = (fun_00a49b10_ptr)(ASI::AddrOf(0x649b10));

    fun_00a28d60 = (fun_00a28d60_ptr)(ASI::AddrOf(0x628d60));
    fun_009a4020 = (fun_009a4020_ptr)(ASI::AddrOf(0x5a4020));
    fun_009a0750 = (fun_009a0750_ptr)(ASI::AddrOf(0x5a0750));
    fun_009de190 = (fun_009de190_ptr)(ASI::AddrOf(0x5de190));
    fun_0099f610 = (fun_0099f610_ptr)(ASI::AddrOf(0x59f610));
    fun_009cd1f0 = (fun_009cd1f0_ptr)(ASI::AddrOf(0x5cd1f0));
    fun_009a1fd0 = (fun_009a1fd0_ptr)(ASI::AddrOf(0x5a1fd0));
    fun_006f8c06 = (fun_006f8c06_ptr)(ASI::AddrOf(0x2f8c06));
    fun_00910de0 = (fun_00910de0_ptr)(ASI::AddrOf(0x510de0));

    vfun163 = (vfun163_ptr)(ASI::AddrOf(0x513C80));
    vfun164 = (vfun164_ptr)(ASI::AddrOf(0x50F8B0));
    vfun41 = (vfun41_ptr)(ASI::AddrOf(0x510d70));

    getSpellLineIsTargetSelf =
        (getSpellLineIsTargetSelf_ptr)(ASI::AddrOf(0x26e410));

    cuiVideoSequence_constructor =
        (cuiVideoSequence_constructor_ptr)(ASI::AddrOf(0x618980));

    CMnuScreen_attach_control =
        (CMnuScreen_attach_control_ptr)(ASI::AddrOf(0x507240));
}

SFSF_ModlistStruct mod_struct;
void __attribute__((no_caller_saved_registers,
                    thiscall)) sf_menu_hook(uint32_t _CAppMenu)
{
    log_info("Starting Menu Hook");
    // String to display in the new label we're attaching to the menu
    char sfsf_info[256];
    sprintf(sfsf_info, "Spell Framework %s\n%d Mod(s) Loaded with %d Error(s)",
            g_framework_mod->mod_version, g_mod_count, g_error_count);
    // Manually move the pointer in order to access the CMNuContainer, We'll need to annotate the CAppMenu Structure more to
    // Switch to a more convential method.
    uint32_t CAppMenu_data = *(uint32_t *)(_CAppMenu + 0x4);
    uint32_t container_hack_ptr = *(uint32_t *)(_CAppMenu + 0x58);
    CMnuContainer *container_hack = (CMnuContainer *)container_hack_ptr;

    CMnuLabel *sfsf_version_label;
    attach_new_label(sfsf_version_label, container_hack, sfsf_info, 6, 10, 729,
                     strlen(sfsf_info) * 4, 100);
    char sfsf_test_button_default[256];
    char sfsf_test_button_pressed[256];
    char sfsf_test_button_disabled[256];
    char sfsf_test_button_highlight[256];
    char sfsf_test_button_label[256];

    sprintf(sfsf_test_button_default, "ui_mainmenu_button_default.msh");
    sprintf(sfsf_test_button_pressed, "ui_mainmenu_button_pressed.msh");
    sprintf(sfsf_test_button_highlight, "");
    sprintf(sfsf_test_button_disabled, "ui_mainmenu_button_disabled.msh");
    sprintf(sfsf_test_button_label, "SHOW MOD LIST");

    // Initialize struct members
    mod_struct.toggle = 0;
    mod_struct.index = 0;

    log_info("Adding Mod List Button");
    int button_index = 15;
    attach_new_button(container_hack, sfsf_test_button_default,
                      sfsf_test_button_pressed, sfsf_test_button_highlight,
                      sfsf_test_button_disabled, sfsf_test_button_label, 7, 822,
                      705, 192, 36, button_index,
                      (uint32_t)&show_mod_list_callback);

    // Call original menu function to show the menu
    // char vid_loc[256];
    // sprintf(vid_loc, "videos\\sfsf");
    // attachVideo((CAppMenu*)_CAppMenu, container_hack, vid_loc);

    s_menu_func(_CAppMenu);
}

bool hasThisAuraRunning(SF_CGdFigureToolbox *_this, uint16_t aura_spell_id,
                        uint16_t figure_id)
{
    if (figureAPI.isAlive(_this->CGdFigure, figure_id))
    {
        if (figureAPI.isFlagSet(_this->CGdFigure, figure_id, AURA_RUNNING))
        {
            uint16_t node_id = figureAPI.getSpellJobStartNode(_this->CGdFigure,
                                                              figure_id);
            while (node_id != 0)
            {
                uint16_t spell_index =
                    toolboxAPI.getSpellIndexFromDLL(_this->CGdDoubleLinkedList,
                                                    node_id);
                uint16_t spell_id = spellAPI.getSpellID(_this->CGdSpell,
                                                        spell_index);
                if (spell_id == aura_spell_id)
                {
                    return 1;
                }
                node_id = toolboxAPI.getNextNode(_this->CGdDoubleLinkedList,
                                                 node_id);
            }
        }
    }
    return false;
}

// CUiMain::FUN_009e6840
void __attribute__((thiscall)) sf_click_vertical_button(SF_CUiMain *_this,
                                                        uint16_t figure_id,
                                                        uint8_t entity_type,
                                                        uint16_t target_id,
                                                        SF_UIElement *element)
{
    log_info("Called SF_CLICK_VERTICAL_BUTTON");
    uint16_t actionID = element->actionType_id;
    uint16_t subActionID = element->actionSubtype_id;
    uint32_t _figure_id = figure_id;
    if ((actionID == 10000) || (actionID == 10001) || (actionID == 10002))
    {
        uint_list_node ac113;
        fun_00a27530(&ac113);
        uint32_t *puvar5 = fun_0086dd60(&_figure_id, 1, _figure_id & 0xffff);
        fun_00a278c0(&ac113, *puvar5);
        SF_CGdTargetData data;
        data.entity_index = target_id;
        data.entity_type = entity_type;
        data.position.X = 0;
        data.position.Y = 0;
        uint32_t uVar3 = fun_00a2ald0(&ac113,
                                      _this->CUiMain_data.CGdControllerClient);
        fun_006a0140(_this->CUiMain_data.CGdControllerClient, uVar3, &data, 0,
                     0);
        if (ac113.first != 0)
        {
            fun_009a2790(&ac113, ac113.first,
                         (uint32_t)ac113.post_last - (uint32_t)ac113.first >>
                         2);
            return;
        }
    }
    if ((actionID != 0) && (actionID < 10000))
    {
        if (spellAPI.hasSpellTag(actionID, SpellTag::AURA_SPELL))
        {
            if (hasThisAuraRunning(_this->CUiMain_data.CGdFigureToolBox,
                                   subActionID, figure_id))
            {
                fun_0069f8d0((_this->CUiMain_data.CGdControllerClient),
                             figure_id);
                return;
            }
        }
        CGdFigureTask task =
            _this->CUiMain_data.CGdFigure->figures[figure_id].ac_1.task;
        SF_CGdTargetData data;
        data.entity_index = target_id;
        data.entity_type = entity_type;
        data.position.X = 0;
        data.position.Y = 0;
        uint32_t some_flag = 0;
        if (task == TASK_MAINCHAR)
        {
            some_flag =
                (uint32_t)(*(uint32_t *)&(_this->CUiMain_data.unkn5[0x2A0]) ==
                           2);
        }
        fun_0069fb90(_this->CUiMain_data.CGdControllerClient, figure_id,
                     element->unknown_flag, element->unknown_config_param,
                     &data, some_flag, 0);
    }
}

uint16_t __attribute__((thiscall)) sf_ui_overlay_fix(SF_CGdFigure *_this, void *CGdResource, uint16_t spell_id,
                                                     uint16_t figure_id)
{
    if ((_this->figures[figure_id].race != 0) && (_this->figures[figure_id].race < 7))
    {
        if (_this->figures[figure_id].owner != 0)
        {
            spell_id = spellAPI.getLeveledSpellID(CGdResource, spell_id, _this->figures[figure_id].level);
        }
    }
    return spell_id;
}

void __attribute__((thiscall)) sf_click_horizontal_button(SF_CUiMain *_this,
                                                          uint_list_node *param1,
                                                          SF_UIElement *param2)
{
    uint16_t action_id = param2->actionType_id;
    uint16_t subAction_id =  param2->actionSubtype_id;
    uint32_t unused;
    uint16_t figure_id = ((*fun_00a28d60(param1, &unused, 0)) >> 8) & 0xffff;
    if (action_id == 0)
    {
        action_id = param2->actionSubtype_id;
        if (action_id < 9)
        {
            return;
        }
        if (action_id > 0xe)
        {
            return;
        }
        uint32_t general_address = (ASI::AddrOf(0x806a86));
        fun_009a4020(_this->CUiMain_data.CUiBuilding,
                     *(uint32_t *)(general_address +
                                   ((uint8_t)_this->CUiMain_data.CGdFigure->figures[figure_id].race * 4) + 2));
        if (_this->CUiMain_data.unknown_action_type == 2)
        {
            return;
        }
        _this->CUiMain_data.unknown_action_type = 2;
        vfun163(_this->CUiMain_data.CUiMainDetailView, 0);
        vfun164(_this->CUiMain_data.CUiBottom);
        vfun164(_this->CUiMain_data.CUiMonument);
        vfun163(_this->CUiMain_data.CUiBuilding, 0);
        return;
    }
    if (action_id > 9999)
    {
        return;
    }
    SF_CGdResourceSpell spell_data;
    spellAPI.getResourceSpellData(_this->CUiMain_data.CGdResource, &spell_data, subAction_id);
    SF_CGdTargetData data;
    data.entity_index = 0;
    data.entity_type = 0;
    data.position.X = 0;
    data.position.Y = 0;

    if (!spellAPI.hasSpellTag(spell_data.spell_line_id, SpellTag::AURA_SPELL))
    {
        if (!getSpellLineIsTargetSelf(_this->CUiMain_data.CGdResource,
                                      spell_data.spell_line_id))
        {
            uint_list_node *node = (uint_list_node *)
                                   fun_009a0750(_this->CUiMain_data.CUiGame);
            fun_009de190(node, param1);
            fun_0099f610(_this->CUiMain_data.CUiGame, param2->actionType_id,
                         param2->actionSubtype_id, param2->unknown_flag,
                         param2->unknown_config_param);
            return;
        }
        if (spell_data.cast_type2 == 5)
        {
            data.position.X =
                _this->CUiMain_data.CGdFigure->figures[figure_id].position.X;
            data.position.Y =
                _this->CUiMain_data.CGdFigure->figures[figure_id].position.Y;
            data.entity_type = 5;
        }
        else
        {
            data.entity_type = 1;
            data.entity_index = figure_id;
        }
        uint_list_node *node = (uint_list_node *)
                               fun_009a0750(_this->CUiMain_data.CUiGame);
        fun_009de190(node, param1);
    }
    else
    {
        if (hasThisAuraRunning(_this->CUiMain_data.CGdFigureToolBox,
                               param2->actionSubtype_id, figure_id))
        {
            fun_0069f8d0((_this->CUiMain_data.CGdControllerClient), figure_id);
            fun_009cd1f0(_this->CUiMain_data.CUiBottom, 1);
            fun_009a1fd0(_this->CUiMain_data.CUiGame, 0);
            return;
        }
        if (!fun_006f8c06(_this->CUiMain_data.CGdFigureToolBox, figure_id,
                          param2->actionSubtype_id, figure_id, 1, 0,
                          1, 1, 1))
        {
            uint32_t *unknown_field = (uint32_t *)vfun41(_this);
            fun_00910de0(unknown_field, _this, 0x140);
            fun_009cd1f0(_this->CUiMain_data.CUiBottom, 1);
            fun_009a1fd0(_this->CUiMain_data.CUiGame, 0);
            return;
        }

        data.entity_type = 1;
        data.entity_index = figure_id;
    }
    fun_0069fb90(_this->CUiMain_data.CGdControllerClient, figure_id,
                 param2->unknown_flag, param2->unknown_config_param, &data, 1,
                 1);
    fun_009cd1f0(_this->CUiMain_data.CUiBottom, 1);
    fun_009a1fd0(_this->CUiMain_data.CUiGame, 0);
    return;
}



/** @} */
