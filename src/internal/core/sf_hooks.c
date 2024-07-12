#include "sf_wrappers.h"
#include "sf_modloader.h"
#include "sf_hooks.h"
#include "../handlers/sf_spelltype_handlers.h"
#include "../registry/sf_spelltype_registry.h"

#include "hooks/sf_refresh_hook.h"
#include "hooks/sf_endspell_hook.h"
#include "hooks/sf_menu_hook.h"
#include "hooks/sf_spelleffect_hook.h"
#include "hooks/sf_subeffect_hook.h"
#include "hooks/sf_damage_hook.h"
#include "hooks/sf_spelltype_hook.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

console_print_ptr console_print;
figure_toolbox_is_targetable_ptr figure_toolbox_is_targetable;

menu_label_ptr initialize_menu_label;
menu_label_set_string_ptr menu_label_set_string;
message_box_ptr show_message_box;
initialize_menu_container_ptr initialize_menu_container;
construct_default_sf_string_ptr construct_default_sf_string;

// Internal data hooks
uint32_t menu_return_addr;
new_operator_ptr new_operator;
menu_label_constructor_ptr menu_label_constructor;
mnu_label_init_data_ptr init_menu_element;
menu_label_set_data_ptr menu_label_set_color;
get_smth_fonts_ptr get_smth_fonts;
menu_label_set_font_ptr menu_label_set_font;
get_font_ptr get_font;
container_add_control_ptr container_add_control;

FUN_0069eaf0_ptr FUN_0069eaf0;
fidfree_ptr fidFree;
SF_String_ctor_ptr SF_String_ctor;
SF_String_dtor_ptr SF_String_dtor;

SpellFunctions spellAPI;
EffectFunctions effectAPI;
ToolboxFunctions toolboxAPI;
FigureFunctions figureAPI;
IteratorFunctions iteratorAPI;
RegistrationFunctions registrationAPI;

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

    original_menu_func = (original_menu_func_ptr)(ASI::AddrOf(0x197b10));
    menu_return_addr = (ASI::AddrOf(0x182799));
    show_message_box = (message_box_ptr)(ASI::AddrOf(0x198660));

    // uint32_t _application = ASI::AddrOf(0x925C64);
    new_operator = (new_operator_ptr)(ASI::AddrOf(0x675A9D));
    menu_label_constructor = (menu_label_constructor_ptr)(ASI::AddrOf(0x51a180));
    init_menu_element = (mnu_label_init_data_ptr)(ASI::AddrOf(0x52cfe0));
    // This does seem to work, but has some strange behaviour, the color format is strange
    menu_label_set_color = (menu_label_set_data_ptr)(ASI::AddrOf(0x530330));

    get_smth_fonts = (get_smth_fonts_ptr)(ASI::AddrOf(0x5357b0));
    menu_label_set_font = (menu_label_set_font_ptr)(ASI::AddrOf(0x530e00));
    get_font = (get_font_ptr)(ASI::AddrOf(0x535180));
    container_add_control = (container_add_control_ptr)(ASI::AddrOf(0x506f30));

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
    DEFINE_FUNCTION(spell, onSpellRemove, 0x32b310);
    INCLUDE_FUNCTION(spell, addSpell, &sf_spelltype_hook);
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
    *(int *)(ASI::AddrOf(0x328d61)) = (int)(&sf_spelltype_hook) - ASI::AddrOf(0x328d65);
    ASI::EndRewrite(add_spell_mreg);
}

void initialize_spell_trigger_hook()
{
    ASI::MemoryRegion add_spell_mreg(ASI::AddrOf(0x278773), 5);
    ASI::BeginRewrite(add_spell_mreg);
    *(unsigned char *)(ASI::AddrOf(0x278773)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x278774)) = (int)(&sf_spelleffect_hook) - ASI::AddrOf(0x278778);
    ASI::EndRewrite(add_spell_mreg);
}

void initialize_subeffect_add_hook()
{
    ASI::MemoryRegion add_spell_mreg(ASI::AddrOf(0x2de3b7), 5);
    ASI::BeginRewrite(add_spell_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2de3b7)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x2de3b8)) = (int)(&sf_subeffect_hook) - ASI::AddrOf(0x2de3bc);
    ASI::EndRewrite(add_spell_mreg);
}

void initialize_spellend_hook()
{
    ASI::MemoryRegion end_spell_mreg(ASI::AddrOf(0x34b0a0), 5);
    ASI::BeginRewrite(end_spell_mreg);
    *(unsigned char *)(ASI::AddrOf(0x34b0a0)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x34b0a1)) = (int)(&sf_endspell_hook) - ASI::AddrOf(0x34b0a5);
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
    *(int *)(ASI::AddrOf(0x329f95)) = (int)(&sf_refresh_hook) - ASI::AddrOf(0x329f99);
    ASI::EndRewrite(refresh_spell_mreg);
}

// Figure out later -> Function (Siege Handler for Multi Effect Spells)
void __declspec(naked) menuload_hook_beta()
{
    asm("push %%edi         \n\t"
        "call %P0           \n\t"
        "pop %%edi          \n\t"
        "jmp *%1            \n\t" : : "i"(sf_menu_hook), "o"(menu_return_addr));
}

void initialize_menuload_hook()
{
    ASI::MemoryRegion menu_load_mreg(ASI::AddrOf(0x182794), 5);
    ASI::BeginRewrite(menu_load_mreg);
    *(unsigned char *)(ASI::AddrOf(0x182794)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x182795)) = (int)(&menuload_hook_beta) - ASI::AddrOf(0x182799);
    ASI::EndRewrite(menu_load_mreg);
}

void initialize_deal_damage_hook()
{
    uint32_t flag_pointer = *(uint32_t *)(ASI::AddrOf(0x2f4b56));
    ASI::MemoryRegion deal_damage_mreg(ASI::AddrOf(0x2f4a57), 9);
    ASI::BeginRewrite(deal_damage_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2f4a57)) = 0x90; // nop trail
    *(unsigned char *)(ASI::AddrOf(0x2f4a58)) = 0x90; // nop trail
    *(unsigned char *)(ASI::AddrOf(0x2f4a59)) = 0x90; // nop trail
    *(unsigned char *)(ASI::AddrOf(0x2f4a5a)) = 0x90; // nop trail
    *(unsigned char *)(ASI::AddrOf(0x2f4a5b)) = 0xE9; // JMP
    *(int *)(ASI::AddrOf(0x2f4a5c)) = (int)(&sf_damage_hook) - ASI::AddrOf(0x2F4A60);
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

    log_info("Hooking Deal Damage Trigger");
    initialize_deal_damage_hook();

    log_info("Hooking AddSpellFromEffect function");
    initialize_subeffect_add_hook();

    log_info("Dirty Menu Loading Trigger Test");
    initialize_menuload_hook();
}