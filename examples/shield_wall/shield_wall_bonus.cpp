#include "../../api/sfsf.h"
#include <windows.h>
#include <stdio.h>

// this example shows another way to implement the shieldwall spell
// it implements shieldwall via single aoe spell instead of combination of AoE and single target spells

// We declare macros for Shieldwall Spell Type and Spell Job
// Spell Type 0xf2 = 242, Spell Job 0xaa = 170

// The custom Spell Type also must be defined within GameData.cff
// and provided with at least one spell corresponding to it

#define SHIELD_WALL_UNIVERSAL_LINE 0xf6
#define SHIELD_WALL_UNIVERSAL_JOB 0xaa


SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;
IteratorFunctions *iteratorAPI;
RegistrationFunctions *registrationAPI;

// we declare spell type handler for Shieldwall, it must initialize ticks and bonus modifier values

void __thiscall shield_wall_universal_type_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // we associate spell type with a spell job
    _this->active_spell_list[spell_index].spell_job = SHIELD_WALL_UNIVERSAL_JOB;
    // we initialize values for a spell, SPELL_TICK_COUNT_AUX is familiar from previous example, it controls amount of spell ticks passed for this specific spell
    // ticks stand for multiple times when the same spell affects the target
    // however, this spell will use only two ticks at all
    // 0 for beginning to apply buff
    // 1 for ending to remove effect
    spellAPI->setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);

    // SPELL_STAT_MUL_MODIFIER will store a percentage by which the targets' armor was increased
    spellAPI->setXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER, 0);
}

// we declare spell end handler for Shieldwall
// this handler would work in case a spell wasn't finished correctly
void __thiscall shield_wall_universal_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    uint16_t target_index = spell->target.entity_index;
    spellAPI->removeDLLNode(_this, spell_index); // we remove spell from the list of active spells over the target
    spellAPI->setEffectDone(_this, spell_index, 0); // we end a spell
    // we pull the percentage by which the target's armor rating was increased
    uint16_t recalc_value = spellAPI->getXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER);
    // we remove the bonus to target's armor rating by adding negative amount of this value
    figureAPI->addBonusMultToStatistic(_this->SF_CGdFigure, ARMOR, target_index, -recalc_value);

}



// we declare spell effect handler which implements spell affecting an area
// the AoE checks for a specified amount of targets in a certain radius around the spellcaster
// if the spell happens to affect again before the previous instance expired, it resets its duration
// spell effect is simulated by individual instances of SHIELDWALL spell applied to every target affected
// this code can be used as snippet for any melee group ability that affects the caster and up to N targets nearby

