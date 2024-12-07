#pragma once

#include "../../../api/sfsf.h"
#include "../../core/sf_wrappers.h"
#include "../../handlers/sf_ai_spell_handlers.h"
#include "../../core/sf_modloader.h"

#include <iostream>
#include <map>
#include <cstdint>
#include <list>
#include <utility>

extern void registerAiSingleTargetHandler(uint16_t spell_line, ai_single_hander_ptr handler);
extern ai_single_hander_ptr get_single_ai_handler(uint16_t spell_line);