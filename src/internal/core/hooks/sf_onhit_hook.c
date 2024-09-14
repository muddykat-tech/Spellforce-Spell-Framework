#include "sf_onhit_hook.h"
#include "../sf_wrappers.h"
#include "../sf_hooks.h"
#include "../sf_modloader.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void __thiscall getTargetData(AutoClass24 * _this, SF_CGdTargetData *target)
{
    target->entity_type = _this->target.entity_type;
    target->entity_index = _this->target.entity_index;
    target->position = _this->target.position;
}

void __thiscall sf_onhit_hook(SF_CGdFigureJobs *_this, uint16_t figure_id, uint32_t param3, uint8_t param4)
{
    if ((_this->CGdFigure->figures[figure_id].owner == -1) || (_this->CGdFigure->figures[figure_id].flags & 10))
    {
        return;
    }
    SF_CGdFigureWeaponStats weapon_stats;
    SF_CGdTargetData target;
    SF_SGtFigureAction action;
    figureAPI.getWeaponStats(_this->CGdFigure, &weapon_stats, figure_id);
    uint16_t weapon_damage = spellAPI.getRandom(_this->OpaqueClass, weapon_stats.max_dmg - weapon_stats.min_rng);
    weapon_damage += weapon_stats.min_dmg;
    getTargetData(&_this->CGdFigure->figures[figure_id].ac_1, &target);
    figureAPI.getTargetAction(_this->CGdFigure, &action, figure_id);
    if (target.entity_type == 1)
    {
        //counter hits set
        bool apply_counter = false;
        if (_this->CGdFigure->figures[target.entity_index].set_type == 0x03)
        {
            uint16_t counter = spellAPI.getRandom(_this->OpaqueClass, 100);
            apply_counter = (counter < 0x0b);
        }
        if (_this->CGdFigure->figures[target.entity_index].flags & F_CHECK_SPELLS_BEFORE_JOB != 0)
        {
              //riposte handler
                
        }
    }
}
