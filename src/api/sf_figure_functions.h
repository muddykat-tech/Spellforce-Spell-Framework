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

typedef enum
{
    kGdJobDefault = 0,
    kGdJobNone = 0,
    kGdJobGroupNothing = 1,
    kGdJobNothing = 1,
    kGdJobStepAside = 2,
    kGdJobGroupWalk = 2,
    kGdJobOfferMe = 3,
    kGdJobShrineWorkerCheckDrop = 4,
    kGdJobCarpenterWork = 5,
    kGdJobWalkToBasePoint = 6,
    kGdJobCastPreResolve = 7,
    kGdJobWalkToAttackMonument = 8,
    kGdJobCattleBreederWalkToDeliverGood = 9,
    kGdJobWoodCutterStoopToDropLog = 10,
    kGdJobCattleBreederCheckDrop = 11,
    kGdJobWoodCutterCheckDrop = 12,
    kGdJobGotoBuildingForWork = 13,
    kGdJobStartWorkAtBuilding = 14,
    kGdJobCattleBreederWalkHome = 15,
    kGdJobCorpseCollectorWalkToCorpse = 16,
    kGdJobBuilderWalkToBuildPos = 17,
    kGdJobBuilderBuild = 18,
    kGdJobCorpseCollectorSearchForWork = 19,
    kGdJobStoneMinerCheckDrop = 20,
    kGdJobMinerCheckResource = 21,
    kGdJobFarmerWalkToDeliverGood = 22,
    kGdJobWoodCutterSearchTree = 23,
    kGdJobWoodCutterWalkToTree = 24,
    kGdJobWoodCutterCheckTree = 25,
    kGdJobWoodCutterCutTree = 26,
    kGdJobWoodCutterWalkHome = 27,
    kGdJobClubMakerSearchForWork = 28,
    kGdJobMinerWalkToWork = 29,
    kGdJobStoneMinerStoopToDropStone = 30,
    kGdJobCorpseCollectorCheckCorpse = 31,
    kGdJobStoneMinerSearchStone = 32,
    kGdJobStoneMinerWalkToStone = 33,
    kGdJobStoneMinerCheckStone = 34,
    kGdJobStoneMinerCrushStone = 35,
    kGdJobStoneMinerWalkHome = 36,
    kGdJobClubMakerWork = 37,
    kGdJobGoto = 38,
    kGdJobWalkToTarget = 39,
    kGdJobHitTarget = 40,
    kGdJobDie = 41,
    kGdJobWarriorNothing = 42,
    kGdJobCast = 43,
    kGdJobCarpenterSearchForWork = 44,
    kGdJobMinerWalkHome = 45,
    kGdJobMinerCheckDrop = 46,
    kGdJobSmelterSearchForWork = 47,
    kGdJobSmelterWork = 48,
    kGdJobMinerWalkToDeliverGood = 49,
    kGdJobCarrierCheckDrop = 50,
    kGdJobGathererSearchResource = 51,
    kGdJobShrineWorkerSearchForWork = 52,
    kGdJobShrineWorkerWalkToMana = 53,
    kGdJobShrineWorkerTakeMana = 54,
    kGdJobShrineWorkerWalkHome = 55,
    kGdJobFoodWorkerSearchForWork = 56,
    kGdJobFoodWorkerWork = 57,
    kGdJobGathererWalkToResource = 58,
    kGdJobGathererCheckResource = 59,
    kGdJobGathererWork = 60,
    kGdJobGathererWalkHome = 61,
    kGdJobPriestSearchForWork = 62,
    kGdJobPriestWork = 63,
    kGdJobFarmerCheckDrop = 64,
    kGdJobFarmerCheckHarvest = 65,
    kGdJobGathererWalkToDeliverGood = 66,
    kGdJobGathererCheckDrop = 67,
    kGdJobEnterBuilding = 68,
    kGdJobExitBuilding = 69,
    kGdJobHunterCheckTarget = 70,
    kGdJobHitTargetRange1 = 71,
    kGdJobStoneMinerWalkToDeliverGood = 72,
    kGdJobWalkToObject = 73,
    kGdJobHitTargetRange2 = 74,
    kGdJobForesterCheckPlant = 75,
    kGdJobForesterSearchForWork = 76,
    kGdJobForesterWalkToWork = 77,
    kGdJobForesterPlant = 78,
    kGdJobForesterWalkHome = 79,
    kGdJobCorpseCollectorWalkHome = 80,
    kGdJobMeleeAbility = 81,
    kGdJobFarmerWaitForWork = 82,
    kGdJobOrcRegenerate = 83,
    kGdJobFarmerWalkToSow = 84,
    kGdJobFarmerSow = 85,
    kGdJobFarmerWalkHomeSow = 86,
    kGdJobCorpseCollectorCutCorpse = 87,
    kGdJobCorpseCollectorWalkToDeliverGood = 88,
    kGdJobFarmerWalkToHarvest = 89,
    kGdJobFarmerHarvest = 90,
    kGdJobFarmerWalkHomeHarvest = 91,
    kGdJobCorpseCollectorCheckDrop = 92,
    kGdJobWoodCutterWalkToDeliverGood = 93,
    kGdJobShrineWorkerCheckMana = 94,
    kGdJobFisherSearchForWork = 95,
    kGdJobFisherWalkToWork = 96,
    kGdJobFisherCheckResource = 97,
    kGdJobFisherWork = 98,
    kGdJobFisherWalkHome = 99,
    kGdJobFisherWalkToDeliverGood = 100,
    kGdJobFisherCheckDrop = 101,
    kGdJobWalkToPortal = 102,
    kGdJobPreCast = 103,
    kGdJobBuilderSearchForWork = 104,
    kGdJobReleaseDelay = 105,
    kGdJobPetIdle = 106,
    kGdJobPetWalkToMaster = 107,
    kGdJobAnimalIdle = 108,
    kGdJobAnimalWalkToNewPlace = 109,
    kGdJobWalkToNPC = 110,
    kGdJobCriticalHit = 111,
    kGdJobMinerSearchResource = 112,
    kGdJobMinerWork = 113,
    kGdJobMinerStoopToDropGood = 114,
    kGdJobFeignDeath = 115,
    kGdJob116 = 116,
    kGdJob117 = 117,
    kGdJob118 = 118,
    kGdJobSmithSearchForWork = 119,
    kGdJobSmithWork = 120,
    kGdJobHunterCheckCorpse = 121,
    kGdJobHunterCutCorpse = 122,
    kGdJobHunterCheckDrop = 123,
    kGdJobCorpseRot = 124,
    kGdJobManualWalkToTarget = 125,
    kGdJobHunterSearchForWork = 126,
    kGdJobHunterWalkToTarget = 127,
    kGdJobHunterHitTarget = 128,
    kGdJobHunterWalkToCorpse = 129,
    kGdJob130 = 130,
    kGdJob131 = 131,
    kGdJobHunterWalkHome = 132,
    kGdJobHunterWalkToDeliverGood = 133,
    kGdJobCattleBreederSearchForWork = 134,
    kGdJobCattleBreederFeed = 135,
    kGdJob136 = 136,
    kGdJobCorpseRotWithLoot = 137,
    kGdJobCastResolve = 138,
    kGdJobWalkToMaster = 139,
    kGdJobCheckBattleSleep = 140,
    kGdJobWalkToAttackBuilding = 141,
    kGdJobWarTowerIdle = 142,
    kGdJobGotHit = 143,
    kGdJobWalkBack = 144,
    kGdJobStrafeLeft = 145,
    kGdJobStrafeRight = 146,
    kGdJobStoop = 147,
    kGdJobStandup = 148,
    kGdJobStrike = 149,
    kGdJobStab = 150,
    kGdJobPunch = 151,
} FigureJobs;

