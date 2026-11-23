#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "./structures/sf_spell_structures.h"

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

typedef struct __attribute__ ((packed))
{
    uint16_t spellline_id;
    uint16_t text_id;
    uint8_t flags;
    uint8_t magic_type;
    uint8_t min_level;
    uint8_t max_level;
    uint8_t availability;
    char ui_handle[64];
    uint16_t description_id;
} SF_CGdResourceSpellLine;

// Declare the function pointers for the SpellFunctions group
DECLARE_FUNCTION(void, setXData, SF_CGdSpell *_this, uint16_t spell_id,
                 uint8_t xdatakey, uint32_t value);
DECLARE_FUNCTION(void, initializeSpellData, SF_CGdSpell *_this,
                 uint16_t spell_id, SpellDataKey key);
/**
 * setEffectDone - Stops a spell effect handler for a given spell.
 * @param clear_spell_data: Zero also clears the spell's XData (framework
 *        always passes 0); non-zero keeps the data across the finish.
 */
DECLARE_FUNCTION(void, setEffectDone, SF_CGdSpell *_this, uint16_t spell_id,
                 uint16_t clear_spell_data);
DECLARE_FUNCTION(uint32_t, addToXData, SF_CGdSpell *_this, uint16_t spell_id,
                 SpellDataKey key, uint32_t value);
DECLARE_FUNCTION(uint32_t, getChanceToResistSpell, void *damage_reduction_class,
                 uint16_t source, uint16_t target,
                 uint16_t spell_index);
DECLARE_FUNCTION(uint16_t, getRandom, AutoClass14 *opaque_class, uint16_t max_value);
DECLARE_FUNCTION(uint8_t, rollWeaponEnchantEffects, SF_CGdFigureJobs *jobs,
                 uint16_t source_index, uint16_t weapon_id,
                 uint16_t *out_spell_ids /* [3] */);
DECLARE_FUNCTION(void, addVisualEffect, SF_CGdSpell *_this, uint16_t spell_id,
                 CGdEffectType effect_id, void *unused,
                 SF_CGdTargetData *target, uint32_t tick_start,
                 uint16_t tick_count, void *corner_coords);
DECLARE_FUNCTION(void, addSpellVisualEffect, SF_CGdSpell *_this, uint16_t spell_index,
                 CGdEffectType effect_id, const SF_CGdTargetData *target,
                 uint16_t tick_count, const SF_Rectangle *corner_coords);
DECLARE_FUNCTION(void, addSpellHitEffect, SF_CGdSpell *_this, uint16_t spell_index,
                 uint16_t target_index, uint16_t tick_count);
DECLARE_FUNCTION(void, addSpellResistedEffect, SF_CGdSpell *_this, uint16_t spell_index,
                 uint16_t target_index, uint16_t tick_count);
DECLARE_FUNCTION(void, addSpellHitWorldEffect, SF_CGdSpell *_this, uint16_t spell_index,
                 SF_Coord position, uint16_t tick_count, const SF_Rectangle *area);
DECLARE_FUNCTION(void, figureAggro, SF_CGdSpell *_this, uint16_t spell_id,
                 uint16_t target_index);
DECLARE_FUNCTION(SF_CGdResourceSpell *, getResourceSpellData, SF_CGdResource *CGdResource,
                 SF_CGdResourceSpell *spellData, uint16_t index);
DECLARE_FUNCTION(uint32_t, getXData, SF_CGdSpell *_this, uint16_t spell_id,
                 SpellDataKey key);
DECLARE_FUNCTION(SF_Rectangle *, getTargetsRectangle, SF_CGdSpell *_this,
                 SF_Rectangle *output, uint16_t spell_id, uint16_t radius,
                 SF_Coord *center_maybe);
/**
 * removeDLLNode - Removes a spell from the list of active spells over the
 * target.
 * @param spell_index: Index of the spell whose effect node list is torn
 *        down.
 */
DECLARE_FUNCTION(void, removeDLLNode, SF_CGdSpell *_this, uint16_t spell_index);
DECLARE_FUNCTION(void, spellClearFigureFlag, SF_CGdSpell *_this,
                 uint16_t spell_id, SpellFlagKey key);
DECLARE_FUNCTION(void, onSpellRemove, SF_CGdSpell *_this, uint16_t spell_id);
/**
 * addSpell - Creates a new spell of a given type.
 * @param start_tick: Absolute game tick when the spell becomes active
 *        (stored as to_do_count = start_tick - current_step).
 * @param aux_entity_index: Only used by the RockBullet spell lines: entity
 *        index stored into XData EFFECT_ENTITY_INDEX3; ignored otherwise,
 *        pass 0.
 */
