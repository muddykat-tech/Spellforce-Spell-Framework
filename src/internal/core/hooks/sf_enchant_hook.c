#include "sf_enchant_hook.h"
#include "../../registry/spell_data_registries/sf_enchant_registry.h"
#include "../../core/sf_wrappers.h"


uint32_t __attribute__((no_caller_saved_registers, thiscall)) sf_enchant_hook(SF_CGdFigure *_this, uint16_t figure_id,
                                                                              uint16_t spell_line_id)
{
    //log_info ("figure id: %d, spelline id %d", figure_id, spell_line_id);
    enchant_handler_ptr handler = get_enchant_handler(spell_line_id);
    return handler(_this, figure_id);
}
