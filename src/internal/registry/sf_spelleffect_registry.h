#ifndef SF_SPELLEFFECT_REGISTRY_H
#define SF_SPELLEFFECT_REGISTRY_H

#include "../../api/sfsf.h"

extern handler_ptr get_spell_effect (const uint16_t spell_job);
extern void registerEffectHandler(uint16_t spell_job, handler_ptr handler);

#endif