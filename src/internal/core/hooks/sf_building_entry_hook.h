#pragma once
#include "../../../api/sfsf.h"
#include "../../../api/sf_general_structures.h"
#include "../../../asi/sf_asi.h"

#include "../sf_hooks.h"
#include "../../registry/building_registry/sf_building_entry_registry.h"

void __thiscall job_start_working_at_building_hook (SF_CGdFigureJobs *_this, uint16_t figure_id);
