#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sf_ui_wrappers.h"
#include "sf_wrappers.h"
#include "sf_hooks.h"

#include "hooks/sf_menu_hook.h"
#include "hooks/sf_console_hook.h"
#include "hooks/sf_vanilla_fix_hook.h"
#include "../registry/sf_mod_registry.h"
#include "../registry/sf_error_registry.h"

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
            while (break_point > 0 && !isspace((unsigned char)output[break_point - 1]))
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


/**
 * @brief Zero-initializes an SF_String (no allocation).
 * Moved here from sf_vanilla_fix_hook.c; exposed as uiAPI.SFStringConstructor.
 */
void __thiscall SFStringConstructor(SF_String *_this)
{
    _this->raw_data = nullptr;
    _this->char_data = nullptr;
    _this->str_length = 0x0;
    _this->str_length_char = 0x0;
}

/**
 * @brief Constructs an SF_String from a null-terminated char string.
 * Moved here from sf_vanilla_fix_hook.c; exposed as uiAPI.SFStringConstructor_char.
 * @note Relies on uiAPI.SFStringSetLength, which is registered at the very top
 * of initialize_data_hooks() before any code path that can call this.
 */
SF_String *__thiscall SFStringConstructor_char(SF_String *_this, const char *char_string)
{
    SFStringConstructor(_this);
    uint32_t length = 0;
    if (char_string != 0)
    {
        //not sure here, might need to use internal implementation?
        length = strlen(char_string);
    }
    uiAPI.SFStringSetLength(_this, length);
    if (length != 0)
    {
        mbstowcs(_this->raw_data, char_string, length);
    }
    _this->str_length = length;
    return _this;
}

/**
 * Helper function to update label text using SF_String
 *
 * @param label The label to update
 * @param text New text to set
 */
void __thiscall updateLabelText(CMnuLabel *label, const char *text)
{
    if (!label || !text)
    {
        return;
    }

    SF_String string_obj;
    SF_String *sf_string = g_create_sf_string(&string_obj, text);
    uiAPI.menuLabelSetString(label, sf_string);
    g_destroy_sf_string(sf_string);
}


CMnuContainer *mod_list;
CMnuContainer *mod_container;
CMnuContainer *mod_info_page;

CMnuLabel *mod_list_title;
CMnuSmpButton *left_nav;
CMnuSmpButton *right_nav;
static bool is_mod_list_shown = false;
static bool does_mod_list_exist = false;

/**
 * @brief Number of mod rows the left panel can show at once.
 */
#define MOD_LIST_ROWS 11

#define MOD_ROW_X       108
#define MOD_ROW_Y_START 40
#define MOD_ROW_PITCH   44
#define MOD_ROW_WIDTH   227
#define MOD_ROW_HEIGHT  36
#define MOD_NAV_Y       540

#define MOD_INFO_PANEL_WIDTH 443
#define MOD_INFO_TITLE_Y     34
#define MOD_INFO_TITLE_H     34
#define MOD_INFO_AUTHOR_Y    62
#define MOD_INFO_AUTHOR_H    28
#define MOD_INFO_TYPE_Y      98
#define MOD_INFO_TYPE_H      28
#define MOD_INFO_DESC_Y      136
#define MOD_INFO_DESC_H      248
#define MOD_INFO_ERROR_Y     396
#define MOD_INFO_ERROR_H     200

/**
 * @brief Approximate pixel width of one glyph in the info panel fonts.
 *
 * Used to centre a text block by measurement - label flags are
 * undocumented. So labels are simply positioned so the widest line sits
 * centred in the panel. this is all just an approximation.
 */
#define MOD_GLYPH_WIDTH 8

/** Info panel label colours, as red, green, blue argument triples. */
#define MOD_TITLE_RGB  0.85f, 0.64f, 0.12f
#define MOD_AUTHOR_RGB 0.75f, 0.75f, 0.75f
#define MOD_TYPE_RGB  0.45f, 0.72f, 0.95f
#define MOD_ERROR_RGB 1.0f, 0.0f, 0.0f

