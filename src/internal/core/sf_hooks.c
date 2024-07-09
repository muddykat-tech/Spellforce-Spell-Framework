#include "sf_wrappers.h"
#include "sf_modloader.h"
#include "sf_hooks.h"
#include "../handlers/sf_spelltype_handlers.h"
#include "../registry/sf_spelltype_registry.h"
#include "../registry/sf_spelleffect_registry.h"
#include "../registry/sf_spellend_registry.h"
#include "../registry/sf_subeffect_registry.h"
#include "../registry/sf_spellrefresh_registry.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

console_print_ptr console_print;
get_spell_spell_line_ptr get_spell_spell_line;
figure_toolbox_get_unkn_ptr figure_toolbox_get_unkn;
figure_toolbox_add_spell_ptr figure_toolbox_add_spell;
figure_toolbox_is_targetable_ptr figure_toolbox_is_targetable;

menu_label_ptr initialize_menu_label;
menu_label_set_string_ptr menu_label_set_string;
message_box_ptr show_message_box;
initialize_menu_container_ptr initialize_menu_container;
construct_default_sf_string_ptr construct_default_sf_string;

new_operator_ptr new_operator;
container_add_control_ptr container_add_control;
menu_label_set_data_ptr menu_label_set_data;

FUN_0069eaf0_ptr FUN_0069eaf0;
fidfree_ptr fidFree;
SF_String_ctor_ptr SF_String_ctor;
SF_String_dtor_ptr SF_String_dtor;
mnu_label_init_data_ptr init_menu_element;

SpellFunctions spellAPI;
EffectFunctions effectAPI;
ToolboxFunctions toolboxAPI;
FigureFunctions figureAPI;
IteratorFunctions iteratorAPI;
RegistrationFunctions registrationAPI;

// Bool function?
int __thiscall CheckCanApply_hook_beta(SF_CGdSpell *_this, uint16_t spell_index)
{
    // We need a map of refresh handlers?
    // And a default handler that does nothing
    // handler takes (SF_CGdSpell *_this, uint16_t spell_index) as params

    // We need to parse in spell_index as it's used in the handler itself
    uint16_t spell_line = _this->active_spell_list[spell_index].spell_line;
    refresh_handler_ptr spellrefresh_handler = get_spell_refresh(spell_line);
    char message[256];
    sprintf(message, "Spell Refresh Hook: [%d]", spell_index);
    log_info(message);

    if (spellrefresh_handler != NULL)
    {
        return spellrefresh_handler(_this, spell_index);
    }
    // Default return is 1, it can be 0
    return 1;
}

void __thiscall EndSpell_hook_beta(SF_CGdSpell *_this, uint16_t spell_index)
{
    // We need a map of end spell handlers?
    // And a default handler that does nothing
    // handler takes (SF_CGdSpell *_this, uint16_t spell_index) as params
    uint16_t spell_line = _this->active_spell_list[spell_index].spell_line;
    handler_ptr spellend_handler = get_spell_end(spell_line);
    if (spellend_handler != NULL)
    {
        spellend_handler(_this, spell_index);
    }
}

