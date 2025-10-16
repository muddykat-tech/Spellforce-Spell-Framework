#pragma once
#include "../../api/sfsf.h"
#include "../core/sf_hooks.h"
#include "../../api/sf_general_structures.h"

void __thiscall default_entry_handler(SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id);
void __thiscall building_repair(SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id);
void __thiscall hq_entry_handler(SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id);
void __thiscall woodcutter_entry_handler (SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id);
void __thiscall sawmill_entry_handler (SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id);
void __thiscall cattle_breeder_entry_handler (SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id);
void __thiscall quarry_entry_handler (SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id);
void __thiscall foodstore_entry_handler (SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id);
void __thiscall hunter_entry_handler (SF_CGdFigureJobs *_this, uint16_t figure_id, uint16_t building_id);
void __thiscall fisher_entry_handler (SF_CGdFigureJobs *_this, uint16_t figure_id, uint16_t building_id);
void __thiscall farmer_entry_handler (SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id);
void __thiscall miner_entry_handler (SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id);
void __thiscall smelter_entry_handler (SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id);
void __thiscall shrine_entry_handler (SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id);
void __thiscall temple_entry_handler (SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id);
void __thiscall forester_entry_handler (SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id);
void __thiscall gatherer_entry_handler (SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id);
void __thiscall forge_entry_handler (SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id);
void __thiscall mace_carver_entry_handler (SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id);
void __thiscall scavenger_entry_handler (SF_CGdFigureJobs *_this, uint16_t figure_id, uint16_t building_id);
