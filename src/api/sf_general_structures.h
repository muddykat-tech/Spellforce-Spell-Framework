/**
 * @file sf_general_structures.h
 * @brief Group of Structures and Enums used throught SFSF
 * @ingroup API
 */

 #pragma once
 #include <stdint.h>
 #include <stdbool.h>


/* |-========== General Structures ==========-| */

typedef struct SF_CGdSpell SF_CGdSpell;
typedef struct SF_CGdFigureToolbox SF_CGdFigureToolbox;
typedef struct SF_GdEffect SF_GdEffect;
typedef struct SF_CGDEffect SF_CGDEffect;
typedef struct SF_CGdWorld SF_CGdWorld;
typedef struct SF_CGdWorldToolBox SF_CGdWorldToolBox;
typedef struct SF_CGdFigureJobs SF_CGdFigureJobs;
typedef struct SF_CGdBuilding SF_CGdBuilding;
typedef struct SF_CGdBuildingToolbox SF_CGdBuildingToolbox;
typedef struct CMnuLabel CMnuLabel;
typedef struct SF_CGdFigure SF_CGdFigure;
typedef struct SF_SpellEffectInfo SF_SpellEffectInfo;
typedef struct SF_CGdResource SF_CGdResource;
typedef struct SF_CGdPlayer SF_CGdPlayer;

/* |-========== Enums ==========-| */

typedef enum
{
    DEBUG_INFO = 0,    // No debug
    DEBUG_LOW  = 1,
    DEBUG_MED  = 2,
    DEBUG_HIGH = 3,
    DEBUG_ALL  = 4     // Verbose
} DebugLevel;

typedef enum : uint8_t
{
    CHECK_SPELLS_BEFORE_CHECK_BATTLE = 0x04,
    CHECK_SPELLS_BEFORE_JOB2 = 0x02,
    UNFREEZE = 0x1
} SpellFlagKey;