static CMnuSmpButton *s_mod_buttons[MOD_LIST_ROWS];

/** Absolute index into get_registered_mods() per row, or -1 for an unused row. */
static int s_mod_row_target[MOD_LIST_ROWS];


/**
 * @brief Drops every cached mod list pointer, this will ensure we don't crash after a map load.
 */
void reset_mod_list_screen()
{
    mod_info_page = nullptr;
    mod_container = nullptr;
    mod_list = nullptr;
    mod_list_title = nullptr;
    left_nav = nullptr;
    right_nav = nullptr;

    mod_struct.title_label = nullptr;
    mod_struct.author_label = nullptr;
    mod_struct.desc_label = nullptr;
    mod_struct.page_label = nullptr;
    mod_struct.error_label = nullptr;
    mod_struct.type_label = nullptr;
    mod_struct.toggle = 0;
    mod_struct.index = 0;

    for (int row = 0; row < MOD_LIST_ROWS; row++)
    {
        s_mod_buttons[row] = nullptr;
        s_mod_row_target[row] = -1;
    }
    is_mod_list_shown = false;
    does_mod_list_exist = false;
}


int calculate_total_unique_mods()
{
    return (int)get_registered_mods().size();
}

int calculate_total_pages(int total_unique_mods, int mods_per_page)
{
    int total_pages = (total_unique_mods + mods_per_page - 1) / mods_per_page;
    return (total_pages == 0) ? 1 : total_pages;
}

int normalize_page_index(int page, int total_pages)
{
    return (page + total_pages) % total_pages;
}

void prepare_mod_title(SFMod *parent_mod, char *mod_title, size_t buffer_size)
{
    snprintf(mod_title, buffer_size, "%s - %s",
             parent_mod->mod_id,
             parent_mod->mod_version);
}

void prepare_mod_author(SFMod *parent_mod, char *mod_author, size_t buffer_size)
{
    snprintf(mod_author, buffer_size, "by %s",
             (parent_mod->mod_author[0] != '\0') ? parent_mod->mod_author : "Unknown");
}

#define MOD_INFO_WRAP 50

void prepare_mod_description(SFMod *parent_mod, char *mod_description, size_t description_buffer_size,
                             char *wrapped_description, size_t wrapped_buffer_size)
{
    snprintf(mod_description, description_buffer_size, "%s",
             parent_mod->mod_description);

    wrap_text(mod_description, wrapped_description, MOD_INFO_WRAP);
}

void prepare_mod_page_info(int page, int total_pages, char *mod_page_info, size_t buffer_size)
{
    snprintf(mod_page_info, buffer_size, "(%d / %d)",
             (page + 1), total_pages);
}

void prepare_mod_error_info(SFMod *parent_mod, char *mod_error_info, size_t error_buffer_size,
                            char *wrapped_error_info, size_t wrapped_buffer_size)
{
    if (parent_mod->mod_errors[0] != 0)
    {
        snprintf(mod_error_info, error_buffer_size, "%s",
                 parent_mod->mod_errors);
        wrap_text(mod_error_info, wrapped_error_info, MOD_INFO_WRAP);
    }
    else
    {
        snprintf(mod_error_info, error_buffer_size, " ");
        snprintf(wrapped_error_info, wrapped_buffer_size, " ");
    }
}


/** @brief Returns the registered mod at an absolute index, or nullptr. */
static const RegisteredMod *get_mod_at(int index)
{
    if (index < 0)
    {
        return nullptr;
    }

    int current = 0;
    for (const RegisteredMod &entry : get_registered_mods())
    {
        if (current == index)
        {
            return &entry;
        }
        current++;
    }
    return nullptr;
}

/**
 * @brief Repositions a label so its text block sits centred, then sets the text.
 */
void set_centred_label_text(CMnuLabel *label, const char *text, int panel_width,
                            int y, int height)
{
    if (label == nullptr || text == nullptr)
    {
        return;
    }

    g_set_label_flags(label, 0xa); //just centers position
    uiAPI.updateLabelText(label, text);
}