void __thiscall menu_trigger(uint32_t _CAppMenu)
{
    // Messy and ugly Code below, you have been warned. ~Muddykat

    SF_String *test_label_string;

    log_info("New Constructor Test");
    char sfsf_info[256];

    sprintf(sfsf_info, "Spellforce Spell Framework %s\n%d Mod(s) Loaded with %d Error(s)", framework_mod->mod_version, mod_count, error_count);
    test_label_string = SF_String_ctor(test_label_string, sfsf_info);

    uint32_t CAppMenu_data = *(uint32_t *)(_CAppMenu + 0x4);
    log_info("Menu Init 1");
    uint32_t CMnuScreen_ptr = *(uint32_t *)(CAppMenu_data + 0x68);
    uint32_t container_hack_ptr = *(uint32_t *)(_CAppMenu + 0x58);
    CMnuContainer *container_hack = (CMnuContainer *)container_hack_ptr;
    log_info("Menu Init 2");
    uint32_t screen_vftable_ptr = CMnuScreen_ptr;
    uint32_t _application = ASI::AddrOf(0x925C64);

    log_info("New Container Test");
    // show_message_box(_application, 0, test_label_string, 1);
    //   This DOES work, it creates a new Container properly, but we now need to attach it somehow...
    new_operator = (new_operator_ptr)(ASI::AddrOf(0x675A9D));

    log_info("New Container Test 2");
    CMnuContainer *new_cont_test = (CMnuContainer *)new_operator(0x340);

    log_info("container init");
    initialize_menu_container(new_cont_test);
    // Probs not best practice to manually set it's parent, there should be some function to do this properly (like container_add_control)
    new_cont_test->CMnuBase_data.parent_ptr = CMnuScreen_ptr;
    log_info("Label Creation");
    CMnuLabel *test_label;
    test_label = (CMnuLabel *)new_operator(0x368);

    menu_label_constructor_ptr menu_label_constructor = (menu_label_constructor_ptr)(ASI::AddrOf(0x51a180));
    menu_label_constructor(test_label);
    log_info("Label Setup");
    // setup menu label data
    mnu_label_init_data_ptr init_menu_element = (mnu_label_init_data_ptr)(ASI::AddrOf(0x52cfe0));

    log_info("Label Setup 1.5");

    init_menu_element(test_label, 10, 10, 100, 100, test_label_string);

    log_info("Label Setup 2");
    menu_label_set_data_ptr menu_label_set_color = (menu_label_set_data_ptr)(ASI::AddrOf(0x530330));

    log_info("Label Setup 2.1");
    // CMnuLabel::FUN_00930330
    uint32_t color = 0xD5C98F;

    get_sf_color_ptr get_sf_color = (get_sf_color_ptr)(ASI::AddrOf(0x619670));

    log_info("Label Setup 2.2");
    // get_sf_color(test_label_string, 0x100);

    // SF_Color *sf_color;
    // sf_color->R = ((float)(color >> 0x10 & 0xff) + 0.0) / 255.0;
    // sf_color->G = ((float)(color >> 8 & 0xff) + 0.0) / 255.0;
    // sf_color->B = ((float)(color & 0xff) + 0.0) / 255.0;

    log_info("Label Setup 3");

    // get fonts
    get_smth_fonts_ptr get_smth_fonts = (get_smth_fonts_ptr)(ASI::AddrOf(0x5357b0));
    SF_FontStruct *fonts = get_smth_fonts();
    get_font_ptr get_font = (get_font_ptr)(ASI::AddrOf(0x535180));
    SF_Font *selected_font = get_font(fonts, 6); // Select font 6 (there are 32 from what I can tell)

    log_info("Label Setup 3.1");
    menu_label_set_font_ptr menu_label_set_font = (menu_label_set_font_ptr)(ASI::AddrOf(0x530e00));

    log_info("Label Setup 3.2");
    menu_label_set_font(test_label, selected_font);

    log_info("Label Setup 3.3");
    // menu_label_set_color(test_label, sf_color->R, sf_color->G, sf_color->B, '\x01');

    log_info("Label Attach Test");
    container_add_control = (container_add_control_ptr)(ASI::AddrOf(0x506f30));
    container_add_control(container_hack, test_label, '\0', '\0', 0);

    menu_label_set_string(test_label, test_label_string);

    log_info("Menu Trigger Was Called");
    original_menu_func(_CAppMenu);

    log_info("Test String Dtor");
    SF_String_dtor(test_label_string);
    log_info("Jumping back to Normal Flow");
}

