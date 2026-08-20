#ifndef UI_WRAPPERS_H
#define UI_WRAPPERS_H

#if defined(__GNUC__) || defined(__clang__)
#define LOG_PRINTF_FMT(fmt_idx, arg_idx) __attribute__((format(printf, fmt_idx, arg_idx)))
#else
#define LOG_PRINTF_FMT(fmt_idx, arg_idx)
#endif

#include "../../api/sfsf.h"
#include "../../asi/sf_asi.h"

void __thiscall SFStringConstructor(SF_String *_this);
SF_String *__thiscall SFStringConstructor_char(SF_String *_this, const char *char_string);
void __thiscall updateLabelText(CMnuLabel *label, const char *text);
void __thiscall show_mod_list(CMnuSmpButton *button);

/**
 * @brief Forgets the cached mod list screen so the next open rebuilds it.
 *
 * Must be called every time the main menu is constructed - the engine destroys
 * the menu's control tree (and our containers with it) when a game loads.
 */
void reset_mod_list_screen();

/** @brief Wraps text to a character width, inserting newlines. */
void wrap_text(const char *input, char *output, size_t max_width);

/**
 * @brief Repositions a label so its widest line sits centred in @p panel_width,
 * then sets its text. Used by the mod list and custom campaign detail panels.
 */
void set_centred_label_text(CMnuLabel *label, const char *text, int panel_width,
                            int y, int height);

/** @brief Sets a label's normal and highlight colours in one call. */
void apply_label_colour(CMnuLabel *label, float red, float green, float blue);
CMnuLabel *__thiscall attachLabel(CMnuLabel *label_ptr, CMnuContainer *parent,
                                    char *label_chars, uint8_t font_index,
                                    uint16_t x_pos, uint16_t y_pos,
                                    uint16_t width, uint16_t height);

CMnuLabel *__thiscall attachMeshedLabel(CMnuLabel *new_label,
                                        CMnuContainer *parent,
                                        char *mesh_char,
                                        char *label_char,
                                        uint8_t font_index,
                                        uint16_t x_pos, uint16_t y_pos,
                                        uint16_t width, uint16_t height);

CMnuSmpButton *__thiscall attachNewButton(CMnuContainer *parent,
                                        char *button_mesh_default,
                                        char *button_mesh_pressed,
                                        char *button_initial_load_mesh,
                                        char *button_mesh_disabled, char *label_char,
                                        uint8_t font_index, uint16_t x_pos,
                                        uint16_t y_pos, uint16_t width,
                                        uint16_t height, int button_index,
                                        uint32_t callback_func_ptr);

CMnuContainer *__thiscall createContainer(
    uint16_t x,
    uint16_t y,
    uint16_t width,
    uint16_t height,
    const char *background_msb,
    const char *border_msb,
    float alpha
    );

#endif
