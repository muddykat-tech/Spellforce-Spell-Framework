#ifndef SF_ENDSPELL_HOOK_H
#define SF_ENDSPELL_HOOK_H
#include "../../../api/sfsf.h"
#include "../../../asi/sf_asi.h"

void __thiscall sf_endspell_hook(SF_CGdSpell *_this, uint16_t spell_index);
void __thiscall sf_figure_end_spells_hook(SF_CGdFigureToolbox *_this, uint16_t figure_id);

#endif
