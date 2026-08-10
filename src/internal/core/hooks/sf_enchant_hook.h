#pragma once
#include "../../../api/sfsf.h"
#include "../../../asi/sf_asi.h"

uint32_t __attribute__((no_caller_saved_registers, thiscall)) sf_enchant_hook(SF_CGdFigure *_this, uint16_t figure_id,
                                                                              uint16_t spell_line_id);
