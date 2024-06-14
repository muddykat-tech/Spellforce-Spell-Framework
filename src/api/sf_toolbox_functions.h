#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "sf_general_structures.h"

// Declare the function pointers for the ToolboxFunctions group
DECLARE_FUNCTION(void, dealDamage, void *CGdFigureToolbox, uint16_t source_index, uint16_t target_index, uint32_t damage, uint32_t is_spell_damage, uint32_t param5, uint32_t param6);
DECLARE_FUNCTION(bool, isTargetable, void *CGdFigureToolbox, uint16_t figure_index);
DECLARE_FUNCTION(uint16_t, figuresCheckHostile, void *CGdFigureToolbox, uint16_t source_index, uint16_t target_index);
DECLARE_FUNCTION(void, buildingDealDamage, void *CGdBuildingToolBox, uint16_t figure_id, uint16_t building_id, uint16_t damage, uint32_t is_spell_damage);
DECLARE_FUNCTION(uint16_t, figuresCheckHostile, void *CGdFigureToolbox, uint16_t source_index, uint16_t target_index);
DECLARE_FUNCTION(uint32_t, hasSpellOnHit, void *CGdFigureToolbox, uint16_t figure_index, uint16_t spell_line_id);
DECLARE_FUNCTION(void, rescaleLevelStats, void *CGdFigureToolbox, uint16_t figure_index);
DECLARE_FUNCTION(void, addSpellToFigure, void *CGdFigureToolbox, uint16_t figure_index, uint16_t spell_index);

DECLARE_FUNCTION_GROUP(Toolbox,
                       dealDamage_ptr dealDamage;
                       isTargetable_ptr isTargetable;
                       figuresCheckHostile_ptr figuresCheckHostile;
                       hasSpellOnHit_ptr hasSpellOnHit;
                       buildingDealDamage_ptr buildingDealDamage;
                       rescaleLevelStats_ptr rescaleLevelStats;
                       addSpellToFigure_ptr addSpellToFigure;
                       );