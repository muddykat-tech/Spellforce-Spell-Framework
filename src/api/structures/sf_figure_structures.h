#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "../sf_general_structures.h"

typedef struct __attribute__((packed))
{
    uint16_t figure_index;
    uint16_t agro_value;
    uint16_t hate_value;
} CGdFigureHateEntry;

typedef struct __attribute__((packed))
{
    uint16_t min_dmg;
    uint16_t max_dmg;
    uint16_t min_rng;
    uint16_t max_rng;
    uint16_t wpn_spd;
    uint16_t wpn_type;
    uint16_t wpn_mat;
} SF_CGdFigureWeaponStats;

typedef struct __attribute__((packed))
{
    uint16_t base_val;
    int16_t bonus_val;
    int8_t bonus_multiplier;
    uint8_t statistic_type;  // NOT CONFIRMED
} FigureStatistic;

typedef struct __attribute__((packed))
{
    uint16_t base_val;
    int16_t bonus_val;
    int8_t bonus_multiplier;
    uint8_t statistic_type;  // NOT CONFIRMED?
    uint16_t missing_val;    // No idea what this is
} FigureStatisticExt;

typedef struct __attribute__((packed))
{
    uint16_t GdJobId;
    CGdFigureTask task;
    uint8_t padding;
    CGdFigureJobFlags flags;
    uint8_t pathing_mode;
    SF_CGdTargetData target;  // it is stored WEIRDLY
    uint8_t SGtFigureAction[0xc];
    uint16_t xdata_key;
    uint8_t walking_mode;
    uint8_t padding2;
    SF_Coord walk_to_pos;
    uint8_t unknown[14];
    uint8_t current_waypoint;
} FigureJobData;

typedef struct __attribute__((packed))
{
    SF_Coord position;
    SF_Coord destination;
    uint16_t to_do_count;
    uint16_t to_do_count_remainder;
    uint16_t anim_length;
    uint16_t anim_length_remainder;
    uint8_t activity;
    uint8_t unknown1;
    uint16_t building;
    GdFigureFlags flags;
    uint8_t race;
    uint8_t level;
    uint16_t owner;
    uint8_t min_attack_range;
    uint8_t max_attack_range;
    uint16_t master_figure;
    uint16_t group_leader;
    uint32_t npc_id;
    uint16_t unit_data_id;
    FigureStatistic armor;
    FigureStatistic agility;
    FigureStatistic charisma;
    FigureStatistic dexterity;
    FigureStatisticExt health;
    FigureStatistic intelligence;
    FigureStatisticExt mana_stuff;
    FigureStatisticExt stamina;
    FigureStatistic strength;
    FigureStatistic wisdom;
    FigureStatistic resistance_fire;
    FigureStatistic resistance_ice;
    FigureStatistic resistance_mental;
    FigureStatistic resistance_black;
    FigureStatistic walk_speed;
    FigureStatistic fight_speed;
    FigureStatistic cast_speed;
    uint16_t equipment[16];  // No idea how this works, may need a class (undefined2[16] in ghidra, but is 20 bytes FigureStatistic is 6 bytes)
    uint16_t head;           // Not sure what this does
    uint32_t unknown2[3];    // three 4 byte data points in a row, no name known for these.
    uint8_t unknown3[168];   // Many 1 byte sections in a row
    uint32_t unknown4[7];    // Many 4 byte sections in a row
    uint16_t unknown5;
    uint8_t unknown6[2];
    SF_CGdFigureWeaponStats weapon_stats;
    uint8_t unknown7[13];
    uint8_t good;  // I assume perhaps alignment?
    uint8_t direction;
    uint8_t unknown8;
    uint8_t path_dir;
    uint8_t path_wish_dir;
    uint32_t path_bits;
    uint8_t unknown9;
    uint32_t unknown10[3];
    uint16_t unknown11;
    CGdFigureHateEntry hate_entry[10];
    uint8_t debug_flags;
    uint8_t unknown12;
    uint16_t formation;
    uint8_t faction;
    uint8_t unknown13;
    uint32_t clan_relations;
    uint16_t unknown14;
    uint32_t padding1;
    FigureJobData current_job; // Current Active Job Data
    uint8_t padding2[3];
    FigureJobData next_job; // Feign Death, Acting in Buildings? Seems to be about the Next Target Job Data?
    uint8_t padding3[3];
    FigureJobData ac_4;
    uint8_t padding4[3];
    uint32_t unknown15[3];  // Skipping some variable sections, contains differences Also Check what FigureJobData is, it's used multiple times in this section
    uint8_t dwarf_rank;
    uint8_t set_type;
} GdFigure;

/**
 * @brief A structure for the global list of figures and related statistics for them
 */
typedef struct __attribute__((packed))
{
    uint16_t max_used;
    GdFigure figures[2000];
} SF_CGdFigure;

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
