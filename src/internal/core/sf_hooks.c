/**
 * @defgroup Hooks Assembly Hooks
 * @ingroup Internal
 * @details This group contains all the hooks used in SFSF.
 * @see initialize_data_hooks()
 */

#include "sf_wrappers.h"
#include "sf_ui_wrappers.h"
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
#include "hooks/sf_vanilla_fix_hook.h"
#include "hooks/sf_building_done_hook.h"
#include "hooks/sf_building_entry_hook.h"
#include "hooks/sf_worker_logic_hook.h"
#include "hooks/sf_phys_effect_hook.h"
#include "hooks/sf_enchant_hook.h"

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
BuildingFunctions buildingAPI;
FigureFunctions figureAPI;
IteratorFunctions iteratorAPI;
UiFunctions uiAPI;
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

    log_debug (DEBUG_HIGH, "| - Spelltypes");

    initialize_menu_data_hooks();

    log_debug (DEBUG_HIGH, "| - Worker Logic");
    initialize_worker_logic_data_hooks();

    log_debug (DEBUG_HIGH, "| - Building Done");
    //TODO RENAME TO DATA HOOKS
    initialize_building_done_hooks();

    log_debug (DEBUG_HIGH, "| - Spelltypes");
    // Required for internal use
    initialize_spelltype_data_hooks();

    // Used in Iterator for AOE Spells Dispose
    log_debug (DEBUG_HIGH, "| - Wrapper Functions");
    initialize_wrapper_data_hooks();

    log_debug (DEBUG_HIGH, "| - On Hit");
    initialize_onhit_data_hooks();

    // Used to print to the game console
    log_debug (DEBUG_HIGH, "| - Game Console");
    initialize_console_data_hooks();

    log_debug (DEBUG_HIGH, "| - Vanilla Fixes");
    initialize_vanilla_fix_hooks();

    log_info("| - FigureAPI Hooks");
    // More defined for external use in api
    DEFINE_FUNCTION(figure, isAlive, 0x1BE4D0);
    DEFINE_FUNCTION(figure, setWalkSpeed, 0x2B7190);
    DEFINE_FUNCTION(figure, addAction, 0x2AE0B0);
    DEFINE_FUNCTION(figure, addBonusMult, 0x35A3E0);
    DEFINE_FUNCTION(figure, decreaseHealth, 0x2b5b40);
    DEFINE_FUNCTION(figure, rescaleMana, 0x2b5d50);
    DEFINE_FUNCTION(figure, rescaleHealth, 0x2b5cd0);
    DEFINE_FUNCTION(figure, getJob, 0x279290);
    DEFINE_FUNCTION(figure, FUN_006e3a90, 0x2e3a90);
    DEFINE_FUNCTION(figure, setJobToDoCount, 0x300910);
    DEFINE_FUNCTION(figure, isFlagSet, 0x279d20);
    DEFINE_FUNCTION(figure, getSpellJobStartNode, 0x2b2de0);
    DEFINE_FUNCTION(figure, subMana, 0x2b5b60);
    DEFINE_FUNCTION(figure, getPosition, 0x2caaf0);
    DEFINE_FUNCTION(figure, setTask, 0x2b7110);
    DEFINE_FUNCTION(figure, getWeaponStats, 0x2b30a0);
    DEFINE_FUNCTION(figure, getAggroValue, 0x2b1ab0);
    DEFINE_FUNCTION(figure, setAggroValue, 0x2b6670);
    DEFINE_FUNCTION(figure, isWarrior, 0x1c1510);
    DEFINE_FUNCTION(figure, getHealersList, 0x2b2870);
    DEFINE_FUNCTION(figure, disposeHealerList, 0x36436D);
    DEFINE_FUNCTION(figure, getCurrentHealthPercent, 0x2fdeb0);
    DEFINE_FUNCTION(figure, prepareJobTransition, 0x2df6e0);
    DEFINE_FUNCTION(figure, onStartJob, 0x2e2180);
    DEFINE_FUNCTION(figure, setJob, 0x2dee90);
    DEFINE_FUNCTION(figure, setFigureWalkPos, 0x2e3660);
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
    DEFINE_FUNCTION(spell, getLeveledSpellID, 0x26de20);

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
    DEFINE_FUNCTION(toolbox, setFigureXData, 0x2f7bb2);
    DEFINE_FUNCTION(toolbox, getFigureXData, 0x2fe442);
    DEFINE_FUNCTION(toolbox, equipArtisanArmour, 0x2f84c1);
    DEFINE_FUNCTION(toolbox, equipArtisanTools, 0x2fd783);
    DEFINE_FUNCTION(toolbox, findClosestMonument, 0x2bb380);

    log_info("| - BuildingAPI Hooks");
    DEFINE_FUNCTION(building, buildingDealDamage, 0x2d6d80);
    DEFINE_FUNCTION(building, buildingCheckAllied, 0x2da420);
    DEFINE_FUNCTION(building, buildingCheckHostile, 0x2da2c0);
    DEFINE_FUNCTION(building, getBuildingClosestVertex, 0x2d9c40);
    DEFINE_FUNCTION(building, setBuildingXData, 0x2daca0);
    DEFINE_FUNCTION(building, attachTowerUnit, 0x2d9290);
    DEFINE_FUNCTION(building, releaseWorkers, 0x2c6280);
    DEFINE_FUNCTION(building, freeAssignedWorker, 0x2d8a00);
    DEFINE_FUNCTION(building, updateProduction, 0x2a2b80);
    DEFINE_FUNCTION(building, findClosestBuilding, 0x2d97d0);
    DEFINE_FUNCTION(building, addFigureToBuilding, 0x2d6f00);

    log_info("| - IteratorAPI Hooks");
    DEFINE_FUNCTION(iterator, figureIteratorInit, 0x3183f0);
    DEFINE_FUNCTION(iterator, figureIteratorSetPointers, 0x31a680);
    DEFINE_FUNCTION(iterator, iteratorSetArea, 0x3195d0);
    DEFINE_FUNCTION(iterator, getNextFigure, 0x318f50);
    DEFINE_FUNCTION(iterator, buildingIteratorInit, 0x318290);
    DEFINE_FUNCTION(iterator, buildingIteratorSetPointers, 0x31A640);
    DEFINE_FUNCTION(iterator, getNextBuilding, 0x318ca0);

    log_info("| - uiAPI Hooks");
    DEFINE_FUNCTION(ui, setBaseAlpha, 0x512EB0);
    DEFINE_FUNCTION(ui, getVisualControl, 0x507BC0);
    DEFINE_FUNCTION(ui, setButtonName, 0x52f8a0);
    DEFINE_FUNCTION(ui, initializeMenuContainer, 0x505780);
    DEFINE_FUNCTION(ui, setupMenuContainerData, 0x50FD30);
    DEFINE_FUNCTION(ui, initializeSmpButton, 0x51a9d0);
    DEFINE_FUNCTION(ui, createButton, 0x52E1E0);
    DEFINE_FUNCTION(ui, setFont, 0x530C20);
    DEFINE_FUNCTION(ui, setButtonIndex, 0x5136A0);
    DEFINE_FUNCTION(ui, setMenuButtonFlag, 0x5308A0);
    DEFINE_FUNCTION(ui, attachCallback, 0x6188B0);
    DEFINE_FUNCTION(ui, vfunction16AttachCallback, 0x532B90);
    DEFINE_FUNCTION(ui, vfunction176, 0x52f520);
    DEFINE_FUNCTION(ui, vfunction25, 0x511AE0);
    DEFINE_FUNCTION(ui, CMnuBaseSetName, 0x512E30);
    DEFINE_FUNCTION(ui, destroyContainer, 0x512AB0);
    DEFINE_FUNCTION(ui, destroyButton, 0xB7B270);
    DEFINE_FUNCTION(ui, setMenuID, 0x50E660);
    DEFINE_FUNCTION(ui, setContainerVisible, 0x513910);
    DEFINE_FUNCTION(ui, setLabelColour, 0x530330);

    log_info("| - uiAPI Wrappers");
    INCLUDE_FUNCTION(ui, updateLabelText, &updateLabelText);
    INCLUDE_FUNCTION(ui, attachLabel, &attachLabel);
    INCLUDE_FUNCTION(ui, attachMeshedLabel, &attachMeshedLabel);
    INCLUDE_FUNCTION(ui, attachNewButton, &attachNewButton);
    INCLUDE_FUNCTION(ui, createContainer, &createContainer);


    log_info("| - SpellAPI Wrappers");
    // Method to include functions WE define in the Internal code.
    INCLUDE_FUNCTION(spell, initializeSpellData, &initializeSpellData);
    INCLUDE_FUNCTION(spell, spellClearFigureFlag, &spellClearFigureFlag);
    INCLUDE_FUNCTION(spell, addSpell, &sf_spelltype_hook);
    INCLUDE_FUNCTION(spell, getSpellID, &sf_get_spell_id);
    INCLUDE_FUNCTION(spell, checkCanApply, &sf_refresh_hook)
    INCLUDE_FUNCTION(spell, getSpellTags, &getSpellTags);
    INCLUDE_FUNCTION(spell, hasSpellTag, &hasSpellTag);

    INCLUDE_FUNCTION(effect, effectXDataExists, &XDataExists);
    log_info("| - FigureAPI Wrappers");
    INCLUDE_FUNCTION(figure, addBonusMultToStatistic, &addBonusMultToStatistic);

    INCLUDE_FUNCTION(figure, getCurrentStat, &getCurrentStat);
    INCLUDE_FUNCTION(figure, getMaxStat, &getMaxStat);
    INCLUDE_FUNCTION(figure, isSiegeUnit, &isSiegeUnit);

    log_info("| - ToolboxAPI Wrappers");
    INCLUDE_FUNCTION(toolbox, hasAuraActive, &hasAuraActive);
    INCLUDE_FUNCTION(toolbox, getPhysDamageReduction, &getPhysDamageReduction);
    INCLUDE_FUNCTION(toolbox, getDistance, &getDistance);

    log_info("| - IteratorAPI Wrappers");
    INCLUDE_FUNCTION(iterator, setupFigureIterator, &setupFigureIterator);
    INCLUDE_FUNCTION(iterator, disposeFigureIterator, &disposeFigureIterator);

    log_info("| - BuildingAPI Wrappers");
    INCLUDE_FUNCTION(building, getBuildingTags, &getBuildingTags);
    INCLUDE_FUNCTION(building, hasBuildingTag, &hasBuildingTag);

    INCLUDE_FUNCTION(building, getBuildingXData, &getBuildingXData);

    INCLUDE_FUNCTION(building, buildingIsScavenger, &buildingIsScavenger);
    INCLUDE_FUNCTION(building, buildingIsFisher, &buildingIsFisher);
    INCLUDE_FUNCTION(building, buildingIsFoodstore, &buildingIsFoodstore);
    INCLUDE_FUNCTION(building, buildingIsForge, &buildingIsForge);
    INCLUDE_FUNCTION(building, buildingIsGatherer, &buildingIsGatherer);
    INCLUDE_FUNCTION(building, buildingIsHQ, &buildingIsHQ);
    INCLUDE_FUNCTION(building, buildingIsHunter, &buildingIsHunter);
    INCLUDE_FUNCTION(building, buildingIsIronMine, &buildingIsIronMine);
    INCLUDE_FUNCTION(building, buildingIsMaceCarver, &buildingIsMaceCarver);
    INCLUDE_FUNCTION(building, buildingIsMoonsilverMine, &buildingIsMoonsilverMine);
    INCLUDE_FUNCTION(building, buildingIsQuarry, &buildingIsQuarry);
    INCLUDE_FUNCTION(building, buildingIsSawmill, &buildingIsSawmill);
    INCLUDE_FUNCTION(building, buildingIsSmelter, &buildingIsSmelter);
    INCLUDE_FUNCTION(building, buildingIsStonecutter, &buildingIsStonecutter);
    INCLUDE_FUNCTION(building, buildingIsTemple, &buildingIsTemple);
    INCLUDE_FUNCTION(building, buildingIsTower, &buildingIsTower);
    INCLUDE_FUNCTION(building, buildingIsWoodcutter, &buildingIsWoodcutter);
    INCLUDE_FUNCTION(building, buildingIsShrine, &buildingIsShrine);

    INCLUDE_FUNCTION(building, buildingIsHabitableSingle, &buildingIsHabitableSingle);
    INCLUDE_FUNCTION(building, buildingIsHabitable, &buildingIsHabitable);

    INCLUDE_FUNCTION(building, getRacialFoodstore, &getRacialFoodstore);
    INCLUDE_FUNCTION(building, getRacialSmelter, &getRacialSmelter);
    INCLUDE_FUNCTION(building, getRacialSawmill, &getRacialSawmill);
    INCLUDE_FUNCTION(building, getRacialStonecutter, &getRacialStonecutter);
    INCLUDE_FUNCTION(building, getRacialWoodcutter, &getRacialWoodcutter);
    INCLUDE_FUNCTION(building, getRacialIronMine, &getRacialIronMine);
    INCLUDE_FUNCTION(building, getRacialQuarry, &getRacialQuarry);

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
    INCLUDE_FUNCTION(registration, linkPhysEffectHandler, &linkPhysEffectHandler);
    INCLUDE_FUNCTION(registration, linkPhysRainHandler, &linkPhysRainHandler);
    INCLUDE_FUNCTION(registration, linkEnchantChanceHandler, &linkEnchantChanceHandler);

    INCLUDE_FUNCTION(registration, registerBuilding, &registerBuilding);
    INCLUDE_FUNCTION(registration, linkBuildingJSON, &linkBuildingJSON);
    INCLUDE_FUNCTION(registration, linkBuildingDoneHandler, &linkBuildingDoneHandler);
    INCLUDE_FUNCTION(registration, linkBuildingEntryHandler, &linkBuildingEntryHandler);
    INCLUDE_FUNCTION(registration, applyBuildingTag, &applyBuildingTag);

}

