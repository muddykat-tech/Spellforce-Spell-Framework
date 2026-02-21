/**
 * @file sf_ui_functions.h
 * @brief Header file for defining User Interface
 *
 * This file provides the necessary data structures and function declarations
 * used to interact with the game's graphical user interface.
 */

#pragma once
#include "./structures/sf_ui_structures.h"

DECLARE_FUNCTION(CMnuContainer *, create_container,
                 uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                 const char *background_msb, const char *border_msb, float alpha)

DECLARE_FUNCTION(CMnuLabel *, attach_new_meshed_label,
                 CMnuLabel *new_label, CMnuContainer *parent,
                 char *mesh_char, char *label_char, uint8_t font_index,
                 uint16_t x_pos, uint16_t y_pos, uint16_t width, uint16_t height)

DECLARE_FUNCTION(CMnuLabel *, attach_new_label,
                 CMnuLabel *label_ptr, CMnuContainer *parent,
                 char *label_chars, uint8_t font_index,
                 uint16_t x_pos, uint16_t y_pos, uint16_t width, uint16_t height)

DECLARE_FUNCTION(void, update_label_text,
                 CMnuLabel *label, const char *text)

DECLARE_FUNCTION(void, destroy_label,
                 CMnuLabel *label)

DECLARE_FUNCTION(CMnuSmpButton *, attach_new_button,
                 CMnuContainer *parent,
                 char *button_mesh_default, char *button_mesh_pressed,
                 char *button_initial_load_mesh, char *button_mesh_disabled,
                 char *label_char, uint8_t font_index,
                 uint16_t x_pos, uint16_t y_pos, uint16_t width, uint16_t height,
                 int button_index, uint32_t callback_func_ptr)

DECLARE_FUNCTION(void, destroy_button,
                 CMnuSmpButton *button)

DECLARE_FUNCTION(void, destroy_container,
                 CMnuContainer *container)

DECLARE_FUNCTION(void, set_element_alpha,
                 void *_this, float alpha)

DECLARE_FUNCTION(uint32_t, get_visual_control,
                 CMnuContainer *_this)

DECLARE_FUNCTION(void, set_font,
                 void *_this, void *input)
/**
 * @ingroup API
 * @brief Group of functions related to UI manipulation.
 */
typedef struct
{
    create_container_ptr create_container;
    attach_new_meshed_label_ptr attach_new_meshed_label;
    attach_new_label_ptr attach_new_label;
    attach_new_button_ptr attach_new_button;
    update_label_text_ptr update_label_text;
    destroy_label_ptr destroy_label;
    destroy_container_ptr destroy_container;
    destroy_button_ptr destroy_button;
    set_element_alpha_ptr set_element_alpha;
    get_visual_control_ptr get_visual_control;
    set_font_ptr set_font;
} UIFunctions;
