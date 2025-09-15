#pragma once
#include "../../../api/sfsf.h"
#include "../../../asi/sf_asi.h"

void __thiscall sf_building_done_hook(SF_CGdBuildingToolbox *_this, uint16_t building_index);
void initialize_building_done_hooks();