typedef enum : uint16_t
{
    kGdSpellLineFireBurst = 1,
    kGdSpellLineHealing = 2,
    kGdSpellLineDeath = 3,
    kGdSpellLineSlowness = 4,
    kGdSpellLinePoison = 5,
    kGdSpellLineInvulnerability = 6,
    kGdSpellLineCurePoison = 7,
    kGdSpellLineFreeze = 9,
    kGdSpellLineFog = 10,
    kGdSpellLineIlluminate = 11,
    kGdSpellLineFireShield = 12,
    kGdSpellLineFireBall = 13,
    kGdSpellLineIceBurst = 14,
    kGdSpellLineIceShield = 15,
    kGdSpellLineDecay = 16,
    kGdSpellLineDecayArea = 17,
    kGdSpellLinePain = 18,
    kGdSpellLineLifeTap = 19,
    kGdSpellLineSummonGoblin = 20,
    kGdSpellLineHypnotize = 21,
    kGdSpellLineIceShieldStun = 22,
    kGdSpellLinePestilence = 23,
    kGdSpellLineCureDisease = 24,
    kGdSpellLinePetrify = 25,
    kGdSpellLinePainArea = 28,
    kGdSpellLineSummonSkeleton = 29,
    kGdSpellLineRaiseDead = 30,
    kGdSpellLineSummonDemon = 31,
    kGdSpellLineDeathGrasp = 32,
    kGdSpellLineSummonChanneler = 33,
    kGdSpellLineInflexibility = 34,
    kGdSpellLineWeaken = 35,
    kGdSpellLineDarkBanishing = 36,
    kGdSpellLineSlownessArea = 37,
    kGdSpellLineInflexibilityArea = 38,
    kGdSpellLineWeakenArea = 39,
    kGdSpellLinePlagueArea = 40,
    kGdSpellLineRemediless = 41,
    kGdSpellLineDarkMight = 42,
    kGdSpellLineHealingArea = 43,
    kGdSpellLineSentinelHealing = 44,
    kGdSpellLineGreaterHealing = 45,
    kGdSpellLineCharmAnimal = 46,
    kGdSpellLineThornShield = 47,
    kGdSpellLineQuickness = 48,
    kGdSpellLineQuicknessArea = 49,
    kGdSpellLineFlexibility = 50,
    kGdSpellLineFlexibilityArea = 51,
    kGdSpellLineStrengthen = 52,
    kGdSpellLineStrengthenArea = 53,
    kGdSpellLineGuard = 54,
    kGdSpellLineRemoveCurse = 55,
    kGdSpellLineRegenerate = 56,
    kGdSpellLineHolyMight = 57,
    kGdSpellLineHallow = 58,
    kGdSpellLineFireShieldDamage = 60,
    kGdSpellLineThornShieldDamage = 61,
    kGdSpellLineForget = 62,
    kGdSpellLineSelfIllusion = 63,
    kGdSpellLineRetention = 64,
    kGdSpellLineBrilliance = 65,
    kGdSpellLineSacrificeMana = 66,
    kGdSpellLineManaTap = 67,
    kGdSpellLineManaDrain = 68,
    kGdSpellLineShock = 69,
    kGdSpellLineDisrupt = 70,
    kGdSpellLineFear = 71,
    kGdSpellLineConfuse = 72,
    kGdSpellLineRainOfFire = 73,
    kGdSpellLineBlizzard = 74,
    kGdSpellLineAcidCloud = 75,
    kGdSpellLineStoneRain = 76,
    kGdSpellLineWallOfRocks = 77,
    kGdSpellLineRingOfRocks = 78,
    kGdSpellLineAmok = 79,
    kGdSpellLineExtinct = 81,
    kGdSpellLineDetectMetal = 82,
    kGdSpellLineDetectMagic = 83,
    kGdSpellLineArrow = 84,
    kGdSpellLineBerserk = 85, // FAKE BERSERK
    kGdSpellLineInvisibility = 86,
    kGdSpellLineStone = 87,
    kGdSpellLineAuraWeakness = 88,
    kGdSpellLineAuraSuffocation = 89,
    kGdSpellLineSuicideDeath = 90,
    kGdSpellLineAuraLifeTap = 91,
    kGdSpellLineSummonSpectre = 92,
    kGdSpellLineFeignDeath = 93,
    kGdSpellLineAuraSlowFighting = 94,
    kGdSpellLineAuraInflexibility = 95,
    kGdSpellLineDispelWhiteAura = 96,
    kGdSpellLineAuraSlowWalking = 97,
    kGdSpellLineAuraInability = 98,
    kGdSpellLineSuffocation = 99,
    kGdSpellLineInability = 100,
    kGdSpellLineSlowFighting = 101,
    kGdSpellLineAuraStrength = 102,
    kGdSpellLineAuraHealing = 103,
    kGdSpellLineAuraEndurance = 104,
    kGdSpellLineSuicideHeal = 105,
    kGdSpellLineSummonWolf = 106,
    kGdSpellLineAuraRegeneration = 107,
    kGdSpellLineDominateAnimal = 108,
    kGdSpellLineSummonBear = 109,
    kGdSpellLineAuraFastFighting = 110,
    kGdSpellLineAuraFlexibility = 111,
    kGdSpellLineDispelBlackAura = 112,
    kGdSpellLineAuraFastWalking = 113,
    kGdSpellLineAuraLight = 114,
    kGdSpellLineAuraDexterity = 115,
    kGdSpellLineDexterity = 116,
    kGdSpellLineEndurance = 117,
    kGdSpellLineFastFighting = 118,
    kGdSpellLineDistract = 119,
    kGdSpellLineDominate = 120,
    kGdSpellLineObjectIllusion = 121,
    kGdSpellLineCharm = 122,
    kGdSpellLineBefriend = 123,
    kGdSpellLineDisenchant = 124,
    kGdSpellLineCharisma = 125,
    kGdSpellLineShockwave = 126,
    kGdSpellLineAuraHypnotization = 127,
    kGdSpellLineDemoralization = 128,
    kGdSpellLineAuraBrilliance = 129,
    kGdSpellLineEnlightenment = 130,
    kGdSpellLineAuraManaTap = 131,
    kGdSpellLineMeditation = 132,
    kGdSpellLineFireElemental = 133,
    kGdSpellLineWaveOfFire = 134,
    kGdSpellLineMeltResistance = 135,
    kGdSpellLineIceElemental = 136,
    kGdSpellLineWaveOfIce = 137,
    kGdSpellLineChillResistance = 138,
    kGdSpellLineRockBullet = 139,
    kGdSpellLineConservation = 140,
    kGdSpellLineEarthElemental = 141,
    kGdSpellLineWaveOfRocks = 142,
    kGdSpellLineArrowTower = 143,
    kGdSpellLineHealingTower = 144,
    kGdSpellLineIceburstTower = 145,
    kGdSpellLineLifeTapAura = 146,
    kGdSpellLineFireBallEffect = 147,
    kGdSpellLineAbilityWarCry = 148,
    kGdSpellLineAbilityBenefactions = 149,
    kGdSpellLineAbilityPatronize = 150,
    kGdSpellLineAbilityEndurance = 151,
    kGdSpellLineAbilityBerserk = 152,
    kGdSpellLineAbilityBlessing = 153,
    kGdSpellLineAbilityShelter = 154,
    kGdSpellLineAbilityDurability = 155,
    kGdSpellLineAbilityTrueShot = 156,
    kGdSpellLineAbilitySteelSkin = 157,
    kGdSpellLineAbilitySalvo = 158,
    kGdSpellLineFireBurstTower = 159,
    kGdSpellLineSpark = 160,
    kGdSpellLineHypnotizeTower = 161,
    kGdSpellLinePainTower = 162,
    kGdSpellLineStoneTower = 163,
    kGdSpellLineProtectionBlack = 164,
    kGdSpellLineDoNotTouchMe = 165,
    kGdSpellLineHealingAura = 166,
    kGdSpellLineHypnotizeTwo = 167,
    kGdSpellLineIceArrowEffect = 168,
    kGdSpellLineIceBlockEffect = 169,
    kGdSpellLineFireBlockEffect = 170,
    kGdSpellLineExtinctTower = 171,
    kGdSpellLineManaTapAura = 172,
    kGdSpellLineFireResistance = 173,
    kGdSpellLineEssenceBlack = 174,
    kGdSpellLineEssenceWhite = 175,
    kGdSpellLineEssenceElemental = 176,
    kGdSpellLineEssenceMental = 177,
    kGdSpellLineAlmightinessBlack = 178,
    kGdSpellLineAlmightinessWhite = 179,
    kGdSpellLineAlmightinessElemental = 180,
    kGdSpellLineAlmightinessMental = 181,
    kGdSpellLineAlmightinessElementalEffect = 182,
    kGdSpellLineEssenceElementalEffect = 183,
    kGdSpellLineAssistance = 184,
    kGdSpellLineHolyTouch = 185,
    kGdSpellLineRevenge = 186,
    kGdSpellLineRootsArea = 187,
    kGdSpellLineSummonTreeWraith = 188,
    kGdSpellLineRoots = 189,
    kGdSpellLineChainHallow = 190,
    kGdSpellLineReinforcement = 191,
    kGdSpellLineAuraEternity = 192,
    kGdSpellLineChainPain = 193,
    kGdSpellLineCannibalize = 194,
    kGdSpellLineTorture = 195,
    kGdSpellLineChainLifetap = 196,
    kGdSpellLineDominateUndead = 197,
    kGdSpellLineSummonBlade = 198,
    kGdSpellLineMutation = 199,
    kGdSpellLineDarknessArea = 200,
    kGdSpellLineChainMutation = 201,
    kGdSpellLineChainFireburst = 202,
    kGdSpellLineSummonFireGolem = 203,
    kGdSpellLineChainFireball = 204,
    kGdSpellLineChainIceburst = 205,
    kGdSpellLineSummonIceGolem = 206,
    kGdSpellLineFreezeArea = 207,
    kGdSpellLineChainRockBullet = 208,
    kGdSpellLineSummonEarthGolem = 209,
    kGdSpellLineFeetClay = 210,
    kGdSpellLineMirrorImage = 211,
    kGdSpellLineChainCharm = 212,
    kGdSpellLineVoodoo = 213,
    kGdSpellLineChainShock = 214,
    kGdSpellLineHypnotizeArea = 215,
    kGdSpellLineConfuseArea = 216,
    kGdSpellLineChainManatap = 217,
    kGdSpellLineManaShield = 218,
    kGdSpellLineShiftMana = 219,
    kGdSpellLineAbilityShiftLife = 220,
    kGdSpellLineAbilityRiposte = 221,
    kGdSpellLineAbilityCriticalHits = 222,
    kGdSpellLineAuraSiegeHuman = 223,
    kGdSpellLineAuraSiegeElf = 225,
    kGdSpellLineAuraSiegeOrc = 226,
    kGdSpellLineAuraSiegeTroll = 227,
    kGdSpellLineAuraSiegeDarkElf = 228,
    kGdSpellLineEternity = 229,
    kGdSpellLineHallowChained = 230,
    kGdSpellLineLifeTapChained = 231,
    kGdSpellLineManaTapChained = 232,
    kGdSpellLineMutationChained = 233,
    kGdSpellLineFireBurstChained = 234,
    kGdSpellLineIceBurstChained = 235,
    kGdSpellLineRockBulletChained = 236,
    kGdSpellLineCharmChained = 237,
    kGdSpellLineShockChained = 238,
    kGdSpellLineFireBallChained = 239,
    kGdSpellLinePainChained = 240,
    kGdSpellLineFakeSpellOneFigure = 241
} GdSpellLine;

