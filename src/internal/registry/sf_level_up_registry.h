#pragma once
#include "stdint.h"


void registerLearnedAbility(uint8_t skill, uint8_t spec, uint8_t level, uint16_t ability_id);
uint16_t getLearnedAbility(uint8_t skill, uint8_t spec, uint8_t level);
