#pragma once
#include "./structures/sf_figure_structures.h"

// Declare the function pointers for the FigureFunctions group

DECLARE_FUNCTION(uint16_t, getCurrentStat, SF_CGdFigure *figure, uint16_t target, StatisticDataKey key);
DECLARE_FUNCTION(uint16_t, getMaxStat, SF_CGdFigure *figure, uint16_t target, StatisticDataKey key);

DECLARE_FUNCTION(bool, isAlive, SF_CGdFigure *figure, uint16_t target);
DECLARE_FUNCTION(bool, setWalkSpeed, SF_CGdFigure *figure, uint16_t target,
                 uint16_t value);
/**
 * addAction - Adds an action for a figure; exact queueing semantics not
 * established.
 * @param figure_action: Pointer to the SF_SGtFigureAction to add
 *        (fields type/subtype/cooldown_action/cooldown_global).
 * Return: Non-zero on success (callers ignore the value).
 */
DECLARE_FUNCTION(bool, addAction, SF_CGdFigure *figure, uint16_t target,
                 void *figure_action);
DECLARE_FUNCTION(void, addBonusMultToStatistic, SF_CGdFigure *figure,
                 StatisticDataKey key, uint16_t target, int8_t value);
DECLARE_FUNCTION(uint8_t, addBonusMult, FigureStatistic *statistic,
                 uint8_t value);

DECLARE_FUNCTION(void, decreaseHealth, SF_CGdFigure *figure, uint16_t figure_id,
                 uint16_t amount);


DECLARE_FUNCTION(void, setJobToDoCount, SF_CGdFigure *figure,
                 uint16_t target_index, uint16_t value);
DECLARE_FUNCTION(uint16_t, getCurrentHealthPercent, SF_CGdFigure *figure,
                 uint16_t figure_id);
DECLARE_FUNCTION(void, rescaleMana, SF_CGdFigure *figure, uint16_t figure_id,
                 uint16_t max_mana);
DECLARE_FUNCTION(void, rescaleHealth, SF_CGdFigure *figure, uint16_t figure_id,
                 uint16_t max_health);
DECLARE_FUNCTION(uint16_t, getJob, SF_CGdFigure *figure, uint16_t figure_id);
DECLARE_FUNCTION(uint16_t, getAggroValue, SF_CGdFigure *_this,
                 uint16_t figure_id, uint16_t target_index);
/**
 * setAggroValue - Sets the aggro value held for a target figure.
 * @param unknown_value: Opaque; purpose unknown (framework passes 0).
 */
DECLARE_FUNCTION(void, setAggroValue, SF_CGdFigure *_this, uint16_t figure_id,
                 uint16_t target_index, uint16_t aggroValue, uint32_t unknown_value);
/**
 * getIncomingSpellList - Fills a uint32 vector ({first, data, post_last}) with
 * the in-flight spell casts (any spell line, not heal-only) against the
 * figure - each 4-byte element is a pair {uint16 caster figure id (low),
 * uint16 spell id (high)}, pushed by the engine when a cast starts
 * (JobPreCast) and removed on cast resolve or job switch.
 * @param out_healer_list: Receives the filled list; the caller reads the
 *        element range first..data.
 * Return: Opaque (framework uses the filled buffer, not the return).
 *
 * The snapshot is owned by the caller and MUST be released with
 * releaseIncomingSpellList().
 */
DECLARE_FUNCTION(void *, getIncomingSpellList, SF_CGdFigure *_this, void *out_healer_list,
                 uint16_t figure_id);
DECLARE_FUNCTION(bool, isFlagSet, SF_CGdFigure *figure, uint16_t figure_index,
                 GdFigureFlags flag);
DECLARE_FUNCTION(uint16_t, getSpellJobStartNode, SF_CGdFigure *figure,
                 uint16_t figure_index);
