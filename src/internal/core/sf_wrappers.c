#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sf_wrappers.h"
#include "sf_hooks.h"

#include "hooks/sf_menu_hook.h"
#include "hooks/sf_console_hook.h"
#include "../registry/sf_mod_registry.h"

/**
 * @defgroup wrappers Framework Wrappers
 * @ingroup Internal
 * @brief Contains wrapper functions that streamline complex setups.
 * @details This file consolidates multiple setup steps into single functions for ease of use,
 * handling the initialization of variables, flags, and resources required for specific tasks.
 * @addtogroup wrappers
 * @{
 */

FUN_0069eaf0_ptr FUN_0069eaf0;
fidfree_ptr fidFree;
SF_String_ctor_ptr g_create_sf_string;
SF_String_dtor_ptr g_destroy_sf_string;
has_spell_effect_ptr has_spell_effect;
create_button_ptr create_button_func;
attach_string_ptr vfunction_apply_string;
vfunction_ptr vfunction176;
vfunction_ptr vfunction25;
vfunction12_ptr vfunction12;
initialize_smp_button_ptr initialize_smp_button;
set_btn_name_ptr set_button_name;
initialize_menu_container_ptr initialize_menu_container;
set_label_color_ptr set_label_color;
container_alpha_ptr set_container_alpha;
setup_menu_container_data_ptr setup_menu_container_data;
vfunction_2_ptr set_font;
set_btn_index_ptr set_button_index;
set_button_flag_ptr set_menu_button_flag;
vfunction2_callback_attach_ptr attach_callback;
vfunction_ptr vfunction16_attach_callback;
CMnuBase_setname_ptr CMnuBase_setname;

void initialize_wrapper_data_hooks()
{
    FUN_0069eaf0 = (FUN_0069eaf0_ptr)(ASI::AddrOf(0x29EAF0));
    fidFree = (fidfree_ptr)(ASI::AddrOf(0x6B6E25));
    has_spell_effect = (has_spell_effect_ptr)(ASI::AddrOf(0x2fe46f));
    set_label_color = (set_label_color_ptr)(ASI::AddrOf(0x530330));
    set_container_alpha = (container_alpha_ptr)(ASI::AddrOf(0x512EB0));
    set_button_name = (set_btn_name_ptr) (ASI::AddrOf(0x52f8a0));
    initialize_menu_container =
        (initialize_menu_container_ptr)(ASI::AddrOf(0x505780));
    setup_menu_container_data =
        (setup_menu_container_data_ptr)(ASI::AddrOf(0x50FD30));
    initialize_smp_button = (initialize_smp_button_ptr) (ASI::AddrOf(0x51a9d0));
    create_button_func = (create_button_ptr) (ASI::AddrOf(0x52E1E0));
    set_font = (vfunction_2_ptr)(ASI::AddrOf(0x530C20));
    set_button_index = (set_btn_index_ptr)(ASI::AddrOf(0x5136a0));
    set_menu_button_flag = (set_button_flag_ptr)(ASI::AddrOf(0x5308A0));
    attach_callback = (vfunction2_callback_attach_ptr)(ASI::AddrOf(0x6188B0));
    vfunction16_attach_callback = (vfunction_ptr) (ASI::AddrOf(0x532B90));
    vfunction176 = (vfunction_ptr)(ASI::AddrOf(0x52f520));
    vfunction25 = (vfunction_ptr)(ASI::AddrOf(0x511ae0));
    CMnuBase_setname = (CMnuBase_setname_ptr)(ASI::AddrOf(0x512E30));

}

void log_message(const char *filename, const char *message)
{
    FILE *file = fopen(filename, "a");
    if (file != NULL)
    {
        fprintf(file, "%s\n", message);
        fclose(file);
    }
}

void log_warning(const char *message)
{
    // Logs a warning message to the console and the debug output
    static char modifiedMessage[256];
    snprintf(modifiedMessage, sizeof(modifiedMessage), "[WARNING] %s", message);
    console_log(modifiedMessage);
}

void log_info(const char *message)
{
    // Logs an informational message to the console and the debug output
    static char modifiedMessage[256];
    snprintf(modifiedMessage, sizeof(modifiedMessage), "[SFSF] %s", message);
    console_log(modifiedMessage);
}

