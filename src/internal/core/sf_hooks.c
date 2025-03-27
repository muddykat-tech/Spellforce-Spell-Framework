/**
 * @defgroup Hooks Assembly Hooks
 * @ingroup Internal
 * @details This group contains all the hooks used in SFSF.
 * @see initialize_data_hooks()
 */

#include "sf_wrappers.h"
#include "sf_modloader.h"
#include "sf_hooks.h"
#include "../handlers/sf_spelltype_handlers.h"
#include "../registry/spell_data_registries/sf_spelltype_registry.h"
#include "../registry/sf_mod_registry.h"

#include "hooks/sf_refresh_hook.h"
#include "hooks/sf_endspell_hook.h"
#include "hooks/sf_menu_hook.h"
#include "hooks/sf_spelleffect_hook.h"
#include "hooks/sf_subeffect_hook.h"
#include "hooks/sf_damage_hook.h"
#include "hooks/sf_spelltype_hook.h"
#include "hooks/sf_console_hook.h"
#include "hooks/sf_onhit_hook.h"
#include "hooks/sf_ai_hook.h"
#include "hooks/sf_utility_hooks.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/** @addtogroup Hooks
 * @{
 */

SpellFunctions spellAPI;
EffectFunctions effectAPI;
ToolboxFunctions toolboxAPI;
FigureFunctions figureAPI;
IteratorFunctions iteratorAPI;
AiFunctions aiAPI;
RegistrationFunctions registrationAPI;

/**
 * @brief Used to initialize all disparate hooks in one place.
 * @details This function is also used to initalize indiviual function hooks.
 * Individual Function hooks are simply storing a pointer to a function that's within Spellforce.
 * Wrapper Functions grab a pointer from our own codebase.
 * @see sf_wrappers.c
 */
void initialize_data_hooks()
{
    log_info("| - Internal Use Hooks");
    initialize_menu_data_hooks();

    // Required for internal use
    initialize_spelltype_data_hooks();

    // Used in Iterator for AOE Spells Dispose
    initialize_wrapper_data_hooks();

    initialize_onhit_data_hooks();

    // Used to print to the game console
    initialize_console_data_hooks();

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
    DEFINE_FUNCTION(figure, subMana, 0x2b5b60);
    DEFINE_FUNCTION(figure, getCurrentMana, 0x2b29c0);
    DEFINE_FUNCTION(figure, getPosition, 0x2caaf0);
    DEFINE_FUNCTION(figure, setTask, 0x2b7110);
    DEFINE_FUNCTION(figure, getWeaponStats, 0x2b30a0);
    DEFINE_FUNCTION(figure, getAggroValue, 0x2b1ab0);
    DEFINE_FUNCTION(figure, setAggroValue, 0x2b6670);
    DEFINE_FUNCTION(figure, isWarrior, 0x1c1510);
    DEFINE_FUNCTION(figure, getHealersList, 0x2b2870);
    DEFINE_FUNCTION(figure, disposeHealerList, 0x36436D);
    DEFINE_FUNCTION(figure, getCurrentHealthPercent, 0x2fdeb0);
    log_info("| - AI API Hooks");

    DEFINE_FUNCTION(ai, getTargetAction, 0x2b2f50);
    DEFINE_FUNCTION(ai, getFigureAction, 0x2b19c0);
    DEFINE_FUNCTION(ai, isAIVectorEmpty, 0x3645c7);
    DEFINE_FUNCTION(ai, getAIVectorFirstElement, 0x3644dd);
    DEFINE_FUNCTION(ai, getAIVectorGetCurrent, 0x364617);
    DEFINE_FUNCTION(ai, getAIVectorLength, 0x2cc10a);
    DEFINE_FUNCTION(ai, AC60AddOrGetEntity, 0x35dcc0);

    DEFINE_FUNCTION(ai, canFigureDoAction, 0x35ace7);
    DEFINE_FUNCTION(ai, setAICurrentActionRanking, 0x35e340);
    DEFINE_FUNCTION(ai, getAICurrentActionRanking, 0x362ed0);
    DEFINE_FUNCTION(ai, getActionStats, 0x35e0d4);
    DEFINE_FUNCTION(ai, getCastType, 0x26dcf0);
    DEFINE_FUNCTION(ai, getPositionToCastAlly, 0x35f197);
    DEFINE_FUNCTION(ai, getPositionToCastEnemy, 0x35b5e2);
    DEFINE_FUNCTION(ai, isUnknownWorldFeature, 0x34eae0);

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
    DEFINE_FUNCTION(spell, getSpellLine, 0x32a980);

    DEFINE_FUNCTION(effect, addEffect, 0x2dc880);
    DEFINE_FUNCTION(effect, setEffectXData, 0x2ddb30);
    DEFINE_FUNCTION(effect, getEffectXData, 0x2dd730);
    DEFINE_FUNCTION(effect, tryEndEffect, 0x2dcaa0);

    log_info("| - ToolboxAPI Hooks");

    DEFINE_FUNCTION(toolbox, dealDamage, 0x2f4a57);
    DEFINE_FUNCTION(toolbox, isTargetable, 0x2fe704);
    DEFINE_FUNCTION(toolbox, figuresCheckHostile, 0x2fe7b9);
    DEFINE_FUNCTION(toolbox, figuresCheckNeutral, 0x2feba4);
    DEFINE_FUNCTION(toolbox, figuresCheckFriendly, 0x2fe953);
    DEFINE_FUNCTION(toolbox, hasSpellOnIt, 0x2fe4ea);
    DEFINE_FUNCTION(toolbox, rescaleLevelStats, 0x2fff48);
    DEFINE_FUNCTION(toolbox, buildingDealDamage, 0x2d6d80);
    DEFINE_FUNCTION(toolbox, addSpellToFigure, 0x2f673a);
    DEFINE_FUNCTION(toolbox, getFigureFromWorld, 0x34F160);
    DEFINE_FUNCTION(toolbox, getSpellIndexOfType, 0x2fd939);
    DEFINE_FUNCTION(toolbox, getSpellIndexFromDLL, 0x2fdd90);
    DEFINE_FUNCTION(toolbox, getNextNode, 0x2fe240);
    DEFINE_FUNCTION(toolbox, figureSetNewJob, 0x2f0ef0);
    DEFINE_FUNCTION(toolbox, removeSpellFromList, 0x2fad57);
    DEFINE_FUNCTION(toolbox, addUnit, 0x2f749a);
    DEFINE_FUNCTION(toolbox, findClosestFreePosition, 0x34e9a0);
    DEFINE_FUNCTION(toolbox, isUnitMelee, 0x2feb2b);

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
    INCLUDE_FUNCTION(spell, addSpell, &sf_spelltype_hook);
    INCLUDE_FUNCTION(spell, getSpellID, &sf_get_spell_id);
    INCLUDE_FUNCTION(spell, checkCanApply, &sf_refresh_hook)
    INCLUDE_FUNCTION(spell, getSpellTags, &getSpellTags);
    INCLUDE_FUNCTION(spell, hasSpellTag, &hasSpellTag);

    log_info("| - FigureAPI Wrappers");
    INCLUDE_FUNCTION(figure, addBonusMultToStatistic, &addBonusMultToStatistic);

    log_info("| - ToolboxAPI Wrappers");
    INCLUDE_FUNCTION(toolbox, hasAuraActive, &hasAuraActive);

    log_info("| - IteratorAPI Wrappers");
    INCLUDE_FUNCTION(iterator, setupFigureIterator, &setupFigureIterator);
    INCLUDE_FUNCTION(iterator, disposeFigureIterator, &disposeFigureIterator);

    log_info("| - RegistrationAPI Wrappers");
    INCLUDE_FUNCTION(registration, registerSpell, &registerSpell);
    INCLUDE_FUNCTION(registration, linkTypeHandler, &linkTypeHandler);
    INCLUDE_FUNCTION(registration, linkEffectHandler, &linkEffectHandler);
    INCLUDE_FUNCTION(registration, linkEndHandler, &linkEndHandler);
    INCLUDE_FUNCTION(registration, linkOnHitHandler, &linkOnHitHandler);
    INCLUDE_FUNCTION(registration, applySpellTag, &applySpellTag);
    INCLUDE_FUNCTION(registration, linkSubEffectHandler, &linkSubEffectHandler);
    INCLUDE_FUNCTION(registration, linkRefreshHandler, &linkRefreshHandler);
    INCLUDE_FUNCTION(registration, linkDealDamageHandler, &linkDealDamageHandler);
    INCLUDE_FUNCTION(registration, linkAOEAIHandler, &linkAOEAIHandler);
    INCLUDE_FUNCTION(registration, linkAvoidanceAIHandler, &linkAvoidanceAIHandler);
    INCLUDE_FUNCTION(registration, linkSingleTargetAIHandler, &linkSingleTargetAIHandler);
}

static void initialize_spelltype_hook()
{
    ASI::MemoryRegion add_spell_mreg(ASI::AddrOf(0x328d60), 5);
    ASI::BeginRewrite(add_spell_mreg);
    *(unsigned char *)(ASI::AddrOf(0x328d60)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x328d61)) = (int)(&sf_spelltype_hook) - ASI::AddrOf(0x328d65);
    ASI::EndRewrite(add_spell_mreg);
}

