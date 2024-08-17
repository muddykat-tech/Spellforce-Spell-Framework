#include "../../src/api/sfsf.h"
#include <windows.h>
#include <stdio.h>

#define SHIELD_WALL_LINE 0xf4
#define SHIELD_WALL_JOB 0xaa
#define PARRY_LINE 0xf4
#define PARRY_JOB 0xab

SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;
IteratorFunctions *iteratorAPI;
RegistrationFunctions *registrationAPI;

void __thiscall parry_type_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    _this->active_spell_list[spell_index].spell_job = PARRY_JOB;
    spellAPI->setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
    spellAPI->setXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER, 0);
}

void __thiscall parry_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    uint16_t source_index = spell->source.entity_index;
    if (curren_tick == 0)
    {
    figureAPI->addBonusMultToStatistic(_this->SF_CGdFigure, ARMOR, target_index, spell_data.params[0]);
    uint16_t recalc_value = 0; //how much percent we need to decrease armour later on
    spellAPI->setXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER, recal_vaulue );
    //trick with todocount
        return;
    }
    else
    {
        uint16_t recalc_value = spellAPI->getXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER);
        figureAPI->addBonusMultToStatistic(_this->SF_CGdFigure, ARMOR, target_index, recalc_value);
    }
       spellAPI->setEffectDone(_this, spell_index, 0);

}

void __thiscall shield_wall_type_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    _this->active_spell_list[spell_index].spell_job = SHIELD_WALL_JOB;
}

void __thiscall shield_wall_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    uint16_t source_index = spell->source.entity_index;
    // Iterators are opaque from the user perspective.
    // Just give enough memory and don't bother what's inside
    CGdFigureIterator figure_iterator;
    iteratorAPI->setupFigureIterator(&figure_iterator, _this);

    SF_SpellEffectInfo effect_info;
    SF_CGdResourceSpell spell_data;
    effect_info.spell_id = spell->spell_id;
    effect_info.job_id = spell->spell_job;
    SF_Rectangle hit_area;
    SF_Coord cast_center = _this->active_spell_list[spell_index].target.position;
    // SF_Coord
    spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);
    spellAPI->getTargetsRectangle(_this, &hit_area, spell_index, spell_data.params[0], &cast_center);
    SF_CGdTargetData relative_data;
    relative_data.position = cast_center;
    relative_data.entity_type = 4;
    relative_data.entity_index = 0;
    uint32_t unused;
    spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitWorld, &unused, &relative_data, _this->OpaqueClass->current_step, 0x19, &hit_area);
    iteratorAPI->iteratorSetArea(&figure_iterator, &cast_center, spell_data.params[2]);
    uint16_t figure_count = spell_data.params[1];
    uint16_t target_index = iteratorAPI->getNextFigure(&figure_iterator);
    while (target_index != 0 && figure_count != 0)
    {
        SF_CGdResourceSpell spell_data_2;
        spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data_2, spell_data.params[0]);

        if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, spell_data_2.spell_line_id))
        {
            target_index = iteratorAPI->getNextFigure(&figure_iterator);
            continue;
        }
        // If figure is not special unit
        // not dead and is not friendly
        // and is targetable - let's work with it
        if (((int16_t)(_this->SF_CGdFigure->figures[target_index].owner) == (int16_t)(_this->SF_CGdFigure->figures[source_index].owner)) &&
            (((uint8_t)(_this->SF_CGdFigure->figures[target_index].flags) & 0xa) == 0) &&
            (toolboxAPI->isTargetable(_this->SF_CGdFigureToolBox, target_index)))
        {
            toolboxAPI->addSpellToFigure(_this->SF_CGdFigureToolBox, target_index, spell_data.params[0]);
            figure_count--;
        }
        target_index = iteratorAPI->getNextFigure(&figure_iterator);
    }
    spellAPI->setEffectDone(_this, spell_index, 0);

    iteratorAPI->disposeFigureIterator(figure_iterator);
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
    registrationAPI = sfsf->registrationAPI;

    SFSpell *shield_wall_spell = registrationAPI->registerSpell(SHIELD_WALL_LINE);
    registrationAPI->linkTypeHandler(aoe_instant_spell, &shield_wall_type_handler);
    registrationAPI->linkEffectHandler(aoe_instant_spell, SHIELD_WALL_JOB, &shield_wall_effect_handler);

    SFSpell *parry_spell = registrationAPI->registerSpell(PARRY_LINE);
    registrationAPI->linkTypeHandler(aoe_instant_spell, &parry_type_handler);
    registrationAPI->linkEffectHandler(aoe_instant_spell, PARRY_JOB, &parry_effect_handler);
}

/***
 * This function MUST be present in your code with the exact declaration
 * otherwise the framework is unable to describe your mod in logs and mod info menu (menu not yet implemented)
 ***/
extern "C" __declspec(dllexport) SFMod *RegisterMod(SpellforceSpellFramework *framework)
{
    return framework->createModInfo("Shield wall Mod", "1.0.0", "UnSchtalch", "A mod designed to provide an example combat ability");
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