#pragma once

#include "../../api/sfsf.h"
#include "../../asi/sf_asi.h"
void __thiscall stone_rain_phys_handler(SF_CGdEffect *_this, uint16_t effect_id);
uint16_t __thiscall spark_phys_handler (SF_CGdEffect *_this, uint16_t source, uint16_t target, bool *isSpellDamage,
                                        uint16_t damage);