/** @brief Sets both colour slots of a label. (normal and on hover)*/
void apply_label_colour(CMnuLabel *label, float red, float green, float blue)
{
    if (label == nullptr)
    {
        return;
    }

    uiAPI.setLabelColour(label, red, green, blue, '\0'); //normal
    uiAPI.setLabelColour(label, red, green, blue, '\x01'); //hover
}

/** @brief Relabels an existing button */
static void set_button_label(CMnuSmpButton *button, const char *text)
{
    if (button == nullptr)
    {
        return;
    }

    SF_String label;
    SF_String *label_string = g_create_sf_string(&label, text);
    uiAPI.CMnuBaseSetName((CMnuBase *)button, label_string);
    uiAPI.setButtonName(button, label_string);
    g_destroy_sf_string(label_string);
}

/**
 * @brief Renders one mod into the right hand info panel.
 *
 * The labels are created once per menu session by build_mod_info_panel();
 */
void show_mod_details(int mod_index)
{
    const RegisteredMod *entry = get_mod_at(mod_index);
    if (entry == nullptr)
    {
        return;
    }

    SFMod *mod = entry->mod;

    char mod_title[512] = {0};
    prepare_mod_title(mod, mod_title, sizeof(mod_title));

    char mod_author[256] = {0};
    prepare_mod_author(mod, mod_author, sizeof(mod_author));

    char mod_description[512] = {0};
    char wrapped_description[1024] = {0};
    prepare_mod_description(mod, mod_description, sizeof(mod_description),
                            wrapped_description, sizeof(wrapped_description));

    char mod_error_info[512] = {0};
    char wrapped_error_info[1024] = {0};
    prepare_mod_error_info(mod, mod_error_info, sizeof(mod_error_info),
                           wrapped_error_info, sizeof(wrapped_error_info));

    set_centred_label_text(mod_struct.title_label, mod_title, MOD_INFO_PANEL_WIDTH, MOD_INFO_TITLE_Y, MOD_INFO_TITLE_H);
    set_centred_label_text(mod_struct.author_label, mod_author,
                           MOD_INFO_PANEL_WIDTH, MOD_INFO_AUTHOR_Y, MOD_INFO_AUTHOR_H);
    set_centred_label_text(mod_struct.type_label, get_mod_type_label(entry->type),
                           MOD_INFO_PANEL_WIDTH, MOD_INFO_TYPE_Y, MOD_INFO_TYPE_H);
    set_centred_label_text(mod_struct.desc_label, wrapped_description,
                           MOD_INFO_PANEL_WIDTH, MOD_INFO_DESC_Y, MOD_INFO_DESC_H);
    set_centred_label_text(mod_struct.error_label, wrapped_error_info,
                           MOD_INFO_PANEL_WIDTH, MOD_INFO_ERROR_Y, MOD_INFO_ERROR_H);
}

/**
 * @brief Points the visible rows at the mods on the given page.
 */
void refresh_mod_list_page(int page)
{
    int total_mods = calculate_total_unique_mods();
    int total_pages = calculate_total_pages(total_mods, MOD_LIST_ROWS);

    page = normalize_page_index(page, total_pages);
    mod_struct.index = page;

    const int first_mod = page * MOD_LIST_ROWS;

    for (int row = 0; row < MOD_LIST_ROWS; row++)
    {
        int mod_index = first_mod + row;
        const RegisteredMod *entry = (mod_index < total_mods) ? get_mod_at(mod_index) : nullptr;

        if (entry != nullptr)
        {
            s_mod_row_target[row] = mod_index;
            set_button_label(s_mod_buttons[row], entry->mod->mod_id);
        }
        else
        {
            s_mod_row_target[row] = -1;
            set_button_label(s_mod_buttons[row], "");
        }
    }

    char mod_page_info[48] = {0};
    prepare_mod_page_info(page, total_pages, mod_page_info, sizeof(mod_page_info));
    if (mod_struct.page_label != nullptr)
    {
        uiAPI.updateLabelText(mod_struct.page_label, mod_page_info);
    }
}