void log_error(const char *message)
{
    // Logs an error message to the console and the debug output, including the last error code
    int lastError = GetLastError();
    static char modifiedMessage[256];
    snprintf(modifiedMessage, sizeof(modifiedMessage),
             "[ERROR] %s [(Win32 SysErr) Last Error: %d]", message, lastError);
    console_log(modifiedMessage);
}

SFLog sf_logger;
SFLog *setup_logger()
{
    sf_logger.logError = &log_error;
    sf_logger.logInfo = &log_info;
    sf_logger.logWarning = &log_warning;
    return &sf_logger;
}

// TODO: Parse me through an as an ActionAPI Wrapped Function
bool __thiscall isActionMelee(SF_SGtFigureAction *_this)
{
    if ((_this->type == 10000) || (_this->type == 0x2711))
    {
        return 1;
    }
    return 0;
}

void __thiscall setupFigureIterator(CGdFigureIterator *iterator,
                                    SF_CGdSpell *spell)
{
    iteratorAPI.figureIteratorInit(iterator, 0x0, 0x0, 0x3ff, 0x3ff);
    iteratorAPI.figureIteratorSetPointers(iterator, spell->SF_CGdFigure,
                                          spell->unkn3, spell->SF_CGdWorld);
}

bool __thiscall hasSpellTag(uint16_t spell_id, SpellTag tag)
{
    return spellAPI.getSpellTags(spell_id) & tag;
}

uint32_t getDistance(SF_Coord *pointA, SF_Coord *pointB)
{

    uint32_t delta;
    uint32_t uVar1;
    uint32_t uVar2;
    uint32_t uVar3;
    uint32_t uVar4;

    delta = (uint32_t)(uint16_t)pointA->X - (uint32_t)(uint16_t)pointB->X;
    uVar2 = (int)delta >> 0x1f;
    uVar2 = (delta ^ uVar2) - uVar2;
    uVar4 = uVar2 & 0xffff;
    delta = (uint32_t)(uint16_t)pointA->Y - (uint32_t)(uint16_t)pointB->Y;
    uVar3 = (int)delta >> 0x1f;
    uVar3 = (delta ^ uVar3) - uVar3;
    uVar1 = uVar3 & 0xffff;
    delta = uVar1;
    if ((uint16_t)uVar2 < (uint16_t)uVar3)
    {
        delta = uVar4;
        uVar4 = uVar1;
    }
    return ((delta * 0xd) >> 5) + uVar4;
}

// Some funky stuff to clean up Iterator memory, not 100% sure if correct
void __thiscall disposeFigureIterator(CGdFigureIterator *iterator)
{
    uint32_t unused;
    FUN_0069eaf0(&(iterator->data.offset_0x30), &unused,
                 ((AutoClass69 *)iterator->data.offset_0x30.ac69_ptr1)->
                 ac69_ptr1, iterator->data.offset_0x30.ac69_ptr1);
    fidFree(iterator->data.offset_0x30.ac69_ptr1);
}

void __thiscall spellEffectCallback(SF_CGdSpell *_this, uint16_t source_index,
                                    uint16_t spell_index,
                                    bool (*condition)(SF_CGdSpell *_this,
                                                      uint16_t source_index,
                                                      uint16_t spell_index),
                                    void (*callback)(SF_CGdSpell *_this,
                                                     uint16_t source_index,
                                                     uint16_t spell_index))
{
    if (!condition || !callback)
        return;

    uint16_t node_id = figureAPI.getSpellJobStartNode(_this->SF_CGdFigure,
                                                      source_index);
    while (node_id != 0)
    {
        uint16_t current_index =
            toolboxAPI.getSpellIndexFromDLL(
                (uint32_t *)_this->SF_CGdDoubleLinkedList, node_id);
        if ((*condition)(_this, current_index, spell_index))
        {
            (*callback)(_this, source_index, current_index);
            break;
        }
        node_id =
            toolboxAPI.getNextNode((uint32_t *)_this->SF_CGdDoubleLinkedList,
                                   node_id);
    }
}


