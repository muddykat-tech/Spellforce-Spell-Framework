#pragma once
#include "./structures/sf_building_structures.h"

/**
 * @file sf_building_functions.h
 * @ingroup API
 * @brief Functions for inspecting and manipulating buildings.
 * @see BuildingTag, SpellDataKey, sf_registration_functions.h
 */

/**
 * @brief Writes a value into a building's XData store.
 *
 * @param _this The building toolbox object.
 * @param building_index Index into the building array.
 * @param key_type The SpellDataKey slot to write.
 * @param value The value to store.
 */
DECLARE_FUNCTION(void, setBuildingXData, SF_CGdBuildingToolbox *_this, uint16_t building_index, uint8_t key_type,
                 uint32_t value);

/**
 * @brief Reads a value from a building's XData store.
 *
 * @param _this The building toolbox object.
 * @param building_index Index into the building array.
 * @param key_type The SpellDataKey slot to read.
 * @return The stored value, or 0 when the building has no XData key assigned.
 */
DECLARE_FUNCTION(uint32_t, getBuildingXData, SF_CGdBuildingToolbox *_this, uint16_t building_index, uint8_t key_type);

/**
 * @brief Attaches a unit into a tower
 *
 * @param _this The building toolbox object.
 * @param building_index Index of the tower / building.
 * @param unit_id The unit type to place inside, as a resource unit id.
 */
DECLARE_FUNCTION(void, attachTowerUnit, SF_CGdBuildingToolbox *_this, uint16_t building_index, uint16_t unit_id);

/**
 * @brief Applies damage to a building from a figure.
 *
 * @param _this The building toolbox object.
 * @param figure_id The figure credited as the damage source.
 * @param building_id The building taking the damage.
 * @param damage The damage amount to apply.
 * @param is_spell_damage Non-zero to attribute the damage to a spell. The
 *        pass 0 for physical
 */
DECLARE_FUNCTION(void, buildingDealDamage, SF_CGdBuildingToolbox *_this, uint16_t figure_id, uint16_t building_id,
                 uint16_t damage, uint32_t is_spell_damage);

/**
 * @brief Tests whether a figure and a building are allied.
 *
 * @param _this The building toolbox object.
 * @param figure_index The figure to test.
 * @param building_index The building to test against.
 * @return true when the two are allied.
 */
DECLARE_FUNCTION(bool, buildingCheckAllied, SF_CGdBuildingToolbox *_this, uint16_t figure_index,
                 uint16_t building_index);

/**
 * @brief Tests whether a figure and a building are hostile to each other.
 * @param _this The building toolbox object.
 * @param figure_index The figure to test.
 * @param building_index The building to test against.
 * @return true when the building is a hostile target for that figure.
 */
DECLARE_FUNCTION(bool, buildingCheckHostile, SF_CGdBuildingToolbox *_this, uint16_t figure_index,
                 uint16_t building_index);

/**
 * @brief Finds the point on a building's footprint nearest to a figure.
 *
 * Buildings occupy several tiles, so spell range and distance checks measure
 * against this vertex rather than the building's centre.
 *
 * @param _this The building toolbox object.
 * @param out_position Caller-supplied coordinate that receives the result.
 * @param building_index The building whose outline is measured.
 * @param figure_index The figure to measure towards.
 * @param unknown Purpose unknown; the framework passes 0.
 * @return out_position, for call chaining.
 */
DECLARE_FUNCTION(SF_Coord *, getBuildingClosestVertex, SF_CGdBuildingToolbox *_this, SF_Coord *out_position,
                 uint16_t building_index, uint16_t figure_index, uint32_t unknown);

/**
 * @brief Releases workers from a building.
 *
 * @param _this The building object.
 * @param building_id The building to release workers from.
 * @param count How many workers to release.
 * @return Purpose of the return value unverified; the framework ignores it.
 */
DECLARE_FUNCTION(uint8_t, releaseWorkers, SF_CGdBuilding *_this, uint16_t building_id, uint8_t count);