/** @brief Left panel row callback */
void __thiscall on_mod_selected(CMnuSmpButton *button)
{
    if (!does_mod_list_exist)
    {
        return;
    }

    for (int row = 0; row < MOD_LIST_ROWS; row++)
    {
        if (s_mod_buttons[row] == button)
        {
            if (s_mod_row_target[row] >= 0)
            {
                show_mod_details(s_mod_row_target[row]);
            }
            return;
        }
    }
}

/**
 * @brief moves the list by a page via delta value.
 */
static void navigate_page(int delta)
{
    if (!does_mod_list_exist)
    {
        return;
    }

    refresh_mod_list_page(mod_struct.index + delta);
}

static void navigate_page_left(CMnuSmpButton *button)
{
    navigate_page(-1);
}

static void navigate_page_right(CMnuSmpButton *button)
{
    navigate_page(1);
}

/** @brief Creates the clickable mod rows, the paging arrows and the counter. */
void build_mod_list_panel(CMnuContainer *list_panel)
{
    char row_default[128]  = "ui_mainmenu_button_default.msh";
    char row_pressed[128]  = "ui_mainmenu_button_pressed.msh";
    char row_disabled[128] = "ui_mainmenu_button_disabled.msh";
    char row_load[1] = "";
    char row_label[1] = "";

    for (int row = 0; row < MOD_LIST_ROWS; row++)
    {
        s_mod_row_target[row] = -1;
        s_mod_buttons[row] = uiAPI.attachNewButton(
            list_panel, row_default, row_pressed, row_load, row_disabled,
            row_label, 7,
            MOD_ROW_X, MOD_ROW_Y_START + (MOD_ROW_PITCH * row),
            MOD_ROW_WIDTH, MOD_ROW_HEIGHT,
            32 + row,
            (uint32_t) &on_mod_selected);
    }

    char btn_disabled[128] = "ui_btn_togglearrow_right_disabled.msh";
    char btn_pressed[128]  = "ui_btn_togglearrow_right_pressed.msh";
    char btn_load[1] = "";
    char btn_default[128]  = "ui_btn_togglearrow_right_default.msh";
    char btn_label[1] = "";

    right_nav = uiAPI.attachNewButton(list_panel, btn_default, btn_pressed, btn_load,
                                      btn_disabled, btn_label, 7, (443 - 96), MOD_NAV_Y,
                                      48, 48, 0, (uint32_t) &navigate_page_right);

    char btn_disabled_left[128] = "ui_btn_togglearrow_left_disabled.msh";
    char btn_pressed_left[128] = "ui_btn_togglearrow_left_pressed.msh";
    char btn_default_left[128] = "ui_btn_togglearrow_left_default.msh";

    left_nav = uiAPI.attachNewButton(list_panel, btn_default_left, btn_pressed_left,
                                     btn_load, btn_disabled_left, btn_label, 7, 48, MOD_NAV_Y,
                                     48, 48, 1, (uint32_t) &navigate_page_left);

    char page_placeholder[8] = "";
    mod_struct.page_label = uiAPI.attachLabel(nullptr, list_panel, page_placeholder,
                                              6, 190, MOD_NAV_Y + 14, 80, 32);
    uiAPI.setMenuID(mod_struct.page_label, 0x6);
}

/**
 * @brief Creates the right hand panel's labels.
 */
