#include "sf_damage_hook.h"

#include "../sf_wrappers.h"
#include "../sf_hooks.h"

#include "../../registry/sf_spelldamage_registry.h"
#include "../../../api/sf_general_structures.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

uint32_t g_damage_return_addr;

void __attribute__((no_caller_saved_registers, thiscall)) sf_deal_damage(SF_CGdFigureToolbox *figureToolbox, uint16_t dmg_source, uint16_t dmg_target, uint32_t damage_amount, uint32_t is_spell_damage, uint32_t param_5, uint32_t vry_unknown_6)
{

    log_info("Called into Overwritten Damage Function Start");
    /* TODO - rewrite this for later use after spell damage function call
      if ((((source != 0) && (iVar6 = CGdFigure::IsAlive(local_270->gd_figure,source), iVar6 != 0)) &&
        (iVar6 = HasSpellOnIt(local_270,source,0xa5), iVar6 != 0)) ||
        (iVar6 = HasSpellOnIt(local_270,target,0xa5), iVar6 != 0)) {
        local_29c = 1;
      }
    */

    bool check_spells_before_job = figureAPI.isFlagSet(figureToolbox->CGdFigure, dmg_target, F_CHECK_SPELLS_BEFORE_JOB);

    log_info("Sanity Check 1");
    bool figure_set_new_job = false;
    if (check_spells_before_job)
    {
        uint16_t spell_job_start_node = figureAPI.getSpellJobStartNode(figureToolbox->CGdFigure, dmg_target);
        uint16_t current_list_size = 0;

        uint16_t ids_by_phase[COUNT][799];
        size_t sizes_by_phase[COUNT] = {0};

        while (spell_job_start_node != 0)
        {
            uint16_t spell_index = toolboxAPI.getSpellIndexFromDLL(figureToolbox->CGdDoubleLinkedList, spell_job_start_node);
            uint16_t spell_line_id = spellAPI.getSpellLine(figureToolbox->CGdSpell, spell_index);
            // collect spell_line_id & spell_index here to list

            for (int i = PRE; i < COUNT; ++i)
            {
                SpellDamagePhase phase = (SpellDamagePhase)i;
                damage_handler_ptr exists = get_spell_damage(spell_line_id, phase);
                if (exists != NULL)
                {
                    ids_by_phase[phase][sizes_by_phase[phase]++] = spell_line_id;
                    current_list_size++;
                }
            }
            spell_job_start_node = toolboxAPI.getNextNode(figureToolbox->CGdDoubleLinkedList, spell_job_start_node);
        }

        for (int i = PRE; i < COUNT; ++i)
        {
            SpellDamagePhase phase = (SpellDamagePhase)i;
            char phase_data[256];
            sprintf(phase_data, "Phase: %d, Size: %d", phase, sizes_by_phase[phase]);
            log_info(phase_data);

            for (size_t i = 0; i < sizes_by_phase[phase]; ++i)
            {
                uint16_t spell_line_id = ids_by_phase[phase][i];
                damage_handler_ptr spell_damage_func = get_spell_damage(spell_line_id, phase);
                damage_amount = spell_damage_func(figureToolbox, dmg_source, dmg_target, damage_amount, is_spell_damage, param_5, vry_unknown_6);
            }
        }
    }

    log_info("Called into Overwritten Damage Function End");
}

void __declspec(naked) sf_damage_hook()
{
    asm("push 0x08(%%ebp)          \n\t"
        "push 0x0c(%%ebp)          \n\t"
        "push 0x10(%%ebp)          \n\t"
        "push 0x14(%%ebp)          \n\t"
        "push 0x18(%%ebp)          \n\t"
        "push 0x1c(%%ebp)          \n\t"
        "mov  -0x26c(%%ebp), %%ecx \n\t"
        "call %P0                  \n\t"
        "movw %%ax, 0x10(%%ebp)    \n\t"
        "jmp *%1                     " : : "i"(sf_deal_damage), "o"(g_damage_return_addr));
}