typedef enum : uint16_t
{
    kGdSpellJobFireburst=1,
    kGdSpellJobHealing=2,
    kGdSpellJobDeath=3,
    kGdSpellJobPoison=4,
    kGdSpellJobCurePoison=5,
    kGdSpellJobInvulerability=6,
    kGdSpellJobUnknown1=7,
    kGdSpellJobIcestrike1=8,
    kGdSpellJobFog=9,
    kGdSpellJobStone=10,
    kGdSpellJobSlowness=11,
    kGdSpellJobFreeze=12,
    kGdSpellJobDecay=13,
    kGdSpellJobPain=14,
    kGdSpellJobIceShield=15,
    kGdSpellJobIlluminate=16,
    kGdSpellJobFireBall1=17,
    kGdSpellJobFireShield1=18,
    kGdSpellJobIceShield2=19,
    kGdSpellJobLifetap=20,
    kGdSpellJobDecay2=21,
    kGdSpellJobIceshield3=22,
    kGdSpellJobSummon=23,
    kGdSpellJobPetrify=24,
    kGdSpellJobPestilence=25,
    kGdSpellJobCureDisease=26,
    kGdSpellJobPainArea=27,
    kGdSpellJobDeathGrasp=28,
    kGdSpellJobInflexibility=29,
    kGdSpellJobDetectMagic=30,
    kGdSpellJobWeaken=31,
    kGdSpellJobDarkBanishing=32,
    kGdSpellJobSlownessArea=33,
    kGdSpellJobInflexibilityArea=34,
    kGdSpellJobWeakenArea=35,
    kGdSpellJobPlaugeArea=36,
    kGdSpellJobRemediless=37,
    kGdSpellJobHealingArea=38,
    kGdSpellJobSentinelHealing=39,
    kGdSpellJobQuickness=40,
    kGdSpellJobQuicknessArea=41,
    kGdSpellJobThornshield=42,
    kGdSpellJobFlexibility=43,
    kGdSpellJobFlexibilityArea=44,
    kGdSpellJobStrength=45,
    kGdSpellJobStrengthArea=46,
    kGdSpellJobGuard=47,
    kGdSpellJobRegenerate=48,
    kGdSpellJobHallow=49,
    kGdSpellJobFireshield2=50,
    kGdSpellJobThorns=51,
    kGdSpellJobRaiseDead=52,
    Job0x35=53,
    Job0x36=54,
    kGdSpellJobForget=55,
    kGdSpellJobRetention=56,
    kGdSpellJobBrilliance=57,
    kGdSpellJobSacrificeMana=58,
    kGdSpellJobManaTap=59,
    kGdSpellJobManaDrain=60,
    kGdSpellJobAmok=61,
    kGdSpellJobConfuse=62,
    kGdSpellJobDisrupt=63,
    kGdSpellJobAcidCloud=64,
    kGdSpellJobRainOfFire=65,
    kGdSpellJobShock=66,
    kGdSpellJobBlizzard=67,
    kGdSpellJobRainOfStone=68,
    kGdSpellJobCharmAnimal=69,
    kGdSpellJobInvisibility=70,
    kGdSpellJobExtinct=71,
    kGdSpellJobRemoveCurse=72,
    kGdSpellJobAura=73,
    kGdSpellJobSuffocation=74,
    kGdSpellJobSlowFighting=75,
    kGdSpellJobInability=76,
    kGdSpellJobSuicideDeath=77,
    kGdSpellJobEndurance=78,
    kGdSpellJobFastFighting=79,
    kGdSpellJobDexterity=80,
    kGdSpellJobSuicideHeal=81,
    kGdSpellJobDispelWhiteAura=82,
    kGdSpellJobDispelBlackAura=83,
    kGdSpellJobDominateAnimal=84,
    kGdSpellJobWave=85,
    kGdSpellJobRockBullet=86,
    kGdSpellJobMeltResistance=87,
    kGdSpellJobChillResistance=88,
    kGdSpellJobCharisma=89,
    kGdSpellJobEnlightement=90,
    kGdSpellJobMeditation=91,
    kGdSpellJobShockwave=92,
    kGdSpellJobBefriend=93,
    kGdSpellJobDominate=94,
    kGdSpellJobCharm=95,
    kGdSpellJobTowerArrow=96,
    kGdSpellJobHealingAura=97,
    kGdSpellJobIceStrike2=98,
    kGdSpellJobDistract=99,
    kGdSpellJobDetectMetal=100,
    kGdSpellJobConservation=101,
    kGdSpellJobLifeTapAura=102,
    kGdSpellJobFireball2=103,
    kGdSpellJobSelfIllusion=104,
    kGdSpellJobAbilityWarCry=105,
    kGdSpellJobAbilityBenefactions=106,
    kGdSpellJobAbilityPatronize=107,
    kGdSpellJobAbilityEndurance=108,
    kGdSpellJobAbilityBerserk=109,
    kGdSpellJobAbilityBoons=110,
    kGdSpellJobAbilityShelter=111,
    kGdSpellJobAbilityDurability=112,
    kGdSpellJobAbilityTrueShot=113,
    kGdSpellJobAbilitySteelskin=114,
    kGdSpellJobAbilitySalvo=115,
    kGdSpellJobSpark=116,
    kGdSpellJobDemoralization=117,
    kGdSpellJobHypnotize=118,
    kGdSpellJobTowerPain=119,
    kGdSpellJobTowerStone=120,
    kGdSpellJobCloakOfNor=121,
    kGdSpellJobFeignDeath=122,
    kGdSpellJobDominateBreak=123,
    kGdSpellJobDisenchant=124,
    Unknown2_0x7d=125,
    kGdSpellJobIce1=126,
    kGdSpellJobIce2=127,
    kGdSpellJobFireBullet=128,
    kGdSpellJobTowerExtinct=129,
    kGdSpellJobManaTapAura=130,
    kGdSpellJobFireResistance=131,
    kGdSpellJobEssenceBlack=132,
    kGdSpellJobAlmightinessBlack=133,
    kGdSpellJobEssenceWhite=134,
    kGdSpellJobAlmightinessWhite=135,
    kGdSpellJobEssenceElemental=136,
    kGdSpellJobAlmightinessElemental=137,
    kGdSpellJobEssenceMental=138,
    kGdSpellJobAlmightinessMental=139,
    kGdSpellJobEssenceElemental2=140,
    kGdSpellJobAlmightinessMental2=141,
    kGdSpellJobAssistance=142,
    kGdSpellJobHolyTouch=143,
    kGdSpellJobRootsArea=144,
    kGdSpellJobEternity=145,
    kGdSpellJobCannibalize=146,
    kGdSpellJobDominateUndead=147,
    kGdSpellJobDarknessArea=148,
    kGdSpellJobFreezeArea=149,
    kGdSpellJobShiftMana=150,
    kGdSpellJobRevenge=151,
    kGdSpellJobTorture=152,
    kGdSpellJobReinforcement=153,
    kGdSpellJobHypnotizeArea=154,
    kGdSpellJobFeetOfClay=155,
    kGdSpellJobConfuseArea=156,
    kGdSpellJobMutation=157,
    kGdSpellJobAbilityShiftLife=158,
    kGdSpellJobAbilityRiposte=159,
    kGdSpellJobAbilityCriticalHits=160,
    kGdSpellJobAbilityManashield=161,
    kGdSpellJobChain=162,
    kGdSpellJobRoots=163,
    kGdSpellJobMirage=164,
    kGdSpellJobFakeSpellOneFigure=165,
    kGdSpellJobFeedback=166,
    kGdSpellJobSiegeAura=167
} GdSpellJob;