/**
 * @brief Clears a figure's assignment to whatever building currently holds it.
 *
 * @param _this The building toolbox object.
 * @param figure_id The figure to unassign.
 */
DECLARE_FUNCTION(void, freeAssignedWorker, SF_CGdBuildingToolbox *_this, uint16_t figure_id);

/**
 * @brief Adjusts a player's production rate for a resource category.
 *
 * @param _this The player object.
 * @param owner The player whose production changes.
 * @param race The race of the building doing the producing.
 * @param production_type The production category to adjust.
 * @param increase Non-zero to increase production, 0 to decrease.
 * @return Purpose of the return value unverified; the framework ignores it.
 */
DECLARE_FUNCTION(uint8_t, updateProduction, SF_CGdPlayer *_this, uint16_t owner, uint8_t race,
                 uint8_t production_type, uint8_t increase);

/**
 * @brief Tests whether a building type carries a given tag.
 *
 * @param building_type The building type to query.
 * @param tag The BuildingTag bit to test.
 * @return true when the type was registered with that tag.
 */
DECLARE_FUNCTION(bool, hasBuildingTag, uint8_t building_type, BuildingTag tag);

/**
 * @brief Returns the full tag bitmask registered for a building type.
 *
 * @param building_type The building type to query.
 * @return The BuildingTag bitmask, or 0 when the type is not registered.
 */
DECLARE_FUNCTION(uint32_t, getBuildingTags, uint8_t building_type);

/**
 * @brief Finds the nearest building of a given type belonging to a player.
 *
 * @param _this The building toolbox object.
 * @param position The position to search outwards from.
 * @param type The building type to look for.
 * @param owner The player the building must belong to.
 * @param radius The maximum search distance.
 * @return The index of the closest matching building, or 0 when none is in range.
 */
DECLARE_FUNCTION(uint16_t, findClosestBuilding, SF_CGdBuildingToolbox *_this, SF_Coord position, uint8_t type,
                 uint16_t owner, uint32_t radius);

/**
 * @brief Assigns a figure to a building, occupying one of its slots.
 *
 * @param _this The building toolbox object.
 * @param building_id The building to place the figure into.
 * @param figure_id The figure being assigned.
 * @param updateFlag Purpose unknown; the framework passes 0 at every call site.
 */
DECLARE_FUNCTION(void, addFigureToBuilding, SF_CGdBuildingToolbox *_this, uint16_t building_id, uint16_t figure_id,
                 bool updateFlag);

/**
 * @name Building type predicates
 *
 * @param _this The building object.
 * @param building_index Index into the building array.
 * @return true when the building carries the corresponding tag.
 * @{
 */
