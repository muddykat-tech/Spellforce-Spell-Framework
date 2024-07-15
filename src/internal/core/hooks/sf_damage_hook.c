#include "sf_damage_hook.h"

#include "../sf_wrappers.h"
#include "../sf_hooks.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
uint32_t sf_damage_return_addr;

void __attribute__((no_caller_saved_registers, thiscall)) sf_deal_damage(SF_CGdFigureToolbox *figureToolbox, uint16_t dmg_source, uint16_t dmg_target, uint32_t damage_amount, uint32_t is_spell_damage, uint32_t param_5, uint32_t vry_unknown_6)
{


    /* rewrite this
      if ((((source != 0) && (iVar6 = CGdFigure::IsAlive(local_270->gd_figure,source), iVar6 != 0)) &&
      (iVar6 = HasSpellOnIt(local_270,source,0xa5), iVar6 != 0)) ||
     (iVar6 = HasSpellOnIt(local_270,target,0xa5), iVar6 != 0)) {
    local_29c = 1;
    }
    */


    /* call this
      iVar6 = CGdFigure::IsFlagSet(local_270->gd_figure,target,CHECK_SPELLS_BEFORE_JOB);
  if (iVar6 != 0) {
    uVar3 = CGdFigure::GetSpellJobStartNode(local_270->gd_figure,target);
    local_28c = CONCAT22(local_28c._2_2_,uVar3);
    */


   /*   here comes them handlers */

   //all code below is obsolete
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

    // Check if spell is applied
    bool check_spells_before_job = figureAPI.isFlagSet(figureToolbox->CGdFigure, dmg_target, F_CHECK_SPELLS_BEFORE_CHECK_BATTLE);
    if (check_spells_before_job)
    {
        // While loop to check for Damage Modifcations triggered from damage taken

        // Main Checks for Spell Cases... This would be where we may be able to implement a registry system of some sort.
        uint32_t job_start_node_index = figureAPI.getSpellJobStartNode(figureToolbox->CGdFigure, dmg_target);
        while (job_start_node_index != 0)
        {
            // spell_index = figureToolbox->data[job_start_node_index].spell_index;
            //  e get the spell index through CGdDoubleLinkList,
            job_start_node_index = 0; // TEMP, just to ensure no infinity loop
        }
    }
    else
    {
    }

    log_info("Called into Overwritten Damage Function");
}

void __declspec(naked) sf_damage_hook_beta()
{
    asm("push 0x08(%%ebp)        \n\t"
        "push 0x0с(%%ebp)        \n\t"
        "push 0x10(%%ebp)        \n\t"
        "push 0x14(%%ebp)        \n\t"
        "push 0x18(%%ebp)        \n\t"
        "push 0x1с(%%ebp)        \n\t"
        "mov 0x26c(%%ebp), %%ecx \n\t"
        "call %P0                \n\t"
        "jmp *%1                 \n\t" : : "i"(sf_deal_damage), "o"(sf_damage_return_addr));
}