DECLARE_FUNCTION(uint16_t, addSpell, SF_CGdSpell *_this, uint16_t spell_id,
                 uint16_t start_tick, SF_CGdTargetData *source,
                 SF_CGdTargetData *target, uint16_t aux_entity_index);
DECLARE_FUNCTION(uint16_t, getSpellLine, SF_CGdSpell *_this, uint16_t spell_id);
DECLARE_FUNCTION(void, figClrChkSplBfrChkBattle, SF_CGdSpell *_this,
                 uint16_t spell_id, uint16_t figure_id);
DECLARE_FUNCTION(void, figTryClrCHkSPlBfrJob2, SF_CGdSpell *_this,
                 uint16_t spell_id);
DECLARE_FUNCTION(void, figTryUnfreeze, SF_CGdSpell *_this, uint16_t spell_id,
                 uint16_t figure_id);
DECLARE_FUNCTION(uint16_t, getSpellID, SF_CGdSpell *_this,
                 uint16_t spell_index);
DECLARE_FUNCTION(uint16_t, getSpellTags, uint16_t spell_index);
DECLARE_FUNCTION(bool, hasSpellTag, uint16_t spell_line_id, SpellTag tag);
DECLARE_FUNCTION(int, checkCanApply, SF_CGdSpell *_this, uint16_t index);
DECLARE_FUNCTION(void, spellEffectCallback, SF_CGdSpell *_this, uint16_t source_index, uint16_t spell_index,
                 bool (*conditionPtr)(SF_CGdSpell *, uint16_t, uint16_t),
                 void (*callbackPtr)(SF_CGdSpell *, uint16_t, uint16_t, uint16_t));
DECLARE_FUNCTION(uint16_t, getLeveledSpellID, SF_CGdResource *CGdResource, uint16_t source_spell_id, uint16_t spell_level);
DECLARE_FUNCTION(void, tryClearCheckSpellsBeforeJob, SF_CGdSpell *_this, uint16_t spell_index, uint16_t figure_index);
/**
 * addSpellToFigure - Starts a spell between two figures (entity targets).
 * @param source_index: Figure index of the source.
 * @param target_index: Figure index of the target.
 * Return: Spell slot index, 0 when no free slot.
 *
 * Framework helper over addSpell: builds zero-position TargetData for
 * both sides; the spell always starts at the current game tick (future
 * starts only via direct addSpell). Figure targets only (entity_type 1
 * implied by name); positional (4/5) spells - direct addSpell (position
 * inside TargetData is meaningful for them, e.g. the AoE center).
 */
DECLARE_FUNCTION(uint16_t, addSpellToFigure, SF_CGdSpell *_this, uint16_t spell_id,
                 uint16_t source_index, uint16_t target_index);


