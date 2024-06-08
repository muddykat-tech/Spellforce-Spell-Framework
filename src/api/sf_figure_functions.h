#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "sf_general_structures.h"

typedef enum
{
	ARMOR,
	AGILITY,
	CHARISMA,
	DEXTERITY,
	HEALTH,
	INTELLIGENCE,
	MANA_STUFF,
	STAMINA,
	STRENGTH,
	WISDOM,
	RESISTANCE_FIRE,
	RESISTANCE_ICE,
	RESISTANCE_MENTAL,
	RESISTANCE_BLACK,
	WALK_SPEED,
	FIGHT_SPEED,
	CAST_SPEED,
} StatisticDataKey;

// Declare the function pointers for the FigureFunctions group
DECLARE_FUNCTION(bool, isAlive, SF_CGdFigure *figure, uint16_t target);
DECLARE_FUNCTION(bool, setWalkSpeed, SF_CGdFigure *figure, uint16_t target, uint16_t value);
DECLARE_FUNCTION(bool, addAction, SF_CGdFigure *figure, uint16_t target, void *maybe_action);
DECLARE_FUNCTION(void, addBonusMultToStatistic, SF_CGdFigure *figure, StatisticDataKey key, uint16_t target, int8_t value);
DECLARE_FUNCTION(uint8_t, addBonusMult, FigureStatistic statistic, uint8_t value);
DECLARE_FUNCTION(uint16_t, getCurrentHealth, SF_CGdFigure *figure, uint16_t figure_id);
DECLARE_FUNCTION(void, decreaseHealth, SF_CGdFigure *figure, uint16_t figure_id, uint16_t amount);
DECLARE_FUNCTION(uint16_t, getCurrentMaxMana, SF_CGdFigure *figure, uint16_t figure_id);
DECLARE_FUNCTION(uint16_t, getCurrentMaxHealth, SF_CGdFigure *figure, uint16_t figure_id);

DECLARE_FUNCTION(void, rescaleMana, SF_CGdFigure *figure, uint16_t figure_id, uint16_t max_mana);
DECLARE_FUNCTION(void, rescaleHealth, SF_CGdFigure *figure, uint16_t figure_id, uint16_t max_health);

/*
 * @brief Macro used to define a group of function pointers for a given struct
 *
 * @details This macro is used to declare a group of function pointers, in this case a Figure struct. It
 * defines a list of function pointers and the names of the functions they point to.
 *
 * @param GroupName The name of the group, used as the prefix for the function pointer variables.
 * @param Functions A list of function pointers, each with a name and type.
 *
 * The generated code will define a struct (with the same name as the GroupName) with each function pointer as a member.
 */
DECLARE_FUNCTION_GROUP(Figure,
					   isAlive_ptr isAlive;
					   setWalkSpeed_ptr setWalkSpeed;
					   addAction_ptr addAction;
					   addBonusMult_ptr addBonusMult;
					   addBonusMultToStatistic_ptr addBonusMultToStatistic;
					   decreaseHealth_ptr decreaseHealth;
					   getCurrentHealth_ptr getCurrentHealth;
					   getCurrentMaxHealth_ptr getCurrentMaxHealth;
					   getCurrentMaxMana_ptr getCurrentMaxMana;
					   rescaleMana_ptr rescaleMana;
					   rescaleHealth_ptr rescaleHealth;);
