#pragma once

#include "../../../api/sfsf.h"
#include "../../core/sf_wrappers.h"
#include "../../handlers/sf_ai_aoe_handlers.h"
#include "../../core/sf_modloader.h"

#include <iostream>
#include <map>
#include <cstdint>
#include <list>
#include <utility>

extern void registerAiAOEHandler(uint16_t spell_line, ai_aoe_handler_ptr handler);
extern ai_aoe_handler_ptr get_ai_aoe_handler(uint16_t spell_line);