int8_t __thiscall addBonusMultExt (FigureStatisticExt *_this, int8_t value)
{
    if (value < 0)
    {
        if ((int)(_this->bonus_multiplier) + (int)value < -127)
        {
            _this->bonus_multiplier = 128;
            return _this->bonus_multiplier;
        }
        _this->bonus_multiplier += value;
        return _this->bonus_multiplier;
    }
    if ((int)_this->bonus_multiplier + (int)value > 126)
    {
        _this->bonus_multiplier = 127;
        return _this->bonus_multiplier;
    }
    _this->bonus_multiplier += value;
    return _this->bonus_multiplier;
}
void __thiscall addBonusMultToStatistic(SF_CGdFigure *figure,
                                        StatisticDataKey key, uint16_t target,
                                        int8_t value)
{
    bool invalid = FALSE;
    FigureStatistic *statistic;
    switch (key)
    {
        case ARMOR:
        {
            statistic = &(figure->figures[target].armor);
            break;
        }
        case AGILITY:
        {
            statistic = &(figure->figures[target].agility);
            break;
        }
        case CHARISMA:
        {
            statistic = &(figure->figures[target].charisma);
            break;
        }
        case DEXTERITY:
        {
            statistic = &(figure->figures[target].dexterity);
            break;
        }
        case INTELLIGENCE:
        {
            statistic = &(figure->figures[target].intelligence);
            break;
        }
        case STRENGTH:
        {
            statistic = &(figure->figures[target].strength);
            break;
        }
        /***
         * Stamina is EXT multiplier and requires specific handling
         */
        case STAMINA:
        {
            addBonusMultExt(&(figure->figures[target].stamina), value);
            return;
        }
        case WISDOM:
        {
            statistic = &(figure->figures[target].wisdom);
            break;
        }
        case RESISTANCE_FIRE:
        {
            statistic = &(figure->figures[target].resistance_fire);
            break;
        }
        case RESISTANCE_ICE:
        {
            statistic = &(figure->figures[target].resistance_ice);
            break;
        }
        case RESISTANCE_MENTAL:
        {
            statistic = &(figure->figures[target].resistance_mental);
            break;
        }
        case RESISTANCE_BLACK:
        {
            statistic = &(figure->figures[target].resistance_black);
            break;
        }
        case WALK_SPEED:
        {
            statistic = &(figure->figures[target].walk_speed);
            break;
        }
        case FIGHT_SPEED:
        {
            statistic = &(figure->figures[target].fight_speed);
            break;
        }
        case CAST_SPEED:
        {
            statistic = &(figure->figures[target].cast_speed);
            break;
        }
        default:
        {
            invalid = TRUE;
            break;
        }
    }

    if (invalid)
    {
        char message[256];
        sprintf(message, "INVALID STATISTIC KEY: %d", key);
        log_warning(message);
        return;
    }
    figureAPI.addBonusMult(statistic, value);
}

void __thiscall spellClearFigureFlag(SF_CGdSpell *_this, uint16_t spell_id,
                                     SpellFlagKey key)
{
    switch (key)
    {
        case CHECK_SPELLS_BEFORE_CHECK_BATTLE:
        {
            spellAPI.figClrChkSplBfrChkBattle(_this, spell_id, 0);
            break;
        }
        case CHECK_SPELLS_BEFORE_JOB2:
        {
            spellAPI.figTryClrCHkSPlBfrJob2(_this, spell_id);
            break;
        }
        case UNFREEZE:
        {
            spellAPI.figTryUnfreeze(_this, spell_id, 0);
            break;
        }
    }
}

bool __thiscall hasAuraActive(SF_CGdFigureToolbox *_this, uint16_t figure_id)
{
    return has_spell_effect(_this, figure_id, 0x49);
}

CMnuLabel * __thiscall attach_new_label(CMnuLabel *label_ptr,
                                        CMnuContainer *parent,
                                        char *label_chars, uint8_t font_index,
                                        uint16_t x_pos, uint16_t y_pos,
                                        uint16_t width, uint16_t height)
{
    char empty[1];
    sprintf(empty, "");
    return attach_new_meshed_label(label_ptr, parent, empty, label_chars,
                                   font_index, x_pos, y_pos, width, height);
}