void __thiscall effect_trigger_hook(SF_CGdSpell *_this)
{
    uint16_t spell_index;
    for (spell_index = 1; spell_index <= _this->max_used; ++spell_index)
    {
        if (_this->active_spell_list[spell_index].spell_id != 0)
        {
            short to_do_count = _this->active_spell_list[spell_index].to_do_count;
            if (to_do_count == 0)
            {
                to_do_count = 0;
            }
            else
            {
                to_do_count = to_do_count + -1;
            }
            _this->active_spell_list[spell_index].to_do_count = to_do_count;
            if (to_do_count == 0)
            {
                uint16_t spell_job = _this->active_spell_list[spell_index].spell_job;
                handler_ptr func = get_spell_effect(spell_job);
                if (func != NULL)
                {
                    func(_this, spell_index);
                }
            }
        }
    }
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

void console_log(const char *message)
{
    if (console_print != NULL)
    {
        uint32_t _application = ASI::AddrOf(0x925C64);
        console_print(_application, message);
    }
    else
    {
        OutputDebugStringA(message);
    }
    // Appened to log file
    log_message("sfsf.log", message);
}

void __thiscall add_spell_from_effect_hook_beta(SF_CGDEffect *_this, uint16_t effect_id)
{
    uint16_t spell_id = effectAPI.getEffectXData(_this, effect_id, EFFECT_SUBSPELL_ID);
    log_info("Sub effect hook triggered");
    if (spell_id)
    {
        SF_CGdResourceSpell spell_data;
        spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, spell_id);
        sub_effect_handler_ptr handler = get_sub_effect_handler(spell_data.spell_line_id);
        if (handler != NULL)
        {
            log_info("Sub effect handler found");
            handler(_this, effect_id);
        }
        // get handler from registry
        // handler takes _this, effect_id
    }
}

void __thiscall dealdamage_hook_beta(SF_CGdFigureToolbox *figureToolbox, uint16_t dmg_source, uint16_t dmg_target, uint32_t damage_amount, uint32_t is_spell_damage, uint32_t param_5, uint32_t vry_unknown_6)
{
    uint16_t target_job = figureAPI.getJob(figureToolbox->CGdFigure, dmg_target);
    bool unknown_job_flag_check = figureAPI.FUN_006e3a90(figureToolbox->CGdFigureJobs, target_job);
    bool is_source_alive = figureAPI.isAlive(figureToolbox->CGdFigure, dmg_source);
    bool spell_effecting_source = toolboxAPI.hasSpellOnIt(figureToolbox, dmg_source, 0xa5);
    bool spell_effecting_target = toolboxAPI.hasSpellOnIt(figureToolbox, dmg_target, 0xa5);

    bool spell_flag_check = dmg_source != 0 && is_source_alive && spell_effecting_source && spell_effecting_target;

    if (unknown_job_flag_check != 0)
    {
        // Sets Job To Do Count
        figureAPI.setJobToDoCount(figureToolbox->CGdFigure, dmg_target, 1);
    }

    bool check_spells_before_job = figureAPI.isFlagSet(figureToolbox->CGdFigure, dmg_target, F_CHECK_SPELLS_BEFORE_CHECK_BATTLE);
    if (check_spells_before_job)
    {
        // Main Checks for Spell Cases... This would be where we may be able to implement a registry system of some sort.
        uint32_t job_start_node_index = figureAPI.getSpellJobStartNode(figureToolbox->CGdFigure, dmg_target);
        while (job_start_node_index != 0)
        {
            // e get the spell index through CGdDoubleLinkList,
            job_start_node_index = 0; // TEMP, just to ensure no infinity loop
        }
    }
    else
    {
    }

    log_info("Called into Overwritten Damage Function");
    // First Call Section
    // Checks if Source is still alive, and if source has spells on it
    // Next it checks the Flag "CHECK_SPELLS_BEFORE_JOB"?
    // If it's FALSE, it will then get the spell job starting node
    // Strange check here, using Undefined 4, it then has a while loop while a field of undefined 4 is not 0
    // This loop will then get the spell_index, to get the spell_line then it checks specific spells eg. 'endurance', if active specific damage calc for endurance.
    // if not, it checks for invulnerability and so on.
}