/** @brief True when the building is a scavenger hut. */
DECLARE_FUNCTION(bool, buildingIsScavenger, SF_CGdBuilding *_this, uint16_t building_index);
/** @brief True when the building is a fisher hut. */
DECLARE_FUNCTION(bool, buildingIsFisher, SF_CGdBuilding *_this, uint16_t building_index);
/** @brief True when the building stores food. */
DECLARE_FUNCTION(bool, buildingIsFoodstore, SF_CGdBuilding *_this, uint16_t building_index);
/** @brief True when the building is a forge. */
DECLARE_FUNCTION(bool, buildingIsForge, SF_CGdBuilding *_this, uint16_t building_index);
/** @brief True when the building is a gatherer hut. */
DECLARE_FUNCTION(bool, buildingIsGatherer, SF_CGdBuilding *_this, uint16_t building_index);
/** @brief True when the building is a headquarters of any size. */
DECLARE_FUNCTION(bool, buildingIsHQ, SF_CGdBuilding *_this, uint16_t building_index);
/** @brief True when the building is a hunter hut. */
DECLARE_FUNCTION(bool, buildingIsHunter, SF_CGdBuilding *_this, uint16_t building_index);
/** @brief True when the building mines iron, that is a miner without the moonsilver tag. */
DECLARE_FUNCTION(bool, buildingIsIronMine, SF_CGdBuilding *_this, uint16_t building_index);
/** @brief True when the building is a mace carver. */
DECLARE_FUNCTION(bool, buildingIsMaceCarver, SF_CGdBuilding *_this, uint16_t building_index);
/** @brief True when the building mines moonsilver, that is a miner carrying the moonsilver tag. */
DECLARE_FUNCTION(bool, buildingIsMoonsilverMine, SF_CGdBuilding *_this, uint16_t building_index);
/** @brief True when the building is a quarry. */
DECLARE_FUNCTION(bool, buildingIsQuarry, SF_CGdBuilding *_this, uint16_t building_index);
/** @brief True when the building is a sawmill. */
DECLARE_FUNCTION(bool, buildingIsSawmill, SF_CGdBuilding *_this, uint16_t building_index);
/** @brief True when the building is a smelter. */
DECLARE_FUNCTION(bool, buildingIsSmelter, SF_CGdBuilding *_this, uint16_t building_index);
/** @brief True when the building is a stonemason. */
DECLARE_FUNCTION(bool, buildingIsStonecutter, SF_CGdBuilding *_this, uint16_t building_index);
/** @brief True when the building is a temple. */
DECLARE_FUNCTION(bool, buildingIsTemple, SF_CGdBuilding *_this, uint16_t building_index);
/** @brief True when the building is a tower. @see attachTowerUnit */
DECLARE_FUNCTION(bool, buildingIsTower, SF_CGdBuilding *_this, uint16_t building_index);
/** @brief True when the building is a woodcutter. */
DECLARE_FUNCTION(bool, buildingIsWoodcutter, SF_CGdBuilding *_this, uint16_t building_index);
/** @brief True when the building is a shrine. */
DECLARE_FUNCTION(bool, buildingIsShrine, SF_CGdBuilding *_this, uint16_t building_index);
/** @brief True when the building can house workers. */
DECLARE_FUNCTION(bool, buildingIsHabitable, SF_CGdBuilding *_this, uint16_t building_index);
/** @brief True when the building houses exactly one worker. */
DECLARE_FUNCTION(bool, buildingIsHabitableSingle, SF_CGdBuilding *_this, uint16_t building_index);
/** @} */

/**
 * @name Racial building lookups
 * @param race The race to look up.
 * @return The building type id, or 0 when that race has no building with the tag.
 * @{
 */
/** @brief Returns the race's food storage building. */
DECLARE_FUNCTION(uint8_t, getRacialFoodstore, uint8_t race);
/** @brief Returns the race's smelter. */
DECLARE_FUNCTION(uint8_t, getRacialSmelter, uint8_t race);
/** @brief Returns the race's sawmill. */
DECLARE_FUNCTION(uint8_t, getRacialSawmill, uint8_t race);
/** @brief Returns the race's stonemason. */
DECLARE_FUNCTION(uint8_t, getRacialStonecutter, uint8_t race);
/** @brief Returns the race's woodcutter. */
DECLARE_FUNCTION(uint8_t, getRacialWoodcutter, uint8_t race);
/**
 * @brief Returns the race's iron mine.
 * @note REVIEW: The moonsilver exclusion in this lookup does not work. It tests
 * (tags & ~MOONSILVER_BUILDING), which is true for any building with any other
 * bit set, so a moonsilver mine is still returned. buildingIsIronMine() should be
 * used as a reference.
 */
DECLARE_FUNCTION(uint8_t, getRacialIronMine, uint8_t race);
/** @brief Returns the race's quarry. */
DECLARE_FUNCTION(uint8_t, getRacialQuarry, uint8_t race);
/** @} */
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
    buildingIsMoonsilverMine_ptr buildingIsMoonsilverMine;
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
    getBuildingXData_ptr getBuildingXData;
    /*
       getRacialSmallHQ_ptr getRacialSmallHQ;
       getRacialMediumHQ_ptr getRacialMediumHQ;
       getRacialLargeHQ_ptr getRacialLargeHQ;
     */
} BuildingFunctions;
