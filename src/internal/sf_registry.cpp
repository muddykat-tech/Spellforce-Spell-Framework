// Already included in sf_registry, may be good to remove.
#include "../api/sfsf.h"
#include "../api/sf_data_utilities.h"

#include "sf_spelleffect_handlers.h"
#include "sf_registry.h"
#include "sf_spelltype_registry.h"
#include "sf_spelleffect_registry.h"
#include "sf_hooks.h"

#include "sf_modloader.h"
#include "sf_utility.h"
#include <windows.h>

// See sf_data_utilities.h
SpellFunctions apiSpellFunctions;
ToolboxFunctions apiToolboxFunctions;
FigureFunctions apiFigureFunctions;
IteratorFunctions apiIteratorFunctions;

// Exposed in sfsf.h
SpellforceSpellFramework frameworkAPI;

// exposed in sf_registry.h
void registerFrameworkAPI(){
	apiSpellFunctions.setXData = setXData;
    apiSpellFunctions.setEffectDone = setEffectDone;
    apiSpellFunctions.addToXDataList = addToXDataList;
    apiSpellFunctions.getChanceToResistSpell = getChanceToResistSpell;
    apiSpellFunctions.getRandom = getRandom;
    apiSpellFunctions.initializeSpellData = &initializeSpellData;
    apiSpellFunctions.getResourceSpellData = getResourceSpellData;
    apiSpellFunctions.figureAggro = figureAggro;
    apiSpellFunctions.addVisualEffect = addVisualEffect;

    apiFigureFunctions.isAlive = isAlive;
    apiFigureFunctions.setWalkSpeed = setWalkSpeed;
    apiFigureFunctions.addAction = addAction;
    apiFigureFunctions.addBonusMult = addBonusMult;
    apiFigureFunctions.addBonusMultToStatistic = addBonusMultToStatistic;

	apiIteratorFunctions.figureIteratorInit = figureIteratorInit;
	apiIteratorFunctions.figureIteratorSetPointers = figureIteratorSetPointers;
	apiIteratorFunctions.iteratorSetArea = iteratorSetArea;
	apiIteratorFunctions.figureIteratorGetNextFigure = figureIteratorGetNextFigure;

    apiToolboxFunctions.isTargetable = figure_toolbox_is_targetable;
    apiToolboxFunctions.dealDamage = dealDamage;

	frameworkAPI.apiSpellFunctions = &apiSpellFunctions;
	frameworkAPI.apiToolboxFunctions = &apiToolboxFunctions;
	frameworkAPI.apiFigureFunctions = &apiFigureFunctions;
    frameworkAPI.apiIteratorFunctions = &apiIteratorFunctions;

	frameworkAPI.registerSpellTypeHandler = &registerSpellTypeHandler;
    frameworkAPI.registerEffectHandler = &registerEffectHandler;
    frameworkAPI.logWarning = &logWarning;
    frameworkAPI.logInfo = &logInfo;
}

void initFramework() {
    
	initDataHooks();

    // setup framework api structure references
    registerFrameworkAPI();
    
    // Setup Vanilla Spells -> see sf_spelltype_handler.h
    initSpellMap();

    // Setup effect job id registration -> see sf_spelleffect_handlers.h (This will MOVE to sf_spelleffect_registry.h)
    initEffectHandlers();

    // Attempt to load all mods -> see sf_modloader.h
    initMods();
}
