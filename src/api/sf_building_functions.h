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
DECLARE_FUNCTION(bool, hasBuildingTag, uint8_t building_type, BuildingTag tag);
DECLARE_FUNCTION(uint32_t, getBuildingTags, uint8_t building_type);
DECLARE_FUNCTION(uint16_t, findClosestBuilding, SF_CGdBuildingToolbox *_this, SF_Coord position, uint8_t type,
                 uint16_t owner, uint32_t radius);
DECLARE_FUNCTION(void, addFigureToBuilding, SF_CGdBuildingToolbox *_this,uint16_t building_id, uint16_t figure_id,
                 bool updateFlag);

DECLARE_FUNCTION(bool, buildingIsScavenger, SF_CGdBuilding *_this, uint16_t building_index);
DECLARE_FUNCTION(bool, buildingIsFisher, SF_CGdBuilding *_this, uint16_t building_index);
DECLARE_FUNCTION(bool, buildingIsFoodstore, SF_CGdBuilding *_this, uint16_t building_index);
DECLARE_FUNCTION(bool, buildingIsForge, SF_CGdBuilding *_this, uint16_t building_index);
DECLARE_FUNCTION(bool, buildingIsGatherer, SF_CGdBuilding *_this, uint16_t building_index);
DECLARE_FUNCTION(bool, buildingIsHQ, SF_CGdBuilding *_this, uint16_t building_index);
DECLARE_FUNCTION(bool, buildingIsHunter, SF_CGdBuilding *_this, uint16_t building_index);
DECLARE_FUNCTION(bool, buildingIsIronMine, SF_CGdBuilding *_this, uint16_t building_index);
DECLARE_FUNCTION(bool, buildingIsMaceCarver, SF_CGdBuilding *_this, uint16_t building_index);
DECLARE_FUNCTION(bool, buildingIsMoonsiverMine, SF_CGdBuilding *_this, uint16_t building_index);
DECLARE_FUNCTION(bool, buildingIsQuarry, SF_CGdBuilding *_this, uint16_t building_index);
DECLARE_FUNCTION(bool, buildingIsSawmill, SF_CGdBuilding *_this, uint16_t building_index);
DECLARE_FUNCTION(bool, buildingIsSmelter, SF_CGdBuilding *_this, uint16_t building_index);
DECLARE_FUNCTION(bool, buildingIsStonecutter, SF_CGdBuilding *_this, uint16_t building_index);
DECLARE_FUNCTION(bool, buildingIsTemple, SF_CGdBuilding *_this, uint16_t building_index);
DECLARE_FUNCTION(bool, buildingIsTower, SF_CGdBuilding *_this, uint16_t building_index);
DECLARE_FUNCTION(bool, buildingIsWoodcutter, SF_CGdBuilding *_this, uint16_t building_index);
DECLARE_FUNCTION(bool, buildingIsShrine, SF_CGdBuilding *_this, uint16_t building_index);
DECLARE_FUNCTION(bool, buildingIsHabitable, SF_CGdBuilding *_this, uint16_t building_index);
DECLARE_FUNCTION(bool, buildingIsHabitableSingle, SF_CGdBuilding *_this, uint16_t building_index);

DECLARE_FUNCTION(uint8_t, getRacialFoodstore, uint8_t race);
DECLARE_FUNCTION(uint8_t, getRacialSmelter, uint8_t race);
DECLARE_FUNCTION(uint8_t, getRacialSawmill, uint8_t race);
DECLARE_FUNCTION(uint8_t, getRacialStonecutter, uint8_t race);
DECLARE_FUNCTION(uint8_t, getRacialWoodcutter, uint8_t race);
DECLARE_FUNCTION(uint8_t, getRacialIronMine, uint8_t race);
DECLARE_FUNCTION(uint8_t, getRacialQuarry, uint8_t race);
/*
   DECLARE_FUNCTION(uint8_t, getRacialSmallHQ, uint8_t race);
   DECLARE_FUNCTION(uint8_t, getRacialMediumHQ, uint8_t race);
   DECLARE_FUNCTION(uint8_t, getRacialLargeHQ, uint8_t race);
 */
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
    getBuildingTags_ptr getBuildingTags;
    hasBuildingTag_ptr hasBuildingTag;
    buildingIsScavenger_ptr buildingIsScavenger;
    buildingIsFisher_ptr buildingIsFisher;
    buildingIsFoodstore_ptr buildingIsFoodstore;
    buildingIsForge_ptr buildingIsForge;
    buildingIsGatherer_ptr buildingIsGatherer;
    buildingIsHQ_ptr buildingIsHQ;
    buildingIsHunter_ptr buildingIsHunter;
    buildingIsIronMine_ptr buildingIsIronMine;
    buildingIsMaceCarver_ptr buildingIsMaceCarver;
    buildingIsMoonsiverMine_ptr buildingIsMoonsiverMine;
    buildingIsQuarry_ptr buildingIsQuarry;
    buildingIsSawmill_ptr buildingIsSawmill;
    buildingIsSmelter_ptr buildingIsSmelter;
    buildingIsStonecutter_ptr buildingIsStonecutter;
    buildingIsTemple_ptr buildingIsTemple;
    buildingIsTower_ptr buildingIsTower;
    buildingIsWoodcutter_ptr buildingIsWoodcutter;
    buildingIsShrine_ptr buildingIsShrine;
    getRacialFoodstore_ptr getRacialFoodstore;
    getRacialSmelter_ptr getRacialSmelter;
    getRacialSawmill_ptr getRacialSawmill;
    getRacialStonecutter_ptr getRacialStonecutter;
    getRacialWoodcutter_ptr getRacialWoodcutter;
    getRacialIronMine_ptr getRacialIronMine;
    getRacialQuarry_ptr getRacialQuarry;
    findClosestBuilding_ptr findClosestBuilding;
    addFigureToBuilding_ptr addFigureToBuilding;
    buildingIsHabitable_ptr buildingIsHabitable;
    buildingIsHabitableSingle_ptr buildingIsHabitableSingle;
    /*
       getRacialSmallHQ_ptr getRacialSmallHQ;
       getRacialMediumHQ_ptr getRacialMediumHQ;
       getRacialLargeHQ_ptr getRacialLargeHQ;
     */
} BuildingFunctions;