uint16_t __thiscall add_spell_hook_beta(SF_CGdSpell *_this, uint16_t spell_id, uint16_t param2, SF_CGdTargetData *source, SF_CGdTargetData *target, uint16_t param5)
{
    uint16_t spell_index;
    uint16_t spell_line;
    if (target->entity_type == 1)
    {
        if (!figure_toolbox_get_unkn(_this->SF_CGdFigureToolBox, target->entity_index))
        {
            return 0;
        }
    }

    // find 1st free slot for spell
    for (spell_index = 1; spell_index < 799; ++spell_index)
    {
        if (_this->active_spell_list[spell_index].spell_id == 0)
        {
            break;
        }
    }
    if (spell_index == 799)
    {
        log_warning("Maximum spell limit (800) reached. Additional spells may not be registered.");
        return 0;
    }
    // max_used spells update
    if (_this->max_used < spell_index)
    {
        _this->max_used = spell_index;
    }

    // somewhere here is smth goes south
    _this->active_spell_list[spell_index].spell_id = spell_id;
    spell_line = get_spell_spell_line(_this->SF_CGdResource, spell_id);
    _this->active_spell_list[spell_index].spell_line = spell_line;
    _this->active_spell_list[spell_index].source.entity_type = source->entity_type;
    _this->active_spell_list[spell_index].source.entity_index = source->entity_index;
    _this->active_spell_list[spell_index].source.position.X = (source->position).X;
    _this->active_spell_list[spell_index].source.position.Y = (source->position).Y;
    _this->active_spell_list[spell_index].target.entity_type = target->entity_type;
    _this->active_spell_list[spell_index].target.entity_index = target->entity_index;
    _this->active_spell_list[spell_index].target.position.X = (target->position).X;
    _this->active_spell_list[spell_index].target.position.Y = (target->position).Y;
    _this->active_spell_list[spell_index].to_do_count = param2 - (uint16_t)(_this->OpaqueClass->current_step);

    handler_ptr func = get_spell_handler(spell_line);
    func(_this, spell_index);

    if (target->entity_type == 1)
    {
        if (spell_line != 0x5A)
        {
            figure_toolbox_add_spell(_this->SF_CGdFigureToolBox, target->entity_index, spell_index);
        }
    }
    return spell_index;
}

/*
    These functions, and future types, will eventually be moved into "sf_api" or some such
*/
void initialize_console_hook()
{
    uint32_t CAppMain_ptr = ASI::AddrOf(0x9229A8);
    uint32_t CAppMenu_ptr = *(uint32_t *)(CAppMain_ptr + 0x4);
    uint32_t CMnuScrConsole_ptr = *(uint32_t *)(CAppMenu_ptr + 0x80);
    console_print = (console_print_ptr)ASI::AddrOf(0x1b58f0);
    SF_String_ctor = (SF_String_ctor_ptr)ASI::AddrOf(0x3837e0);
    SF_String_dtor = (SF_String_dtor_ptr)ASI::AddrOf(0x3839c0);
}

