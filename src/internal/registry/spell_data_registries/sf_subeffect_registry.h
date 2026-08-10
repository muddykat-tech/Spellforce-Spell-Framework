#pragma once

#include <stdint.h>
#include "../../../api/sfsf.h"

extern void registerSubEffectHandler(uint16_t spell_line,
                                     sub_effect_handler_ptr handler);
extern sub_effect_handler_ptr get_sub_effect_handler(uint16_t spell_line);
extern void register_vanilla_sub_effect_handlers();
