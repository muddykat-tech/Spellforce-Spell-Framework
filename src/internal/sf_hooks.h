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
extern isAlive_ptr isAlive;
extern setWalkSpeed_ptr setWalkSpeed;
extern addBonusMult_ptr addBonusMult;
extern addAction_ptr addAction;
extern void __thiscall addBonusMultToStatistic(SF_CGdFigure* figure, StatisticDataKey key, uint16_t target, uint8_t value);

extern void ConsoleLog(char*);

#endif 