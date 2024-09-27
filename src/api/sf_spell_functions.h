#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "sf_general_structures.h"

typedef enum
{
    CHECK_SPELLS_BEFORE_CHECK_BATTLE = 0x04,
    CHECK_SPELLS_BEFORE_JOB2 = 0x02,
    UNFREEZE = 0x1
} SpellFlagKey;

typedef enum
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
    kGdSpellLineBerserk = 85,
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

typedef struct __attribute__((packed))
{
    uint16_t spell_id;
    uint16_t spell_line_id;
    uint8_t skill_requirements[12];
    uint16_t mana_cost;
    uint32_t cast_time;
    uint32_t recast_time;
    uint16_t min_range;
    uint16_t max_range;
    uint8_t cast_type1;
    uint8_t cast_type2;
    uint32_t params[10];
    uint16_t effect_power;
    uint16_t effect_range;
} SF_CGdResourceSpell;

// Declare the function pointers for the SpellFunctions group
DECLARE_FUNCTION(void, setXData, SF_CGdSpell *_this, uint16_t spell_id, uint8_t xdatakey, uint32_t value);
DECLARE_FUNCTION(void, initializeSpellData, SF_CGdSpell *_this, uint16_t spell_id, SpellDataKey key);
DECLARE_FUNCTION(void, setEffectDone, SF_CGdSpell *_this, uint16_t spell_id, uint16_t param_2);
DECLARE_FUNCTION(uint32_t, addToXData, SF_CGdSpell *_this, uint16_t spell_id, SpellDataKey key, uint32_t value);
DECLARE_FUNCTION(uint32_t, getChanceToResistSpell, void *autoclass34, uint16_t source, uint16_t target, SF_SpellEffectInfo effect_info);
DECLARE_FUNCTION(uint16_t, getRandom, void *autoclass14, uint16_t max_value);
DECLARE_FUNCTION(void, addVisualEffect, SF_CGdSpell *_this, uint16_t spell_id, CGdEffectType effect_id, void *unused, SF_CGdTargetData *target, uint32_t tick_start, uint16_t tick_count, void *corner_coords);
DECLARE_FUNCTION(void, figureAggro, SF_CGdSpell *_this, uint16_t spell_id, uint16_t target_index);
DECLARE_FUNCTION(SF_CGdResourceSpell *, getResourceSpellData, void *, SF_CGdResourceSpell *spellData, uint16_t index);
DECLARE_FUNCTION(uint32_t, getXData, SF_CGdSpell *_this, uint16_t spell_id, SpellDataKey key);
DECLARE_FUNCTION(SF_Rectangle *, getTargetsRectangle, SF_CGdSpell *_this, SF_Rectangle *output, uint16_t spell_id, uint16_t radius, SF_Coord *center_maybe);
DECLARE_FUNCTION(void, removeDLLNode, SF_CGdSpell *_this, uint16_t param_1);
DECLARE_FUNCTION(void, spellClearFigureFlag, SF_CGdSpell *_this, uint16_t spell_id, SpellFlagKey key);
DECLARE_FUNCTION(void, onSpellRemove, SF_CGdSpell *_this, uint16_t spell_id);
DECLARE_FUNCTION(uint16_t, addSpell, SF_CGdSpell *_this, uint16_t spell_id, uint16_t param2, SF_CGdTargetData *source, SF_CGdTargetData *target, uint16_t param5);
DECLARE_FUNCTION(uint16_t, getSpellLine, SF_CGdSpell *_this, uint16_t spell_id);
DECLARE_FUNCTION(void, figClrChkSplBfrChkBattle, SF_CGdSpell *_this, uint16_t spell_id, uint16_t unk1);
DECLARE_FUNCTION(void, figTryClrCHkSPlBfrJob2, SF_CGdSpell *_this, uint16_t spell_id);
DECLARE_FUNCTION(void, figTryUnfreeze, SF_CGdSpell *_this, uint16_t spell_id, uint16_t unk1);
DECLARE_FUNCTION(uint16_t, getSpellID, SF_CGdSpell *_this, uint16_t spell_index);
DECLARE_FUNCTION(int, getSpellTag, uint16_t spell_index);

DECLARE_FUNCTION(int, checkCanApply, SF_CGdSpell *_this, uint16_t index);

DECLARE_FUNCTION_GROUP(Spell,
                       setXData_ptr setXData;
                       initializeSpellData_ptr initializeSpellData;
                       setEffectDone_ptr setEffectDone;
                       addToXData_ptr addToXData;
                       getChanceToResistSpell_ptr getChanceToResistSpell;
                       getRandom_ptr getRandom;
                       getResourceSpellData_ptr getResourceSpellData;
                       addVisualEffect_ptr addVisualEffect;
                       figureAggro_ptr figureAggro;
                       getXData_ptr getXData;
                       getTargetsRectangle_ptr getTargetsRectangle;
                       removeDLLNode_ptr removeDLLNode;
                       spellClearFigureFlag_ptr spellClearFigureFlag;
                       addSpell_ptr addSpell;
                       getSpellLine_ptr getSpellLine;
                       onSpellRemove_ptr onSpellRemove;
                       getSpellID_ptr getSpellID;
                       checkCanApply_ptr checkCanApply;
                       getSpellTag_ptr getSpellTag;

                       // flag clear function used inside spellClearFigureFlag Wrapper
                       figClrChkSplBfrChkBattle_ptr figClrChkSplBfrChkBattle;
                       figTryClrCHkSPlBfrJob2_ptr figTryClrCHkSPlBfrJob2;
                       figTryUnfreeze_ptr figTryUnfreeze;);