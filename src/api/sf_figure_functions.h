#pragma once
#include "./structures/sf_figure_structures.h"

// Declare the function pointers for the FigureFunctions group

DECLARE_FUNCTION(uint16_t, getCurrentStat, SF_CGdFigure *figure, uint16_t target, StatisticDataKey key);
DECLARE_FUNCTION(uint16_t, getMaxStat, SF_CGdFigure *figure, uint16_t target, StatisticDataKey key);

DECLARE_FUNCTION(bool, isAlive, SF_CGdFigure *figure, uint16_t target);
DECLARE_FUNCTION(bool, setWalkSpeed, SF_CGdFigure *figure, uint16_t target,
                 uint16_t value);
DECLARE_FUNCTION(bool, addAction, SF_CGdFigure *figure, uint16_t target,
                 void *maybe_action);
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
DECLARE_FUNCTION(uint16_t, getAggroValue, SF_CGdFigure * _this,
                 uint16_t figure_id, uint16_t target_index);
DECLARE_FUNCTION(void, setAggroValue, SF_CGdFigure * _this, uint16_t figure_id,
                 uint16_t target_index, uint16_t aggroValue, uint32_t unkn);
DECLARE_FUNCTION(void *, getHealersList, SF_CGdFigure * _this, void *autoclass9,
                 uint16_t figure_id);
DECLARE_FUNCTION(bool, isFlagSet, SF_CGdFigure *figure, uint16_t figure_index,
                 GdFigureFlags flag);
DECLARE_FUNCTION(uint16_t, getSpellJobStartNode, SF_CGdFigure *figure,
                 uint16_t figure_index);
DECLARE_FUNCTION(void, subMana, SF_CGdFigure *_this, uint16_t param_1,
                 uint32_t param_2);

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
DECLARE_FUNCTION(void, setJob, SF_CGdFigureJobs *_this,uint16_t figure_id,uint16_t job_id);
DECLARE_FUNCTION(void, onStartJob,  SF_CGdFigureJobs *_this, uint16_t figure_id, uint16_t job_id);
DECLARE_FUNCTION(void, disposeHealerList, void *healer_list);
DECLARE_FUNCTION(void, setFigureWalkPos, SF_CGdFigureJobs *_this,uint16_t figure_id,SF_Coord *target_pos,
                 uint8_t walk_mode,uint32_t param_4);
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
    getHealersList_ptr getHealersList;
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

    disposeHealerList_ptr disposeHealerList;
    FUN_006e3a90_ptr FUN_006e3a90;
    prepareJobTransition_ptr prepareJobTransition;
    onStartJob_ptr onStartJob;
    setJob_ptr setJob;
    setFigureWalkPos_ptr setFigureWalkPos;
} FigureFunctions;
