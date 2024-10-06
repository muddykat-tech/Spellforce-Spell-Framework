#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "sf_general_structures.h"

// Declare the function pointers for the ToolboxFunctions group
DECLARE_FUNCTION(void, dealDamage, SF_CGdFigureToolbox *CGdFigureToolbox, uint16_t source_index, uint16_t target_index, uint32_t damage, uint32_t is_spell_damage, uint32_t param5, uint32_t param6);
DECLARE_FUNCTION(bool, isTargetable, SF_CGdFigureToolbox *CGdFigureToolbox, uint16_t figure_index);
DECLARE_FUNCTION(uint16_t, figuresCheckHostile, SF_CGdFigureToolbox *CGdFigureToolbox, uint16_t source_index, uint16_t target_index);
DECLARE_FUNCTION(void, buildingDealDamage, void *CGdBuildingToolBox, uint16_t figure_id, uint16_t building_id, uint16_t damage, uint32_t is_spell_damage);
DECLARE_FUNCTION(uint16_t, figuresCheckNeutral, SF_CGdFigureToolbox *CGdFigureToolbox, uint16_t source_index, uint16_t target_index);
DECLARE_FUNCTION(uint16_t, figuresCheckFriendly, SF_CGdFigureToolbox *CGdFigureToolbox, uint16_t source_index, uint16_t target_index);
DECLARE_FUNCTION(uint32_t, hasSpellOnIt, SF_CGdFigureToolbox *CGdFigureToolbox, uint16_t figure_index, uint16_t spell_line_id);
DECLARE_FUNCTION(void, rescaleLevelStats, SF_CGdFigureToolbox *CGdFigureToolbox, uint16_t figure_index);
DECLARE_FUNCTION(void, addSpellToFigure, SF_CGdFigureToolbox *CGdFigureToolbox, uint16_t figure_index, uint16_t spell_index);
DECLARE_FUNCTION(uint16_t, getFigureFromWorld, void *CGdWorldToolBox, uint16_t param1, uint16_t param2, uint32_t param3);
DECLARE_FUNCTION(uint16_t, getSpellIndexOfType, SF_CGdFigureToolbox *CGdFigureToolbox, uint16_t target_index, uint16_t spell_line, uint16_t last_known_index);
DECLARE_FUNCTION(uint16_t, getSpellIndexFromDLL, uint32_t *CGdDoubleLinkedList, uint16_t spell_job_start_node);
DECLARE_FUNCTION(uint16_t, getNextNode, uint32_t *CGdDoubleLinkedList, uint16_t current_node);
DECLARE_FUNCTION(void, figureSetNewJob, void *CGdFigureJobs, uint32_t figure_id, uint32_t new_job, uint32_t param_3, uint32_t param_4, uint32_t param_5);
DECLARE_FUNCTION(uint32_t, removeSpellFromList, SF_CGdFigureToolbox *CGdFigureToolbox, uint16_t param_1, uint16_t param_2);
DECLARE_FUNCTION(uint16_t, addUnit, SF_CGdFigureToolbox *_this, uint16_t posX, uint16_t posY, uint16_t owner, uint16_t figure_type,
                    uint16_t spawn_effect, int npc_id, int faction, int disable_ai);
DECLARE_FUNCTION(bool, findClosestFreePosition, SF_CGdWorldToolBox* _this, SF_Coord *param_1,SF_Coord *param_2,
                    uint16_t sector, SF_Coord *return_value)

DECLARE_FUNCTION_GROUP(Toolbox,
                       dealDamage_ptr dealDamage;
                       isTargetable_ptr isTargetable;
                       figuresCheckHostile_ptr figuresCheckHostile;
                       figuresCheckNeutral_ptr figuresCheckNeutral;
                       figuresCheckFriendly_ptr figuresCheckFriendly;
                       hasSpellOnIt_ptr hasSpellOnIt;
                       buildingDealDamage_ptr buildingDealDamage;
                       rescaleLevelStats_ptr rescaleLevelStats;
                       addSpellToFigure_ptr addSpellToFigure;
                       getFigureFromWorld_ptr getFigureFromWorld;
                       getSpellIndexOfType_ptr getSpellIndexOfType;
                       getSpellIndexFromDLL_ptr getSpellIndexFromDLL;
                       getNextNode_ptr getNextNode;
                       figureSetNewJob_ptr figureSetNewJob;
                       removeSpellFromList_ptr removeSpellFromList;
                       addUnit_ptr addUnit;
                       findClosestFreePosition_ptr findClosestFreePosition;
                       );