static void initialize_spell_trigger_hook()
{
    ASI::MemoryRegion add_spell_mreg(ASI::AddrOf(0x278773), 5);
    ASI::BeginRewrite(add_spell_mreg);
    *(unsigned char *)(ASI::AddrOf(0x278773)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x278774)) = (int)(&sf_spelleffect_hook) - ASI::AddrOf(0x278778);
    ASI::EndRewrite(add_spell_mreg);
}

static void initialize_subeffect_add_hook()
{
    ASI::MemoryRegion add_spell_mreg(ASI::AddrOf(0x2de3b7), 5);
    ASI::BeginRewrite(add_spell_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2de3b7)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x2de3b8)) = (int)(&sf_subeffect_hook) - ASI::AddrOf(0x2de3bc);
    ASI::EndRewrite(add_spell_mreg);
}

static void initialize_spellend_hook()
{
    ASI::MemoryRegion end_spell_mreg(ASI::AddrOf(0x34b0a0), 5);
    ASI::BeginRewrite(end_spell_mreg);
    *(unsigned char *)(ASI::AddrOf(0x34b0a0)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x34b0a1)) = (int)(&sf_endspell_hook) - ASI::AddrOf(0x34b0a5);
    ASI::EndRewrite(end_spell_mreg);
}

static void initialize_spellrefresh_hook()
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