//TODO export spelljobs structure
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
     * @param clear_spell_data Pass 0 to also clear the spell's XData on finish;
     * non-zero keeps it. The framework always passes 0.
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
     * In case the spell has requirements for more than one magic school, the school with the highest requirement is
     * regarded as primary.
     *
     * @param damage_reduction_class Engine component (CGdAutoClass34) consumed by
     * resist/damage-reduction lookups (see the getPhysDamageReduction wrapper);
     * exact internals not established.
     * @param source The source character or entity casting the spell.
     * @param target The target character or entity that we want the resistance from.
     * @param spell_id spell index (not spell line!)
     * @return The chance that the spell will be resisted, sometimes used as a 'chance' to fully resist the spell, also
     * used to mitigate the damage dealt with the spell.
     */
    getChanceToResistSpell_ptr getChanceToResistSpell;

    /**
     * @brief Generates a random value up to the specified maximum value.
     *
     * This function returns a random value between 0 and the specified maximum value (inclusive).
     * @param opaque_class Opaque RNG class of the spell global object (the spell
     * object's OpaqueClass in framework calls).
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
     * @param target The entity target data which the visual effect will be centered on. The X-Y coordinates passed as
     * part of the target data will determine the visual effect offset relative to the target. @see SF_CGdTargetData
     * @param tick_start The internal game tick when the visual effect must appear.
     * @param tick_count The number of ticks the visual effect will last.
     * @param corner_coords The area that the visual effect would be within for AoE effects. Use {0,0} for non AOE
     * Spells.
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
     * @return A pointer to the rectangle representing the affected area that contains all possible targets of the
     * spell.
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
     * It works as a wrapper of more specific flag clearing functions, @see figClrChkSplBfrChkBattle or
     * figTryClrCHkSPlBfrJob2
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
     * @param game_tick The internal game tick when the spell should appear. This value can be determined via the opaque
     * class in the global spell object.
     * @param source The index of the spell caster (source entity).
     * @param target The Spell Target entity index.
     * @param aux_entity_index Only used by the RockBullet spell lines: entity index
     * stored into XData EFFECT_ENTITY_INDEX3. Ignored otherwise - pass 0.
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
     * The spell effect ID can be used to obtain spell parameters (such as damage, radius, duration, etc.) from
     * GameData.cff
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
     * This function returns a set of tags associated with a given spell, which can be used for filtering or
     * classification.
     * @param spell_index The index of the spell.
     * @return A set of tags related to the spell.
     */
    getSpellTags_ptr getSpellTags;

    /**
     * @brief Checks if a spell has a specific tag.
     *
     * This function determines whether the given spell contains the specified tag.
     * @param spell_line_id The ID of the spell.
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

    /**
     * @brief Used to get the Spell ID for units that can be leveled.
     * @example Rune Army Units
     */
    getLeveledSpellID_ptr getLeveledSpellID;

    tryClearCheckSpellsBeforeJob_ptr tryClearCheckSpellsBeforeJob;

    /**
     * @brief Rolls the shooter's weapon enchant pool (up to 3 slots, one
     * getRandom(10000) per slot, registry chance, race leveling 1-6 applied).
     * Returns the number of winning enchant spell ids written into
     * @p out_spell_ids (first @p return-value positions; the rest untouched -
     * zero-initialize before calling). Framework mechanism behind the
     * RANGED_SHOT phase: per-arrow enchant procs on rapid-fire clones.
     */
    rollWeaponEnchantEffects_ptr rollWeaponEnchantEffects;

    /**
     * @brief Spawn a spell visual effect without the TargetData/rect boilerplate.
     *
     * Framework helper around addVisualEffect: it keeps the live out-buffer the
     * engine writes to, passes a local copy of the target data and uses the current
     * game tick as tick_start.
     * @param _this A pointer to the global spell object.
     * @param spell_index The Spell Index of the currently active spell.
     * @param effect_id The ID of the visual effect to apply. @see kGdEffectSpellHitTarget
     * @param target The entity target data which the visual effect will be centered on. The structure is copied, so
     * a temporary may be passed.
     * @param tick_count The number of ticks the visual effect will last.
     * @param corner_coords The area that the visual effect would be within for AoE effects. NULL means the zero
     * rectangle {0,0} for non AOE spells.
     */
    addSpellVisualEffect_ptr addSpellVisualEffect;

    /**
     * @brief Spawn the spell-hit visual effect on a figure.
     *
     * Framework helper. Shortcut for addSpellVisualEffect with kGdEffectSpellHitTarget, a figure target
     * (entity_type 1, zero position) and the zero rectangle.
     * @param _this A pointer to the global spell object.
     * @param spell_index The Spell Index of the currently active spell.
     * @param target_index The figure the visual effect will be centered on.
     * @param tick_count The number of ticks the visual effect will last.
     */
    addSpellHitEffect_ptr addSpellHitEffect;

    /**
     * @brief Spawn the spell-resisted visual effect on a figure.
     *
     * Framework helper. Shortcut for addSpellVisualEffect with kGdEffectSpellTargetResisted, a figure target
     * (entity_type 1, zero position) and the zero rectangle.
     * @param _this A pointer to the global spell object.
     * @param spell_index The Spell Index of the currently active spell.
     * @param target_index The figure the visual effect will be centered on.
     * @param tick_count The number of ticks the visual effect will last.
     */
    addSpellResistedEffect_ptr addSpellResistedEffect;

    /**
     * @brief Spawn the world-hit visual effect at a map position.
     *
     * Framework helper. Shortcut for addSpellVisualEffect with kGdEffectSpellHitWorld, a world target
     * (entity_type 4, index 0) at the given position.
     * @param _this A pointer to the global spell object.
     * @param spell_index The Spell Index of the currently active spell.
     * @param position The map coordinates the visual effect will be centered on.
     * @param tick_count The number of ticks the visual effect will last.
     * @param area The area that the visual effect would be within for AoE effects. NULL means the zero rectangle
     * {0,0}.
     */
    addSpellHitWorldEffect_ptr addSpellHitWorldEffect;

    /**
     * @brief Starts a spell between two figures (entity targets).
     *
     * Framework helper over addSpell: builds zero-position SF_CGdTargetData
     * for source and target, the spell always starts at the current game
     * tick. Figure targets only (entity_type 1 implied by name);
     * positional (4/5) spells - direct addSpell (position inside
     * TargetData is meaningful for them, e.g. the AoE center; also for
     * future starts).
     * @param _this A pointer to the global spell object.
     * @param spell_id The Spell Type ID of the spell to start.
     * @param source_index Figure index of the source.
     * @param target_index Figure index of the target.
     * @return The spell slot index, 0 when no free slot.
     */
    addSpellToFigure_ptr addSpellToFigure;

} SpellFunctions;
