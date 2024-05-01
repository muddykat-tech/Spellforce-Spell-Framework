#include "api/sf_spell.h"
#include "asi/sf_asi.h"

uint16_t __thiscall addSpell(void *_this, uint16_t spell_id, uint16_t param2, SF_CGdTargetData *source, SF_CGdTargetData *target, uint16_t param5)
{
	uint16_t spell_index;
	uint16_t spell_line;
	if (target->entity_type == 1)
	{
		if (FALSE) //TODO call fun_06fe704 with _this+0x24 == 0
		{
			return 0;
		}
	}
	SF_GdSpell *active_spell_list = (SF_GdSpell *)((uint32_t)_this+0x56);

	//find 1st free slot for spell
	for (spell_index =1; spell_index < 799; ++spell_index)
	{
		if (active_spell_list[spell_index].spell_id == 0)
		{
			break;
		}
	}
	if (spell_index == 799)
	{
		return 0;
	}
	//max_used spells update
	if (*(uint16_t *)((uint32_t)_this+0x3C) < spell_index)
	{
		*(uint16_t *)((uint32_t)_this+0x3C) = spell_index;
	}
	active_spell_list[spell_index].spell_id = spell_id;
	spell_line = 1; ///TODO call fun_066e100 with _this+0x3c as param
	active_spell_list[spell_index].spell_line = spell_line;
	active_spell_list[spell_index].source.entity_type = source->entity_type;
	active_spell_list[spell_index].source.entity_index = source->entity_index;
	active_spell_list[spell_index].source.position.X = (source->position).X;
	active_spell_list[spell_index].source.position.Y = (source->position).Y;
	active_spell_list[spell_index].target.entity_type = target->entity_type;
	active_spell_list[spell_index].target.entity_index = target->entity_index;
	active_spell_list[spell_index].target.position.X = (target->position).X;
	active_spell_list[spell_index].target.position.Y = (target->position).Y;
	//dirty AF
	uint32_t unkn_ptr = *((uint32_t *)(((uint32_t)_this+0x4)));
	uint16_t current_step = *(uint16_t *)(unkn_ptr+0xc);
	active_spell_list[spell_index].to_do_count = param2 - current_step;
	//TODO Spell Line handler

	return spell_index;

}

void hookBetaVersion()
{

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