void build_mod_info_panel(CMnuContainer *info_panel)
{
    char placeholder[2] = " ";

    mod_struct.title_label = uiAPI.attachLabel(nullptr, info_panel, placeholder,
                                               4, 0, MOD_INFO_TITLE_Y,
                                               MOD_INFO_PANEL_WIDTH, MOD_INFO_TITLE_H);
    uiAPI.setMenuID(mod_struct.title_label, 0x6);
    g_set_label_flags(mod_struct.title_label, 0xa);
    apply_label_colour(mod_struct.title_label, MOD_TITLE_RGB);

    mod_struct.author_label = uiAPI.attachLabel(nullptr, info_panel, placeholder,
                                                8, 0, MOD_INFO_AUTHOR_Y,
                                                MOD_INFO_PANEL_WIDTH, MOD_INFO_AUTHOR_H);
    uiAPI.setMenuID(mod_struct.author_label, 0x6);
    apply_label_colour(mod_struct.author_label, MOD_AUTHOR_RGB);

    mod_struct.type_label = uiAPI.attachLabel(nullptr, info_panel, placeholder,
                                              10, 0, MOD_INFO_TYPE_Y,
                                              MOD_INFO_PANEL_WIDTH, MOD_INFO_TYPE_H);
    uiAPI.setMenuID(mod_struct.type_label, 0x6);
    apply_label_colour(mod_struct.type_label, MOD_TYPE_RGB);

    mod_struct.desc_label = uiAPI.attachLabel(nullptr, info_panel, placeholder,
                                              11, 0, MOD_INFO_DESC_Y,
                                              MOD_INFO_PANEL_WIDTH, MOD_INFO_DESC_H);

    uiAPI.setMenuID(mod_struct.desc_label, 0x6);

    mod_struct.error_label = uiAPI.attachLabel(nullptr, info_panel, placeholder,
                                               11, 0, MOD_INFO_ERROR_Y,
                                               MOD_INFO_PANEL_WIDTH, MOD_INFO_ERROR_H);
    uiAPI.setMenuID(mod_struct.error_label, 0x6);
    apply_label_colour(mod_struct.error_label, MOD_ERROR_RGB);
}

void __fastcall close_mod_list_callback(CMnuSmpButton *button, int32_t *cui_menu_ptr_maybe)
{
    CMnuContainer *parent_container =
        (CMnuContainer *) button->CMnuBase_data.param_2_callback;

    if (parent_container != nullptr)
    {
        uiAPI.setContainerVisible(parent_container, false, false);
    }
    is_mod_list_shown = false;
}

CMnuContainer *__thiscall createContainer(
    uint16_t x,
    uint16_t y,
    uint16_t width,
    uint16_t height,
    const char *background_msb,
    const char *border_msb,
    float alpha
    )
{
    CMnuContainer *container;
    SF_String s_bg, s_border;
    SF_String *p_bg, *p_border;

    container = (CMnuContainer *)uiAPI.newOperator(0x340);
    if (!container)
    {
        log_error("Failed to allocate CMnuContainer");
        return NULL;
    }

    uiAPI.initializeMenuContainer(container);

    p_bg = g_create_sf_string(&s_bg, (char *)background_msb);
    p_border = g_create_sf_string(&s_border, (char *)border_msb);

    uiAPI.setupMenuContainerData(
        container,
        x,
        y,
        width,
        height,
        p_bg,
        p_border
        );

    g_destroy_sf_string(p_bg);
    g_destroy_sf_string(p_border);
    void *CMnuVisControl = (void *) uiAPI.getVisualControl(container);
    uiAPI.setBaseAlpha(CMnuVisControl, alpha);

    return container;
}

void add_close_button(CMnuContainer *mod_list)
{
    char close_btn_default[128] = "ui_btn_nav_back_default.msh";
    char close_btn_pressed[128] = "ui_btn_nav_back_pressed.msh";
    char close_btn_disabled[128] = "ui_btn_nav_back_disabled.msh";
    char close_btn_load[1] = "";
    char close_btn_label[1] = "";

    uiAPI.attachNewButton(
        mod_list,
        close_btn_default,
        close_btn_pressed,
        close_btn_load,
        close_btn_disabled,
        close_btn_label,
        7,
        28,
        700,
        48,
        48,
        2,
        (uint32_t) &close_mod_list_callback
        );
}