void initialize_data_hooks()
{
    log_info("| - Internal Use Hooks");
    // Required for internal use
    get_spell_spell_line = (get_spell_spell_line_ptr)(ASI::AddrOf(0x26E100));
    figure_toolbox_get_unkn = (figure_toolbox_get_unkn_ptr)(ASI::AddrOf(0x2FE704));
    figure_toolbox_add_spell = (figure_toolbox_add_spell_ptr)(ASI::AddrOf(0x2F673A));

    initialize_menu_label = (menu_label_ptr)(ASI::AddrOf(0x51a180));
    menu_label_set_string = (menu_label_set_string_ptr)(ASI::AddrOf(0x52fab0));

    initialize_menu_container = (initialize_menu_container_ptr)(ASI::AddrOf(0x505780));
    construct_default_sf_string = (construct_default_sf_string_ptr)(ASI::AddrOf(0x383900));

    // used in Iterator for AOE Spells Dispose
    FUN_0069eaf0 = (FUN_0069eaf0_ptr)(ASI::AddrOf(0x29EAF0));
    fidFree = (fidfree_ptr)(ASI::AddrOf(0x6B6E25));

    log_info("| - FigureAPI Hooks");
    // More defined for external use in api
    DEFINE_FUNCTION(figure, isAlive, 0x1BE4D0);
    DEFINE_FUNCTION(figure, setWalkSpeed, 0x2B7190);
    DEFINE_FUNCTION(figure, addAction, 0x2AE0B0);
    DEFINE_FUNCTION(figure, addBonusMult, 0x35A3E0);
    DEFINE_FUNCTION(figure, decreaseHealth, 0x2b5b40);
    DEFINE_FUNCTION(figure, getCurrentHealth, 0x279350);
    DEFINE_FUNCTION(figure, getCurrentMaxMana, 0x2b2a20);
    DEFINE_FUNCTION(figure, rescaleMana, 0x2b5d50);
    DEFINE_FUNCTION(figure, getCurrentMaxHealth, 0x2b2970);
    DEFINE_FUNCTION(figure, rescaleHealth, 0x2b5cd0);
    DEFINE_FUNCTION(figure, getJob, 0x279290);
    DEFINE_FUNCTION(figure, FUN_006e3a90, 0x2e3a90);
    DEFINE_FUNCTION(figure, setJobToDoCount, 0x300910);
    DEFINE_FUNCTION(figure, isFlagSet, 0x279d20);
    DEFINE_FUNCTION(figure, getSpellJobStartNode, 0x2b2de0);

    log_info("| - SpellAPI Hooks");
    // Define the function pointers for SpellFunctions group
    DEFINE_FUNCTION(spell, setXData, 0x329C40);
    DEFINE_FUNCTION(spell, setEffectDone, 0x32A730);
    DEFINE_FUNCTION(spell, addToXData, 0x32AAC0);
    DEFINE_FUNCTION(spell, getChanceToResistSpell, 0x317BA0);
    DEFINE_FUNCTION(spell, getRandom, 0x2AD200);
    DEFINE_FUNCTION(spell, addVisualEffect, 0x329B30);
    DEFINE_FUNCTION(spell, figureAggro, 0x329c90);
    DEFINE_FUNCTION(spell, getResourceSpellData, 0x26dc40);
    DEFINE_FUNCTION(spell, getXData, 0x32aa60);
    DEFINE_FUNCTION(spell, getTargetsRectangle, 0x329D80);
    DEFINE_FUNCTION(spell, removeDLLNode, 0x34B030);
    DEFINE_FUNCTION(spell, figClrChkSplBfrChkBattle, 0x32a470);
    DEFINE_FUNCTION(spell, figTryClrCHkSPlBfrJob2, 0x32a4f0);
    DEFINE_FUNCTION(spell, figTryUnfreeze, 0x32a5a0);
    DEFINE_FUNCTION(spell, onSpellRemove, 0x32b310); // We may need to hook this?
    INCLUDE_FUNCTION(spell, addSpell, &add_spell_hook_beta);
    DEFINE_FUNCTION(effect, addEffect, 0x2dc880);
    DEFINE_FUNCTION(effect, setEffectXData, 0x2ddb30);
    DEFINE_FUNCTION(effect, getEffectXData, 0x2dd730);

    log_info("| - ToolboxAPI Hooks");

    DEFINE_FUNCTION(toolbox, dealDamage, 0x2f4a57);
    DEFINE_FUNCTION(toolbox, isTargetable, 0x2fe704);
    DEFINE_FUNCTION(toolbox, figuresCheckHostile, 0x2fe7b9);
    DEFINE_FUNCTION(toolbox, hasSpellOnIt, 0x2fe4ea);
    DEFINE_FUNCTION(toolbox, rescaleLevelStats, 0x2fff48);
    DEFINE_FUNCTION(toolbox, buildingDealDamage, 0x2d6d80);
    DEFINE_FUNCTION(toolbox, addSpellToFigure, 0x2f673a);
    DEFINE_FUNCTION(toolbox, getFigureFromWorld, 0x34F160);
    DEFINE_FUNCTION(toolbox, getSpellIndexOfType, 0x2fd939);

    log_info("| - IteratorAPI Hooks");
    DEFINE_FUNCTION(iterator, figureIteratorInit, 0x3183f0);
    DEFINE_FUNCTION(iterator, figureIteratorSetPointers, 0x31a680);
    DEFINE_FUNCTION(iterator, iteratorSetArea, 0x3195d0);
    DEFINE_FUNCTION(iterator, getNextFigure, 0x318f50);
    DEFINE_FUNCTION(iterator, buildingIteratorInit, 0x318290);
    DEFINE_FUNCTION(iterator, buildingIteratorSetPointers, 0x31A640);

    log_info("| - SpellAPI Wrappers");
    // Method to include functions WE define in the Internal code.
    INCLUDE_FUNCTION(spell, initializeSpellData, &initializeSpellData);
    INCLUDE_FUNCTION(spell, spellClearFigureFlag, &spellClearFigureFlag);

    log_info("| - FigureAPI Wrappers");
    INCLUDE_FUNCTION(figure, addBonusMultToStatistic, &addBonusMultToStatistic);

    log_info("| - IteratorAPI Wrappers");
    INCLUDE_FUNCTION(iterator, setupFigureIterator, &setupFigureIterator);
    INCLUDE_FUNCTION(iterator, disposeFigureIterator, &disposeFigureIterator);

    log_info("| - RegistrationAPI Wrappers");
    INCLUDE_FUNCTION(registration, registerSpell, &registerSpell);
    INCLUDE_FUNCTION(registration, linkTypeHandler, &linkTypeHandler);
    INCLUDE_FUNCTION(registration, linkEffectHandler, &linkEffectHandler);
    INCLUDE_FUNCTION(registration, linkEndHandler, &linkEndHandler);
    INCLUDE_FUNCTION(registration, applySpellTag, &applySpellTag);
    INCLUDE_FUNCTION(registration, linkSubEffectHandler, &linkSubEffectHandler);
    INCLUDE_FUNCTION(registration, linkRefreshHandler, &linkRefreshHandler);
}

