#pragma once

#include "../../../api/sfsf.h"
#include "../../core/sf_wrappers.h"

#include <list>
#include <utility>

sub_effect_handler_ptr get_rain_handler(uint16_t spell_line);
std::list<std::pair<uint16_t, phys_effect_handler_ptr>> get_phys_effect_phase(OnHitPhase phase);

void registerPhysRainHandler(uint16_t spell_line_id, sub_effect_handler_ptr handler);
void registerPhysEffectHandler(uint16_t spell_line_id, phys_effect_handler_ptr handler, OnHitPhase phase);
