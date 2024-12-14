#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sf_wrappers.h"
#include "sf_hooks.h"

#include "hooks/sf_menu_hook.h"
#include "hooks/sf_console_hook.h"
#include "../registry/sf_mod_registry.h"

static FUN_0069eaf0_ptr FUN_0069eaf0;
static fidfree_ptr fidFree;

SF_String_ctor_ptr g_create_sf_string;
SF_String_dtor_ptr g_destroy_sf_string;


void initialize_wrapper_data_hooks()
{
    FUN_0069eaf0 = (FUN_0069eaf0_ptr)(ASI::AddrOf(0x29EAF0));
    fidFree = (fidfree_ptr)(ASI::AddrOf(0x6B6E25));
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

typedef void(__thiscall *initialize_menu_container_ptr)(CMnuContainer *_this);

typedef void(__thiscall *setup_menu_container_data_ptr)(CMnuContainer *_this, float x, float y, float width, float height, SF_String * background, SF_String * border);

typedef void(__thiscall *container_alpha_ptr)(CMnuContainer *_this, float alpha);

create_button_ptr create_button_func; 
attach_string_ptr vfunction_apply_string;
vfunction_ptr vfunction176;
vfunction_ptr vfunction25;
vfunction12_ptr vfunction12;
initialize_smp_button_ptr initialize_smp_button;
set_btn_name_ptr set_button_name;
initialize_menu_container_ptr initialize_menu_container;



CMnuLabel * __thiscall attach_new_label(CMnuLabel *label_ptr, CMnuContainer *parent, char *label_chars, uint8_t font_index, uint16_t x_pos, uint16_t y_pos, uint16_t width, uint16_t height)
{
    char empty[1];
    sprintf(empty, "");
    return attach_new_meshed_label(label_ptr, parent, empty, label_chars, font_index, x_pos, y_pos, width, height);
}
static bool is_init_finished = false;
void attach_mod_labels(CMnuContainer *_container, int mods_per_page, int page)
{
    
    log_info("Mod Label Callback");
    SFMod *old_parent = NULL;  // Initialize to NULL
    int index = 0;
    int use_index = (mods_per_page * page);  // Start index for current page
    int max_index = use_index + mods_per_page; // Max index to process for the current page
    char struct_info[64];
    sprintf(struct_info, "Mod Struct: %x", &mod_struct);
    log_info(struct_info);
    // Calculate the Y offset based on the page number (page * height of one label, which is 24)
    int y_offset = 48 + (page * mods_per_page * 24);  // Start at 48 for the first label

    // Loop through the list of spells (or mods) to add labels for each mod
    for (SFSpell *spell_data : g_internal_spell_list)
    {
        SFMod *parent_mod = spell_data->parent_mod;
        
        // Check if the parent mod is different from the previous one
        if (old_parent != parent_mod)
        {
            // Only process mods within the current page range
            if (index >= use_index && index < max_index)
            {
                log_info("Use them pointers");
                CMnuLabel *mod_title_label = mod_struct.title_label;
                CMnuLabel *mod_description_label = mod_struct.desc_label;

                log_info("Get Mod Info");
                // Format title and version with author in bold font
                char mod_title[512];
                snprintf(mod_title, sizeof(mod_title), "%s %s by %s", 
                         parent_mod->mod_id, 
                         parent_mod->mod_version, 
                         parent_mod->mod_author);
                
                // Format description (regular font)
                char mod_description[512];
                snprintf(mod_description, sizeof(mod_description), "%s", 
                         parent_mod->mod_description);


                log_info("Check init logic");
                if(is_init_finished)
                {
                    log_info("Title Exists, Let's Change the Text instead");
                    SF_String title_string;
                    SF_String *sf_string_mod_title = g_create_sf_string(&title_string, mod_title);

                    char info_test[128];
                    sprintf(info_test, "Testing PTR in mod label existing: %x", mod_title_label);
                    log_info(info_test);
                    g_menu_label_set_string(mod_title_label, sf_string_mod_title);
                    g_destroy_sf_string(sf_string_mod_title);

                    log_info("Desc Exists, Let's Change the Text instead");
                    SF_String desc_string;
                    SF_String *sf_string_mod_desc = g_create_sf_string(&desc_string, mod_description);
                    g_menu_label_set_string(mod_description_label, sf_string_mod_desc);
                    g_destroy_sf_string(sf_string_mod_desc);
                }
                else 
                {
                    log_info("Attach Label Title");
                    
                    mod_title_label = attach_new_label(mod_title_label, _container, mod_title, 6, 48, y_offset + (index % mods_per_page) * 36, 227, 36);
                    
                    char info_test[128];
                    sprintf(info_test, "Testing PTR in mod label: %x", mod_title_label);
                    log_info(info_test);

                    log_info("Attach Label Desc");
                    mod_description_label = attach_new_label(mod_description_label, _container, mod_description, 11, 48, y_offset + (index % mods_per_page) * 36 + 20, 227, 36);
                    mod_struct.title_label = mod_title_label;
                    mod_struct.desc_label = mod_description_label;
                    
                    is_init_finished = true;
                }
                log_info("Finished Mod Info");
            }
            log_info("Go to Next Mod");
            old_parent = parent_mod;
            index++;
        }
    }
    log_info("Mod Info Attached and Setup");
}


void __fastcall navigate_callback(CMnuSmpButton *button, int32_t* cui_menu_ptr_maybe)
{
    log_info("Nav Start");
    
    CMnuContainer *parent = button->CMnuBase_data.param_2_callback;
    log_info("Get Mod Struct");

    char struct_info[64];
    sprintf(struct_info, "Mod Struct: %x", &mod_struct);
    log_info(struct_info);

    log_info("Get Index");
    uint8_t index = mod_struct.index;

    log_info("Check Mod Count");
    if(index > g_mod_count) 
    {
        log_info("Set Index Again");
        mod_struct.index = 0;
        index = 0;
    }
    log_info("Attach Mod Label");
    attach_mod_labels(parent, 1, index);
    log_info("Increase Index");
    mod_struct.index = index + 1;
    log_info("Nav End");
}

SFSF_ModlistStruct modinformation;
CMnuContainer *mod_list;
void __thiscall show_mod_list(CMnuSmpButton *button)
{
    log_info("Mod List Callback");
    CMnuContainer *parent = button->CMnuBase_data.param_2_callback;
    SF_String s_menu_border, s_menu_background, s_alt_btn_name;
    SF_String * p_menu_border, * p_menu_background, * p_alt_btn_name;

    char alt_name[32] = "HIDE MOD LIST";
    p_alt_btn_name = g_create_sf_string(&s_alt_btn_name, alt_name);

    set_button_name = (set_btn_name_ptr) (ASI::AddrOf(0x52f8a0));
    set_button_name(button, p_alt_btn_name);

    g_destroy_sf_string(p_alt_btn_name);
    mod_list = (CMnuContainer *) g_new_operator(0x340);
    initialize_menu_container = (initialize_menu_container_ptr)(ASI::AddrOf(0x505780));
    initialize_menu_container(mod_list);

    // Setup mesh loading for background of the container.
    char menu_border[128] = "ui_bgr_options_select_border.msb";
    char menu_background_fade[128] = "ui_bgr_options_select_border_transparency.msb";
    p_menu_border = g_create_sf_string(&s_menu_border, menu_border);
    p_menu_background = g_create_sf_string(&s_menu_background, menu_background_fade);
    
    setup_menu_container_data_ptr setup_menu_container_data = (setup_menu_container_data_ptr)(ASI::AddrOf(0x50FD30));
    setup_menu_container_data(mod_list, 500, 224, 432, 307, p_menu_background, p_menu_border);

    container_alpha_ptr set_container_alpha = (container_alpha_ptr)(ASI::AddrOf(0x512EB0));
    set_container_alpha(mod_list, 0.99);

    g_destroy_sf_string(p_menu_background);
    g_destroy_sf_string(p_menu_border);

    log_info("Attach ModList Container Callback");
    // Add new container to this container.
    g_container_add_control(parent, mod_list, (char *)0x01, (char *)0x01, 0);

    log_info("Add Nav Buttons");
    char btn_disabled[128]= "ui_btn_togglearrow_right_disabled.msh";
    char btn_pressed[128]  = "ui_btn_togglearrow_right_pressed.msh";
    char btn_load[1] = "";
    char btn_default[128]  = "ui_btn_togglearrow_right_default.msh";
    char btn_label[1] = "";
     
    attach_new_button(mod_list, btn_default, btn_pressed, btn_load, btn_disabled, btn_label, 7, (432 - (48 + 32)), 232, 32, 32, 0, &navigate_callback);

    char btn_disabled_left[128] = "ui_btn_togglearrow_left_disabled.msh";
    char btn_pressed_left[128] = "ui_btn_togglearrow_left_pressed.msh";
    char btn_default_left[128] = "ui_btn_togglearrow_left_default.msh";
     
    attach_new_button(mod_list, btn_default_left, btn_pressed_left, btn_load, btn_disabled_left, btn_label, 7, 48, 232, 32, 32, 1, &navigate_callback);

    log_info("Add Mod Labels");
    attach_mod_labels(mod_list, 1, 0);
}

void __fastcall show_mod_list_callback(CMnuSmpButton *button, int32_t* cui_menu_ptr_maybe)
{
    log_info("Callback for Main Mod List Button");
    CMnuContainer *parent = button->CMnuBase_data.param_2_callback;

    char btn_info[64];
    sprintf(btn_info, "Button Info Struct PTR: %x", mod_struct);
    log_info(btn_info);
    
    uint8_t toggle = mod_struct.toggle;

    if(toggle == 0)
    {
        log_info("Show Mod List");
        show_mod_list(button);
        mod_struct.toggle = 1;
    }
    if(toggle == 1)
    {
        log_info("Hide Mod List");
        char alt_name[32] = "SHOW MOD LIST";
        SF_String s_alt_btn_name;
        SF_String * p_alt_btn_name;
        p_alt_btn_name = g_create_sf_string(&s_alt_btn_name, alt_name);

        set_button_name = (set_btn_name_ptr) (ASI::AddrOf(0x52f8a0));
        set_button_name(button, p_alt_btn_name);
        g_destroy_sf_string(p_alt_btn_name);
        mod_struct.toggle = 0;
    }
}


void __thiscall attach_new_button(CMnuContainer *parent, char *button_mesh_default, char *button_mesh_pressed, char *button_initial_load_mesh, char *button_mesh_disabled, char *label_char, uint8_t font_index, uint16_t x_pos, uint16_t y_pos, uint16_t width, uint16_t height, int button_index, uint32_t callback_func_ptr)
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

    initialize_smp_button = (initialize_smp_button_ptr) (ASI::AddrOf(0x51a9d0));
    new_button = initialize_smp_button(new_button);
    SF_Font *selected_font = g_get_font(fonts, font_index);

    set_btn_name_ptr set_button_name = (set_btn_name_ptr) (ASI::AddrOf(0x512E30));
    set_button_name(new_button, label_string);

    // This seems to fill out the actual button data itself.
    create_button_func = (create_button_ptr) (ASI::AddrOf(0x52E1E0));
    create_button_func(new_button,x_pos,y_pos,width,height,mesh_string_default,init_load_mesh,mesh_string_pressed,mesh_string_disabled);

    vfunction_2_ptr set_font = (vfunction_2_ptr)(ASI::AddrOf(0x530C20)); // Original 0x530c50
    set_font(new_button, selected_font);

    set_btn_index_ptr set_button_index = (set_btn_index_ptr)(ASI::AddrOf(0x5136a0));
    set_button_index(new_button, button_index);

    set_button_flag_ptr set_menu_button_flag = (set_button_flag_ptr)(ASI::AddrOf(0x5308A0));
    set_menu_button_flag(new_button, (char)0x1);
    
    set_button_name = (set_btn_name_ptr) (ASI::AddrOf(0x52f8a0));
    set_button_name(new_button, label_string);

    CUtlCallback2 callback;
    callback.vtable_ptr = *(uint32_t *)(ASI::AddrOf(0x7F9C64));
    callback.param_ptr = parent;
    callback.callback_func = callback_func_ptr;
    
    vfunction2_callback_attach_ptr attach_callback = (vfunction2_callback_attach_ptr)(ASI::AddrOf(0x6188B0));
    uint32_t param1, param2, param3;

    attach_callback(&callback, &param1, &param2, &param3);

    new_button->CMnuBase_data.param_1_callback = param1;
    new_button->CMnuBase_data.param_2_callback = param2;
    new_button->CMnuBase_data.param_3_callback = param3;

    vfunction_2_ptr vfunction16_attach_callback = (vfunction_2_ptr) (ASI::AddrOf(0x532B90));
    vfunction16_attach_callback(new_button, '\x01');

    g_container_add_control(parent, new_button, (char *)0x01, (char *)0x01, 0);

    g_destroy_sf_string(mesh_string_default);
    g_destroy_sf_string(mesh_string_pressed);
    g_destroy_sf_string(init_load_mesh);
    g_destroy_sf_string(mesh_string_disabled);
    g_destroy_sf_string(label_string);
}

CMnuLabel * __thiscall attach_new_meshed_label(CMnuLabel *new_label, CMnuContainer *parent, char *mesh_char, char *label_char, uint8_t font_index, uint16_t x_pos, uint16_t y_pos, uint16_t width, uint16_t height)
{
    SF_String m_mesh_string;
    SF_String m_label_string;

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
    vfunction25(new_label, (char *)0x1);

    g_menu_label_set_font(new_label, selected_font);

    g_container_add_control(parent, new_label, (char *)0x01, (char *)0x01, 0);

    vfunction12 = (vfunction12_ptr)(ASI::AddrOf(0x511ae0));
    vfunction12(parent, new_label, (char *)0x1);
    g_menu_label_set_string(new_label, label_string);

    g_destroy_sf_string(label_string);
    g_destroy_sf_string(mesh_string);

    char label_info[64];
    sprintf(label_info, "LABEL PTR: %x", new_label);
    log_info(label_info);

    return new_label;
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