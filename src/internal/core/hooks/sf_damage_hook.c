#include "sf_damage_hook.h"

#include "../sf_wrappers.h"
#include "../sf_hooks.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void __thiscall sf_damage_hook(SF_CGdFigureToolbox *figureToolbox, uint16_t dmg_source, uint16_t dmg_target, uint32_t damage_amount, uint32_t is_spell_damage, uint32_t param_5, uint32_t vry_unknown_6)
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