typedef enum
{
    BUILDING_POSITION_X = 0x01,
    BUILDING_POSITION_Y = 0x02,
    WORKER_HOST_BUILDING_POS_X = 0x3,
    WORKER_HOST_BUILDING_POS_Y = 0x4,
    SPELL_TICK_COUNT_AUX = 0x05,
    EFFECT_EFFECT_INDEX = 0x06,
    EFFECT_SPELL_ID = 0x09,
    SPELL_STAT_MUL_MODIFIER = 0x0A,
    SPELL_CONSERVATION_SHIELD = 0x0B,
    SPELL_PESTILENCE_DAMAGE = 0x0E,
    WORKER_DELIVERY_BUILDING = 0x10,
    EFFECT_SPELL_INDEX = 0x11,
    SPELL_TICK_COUNT = 0x12,
    EFFECT_ENTITY_TYPE2 = 0x13,
    BUILDING_EFFECT_ID = 0x16,
    EFFECT_ENTITY_INDEX2 = 0x1A,
    EFFECT_SUBSPELL_ID = 0x1C,
    EFFECT_PHYSICAL_DAMAGE = 0x1E,
    BUILDING_ANIM_LENGTH = 0x20,
    EFFECT_MANA_COST = 0x21,
    EFFECT_SPELL_LINE = 0x24,
    BUILDING_ANIM_START = 0x25,
    SPELL_DOUBLE_DAMAGE = 0x26,
    BUILDING_QUEUE_COUNT = 0x28,
    SPELL_STAT_MUL_MODIFIER2 = 0x27,
    SPELL_STAT_MUL_MODIFIER3 = 0x2B,
    SPELL_STAT_MUL_MODIFIER4 = 0x2C,
    EFFECT_ENTITY_INDEX3 = 0x2D,
    EFFECT_ENTITY_INDEX = 0x2F,
    EFFECT_ENTITY_TYPE = 0x30,
    EFFECT_DO_NOT_ADD_SUBSPELL = 0x33,
} SpellDataKey;

typedef enum
{
    kGdEffectNone = 0,
    kGdEffectSpellCast = 1,
    kGdEffectSpellHitWorld = 2,
    kGdEffectSpellHitTarget = 3,
    kGdEffectSpellDOTHitTarget = 4,
    kGdEffectSpellMissTarget = 5,
    kGdEffectSpellResolve = 6,
    kGdEffectSummonWorker = 7,
    kGdEffectWorkerAppears = 8,
    kGdEffectSummonHero = 9,
    kGdEffectHeroAppears = 10,
    kGdEffectSpellTargetResisted = 11,
    kGdEffectSpellResolveSelf = 12,
    kGdEffectMeteorFall = 13,
    kGdEffectMeteorHit = 14,
    kGdEffectBlizzardFall = 15,
    kGdEffectBlizzardHit = 16,
    kGdEffectStoneFall = 17,
    kGdEffectStoneHit = 18,
    kGdEffectPetAppears = 19,
    kGdEffectTest = 20,
    kGdEffectMonumentClaimed = 21,
    kGdEffectMonumentWorking = 22,
    kGdEffectAuraResolve = 23,
    kGdEffectProjectile = 24,
    kGdEffectBuilding = 25,
    kGdEffectPlayerBind = 26,
    kGdEffectSummonMainChar = 27,
    kGdEffectMainCharAppears = 28,
    kGdEffectTitanProduction = 29,
    kGdEffectTitanAppears = 30,
    kGdEffectMentalTowerCast = 31,
    kGdEffectMentalTowerIdle = 32,
    kGdEffectMonumentBullet = 33,
    kGdEffectMonumentHitFigure = 34,
    kGdEffectSpellAssistanceHitFigure = 35,
    kGdEffectChainResolve = 36,
    kGdEffectSpellVoodooHitFigure = 37,
    kGdEffectSpellManaShieldHitFigure = 38,
    kGdEffectMax = 39
} CGdEffectType;