static void initialize_spelltype_hook()
{
    ASI::MemoryRegion add_spell_mreg (ASI::AddrOf(0x328d60), 5);
    ASI::BeginRewrite(add_spell_mreg);
    *(unsigned char *)(ASI::AddrOf(0x328d60)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x328d61)) = (int)(&sf_spelltype_hook) -
                                      ASI::AddrOf(0x328d65);
    ASI::EndRewrite(add_spell_mreg);
}

static void initialize_spell_trigger_hook()
{
    ASI::MemoryRegion add_spell_mreg (ASI::AddrOf(0x278773), 5);
    ASI::BeginRewrite(add_spell_mreg);
    *(unsigned char *)(ASI::AddrOf(0x278773)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x278774)) = (int)(&sf_spelleffect_hook) -
                                      ASI::AddrOf(0x278778);
    ASI::EndRewrite(add_spell_mreg);
}

static void initialize_building_done_hook()
{
    ASI::MemoryRegion done_mreg1 (ASI::AddrOf(0x2f05d3), 5);
    ASI::BeginRewrite(done_mreg1);
    *(unsigned char *)(ASI::AddrOf(0x2f05d3)) = 0xE8;  // CALL instruction
    *(int *)(ASI::AddrOf(0x2f05d4)) = (int)(&sf_building_done_hook) - ASI::AddrOf(0x2f05d8);
    ASI::EndRewrite(done_mreg1);

    ASI::MemoryRegion done_mreg2 (ASI::AddrOf(0x2d6d31), 5);
    ASI::BeginRewrite(done_mreg2);
    *(unsigned char *)(ASI::AddrOf(0x2d6d31)) = 0xE8;  // CALL instruction
    *(int *)(ASI::AddrOf(0x2d6d32)) = (int)(&sf_building_done_hook) - ASI::AddrOf(0x2d6d36);
    ASI::EndRewrite(done_mreg2);

    ASI::MemoryRegion done_mreg3 (ASI::AddrOf(0x27a65c), 5);
    ASI::BeginRewrite(done_mreg3);
    *(unsigned char *)(ASI::AddrOf(0x27a65c)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x27a65d)) = (int)(&sf_building_done_hook) - ASI::AddrOf(0x27a661);
    ASI::EndRewrite(done_mreg3);
}