void wrap_text(const char *input, char *output, size_t max_width)
{
    size_t input_len = strlen(input);
    size_t current_line_width = 0;
    size_t output_index = 0;

    for (size_t i = 0; i < input_len; ++i)
    {
        output[output_index++] = input[i];
        current_line_width++;

        if (current_line_width >= max_width)
        {
            size_t break_point = output_index;
            while (break_point > 0 && !isspace(output[break_point - 1]))
            {
                break_point--;
            }

            if (break_point > 0)
            {
                output[break_point - 1] = '\n';
                current_line_width = output_index - break_point;
            }
            else
            {
                output[output_index++] = '\n';
                current_line_width = 0;
            }
        }
    }

    output[output_index] = '\0';
}

static bool is_init_finished = false;

/**
 * Helper function to update label text using SF_String
 *
 * @param label The label to update
 * @param text New text to set
 */
static void update_label_text(CMnuLabel *label, const char *text)
{
    if (!label || !text)
    {
        return;
    }

    SF_String string_obj;
    SF_String *sf_string = g_create_sf_string(&string_obj, text);
    g_menu_label_set_string(label, sf_string);
    g_destroy_sf_string(sf_string);
}

/**
 * Attaches mod labels to the container based on pagination settings
 *
 * @param container The menu container to attach labels to
 * @param mods_per_page Number of mods to display per page
 * @param page Current page number (0-based)
 */
void attach_mod_labels(CMnuContainer *container, int mods_per_page, int page)
{
    if (g_internal_spell_list.empty())
    {
        return;  // Early return if no mods to display
    }

    // Calculate pagination values
    const int start_index = mods_per_page * page;
    const int end_index = start_index + mods_per_page;
    const int y_base_offset = 48;
    const int y_item_spacing = 36;

    SFMod *current_parent = nullptr;
    int mod_index = 0;

    // Iterate through spell list to find unique parent mods
    for (const SFSpell *spell_data : g_internal_spell_list)
    {
        SFMod *parent_mod = spell_data->parent_mod;

        // Only process each mod once
        if (current_parent != parent_mod)
        {
            // Check if this mod should be displayed on the current page
            if (mod_index >= start_index && mod_index < end_index)
            {
                const int relative_y_pos = (mod_index - start_index) *
                                           y_item_spacing;
                const int absolute_y_pos = y_base_offset + relative_y_pos;

                // Prepare mod information strings
                char mod_title[512] = {0};
                snprintf(mod_title, sizeof(mod_title), "%s %s\nby %s",
                         parent_mod->mod_id,
                         parent_mod->mod_version,
                         parent_mod->mod_author);

                char mod_description[512] = {0};
                snprintf(mod_description, sizeof(mod_description), "%s",
                         parent_mod->mod_description);

                char wrapped_description[1024] = {0};
                wrap_text(mod_description, wrapped_description, 64);

                char mod_page_info[48] = {0};
                snprintf(mod_page_info, sizeof(mod_page_info), "(%u / %u)",
                         (mod_index + 1), (g_mod_count + 1));

                char mod_error_info[512] = {0};
                if (parent_mod->mod_errors && parent_mod->mod_errors[0] != '\0')
                {
                    snprintf(mod_error_info, sizeof(mod_error_info), "%s",
                             parent_mod->mod_errors);
                }
                else
                {
                    snprintf(mod_error_info, sizeof(mod_error_info),
                             "No errors found");
                }

                char wrapped_error_info[1024] = {0};
                wrap_text(mod_error_info, wrapped_error_info, 64);

                // Update or create labels
                if (is_init_finished)
                {
                    // Update existing labels
                    update_label_text(mod_struct.title_label, mod_title);
                    update_label_text(mod_struct.desc_label,
                                      wrapped_description);
                    update_label_text(mod_struct.page_label, mod_page_info);
                    update_label_text(mod_struct.error_label,
                                      wrapped_error_info);
                }
                else
                {
                    // Create new labels
                    mod_struct.title_label =
                        attach_new_label(nullptr, container, mod_title,
                                         6, 100, absolute_y_pos - 32, 50,
                                         y_item_spacing);

                    mod_struct.desc_label =
                        attach_new_label(nullptr, container,
                                         wrapped_description,
                                         11, 48, absolute_y_pos + 24,
                                         227, y_item_spacing);

                    mod_struct.page_label =
                        attach_new_label(nullptr, container, mod_page_info,
                                         6, 92, 382, 50, y_item_spacing);

                    mod_struct.error_label =
                        attach_new_label(nullptr, container, wrapped_error_info,
                                         11, 48, absolute_y_pos + 224,
                                         227, y_item_spacing);

                    // Set label colors
                    set_label_color(mod_struct.error_label, 1.0, 0.0, 0.0,
                                    '\0');
                    set_label_color(mod_struct.title_label, 0.85, 0.64, 0.12,
                                    '\0');
                    set_label_color(mod_struct.title_label, 0.85, 0.64, 0.12,
                                    '\x01');

                    is_init_finished = true;
                }
            }

            current_parent = parent_mod;
            mod_index++;
        }
    }
}

