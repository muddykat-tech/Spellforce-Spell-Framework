#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sf_wrappers.h"
#include "sf_hooks.h"

#include "hooks/sf_menu_hook.h"
#include "hooks/sf_console_hook.h"

static FUN_0069eaf0_ptr FUN_0069eaf0;
static fidfree_ptr fidFree;

SF_String_ctor_ptr g_create_sf_string;
SF_String_dtor_ptr g_destroy_sf_string;
has_spell_effect_ptr has_spell_effect;

void initialize_wrapper_data_hooks()
{
    FUN_0069eaf0 = (FUN_0069eaf0_ptr)(ASI::AddrOf(0x29EAF0));
    fidFree = (fidfree_ptr)(ASI::AddrOf(0x6B6E25));
    has_spell_effect = (has_spell_effect_ptr)(ASI::AddrOf(0x2fe46f));
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
    snprintf(modifiedMessage, sizeof(modifiedMessage), "[ERROR] %s [Last Error: %d]", message, lastError);
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

void __thiscall setupFigureIterator(CGdFigureIterator *iterator, SF_CGdSpell *spell)
{
    iteratorAPI.figureIteratorInit(iterator, 0x0, 0x0, 0x3ff, 0x3ff);
    iteratorAPI.figureIteratorSetPointers(iterator, spell->SF_CGdFigure, spell->unkn3, spell->SF_CGdWorld);
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
    FUN_0069eaf0(&(iterator->data.offset_0x30), &unused, ((AutoClass69 *)iterator->data.offset_0x30.ac69_ptr1)->ac69_ptr1, iterator->data.offset_0x30.ac69_ptr1);
    fidFree(iterator->data.offset_0x30.ac69_ptr1);
}

void __thiscall addBonusMultToStatistic(SF_CGdFigure *figure, StatisticDataKey key, uint16_t target, int8_t value)
{
    bool invalid = FALSE;
    FigureStatistic *statistic;
    switch (key)
    {
    case ARMOR:
        statistic = &(figure->figures[target].armor);
        break;
    case AGILITY:
        statistic = &(figure->figures[target].agility);
        break;
    case CHARISMA:
        statistic = &(figure->figures[target].charisma);
        break;
    case DEXTERITY:
        statistic = &(figure->figures[target].dexterity);
        break;
    case INTELLIGENCE:
        statistic = &(figure->figures[target].intelligence);
        break;
    case STRENGTH:
        statistic = &(figure->figures[target].strength);
        break;
    case WISDOM:
        statistic = &(figure->figures[target].wisdom);
        break;
    case RESISTANCE_FIRE:
        statistic = &(figure->figures[target].resistance_fire);
        break;
    case RESISTANCE_ICE:
        statistic = &(figure->figures[target].resistance_ice);
        break;
    case RESISTANCE_MENTAL:
        statistic = &(figure->figures[target].resistance_mental);
        break;
    case RESISTANCE_BLACK:
        statistic = &(figure->figures[target].resistance_black);
        break;
    case WALK_SPEED:
        statistic = &(figure->figures[target].walk_speed);
        break;
    case FIGHT_SPEED:
        statistic = &(figure->figures[target].fight_speed);
        break;
    case CAST_SPEED:
        statistic = &(figure->figures[target].cast_speed);
        break;
    default:
        // Handle default case if needed
        invalid = TRUE;
        break;
    }

    if (invalid)
    {
        log_warning("INVALID STATISTIC KEY");
        return;
    }

    figureAPI.addBonusMult(statistic, value);
}

void __thiscall spellClearFigureFlag(SF_CGdSpell *_this, uint16_t spell_id, SpellFlagKey key)
{
    switch (key)
    {
    case CHECK_SPELLS_BEFORE_CHECK_BATTLE:
        spellAPI.figClrChkSplBfrChkBattle(_this, spell_id, 0);
        break;
    case CHECK_SPELLS_BEFORE_JOB2:
        spellAPI.figTryClrCHkSPlBfrJob2(_this, spell_id);
        break;
    case UNFREEZE:
        spellAPI.figTryUnfreeze(_this, spell_id, 0);
        break;
    }
}

bool __thiscall hasAuraActive(SF_CGdFigureToolbox *_this, uint16_t figure_id)
{
    return has_spell_effect(_this, figure_id, 0x49);
}
// Temp
typedef void(__thiscall *vfunction_2_ptr)(void *_this, void *input);
typedef void(__thiscall *vfunction_ptr)(void *label, char *p1);
typedef void(__thiscall *vfunction12_ptr)(void *container, void *test, char *p1);

typedef void(__thiscall *attach_string_ptr)(void *container, void *string);
typedef void(__thiscall *set_button_flag_ptr)(void *container, char flag);

typedef void(__thiscall *set_btn_index_ptr)(void *container, int index);

typedef void(__thiscall *create_button_ptr)(CMnuSmpButton *smpButton, float x_pos, float y_pos, float width,
          float height,SF_String *default_mesh,SF_String *unkn_mesh_string,SF_String *pressed_mesh,SF_String *disable_mesh);


typedef void (__thiscall *set_btn_name_ptr)(void *button, SF_String* string);

typedef CMnuSmpButton*(__thiscall *initialize_smp_button_ptr)(CMnuSmpButton *btn);


typedef void(__thiscall *vfunction2_callback_attach_ptr)(void *, void *,void *,void *);

create_button_ptr create_button_func; 
attach_string_ptr vfunction_apply_string;
vfunction_ptr vfunction176;
vfunction_ptr vfunction25;
vfunction12_ptr vfunction12;
initialize_smp_button_ptr initialize_smp_button;
set_btn_name_ptr set_button_name;


void __thiscall attach_new_label(CMnuContainer *parent, char *label_chars, uint8_t font_index, uint16_t x_pos, uint16_t y_pos, uint16_t width, uint16_t height)
{
    char empty[1];
    sprintf(empty, "");
    attach_new_meshed_label(parent, empty, label_chars, font_index, x_pos, y_pos, width, height);
}

void __fastcall callback_test(CMnuSmpButton *button, int32_t* cui_menu_ptr_maybe)
{
    log_info("Callback Start");
    CMnuContainer *parent = button->CMnuBase_data.param_2_callback;
    char test_label[128];
    sprintf(test_label, "Test Label: %x", parent);
    log_info(test_label);
    attach_new_label(parent, test_label, 6, 10,10, 100, 100);
    log_info("Callback End");
}

void __thiscall attach_new_button(CMnuContainer *parent, char *button_mesh_default, char *button_mesh_pressed, char *button_initial_load_mesh, char *button_mesh_disabled, char *label_char, uint8_t font_index, uint16_t x_pos, uint16_t y_pos, uint16_t width, uint16_t height, int button_index)
{
    SF_String m_mesh_string_default;
    SF_String m_mesh_string_pressed;
    SF_String m_button_initial_load_mesh;
    SF_String m_mesh_string_disabled;

    SF_String m_label_string;
    CMnuSmpButton *new_button;
    void * new_btn_operation;

    SF_FontStruct *fonts = g_get_smth_fonts();
    SF_String *label_string = g_create_sf_string(&m_label_string, label_char);

    // Default
    SF_String *mesh_string_default = g_create_sf_string(&m_mesh_string_default, button_mesh_default);

    // Pressed
    SF_String *mesh_string_pressed = g_create_sf_string(&m_mesh_string_pressed, button_mesh_pressed);

    // Highlight
    SF_String *init_load_mesh = g_create_sf_string(&m_button_initial_load_mesh, button_initial_load_mesh);

    // Disabled
    SF_String *mesh_string_disabled = g_create_sf_string(&m_mesh_string_disabled, button_mesh_disabled);
    
    // 0x3b0 seems to corralate to CUiStartMenu, but is directly cast to be a type of CUiFrameStats
    new_button = (CMnuSmpButton *)g_new_operator(0x428); // 0x368, 0x3b0 and 0x708 are all valid. (I suspect that they're creating objects that have CMnuLabel as an Parent Class).

    if (font_index > 32)
    {
        log_error("Invalid font index 0~32, defaulting to font 6");
        font_index = 6;
    }

    // Put breakpoint here gives pointer back get address of invalid structure spot.
    log_info("init button");
    initialize_smp_button = (initialize_smp_button_ptr) (ASI::AddrOf(0x51a9d0));
    new_button = initialize_smp_button(new_button);
    SF_Font *selected_font = g_get_font(fonts, font_index);

    log_info("Setup Button Meta Name");
    set_btn_name_ptr set_button_name = (set_btn_name_ptr) (ASI::AddrOf(0x512E30));
    set_button_name(new_button, label_string);

    log_info("setup button data");
    // This seems to fill out the actual button data itself.
    create_button_func = (create_button_ptr) (ASI::AddrOf(0x52E1E0));
    create_button_func(new_button,x_pos,y_pos,width,height,mesh_string_default,init_load_mesh,mesh_string_pressed,mesh_string_disabled);


    log_info("set button font");
    vfunction_2_ptr set_font = (vfunction_2_ptr)(ASI::AddrOf(0x530C20)); // Original 0x530c50
    set_font(new_button, selected_font);


    log_info("set button index");
    set_btn_index_ptr set_button_index = (set_btn_index_ptr)(ASI::AddrOf(0x5136a0));
    set_button_index(new_button, button_index);

    log_info("set button flag");
    set_button_flag_ptr set_menu_button_flag = (set_button_flag_ptr)(ASI::AddrOf(0x5308A0));
    set_menu_button_flag(new_button, (char)0x1);
    
    log_info("set button name again");
    set_button_name = (set_btn_name_ptr) (ASI::AddrOf(0x52f8a0));
    set_button_name(new_button, label_string);

    log_info("Setup Button Callback p1");
    CUtlCallback2 callback;
    callback.vtable_ptr = *(uint32_t *)(ASI::AddrOf(0x7F9C64));
    callback.param_ptr = parent;
    callback.callback_ptr = &callback_test;

    
    log_info("Setup Button Callback p2");
    vfunction2_callback_attach_ptr attach_callback = (vfunction2_callback_attach_ptr)(ASI::AddrOf(0x6188B0));
    uint32_t param1, param2, param3;

    attach_callback(&callback, &param1, &param2, &param3);

    new_button->CMnuBase_data.param_1_callback = param1;
    new_button->CMnuBase_data.param_2_callback = param2;
    new_button->CMnuBase_data.param_3_callback = param3;

    
    log_info("Setup Button Callback p3");
    vfunction_2_ptr vfunction16_attach_callback = (vfunction_2_ptr) (ASI::AddrOf(0x532B90));
    vfunction16_attach_callback(new_button, '\x01');

    int32_t pointer_to_callback = &callback;
    int32_t pointer_to_container = parent;
    char info[256];
    sprintf(info, "Callback Struct Pointer: %x | container_ptr: %x", pointer_to_callback, pointer_to_container); 
    log_info(info);

    log_info("add to container");
    g_container_add_control(parent, new_button, (char *)0x01, (char *)0x01, 0);


    log_info("cleanup strings");
    g_destroy_sf_string(mesh_string_default);
    g_destroy_sf_string(mesh_string_pressed);
    g_destroy_sf_string(init_load_mesh);
    g_destroy_sf_string(mesh_string_disabled);
    g_destroy_sf_string(label_string);
    
    log_info("finished");
}

void __thiscall attach_new_meshed_label(CMnuContainer *parent, char *mesh_char, char *label_char, uint8_t font_index, uint16_t x_pos, uint16_t y_pos, uint16_t width, uint16_t height)
{
    SF_String m_mesh_string;
    SF_String m_label_string;
    CMnuLabel *new_label;

    SF_FontStruct *fonts = g_get_smth_fonts();
    SF_String *label_string = g_create_sf_string(&m_label_string, label_char);
    SF_String *mesh_string = g_create_sf_string(&m_mesh_string, mesh_char);
    // 0x3b0 seems to corralate to CUiStartMenu, but is directly cast to be a type of CUiFrameStats
    new_label = (CMnuLabel *)g_new_operator(0x3b0); // 0x368, 0x3b0 and 0x708 are all valid. (I suspect that they're creating objects that have CMnuLabel as an Parent Class).

    if (font_index > 32)
    {
        log_error("Invalid font index 0~32, defaulting to font 6");
        font_index = 6;
    }

    SF_Font *selected_font = g_get_font(fonts, font_index);

    g_menu_label_constructor(new_label);

    // Start setting flags to tell Spellforce what this label is used for.
    g_set_label_flags(new_label, 7);

    g_init_menu_element(new_label, x_pos, y_pos, width, height, mesh_string);

    vfunction176 = (vfunction_ptr)(ASI::AddrOf(0x52f520));
    vfunction176(new_label, (char *)0x1);

    vfunction25 = (vfunction_ptr)(ASI::AddrOf(0x511ae0));
    vfunction25(new_label, (char *)0x0);

    g_menu_label_set_font(new_label, selected_font);

    g_container_add_control(parent, new_label, (char *)0x01, (char *)0x01, 2);

    vfunction12 = (vfunction12_ptr)(ASI::AddrOf(0x511ae0));
    vfunction12(parent, new_label, (char *)0x0);
    g_menu_label_set_string(new_label, label_string);

    g_destroy_sf_string(label_string);
    g_destroy_sf_string(mesh_string);
}

uint16_t __thiscall sf_get_spell_id(SF_CGdSpell *_this, uint16_t spell_index)
{
    return _this->active_spell_list[spell_index].spell_id;
}

/*
    char mod_id[64];
    char mod_version[24];
    char mod_description[128];
    char mod_author[128];
*/
SFMod *createModInfo(const char *mod_id, const char *mod_version, const char *mod_author, const char *mod_description)
{
    // Sanitize the mod info, no buffer overflows for us!
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