#ifndef UTILITY_H
#define UTILITY_H

#include "../../api/sfsf.h"
#include "../../asi/sf_asi.h"

typedef SF_String *(__thiscall *SF_String_ctor_ptr)(SF_String *, const char *);
typedef void(__thiscall *SF_String_dtor_ptr)(SF_String *);
void log_message(const char *filename, const char *message);

void initialize_wrapper_data_hooks();

extern bool __thiscall isActionMelee(SF_SGtFigureAction *_this);
extern void __thiscall setupFigureIterator(CGdFigureIterator *iterator, SF_CGdSpell *spell);
extern void __thiscall disposeFigureIterator(CGdFigureIterator iterator);
extern void __thiscall addBonusMultToStatistic(SF_CGdFigure *figure, StatisticDataKey key, uint16_t target, int8_t value);
extern void __thiscall spellClearFigureFlag(SF_CGdSpell *spell, uint16_t spell_index, SpellFlagKey key);
extern uint16_t __thiscall sf_get_spell_id(SF_CGdSpell *_this, uint16_t spell_index);
extern uint32_t getDistance(SF_Coord *pointA, SF_Coord *pointB);
extern SFLog *setup_logger();
extern SF_String_ctor_ptr g_create_sf_string;
extern SF_String_dtor_ptr g_destroy_sf_string;

extern "C" void log_warning(const char *message);
extern "C" void log_info(const char *message);
extern "C" void log_error(const char *message);

#endif