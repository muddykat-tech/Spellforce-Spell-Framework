#ifndef SF_ONHIT_REGISTRY_H
#define SF_ONHIT_REGISTRY_H

#include "../../../api/sfsf.h"
#include <list>
#include <utility>

extern std::list<std::pair<uint16_t, onhit_handler_ptr> > get_onhit_phase(OnHitPhase hitphase);
extern void registerOnHitHandler(uint16_t spell_line_id, onhit_handler_ptr handler, OnHitPhase phase);

void run_onhit_shot_phase(SF_CGdFigureJobs *jobs, uint16_t source_index,
                          uint16_t target_index, uint16_t donor_effect_id);

#endif
