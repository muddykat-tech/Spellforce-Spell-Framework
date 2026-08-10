#pragma once

#include "../../../api/sfsf.h"
#include "../../../asi/sf_asi.h"

void initialize_worker_logic_data_hooks();
bool __thiscall onWoodcutterFinishJob(SF_CGdFigureJobs *_this, uint16_t figure_id);
bool __thiscall onMinerFinishJob(SF_CGdFigureJobs *_this, uint16_t figure_id);
bool __thiscall onStoneMinerFinishJob(SF_CGdFigureJobs *_this, uint16_t figure_id);
void __thiscall onCarpenterFinishJob(SF_CGdFigureJobs *_this, uint16_t figure_id);
