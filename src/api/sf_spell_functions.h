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
DECLARE_FUNCTION(void, setXData, SF_CGdSpell *_this, uint16_t spell_id,
                 uint8_t xdatakey, uint32_t value);
DECLARE_FUNCTION(void, initializeSpellData, SF_CGdSpell *_this,
                 uint16_t spell_id, SpellDataKey key);
DECLARE_FUNCTION(void, setEffectDone, SF_CGdSpell *_this, uint16_t spell_id,
                 uint16_t param_2);
DECLARE_FUNCTION(uint32_t, addToXData, SF_CGdSpell *_this, uint16_t spell_id,
                 SpellDataKey key, uint32_t value);
DECLARE_FUNCTION(uint32_t, getChanceToResistSpell, void *autoclass34,
                 uint16_t source, uint16_t target,
                 SF_SpellEffectInfo effect_info);
DECLARE_FUNCTION(uint16_t, getRandom, void *autoclass14, uint16_t max_value);
DECLARE_FUNCTION(void, addVisualEffect, SF_CGdSpell *_this, uint16_t spell_id,
                 CGdEffectType effect_id, void *unused,
                 SF_CGdTargetData *target, uint32_t tick_start,
                 uint16_t tick_count, void *corner_coords);
DECLARE_FUNCTION(void, figureAggro, SF_CGdSpell *_this, uint16_t spell_id,
                 uint16_t target_index);
DECLARE_FUNCTION(SF_CGdResourceSpell *, getResourceSpellData, void *,
                 SF_CGdResourceSpell *spellData, uint16_t index);
DECLARE_FUNCTION(uint32_t, getXData, SF_CGdSpell *_this, uint16_t spell_id,
                 SpellDataKey key);
DECLARE_FUNCTION(SF_Rectangle *, getTargetsRectangle, SF_CGdSpell *_this,
                 SF_Rectangle *output, uint16_t spell_id, uint16_t radius,
                 SF_Coord *center_maybe);
DECLARE_FUNCTION(void, removeDLLNode, SF_CGdSpell *_this, uint16_t param_1);
DECLARE_FUNCTION(void, spellClearFigureFlag, SF_CGdSpell *_this,
                 uint16_t spell_id, SpellFlagKey key);
DECLARE_FUNCTION(void, onSpellRemove, SF_CGdSpell *_this, uint16_t spell_id);
DECLARE_FUNCTION(uint16_t, addSpell, SF_CGdSpell *_this, uint16_t spell_id,
                 uint16_t param2, SF_CGdTargetData *source,
                 SF_CGdTargetData *target, uint16_t param5);
DECLARE_FUNCTION(uint16_t, getSpellLine, SF_CGdSpell *_this, uint16_t spell_id);
DECLARE_FUNCTION(void, figClrChkSplBfrChkBattle, SF_CGdSpell *_this,
                 uint16_t spell_id, uint16_t unk1);
DECLARE_FUNCTION(void, figTryClrCHkSPlBfrJob2, SF_CGdSpell *_this,
                 uint16_t spell_id);
DECLARE_FUNCTION(void, figTryUnfreeze, SF_CGdSpell *_this, uint16_t spell_id,
                 uint16_t unk1);
DECLARE_FUNCTION(uint16_t, getSpellID, SF_CGdSpell *_this,
                 uint16_t spell_index);
DECLARE_FUNCTION(uint16_t, getSpellTags, uint16_t spell_index);
DECLARE_FUNCTION(bool, hasSpellTag, uint16_t spell_id, SpellTag tag);
DECLARE_FUNCTION(int, checkCanApply, SF_CGdSpell *_this, uint16_t index);
DECLARE_FUNCTION(void, spellEffectCallback, SF_CGdSpell *_this,
                 uint16_t source_index, uint16_t spell_index,
                 bool (*conditionPtr)(SF_CGdSpell *, uint16_t, uint16_t),
                 void (*callbackPtr)(SF_CGdSpell *, uint16_t, uint16_t,
                                     uint16_t));

/**
 * @brief Represents a collection of function pointers for managing spell-related operations
 * @ingroup API
 */
