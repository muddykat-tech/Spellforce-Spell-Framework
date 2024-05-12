#ifndef HOOKS_H
#define HOOKS_H

#include "../api/sfsf.h"
#include "../api/sf_data_utilities.h"
#include "../asi/sf_asi.h"

extern void initBetaHooks();
extern void initDataHooks();
extern setXData_ptr setXData;
extern setEffectDoneFunc setEffectDone;
extern getResourceSpellData_ptr getResourceSpellData;
extern xDataListAddTo_ptr addToXDataList;
extern dealDamage_ptr dealDamage;
extern resistSpell_ptr getChanceToResistSpell;
extern figure_toolbox_is_targetable_ptr figure_toolbox_is_targetable;
extern getRandom_ptr getRandom;
extern isAlive_ptr isAlive;
extern setWalkSpeed_ptr setWalkSpeed;
extern addBonusMult_ptr addBonusMult;
extern addAction_ptr addAction;
extern addVisualEffect_ptr addVisualEffect;
extern figureAggro_ptr figureAggro;

extern void __thiscall addBonusMultToStatistic(SF_CGdFigure* figure, StatisticDataKey key, uint16_t target, uint8_t value);

extern void ConsoleLog(char*);

#endif 