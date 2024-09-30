#ifndef SF_ONHIT_HANDLER_H
#define SF_ONHIT_HANDLER_H

#include "../../api/sfsf.h"
#include "../../asi/sf_asi.h"

uint16_t __thiscall trueshot_onhit_handler(SF_CGdFigureJobs *_this, uint16_t source_index, uint16_t target_index, uint16_t damage);
uint16_t __thiscall riposte_onhit_handler(SF_CGdFigureJobs *_this, uint16_t source_index, uint16_t target_index, uint16_t damage);
uint16_t __thiscall assistance_onhit_handler(SF_CGdFigureJobs *_this, uint16_t source_index, uint16_t target_index, uint16_t damage);

#endif