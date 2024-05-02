#include "sf_modloader.h"
#include "sf_hooks.h"
#include "sf_spellhandler.h"

uint16_t __thiscall addSpell_hook_beta(SF_CGdSpell *_this, uint16_t spell_id, uint16_t param2, SF_CGdTargetData *source, SF_CGdTargetData *target, uint16_t param5)
{
	uint16_t spell_index;
	uint16_t spell_line;
	if (target->entity_type == 1)
	{
		if (!figure_toolbox_get_unkn(_this->SF_CGdFigureToolBox, target->entity_index))
		{
			return 0;
		}
	}

	//find 1st free slot for spell
	for (spell_index =1; spell_index < 799; ++spell_index)
	{
		if (_this->active_spell_list[spell_index].spell_id == 0)
		{
			break;
		}
	}
	if (spell_index == 799)
	{
		return 0;
	}
	//max_used spells update
	if (_this->max_used < spell_index)
	{
		_this->max_used = spell_index;
	}

	//somewhere here is smth goes south
	_this->active_spell_list[spell_index].spell_id = spell_id;
	spell_line = get_spell_spell_line (_this->SF_CGdResource, spell_id);
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

	handler_ptr func = get_spell_handler(handler_map, spell_line);
	func (_this, spell_index);

	if (target->entity_type == 1)
	{
		if (spell_line != 0x5A)
		{
			figure_toolbox_add_spell (_this->SF_CGdFigureToolBox, target->entity_index, spell_index);
		}
	}
	return spell_index;

}

// Exposed in sf_hooks.h
void initBetaHooks()
{
	get_spell_spell_line = (get_spell_spell_line_ptr) (ASI::AddrOf(0x26E100));
	figure_toolbox_get_unkn = (figure_toolbox_get_unkn_ptr) (ASI::AddrOf(0x2FE704));
	figure_toolbox_add_spell = (figure_toolbox_add_spell_ptr) (ASI::AddrOf(0x2F673A));
	setXData = (setXData_ptr) ASI::AddrOf(0x329C40);

	ASI::MemoryRegion add_spell_mreg(ASI::AddrOf(0x328d60), 5); //building selector
    ASI::BeginRewrite(add_spell_mreg);
        *(unsigned char*)(ASI::AddrOf(0x328d60)) = 0xE9;   // jmp instruction
        *(int*)(ASI::AddrOf(0x328d61)) = (int)(&addSpell_hook_beta) - ASI::AddrOf(0x328d65);
    ASI::EndRewrite(add_spell_mreg);
}
