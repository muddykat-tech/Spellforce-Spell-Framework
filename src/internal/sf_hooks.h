#ifndef HOOKS_H
#define HOOKS_H

#include "../api/sfsf.h"
#include "../api/sf_data_utilities.h"

extern void initBetaHooks();
extern void initDataHooks();
extern setXData_ptr setXData;
extern setEffectDoneFunc setEffectDone;
extern xDataListAddTo_ptr addToXDataList;
extern dealDamage_ptr dealDamage;
extern resistSpell_ptr getChanceToResistSpell;
extern getRandom_ptr getRandom;

extern void ConsoleLog(char*);

#endif 