static void initialize_building_entry_hook()
{
    ASI::MemoryRegion done_mreg1 (ASI::AddrOf(0x2f16f7), 5);
    ASI::BeginRewrite(done_mreg1);
    *(unsigned char *)(ASI::AddrOf(0x2f16f7)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x2f16f8)) = (int)(&job_start_working_at_building_hook) - ASI::AddrOf(0x2f16fc);
    ASI::EndRewrite(done_mreg1);
}


static void initialize_subeffect_add_hook()
{
    ASI::MemoryRegion add_spell_mreg (ASI::AddrOf(0x2de3b7), 5);
    ASI::BeginRewrite(add_spell_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2de3b7)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x2de3b8)) = (int)(&sf_subeffect_hook) - ASI::AddrOf(0x2de3bc);
    ASI::EndRewrite(add_spell_mreg);
/*
    ASI::MemoryRegion add_spell_mreg2 (ASI::AddrOf(0x2de3ed), 5);
    ASI::BeginRewrite(add_spell_mreg2);
 *(unsigned char *)(ASI::AddrOf(0x2de3ed)) = 0xE8; // CALL instruction
 *(int *)(ASI::AddrOf(0x2de3ee)) = (int)(&sf_phys_effect_hook) - ASI::AddrOf(0x2de3f2);
    ASI::EndRewrite(add_spell_mreg2);
 */
}

