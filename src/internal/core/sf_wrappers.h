#ifndef WRAPPERS_H
#define WRAPPERS_H

#if defined(__GNUC__) || defined(__clang__)
#define LOG_PRINTF_FMT(fmt_idx, arg_idx) __attribute__((format(printf, fmt_idx, arg_idx)))
#else
#define LOG_PRINTF_FMT(fmt_idx, arg_idx)
#endif

#include "../../api/sfsf.h"
#include "../../asi/sf_asi.h"

typedef SF_String *(__thiscall *SF_String_ctor_ptr)(SF_String *, const char *);
typedef void (__thiscall *SF_String_dtor_ptr)(SF_String *);
typedef bool (__thiscall *has_spell_effect_ptr)(SF_CGdFigureToolbox *_this,
                                                uint16_t param_1,
                                                uint16_t param_2);

void log_message(const char *filename, const char *message, ...);

void initialize_wrapper_data_hooks();

uint16_t __thiscall getCurrentStat(SF_CGdFigure *_this, uint16_t target, StatisticDataKey key);
uint16_t __thiscall getMaxStat(SF_CGdFigure *_this, uint16_t target, StatisticDataKey key);


uint32_t __thiscall getDistance(SF_Coord *pointA, SF_Coord *pointB);
bool __thiscall isSiegeUnit (SF_CGdFigure *_this, uint16_t figure_index);

extern bool __thiscall hasAuraActive(SF_CGdFigureToolbox *_this, uint16_t figure_id);
extern bool __thiscall isActionMelee(SF_SGtFigureAction *_this);
extern bool __thiscall hasSpellTag(uint16_t spell_id, SpellTag tag) __attribute__((optimize("-O0")));
extern void __thiscall setupFigureIterator(CGdFigureIterator *iterator, SF_CGdSpell *spell);
extern void __thiscall disposeFigureIterator(CGdFigureIterator *iterator);
bool __thiscall hasBuildingTag(uint8_t building_type, BuildingTag tag)  __attribute__((optimize("-O0")));

uint32_t __thiscall getBuildingXData(SF_CGdBuildingToolbox *_this, uint16_t building_index, uint8_t key_type);

bool __thiscall buildingIsScavenger(SF_CGdBuilding *_this, uint16_t building_index);
bool __thiscall buildingIsFisher(SF_CGdBuilding *_this, uint16_t building_index);
bool __thiscall buildingIsFoodstore(SF_CGdBuilding *_this, uint16_t building_index);
bool __thiscall buildingIsForge(SF_CGdBuilding *_this, uint16_t building_index);
bool __thiscall buildingIsGatherer(SF_CGdBuilding *_this, uint16_t building_index);
bool __thiscall buildingIsHQ(SF_CGdBuilding *_this, uint16_t building_index);
bool __thiscall buildingIsHunter(SF_CGdBuilding *_this, uint16_t building_index);
bool __thiscall buildingIsIronMine(SF_CGdBuilding *_this, uint16_t building_index);
bool __thiscall buildingIsMaceCarver(SF_CGdBuilding *_this, uint16_t building_index);
bool __thiscall buildingIsMoonsilverMine(SF_CGdBuilding *_this, uint16_t building_index);
bool __thiscall buildingIsQuarry(SF_CGdBuilding *_this, uint16_t building_index);
bool __thiscall buildingIsSawmill(SF_CGdBuilding *_this, uint16_t building_index);
bool __thiscall buildingIsSmelter(SF_CGdBuilding *_this, uint16_t building_index);
bool __thiscall buildingIsStonecutter(SF_CGdBuilding *_this, uint16_t building_index);
bool __thiscall buildingIsTemple(SF_CGdBuilding *_this, uint16_t building_index);
bool __thiscall buildingIsTower(SF_CGdBuilding *_this, uint16_t building_index);
bool __thiscall buildingIsWoodcutter(SF_CGdBuilding *_this, uint16_t building_index);
bool __thiscall buildingIsShrine(SF_CGdBuilding *_this, uint16_t building_index);
bool __thiscall buildingIsHabitable(SF_CGdBuilding *_this, uint16_t building_index);
bool __thiscall buildingIsHabitableSingle(SF_CGdBuilding *_this, uint16_t building_index);
bool __thiscall XDataExists(SF_CGdEffect *_this, uint16_t effect_index, SpellDataKey data);

void __thiscall tryClearCheckSpellsBeforeJob(SF_CGdSpell *_this, uint16_t spell_index, uint16_t figure_index);

/**
 * sf_end_spell_now - Ends a spell immediately: clears the figure's
 * check-spells flags, removes the spell from the DLL list and marks the
 * effect done.
 * @param _this: Spell global object owning the active spell list.
 * @param spell_index: Index of the spell to end.
 * @param try_unfreeze: Pass true to call figTryUnfreeze first (the
 *        hypnotize damage handler's unfreeze-on-damage path); the plain
 *        trio+done sites pass false.
 *
 * Stronger than default_end_handler, which only removes the DLL node and
 * marks the effect done without the figure flag cleanup. Not the same as
 * tryClearCheckSpellsBeforeJob, which clears one figure flag and never
 * removes the spell.
 */
