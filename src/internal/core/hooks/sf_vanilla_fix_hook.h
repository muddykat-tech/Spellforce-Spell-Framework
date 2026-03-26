#pragma once

#include "../../../api/sfsf.h"
#include "../../../asi/sf_asi.h"

SF_String * __thiscall portrait_overlay_hook(AutoClass101 *_this, SF_String *param_1, uint16_t figure_id,
                                             uint32_t param3,uint32_t param4, uint32_t param5);

uint16_t __thiscall get_figure_statistic_max_mp(SF_CGdFigure *_this, uint16_t figure_id);
uint16_t __thiscall get_figure_statistic_max_sta(SF_CGdFigure *_this, uint16_t figure_id);
uint16_t __thiscall get_figure_statistic_max_hp(SF_CGdFigure *_this, uint16_t figure_id);

uint16_t __thiscall get_figure_statistic_current_ac(SF_CGdFigure *_this, uint16_t figure_id);
uint16_t __thiscall get_figure_statistic_current_agi(SF_CGdFigure *_this, uint16_t figure_id);
uint16_t __thiscall get_figure_statistic_current_cha(SF_CGdFigure *_this, uint16_t figure_id);
uint16_t __thiscall get_figure_statistic_current_dex(SF_CGdFigure *_this, uint16_t figure_id);
uint16_t __thiscall get_figure_statistic_current_hp(SF_CGdFigure *_this, uint16_t figure_id);
uint16_t __thiscall get_figure_statistic_current_int(SF_CGdFigure *_this, uint16_t figure_id);
uint16_t __thiscall get_figure_statistic_current_mp(SF_CGdFigure *_this, uint16_t figure_id);
uint16_t __thiscall get_figure_statistic_current_sta(SF_CGdFigure *_this, uint16_t figure_id);
uint16_t __thiscall get_figure_statistic_current_str(SF_CGdFigure *_this, uint16_t figure_id);
uint16_t __thiscall get_figure_statistic_current_wis(SF_CGdFigure *_this, uint16_t figure_id);
uint16_t __thiscall get_figure_statistic_current_fire_res(SF_CGdFigure *_this, uint16_t figure_id);
uint16_t __thiscall get_figure_statistic_current_ice_res(SF_CGdFigure *_this, uint16_t figure_id);
uint16_t __thiscall get_figure_statistic_current_mental_res(SF_CGdFigure *_this, uint16_t figure_id);
uint16_t __thiscall get_figure_statistic_current_black_res(SF_CGdFigure *_this, uint16_t figure_id);
uint16_t __thiscall get_figure_statistic_current_walk_spd(SF_CGdFigure *_this, uint16_t figure_id);
uint16_t __thiscall get_figure_statistic_current_fight_spd(SF_CGdFigure *_this, uint16_t figure_id);
uint16_t __thiscall get_figure_statistic_current_cast_spd(SF_CGdFigure *_this, uint16_t figure_id);
void initialize_vanilla_fix_hooks();
