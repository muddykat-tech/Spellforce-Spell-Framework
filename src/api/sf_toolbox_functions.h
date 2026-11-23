#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "sf_general_structures.h"
#include "./structures/sf_toolbox_structures.h"

// Declare the function pointers for the ToolboxFunctions group

/**
 * dealDamage - Applies damage to a figure, running defensive abilities.
 * @param is_ranged: Physical projectile flag consumed by defensive
 *        abilities: endurance/durability mitigate only when 0 (and not
 *        spell damage), reflect shields trigger only when 0, steelskin
 *        fully blocks only when 1. Framework passes 1 from projectile/rain
 *        effects, 0 elsewhere.
 * @param damage_event_param: Forwarded as the last dword of the floating
 *        damage-text events (AutoClass30 types 5/0x1f/0x19); likely the
 *        number style/color. Framework always passes 0.
 */
DECLARE_FUNCTION(void, dealDamage, SF_CGdFigureToolbox *CGdFigureToolbox, uint16_t source_index, uint16_t target_index,
                 uint32_t damage, uint32_t is_spell_damage, uint32_t is_ranged, uint32_t damage_event_param);
DECLARE_FUNCTION(bool, isTargetable, SF_CGdFigureToolbox *CGdFigureToolbox, uint16_t figure_index);
DECLARE_FUNCTION(uint16_t, figuresCheckHostile, SF_CGdFigureToolbox *CGdFigureToolbox, uint16_t source_index,
                 uint16_t target_index);
DECLARE_FUNCTION(uint16_t, figuresCheckNeutral, SF_CGdFigureToolbox *CGdFigureToolbox, uint16_t source_index,
                 uint16_t target_index);
DECLARE_FUNCTION(uint16_t, figuresCheckFriendly,
                 SF_CGdFigureToolbox *CGdFigureToolbox, uint16_t source_index,
                 uint16_t target_index);
DECLARE_FUNCTION(uint32_t, hasSpellOnIt, SF_CGdFigureToolbox *CGdFigureToolbox,
                 uint16_t figure_index, uint16_t spell_line_id);
DECLARE_FUNCTION(void, rescaleLevelStats, SF_CGdFigureToolbox *CGdFigureToolbox,
                 uint16_t figure_index);
DECLARE_FUNCTION(void, attachSpellToFigure, SF_CGdFigureToolbox *CGdFigureToolbox,
                 uint16_t figure_index, uint16_t spell_index);
/**
 * getFigureFromWorld - Finds a figure standing on a map cell.
 * @param pos_x: X coordinate of the cell.
 * @param pos_y: Y coordinate of the cell.
 * @param include_dead: Non-zero returns any figure on the cell; zero skips
 *        figures carrying the IS_DEAD flag (GdFigureFlags 0x8).
 * Return: Figure index, 0 when none matched.
 */
DECLARE_FUNCTION(uint16_t, getFigureFromWorld, SF_CGdWorldToolBox *CGdWorldToolBox,
                 uint16_t pos_x, uint16_t pos_y, uint32_t include_dead);
DECLARE_FUNCTION(uint16_t, getSpellIndexOfType,
                 SF_CGdFigureToolbox *CGdFigureToolbox, uint16_t target_index,
                 uint16_t spell_line, uint16_t last_known_index);
DECLARE_FUNCTION(uint16_t, getSpellIndexFromDLL, uint32_t *CGdDoubleLinkedList,
                 uint16_t spell_job_start_node);
DECLARE_FUNCTION(uint16_t, getNextNode, uint32_t *CGdDoubleLinkedList,
                 uint16_t current_node);
/**
 * figureSetNewJob - Interrupts the current job and assigns a new one.
 * @param interrupt_current_job: Non-zero allows leaving the current job
 *        (also forwarded to the old-job cleanup handlers); zero rejects
 *        the switch for most jobs unless force_switch is set. Combat jobs
 *        in the engine pass 1.
 * @param force_switch: Non-zero overrides the rejection when the old-job
 *        cleanup did not confirm; engine passes 1 only for kGdJobDie.
 * @param cleanup_context: Extra argument forwarded to some old-job cleanup
 *        handlers; 0 for combat jobs.
 */
DECLARE_FUNCTION(void, figureSetNewJob, SF_CGdFigureJobs *CGdFigureJobs, uint32_t figure_id,
                 uint32_t new_job, uint32_t interrupt_current_job, uint32_t force_switch,
                 uint32_t cleanup_context); // @SHUFFLE Move to Figure Functions
/**
 * removeSpellFromList - Removes a spell node from a figure's active spell
 * list.
 * @param figure_id: Figure whose list is modified.
 * @param spell_index: Spell index (node value) to remove.
 * Return: Non-zero when the node was found and removed.
 */
DECLARE_FUNCTION(uint32_t, removeSpellFromList,
                 SF_CGdFigureToolbox *CGdFigureToolbox, uint16_t figure_id,
                 uint16_t spell_index);