void __thiscall show_mod_list(CMnuSmpButton *button)
{
    CMnuContainer *parent = (CMnuContainer *) button->CMnuBase_data.param_2_callback;
    if(!does_mod_list_exist)
    {
        is_mod_list_shown = true;
        mod_info_page = uiAPI.createContainer(
            0, 0, 1024, 768,
            "ui_bgr_landscape_bg.msb",
            "", 0.99f
            );

        mod_container = uiAPI.createContainer(
            11,6,1008,757,
            "ui_bgr_pregame_border_transparency.msb",
            "ui_bgr_pregame_border.msb", 0.5f
            );

        mod_list = uiAPI.createContainer(
            59, 50, 443, 619,
            "ui_bgr_pregame_border_right_transparency.msb",
            "ui_bgr_pregame_border_right.msb", 0.5f
            );

        CMnuContainer *mod_list_info = uiAPI.createContainer(
            502, 50, 443, 619,
            "ui_bgr_pregame_border_right_transparency.msb",
            "ui_bgr_pregame_border_right.msb", 0.5f
            );

        if (!mod_info_page || !mod_container || !mod_list || !mod_list_info)
        {
            log_error("Unable to create Mod Menu, a container allocation failed");

            if (mod_info_page)
            {
                uiAPI.destroyContainer(mod_info_page);
            }
            if (mod_container)
            {
                uiAPI.destroyContainer(mod_container);
            }
            if (mod_list)
            {
                uiAPI.destroyContainer(mod_list);
            }
            if (mod_list_info)
            {
                uiAPI.destroyContainer(mod_list_info);
            }

            reset_mod_list_screen();
            return;
        }

        uiAPI.containerAddControl(parent, (CMnuBase *)mod_info_page, '\x01', '\x01', 0);

        uiAPI.containerAddControl(mod_info_page, (CMnuBase *)mod_container, '\x01', '\x01', 0);

        uiAPI.containerAddControl(mod_container, (CMnuBase *)mod_list, '\x01', '\x01', 0);
        uiAPI.containerAddControl(mod_container, (CMnuBase *)mod_list_info, '\x01', '\x01', 0);

        add_close_button(mod_info_page);

        build_mod_list_panel(mod_list);
        build_mod_info_panel(mod_list_info);
        refresh_mod_list_page(0);
        show_mod_details(0);

        char sfsf_mod_info[32] = "Mod Information";
        mod_list_title = uiAPI.attachLabel(nullptr, mod_container, sfsf_mod_info, 6, 468, 16, 128, 16);

        uiAPI.setMenuID(mod_list_title, 0x6);
        uiAPI.setLabelColour(mod_list_title, 0.85, 0.64, 0.12, '\0');
        uiAPI.setLabelColour(mod_list_title, 0.85, 0.64, 0.12, '\x01');

        does_mod_list_exist = true;
    }
    else if (mod_info_page != nullptr)
    {
        // Same menu session, so the screen *should* still be attached, so we can toggle visibility.
        is_mod_list_shown = !is_mod_list_shown;
        uiAPI.setContainerVisible(mod_info_page, is_mod_list_shown, 0);
    }
}

CMnuSmpButton *__thiscall attachNewButton(CMnuContainer *parent,
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
    //void *new_btn_operation;

    SF_FontStruct *fonts = uiAPI.getFonts();
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
    // 0x368, 0x3b0 and 0x708 are all valid. (I suspect that they're creating
    // objects that have CMnuLabel as a Parent Class).
    new_button = (CMnuSmpButton *)uiAPI.newOperator(0x428);

    if (font_index > 32)
    {
        log_error("Invalid font index 0~32, defaulting to font 6");
        font_index = 6;
    }

    new_button = uiAPI.initializeSmpButton(new_button);
    SF_Font *selected_font = uiAPI.getFont(fonts, font_index);

    uiAPI.CMnuBaseSetName((CMnuBase *)new_button, label_string);

    uiAPI.createButton(new_button, x_pos, y_pos, width, height, mesh_string_default,
                       init_load_mesh, mesh_string_pressed, mesh_string_disabled);

    uiAPI.setFont(new_button, selected_font);

    uiAPI.setButtonIndex(new_button, button_index);

    uiAPI.setMenuButtonFlag(new_button, '\x01');

    uiAPI.setButtonName(new_button, label_string);

    CUtlCallback2 callback;
    callback.vtable_ptr = *(uint32_t *)(ASI::AddrOf(0x7F9C64));
    callback.param_ptr = (uint32_t) parent;
    callback.callback_func = callback_func_ptr;

    uint32_t param1, param2, param3;

    uiAPI.attachCallback(&callback, &param1, &param2, &param3);

    new_button->CMnuBase_data.param_1_callback = param1;
    new_button->CMnuBase_data.param_2_callback = param2;
    new_button->CMnuBase_data.param_3_callback = param3;

    uiAPI.vfunction16AttachCallback(new_button, '\x01');

    uiAPI.containerAddControl(parent,  (CMnuBase *)new_button, '\x01', '\x01', 0);

    g_destroy_sf_string(mesh_string_default);
    g_destroy_sf_string(mesh_string_pressed);
    g_destroy_sf_string(init_load_mesh);
    g_destroy_sf_string(mesh_string_disabled);
    g_destroy_sf_string(label_string);

    return new_button;
}