void __fastcall navigate_callback_left(CMnuSmpButton *button,
                                       int32_t *cui_menu_ptr_maybe)
{
    CMnuContainer *parent =
        (CMnuContainer *) button->CMnuBase_data.param_2_callback;
    uint8_t index = mod_struct.index;

    if((index - 1) < 0)
    {
        mod_struct.index = g_mod_count;
        index = g_mod_count;
    }
    else
    {
        index = index - 1;
    }
    attach_mod_labels(parent, 1, index);
    mod_struct.index = index;
}

void __fastcall navigate_callback_right(CMnuSmpButton *button,
                                        int32_t *cui_menu_ptr_maybe)
{
    CMnuContainer *parent =
        (CMnuContainer *) button->CMnuBase_data.param_2_callback;
    uint8_t index = mod_struct.index;

    index = index + 1;
    if(index > g_mod_count)
    {
        mod_struct.index = 0;
        index = 0;
    }
    attach_mod_labels(parent, 1, index);
    mod_struct.index = index;
}

SFSF_ModlistStruct modinformation;
CMnuContainer *mod_list;
static bool is_mod_list_shown = false;
static bool does_mod_list_exist = false;

void __thiscall show_mod_list(CMnuSmpButton *button)
{
    CMnuContainer *parent =
        (CMnuContainer *) button->CMnuBase_data.param_2_callback;


    if(!does_mod_list_exist)
    {
        is_mod_list_shown = true;
        SF_String s_menu_border, s_menu_background, s_alt_btn_name;
        SF_String *p_menu_border, *p_menu_background, *p_alt_btn_name;

        char alt_name[32] = "HIDE MOD LIST";
        p_alt_btn_name = g_create_sf_string(&s_alt_btn_name, alt_name);

        set_button_name(button, p_alt_btn_name);

        g_destroy_sf_string(p_alt_btn_name);
        mod_list = (CMnuContainer *) g_new_operator(0x340);
        initialize_menu_container(mod_list);

        // Setup mesh loading for background of the container.
        char menu_border[128] = "ui_bgr_options_select_border.msb";
        char menu_background_fade[128] =
            "ui_bgr_options_select_border_transparency.msb";
        p_menu_border = g_create_sf_string(&s_menu_border, menu_border);
        p_menu_background = g_create_sf_string(&s_menu_background,
                                               menu_background_fade);

        setup_menu_container_data(mod_list, 500, 124, 432, 432,
                                  p_menu_background, p_menu_border);

        set_container_alpha(mod_list, 0.99);

        g_destroy_sf_string(p_menu_background);
        g_destroy_sf_string(p_menu_border);

        // Add new container to this container.
        g_container_add_control(parent, (CMnuBase *)mod_list, '\x01', '\x01',
                                0);

        char btn_disabled[128]= "ui_btn_togglearrow_right_disabled.msh";
        char btn_pressed[128]  = "ui_btn_togglearrow_right_pressed.msh";
        char btn_load[1] = "";
        char btn_default[128]  = "ui_btn_togglearrow_right_default.msh";
        char btn_label[1] = "";

        attach_new_button(mod_list, btn_default, btn_pressed, btn_load,
                          btn_disabled, btn_label, 7, (432 - (48 + 32)), 332,
                          48, 48, 0, (uint32_t) &navigate_callback_right);

        char btn_disabled_left[128] = "ui_btn_togglearrow_left_disabled.msh";
        char btn_pressed_left[128] = "ui_btn_togglearrow_left_pressed.msh";
        char btn_default_left[128] = "ui_btn_togglearrow_left_default.msh";

        attach_new_button(mod_list, btn_default_left, btn_pressed_left,
                          btn_load, btn_disabled_left, btn_label, 7, 28, 332,
                          48, 48, 1, (uint32_t) &navigate_callback_left);

        attach_mod_labels(mod_list, 1, 0);
        does_mod_list_exist = true;
    }
    else
    {
        if(is_mod_list_shown)
        {
            log_info("Hide Mod List");
            char alt_name[32] = "SHOW MOD LIST";
            SF_String s_alt_btn_name;
            SF_String *p_alt_btn_name;
            p_alt_btn_name = g_create_sf_string(&s_alt_btn_name, alt_name);

            set_button_name(button, p_alt_btn_name);
            g_destroy_sf_string(p_alt_btn_name);

            set_container_alpha(mod_list, 0.0);
            is_mod_list_shown = false;
        }
        else
        {
            log_info("Show Mod List");
            char alt_name[32] = "HIDE MOD LIST";
            SF_String s_alt_btn_name;
            SF_String *p_alt_btn_name;
            p_alt_btn_name = g_create_sf_string(&s_alt_btn_name, alt_name);

            set_button_name(button, p_alt_btn_name);
            g_destroy_sf_string(p_alt_btn_name);

            set_container_alpha(mod_list, 0.99);
            is_mod_list_shown = true;
        }
    }
}

