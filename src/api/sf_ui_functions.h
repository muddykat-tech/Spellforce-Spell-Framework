/**
 * @file sf_ui_functions.h
 * @brief Header file for defining User Interface
 *
 * This file provides the necessary data structures and function declarations
 * used to interact with the game's graphical user interface.
 */

#pragma once
#include "./structures/sf_ui_structures.h"

DECLARE_FUNCTION(CMnuContainer *, createContainer,
uint16_t x, uint16_t y, uint16_t width, uint16_t height,
const char *background_msb, const char *border_msb, float alpha)

DECLARE_FUNCTION(CMnuLabel *, attachMeshedLabel,
CMnuLabel *new_label, CMnuContainer *parent,
char *mesh_char, char *label_char, uint8_t font_index,
uint16_t x_pos, uint16_t y_pos, uint16_t width, uint16_t height)

DECLARE_FUNCTION(CMnuLabel *, attachLabel,
CMnuLabel *label_ptr, CMnuContainer *parent,
char *label_chars, uint8_t font_index,
uint16_t x_pos, uint16_t y_pos, uint16_t width, uint16_t height)

DECLARE_FUNCTION(void, updateLabelText,
CMnuLabel *label, const char *text)

DECLARE_FUNCTION(void, destroyLabel,
CMnuLabel *label)

DECLARE_FUNCTION(CMnuSmpButton *, attachNewButton,
CMnuContainer *parent,
char *button_mesh_default, char *button_mesh_pressed,
char *button_initial_load_mesh, char *button_mesh_disabled,
char *label_char, uint8_t font_index,
uint16_t x_pos, uint16_t y_pos, uint16_t width, uint16_t height,
int button_index, uint32_t callback_func_ptr)

DECLARE_FUNCTION(void, destroyButton,
CMnuSmpButton *button)

DECLARE_FUNCTION(void, destroyContainer,
CMnuContainer *container)

DECLARE_FUNCTION(void, setElementAlpha,
void *_this, float alpha)

DECLARE_FUNCTION(uint32_t, getVisualControl,
CMnuContainer *_this)

DECLARE_FUNCTION(void, setFont,
void *_this, void *input)

DECLARE_FUNCTION(void, setBaseAlpha, void *_this, float alpha);
DECLARE_FUNCTION(void, initializeMenuContainer, CMnuContainer * _this);
DECLARE_FUNCTION(void, setupMenuContainerData, CMnuContainer *_this, float x, float y,
                                                         float width,
                                                         float height,
                                                         SF_String *background,
                                                         SF_String *border);

DECLARE_FUNCTION(CMnuSmpButton* , initializeSmpButton, CMnuSmpButton *btn);

DECLARE_FUNCTION(void, createButton, CMnuSmpButton *smpButton,
                                             float x_pos, float y_pos,
                                             float width, float height,
                                             SF_String *default_mesh,
                                             SF_String *unkn_mesh_string,
                                             SF_String *pressed_mesh,
                                             SF_String *disable_mesh);

DECLARE_FUNCTION(void, setButtonIndex, void*, int index);

DECLARE_FUNCTION(void, setMenuButtonFlag, void *container, char flag);

DECLARE_FUNCTION(void, attachCallback, void *, void *,void *, void *);

DECLARE_FUNCTION(void, vfunction16AttachCallback, void *_this, char p1);

DECLARE_FUNCTION(void, vfunction176, void *_this, char p1);

DECLARE_FUNCTION(void, vfunction25, void *_this, char p1);

DECLARE_FUNCTION(void, CMnuBaseSetName, CMnuBase *_this, SF_String *name);

DECLARE_FUNCTION(void, setButtonName, void *button, SF_String *string);

DECLARE_FUNCTION(void, setMenuID, void *container, uint32_t id);

DECLARE_FUNCTION(void, setContainerVisible, void *container, uint32_t visible, uint8_t _unknown);
DECLARE_FUNCTION(void, setLabelColour, CMnuLabel *_this, float r, float g, float b, char flag);

/**
 * @ingroup API
 * @brief Group of functions related to UI manipulation.
 */
typedef struct
{
    createContainer_ptr createContainer;
    attachNewButton_ptr attachNewButton;
    destroyLabel_ptr destroyLabel;
    setLabelColour_ptr setLabelColour;
    destroyContainer_ptr destroyContainer;
    destroyButton_ptr destroyButton;
    setElementAlpha_ptr setElementAlpha;
    getVisualControl_ptr getVisualControl;
    setFont_ptr setFont;
    updateLabelText_ptr updateLabelText;
    attachLabel_ptr attachLabel;
    attachMeshedLabel_ptr attachMeshedLabel;
    setBaseAlpha_ptr setBaseAlpha;
    setButtonName_ptr setButtonName;
    initializeMenuContainer_ptr initializeMenuContainer;
    setupMenuContainerData_ptr setupMenuContainerData;
    initializeSmpButton_ptr initializeSmpButton;
    createButton_ptr createButton;
    setButtonIndex_ptr setButtonIndex;
    setMenuButtonFlag_ptr setMenuButtonFlag;
    attachCallback_ptr attachCallback;
    vfunction16AttachCallback_ptr vfunction16AttachCallback;
    vfunction176_ptr vfunction176;
    vfunction25_ptr vfunction25;
    CMnuBaseSetName_ptr CMnuBaseSetName;
    setMenuID_ptr setMenuID;
    setContainerVisible_ptr setContainerVisible;
} UiFunctions;
