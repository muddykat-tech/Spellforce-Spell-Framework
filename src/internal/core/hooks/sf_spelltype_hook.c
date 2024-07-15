#include "../../registry/sf_spelltype_registry.h"
#include "../sf_wrappers.h"
#include "../sf_hooks.h"

#include "sf_spelltype_hook.h"

static figure_toolbox_get_unkn_ptr s_figure_toolbox_get_unkn;
static get_spell_spell_line_ptr s_get_spell_spell_line;
static figure_toolbox_add_spell_ptr s_figure_toolbox_add_spell;

void initialize_spelltype_data_hooks()
{
    s_get_spell_spell_line = (get_spell_spell_line_ptr)(ASI::AddrOf(0x26E100));
    s_figure_toolbox_get_unkn = (figure_toolbox_get_unkn_ptr)(ASI::AddrOf(0x2FE704));
    s_figure_toolbox_add_spell = (figure_toolbox_add_spell_ptr)(ASI::AddrOf(0x2F673A));
}

uint16_t __thiscall sf_spelltype_hook(SF_CGdSpell *_this, uint16_t spell_id, uint16_t param2, SF_CGdTargetData *source, SF_CGdTargetData *target, uint16_t param5)
{
    uint16_t spell_index;
    uint16_t spell_line;
    if (target->entity_type == 1)
    {
        if (!s_figure_toolbox_get_unkn(_this->SF_CGdFigureToolBox, target->entity_index))
        {
            return 0;
        }
    }

    // find 1st free slot for spell
    for (spell_index = 1; spell_index < 799; ++spell_index)
    {
        if (_this->active_spell_list[spell_index].spell_id == 0)
        {
            break;
        }
    }
    if (spell_index == 799)
    {
        log_warning("Maximum spell limit (800) reached. Additional spells may not be registered.");
        return 0;
    }
    // max_used spells update
    if (_this->max_used < spell_index)
    {
        _this->max_used = spell_index;
    }

    // somewhere here is smth goes south
    _this->active_spell_list[spell_index].spell_id = spell_id;
    spell_line = s_get_spell_spell_line(_this->SF_CGdResource, spell_id);
    _this->active_spell_list[spell_index].spell_line = spell_line;
    _this->active_spell_list[spell_index].source.entity_type = source->entity_type;
    _this->active_spell_list[spell_index].source.entity_index = source->entity_index;
    _this->active_spell_list[spell_index].source.position.X = (source->position).X;
    _this->active_spell_list[spell_index].source.position.Y = (source->position).Y;
    _this->active_spell_list[spell_index].target.entity_type = target->entity_type;
    _this->active_spell_list[spell_index].target.entity_index = target->entity_index;
    _this->active_spell_list[spell_index].target.position.X = (target->position).X;
    _this->active_spell_list[spell_index].target.position.Y = (target->position).Y;
    _this->active_spell_list[spell_index].to_do_count = param2 - (uint16_t)(_this->OpaqueClass->current_step);

    handler_ptr func = get_spell_handler(spell_line);
    func(_this, spell_index);

    if (target->entity_type == 1)
    {
        if (spell_line != 0x5A)
        {
            s_figure_toolbox_add_spell(_this->SF_CGdFigureToolBox, target->entity_index, spell_index);
        }
    }
    return spell_index;
}