void __fastcall show_mod_list_callback(CMnuSmpButton *button,
                                       int32_t *cui_menu_ptr_maybe)
{
    CMnuContainer *parent =
        (CMnuContainer *) button->CMnuBase_data.param_2_callback;
    uint8_t toggle = mod_struct.toggle;

    show_mod_list(button);
}


void __thiscall attach_new_button(CMnuContainer *parent,
                                  char *button_mesh_default,
                                  char *button_mesh_pressed,
                                  char *button_initial_load_mesh,
                                  char *button_mesh_disabled, char *label_char,
                                  uint8_t font_index, uint16_t x_pos,
                                  uint16_t y_pos, uint16_t width,
                                  uint16_t height, int button_index,
                                  uint32_t callback_func_ptr)
{
    SF_String m_mesh_string_default;
    SF_String m_mesh_string_pressed;
    SF_String m_button_initial_load_mesh;
    SF_String m_mesh_string_disabled;

    SF_String m_label_string;
    CMnuSmpButton *new_button;
    void *new_btn_operation;

    SF_FontStruct *fonts = g_get_smth_fonts();
    SF_String *label_string = g_create_sf_string(&m_label_string, label_char);

    // Default
    SF_String *mesh_string_default = g_create_sf_string(&m_mesh_string_default,
                                                        button_mesh_default);

    // Pressed
    SF_String *mesh_string_pressed = g_create_sf_string(&m_mesh_string_pressed,
                                                        button_mesh_pressed);

    // Highlight
    SF_String *init_load_mesh = g_create_sf_string(&m_button_initial_load_mesh,
                                                   button_initial_load_mesh);

    // Disabled
    SF_String *mesh_string_disabled =
        g_create_sf_string(&m_mesh_string_disabled, button_mesh_disabled);

    // 0x3b0 seems to corralate to CUiStartMenu, but is directly cast to be a type of CUiFrameStats
    new_button = (CMnuSmpButton *)g_new_operator(0x428); // 0x368, 0x3b0 and 0x708 are all valid. (I suspect that they're creating objects that have CMnuLabel as an Parent Class).

    if (font_index > 32)
    {
        log_error("Invalid font index 0~32, defaulting to font 6");
        font_index = 6;
    }

    new_button = initialize_smp_button(new_button);
    SF_Font *selected_font = g_get_font(fonts, font_index);

    CMnuBase_setname((CMnuBase *)new_button, label_string);

    // This seems to fill out the actual button data itself.
    create_button_func(new_button,x_pos,y_pos,width,height,mesh_string_default,
                       init_load_mesh,mesh_string_pressed,mesh_string_disabled);

    set_font(new_button, selected_font);

    set_button_index(new_button, button_index);

    set_menu_button_flag(new_button, '\x01');

    set_button_name(new_button, label_string);

    CUtlCallback2 callback;
    callback.vtable_ptr = *(uint32_t *)(ASI::AddrOf(0x7F9C64));
    callback.param_ptr = (uint32_t) parent;
    callback.callback_func = callback_func_ptr;

    uint32_t param1, param2, param3;

    attach_callback(&callback, &param1, &param2, &param3);

    new_button->CMnuBase_data.param_1_callback = param1;
    new_button->CMnuBase_data.param_2_callback = param2;
    new_button->CMnuBase_data.param_3_callback = param3;

    vfunction16_attach_callback(new_button, '\x01');

    g_container_add_control(parent,  (CMnuBase *)new_button, '\x01', '\x01', 0);

    g_destroy_sf_string(mesh_string_default);
    g_destroy_sf_string(mesh_string_pressed);
    g_destroy_sf_string(init_load_mesh);
    g_destroy_sf_string(mesh_string_disabled);
    g_destroy_sf_string(label_string);
}