void __thiscall sf_end_spell_now(SF_CGdSpell *_this, uint16_t spell_index, bool try_unfreeze);

/**
 * addSpellVisualEffect - Show a spell visual effect without the
 * TargetData/rect boilerplate.
 * @param _this: Spell global object owning the active spell list.
 * @param spell_index: Index of the currently active spell.
 * @param effect_id: Visual effect to apply.
 * @param target: Entity the effect is centered on; copied, so a temporary
 *        may be passed.
 * @param tick_count: Number of ticks the visual effect lasts.
 * @param corner_coords: Area rectangle for AoE effects; NULL means the
 *        zero rectangle {0, 0}.
 *
 * Keeps the live out-buffer the engine writes to (addVisualEffect's
 * @p unused slot), passes the current game tick as tick_start and leaves
 * the call sequence of the migrated blocks unchanged.
 */
void __thiscall addSpellVisualEffect(SF_CGdSpell *_this, uint16_t spell_index, CGdEffectType effect_id,
                                     const SF_CGdTargetData *target, uint16_t tick_count,
                                     const SF_Rectangle *corner_coords);

/**
 * addSpellHitEffect - Show the spell-hit visual effect on a figure.
 * @param _this: Spell global object owning the active spell list.
 * @param spell_index: Index of the currently active spell.
 * @param target_index: Figure the effect is centered on.
 * @param tick_count: Number of ticks the visual effect lasts.
 *
 * Shortcut for addSpellVisualEffect with kGdEffectSpellHitTarget, a
 * figure target (entity_type 1, zero position) and the zero rectangle.
 */
void __thiscall addSpellHitEffect(SF_CGdSpell *_this, uint16_t spell_index,
                                  uint16_t target_index, uint16_t tick_count);

/**
 * addSpellResistedEffect - Show the spell-resisted visual effect on a figure.
 * @param _this: Spell global object owning the active spell list.
 * @param spell_index: Index of the currently active spell.
 * @param target_index: Figure the effect is centered on.
 * @param tick_count: Number of ticks the visual effect lasts.
 *
 * Shortcut for addSpellVisualEffect with kGdEffectSpellTargetResisted, a
 * figure target (entity_type 1, zero position) and the zero rectangle.
 */
void __thiscall addSpellResistedEffect(SF_CGdSpell *_this, uint16_t spell_index,
                                       uint16_t target_index, uint16_t tick_count);

/**
 * addSpellHitWorldEffect - Show the world-hit visual effect at a map position.
 * @param _this: Spell global object owning the active spell list.
 * @param spell_index: Index of the currently active spell.
 * @param position: Map coordinates the effect is centered on.
 * @param tick_count: Number of ticks the visual effect lasts.
 * @param area: Area rectangle for AoE effects; NULL means the zero
 *        rectangle {0, 0}.
 *
 * Shortcut for addSpellVisualEffect with kGdEffectSpellHitWorld, a world
 * target (entity_type 4, index 0) at the given position.
 */
void __thiscall addSpellHitWorldEffect(SF_CGdSpell *_this, uint16_t spell_index, SF_Coord position,
                                       uint16_t tick_count, const SF_Rectangle *area);

/**
 * addSpellToFigure - Start a spell between two figures (entity targets).
 * @param _this: Spell global object.
 * @param spell_id: Spell type id to start.
 * @param source_index: Figure index of the source.
 * @param target_index: Figure index of the target.
 * Return: Spell slot index, 0 when no free slot.
 *
 * Shortcut for addSpell with zero-position TargetData on both sides; the
 * spell always starts at the current game tick. Figure targets only
 * (entity_type 1 implied by name); positional (4/5) spells - direct
 * addSpell (position inside TargetData is meaningful for them, e.g. the
 * AoE center). Future starts are not supported here either: use addSpell
 * directly for those.
 */
uint16_t __thiscall addSpellToFigure(SF_CGdSpell *_this, uint16_t spell_id,
                                     uint16_t source_index, uint16_t target_index);

/**
 * buildTargetRectangle - Build the bounding box around two positions.
 * @param pos_a: First position (e.g. the source figure).
 * @param pos_b: Second position (e.g. the target figure).
 * @param out_rect: Receives the packed rectangle: partA carries the
 *        top-left corner as (Y << 16 | X), partB the bottom-right one.
 */
void __thiscall buildTargetRectangle(SF_Coord pos_a, SF_Coord pos_b, SF_Rectangle *out_rect);

/**
 * sumIncomingHealFromList - Sum the heal volumes of the target's in-flight
 * spell pairs: takes a getIncomingSpellList snapshot internally, resolves
 * each pair's spell id (high ushort) through the resource table and counts
 * only HEALING_SPELL tagged lines (a flying damage spell is not incoming
 * healing), each counted spell contributing its own params[0].
 * @param figures: Global figure list.
 * @param resource: Global spell resource table.
 * @param target_index: Figure the in-flight casts are aimed at.
 * Return: Total in-flight tagged healing in HP units (uint32).
 *
 * The snapshot is owned here: taken on entry and released exactly once on
 * the single return path - the caller never sees the list buffer.
 */