typedef enum
{
    ARMOR,
    AGILITY,
    CHARISMA,
    DEXTERITY,
    HEALTH,
    INTELLIGENCE,
    MANA,
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


typedef enum : uint8_t
{
    TASK_WORKER = 2,
    TASK_WOODCUTTER = 3,
    TASK_QUARRY = 4,
    TASK_MINE = 5,
    TASK_FORGE = 6,
    TASK_CARPENTER = 7,
    TASK_HERO = 9,
    TASK_MAINCHAR = 10,
    TASK_NPC = 11,
    TASK_PET = 12,
    TASK_HUNTER = 13,
    TASK_HUNTING_LODGE = 14,
    TASK_CATTLE_BREEDER = 15,
    TASK_FISHER = 16,
    TASK_MERCHANT = 17,
    TASK_MASTER_WOODCUTTER = 18,
    TASK_SHRINE = 21,
    TASK_PRIEST = 22,
    TASK_SMELTER = 23,
    TASK_STOREKEEPER = 24,
    TASK_GATHERER = 25,
    TASK_FARMER = 26,
    TASK_FORESTER = 27,
    TASK_SCAVENGER =29,
    TASK_MACE_CARVER = 30,
    TASK_MASTER_FISHER = 32
} CGdFigureTask;

typedef enum
{
    UNDEAD = 0x1,
    RESESRVED_ONLY = 0x2,
    AGGROED = 0x4,
    IS_DEAD = 0x8,
    REDO = 0x10,
    F_CHECK_SPELLS_BEFORE_JOB = 0x20,
    F_CHECK_SPELLS_BEFORE_CHECK_BATTLE = 0x40,
    WALK_JOB_WAIT = 0x80,
    FREEZED = 0x100,
    HAS_LOOT = 0x200,
    HAS_DIALOG = 0x400,
    FEMALE = 0x800,
    GOT_AGGRO = 0x1000,
    RETREAT = 0x2000,
    NO_WAY_TO_TARGET = 0x4000,
    AURA_RUNNING = 0x8000,
    AI_BLOCKED = 0x10000,
    TOWER = 0x20000,
    IS_SWAPPING = 0x40000,
    CUR_ACTIVE_DIALOG = 0x80000,
    IS_IN_FIGHT = 0x100000,
    VIEW_MODE_1ST_3RD = 0x200000,
    IS_TALKING = 0x400000,
    IS_IMPORTANT_DIALOG = 0x800000,
    UNKILLABLE = 0x1000000,
    FOLLOW_MODE = 0x2000000,
    HIT_LEFT_HAND_NEXT = 0x4000000,
    FOREST_SPIRIT = 0x8000000,
    VIP = 0x10000000,
    ILLUSION = 0x20000000,
    SPAWN = 0x40000000,
    USED_FOR_REVENGE = 0x80000000
} GdFigureFlags;

typedef enum : uint16_t
{
    MANUAL_JOB_CHANGE = 1,
    SKIP_ONCE = 2,
    MANUAL_HIT_TARGET = 4,
    CORPSE_CANT_ROT = 8,
    START_WALK = 16,
    RUN_MODE = 32,
    WAR = 64,
    CHECK_BATTLE = 128,
    PATROL_MODE = 256,
    WAY_POINTS_READ_REVERSE = 512,
    SUPERIOR_PATHING = 1024,
    ROUND_HIT = 2048,
    DEATH_BLOW = 4096,
    START_WORK_AT_BUILDING_FORCE_JOB = 8192,
} CGdFigureJobFlags;


/* |-========== Mod Structures ==========-| */

typedef struct __attribute__((packed))
{
    char mod_id[64];
    char mod_version[24];
    char mod_description[128];
    char mod_author[128];
    char mod_errors[256];  //Large Buffer for all error reporting needs.
} SFMod;

typedef void (*log_function_ptr)(const char *format, ...);
typedef struct __attribute__((packed))
{
    log_function_ptr logError;
    log_function_ptr logWarning;
    log_function_ptr logInfo;
} SFLog;

typedef enum : uint16_t
{
    NONE = 0x0,
    SUMMON_SPELL = 0x1,
    DOMINATION_SPELL = 0x2,
    CHAIN_SPELL = 0x4,
    WHITE_AURA_SPELL = 0x8,
    BLACK_AURA_SPELL = 0x10,
    TARGET_ONHIT_SPELL = 0x20,
    COMBAT_ABILITY_SPELL = 0x40,
    AOE_SPELL = 0x80,
    SIEGE_AURA_SPELL = 0x100,
    AURA_SPELL = 0x200,
    STACKABLE_SPELL = 0x400,
    SPELL_TAG_COUNT = 12
} SpellTag;

typedef enum : uint32_t
{
    HQ_BUILDING = 0x1,
    WOODCUTTER_BUILDING = 0x2,
    BREEDER_BUILDING = 0x4,
    QUARRY_BUILDING = 0x8,
    FOODSTORE_BUILDING = 0x10,
    HUNTER_BUILDING = 0x20,
    FISHER_BUILDING = 0x40,
    FARM_BUILDING = 0x80,
    MINER_BUILDING = 0x100,
    SMELTER_BUILDING = 0x200,
    SHRINE_BUILDING = 0x400,
    TEMPLE_BUILDING = 0x800,
    FORESTER_BUILDING = 0x1000,
    GATHERER_BUILDING = 0x2000,
    FORGE_BUILDING = 0x4000,
    MACE_CARVER_BUILDING = 0x8000,
    TOWER_BUILDING = 0x10000,
    MASTER_BUILDING = 0x20000,
    MOONSILVER_BUILDING = 0x40000,
    SAWMILL_BUILDING = 0x80000,
    SCAVENGER_BUILDING = 0x100000,
    STONEMASON_BUILDING = 0x200000,
    HABITABLE_BUILDING = 0x400000,
    HABITABLE_SINGLE_BUILDING = 0x800000
} BuildingTag;

typedef enum
{
    PHASE_0 = 0,
    PHASE_1,
    PHASE_2,
    PHASE_3,
    PHASE_4,
    PHASE_5,
    OnHitEnd
} OnHitPhase;

typedef enum
{
    PRE,
    DEFAULT,
    POST,
    COUNT
} SpellDamagePhase;

typedef struct __attribute__((packed))
{
    uint8_t toggle;  // toggle used to track if a button is enabled or not.
    uint32_t index;   // Used to hold the index of loaded mods for the showmod page
    CMnuLabel *title_label;  // Title Label
    CMnuLabel *desc_label;  // Description Label
    CMnuLabel *page_label;  // Page Index Label
    CMnuLabel *error_label;  // Error Info Label
} SFSF_ModlistStruct;


/* |-========== Global Structures ==========-| */

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

typedef struct __attribute__((packed))
{
    uint32_t *first;        /**< Pointer to the first element in the list. */
    uint32_t *data;         /**< Pointer to the data element within the list. */
    uint32_t *post_last;    /**< Pointer to the element following the last in the list. */
} uint_list_node;

typedef struct __attribute__((packed))
{
    uint16_t X;
    uint16_t Y;
} SF_Coord;

typedef struct __attribute__((packed))
{
    uint32_t R;
    uint32_t G;
} SF_RGColor;

typedef struct __attribute__((packed))
{
    uint8_t entity_type;  // 1 for figure, 2 for building, 3 for object
    uint16_t entity_index;
    SF_Coord position;
} SF_CGdTargetData;

typedef struct __attribute__((packed))
{
    void *raw_data;
    uint32_t str_length;
    uint32_t unknown_length_var;
    char *data;
} SF_String;

typedef struct __attribute__((packed))
{
    uint32_t partA;
    uint32_t partB;
} SF_Rectangle;


typedef struct __attribute__((packed))
{
    uint8_t font_data[0x1fa0];
} SF_Font;

typedef struct __attribute__((packed))
{
    SF_Font *smth_font[32];
    uint8_t unkn_data[0x8];
} SF_FontStruct;

typedef struct __attribute__((packed))
{
    uint32_t R;
    uint32_t G;
    uint32_t B;
} SF_Color;

/* |-========== AutoClass Start ==========-| */
// These classes are currently only partially understood, and are not fully annotated

typedef struct __attribute__((packed))
{
    uint32_t *ac69_ptr1;
    uint32_t *ac69_ptr2;
    uint32_t *ac69_ptr3;
    uint8_t unkn1;
    uint8_t unkn2;
    uint16_t figure_index;
} AutoClass69;

// This class is required for the RANDOM function, AutoClass14 also seems to hold some relevance to the game world as well
// This class is initialized in the GameInit function, so it is likely very important.
typedef struct __attribute__((packed))
{
    uint16_t unknown_field_0;
    uint16_t unknown_field_2;
    uint16_t unknown_field_4;
    uint8_t unknown1[6];
    uint32_t current_step;
    uint8_t unknown_field_10;
    uint8_t unknown2[3];
    uint32_t unknown_field_14;
    uint8_t unknown_field_18;
    uint8_t unknown_field_19;
} AutoClass14;

typedef struct __attribute__((packed))
{
    uint32_t global_player_id;
    uint8_t use;
    uint32_t good_amount[20];
    uint32_t good_remainder[20];
    uint32_t unknown[2];
    uint16_t avatar_figure_index;
    SF_String avatar_name;
    uint16_t hero_figure_index[5];
    SF_String hero_names[5];
    uint16_t unknown2;
    uint16_t civilian_count[6];
    uint16_t warrior_count[2];
    uint8_t unknown3[29];
    uint8_t alliance;
    uint8_t hq_updrade_level[2];
    void *monument_list;
    uint32_t unknown4[2];
    void *building_list;
    uint32_t unknown5[7];
    uint32_t runepower_current;
    uint32_t runepower_max;
    uint16_t group;
    uint32_t unknown6;
    uint16_t unknown7;
    uint8_t swarm_size;
} GdPlayer;

struct __attribute__((packed)) SF_CGdPlayer
{
    AutoClass14 *opaqueClass;
    uint16_t max_used;
    GdPlayer players[200];
    ushort_list_node nodes[2000];
};

typedef struct __attribute__((packed))
{
    uint16_t uknwn1;
    uint16_t uknwn2;
    uint16_t uknwn3;
    uint8_t uknwn4;
} SF_world_unkn_1;

typedef struct __attribute__((packed))
{
    uint16_t world_cell_flags;
    uint16_t sector;
    uint8_t uknwn2;
    uint8_t tile_type;
} SF_world_unkn_2;

typedef struct __attribute__((packed))
{
    uint8_t uknwn1[10];
    uint8_t block_value;
    uint8_t uknwn2[3];
} SF_world_unkn_3;

typedef struct __attribute__((packed))
{
    SF_Rectangle rect;
    uint8_t unkn1;
} SF_world_unkn_4;

struct __attribute__((packed)) SF_CGdWorld
{
    SF_world_unkn_1 unknown1[10001];
    SF_String map_name;
    uint16_t map_size;
    uint8_t unknown2;
    SF_world_unkn_2 cells[1048576];
    uint16_t heightmap[1048576];
    uint16_t unknown3;
    uint8_t unknown4[131070];
    SF_world_unkn_4 unknown5[100];
    uint16_t change_count;
    SF_world_unkn_3 unknown6[255];
    uint8_t unknown7[63];
};

/* Menu Structures */

typedef struct __attribute__((packed))
{
    uint32_t unkn;
    uint32_t param_1_callback;
    uint32_t param_2_callback;
    uint32_t param_3_callback;
    uint8_t data[0x118];
    uint32_t parent_ptr;
    uint8_t data2[0xdc];
} CMnuBase_data;

typedef struct __attribute__((packed))
{
    uint32_t vftable_ptr;
    CMnuBase_data data;
} CMnuBase;

typedef struct __attribute__((packed))
{
    uint32_t vftablePTR;
    uint8_t unknown_data2[0xcc];
} CUiOption;

struct __attribute__((packed)) CMnuLabel
{
    uint32_t vftablePTR;
    CMnuBase_data CMnuBase_data;
    uint8_t CMnuVisControl_data[0x9C];
    uint8_t CMnuLabel_data[0xc0];
};

typedef struct __attribute__((packed))
{
    uint32_t vtable_ptr;
    uint32_t param_ptr;
    uint32_t callback_func;
} CUtlCallback2;

typedef struct __attribute__((packed))
{
    uint8_t padding_start[0x13];
    uint32_t button_ptr;
    uint8_t padding_end[0x119];
} CMnuButton_data;

typedef struct __attribute__((packed))
{
    uint32_t vftablePTR;
    CMnuBase_data CMnuBase_data;
    uint8_t CMnuVisControl_data[0x9c];
    CMnuButton_data CMnuButton_data;
    uint8_t CMnuSmpButton_data[0x50];
} CMnuSmpButton;

typedef struct __attribute__((packed))
{
    uint32_t vftablePTR;
    CMnuBase_data CMnuBase_data;
    uint8_t CMnuVisControl_data[0x9C];
    uint8_t CMnuContainer_data[0x98];
} CMnuContainer;

typedef struct __attribute__((packed))
{
    uint8_t unkn1[0x8];
    uint8_t offset_0x8;
    uint8_t unkn2[0x3];
    uint32_t offset_0xc;
    uint32_t offset_0x10;
    uint8_t offset_0x14;
    uint8_t unkn3[0x3];
    uint32_t offset_0x18;
    uint32_t offset_0x1c;
    uint8_t unknown[0x44];
    SF_String unknown_string;
} data_CUiStartMenu;

typedef struct __attribute__((packed))
{
    uint32_t vftablePTR;
    uint8_t CMnuBase_data[0x208];
    uint8_t CMnuVisControl_data[0x9C];
    uint8_t CMnuContainer_data[0x98];
    data_CUiStartMenu CUiStartMenu_data;
} CUiStartMenu;

typedef struct __attribute__((packed))
{
    uint16_t actionType_id;
    uint16_t actionSubtype_id;
    uint8_t unknown_data[12];
    uint8_t unknown_flag;
    uint8_t padding_maybe;
    uint8_t unknown_config_param;
} SF_UIElement;


typedef struct __attribute__((packed))
{
    void *unkn_ptr1;
    uint32_t AutoClass72[5];
    uint32_t CGdEventIterator[5];
    void *unkn_ptr2;
    uint16_t unkn_value;
    uint8_t padding[0x19a];
} CGdControllerClientData;

typedef struct __attribute__((packed))
{
    void *vftable;
    uint32_t CGdControllerBaseData[0x220];
    CGdControllerClientData data;
} CGdControllerClient;

typedef struct __attribute__((packed))
{
    uint8_t unkn[0xb8];
    uint32_t offset_b8;
    uint8_t unkn2[0x7028];
    SF_CGdFigureToolbox *CGdFigureToolBox;
    void *CGdResource;
    SF_CGdFigure *CGdFigure;
    uint8_t unkn3[0xc];
    CGdControllerClient *CGdControllerClient;
    void *UiDbProxy;
    uint8_t unkn4[0x10];
    void *CUiGame;
    void *CDrwGameView;
    uint8_t unkn5[0x2A4];
    uint32_t unknown_action_type;
    uint8_t unkn9[0x38];
    void *CUiMainDetailView;
    void *unkn_ptr1;
    void *unkn_ptr2;
    void *CUiBuilding;
    void *CUiBottom;
    void *CUiMonument;
    uint8_t unkn6[0x10];
    void *AutoClass112;
    void *AutoClass113;
    uint8_t unkn7[0x1152];
    uint16_t offset_857e;
    uint8_t uknn8[0xda];
} CUiMain_data;

typedef struct __attribute__((packed))
{
    uint32_t CUiMain_cftable_ptr;
    CMnuBase_data CMnuBaseData;
    uint8_t CMnuVisControl_data[0x9C];
    uint8_t CMnuContainer_data[0x98];
    CUiMain_data CUiMain_data;
} SF_CUiMain;

typedef struct __attribute__((packed))
{
    uint8_t unkn[4];
    uint32_t offset_0x4;
    uint32_t offset_0x8;
    SF_String string1;
    SF_String string2;
    uint32_t offset_0x2c;
} SF_CUiVideoSequence_data;

typedef struct __attribute__((packed))
{
    uint32_t vtable_ptr;
    CMnuBase_data baseData;
    uint8_t CMnuVisControl_data[0x9C];
    uint8_t CMnuContainer_data[0x98];
    SF_CUiVideoSequence_data videoData;
} SF_CUiVideoSequence;

typedef struct __attribute__((packed))
{
    uint32_t vtable_ptr;
    CMnuBase_data baseData;
    uint8_t CMnuVisControl_data[0x9C];
    uint8_t CMnuContainer_data[0x98];
    uint8_t videoData[0xc];
} SF_CUiVideo;

typedef struct __attribute__((packed))
{
    uint8_t data[0x28];
    SF_CUiVideoSequence *CUiVideoSequence_ptr;
    CMnuBase *CMnuBase_ptr;
    uint8_t more_data[0x658];
} CAppMenu_data;

typedef struct __attribute__((packed))
{
    uint32_t vftable_ptr;
    CAppMenu_data CAppMenu_data;
} CAppMenu;

/* |-========== Internal Functions ==========-| */
// These functions are used in SFSF internally, and WILL be moved.

typedef void (__thiscall *console_print_ptr)(uint32_t, const char *);
typedef uint16_t (__thiscall *get_spell_spell_line_ptr)(void *, uint16_t);
typedef uint32_t (__thiscall *figure_toolbox_get_unkn_ptr)(void *, uint16_t);
typedef void (__thiscall *figure_toolbox_add_spell_ptr)(void *, uint16_t,
                                                        uint16_t);
typedef bool (__thiscall *figure_toolbox_is_targetable_ptr)(
    void *CGdFigureToolbox, uint16_t figure_index);
typedef uint32_t (__thiscall *FUN_0069eaf0_ptr)(void *ac69, void *ac69_2,
                                                void *ac69_3, void *ac69_4);
typedef void (*fidfree_ptr)(uint32_t *memory_ptr);
typedef void (__thiscall *menu_label_ptr)(CMnuLabel *_this);
typedef void (__thiscall *menu_label_set_string_ptr)(CMnuLabel *_this,
                                                     SF_String *string);
typedef void (__thiscall *initialize_menu_container_ptr)(CMnuContainer *_this);
typedef SF_String *(__thiscall *construct_default_sf_string_ptr)(
    SF_String *_this);

typedef void (__thiscall *construct_start_menu_ptr)(CUiStartMenu *_this,
                                                    uint32_t p1);

typedef void (__thiscall *mnu_label_init_data_ptr)(void *_this, float xpos,
                                                   float ypos, float width,
                                                   float height,
                                                   SF_String *string);
typedef void (__thiscall *message_box_ptr)(uint32_t CAppMenu,
                                           uint16_t description_id,
                                           SF_String *string_ptr,
                                           uint16_t hasOffset);

typedef void (__thiscall *menu_label_constructor_ptr)(CMnuLabel *_this);
typedef void (__thiscall *set_label_flags_ptr)(CMnuLabel *_this,
                                               uint32_t flags);

typedef void (__fastcall *original_menu_func_ptr)(uint32_t param1);

typedef void *(__cdecl *new_operator_ptr)(uint32_t param_1);
typedef void (__thiscall *container_add_control_ptr)(CMnuContainer *_this,
                                                     CMnuBase *CMnubase,
                                                     char c1, char c2,
                                                     uint32_t p4);
typedef void (__thiscall *menu_label_set_data_ptr)(CMnuLabel *_this,
                                                   uint32_t color_red,
                                                   uint32_t color_green,
                                                   uint32_t color_blue,
                                                   uint8_t unknchar);
typedef void (__thiscall *get_sf_color_ptr)(SF_String *_this,
                                            uint32_t color_id);
typedef SF_FontStruct *(__thiscall *get_smth_fonts_ptr)(void);
typedef SF_Font *(__thiscall *get_font_ptr)(SF_FontStruct *_this,
                                            uint32_t font_id);
typedef void (__thiscall *menu_label_set_font_ptr)(void *_this, SF_Font *font);
typedef CUiOption *(__thiscall *create_option_ptr)(CUiOption *_this);

// SF Menu / GUI Hooks for Auras
typedef void (__thiscall *autoclass113_fun_00a27530_ptr)(
    void *_this_autoclass113);
typedef uint32_t *(__thiscall *fun_0086dd60_ptr)(void *_this,uint8_t p1,
                                                 uint32_t p2);
typedef void (__thiscall *autoclass113_fun_00a278c0_ptr)(
    void *_this_autoclass113, uint32_t p1);

// Weird __cdecl class here, blind passthrough and hope it works.
typedef uint16_t (__cdecl *fun_00a2a1d0_ptr)(void *some_ptr, void *some_ptr_2);

typedef void (__thiscall *fun_00a49b10_ptr)(CGdControllerClient *_this,
                                            uint8_t param1);

typedef uint32_t *(__thiscall *fun_00a28d60_ptr)(void *_this, void *param1,
                                                 uint32_t param2);
typedef void (__thiscall *fun_009a4020_ptr)(void *_this, uint32_t param1);

typedef void (__thiscall *vfun164_ptr)(void *CMnuContainer);
typedef void (__thiscall *vfun163_ptr)(void *CMnuContainer, uint8_t param1);
typedef void *(__thiscall *vfun41_ptr)(void *CMnuContainer);

typedef uint32_t (__thiscall *fun_006a0140_ptr)(void *_this, uint16_t p1,
                                                SF_CGdTargetData *p2_ptr,
                                                uint8_t p3, uint8_t p4);
typedef void (__thiscall *fun_009a2790_ptr)(void *_this, void *_p1,
                                            uint32_t p2);
typedef uint32_t (__thiscall *fun_0069f8d0_ptr)(void *_this, uint32_t p1);
typedef uint32_t (__thiscall *fun_0069fb90_ptr)(void *_this, uint16_t figure_id,
                                                uint8_t po2, uint16_t unkn_p3,
                                                SF_CGdTargetData *data,
                                                uint32_t p5, uint32_t p6);
typedef bool (__thiscall *getSpellLineIsTargetSelf_ptr)(void *_this,
                                                        uint16_t spell_line_id);
typedef void *(__thiscall *fun_009a0750_ptr)(void *_this);
typedef void *(__thiscall *fun_009de190_ptr)(void *_this, void *param_1);
typedef void (__thiscall *fun_0099f610_ptr)(void *_this, uint16_t param1,
                                            uint16_t param2, uint8_t param3,
                                            uint16_t param4);
typedef void (__thiscall *fun_009cd1f0_ptr)(void *SF_CUiBottom, uint8_t param1);
typedef void (__thiscall *fun_009a1fd0_ptr)(void *SF_CUiGame, uint32_t param1);
typedef uint32_t (__thiscall *fun_006f8c06_ptr)(SF_CGdFigureToolbox *_this,
                                                uint16_t figure_id,
                                                uint16_t spell_id,
                                                uint32_t target_id,
                                                uint8_t param4, uint32_t param5,
                                                uint32_t param6,
                                                uint32_t param7,
                                                uint32_t param8);
typedef void (__thiscall *fun_00910de0_ptr)(void *_this, void *param1,
                                            uint32_t param2);

typedef uint32_t (__thiscall *cuiVideoSequence_constructor_ptr)(
    SF_CUiVideo *_this, SF_String *p1);
typedef void (__thiscall *CMnuScreen_attach_control_ptr)(void *_CMnuScreen_ptr,
                                                         CMnuBase *base,
                                                         char flag);

extern void attachVideo(CAppMenu *CAppMenu_ptr,  CMnuContainer *parent,
                        char *video_loc_and_name_charsC);
extern CMnuLabel * __thiscall attach_new_meshed_label(CMnuLabel *label_ptr,
                                                      CMnuContainer *parent,
                                                      char *mesh_name,
                                                      char *label_text,
                                                      uint8_t font_index,
                                                      uint16_t x_pos,
                                                      uint16_t y_pos,
                                                      uint16_t width,
                                                      uint16_t height);
extern CMnuLabel * __thiscall attach_new_label(CMnuLabel *label_ptr,
                                               CMnuContainer *parent,
                                               char *label_text,
                                               uint8_t font_index,
                                               uint16_t x_pos, uint16_t y_pos,
                                               uint16_t width, uint16_t height);
extern CMnuSmpButton * __thiscall attach_new_button(CMnuContainer *parent,
                                                    char *button_mesh_default,
                                                    char *button_mesh_pressed,
                                                    char *button_mesh_highlight,
                                                    char *button_mesh_disabled,
                                                    char *label_char, uint8_t font_index,
                                                    uint16_t x_pos, uint16_t y_pos,
                                                    uint16_t width, uint16_t height,
                                                    int button_index,
                                                    uint32_t callback_function_pointer,
                                                    int callback_param1);

extern void __fastcall show_mod_list_callback(CMnuSmpButton *button);

extern SFSF_ModlistStruct mod_struct;

/**
 * @brief Declares a function with the specified return type, name, and arguments.
 *
 * This macro is used to define functions that we use for hooking into the game and to expose these to the API.
 * we use it to cut down on boilerplate code typedefs and structures.
 *
 * @param return_type The return type of the function.
 * @param function_name The name of the function.
 * @param ... The function parameters and types.
 */
 #define DECLARE_FUNCTION(type, name, ...) \
         typedef type (__thiscall *name ## _ptr)(__VA_ARGS__);