static void __declspec(naked) menuload_hook_beta()
{
    asm("push %%edi         \n\t"
        "call %P0           \n\t"
        "pop %%edi          \n\t"
        "jmp *%1            \n\t" : : "i"(sf_menu_hook), "o"(g_menu_return_addr));
}

static void initialize_menuload_hook()
{
    ASI::MemoryRegion menu_load_mreg(ASI::AddrOf(0x182794), 5);
    ASI::BeginRewrite(menu_load_mreg);
    *(unsigned char *)(ASI::AddrOf(0x182794)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x182795)) = (int)(&menuload_hook_beta) - ASI::AddrOf(0x182799);
    ASI::EndRewrite(menu_load_mreg);
}

static void initialize_deal_damage_hook()
{
    // uint32_t flag_pointer = *(uint32_t *)(ASI::AddrOf());
    g_damage_return_addr = ASI::AddrOf(0x2f5465);
    ASI::MemoryRegion deal_damage_mreg(ASI::AddrOf(0x2f4af3), 6);
    ASI::BeginRewrite(deal_damage_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2f4af3)) = 0x90; // nop trail
    *(unsigned char *)(ASI::AddrOf(0x2f4af4)) = 0xE9; // JMP
    *(int *)(ASI::AddrOf(0x2f4af5)) = (int)(&sf_damage_hook) - ASI::AddrOf(0x2f4af9);
}

static void initialize_onhit_hook()
{
    ASI::MemoryRegion onhit_mreg(ASI::AddrOf(0x2e0b00), 5);
    ASI::BeginRewrite(onhit_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2e0b00)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x2e0b01)) = (int)(&sf_onhit_hook) - ASI::AddrOf(0x2e0b05);
    ASI::EndRewrite(onhit_mreg);
}
static void initialize_ai_support_spell_hook()
{
    ASI::MemoryRegion ai_support_mreg(ASI::AddrOf(0x35d353), 5);
    ASI::BeginRewrite(ai_support_mreg);
    *(unsigned char *)(ASI::AddrOf(0x35d353)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x35d354)) = (int)(&rank_support_spell_hook) - ASI::AddrOf(0x35d358);
    ASI::EndRewrite(ai_support_mreg);
}

