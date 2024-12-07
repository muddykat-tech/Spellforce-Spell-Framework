/**
 * @file sf_ai_functions.h
 * @brief Header file for defining AI-related battle functions for managing spellcasting,
 *        NPC behavior, and target actions in the game.
 * 
 * This file provides the necessary data structures and function declarations
 * used by the AI to interact with the game's entities, actions, and battle mechanics.
 * Functions include getting and setting AI actions, managing AI figures, and handling
 * spell casting behaviors.
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "sf_general_structures.h"

/**
 * @struct ushort_list_node
 * @brief Represents a node in a list of unsigned short values.
 * 
 * Used for managing lists of short integer data related to game entities or actions.
 */
typedef struct __attribute__((packed))
{
    uint16_t *first;        /**< Pointer to the first element in the list. */
    uint16_t *data;         /**< Pointer to the data element within the list. */
    uint16_t *post_last;    /**< Pointer to the element following the last in the list. */
} ushort_list_node;

/**
 * @struct AutoClass60_data
 * @brief Stores data related to a specific entity in the AI system.
 * 
 * This structure holds the entity index, padding, and hate value,
 * which are used to determine target preferences for battle AI.
 */
typedef struct __attribute((packed))
{
    uint16_t entity_index;  /**< Index of the entity in the game world. */
    uint16_t padding;       /**< Padding for alignment, not used in logic. */
    uint32_t hate_value;    /**< The entity's hate value, used to determine aggressiveness. */
} AutoClass60_data;

/**
 * @struct AutoClass60
 * @brief Holds a list of entities with associated data and an entity count.
 * 
 * This structure is used to manage groups of allied or enemy entities in the game.
 */
typedef struct __attribute((packed))
{
    AutoClass60_data data[500]; /**< Array of entity data. */
    uint16_t entityCount;       /**< Number of entities in the group. */
} AutoClass60;

/**
 * @struct CGdAIBattleData
 * @brief The main structure for handling AI battle data.
 * 
 * This structure encapsulates various components required for managing AI behavior,
 * including information about figures, targets, spells, and world data.
 * It is utilized within the game's AI systems to coordinate and process actions, strategies, and outcomes.
 */
typedef struct __attribute__((packed))
{
    SF_CGdBuilding *CGdBuilding;                /**< Pointer to the building data. */
    void *CGdBuildingToolBox;                   /**< ToolBox for managing building-related operations. */
    void *autoclass30;                          /**< Unidentified component */
    SF_CGdFigure *CGdFigure;                    /**< Pointer to the Global Figure data structure . */
    SF_CGdFigureJobs *CGdFigureJobs;            /**< Pointer to the Global Figure Job data structure. */
    SF_CGdFigureToolbox *CGdFigureToolBox;      /**< Toolbox for figure-related operations. */
    void *autoclass34;                          /**< Unidentified component */
    void *CGdFormation;                         
    void *CGdDoubleLinkList;
    void *CGdInfluenceMap;
    void *CGdObject;
    void *CGdObjectToolBox;
    void *CGdPlayer;
    void *CGdResource;
    SF_CGdSpell *CGdSpell;
    void *autoclass22;
    void *autoclass50;
    void *CGdVisibility;
    SF_CGdWorld *CGdWorld;                      /**< Pointer to the game world data. */
    SF_CGdWorldToolBox *CGdWorldToolBox;        /**< Toolbox for managing world-related operations. */
    void *autoclass64;
    AutoClass60 ally_figures;
    uint16_t unkn1;
    AutoClass60 enemy_figures;
    uint16_t unkn2;
    AutoClass60 building_ally;
    uint16_t unkn3;
    AutoClass60 building_not_ally;
    uint16_t unkn4;
    uint32_t unkn5;
    SF_SGtFigureAction current_action;
    uint16_t current_action_min_rng;
    uint16_t current_action_max_rng;
    SF_String world_name;
    uint32_t can_be_target_maybe;
    uint16_t unkn6;
    uint8_t current_figure_sight_range_maybe;
    uint8_t unkn7;
    uint32_t figures_missing_hp;
    uint32_t current_figure_is_aggroed;
    uint32_t current_figure_is_not_mainchar_or_hero;
    uint32_t current_figure_has_amok;
    uint32_t unkn_flag1;
    uint32_t current_figure_is_mainchar;
    uint32_t current_figure_is_hero;
    uint32_t current_figure_has_owner;
    uint32_t manual_hit_target_maybe;
    uint32_t current_figure_is_tower;
    uint32_t current_figure_is_warrior;
    uint32_t something_related_to_ranking;
    uint8_t current_figure_target_type;
    uint16_t current_figure_target_index;
    SF_Coord current_figure_target_position;
    uint8_t unkn8;
    uint32_t current_hate;
    uint32_t current_ranking;
    uint8_t unkn_list[0xfa2];               /**< Large array for unknown purposes */
    uint16_t unkn9;
    uint32_t current_figure_noaggroattack;
    uint32_t is_map_arena3;
    ushort_list_node some_figure_list[2000];
    ushort_list_node unkn_node;
    ushort_list_node another_figure_list[2000];
    uint32_t unkn_array[2000];
    uint16_t current_figure;
    SF_Coord current_figure_pos;
    uint16_t unkn10;
    uint32_t current_figure_is_combat_involved_maybe;
    uint32_t current_figure_has_aggro;
    uint32_t current_figure_has_demoralization;
    uint32_t unkn_flag2;
    uint32_t figures_max_hp;
    SF_CGdTargetData current_source_maybe;
    SF_CGdTargetData current_target;
    uint16_t unkn11;
    uint32_t force_process_figure_maybe;
    SF_CGdTargetData unknown_target;
    uint8_t unkn12;
    uint32_t some_ranking;
    uint32_t current_figure_is_pet;
    uint16_t current_figure_master;
    uint8_t unkn13;
    uint8_t unkn14;
    uint32_t figure_is_charmed_summon_maybe;
    uint32_t unkn_flag3;
    uint32_t current_target_level_possibly;
    uint32_t action_is_siege_aura;
} CGdAIBattleData;