void initialize_spelltype_hook()
{
    ASI::MemoryRegion add_spell_mreg(ASI::AddrOf(0x328d60), 5);
    ASI::BeginRewrite(add_spell_mreg);
    *(unsigned char *)(ASI::AddrOf(0x328d60)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x328d61)) = (int)(&add_spell_hook_beta) - ASI::AddrOf(0x328d65);
    ASI::EndRewrite(add_spell_mreg);
}

void initialize_spell_trigger_hook()
{
    ASI::MemoryRegion add_spell_mreg(ASI::AddrOf(0x278773), 5);
    ASI::BeginRewrite(add_spell_mreg);
    *(unsigned char *)(ASI::AddrOf(0x278773)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x278774)) = (int)(&effect_trigger_hook) - ASI::AddrOf(0x278778);
    ASI::EndRewrite(add_spell_mreg);
}

void initialize_sub_effect_add_hook()
{
    ASI::MemoryRegion add_spell_mreg(ASI::AddrOf(0x2de3b7), 5);
    ASI::BeginRewrite(add_spell_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2de3b7)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x2de3b8)) = (int)(&add_spell_from_effect_hook_beta) - ASI::AddrOf(0x2de3bc);
    ASI::EndRewrite(add_spell_mreg);
}

void initialize_spellend_hook()
{
    ASI::MemoryRegion end_spell_mreg(ASI::AddrOf(0x34b0a0), 5);
    ASI::BeginRewrite(end_spell_mreg);
    *(unsigned char *)(ASI::AddrOf(0x34b0a0)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x34b0a1)) = (int)(&EndSpell_hook_beta) - ASI::AddrOf(0x34b0a5);
    ASI::EndRewrite(end_spell_mreg);
}