static void initialize_spellend_hook()
{
    ASI::MemoryRegion end_spell_mreg (ASI::AddrOf(0x34b0a0), 5);
    ASI::BeginRewrite(end_spell_mreg);
    *(unsigned char *)(ASI::AddrOf(0x34b0a0)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x34b0a1)) = (int)(&sf_endspell_hook) - ASI::AddrOf(0x34b0a5);
    ASI::EndRewrite(end_spell_mreg);
}

static void initialize_spellrefresh_hook()
{
    ASI::MemoryRegion refresh_spell_mreg (ASI::AddrOf(0x329f90), 9);
    ASI::BeginRewrite(refresh_spell_mreg);
    *(unsigned char *)(ASI::AddrOf(0x329f90)) = 0x90; // nop trail
    *(unsigned char *)(ASI::AddrOf(0x329f91)) = 0x90; // nop trail
    *(unsigned char *)(ASI::AddrOf(0x329f92)) = 0x90; // nop trail
    *(unsigned char *)(ASI::AddrOf(0x329f93)) = 0x90; // nop trail
    *(unsigned char *)(ASI::AddrOf(0x329f94)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x329f95)) = (int)(&sf_refresh_hook) -
                                      ASI::AddrOf(0x329f99);
    ASI::EndRewrite(refresh_spell_mreg);
}

static void __declspec(naked) menuload_hook_beta()
{
    asm ("push %%edi         \n\t"  // Storing a pointer to CAppMenu
         "call %P0           \n\t"  // Calling the Hook Function (this also calls the function we replace)
         "pop %%edi          \n\t"  // Restoring EDI, then we jump back to the address of the original function that we interrupted.
         "jmp *%1            \n\t" : : "i" (sf_menu_hook),
         "o" (g_menu_return_addr));
}
/*
   static void __declspec(naked) ui_overlay_hook()
   {
    asm ("pop %%eax                 \n\t"       // Pop EAX to clean it for our use case
         "push -0x184(%%ebp)        \n\t"       // Push Figure ID into our hook
         "push %%eax                \n\t"       // Push Spell ID into our hook
         "push %%ecx                \n\t"       // Push CGDResource into our hook
         "mov -0x1d4(%%ebp), %%ecx  \n\t"       // CGDFigure is moved into our hook
         "call %P0                  \n\t"       // Call our Hook
         "push %%eax                \n\t"       // Put Correct Spell ID from out Hook to the stack
         "lea -0x120(%%ebp),%%eax   \n\t"       // Assign the EAX to original stack variable.
         "mov 0x10(%%edi),%%ecx     \n\t"       // Restore ECX to CGDResource
         "jmp *%1                   \n\t" : :   // Jump back to control flow
         "i" (sf_ui_overlay_fix),
         "o" (g_ui_hook_fix_addr));
   }

   static void __declspec(naked) ui_overlay_hook2()
   {
    asm ("pop %%eax                 \n\t"       // Pop EAX to clean it for our use case
         "push -0x184(%%ebp)        \n\t"       // Push Figure ID into our hook
         "push %%eax                \n\t"       // Push Spell ID into our hook
         "push %%ecx                \n\t"       // Push CGDResource into our hook
         "mov -0x1d4(%%ebp), %%ecx  \n\t"       // CGDFigure is moved into our hook
         "call %P0                  \n\t"       // Call our Hook
         "push %%eax                \n\t"       // Put Correct Spell ID from out Hook to the stack
         "lea -0x120(%%ebp),%%eax   \n\t"       // Assign the EAX to original stack variable.
         "mov 0x10(%%edi),%%ecx     \n\t"       // Restore ECX to CGDResource
         "jmp *%1                   \n\t" : :   // Jump back to control flow
         "i" (sf_ui_overlay_fix),
         "o" (g_ui_hook_fix_addr2));
   }

   static void __declspec(naked) ui_enchant_chance_hook()
   {
    asm ("nop                      \n\t"
         "call %P0                 \n\t"       // Call our Hook
         "jmp *%1                  \n\t" : :  // Jump back to control flow
         "i" (sf_enchant_hook),
         "o" (g_sf_enchant_addr));
   }

   static void __declspec(naked) ui_enchant_chance_hook2()
   {
    asm ("nop                      \n\t"
         "call %P0                 \n\t"       // Call our Hook
         "jmp *%1                  \n\t" : :  // Jump back to control flow
         "i" (sf_enchant_hook),
         "o" (g_sf_enchant_addr2));
   }
 */