uint32_t __thiscall sumIncomingHealFromList(SF_CGdFigure *figures, SF_CGdResource *resource,
                                            uint16_t target_index);

/**
 * sumIncomingPendingHeals - Total incoming heal volume for the target:
 * the in-flight tagged pairs (sumIncomingHealFromList; snapshot owned
 * internally) plus the heal amounts of allies currently walking to cast a
 * healing spell on the target (the "solved but not yet pre-cast" window;
 * job 43+ casts are already in the list, so both scopes stay disjoint).
 * @param figures: Global figure list.
 * @param resource: Global spell resource table.
 * @param allies: Ally roster to scan (EntityPriorityList).
 * @param target_index: Figure the incoming casts are aimed at.
 * Return: Total incoming healing in HP units (uint32).
 *
 * Walk-phase gate order per figure is fixed: live flags, job is WalkToTarget
 * (39), ManualWalkToTarget (125) or PreCast (103), job target is the given
 * figure, action is a HEALING_SPELL tagged spell line, then the spell
 * params[0] is added via getResourceSpellData. Pure deterministic reads of
 * sim state: no writes, no RNG; O(entityCount <= 500) field reads plus one
 * list snapshot per call.
 */
uint32_t __thiscall sumIncomingPendingHeals(SF_CGdFigure *figures, SF_CGdResource *resource,
                                            const EntityPriorityList *allies,
                                            uint16_t target_index);

/**
 * sumIncomingPendingHealsAllFigures - The walk-phase count of
 * sumIncomingPendingHeals (same per-figure predicate), but over the whole
 * figure range [1..max_used] with alliance gates instead of an
 * EntityPriorityList roster: live figure with an owner and not hostile
 * towards the target. It does not include the in-flight list part - callers
 * combine it with sumIncomingHealFromList.
 * @param toolbox: Global figure toolbox (hostility check).
 *
 * Alternative for the freshness of the ally roster: callers may
 * switch the handler call to this variant paired with sumIncomingHealFromList
 * (identical walk-phase semantics, alliance gates replace the roster); also
 * used from the aura effect context, which has no roster path.
 */
uint32_t __thiscall sumIncomingPendingHealsAllFigures(SF_CGdFigure *figures, SF_CGdResource *resource,
                                                      SF_CGdFigureToolbox *toolbox, uint16_t target_index);

extern void __thiscall spellEffectCallback(SF_CGdSpell *_this,
                                           uint16_t source_index,
                                           uint16_t spell_index,
                                           bool (*condition)(SF_CGdSpell *_this,
                                                             uint16_t
                                                             spell_index,
                                                             uint16_t
                                                             walked_index),
                                           void (*callback)(SF_CGdSpell *_this,
                                                            uint16_t
                                                            source_index,
                                                            uint16_t
                                                            walked_index,
                                                            uint16_t spell_index));

extern void __thiscall addBonusMultToStatistic(SF_CGdFigure *figure,
                                               StatisticDataKey key,
                                               uint16_t target, int8_t value);
extern void __thiscall spellClearFigureFlag(SF_CGdSpell *spell,
                                            uint16_t spell_index,
                                            SpellFlagKey key);
extern uint16_t __thiscall sf_get_spell_id(SF_CGdSpell *_this,
                                           uint16_t spell_index);
extern SFLog *setup_logger();

extern uint16_t __thiscall getPhysDamageReduction(SF_CGdFigureToolbox *_this, uint16_t source_index,
                                                  uint16_t target_index,
                                                  uint16_t action_id);




typedef uint16_t (__thiscall *get_phys_damage_reduction_ptr)(void *AutoClass34, uint16_t source_index,
                                                             uint16_t target_index,
                                                             uint16_t unkn);
extern FUN_0069eaf0_ptr FUN_0069eaf0;
extern fidfree_ptr fidFree;
extern SF_String_ctor_ptr g_create_sf_string;
extern SF_String_dtor_ptr g_destroy_sf_string;
extern has_spell_effect_ptr has_spell_effect;

extern get_phys_damage_reduction_ptr g_get_reduced_damage;

/** @brief Logs a warning at DEBUG_INFO, i.e. always shown. */
extern "C" void log_warning(const char *format, ...) LOG_PRINTF_FMT(1, 2);

/**
 * @brief Logs a warning that is only shown once the log level reaches @p level.
 */
extern "C" void log_warning_level(DebugLevel level, const char *format, ...) LOG_PRINTF_FMT(2, 3);
extern "C" void log_info(const char *format, ...) LOG_PRINTF_FMT(1, 2);
extern "C" void log_error(const char *format, ...) LOG_PRINTF_FMT(1, 2);
extern "C" void log_debug(DebugLevel level, const char *format, ...);
extern const char *debug_level_to_string(DebugLevel level);
extern DebugLevel global_debug_level;

#endif