void __thiscall shieldwall_universal_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // we pull the pointer for this instance of spell
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];

    // we store index of the spellcaster
    uint16_t source_index = _this->active_spell_list[spell_index].source.entity_index;

    uint32_t current_tick = spellAPI->getXData(_this, spell_index, SPELL_TICK_COUNT_AUX);
    // we increase amount of ticks passed by 1
    spellAPI->addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1);



    // it's worth of mentioning, that in case of single spell type spell, the spell is going to be automatically applied to the spellcaster when it's casted
    // however, we shouldn't be able to apply the shieldwall to the target until the previous instance of the spell is expired
    // if we just stop the spell with setEffectDone, the spell won't be casted on targets around the caster which aren't currently benefiting from the shieldwall
    // because the spell is already applied, we should remove its new instance from the caster if there is one
    // but then the spell logic should keep working intact

    // here specify the caster as the target and launch refresh check to remove the new instance of shieldwall from the caster if there is any
    spell->target.entity_index = source_index;
    spellAPI->checkCanApply(_this, spell_index);

    // the new instance was pruned, let's apply visual effects and extend the spell to surrounding figures, and also make the spell last no more than its duration
    if (current_tick == 0)
    {
    // we load the spell parameters from GameData.cff
    // we'll use them later
    SF_CGdResourceSpell spell_data;
    spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);


    // we declare structure to specify the area affected by the AoE effect
    SF_Rectangle hit_area;

    // we declare structure to store the center of the spell
    SF_Coord cast_center;
    // we get XY coordinates of the spell center with API function
    // basically, we just center the spell around its spellcaster (source)
    figureAPI->getPosition(_this->SF_CGdFigure, &cast_center, source_index);

    // we declare structure for relative position of visual effect
    SF_CGdTargetData relative_data;
    figureAPI->getPosition(_this->SF_CGdFigure, &relative_data.position, source_index);
    relative_data.entity_type = 4;
    relative_data.entity_index = 0;
    uint32_t unused;

    // we get coordinates of area affected and record them as a squared circle
    // spell_data.params[0] stands for a spell radius
    // the radius is measured in game units, 10-15 is enough for quite big spell area
    spellAPI->getTargetsRectangle(_this, &hit_area, spell_index, spell_data.params[0], &cast_center);

    // we apply the visual effect filling the area which we specified above
    spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitWorld, &unused, &relative_data, _this->OpaqueClass->current_step, 0x19, &hit_area);


    // we declare an iterator
    // iterator is a class which searches for targets (figures) in certain area
    // iterators are opaque from the user perspective.
    // just give enough memory and don't bother what's inside
    CGdFigureIterator figure_iterator;
    iteratorAPI->setupFigureIterator(&figure_iterator, _this);
    // we set iterator area as a circle of specific radius (defined in spell data) with the spellcaster being at its center
    iteratorAPI->iteratorSetArea(&figure_iterator, &cast_center, spell_data.params[0]);

    uint16_t target_index = iteratorAPI->getNextFigure(&figure_iterator);

    // the spell can affect only certain amount of figures
    // we load this amount from spell parameters in GameData.cff
    uint16_t figure_count = spell_data.params[1];

    // the spell is automatically applied to spellcaster on cast
    // so unlike in previous version of shieldwall we don't have to increase figure_count by one to apply the spell for free to spellcaster

    while (target_index != 0 && figure_count != 0)
    // we apply the spell as long as there are viable targets around and as long as we didn't exceed figures limit
    {
        // we apply the spell only to living figures belonging to our own faction, as long as those figures are targetable
        // notice that we affect those flags directly via respective structures rather than with API functions as in previous example
        // both options are allowable
        if (((int16_t)(_this->SF_CGdFigure->figures[target_index].owner) == (int16_t)(_this->SF_CGdFigure->figures[source_index].owner)) &&
            (((uint8_t)(_this->SF_CGdFigure->figures[target_index].flags) & 0xa) == 0) &&
            (toolboxAPI->isTargetable(_this->SF_CGdFigureToolBox, target_index)))
        {
            // we also can't apply the spell to target if the target is already affected with another instance of the spell
            // checkCanApply triggers the refresh handler for a spell specified via its spell_index (SHIELDWALL GROUP in this case)
            // if checkCanApply returns false, it means the SHIELDWALL already affects the target
            spell->target.entity_index = target_index;
            // checkCanApply addresses the same spell which we're currently working with (SHIELDWALL GROUP)
            // however, we have to know the index of the target which the iterator returned for us
            // because we can't pass target index directly, we change target index within the spell itself
                if (spellAPI->checkCanApply(_this, spell_index))
                {
                    // checkCanApply returned 1 here, so we add the spell to a long validated target
                    // spell_data.params[3] stands for SHIELDWALL spell data id which we're going to apply to the target
                    // _this->OpaqueClass->current_step stands for the spell starting tick (meaning game ticks, not spell tick)
                    // it's unknown what's the last parameter is standing for
                    toolboxAPI->addSpellToFigure(_this, spell_index, _this->OpaqueClass->current_step, &source, &target, 0);

                    spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitTarget, &unused, &relative_data, _this->OpaqueClass->current_step, 0x19, &hit_area);
                    // we spent one usage of the spell, hence we decrease the possible limit by one
                    figure_count--;
                }
        }
        // we search for the next target with iterator API function
        target_index = iteratorAPI->getNextFigure(&figure_iterator);
    }

    iteratorAPI->disposeFigureIterator(figure_iterator);

    // we disable the spell effect from being triggered until specified amount of time passes
    uint16_t ticks_interval = spell_data.params[1];
    _this->active_spell_list[spell_index].to_do_count = (uint16_t)((ticks_interval * 10) / 1000);
    }
    else
    // current_tick == 1, it's time to end the spell
    {
      spellAPI->setEffectDone(_this, spell_index, 0);
    }

    // all figures in radius were checked or all usages of spell was spent, we finish the SHIELDWALL GROUP spell


    // we release the memory which we allocated for iterator

}