static void initialize_ui_overlay_fix_hook()
{
    ASI::MemoryRegion ui_load_mreg (ASI::AddrOf(0x647b9c), 5);
    ASI::BeginRewrite(ui_load_mreg);
    *(uint8_t *)(ASI::AddrOf(0x647b9c)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x647b9d)) = (int)(&portrait_overlay_hook) - ASI::AddrOf(0x647ba1);
    ASI::EndRewrite(ui_load_mreg);

    //just skipping percentage include via cmp eax eax
    ASI::MemoryRegion inv_tt_mreg (ASI::AddrOf(0x5d274c), 5);
    ASI::BeginRewrite(inv_tt_mreg);
    *(uint8_t *)(ASI::AddrOf(0x5d274c)) = 0x90;  //Nop Trail
    *(uint8_t *)(ASI::AddrOf(0x5d274d)) = 0x90;  //Nop Trail
    *(uint8_t *)(ASI::AddrOf(0x5d274e)) = 0x90;  //Nop Trail
    *(uint8_t *)(ASI::AddrOf(0x5d274f)) = 0x39;  // CMP EAX,
    *(uint8_t *)(ASI::AddrOf(0x5d2750)) = 0xC0;  // EAX
    ASI::EndRewrite(inv_tt_mreg);

    ASI::MemoryRegion get_name_mreg (ASI::AddrOf(0x5670cb), 5);
    ASI::BeginRewrite(get_name_mreg);
    *(uint8_t *)(ASI::AddrOf(0x5670cb)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x5670cc)) = (int)(&getSpecialFigureName) - ASI::AddrOf(0x5670d0);
    ASI::EndRewrite(get_name_mreg);
}

static void initialize_menuload_hook()
{
    ASI::MemoryRegion menu_load_mreg (ASI::AddrOf(0x182794), 5);
    ASI::BeginRewrite(menu_load_mreg);
    *(unsigned char *)(ASI::AddrOf(0x182794)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x182795)) = (int)(&menuload_hook_beta) - ASI::AddrOf(0x182799);
    ASI::EndRewrite(menu_load_mreg);
}

static void initialize_deal_damage_hook()
{
    // uint32_t flag_pointer = *(uint32_t *)(ASI::AddrOf());
    g_damage_return_addr = ASI::AddrOf(0x2f5465);
    ASI::MemoryRegion deal_damage_mreg (ASI::AddrOf(0x2f4af3), 6);
    ASI::BeginRewrite(deal_damage_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2f4af3)) = 0x90; // nop trail
    *(unsigned char *)(ASI::AddrOf(0x2f4af4)) = 0xE9; // JMP
    *(int *)(ASI::AddrOf(0x2f4af5)) = (int)(&sf_damage_hook) - ASI::AddrOf(0x2f4af9);
}

static void initialize_onhit_hook()
{
    ASI::MemoryRegion onhit_mreg (ASI::AddrOf(0x2e0b00), 5);
    ASI::BeginRewrite(onhit_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2e0b00)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x2e0b01)) = (int)(&sf_onhit_hook) - ASI::AddrOf(0x2e0b05);
    ASI::EndRewrite(onhit_mreg);
}
static void initialize_ai_support_spell_hook()
{
    ASI::MemoryRegion ai_support_mreg (ASI::AddrOf(0x35d353), 5);
    ASI::BeginRewrite(ai_support_mreg);
    *(unsigned char *)(ASI::AddrOf(0x35d353)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x35d354)) = (int)(&rank_support_spell_hook) -
                                      ASI::AddrOf(0x35d358);
    ASI::EndRewrite(ai_support_mreg);
}

static void initialize_ai_offensive_hook()
{
    ASI::MemoryRegion ai_offensive_mreg (ASI::AddrOf(0x35d2f8), 5);
    ASI::BeginRewrite(ai_offensive_mreg);
    *(unsigned char *)(ASI::AddrOf(0x35d2f8)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x35d2f9)) = (int)(&rank_offensive_spell_hook) -
                                      ASI::AddrOf(0x35d2fd);
    ASI::EndRewrite(ai_offensive_mreg);

    ASI::MemoryRegion ai_offensive_mreg2 (ASI::AddrOf(0x360fa6), 5);
    ASI::BeginRewrite(ai_offensive_mreg2);
    *(unsigned char *)(ASI::AddrOf(0x360fa6)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x360fa7)) = (int)(&rank_offensive_spell_hook) -
                                      ASI::AddrOf(0x360fab);
    ASI::EndRewrite(ai_offensive_mreg2);
}