DECLARE_FUNCTION(uint16_t, addUnit, SF_CGdFigureToolbox *_this, uint16_t posX,
                 uint16_t posY, uint16_t owner, uint16_t figure_type,
                 uint16_t spawn_effect, int npc_id, int faction,
                 int disable_ai);
/**
 * findClosestFreePosition - Spiral search for the closest walkable free
 * cell around a point.
 * @param center_pos: Point the spiral offsets are added to.
 * @param search_radius: Per-axis search extents in tiles (X and Y).
 * Return: Non-zero when a free cell was found.
 */
DECLARE_FUNCTION(bool, findClosestFreePosition, SF_CGdWorldToolBox *_this,
                 SF_Coord *center_pos, SF_Coord *search_radius,
                 uint16_t sector, SF_Coord *return_value)
DECLARE_FUNCTION(bool, isUnitMelee, SF_CGdFigureToolbox *CGdFigureToolbox,
                 uint16_t figure_id);

DECLARE_FUNCTION(bool, hasAuraActive, SF_CGdFigureToolbox *_this,
                 uint16_t figure_id);

DECLARE_FUNCTION(uint16_t, getPhysDamageReduction, SF_CGdFigureToolbox *_this, uint16_t source_index,
                 uint16_t target_index,  uint16_t action_id);

DECLARE_FUNCTION(uint32_t, getFigureXData, SF_CGdFigureToolbox *_this, uint16_t figure_id, uint8_t xdata_type);
DECLARE_FUNCTION(void, setFigureXData, SF_CGdFigureToolbox *_this, uint16_t figure_id, uint8_t xdata_type,
                 uint32_t value);

DECLARE_FUNCTION(void, equipArtisanArmour, SF_CGdFigureToolbox *_this, uint16_t figure_id);
/**
 * equipArtisanTools - Equips a worker figure from the race equipment cache.
 * @param tool_index: Index into the per-race worker equipment cache (22
 *        entries per race) written to equipment slot 1.
 * @param secondary_index: Same cache, written to equipment slot 3.
 */
DECLARE_FUNCTION(void, equipArtisanTools, SF_CGdFigureToolbox *_this, uint16_t figure_id, uint32_t tool_index,
                 uint32_t secondary_index);

DECLARE_FUNCTION(uint8_t, getSightRange, SF_CGdFigureToolbox *_this, uint16_t figure_id);
// getDistance(SF_Coord *pointA, SF_Coord *pointB)
DECLARE_FUNCTION(uint32_t, getDistance, SF_Coord *pointA, SF_Coord *pointB);

DECLARE_FUNCTION(uint16_t, findClosestMonument, void *SF_CGdObjectToolBox, uint16_t figure_id, uint16_t *result);

/**
 * doMapOutCry - Broadcasts a map outcry event to the game listeners.
 * @param outcry_data_byte: Opaque byte payload of the event record
 *        (framework passes 4).
 * @param outcry_data_word: Opaque word payload of the event record
 *        (framework passes 0).
 */
DECLARE_FUNCTION(void, doMapOutCry, void *AC30, uint16_t outCryType, uint16_t owner, uint8_t outcry_data_byte, uint16_t outcry_data_word,
                 uint16_t pos_x, uint16_t pos_y);
//bool isBuildingAllied(SF_CGdBuildingToolbox *_this, uint16_t figure_index, uint16_t building_index)



/**
 * @ingroup API
 * @brief Holds most of the Toolbox Functions relevent for custom spells. Includes functions for the manipulation of
 * Figures.
 */
typedef struct
{
    getDistance_ptr getDistance;
    dealDamage_ptr dealDamage;
    isTargetable_ptr isTargetable;
    figuresCheckHostile_ptr figuresCheckHostile;
    figuresCheckNeutral_ptr figuresCheckNeutral;
    figuresCheckFriendly_ptr figuresCheckFriendly;
    hasSpellOnIt_ptr hasSpellOnIt;
    hasAuraActive_ptr hasAuraActive;
    rescaleLevelStats_ptr rescaleLevelStats;
    attachSpellToFigure_ptr attachSpellToFigure;
    getFigureFromWorld_ptr getFigureFromWorld;
    getSpellIndexOfType_ptr getSpellIndexOfType;
    getSpellIndexFromDLL_ptr getSpellIndexFromDLL; // Use Wrappers instead of direct passthrough
    getNextNode_ptr getNextNode; // Use Wrappers instead of direct passthrough
    figureSetNewJob_ptr figureSetNewJob; // Send to Figure Functions
    removeSpellFromList_ptr removeSpellFromList;
    addUnit_ptr addUnit;
    findClosestFreePosition_ptr findClosestFreePosition;
    isUnitMelee_ptr isUnitMelee;
    getPhysDamageReduction_ptr getPhysDamageReduction;
    getFigureXData_ptr getFigureXData;
    setFigureXData_ptr setFigureXData;
    equipArtisanArmour_ptr equipArtisanArmour;
    equipArtisanTools_ptr equipArtisanTools;
    findClosestMonument_ptr findClosestMonument;
    doMapOutCry_ptr doMapOutCry;
    getSightRange_ptr getSightRange;
} ToolboxFunctions;
