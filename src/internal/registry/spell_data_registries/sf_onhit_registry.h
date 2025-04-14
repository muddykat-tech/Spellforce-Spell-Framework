#ifndef SF_ONHIT_REGISTRY_H
#define SF_ONHIT_REGISTRY_H

#include "../../../api/sfsf.h"
#include <list>
#include <utility>

extern std::list<std::pair<uint16_t,
                           onhit_handler_ptr> > get_onhit_phase(
    OnHitPhase hitphase);
extern void registerOnHitHandler(uint16_t spell_line_id,
                                 onhit_handler_ptr handler, OnHitPhase phase);

#endif