static void initialize_ai_aoe_hook()
{
    ASI::MemoryRegion ai_offensive_mreg (ASI::AddrOf(0x35cb4f), 5);
    ASI::BeginRewrite(ai_offensive_mreg);
    *(unsigned char *)(ASI::AddrOf(0x35cb4f)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x35cb50)) = (int)(&ai_AOE_hook) - ASI::AddrOf(0x35cb54);
    ASI::EndRewrite(ai_offensive_mreg);

    ASI::MemoryRegion ai_offensive_mreg2 (ASI::AddrOf(0x35cadc), 5);
    ASI::BeginRewrite(ai_offensive_mreg2);
    *(unsigned char *)(ASI::AddrOf(0x35cadc)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x35cadd)) = (int)(&ai_AOE2_hook) - ASI::AddrOf(0x35cae1);
    ASI::EndRewrite(ai_offensive_mreg2);
}

static void initialize_avoidance_hook()
{
    ASI::MemoryRegion ai_avoidance_mreg (ASI::AddrOf(0x35d39e), 5);
    ASI::BeginRewrite(ai_avoidance_mreg);
    *(unsigned char *)(ASI::AddrOf(0x35d39e)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x35d39f)) = (int)(&avoidance_penalty_hook) -
                                      ASI::AddrOf(0x35d3a3);
    ASI::EndRewrite(ai_avoidance_mreg);
}
static void initialize_utility_hooks()
{
    log_info("Hooking Combat Ability Detection");
    ASI::MemoryRegion is_ability_line_mreg (ASI::AddrOf(0x32afb0), 5);
    ASI::BeginRewrite(is_ability_line_mreg);
    *(unsigned char *)(ASI::AddrOf(0x32afb0)) = 0xE9; // JUMP instruction
    *(int *)(ASI::AddrOf(0x32afb1)) = (int)(&is_combat_ability) - ASI::AddrOf(0x32afb5);
    ASI::EndRewrite(is_ability_line_mreg);

    log_info("Hooking AOE Spell Detection");
    ASI::MemoryRegion is_aoe_line_mreg (ASI::AddrOf(0x32ac90), 5);
    ASI::BeginRewrite(is_aoe_line_mreg);
    *(unsigned char *)(ASI::AddrOf(0x32ac90)) = 0xE9; // JUMP instruction
    *(int *)(ASI::AddrOf(0x32ac91)) = (int)(&is_aoe_spell) - ASI::AddrOf(0x32ac95);
    ASI::EndRewrite(is_aoe_line_mreg);

    log_info("Hooking Aura Detection");
    ASI::MemoryRegion is_aura_mreg (ASI::AddrOf(0x32ae20), 5);
    ASI::BeginRewrite(is_aura_mreg);
    *(unsigned char *)(ASI::AddrOf(0x32ae20)) = 0xE9; // JUMP instruction
    *(int *)(ASI::AddrOf(0x32ae21)) = (int)(&is_aura_spell) - ASI::AddrOf(0x32ae25);
    ASI::EndRewrite(is_aura_mreg);

    log_info("Hooking Black Aura Detection");
    ASI::MemoryRegion is_black_aura_mreg (ASI::AddrOf(0x32aec0), 5);
    ASI::BeginRewrite(is_black_aura_mreg);
    *(unsigned char *)(ASI::AddrOf(0x32aec0)) = 0xE9; // JUMP instruction
    *(int *)(ASI::AddrOf(0x32aec1)) = (int)(&is_black_aura_spell) - ASI::AddrOf(0x32aec5);
    ASI::EndRewrite(is_black_aura_mreg);

    log_info("Hooking White Aura Detection");
    ASI::MemoryRegion is_white_aura_mreg (ASI::AddrOf(0x32af00), 5);
    ASI::BeginRewrite(is_white_aura_mreg);
    *(unsigned char *)(ASI::AddrOf(0x32af00)) = 0xE9; // JUMP instruction
    *(int *)(ASI::AddrOf(0x32af01)) = (int)(&is_white_aura_spell) - ASI::AddrOf(0x32af05);
    ASI::EndRewrite(is_white_aura_mreg);

    log_info("Hooking Summon Spell Detection");
    ASI::MemoryRegion is_summon_spell_mreg (ASI::AddrOf(0x32b060), 5);
    ASI::BeginRewrite(is_summon_spell_mreg);
    *(unsigned char *)(ASI::AddrOf(0x32b060)) = 0xE9; // JUMP instruction
    *(int *)(ASI::AddrOf(0x32b061)) = (int)(&is_summon_spellline) - ASI::AddrOf(0x32b065);
    ASI::EndRewrite(is_summon_spell_mreg);

    log_info("Hooking Domination Spell Detection");
    ASI::MemoryRegion is_domination_spell_mreg (ASI::AddrOf(0x32ac20), 5);
    ASI::BeginRewrite(is_domination_spell_mreg);
    *(unsigned char *)(ASI::AddrOf(0x32ac20)) = 0xE9;  // JUMP instruction
    *(int *)(ASI::AddrOf(0x32ac21)) = (int)(&is_domination_spell) - ASI::AddrOf(0x32ac25);
    ASI::EndRewrite(is_domination_spell_mreg);

    log_info("Hooking Domination Spell Line Detection");
    ASI::MemoryRegion is_domination_spellline_mreg (ASI::AddrOf(0x32af60), 5);
    ASI::BeginRewrite(is_domination_spellline_mreg);
    *(unsigned char *)(ASI::AddrOf(0x32af60)) = 0xE9;  // JUMP instruction
    *(int *)(ASI::AddrOf(0x32af61)) = (int)(&is_domination_spellline) - ASI::AddrOf(0x32af65);
    ASI::EndRewrite(is_domination_spellline_mreg);

    // log_info("Temp Storm School Hook");
    // ASI::MemoryRegion storm_test_mreg (ASI::AddrOf(0x7FFD38), 1);
    // ASI::BeginRewrite(storm_test_mreg);
    // *(unsigned char *)(ASI::AddrOf(0x7FFD38)) = 0x04;
    // ASI::EndRewrite(storm_test_mreg);
}

