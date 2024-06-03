#include "../../src/api/sfsf.h"
#include <windows.h>
#include <stdio.h>
// For convenience only; You can put headers just near the file and remove ../api/
// NOTE sfsf.h includes the OTHER api files, the other files are still required

// Another convenience: you can ommit functions you won't need, or call everything from the framework structure
SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;
IteratorFunctions *iteratorAPI;


void __thiscall thuderstorm_type_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    _this->active_spell_list[spell_index].spell_job = 0xf2;
    spellAPI->setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall thuderstorm_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    uint16_t source_index = spell->source.entity_index;
    // Iterators are opaque from the user perspective.
    // Just give enough memory and don't bother what's inside
    CGdFigureIterator iterator_memory;
    iteratorAPI->setupFigureIterator(&iterator_memory, _this);

    // iteratorAPI->figureIteratorInit(&iterator_memory, 0x0, 0x0, 0x3ff, 0x3ff);
    // iteratorAPI->figureIteratorSetPointers(&iterator_memory, _this->SF_CGdFigure, _this->unkn3, _this->SF_CGdWorld);

    SF_SpellEffectInfo effect_info;
    SF_CGdResourceSpell spell_data;
    spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);

    effect_info.spell_id = spell->spell_id;
    effect_info.job_id = spell->spell_job;
    uint16_t ticks_total = spell_data.params[2];
    uint16_t ticks_interval = spell_data.params[3];
    SF_Rectangle hit_area;
    SF_Coord cast_center = _this->active_spell_list[spell_index].target.position;
    // SF_Coord
    spellAPI->getTargetsRectangle(_this, &hit_area, spell_index, spell_data.params[0], &cast_center);

    uint32_t tick_current = spellAPI->getXData(_this, spell_index, SPELL_TICK_COUNT_AUX);
    uint16_t ticks_passed = spellAPI->addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1);

    // visual effect addition, if it's 1st tick
    if (tick_current == 0)
    {
        SF_CGdTargetData relative_data;
        relative_data.position = cast_center;
        relative_data.entity_type = 4;
        relative_data.entity_index = 0;
        uint32_t unused;
        spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitWorld, &unused, &relative_data, _this->OpaqueClass->current_step, 0x19, &hit_area);
    }

    iteratorAPI->iteratorSetArea(&iterator_memory, &cast_center, spell_data.params[0]);
    uint16_t target_index = iteratorAPI->getNextFigure(&iterator_memory);

    if (ticks_passed < ticks_total)
    {
        while (target_index != 0)
        {
            // If figure is not special unit
            // not dead and is not friendly
            // and is targetable - let's work with it
            if (((int16_t)(_this->SF_CGdFigure->figures[target_index].owner) != -1) &&
                (((uint8_t)(_this->SF_CGdFigure->figures[target_index].flags) & 0xa) == 0) &&
                (toolboxAPI->figuresCheckHostile(_this->SF_CGdFigureToolBox, source_index, target_index)) &&
                (toolboxAPI->isTargetable(_this->SF_CGdFigureToolBox, target_index)))
            {
                /* debug output example
                Let it be here
                char aliveInfo[256];
                sprintf(aliveInfo, "Flags list: Target %hd \n", target_index);
                sfsf->logInfo(aliveInfo);
                */
                uint16_t random_roll = spellAPI->getRandom(_this->OpaqueClass, 100);
                uint32_t resist_chance = spellAPI->getChanceToResistSpell(_this->unkn2, source_index, target_index, effect_info);
                if ((uint32_t)resist_chance < random_roll)
                {
                    if (figureAPI->isAlive(_this->SF_CGdFigure, target_index))
                    {
                        SF_CGdTargetData relative_data;
                        relative_data.position = {0, 0};
                        relative_data.entity_type = 1;
                        relative_data.entity_index = target_index;
                        uint32_t unused;
                        SF_Rectangle aux_data;
                        aux_data.partA = 0;
                        aux_data.partB = 0;
                        spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitTarget, &unused, &relative_data, _this->OpaqueClass->current_step, 10, &hit_area);
                        toolboxAPI->dealDamage(_this->SF_CGdFigureToolBox, source_index, target_index, spell_data.params[1], 1, 0, 0);
                    }
                    else
                    {
                        spellAPI->figureAggro(_this, spell_index, target_index);
                    }
                }
            }
            target_index = iteratorAPI->getNextFigure(&iterator_memory);
        }
        _this->active_spell_list[spell_index].to_do_count = (uint16_t)((ticks_interval * 10) / 1000);
    }
    else
    {
        spellAPI->setEffectDone(_this, spell_index, 0);
    }
    iteratorAPI->disposeFigureIterator(iterator_memory);
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
    iteratorAPI = sfsf->iteratorAPI;
    // Here comes the spell type registration.
    // Note: you need to pass pointer to function here
    sfsf->registerSpellTypeHandler(242, &thuderstorm_type_handler);

    // Here comes the spell logic (effect) registration; f2 = 242
    sfsf->registerEffectHandler(0xf2, &thuderstorm_effect_handler);
}

/***
 * This function MUST be present in your code with the exact declaration
 * otherwise the framework is unable to describe your mod in logs and mod info menu (menu not yet implemented)
 ***/
extern "C" __declspec(dllexport) SFMod RegisterMod(SpellforceSpellFramework* framework) {
    return framework->createModInfo("Persistent AOE Example Mod", "1.0.0", "UnSchtalch", "A mod designed to provide an example of a Persistent AOE spell");
}
 
// Required to be present, not required for any functionality
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