typedef struct
{
    /**
     * @brief This function is used to assign specific value to an XData key of a given spell
     * @param _this a pointer to the global spell object
     * @param spell_id The Spell Index of the currently active spell
     * @param uint32_t The predefined enumerator of the XData Key.
     * @param value The value you wish to assign expressed as integer.
     */
    setXData_ptr setXData;

    /**
     * @brief Initializes spell-related data for a given entity.
     *
     * This function initializes spell data for a given entity, using the specified spell ID and key.
     * @param _this A pointer to the global spell object.
     * @param spell_id The Spell Index of the spell.
     * @param key The SpellDataKey to initialize the data with.
     */
    initializeSpellData_ptr initializeSpellData;

    /**
     * @brief Stops a spell effect handler for a given spell.
     *
     * This function marks the spell effect as completed, which will lead to its automatic termination
     * during the next game tick.
     * @param _this A pointer to the global spell object.
     * @param spell_id The Spell Index of the currently active spell.
     * @param param_2 The second parameter whose purpose is currently unknown; it should always be set to 0.
     */
    setEffectDone_ptr setEffectDone;

    /**
     * @brief Adds a value to a specific XData entry for the given spell.
     *
     * This function increments the XData value associated with the given spell and XData key.
     * @param _this A pointer to the global spell object.
     * @param spell_id The Spell Index of the currently active spell.
     * @param key The XData key to modify.
     * @param value The value to add to the XData entry.
     * @return Summed value of the spell XData Key
     */
    addToXData_ptr addToXData;

    /**
     * @brief Returns target's resistance to a primary magic school that a specified spell belongs in.
     * In case the spell has requirements for more than one magic school, the school with the highest requirement is regarded as primary.
     *
     * @param autoclass34 A pointer to an internal class.
     * @param source The source character or entity casting the spell.
     * @param target The target character or entity that we want the resistance from.
     * @param effect_info spell effect ID and spell job ID grouped in a single variable of SF_SpellEffectInfo
     * @return The chance that the spell will be resisted, sometimes used as a 'chance' to fully resist the spell, also used to mitigate the damage dealt with the spell.
     */
    getChanceToResistSpell_ptr getChanceToResistSpell;

    /**
     * @brief Generates a random value up to the specified maximum value.
     *
     * This function returns a random value between 0 and the specified maximum value (inclusive).
     * @param autoclass14 A pointer to the Opaque class of the spell global object.
     * @param max_value The upper bound for the random number.
     * @return A random value between 0 and the specified maximum.
     */
    getRandom_ptr getRandom;

    getResourceSpellData_ptr getResourceSpellData;

    /**
     * @brief spawn a visual effect.
     *
     * This function attaches a visual effect to the given spell and its corresponding target.
     * @param _this A pointer to the global spell object.
     * @param spell_id The Spell Index of the currently active spell.
     * @param effect_id The ID of the visual effect to apply. @see kGdEffectSpellHitTarget
     * @param unused Unused parameter that may be for future extensibility; Set to 0 for now.
     * @param target The entity target data which the visual effect will be centered on. The X-Y coordinates passed as part of the target data will determine the visual effect offset relative to the target. @see SF_CGdTargetData
     * @param tick_start The internal game tick when the visual effect must appear.
     * @param tick_count The number of ticks the visual effect will last.
     * @param corner_coords The area that the visual effect would be within for AoE effects. Use {0,0} for non AOE Spells.
     */
    addVisualEffect_ptr addVisualEffect;

    /**
     * @brief Used to trigger aggressive behavior for a figure
     *
     * @param _this A pointer to the global spell object.
     * @param spell_id The Spell Index of the active spell.
     * @param target_index The figure to make aggressive towards the caster of the spell.
     */
    figureAggro_ptr figureAggro;

    /**
     * @brief Retrieves the XData value for a specific spell.
     *
     * This function returns the XData value stored for a given spell and key.
     * @param _this A pointer to the global spell object.
     * @param spell_id The Spell Index of the currently active spell.
     * @param key Numerical XData key ID or its predefined enumerator.
     * @return The value associated with the XData key.
     */
    getXData_ptr getXData;

    /**
     * @brief This function is used to calculate area of effect for a spell.
     *
     * The Area of effect for a spell is a circle of a given radius within a rectangle
     * @param _this A pointer to the global spell object.
     * @param output A pointer to the rectangle structure to store the result.
     * @param spell_id The Spell Index of the currently active spell.
     * @param radius The radius of the area of effect.
     * @param center_maybe The center coordinates of the area of effect.
     * @return A pointer to the rectangle representing the affected area that contains all possible targets of the spell.
     */
    getTargetsRectangle_ptr getTargetsRectangle;

    /**
     * @brief Used to remove a spell from the list of active spells over the target.
     * @warning This function should be used together with setEffectDone in order to correctly finish the spell.
     * @param _this A pointer to the global spell object.
     * @param spell_index The Spell Index of the currently active spell.
     */
    removeDLLNode_ptr removeDLLNode;

    /**
     * @brief Clears a specific flag on the spell object.
     *
     * It works as a wrapper of more specific flag clearing functions, @see figClrChkSplBfrChkBattle or figTryClrCHkSPlBfrJob2
     * @param _this A pointer to the global spell object.
     * @param spell_id The Spell Index of the currently active spell.
     * @param key The flag key to clear. @see SpellFlagKey
     */
    spellClearFigureFlag_ptr spellClearFigureFlag;

    /**
     * @brief Creates a new spell of a given spell type.
     *
     * This function creates a new spell of the specified spell type and associates it with a source and target entity.
     * The spell will be active during the game, and the time it appears is determined by the internal game tick.
     *
     * @param _this A pointer to the global spell object.
     * @param spell_id The Spell Type ID of the spell to create.
     * @param game_tick The internal game tick when the spell should appear. This value can be determined via the opaque class in the global spell object.
     * @param source The index of the spell caster (source entity).
     * @param target The Spell Target entity index.
     * @param param5 The purpose of this parameter is currently unknown. For safe use, it is recommended to pass it as 0.
     */
    addSpell_ptr addSpell;

    /**
     * @brief Retrieves the spell type ID of a given spell.
     *
     * This function returns the spell type ID associated with a given spell.
     *
     * @param _this A pointer to the global spell object.
     * @param spell_id The index of the spell for which the type ID is to be retrieved
     * @return The Spell Type ID associated with the given spell.
     */
    getSpellLine_ptr getSpellLine;

    /**
     * @brief This function is triggered when a spell stops.
     */
    onSpellRemove_ptr onSpellRemove;

    /**
     * @brief Retrieves the Spell Effect ID for a given spell index.
     * The spell effect ID can be used to obtain spell parameters (such as damage, radius, duration, etc.) from GameData.cff
     * @param _this A pointer to the global spell object.
     * @param spell_index The index of the spell.
     * @return The Spell Effect ID.
     */
    getSpellID_ptr getSpellID;

    /**
     * @brief Calls the Refresh handler for a specified spell type.
     *
     * This function acts as a wrapper to invoke the Refresh handler associated with the given spell.
     * It returns the value that the Refresh handler returns.
     *
     * @param _this A pointer to the global spell object.
     * @param spell_id The index of the spell for which the Refresh handler is called.
     * @return The value returned by the Refresh handler.
     */
    checkCanApply_ptr checkCanApply;

    /**
     * @brief Retrieves the tags associated with a spell.
     *
     * This function returns a set of tags associated with a given spell, which can be used for filtering or classification.
     * @param spell_index The index of the spell.
     * @return A set of tags related to the spell.
     */
    getSpellTags_ptr getSpellTags;

    /**
     * @brief Checks if a spell has a specific tag.
     *
     * This function determines whether the given spell contains the specified tag.
     * @param spell_id The ID of the spell.
     * @param tag The tag to check for. @see SpellTag
     * @return True if the spell has the specified tag, false otherwise.
     */
    hasSpellTag_ptr hasSpellTag;

    /**
     * @brief Clears the CHECK_SPELLS_BEFORE_CHECK_BATTLE flag
     */
    figClrChkSplBfrChkBattle_ptr figClrChkSplBfrChkBattle;

    /**
     * @brief clears the CHECK_SPELLS_BEFORE_JOB2 flag
     */
    figTryClrCHkSPlBfrJob2_ptr figTryClrCHkSPlBfrJob2;

    /**
     * @brief clears the UNFREEZE flag
     */
    figTryUnfreeze_ptr figTryUnfreeze;

    /**
     * @brief callback that iterates through spells on a target, checks a condition then executes the callback.
     */
    spellEffectCallback_ptr spellEffectCallback;
} SpellFunctions;