// we declare refresh handler for AoE spell
// this handler is called whenever we're casting shieldwall universal, and will return 0 if the spell is already present or 1 when it's not
// this handler can be used as a snippet for any other similar spell which can be refreshed

int __thiscall shield_wall_universal_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index) //we casted shieldwall group again before the previous expired
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    // we declare target index with value which we stored into SHIELDWALL GROUP spell above
    uint16_t source_index = spell->source.entity_index;
    uint16_t target_index = spell->target.entity_index;

    // we check whether the figure has the Shieldwall applied to it already

    if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, SHIELD_WALL_GROUP_LINE))
        // because the spell is automatically applied on cast, we have to remove the new instance of the spell
        // however, we're not going to completely stop the spell, because it should affect friendly units around the spellcaster
        // we just remove the new instance of shieldwall from spellcaster
        if (source_index == target_index)
            {
                toolboxAPI->removeSpellFromList(_this->SF_CGdFigureToolBox, source_index, spell_index);
                return 1;
            }
        else
            // the SHIELDWALL spell already exists over the target and target isn't the spellcaster
            // we return 0, because the spell can't be applied to target at this moment
            {
            return 0;
            }
    else
        // the target isn't affected by the SHIELDWALL spell, we allow to apply the spell effect to target
        {
            return 1;
        }
}



/***
 * This function MUST be present in your code with the exact declaration
 * otherwise framework won't load your plugin
 ***/
extern "C" __declspec(dllexport) void InitModule(SpellforceSpellFramework *framework)
{
    // we pull pointers to framework methods and structures
    sfsf = framework;
    spellAPI = sfsf->spellAPI;
    toolboxAPI = sfsf->toolboxAPI;
    figureAPI = sfsf->figureAPI;
    iteratorAPI = sfsf->iteratorAPI;
    registrationAPI = sfsf->registrationAPI;


    // we register handlers for custom spell
    //
    SFSpell *shield_wall_universal_spell = registrationAPI->registerSpell(SHIELD_WALL_UNIVERSAL_LINE);
    registrationAPI->linkTypeHandler(shield_wall_universal_spell, &shield_wall_universal_type_handler);
    registrationAPI->linkEffectHandler(shield_wall_universal_spell, SHIELD_WALL_UNIVERSAL_JOB, &shield_wall_universal_effect_handler);
    registrationAPI->linkRefreshHandler(shield_wall_universal_group_spell, &shield_wall_universal_refresh_handler);
    registrationAPI->linkEndHandler(shield_wall_universal_spell, &shield_wall_universal_end_handler);
}

/***
 * This function MUST be present in your code with the exact declaration
 * otherwise the framework is unable to describe your mod in logs and mod info menu (menu not yet implemented)
 ***/
extern "C" __declspec(dllexport) SFMod *RegisterMod(SpellforceSpellFramework *framework)
{
    return framework->createModInfo("Shield wall Mod", "1.0.0", "UnSchtalch, Teekius", "A mod designed to demonstrate spell providing temporary AoE buff to player-controlled units.");
}

// Required to be present by, not required for any functionality
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
