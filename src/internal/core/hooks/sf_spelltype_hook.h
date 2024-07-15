#ifndef SF_SPELLTYPE_HOOK_H
#define SF_SPELLTYPE_HOOK_H

#include "../../../api/sfsf.h"
#include "../../../asi/sf_asi.h"

uint16_t __thiscall sf_spelltype_hook(SF_CGdSpell *_this, uint16_t spell_id, uint16_t param2, SF_CGdTargetData *source, SF_CGdTargetData *target, uint16_t param5);
void initialize_spelltype_data_hooks();

#endif