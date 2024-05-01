#include "api/sf_spell.h"
#include "asi/sf_asi.h"
uint16_t (__thiscall *get_spell_spell_line) (void *, uint16_t);
uint32_t (__thiscall *figure_toolbox_get_unkn)(void *, uint16_t);
void (__thiscall *figure_toolbox_add_spell)(void *, uint16_t, uint16_t);

uint16_t __thiscall addSpell(SF_CGdSpell *_this, uint16_t spell_id, uint16_t param2, SF_CGdTargetData *source, SF_CGdTargetData *target, uint16_t param5)
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
	//TODO Spell Line handler

	if (target->entity_type == 1)
	{
		if (spell_line != 0x5A)
		{
			figure_toolbox_add_spell (_this->SF_CGdFigureToolBox, target->entity_index, spell_index);
		}
	}
	return spell_index;

}

void hookBetaVersion()
{
	///this is optional, I'm lazy to make proper typedefs
	get_spell_spell_line = (uint16_t (__thiscall *)(void *, uint16_t))(ASI::AddrOf(0x26E100));
	figure_toolbox_get_unkn = (uint32_t (__thiscall *)(void *, uint16_t)) (ASI::AddrOf(0x2FE704));
	figure_toolbox_add_spell = (void (__thiscall *)(void *,uint16_t, uint16_t)) ASI::AddrOf(0x2F673A);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        if (!ASI::Init(hModule))
            return FALSE;
        //!ASI::CheckSFVersion(ASI::SF_154) &&
        if (!ASI::CheckSFVersion(ASI::SF_BETA))
        {
            return FALSE;
        }
        else
        {
            hookBetaVersion();
            break;
        }
     break;
    }
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}