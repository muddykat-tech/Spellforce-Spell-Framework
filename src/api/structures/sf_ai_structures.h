#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "../sf_general_structures.h"

/**
 * @struct SF_SGtFigureAction
 */
typedef struct __attribute__((packed))
{
    uint16_t type;
    uint16_t subtype;
    uint16_t unkn2;
    uint16_t unkn3;
    uint16_t unkn4;
    uint16_t unkn5;
} SF_SGtFigureAction;

/**
 * @struct EntityPriorityList_data
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
} EntityPriorityList_data;

/**
 * @struct EntityPriorityList
 * @brief Holds a list of entities with associated data and an entity count.
 *
 * This structure is used to manage groups of allied or enemy entities in the game.
 */
typedef struct __attribute((packed))
{
    EntityPriorityList_data data[500]; /**< Array of entity data. */
    uint16_t entityCount;       /**< Number of entities in the group. */
} EntityPriorityList;

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
    SF_CGdPlayer *CGdPlayer;
    void *CGdResource;
    SF_CGdSpell *CGdSpell;
    void *autoclass22;
    void *autoclass50;
    void *CGdVisibility;
    SF_CGdWorld *CGdWorld;                      /**< Pointer to the game world data. */
    SF_CGdWorldToolBox *CGdWorldToolBox;        /**< Toolbox for managing world-related operations. */
    void *autoclass64;
    EntityPriorityList ally_figures;
    uint16_t unkn1;
    EntityPriorityList enemy_figures;
    uint16_t unkn2;
    EntityPriorityList building_ally;
    uint16_t unkn3;
    EntityPriorityList building_not_ally;
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