static void initialize_spell_buttons_hooks()
{

    ASI::MemoryRegion vfunction208_mreg_1 (ASI::AddrOf(0x5ebb23), 5);
    ASI::BeginRewrite(vfunction208_mreg_1);
    *(unsigned char *)(ASI::AddrOf(0x5ebb23)) = 0xE8;   // CALL instruction
    *(int *)(ASI::AddrOf(0x5ebb24)) = (int)(&sf_click_vertical_button) - ASI::AddrOf(0x5ebb28);
    ASI::EndRewrite(vfunction208_mreg_1);

    ASI::MemoryRegion vfunction208_mreg_2 (ASI::AddrOf(0x5ebb7b), 5);
    ASI::BeginRewrite(vfunction208_mreg_2);
    *(unsigned char *)(ASI::AddrOf(0x5ebb7b)) = 0xE8;   // CALL instruction
    *(int *)(ASI::AddrOf(0x5ebb7c)) = (int)(&sf_click_vertical_button) - ASI::AddrOf(0x5bb80);
    ASI::EndRewrite(vfunction208_mreg_2);

    ASI::MemoryRegion  vfunction210_mreg(ASI::AddrOf(0x5ed94a), 5);
    ASI::BeginRewrite(vfunction210_mreg);
    *(unsigned char *)(ASI::AddrOf(0x5ed94a)) = 0xE8;  // JUMP instruction
    *(int *)(ASI::AddrOf(0x5ed94b)) = (int)(&sf_click_horizontal_button) - ASI::AddrOf(0x5ed94f);
    ASI::EndRewrite(vfunction210_mreg);

}

