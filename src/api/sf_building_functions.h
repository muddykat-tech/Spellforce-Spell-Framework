#pragma once
#include "./structures/sf_building_structures.h"

DECLARE_FUNCTION(void, setBuildingXData, SF_CGdBuildingToolbox *_this, uint16_t building_index, uint8_t xdatakey,
                 uint32_t value);
DECLARE_FUNCTION(void, attachTowerUnit, SF_CGdBuildingToolbox *_this, uint16_t building_index, uint16_t unit_id);
DECLARE_FUNCTION(void, buildingDealDamage, SF_CGdBuildingToolbox *_this, uint16_t figure_id, uint16_t building_id,
                 uint16_t damage, uint32_t is_spell_damage);
DECLARE_FUNCTION(bool, buildingCheckAllied, SF_CGdBuildingToolbox *_this, uint16_t figure_index,
                 uint16_t building_index);
DECLARE_FUNCTION(bool, buildingCheckHostile, SF_CGdBuildingToolbox *_this, uint16_t figure_index,
                 uint16_t building_index);
DECLARE_FUNCTION(SF_Coord *, getBuildingClosestVertex, SF_CGdBuildingToolbox *_this, SF_Coord *out_position,
                 uint16_t building_index, uint16_t figure_index, uint32_t unknown);
DECLARE_FUNCTION(uint8_t, releaseWorkers, SF_CGdBuilding* _this, uint16_t building_id, uint8_t count);
DECLARE_FUNCTION(void, freeAssignedWorker, SF_CGdBuildingToolbox *_this, uint16_t figure_id);
DECLARE_FUNCTION(uint8_t, updateProduction, SF_CGdPlayer *_this, uint16_t owner, uint8_t race,
                 uint8_t production_type, uint8_t increase);


typedef struct
{
    setBuildingXData_ptr setBuildingXData;
    attachTowerUnit_ptr attachTowerUnit;
    buildingDealDamage_ptr buildingDealDamage;
    buildingCheckAllied_ptr buildingCheckAllied;
    buildingCheckHostile_ptr buildingCheckHostile;
    getBuildingClosestVertex_ptr getBuildingClosestVertex;
    releaseWorkers_ptr releaseWorkers;
    freeAssignedWorker_ptr freeAssignedWorker;
    updateProduction_ptr updateProduction;
} BuildingFunctions;
