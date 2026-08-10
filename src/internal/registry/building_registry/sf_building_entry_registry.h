#pragma once

#include "../../../api/sfsf.h"
#include "../../core/sf_wrappers.h"
#include "../../core/sf_modloader.h"


void registerBuildingEntryHandler(uint8_t building_type, building_entry_handler_ptr handler);
building_entry_handler_ptr get_building_entry_handler(uint8_t building_type);
