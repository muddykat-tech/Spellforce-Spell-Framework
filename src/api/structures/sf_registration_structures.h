#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "../sf_general_structures.h"

typedef uint16_t (__thiscall *damage_handler_ptr)(SF_CGdFigureToolbox *_toolbox,
                                                  uint16_t source,
                                                  uint16_t target,
                                                  uint16_t current_damage,
                                                  uint16_t is_spell_damage,
                                                  uint32_t is_ranged_damage,
                                                  uint16_t spell_id);

typedef void (__thiscall *handler_ptr)(SF_CGdSpell *, uint16_t);
typedef uint32_t (__thiscall *ai_aoe_handler_ptr)(
    SF_CGdBattleDevelopment *_this, SF_Coord *position, uint16_t spell_line,
    SF_CGdResourceSpell *spell_data);
typedef uint32_t (__thiscall *ai_single_handler_ptr)(
    SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line,
    SF_CGdResourceSpell *spell_data);
typedef uint32_t (__thiscall *ai_avoidance_handler_ptr)(CGdAIBattleData *_this,
                                                        uint16_t figure_index,
                                                        uint16_t spell_line);
typedef uint16_t (__thiscall *onhit_handler_ptr)(SF_CGdFigureJobs *,
                                                 uint16_t source,
                                                 uint16_t target,
                                                 uint16_t damage);
typedef int (__thiscall *refresh_handler_ptr)(SF_CGdSpell *, uint16_t);

typedef struct __attribute__((packed))
{
    uint16_t spell_id;
    uint16_t spell_effect_id;
    uint16_t spell_tags;
    handler_ptr spell_type_handler;
    handler_ptr spell_effect_handler;
    handler_ptr spell_end_handler;
    onhit_handler_ptr spell_onhit_handler;
    refresh_handler_ptr spell_refresh_handler;
    sub_effect_handler_ptr sub_effect_handler;
    damage_handler_ptr deal_damage_handler;
    ai_aoe_handler_ptr ai_aoe_handler;
    ai_single_handler_ptr ai_single_handler;
    ai_avoidance_handler_ptr ai_avoidance_handler;
    SpellDamagePhase damage_phase;
    OnHitPhase hit_phase;
    SFMod *parent_mod;
} SFSpell;
