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
#include "./structures/sf_ai_structures.h"

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
DECLARE_FUNCTION(SF_SGtFigureAction *, getTargetAction, SF_CGdFigure *figure,
                 SF_SGtFigureAction *action, uint16_t figure_id);

DECLARE_FUNCTION(SF_SGtFigureAction *, getFigureAction, SF_CGdFigure *figure,
                 SF_SGtFigureAction *action, uint16_t figure_id,
                 uint8_t action_index);

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
DECLARE_FUNCTION(void, setAICurrentActionRanking,
                 SF_CGdBattleDevelopment *_this, int rank);

DECLARE_FUNCTION(int, getAICurrentActionRanking,
                 SF_CGdBattleDevelopment *_this);

DECLARE_FUNCTION(bool, canFigureDoAction, SF_CGdBattleDevelopment *_this,
                 SF_SGtFigureAction *action);
DECLARE_FUNCTION(void, getActionStats, SF_CGdBattleDevelopment *_this,
                 uint16_t *minRange, uint16_t *maxRange,
                 SF_CGdResourceSpell *spellData);

// May be moved to General if it is used elsewhere
DECLARE_FUNCTION(bool, isAIVectorEmpty, void *_this);
DECLARE_FUNCTION(uint16_t **, getAIVectorFirstElement, void *_this,
                 uint16_t **pvalue);
DECLARE_FUNCTION(uint16_t **, getAIVectorGetCurrent, void *_this,
                 uint16_t **value);
DECLARE_FUNCTION(int, getAIVectorLength, void *_this);
DECLARE_FUNCTION(uint16_t, getCastType, void *resource, uint16_t spellID);
DECLARE_FUNCTION(void *, AC60AddOrGetEntity, void *_autoclass60,
                 uint16_t entity_index);
DECLARE_FUNCTION(SF_Coord *, getPositionToCastAlly,
                 SF_CGdBattleDevelopment *_this,SF_Coord *param_1,
                 void *std_vector_uint16_t);
DECLARE_FUNCTION(SF_Coord *, getPositionToCastEnemy,
                 SF_CGdBattleDevelopment *_this,SF_Coord *param_1,
                 void *std_vector_uint16_t);

DECLARE_FUNCTION(bool, isUnknownWorldFeature, SF_CGdWorldToolBox *,
                 uint16_t figure_index, SF_Coord * param2, SF_Coord *param3,
                 uint32_t param4, SF_Coord * param5, uint16_t param6);

/**
 * @ingroup API
 * @brief Group of functions related to AI manipulation.
 */
typedef struct
{
    getTargetAction_ptr getTargetAction;
    getFigureAction_ptr getFigureAction;
    canFigureDoAction_ptr canFigureDoAction;
    /**
     * @brief sets the ranking of the AI's current selected action
     */
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
} AiFunctions;