static void initialize_worker_logic_hooks()
{
    ASI::MemoryRegion woodcutter_mreg1 (ASI::AddrOf(0x2e68db), 5);
    ASI::BeginRewrite(woodcutter_mreg1);
    *(unsigned char *)(ASI::AddrOf(0x2e68db)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x2e68dc)) = (int)(&onWoodcutterFinishJob) - ASI::AddrOf(0x2e68e0);
    ASI::EndRewrite(woodcutter_mreg1);

    ASI::MemoryRegion woodcutter_mreg2 (ASI::AddrOf(0x2ef28d), 5);
    ASI::BeginRewrite(woodcutter_mreg2);
    *(unsigned char *)(ASI::AddrOf(0x2ef28d)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x2ef28e)) = (int)(&onWoodcutterFinishJob) - ASI::AddrOf(0x2ef292);
    ASI::EndRewrite(woodcutter_mreg2);

    ASI::MemoryRegion miner_mreg1 (ASI::AddrOf(0x2ec588), 5);
    ASI::BeginRewrite(miner_mreg1);
    *(unsigned char *)(ASI::AddrOf(0x2ec588)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x2ec589)) = (int)(&onMinerFinishJob) - ASI::AddrOf(0x2ec58d);
    ASI::EndRewrite(miner_mreg1);

    ASI::MemoryRegion miner_mreg2 (ASI::AddrOf(0x2e5d0b), 5);
    ASI::BeginRewrite(miner_mreg2);
    *(unsigned char *)(ASI::AddrOf(0x2e5d0b)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x2e5d0c)) = (int)(&onMinerFinishJob) - ASI::AddrOf(0x2e5d10);
    ASI::EndRewrite(miner_mreg2);

    ASI::MemoryRegion stone_miner_mreg1 (ASI::AddrOf(0x2ee55d), 5);
    ASI::BeginRewrite(stone_miner_mreg1);
    *(unsigned char *)(ASI::AddrOf(0x2ee55d)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x2ee55e)) = (int)(&onStoneMinerFinishJob) - ASI::AddrOf(0x2ee562);
    ASI::EndRewrite(stone_miner_mreg1);

    ASI::MemoryRegion stone_miner_mreg2 (ASI::AddrOf(0x2e6427), 5);
    ASI::BeginRewrite(stone_miner_mreg2);
    *(unsigned char *)(ASI::AddrOf(0x2e6427)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x2e6428)) = (int)(&onStoneMinerFinishJob) - ASI::AddrOf(0x2e642c);
    ASI::EndRewrite(stone_miner_mreg2);

    ASI::MemoryRegion carpenter_mreg(ASI::AddrOf(0x2f1849), 5);
    ASI::BeginRewrite(carpenter_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2f1849)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x2f184a)) = (int)(&onCarpenterFinishJob) - ASI::AddrOf(0x2f184e);
    ASI::EndRewrite(carpenter_mreg);


    ASI::MemoryRegion isWoodCutter_mreg (ASI::AddrOf(0x2f1dab), 5);
    ASI::BeginRewrite(isWoodCutter_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2f1dab)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x2f1dac)) = (int)(&buildingIsQuarry) - ASI::AddrOf(0x2f1db0);
    ASI::EndRewrite(isWoodCutter_mreg);

    ASI::MemoryRegion isQuarry_mreg (ASI::AddrOf(0x2f0afb), 5);
    ASI::BeginRewrite(isQuarry_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2f0afb)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x2f0afc)) = (int)(&buildingIsQuarry) - ASI::AddrOf(0x2f0b00);
    ASI::EndRewrite(isQuarry_mreg);

    ASI::MemoryRegion isIronMine_mreg (ASI::AddrOf(0x2efc83), 5);
    ASI::BeginRewrite(isIronMine_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2efc83)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x2efc84)) = (int)(&buildingIsIronMine) - ASI::AddrOf(0x2efc88);
    ASI::EndRewrite(isIronMine_mreg);

    ASI::MemoryRegion isIronMine_mreg2 (ASI::AddrOf(0x2f02a1), 5);
    ASI::BeginRewrite(isIronMine_mreg2);
    *(unsigned char *)(ASI::AddrOf(0x2f02a1)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x2f02a2)) = (int)(&buildingIsIronMine) - ASI::AddrOf(0x2f02a6);
    ASI::EndRewrite(isIronMine_mreg2);


    ASI::MemoryRegion isSawmill_mreg (ASI::AddrOf(0x2ef82d), 5);
    ASI::BeginRewrite(isSawmill_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2ef82d)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x2ef82e)) = (int)(&buildingIsSawmill) - ASI::AddrOf(0x2ef832);
    ASI::EndRewrite(isSawmill_mreg);

    ASI::MemoryRegion isStoneMason_mreg (ASI::AddrOf(0x2eeb2d), 5);
    ASI::BeginRewrite(isStoneMason_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2eeb2d)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x2eeb2e)) = (int)(&buildingIsStonecutter) - ASI::AddrOf(0x2eeb32);
    ASI::EndRewrite(isStoneMason_mreg);

    ASI::MemoryRegion isSmelterMason_mreg (ASI::AddrOf(0x2ec022), 5);
    ASI::BeginRewrite(isSmelterMason_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2ec022)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x2ec023)) = (int)(&buildingIsSmelter) - ASI::AddrOf(0x2ec027);
    ASI::EndRewrite(isSmelterMason_mreg);

    ASI::MemoryRegion isMoonsilverMine_mreg(ASI::AddrOf(0x2efd57), 5);
    ASI::BeginRewrite(isMoonsilverMine_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2efd57)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x2efd58)) = (int)(&buildingIsMoonsilverMine) - ASI::AddrOf(0x2efd5c);
    ASI::EndRewrite(isMoonsilverMine_mreg);

    ASI::MemoryRegion isMoonsilverMine_mreg2(ASI::AddrOf(0x2f02b7), 5);
    ASI::BeginRewrite(isMoonsilverMine_mreg2);
    *(unsigned char *)(ASI::AddrOf(0x2f02b8)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x2f02b8)) = (int)(&buildingIsMoonsilverMine) - ASI::AddrOf(0x2f02bc);
    ASI::EndRewrite(isMoonsilverMine_mreg2);

    ASI::MemoryRegion isHabitable_mreg(ASI::AddrOf(0x2c7600), 7);
    ASI::BeginRewrite(isHabitable_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2c7600)) = 0xE9; // JUMP instruction
    *(int *)(ASI::AddrOf(0x2c7601)) = (int)(&buildingIsHabitable) - ASI::AddrOf(0x2c7605);
    *(unsigned char *)(ASI::AddrOf(0x2c7605)) = 0x90;
    *(unsigned char *)(ASI::AddrOf(0x2c7606)) = 0x90;
    ASI::EndRewrite(isHabitable_mreg);

    ASI::MemoryRegion isHabitableS_mreg(ASI::AddrOf(0x2c7530), 7);
    ASI::BeginRewrite(isHabitableS_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2c7530)) = 0xE9; // JUMP instruction
    *(int *)(ASI::AddrOf(0x2c7531)) = (int)(&buildingIsHabitableSingle) - ASI::AddrOf(0x2c7535);
    *(unsigned char *)(ASI::AddrOf(0x2c7535)) = 0x90;
    *(unsigned char *)(ASI::AddrOf(0x2c7536)) = 0x90;
    ASI::EndRewrite(isHabitableS_mreg);

    ASI::MemoryRegion isTower_mreg(ASI::AddrOf(0x2c71b0), 7);
    ASI::BeginRewrite(isTower_mreg);
    *(unsigned char *)(ASI::AddrOf(0x2c71b0)) = 0xE9; // JUMP instruction
    *(int *)(ASI::AddrOf(0x2c71b1)) = (int)(&buildingIsTower) - ASI::AddrOf(0x2c71b5);
    *(unsigned char *)(ASI::AddrOf(0x2c71b5)) = 0x90;
    *(unsigned char *)(ASI::AddrOf(0x2c71b6)) = 0x90;
    ASI::EndRewrite(isTower_mreg);

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

    log_info("Hook UI Overlay to Fix Descriptions");
    initialize_ui_overlay_fix_hook();

    log_info("Hook AI AOE Handling");
    initialize_ai_aoe_hook();

    log_info ("Hooking Building Done Handling");
    initialize_building_done_hook();

    log_info ("Hooking Building Entry Handling");
    initialize_building_entry_hook();

    log_info ("Hooking Worker Logic");
    initialize_worker_logic_hooks();

    log_info("Hooking Utility Functions");
    initialize_utility_hooks();

    log_info("Hooking spell buttons");
    initialize_spell_buttons_hooks();

}

/**
 * @}
 */
