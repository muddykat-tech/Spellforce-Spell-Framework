#pragma once

#include "../../../api/sfsf.h"

extern void registerAiSingleTargetHandler(uint16_t spell_line, ai_single_hander_ptr handler);
extern ai_single_hander_ptr get_single_ai_handler(uint16_t spell_line);