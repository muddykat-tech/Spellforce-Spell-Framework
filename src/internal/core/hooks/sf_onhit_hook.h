#ifndef SF_ONHIT_HOOK_H
#define SF_ONHIT_HOOK_H

#include "../../../api/sfsf.h"
#include "../../../asi/sf_asi.h"

void __thiscall sf_onhit_hook(void *_figureJobs, uint16_t figure_id, uint32_t param3, uint8_t param4);
void initialize_onhit_data_hooks();

#endif