static void initialize_ai_offensive_hook()
{
    ASI::MemoryRegion ai_offensive_mreg(ASI::AddrOf(0x35d2f8), 5);
    ASI::BeginRewrite(ai_offensive_mreg);
    *(unsigned char *)(ASI::AddrOf(0x35d2f8)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x35d2f9)) = (int)(&rank_offensive_spell_hook) - ASI::AddrOf(0x35d2fd);
    ASI::EndRewrite(ai_offensive_mreg);

    ASI::MemoryRegion ai_offensive_mreg2(ASI::AddrOf(0x360fa6), 5);
    ASI::BeginRewrite(ai_offensive_mreg2);
    *(unsigned char *)(ASI::AddrOf(0x360fa6)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x360fa7)) = (int)(&rank_offensive_spell_hook) - ASI::AddrOf(0x360fab);
    ASI::EndRewrite(ai_offensive_mreg2);
}

static void initialize_ai_aoe_hook()
{
     ASI::MemoryRegion ai_offensive_mreg(ASI::AddrOf(0x35cb4f), 5);
    ASI::BeginRewrite(ai_offensive_mreg);
    *(unsigned char *)(ASI::AddrOf(0x35cb4f)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x35cb50)) = (int)(&ai_AOE_hook) - ASI::AddrOf(0x35cb54);
    ASI::EndRewrite(ai_offensive_mreg);

    ASI::MemoryRegion ai_offensive_mreg2(ASI::AddrOf(0x35cadc), 5);
    ASI::BeginRewrite(ai_offensive_mreg2);
    *(unsigned char *)(ASI::AddrOf(0x35cadc)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x35cadd)) = (int)(&ai_AOE_hook) - ASI::AddrOf(0x35cae1);
    ASI::EndRewrite(ai_offensive_mreg2);
}

static void initialize_avoidance_hook()
{
    ASI::MemoryRegion ai_avoidance_mreg(ASI::AddrOf(0x35d39e), 5);
    ASI::BeginRewrite(ai_avoidance_mreg);
    *(unsigned char *)(ASI::AddrOf(0x35d39e)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x35d39f)) = (int)(&avoidance_penalty_hook) - ASI::AddrOf(0x35d3a3);
    ASI::EndRewrite(ai_avoidance_mreg);
}
static void initialize_utility_hooks()
{
    log_info("Hooking Combat Ability Detection");
    ASI::MemoryRegion is_ability_line_mreg(ASI::AddrOf(0x32afb0), 5);
    ASI::BeginRewrite(is_ability_line_mreg);
    *(unsigned char *)(ASI::AddrOf(0x32afb0)) = 0xE9; // JUMP instruction
    *(int *)(ASI::AddrOf(0x32afb1)) = (int)(&is_combat_ability) - ASI::AddrOf(0x32afb5);
    ASI::EndRewrite(is_ability_line_mreg);

    log_info("Hooking AOE Spell Detection");
    ASI::MemoryRegion is_aoe_line_mreg(ASI::AddrOf(0x32ac90), 5);
    ASI::BeginRewrite(is_aoe_line_mreg);
    *(unsigned char *)(ASI::AddrOf(0x32ac90)) = 0xE9; // JUMP instruction
    *(int *)(ASI::AddrOf(0x32ac91)) = (int)(&is_aoe_spell) - ASI::AddrOf(0x32ac95);
    ASI::EndRewrite(is_aoe_line_mreg);

    log_info("Hooking Aura Detection");
    ASI::MemoryRegion is_aura_mreg(ASI::AddrOf(0x32ae20), 5);
    ASI::BeginRewrite(is_aura_mreg);
    *(unsigned char *)(ASI::AddrOf(0x32ae20)) = 0xE9; // JUMP instruction
    *(int *)(ASI::AddrOf(0x32ae21)) = (int)(&is_aura_spell) - ASI::AddrOf(0x32ae25);
    ASI::EndRewrite(is_aura_mreg);

    log_info("Hooking Black Aura Detection");
    ASI::MemoryRegion is_black_aura_mreg(ASI::AddrOf(0x32aec0), 5);
    ASI::BeginRewrite(is_black_aura_mreg);
    *(unsigned char *)(ASI::AddrOf(0x32aec0)) = 0xE9; // JUMP instruction
    *(int *)(ASI::AddrOf(0x32aec1)) = (int)(&is_black_aura_spell) - ASI::AddrOf(0x32aec5);
    ASI::EndRewrite(is_black_aura_mreg);

    log_info("Hooking White Aura Detection");
    ASI::MemoryRegion is_white_aura_mreg(ASI::AddrOf(0x32af00), 5);
    ASI::BeginRewrite(is_white_aura_mreg);
    *(unsigned char *)(ASI::AddrOf(0x32af00)) = 0xE9; // JUMP instruction
    *(int *)(ASI::AddrOf(0x32af01)) = (int)(&is_white_aura_spell) - ASI::AddrOf(0x32af05);
    ASI::EndRewrite(is_white_aura_mreg);

    log_info("Hooking Summon Spell Detection");
    ASI::MemoryRegion is_summon_spell_mreg(ASI::AddrOf(0x32b060), 5);
    ASI::BeginRewrite(is_summon_spell_mreg);
    *(unsigned char *)(ASI::AddrOf(0x32b060)) = 0xE9; // JUMP instruction
    *(int *)(ASI::AddrOf(0x32b061)) = (int)(&is_summon_spell) - ASI::AddrOf(0x32b065);
    ASI::EndRewrite(is_summon_spell_mreg);

    log_info("Hooking Domination Spell Detection");
    ASI::MemoryRegion is_domination_spell_mreg(ASI::AddrOf(0x32ac20), 5);
    ASI::BeginRewrite(is_domination_spell_mreg);
    *(unsigned char *)(ASI::AddrOf(0x32ac20)) = 0xE9; // JUMP instruction
    *(int *)(ASI::AddrOf(0x32ac21)) = (int)(&is_domination_spell) - ASI::AddrOf(0x32ac25);
    ASI::EndRewrite(is_domination_spell_mreg);

    log_info("Hooking Domination Spell Line Detection");
    ASI::MemoryRegion is_domination_spellline_mreg(ASI::AddrOf(0x32af60), 5);
    ASI::BeginRewrite(is_domination_spellline_mreg);
    *(unsigned char *)(ASI::AddrOf(0x32af60)) = 0xE9; // JUMP instruction
    *(int *)(ASI::AddrOf(0x32af61)) = (int)(&is_domination_spellline) - ASI::AddrOf(0x32af65);
    ASI::EndRewrite(is_domination_spellline_mreg);

    log_info("Temp Storm School Hook");
    ASI::MemoryRegion storm_test_mreg(ASI::AddrOf(0x7FFD38), 1);
    ASI::BeginRewrite(storm_test_mreg);
    *(unsigned char *)(ASI::AddrOf(0x7FFD38)) = 0x04;
    ASI::EndRewrite(storm_test_mreg);
}

