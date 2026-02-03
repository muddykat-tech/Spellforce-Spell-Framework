#include "../../src/api/sfsf.h"

// NOTE sfsf.h includes the OTHER api files, but other libraries are still required
#include <windows.h>
#include <stdio.h>

#define HEAVY_STRIKES_LINE 250
#define HEAVY_STRIKES_JOB 190

SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;
EffectFunctions *effectAPI;
RegistrationFunctions *registrationAPI;
AiFunctions *aiAPI;
SFLog *logAPI;

void __thiscall heavy_strikes_type_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    _this->active_spell_list[spell_index].spell_job = HEAVY_STRIKES_JOB;
    spellAPI->setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
    spellAPI->setXData(_this, spell_index, EFFECT_EFFECT_INDEX, 0);
}

void __thiscall heavy_strikes_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdFigure *sf_figures = _this->SF_CGdFigure;
    uint16_t source_index = _this->active_spell_list[spell_index].source.entity_index;
    if ((sf_figures->figures[source_index].owner != (uint16_t)(-1)) &&
        ((sf_figures->figures[source_index].flags & GdFigureFlags::REDO) == 0))
    {
        sf_figures->figures[source_index].flags = static_cast<GdFigureFlags>(sf_figures->figures[source_index].flags &
                                                                             (~static_cast<unsigned int>(GdFigureFlags::
                                                                                                         AURA_RUNNING)));
    }
    uint16_t effect_index = spellAPI->getXData(_this, spell_index, EFFECT_EFFECT_INDEX);
    if (effect_index != 0)
    {
        effectAPI->tryEndEffect(_this->SF_CGdEffect, effect_index);
    }
    spellAPI->figTryClrCHkSPlBfrJob2(_this, spell_index);
    spellAPI->figClrChkSplBfrChkBattle(_this, spell_index, 0);
    spellAPI->removeDLLNode(_this, spell_index);
    spellAPI->setEffectDone(_this, spell_index, 0);
}

void __thiscall heavy_strikes_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    SF_CGdResourceSpell spell_data;

    uint16_t source_index = spell->source.entity_index;
    spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);
    _this->active_spell_list[spell_index].to_do_count = (spell_data.params[2] * 10) / 1000;

    SF_CGdTargetData source_data;
    source_data.entity_index = source_index;
    source_data.entity_type = 1;
    source_data.position = {0,0};
    if (spell->source.entity_type == 1)
    {
        if ((_this->SF_CGdFigure->figures[source_index].owner != (uint16_t)(-1)) &&
            ((*(uint8_t *)(&_this->SF_CGdFigure->figures[source_index].flags) & 0xa) == 0))
        {

            uint32_t tick = spellAPI->addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1);
            if (tick == 1)
            {
                _this->SF_CGdFigure->figures[source_index].flags |= AURA_RUNNING;
                SF_Rectangle rect = {0, 0};
                uint16_t effect_index = effectAPI->addEffect(_this->SF_CGdEffect, kGdEffectSpellHitTarget, &source_data,
                                                             &source_data,
                                                             _this->OpaqueClass->current_step, 0, &rect);

                spellAPI->setXData(_this, spell_index, EFFECT_EFFECT_INDEX, effect_index);
                effectAPI->setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_SPELL_INDEX, spell_index);

                _this->SF_CGdFigure->figures[source_index].flags |= F_CHECK_SPELLS_BEFORE_JOB;
                _this->active_spell_list[spell_index].flags |= CHECK_SPELLS_BEFORE_JOB2;

            }
            uint16_t mana_cost = spell_data.params[1];
            uint16_t current_mp = figureAPI->getCurrentStat(_this->SF_CGdFigure, source_index, MANA);
            if (current_mp >= mana_cost)
            {
                return;
            }
        }
    }
    if (source_index != 0)
    {
        _this->SF_CGdFigure->figures[source_index].flags &= ~AURA_RUNNING;
    }
    uint16_t effect_index = spellAPI->getXData(_this, spell_index, EFFECT_EFFECT_INDEX);
    effectAPI->tryEndEffect(_this->SF_CGdEffect, effect_index);
    spellAPI->setXData(_this, spell_index, EFFECT_EFFECT_INDEX, 0);
    spellAPI->setEffectDone(_this, spell_index, 0);
    return;

}

uint16_t __thiscall heavy_strikes_onhit_handler(SF_CGdFigureJobs *_this, uint16_t source_index, uint16_t target_index,
                                                uint16_t damage)
{
    if (damage == 0 || damage == 0x7fff)
    {
        return damage;
    }
    SF_SGtFigureAction action;
    aiAPI->getTargetAction(_this->CGdFigure, &action, source_index);

    uint16_t spell_index = toolboxAPI->getSpellIndexOfType(_this->CGdFigureToolBox, source_index, HEAVY_STRIKES_LINE,
                                                           0);

    uint16_t spell_id = spellAPI->getSpellID(_this->CGdSpell, spell_index);
    SF_CGdResourceSpell spell_data;
    spellAPI->getResourceSpellData(_this->CGdResource, &spell_data, spell_id);

    bool isMeleeAttack = 0;
    if ((action.type == 10000) || (action.type == 10001))
    {
        isMeleeAttack = 1;
    }
    if (isMeleeAttack)
    {
        uint16_t damage_increase = spell_data.params[0];
        uint16_t mana_cost = spell_data.params[1];
        uint16_t current_mp = figureAPI->getCurrentStat(_this->CGdFigure, source_index, MANA);
        if (current_mp >= mana_cost)
        {
            damage += damage_increase;
            figureAPI->subMana(_this->CGdFigure, source_index, mana_cost);
        }
        else
        {
            if (source_index != 0)
            {
                _this->CGdFigure->figures[source_index].flags &= ~AURA_RUNNING;
            }
            uint16_t effect_index = spellAPI->getXData(_this->CGdSpell, spell_index, EFFECT_EFFECT_INDEX);
            effectAPI->tryEndEffect(_this->CGdEffect, effect_index);
            spellAPI->setXData(_this->CGdSpell, spell_index, EFFECT_EFFECT_INDEX, 0);
            spellAPI->setEffectDone(_this->CGdSpell, spell_index, 0);
        }
    }
    return damage;
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
    effectAPI = sfsf->effectAPI;
    aiAPI = sfsf->aiAPI;
    logAPI = sfsf->logAPI;

    SFSpell *heavy_strikes_spell = registrationAPI->registerSpell(HEAVY_STRIKES_LINE);
    registrationAPI->linkTypeHandler(heavy_strikes_spell, &heavy_strikes_type_handler);
    registrationAPI->linkEffectHandler(heavy_strikes_spell, HEAVY_STRIKES_JOB, &heavy_strikes_effect_handler);
    registrationAPI->linkEndHandler(heavy_strikes_spell, &heavy_strikes_end_handler);
    registrationAPI->linkOnHitHandler(heavy_strikes_spell, &heavy_strikes_onhit_handler, OnHitPhase::PHASE_4);
    registrationAPI->applySpellTag(heavy_strikes_spell,SpellTag::AURA_SPELL);
    registrationAPI->applySpellTag(heavy_strikes_spell,SpellTag::COMBAT_ABILITY_SPELL);

}


/***
 * This function MUST be present in your code with the exact declaration
 * otherwise the framework is unable to describe your mod in logs and mod info menu (menu not yet implemented)
 ***/

extern "C" __declspec(dllexport) SFMod *RegisterMod(SpellforceSpellFramework *framework)
{
    return framework->createModInfo("Heavy strikes aura", "1.0.0", "UnSchtalch",
                                    "This heavy_strikes provides example of custom aura spell");
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
