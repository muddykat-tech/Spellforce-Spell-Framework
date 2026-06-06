#pragma once

#include "../../../api/sfsf.h"
#include "../../core/sf_wrappers.h"
#include "../../core/sf_modloader.h"


void registerBuildingDoneHandler(uint8_t building_type, building_done_handler_ptr handler);
building_done_handler_ptr get_building_done_handler(uint8_t building_type);
