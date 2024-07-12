#ifndef SF_SPELLTYPE_HOOK_H
#define SF_SPELLTYPE_HOOK_H

#include "../../../api/sfsf.h"
#include "../../../asi/sf_asi.h"

extern figure_toolbox_get_unkn_ptr figure_toolbox_get_unkn;
extern get_spell_spell_line_ptr get_spell_spell_line;
extern figure_toolbox_add_spell_ptr figure_toolbox_add_spell;

uint16_t __thiscall sf_spelltype_hook(SF_CGdSpell *_this, uint16_t spell_id, uint16_t param2, SF_CGdTargetData *source, SF_CGdTargetData *target, uint16_t param5);

#endif