CMnuLabel * __thiscall attach_new_meshed_label(CMnuLabel *new_label,
                                               CMnuContainer *parent,
                                               char *mesh_char,
                                               char *label_char,
                                               uint8_t font_index,
                                               uint16_t x_pos, uint16_t y_pos,
                                               uint16_t width, uint16_t height)
{
    SF_String m_mesh_string;
    SF_String m_label_string;

    SF_FontStruct *fonts = g_get_smth_fonts();
    SF_String *label_string = g_create_sf_string(&m_label_string, label_char);
    SF_String *mesh_string = g_create_sf_string(&m_mesh_string, mesh_char);

    new_label = (CMnuLabel *)g_new_operator(0x3b0);

    if (font_index > 32)
    {
        log_error("Invalid font index 0~32, defaulting to font 6");
        font_index = 6;
    }

    SF_Font *selected_font = g_get_font(fonts, font_index);

    g_menu_label_constructor(new_label);

    g_set_label_flags(new_label, 7);

    g_init_menu_element(new_label, x_pos, y_pos, width, height, mesh_string);

    g_menu_label_set_font(new_label, selected_font);

    g_container_add_control(parent, (CMnuBase *) new_label, '\x01', '\x01', 0);

    g_menu_label_set_string(new_label, label_string);

    g_destroy_sf_string(label_string);
    g_destroy_sf_string(mesh_string);

    return new_label;
}

uint16_t __thiscall sf_get_spell_id(SF_CGdSpell *_this, uint16_t spell_index)
{
    return _this->active_spell_list[spell_index].spell_id;
}

SFMod *createModInfo(const char *mod_id, const char *mod_version,
                     const char *mod_author, const char *mod_description)
{
    SFMod *mod = (SFMod *)malloc(sizeof(SFMod));

    strncpy(mod->mod_id, mod_id, 63);
    mod->mod_id[63] = '\0';

    strncpy(mod->mod_version, mod_version, 23);
    mod->mod_version[23] = '\0';

    strncpy(mod->mod_description, mod_description, 127);
    mod->mod_description[127] = '\0';

    strncpy(mod->mod_author, mod_author, 127);
    mod->mod_author[127] = '\0';

    return mod;
}

// WARNING: Function not fully implemented.
// This function successfully loads a video, but the video container is attached to a layer above the default container.
// As a result, the video container intercepts click events and prevents proper interaction with the underlying elements.
void attachVideo(CAppMenu *CAppMenu_ptr, CMnuContainer *parent,
                 char *video_loc_and_name_chars)
{
    SF_CUiVideo *video_ptr = (SF_CUiVideo *) g_new_operator(0x348);
    SF_String m_video_name_string;
    SF_String *video_name_string = g_create_sf_string(&m_video_name_string,
                                                      video_loc_and_name_chars);
    // TODO Cleanup: Not Sequence, just normal Video, haven't renamed it from previous investigations.
    video_ptr =(SF_CUiVideo *) cuiVideoSequence_constructor(video_ptr,
                                                            video_name_string);

    char controller_mark_chars[64];
    sprintf(controller_mark_chars, "<Cont>CreditsVideoController");
    SF_String m_controller_mark_string;
    SF_String *controller_mark_string =
        g_create_sf_string(&m_controller_mark_string, controller_mark_chars);
    CMnuBase_setname((CMnuBase *)video_ptr, controller_mark_string);

    //void *_CMnuScreen_ptr, CMnuBase* base, char flag
    //CMnuScreen_attach_control(parent, CAppMenu_ptr->CAppMenu_data.CMnuBase_ptr, '\x01');
}

/**
 * @}
 */