void initialize_spellrefresh_hook()
{
    ASI::MemoryRegion refresh_spell_mreg(ASI::AddrOf(0x329f90), 9);
    ASI::BeginRewrite(refresh_spell_mreg);
    *(unsigned char *)(ASI::AddrOf(0x329f90)) = 0x90; // nop trail
    *(unsigned char *)(ASI::AddrOf(0x329f91)) = 0x90; // nop trail
    *(unsigned char *)(ASI::AddrOf(0x329f92)) = 0x90; // nop trail
    *(unsigned char *)(ASI::AddrOf(0x329f93)) = 0x90; // nop trail
    *(unsigned char *)(ASI::AddrOf(0x329f94)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x329f95)) = (int)(&CheckCanApply_hook_beta) - ASI::AddrOf(0x329f99);
    ASI::EndRewrite(refresh_spell_mreg);
}

original_menu_func_ptr original_menu_func;
uint32_t menu_return_addr;

// Figure out later -> Function (Siege Handler for Multi Effect Spells)
void __declspec(naked) menuload_hook_beta()
{
    asm("push %%edi         \n\t"
        "call %P0           \n\t"
        "pop %%edi          \n\t"
        "jmp *%1            \n\t" : : "i"(menu_trigger), "o"(menu_return_addr));
}

// Figure out later -> Function to add display in menu?
// Would like to display version of Mod framework and perhaps mod page with loaded mods
// NON Functional Hook, @TODO need to check if possible to hook this area [CUiStartMenu::~CUiStartMenu(CUiStartMenu *this, uint32_t param_1)]
void initialize_menuload_hook()
{
    // setup original_menu_func
    // setup return_addr
    original_menu_func = (original_menu_func_ptr)(ASI::AddrOf(0x197b10));
    menu_return_addr = (ASI::AddrOf(0x182799));
    show_message_box = (message_box_ptr)(ASI::AddrOf(0x198660));

    ASI::MemoryRegion menu_load_mreg(ASI::AddrOf(0x182794), 5);
    ASI::BeginRewrite(menu_load_mreg);
    *(unsigned char *)(ASI::AddrOf(0x182794)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x182795)) = (int)(&menuload_hook_beta) - ASI::AddrOf(0x182799);
    ASI::EndRewrite(menu_load_mreg);
}

void initialize_deal_damage_hook()
{
    ASI::MemoryRegion deal_damage_mreg(ASI::AddrOf(0x2f4a57), 9);
    ASI::BeginRewrite(deal_damage_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2f4a57)) = 0x90; // nop trail
    *(unsigned char *)(ASI::AddrOf(0x2f4a58)) = 0x90; // nop trail
    *(unsigned char *)(ASI::AddrOf(0x2f4a59)) = 0x90; // nop trail
    *(unsigned char *)(ASI::AddrOf(0x2f4a5a)) = 0x90; // nop trail
    *(unsigned char *)(ASI::AddrOf(0x2f4a5b)) = 0xE9; // JMP
    *(int *)(ASI::AddrOf(0x2f4a5c)) = (int)(&dealdamage_hook_beta) - ASI::AddrOf(0x2F4A60);
}

void initialize_beta_hooks()
{
    log_info("Hooking Game Console");
    initialize_console_hook();

    log_info("Hooking Spell Types");
    initialize_spelltype_hook();

    log_info("Hooking Spell Triggers");
    initialize_spell_trigger_hook();

    log_info("Hooking Spell Refresh Triggers");
    initialize_spellrefresh_hook();

    log_info("Hooking Spell End Triggers");
    initialize_spellend_hook();

    // log_info("Hooking Deal Damage Trigger");
    // initialize_deal_damage_hook();

    log_info("Hooking AddSpellFromEffect function");
    initialize_sub_effect_add_hook();

    log_info("Dirty Menu Loading Trigger Test");
    initialize_menuload_hook();
}