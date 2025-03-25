#pragma once
#include <stdint.h>
#include "../../src/api/sfsf.h"


typedef AutoClass24 *(__thiscall *getAutoClass24_ptr)(SF_CGdFigure *_this, uint16_t figure_id);
typedef uint32_t(__thiscall *FUN_006f8c06_ptr)(SF_CGdFigureToolbox *_this, uint16_t param_1, uint16_t param_2, uint32_t param_3, uint8_t param_4,
    SF_Coord param_5, uint32_t param_6, uint32_t param_7, uint32_t param_8);
typedef void(__thiscall *FUN_006fa3ce_ptr)(SF_CGdFigureToolbox *_this, uint16_t param_1, uint32_t param_2);
typedef void(__thiscall *setFigureAction_ptr)(AutoClass24 *_this, SF_SGtFigureAction *action);
