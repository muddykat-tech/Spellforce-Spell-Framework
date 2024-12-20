#include "../../api/sfsf.h"
#include "../../asi/sf_asi.h"
#include "../core/sf_hooks.h"

uint32_t __thiscall raise_dead_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data);
uint32_t __thiscall area_heal_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data);
uint32_t __thiscall revenge_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data);
uint32_t __thiscall torture_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data);
uint32_t __thiscall fog_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data);
uint32_t __thiscall area_pain_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data);
uint32_t __thiscall rain_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data);
uint32_t __thiscall area_freeze_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data);
uint32_t __thiscall hypnotize_area_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data);
//actually roots and darkness area.
//by default if handler is missing we will default to area pain
uint32_t __thiscall default_aoe_offensive_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data);