DECLARE_FUNCTION(void, subMana, SF_CGdFigure *_this, uint16_t figure_index, int32_t amount);
DECLARE_FUNCTION(void, subHealth, SF_CGdFigure *_this, uint16_t figure_index, int32_t amount);
DECLARE_FUNCTION(SF_Coord, getPosition, SF_CGdFigure *figure,
                 SF_Coord *position, uint16_t figure_id);
DECLARE_FUNCTION(SF_CGdFigureWeaponStats *, getWeaponStats,
                 SF_CGdFigure *figure, SF_CGdFigureWeaponStats *stats_in,
                 uint16_t figure_id);
DECLARE_FUNCTION(bool, isWarrior, SF_CGdFigure *figure, uint16_t figure_id);
DECLARE_FUNCTION(void, setTask, SF_CGdFigure *_figure, uint16_t figure_index,
                 CGdFigureTask figureTask);

DECLARE_FUNCTION(bool, isSiegeUnit, SF_CGdFigure *figure, uint16_t figure_id);

// Unknown Functons
DECLARE_FUNCTION(bool, FUN_006e3a90, SF_CGdFigureJobs *_this, uint16_t figure_id);
DECLARE_FUNCTION(void, prepareJobTransition, SF_CGdFigureJobs *_this, uint16_t figure_id, uint16_t job_from,
                 uint16_t job_to);
DECLARE_FUNCTION(void, setJob, SF_CGdFigureJobs *_this, uint16_t figure_id, uint16_t job_id);
DECLARE_FUNCTION(void, onStartJob,  SF_CGdFigureJobs *_this, uint16_t figure_id, uint16_t job_id);
/**
 * releaseIncomingSpellList - Frees an incoming spell list snapshot obtained
 * from getIncomingSpellList: releases the buffer and zeroes all three
 * pointers. Safe on an already empty or already released snapshot; skipping
 * the call leaks the snapshot buffer (the caller owns it).
 */
DECLARE_FUNCTION(void, releaseIncomingSpellList, void *healer_list);
/**
 * setFigureWalkPos - Orders a figure to walk to a position.
 * @param set_redo_flag: Non-zero sets the figure REDO flag
 *        (GdFigureFlags 0x10), forcing job re-evaluation.
 */
DECLARE_FUNCTION(void, setFigureWalkPos, SF_CGdFigureJobs *_this, uint16_t figure_id, SF_Coord *target_pos,
                 uint8_t walk_mode, uint32_t set_redo_flag);
/**
 * @ingroup API
 * @brief Group of functions to manipulate the behavior and statistics of the game figures (units)
 */
typedef struct
{
    isAlive_ptr isAlive;
    isSiegeUnit_ptr isSiegeUnit;
    isFlagSet_ptr isFlagSet;
    isWarrior_ptr isWarrior;

    addAction_ptr addAction;
    addBonusMult_ptr addBonusMult;
    addBonusMultToStatistic_ptr addBonusMultToStatistic;

    getCurrentStat_ptr getCurrentStat;
    getMaxStat_ptr getMaxStat;
    getWeaponStats_ptr getWeaponStats;
    getAggroValue_ptr getAggroValue;
    getIncomingSpellList_ptr getIncomingSpellList;
    getJob_ptr getJob;
    getSpellJobStartNode_ptr getSpellJobStartNode;
    getCurrentHealthPercent_ptr getCurrentHealthPercent;
    getPosition_ptr getPosition;

    setTask_ptr setTask;
    setAggroValue_ptr setAggroValue;
    setWalkSpeed_ptr setWalkSpeed;
    setJobToDoCount_ptr setJobToDoCount;

    rescaleMana_ptr rescaleMana;
    rescaleHealth_ptr rescaleHealth;

    decreaseHealth_ptr decreaseHealth;
    subMana_ptr subMana;
    subHealth_ptr subHealth;

    releaseIncomingSpellList_ptr releaseIncomingSpellList;
    FUN_006e3a90_ptr FUN_006e3a90;
    prepareJobTransition_ptr prepareJobTransition;
    onStartJob_ptr onStartJob;
    setJob_ptr setJob;
    setFigureWalkPos_ptr setFigureWalkPos;
} FigureFunctions;