// Declare the function pointers for the FigureFunctions group
DECLARE_FUNCTION(bool, isAlive, SF_CGdFigure *figure, uint16_t target);
DECLARE_FUNCTION(bool, setWalkSpeed, SF_CGdFigure *figure, uint16_t target, uint16_t value);
DECLARE_FUNCTION(bool, addAction, SF_CGdFigure *figure, uint16_t target, void *maybe_action);
DECLARE_FUNCTION(void, addBonusMultToStatistic, SF_CGdFigure *figure, StatisticDataKey key, uint16_t target, int8_t value);
DECLARE_FUNCTION(uint8_t, addBonusMult, FigureStatistic *statistic, uint8_t value);
DECLARE_FUNCTION(uint16_t, getCurrentHealth, SF_CGdFigure *figure, uint16_t figure_id);
DECLARE_FUNCTION(void, decreaseHealth, SF_CGdFigure *figure, uint16_t figure_id, uint16_t amount);
DECLARE_FUNCTION(uint16_t, getCurrentMaxMana, SF_CGdFigure *figure, uint16_t figure_id);
DECLARE_FUNCTION(uint16_t, getCurrentMaxHealth, SF_CGdFigure *figure, uint16_t figure_id);
DECLARE_FUNCTION(void, setJobToDoCount, SF_CGdFigure *figure, uint16_t target_index, uint16_t value);

