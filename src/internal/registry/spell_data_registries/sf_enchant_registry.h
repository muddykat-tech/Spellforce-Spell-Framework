#pragma once

#include "../../../api/sfsf.h"

enchant_handler_ptr get_enchant_handler(uint16_t spell_line);
void registerEnchantHandler(uint16_t spell_line_id, enchant_handler_ptr handler);
void register_vanilla_enchants_handlers();
