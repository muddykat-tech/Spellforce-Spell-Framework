#ifndef SF_DAMAGE_HOOK_H
#define SF_DAMAGE_HOOK_H

#include "../../../api/sfsf.h"
#include "../../../asi/sf_asi.h"

void __thiscall sf_damage_hook(SF_CGdFigureToolbox *figureToolbox, uint16_t dmg_source, uint16_t dmg_target, uint32_t damage_amount, uint32_t is_spell_damage, uint32_t param_5, uint32_t vry_unknown_6);

#endif