/**
 * @struct SF_CGdBattleDevelopment
 * @brief High-level structure managing AI battle development.
 */
typedef struct __attribute__((packed))
{
    void *vfTable;                  /**< Virtual function table pointer. */
    void *battleFactory;            /**< Pointer to the battle factory object. */
    CGdAIBattleData battleData;     /**< Detailed AI battle data. */
} SF_CGdBattleDevelopment;

/**
 * @brief Retrieves the target action for a specific figure.
 * 
 * This function determines the action a figure should perform based on the target
 * it is currently focused on. The figure's action is retrieved from the action
 * list and returned.
 * 
 * @param figure Pointer to the figure that will perform the action.
 * @param action Pointer to an action structure where the action details will be stored.
 * @param figure_id ID of the figure whose action is being queried.
 * @return SF_SGtFigureAction* A pointer to the action that the figure should perform.
 */
DECLARE_FUNCTION(SF_SGtFigureAction *, getTargetAction, SF_CGdFigure *figure, SF_SGtFigureAction *action, uint16_t figure_id);

DECLARE_FUNCTION(SF_SGtFigureAction *, getFigureAction, SF_CGdFigure *figure, SF_SGtFigureAction *action, uint16_t figure_id, uint8_t action_index);

/**
 * @brief Sets the ranking of the current action for AI decision-making.
 * 
 * This function assigns a ranking value to the AI's current action, which influences
 * the AI's decision-making process for which action to perform next based on its
 * priorities and the battle situation.
 * 
 * @param _this Pointer to the AI battle development structure.
 * @param rank The ranking value to assign to the current action. Higher values indicate higher priority.
 */
DECLARE_FUNCTION(void, setAICurrentActionRanking, SF_CGdBattleDevelopment *_this, int rank);

DECLARE_FUNCTION(int, getAICurrentActionRanking, SF_CGdBattleDevelopment *_this);

DECLARE_FUNCTION(bool, canFigureDoAction, SF_CGdBattleDevelopment *_this, SF_SGtFigureAction *action);
DECLARE_FUNCTION(void, getActionStats, SF_CGdBattleDevelopment *_this, uint16_t *minRange, uint16_t *maxRange, SF_CGdResourceSpell *spellData);

// May be moved to General if it is used elsewhere
DECLARE_FUNCTION(bool, isAIVectorEmpty, void *_this);
DECLARE_FUNCTION(uint16_t **, getAIVectorFirstElement, void *_this, uint16_t **pvalue);
DECLARE_FUNCTION(uint16_t **, getAIVectorGetCurrent, void *_this, uint16_t **value);
DECLARE_FUNCTION(int, getAIVectorLength, void *_this);
DECLARE_FUNCTION(uint16_t, getCastType, void* resource, uint16_t spellID);
DECLARE_FUNCTION(void *, AC60AddOrGetEntity, void *_autoclass60, uint16_t entity_index);
DECLARE_FUNCTION(SF_Coord *, getPositionToCastAlly, SF_CGdBattleDevelopment *_this,SF_Coord *param_1,void *std_vector_uint16_t);
DECLARE_FUNCTION(SF_Coord *, getPositionToCastEnemy, SF_CGdBattleDevelopment *_this,SF_Coord *param_1,void *std_vector_uint16_t);

DECLARE_FUNCTION(bool, isUnknownWorldFeature, SF_CGdWorldToolBox *, uint16_t figure_index, SF_Coord * param2, SF_Coord *param3, uint32_t param4, SF_Coord * param5, uint16_t param6);

/**
 * @function_group Ai
 * @brief Group of functions related to AI manipulation.
 */
DECLARE_FUNCTION_GROUP(Ai,
                       getTargetAction_ptr getTargetAction;
                       getFigureAction_ptr getFigureAction;
                       canFigureDoAction_ptr canFigureDoAction;
                       setAICurrentActionRanking_ptr setAICurrentActionRanking;
                       getAICurrentActionRanking_ptr getAICurrentActionRanking;
                       isAIVectorEmpty_ptr isAIVectorEmpty;
                       getAIVectorFirstElement_ptr getAIVectorFirstElement;
                       getAIVectorGetCurrent_ptr getAIVectorGetCurrent;
                       getAIVectorLength_ptr getAIVectorLength;
                       AC60AddOrGetEntity_ptr AC60AddOrGetEntity;
                       getActionStats_ptr getActionStats;
                       getCastType_ptr getCastType;
                       getPositionToCastAlly_ptr getPositionToCastAlly;
                       getPositionToCastEnemy_ptr getPositionToCastEnemy;
                       isUnknownWorldFeature_ptr isUnknownWorldFeature;
                       );