SFMod *createModInfo(const char *mod_id, const char *mod_version,
                     const char *mod_author, const char *mod_description)
{
    SFMod *mod = (SFMod *)malloc(sizeof(SFMod));

    if (mod == nullptr)
    {
        log_error("Unable to allocate SFMod for [%s]", (mod_id != nullptr) ? mod_id : "<unnamed>");
        return nullptr;
    }

    clear_mod_errors(mod);

    strncpy(mod->mod_id, mod_id, sizeof(mod->mod_id) - 1);
    mod->mod_id[sizeof(mod->mod_id) - 1] = '\0';

    strncpy(mod->mod_version, mod_version, sizeof(mod->mod_version) - 1);
    mod->mod_version[sizeof(mod->mod_version) - 1] = '\0';

    strncpy(mod->mod_description, mod_description, sizeof(mod->mod_description) - 1);
    mod->mod_description[sizeof(mod->mod_description) - 1] = '\0';

    strncpy(mod->mod_author, mod_author, sizeof(mod->mod_author) - 1);
    mod->mod_author[sizeof(mod->mod_author) - 1] = '\0';

    return mod;
}

/*
   void attachVideo(CAppMenu *CAppMenu_ptr, CMnuContainer *parent,
                 char *video_loc_and_name_chars)
   {
    SF_CUiVideo *video_ptr = (SF_CUiVideo *) uiAPI.newOperator(0x348);
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
 */

CMnuLabel *__thiscall attachMeshedLabel(CMnuLabel *new_label,
                                         CMnuContainer *parent,
                                         char *mesh_char,
                                         char *label_char,
                                         uint8_t font_index,
                                         uint16_t x_pos, uint16_t y_pos,
                                         uint16_t width, uint16_t height)
{
    SF_String m_mesh_string;
    SF_String m_label_string;

    SF_FontStruct *fonts = uiAPI.getFonts();
    SF_String *label_string = g_create_sf_string(&m_label_string, label_char);
    SF_String *mesh_string = g_create_sf_string(&m_mesh_string, mesh_char);

    new_label = (CMnuLabel *)uiAPI.newOperator(0x3b0);

    if (font_index > 32)
    {
        log_error("Invalid font index 0~32, defaulting to font 6");
        font_index = 6;
    }

    SF_Font *selected_font = uiAPI.getFont(fonts, font_index);

    uiAPI.menuLabelConstructor(new_label);

    g_set_label_flags(new_label, 7);

    uiAPI.initMenuElement(new_label, x_pos, y_pos, width, height, mesh_string);

    uiAPI.menuLabelSetFont(new_label, selected_font);

    uiAPI.containerAddControl(parent, (CMnuBase *) new_label, '\x01', '\x01', 0);

    uiAPI.menuLabelSetString(new_label, label_string);

    g_destroy_sf_string(label_string);
    g_destroy_sf_string(mesh_string);

    return new_label;
}

CMnuLabel *__thiscall attachLabel(CMnuLabel *label_ptr,
                                   CMnuContainer *parent,
                                   char *label_chars, uint8_t font_index,
                                   uint16_t x_pos, uint16_t y_pos,
                                   uint16_t width, uint16_t height)
{
    char empty[] = "";
    return attachMeshedLabel(label_ptr, parent, empty, label_chars,
                             font_index, x_pos, y_pos, width, height);
}