DECLARE_FUNCTION(void, rescaleMana, SF_CGdFigure *figure, uint16_t figure_id, uint16_t max_mana);
DECLARE_FUNCTION(void, rescaleHealth, SF_CGdFigure *figure, uint16_t figure_id, uint16_t max_health);
DECLARE_FUNCTION(uint16_t, getJob, SF_CGdFigure *figure, uint16_t figure_id);
DECLARE_FUNCTION(uint16_t, getAggroValue, SF_CGdFigure *, uint16_t figure_id, uint16_t target_index);
DECLARE_FUNCTION(void, setAggroValue, SF_CGdFigure *, uint16_t figure_id, uint16_t target_index, uint16_t aggroValue, uint32_t unkn);

DECLARE_FUNCTION(bool, isFlagSet, SF_CGdFigure *figure, uint16_t figure_index, GdFigureFlags flag);
DECLARE_FUNCTION(uint16_t, getSpellJobStartNode, SF_CGdFigure *figure, uint16_t figure_index);
DECLARE_FUNCTION(void, subMana, SF_CGdFigure *_this, uint16_t param_1, uint32_t param_2);
DECLARE_FUNCTION(uint16_t, getCurrentMana, SF_CGdFigure *figure, uint16_t figure_id);
DECLARE_FUNCTION(SF_Coord, getPosition, SF_CGdFigure *figure, SF_Coord *position, uint16_t figure_id);
DECLARE_FUNCTION(SF_CGdFigureWeaponStats *, getWeaponStats, SF_CGdFigure *figure, SF_CGdFigureWeaponStats *stats_in, uint16_t figure_id);
DECLARE_FUNCTION(bool, isWarrior, SF_CGdFigure *figure, uint16_t figure_id);
DECLARE_FUNCTION(void, setTask, SF_CGdFigure *_figure, uint16_t figure_index, CGdFigureTask figureTask);
// Unknown Functons
DECLARE_FUNCTION(bool, FUN_006e3a90, uint32_t *CGdFigureJobs, uint16_t figure_id);

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
                       rescaleHealth_ptr rescaleHealth;
                       getJob_ptr getJob;
                       isFlagSet_ptr isFlagSet;
                       getSpellJobStartNode_ptr getSpellJobStartNode;
                       setJobToDoCount_ptr setJobToDoCount;
                       FUN_006e3a90_ptr FUN_006e3a90;
                       subMana_ptr subMana;
                       getCurrentMana_ptr getCurrentMana;
                       getPosition_ptr getPosition;
                       setTask_ptr setTask;
                       getWeaponStats_ptr getWeaponStats;
                       getAggroValue_ptr getAggroValue;
                       setAggroValue_ptr setAggroValue;
                       isWarrior_ptr isWarrior;);
