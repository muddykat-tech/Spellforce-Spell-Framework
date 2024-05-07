#include "sf_modloader.h"
#include "sf_hooks.h"
#include "sf_spelltype_handlers.h"
#include "sf_spelltype_registry.h"
#include "sf_spelleffect_handlers.h"
#include "sf_utility.h"

#include "../api/sf_data_utilities.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

setXData_ptr setXData;
ConsolePrint_ptr ConsolePrint;
get_spell_spell_line_ptr get_spell_spell_line;
figure_toolbox_get_unkn_ptr figure_toolbox_get_unkn;
figure_toolbox_add_spell_ptr figure_toolbox_add_spell;
uint32_t CMnuScrConsole_ptr = 0;
SF_String_ctor_ptr SF_String_ctor;
SF_String_dtor_ptr SF_String_dtor;

void __thiscall triggerEffect_hook(SF_CGdSpell *_this){
	uint16_t spell_index;
	for (spell_index = 1; spell_index <= _this->max_used; ++spell_index)
    {
        if (_this->active_spell_list[spell_index].spell_id != 0)
        {
            short to_do_count = _this->active_spell_list[spell_index].to_do_count;
            if(to_do_count == 0){
				to_do_count = 0;
			} else {
				to_do_count = to_do_count + -1;
			}
			_this->active_spell_list[spell_index].to_do_count = to_do_count;
			if(to_do_count == 0){
				uint16_t spell_job = _this->active_spell_list[spell_index].spell_job;
				handler_ptr func = get_spell_effect(spell_job);
				func(_this, spell_index);
			}
        }
    }
}


void ConsoleLog(char* message)
{
	if (!CMnuScrConsole_ptr) return;
	SF_String sf_string;
	SF_String_ctor(&sf_string, message);
	ConsolePrint(CMnuScrConsole_ptr, &sf_string);
	// Free the memory once log has been sent.
	SF_String_dtor(&sf_string);

}

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
		logWarning("Warning: Maximum spell limit (800) reached. Additional spells may not be registered.");
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

	handler_ptr func = get_spell_handler(spell_line);
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

void initConsoleHook(){
	uint32_t CAppMain_ptr = ASI::AddrOf(0x9229A8);
	uint32_t CAppMenu_ptr = *(uint32_t*) (CAppMain_ptr + 0x4);
	uint32_t CMnuScrConsole_ptr = *(uint32_t*) (CAppMenu_ptr + 0x80);
	ConsolePrint = (ConsolePrint_ptr) ASI::AddrOf(0x534e70);
	SF_String_ctor =(SF_String_ctor_ptr) ASI::AddrOf(0x3837e0);
	SF_String_dtor =(SF_String_dtor_ptr) ASI::AddrOf(0x3839c0);
}

void initDataHooks(){
	get_spell_spell_line = (get_spell_spell_line_ptr) (ASI::AddrOf(0x26E100));
	figure_toolbox_get_unkn = (figure_toolbox_get_unkn_ptr) (ASI::AddrOf(0x2FE704));
	figure_toolbox_add_spell = (figure_toolbox_add_spell_ptr) (ASI::AddrOf(0x2F673A));
	setXData = (setXData_ptr) ASI::AddrOf(0x329C40);
}

void initSpellTypeHook(){
	ASI::MemoryRegion add_spell_mreg(ASI::AddrOf(0x328d60), 5); //building selector
    ASI::BeginRewrite(add_spell_mreg);
        *(unsigned char*)(ASI::AddrOf(0x328d60)) = 0xE9;   // jmp instruction
        *(int*)(ASI::AddrOf(0x328d61)) = (int)(&addSpell_hook_beta) - ASI::AddrOf(0x328d65);
    ASI::EndRewrite(add_spell_mreg);
}

// Exposed in sf_hooks.h
void initBetaHooks()
{
	initConsoleHook();
	initDataHooks();
	initSpellTypeHook();
}