void initialize_spell_buttons_hooks()
{
    ASI::MemoryRegion vfunction208_mreg_1 (ASI::AddrOf(0x5ebb23), 5);
    ASI::BeginRewrite(vfunction208_mreg_1);
    *(unsigned char *)(ASI::AddrOf(0x5ebb23)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x5ebb24)) = (int)(&sf_click_vertical_button) - ASI::AddrOf(0x5ebb28);
    ASI::EndRewrite(vfunction208_mreg_1);

    ASI::MemoryRegion vfunction208_mreg_2 (ASI::AddrOf(0x5ebb7b), 5);
    ASI::BeginRewrite(vfunction208_mreg_2);
    *(unsigned char *)(ASI::AddrOf(0x5ebb7b)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x5ebb7c)) = (int)(&sf_click_vertical_button) - ASI::AddrOf(0x5bb80);
    ASI::EndRewrite(vfunction208_mreg_2);
    
    /*
    log_info("Hooking CUiMain::FUN_009e5940 (Some kind of Render SubFunction that handles GUI Button Flashing)");
    ASI::MemoryRegion fun9e5940_mreg(ASI::AddrOf(0x5DEA96), 5);
    ASI::BeginRewrite(fun9e5940_mreg);
    *(unsigned char *)(ASI::AddrOf(0x5DEA96)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x5DEA97)) = (int)(&sf_handle_button_flashing_maybe) - ASI::AddrOf(0x5DEA9B);
    ASI::EndRewrite(fun9e5940_mreg);
    */
}

void initialize_beta_hooks()
{
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

    log_info("Hooking On Hit Trigger");
    initialize_onhit_hook();

    log_info("Hooking AI Support Spell Handling");
    initialize_ai_support_spell_hook();

    log_info("Hooking AI Offensive Spell Handling");
    initialize_ai_offensive_hook();

    log_info("Hooking AI Spell Avoidance Handling");
    initialize_avoidance_hook();

    initialize_ai_aoe_hook();
    
    log_info("Hooking Utility Functions");
    initialize_utility_hooks();

    log_info("Hooking spell buttons");
    initialize_spell_buttons_hooks();
}

/**
 * @}
 */