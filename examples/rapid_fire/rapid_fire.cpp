#include "../../src/api/sfsf.h"

// NOTE sfsf.h includes the OTHER api files, but other libraries are still required
#include <windows.h>
#include <stdio.h>

#define rapid_fire_LINE 0xf2
#define RAPID_FIRE_JOB 0xaa

SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;
RegistrationFunctions *registrationAPI;
SFLog *logAPI;
EffectFunctions *effectAPI;
AiFunctions *aiAPI;

bool __thiscall isActionMelee(SF_SGtFigureAction *_this)
{
    if ((_this->type == 10000) || (_this->type == 0x2711))
    {
        return 1;
    }
    return 0;
}


void __thiscall rapid_fire_type_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    _this->active_spell_list[spell_index].spell_job = RAPID_FIRE_JOB;
    spellAPI->setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall rapid_fire_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    spellAPI->figTryClrCHkSPlBfrJob2(_this, spell_index);
    spellAPI->figClrChkSplBfrChkBattle(_this, spell_index, 0);
    spellAPI->removeDLLNode(_this, spell_index);
    spellAPI->setEffectDone(_this, spell_index, 0);
}

uint16_t __thiscall rapid_fire_onhit_handler(SF_CGdFigureJobs *_this,
                                             uint16_t source_index,
                                             uint16_t target_index, uint16_t damage)
{
    SF_SGtFigureAction action;
    aiAPI->getTargetAction(_this->CGdFigure, &action, source_index);
    bool isMeleeAttack = isActionMelee(&action);
    //logAPI->logInfo("On Hit action %d", action.type);
    if (!isMeleeAttack)
    {
        return damage;
    }
    SF_CGdResourceSpell spell_data;
    uint16_t spell_index = toolboxAPI->getSpellIndexOfType(_this->CGdFigureToolBox, source_index, rapid_fire_LINE, 0);
    if (spell_index == 0)
    {
        return damage;
    }
    SF_GdSpell *spell = &_this->CGdSpell->active_spell_list[spell_index];
    uint16_t spell_id = spellAPI->getSpellID(_this->CGdSpell, spell_index);
    spellAPI->getResourceSpellData(_this->CGdResource, &spell_data, spell_id);

    SF_SpellEffectInfo effect_info;
    effect_info.spell_id = spell->spell_id;
    effect_info.job_id = spell->spell_job;

    uint16_t resistance = spellAPI->getChanceToResistSpell(_this->CGdSpell->unkn2, source_index, target_index,
                                                           effect_info);
    uint16_t random = spellAPI->getRandom(_this->OpaqueClass, 100);
    //logAPI->logInfo("On Hit active %d %d", resistance, random);
    if (resistance < random)
    {
        toolboxAPI->dealDamage(_this->CGdFigureToolBox, source_index, target_index, spell_data.params[2], 1, 0, 0);
    }
    figureAPI->subMana(_this->CGdFigure, source_index, spell_data.params[1]);
    return damage;
}

void __thiscall rapid_fire_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    uint16_t source_index = spell->source.entity_index;
    SF_CGdResourceSpell spell_data;
    spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);
    SF_CGdTargetData source_data;
    source_data.entity_index = source_index;
    source_data.entity_type = 1;
    source_data.position = {0,0};
    spell->to_do_count = (spell_data.params[0] * 10) / 1000;
    uint32_t tick = spellAPI->addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1);
    if (tick == 1)
    {
        _this->SF_CGdFigure->figures[source_index].flags |= AURA_RUNNING;
        _this->SF_CGdFigure->figures[source_index].flags |= F_CHECK_SPELLS_BEFORE_JOB;
        _this->active_spell_list[spell_index].flags |= CHECK_SPELLS_BEFORE_JOB2;
        SF_Rectangle rect = {0, 0};
        uint16_t effect_index = effectAPI->addEffect(_this->SF_CGdEffect, kGdEffectSpellHitTarget, &source_data,
                                                     &source_data,
                                                     _this->OpaqueClass->current_step, 0, &rect);

        spellAPI->setXData(_this, spell_index, EFFECT_EFFECT_INDEX, effect_index);
        effectAPI->setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_SPELL_INDEX, spell_index);
    }
    uint16_t current_mp = figureAPI->getCurrentStat(_this->SF_CGdFigure, source_index, MANA);
    uint16_t manacost = spell_data.params[1];
    if (current_mp >= manacost)
    {
        return;
    }
    if (source_index != 0)
    {
        _this->SF_CGdFigure->figures[source_index].flags &= ~AURA_RUNNING;
    }

    uint16_t effect_index = spellAPI->getXData(_this, spell_index, EFFECT_EFFECT_INDEX);
    effectAPI->tryEndEffect(_this->SF_CGdEffect, effect_index);
    spellAPI->setXData(_this, spell_index, EFFECT_EFFECT_INDEX, 0);
    spellAPI->setEffectDone(_this, spell_index, 0);
}


/***
 * This function MUST be present in your code with the exact declaration
 * otherwise framework won't load your plugin
 ***/

extern "C" __declspec(dllexport) void InitModule(SpellforceSpellFramework *framework)
{
    sfsf = framework;
    spellAPI = sfsf->spellAPI;
    toolboxAPI = sfsf->toolboxAPI;
    figureAPI = sfsf->figureAPI;
    registrationAPI = sfsf->registrationAPI;
    logAPI = sfsf->logAPI;
    effectAPI = sfsf->effectAPI;
    aiAPI = sfsf->aiAPI;

    SFSpell *rapid_fire_spell = registrationAPI->registerSpell(rapid_fire_LINE);
    registrationAPI->linkTypeHandler(rapid_fire_spell, &rapid_fire_type_handler);
    registrationAPI->linkEffectHandler(rapid_fire_spell, RAPID_FIRE_JOB, &rapid_fire_effect_handler);
    registrationAPI->linkEndHandler(rapid_fire_spell, &rapid_fire_end_handler);
    registrationAPI->applySpellTag(rapid_fire_spell,SpellTag::AURA_SPELL);
    registrationAPI->linkOnHitHandler(rapid_fire_spell, &rapid_fire_onhit_handler, PHASE_4);
}


/***
 * This function MUST be present in your code with the exact declaration
 * otherwise the framework is unable to describe your mod in logs and mod info menu (menu not yet implemented)
 ***/

extern "C" __declspec(dllexport) SFMod *RegisterMod(SpellforceSpellFramework *framework)
{
    return framework->createModInfo("The custom spell rapid_fire", "1.0.0", "UnSchtalch",
                                    "On hit aura test");
}


// Required to be present by OS, not required for any functionality

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            /* Code path executed when DLL is loaded into a process's address space. */
            break;

        case DLL_THREAD_ATTACH:
            /* Code path executed when a new thread is created within the process. */
            break;

        case DLL_THREAD_DETACH:
            /* Code path executed when a thread within the process has exited *cleanly*. */
            break;

        case DLL_PROCESS_DETACH:
            /* Code path executed when DLL is unloaded from a process's address space. */
            break;
    }

    return TRUE;
}
