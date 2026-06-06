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

extern "C" void log_warning(const char *format, ...) LOG_PRINTF_FMT(1, 2);
extern "C" void log_info(const char *format, ...) LOG_PRINTF_FMT(1, 2);
extern "C" void log_error(const char *format, ...) LOG_PRINTF_FMT(1, 2);
extern "C" void log_debug(DebugLevel level, const char *format, ...);
extern const char * debug_level_to_string(DebugLevel level);
extern DebugLevel global_debug_level;

#endif
