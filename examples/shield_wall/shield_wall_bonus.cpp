#include "../../src/api/sfsf.h"
#include <windows.h>
#include <stdio.h>

// this example shows another way to implement the shieldwall spell
// it implements shieldwall via single aoe spell instead of combination of two spells (AoE and individual)
// such implementation is closer to vanilla implementation of AoE buffs

// We declare macros for Shieldwall Spell Type and Spell Job
// Spell Type 0xf6 = 246, Spell Job 0xad = 173

// The custom Spell Type also must be defined within GameData.cff
// and provided with at least one spell corresponding to it

#define SHIELD_WALL_UNIVERSAL_LINE 0xf6
#define SHIELD_WALL_UNIVERSAL_JOB 0xad


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

    // we recall armor rating bonus modifier from spell XData
    // because this value is the same for every target affected, we should do it here instead of doing it within the loop below
    uint16_t recalc_value = spellAPI->getXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER);

    for (uint16_t target_index = 0; target_index <= _this->SF_CGdFigure->max_used; target_index++)
        {
            // this code reiterates what we're doing at the tick 1 when the spell duration ends
            // we scan for all figures present, and check whether they're affected with Shieldwall spell
            // if they're affected, and 'getSpellIndexOfType' which is ordered to ignore the given spell_index returns 0, it means they're affected only with the instance of Shieldwall we're searching for
            // then we remove the spell from the target's spell list and fix the target's statistic
            uint16_t spell_index_current = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox, target_index, SHIELD_WALL_UNIVERSAL_LINE, spell_index);

            // we check whether the target is affected with the Shieldwall
            if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, SHIELD_WALL_UNIVERSAL_LINE))
                // if the target is affected with the Shieldwall and 'getSpellIndexOfType' returned 0, it means the target is affected with the spell we're looking for
                if (spell_index_current == 0)
                                {
                                    // we remove the spell from the target
                                    toolboxAPI->removeSpellFromList(_this->SF_CGdFigureToolBox, target_index, spell_index);
                                    // we substract bonus modifier from the target's armor rating
                                    figureAPI->addBonusMultToStatistic(_this->SF_CGdFigure, ARMOR, target_index, -recalc_value);
                                }
        }
    // we end the spell itself after we finished scanning AoE
    spellAPI->removeDLLNode(_this, spell_index);
    spellAPI->setEffectDone(_this, spell_index, 0);
}



// we declare the effect handler for the shieldwall
// unlike the shieldwall group, it will run a check for AoE and change the armor rating for all affected targets within itself
// the second spell won't


void __thiscall shield_wall_universal_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // we pull the pointer for this instance of spell
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];

    // we initiate ticks counter which is necessary to emulate spell duration
    uint32_t current_tick = spellAPI->getXData(_this, spell_index, SPELL_TICK_COUNT_AUX);
    // we increase amount of ticks passed by 1
    spellAPI->addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1);


    if (current_tick == 0)
        {
        // we load the spell parameters from GameData.cff
        // we'll need them only in tick 0
        // we will access armor bonus directly with spell XData in tick 1
        SF_CGdResourceSpell spell_data;
        spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);

        // we store index of the spellcaster
        uint16_t source_index = _this->active_spell_list[spell_index].source.entity_index;

        // we pull bonus modifier to armor rating from spell data into local variable
        uint8_t recalc_value = spell_data.params[2];

        // we store bonus modifier directly within spell XData to be able to access it in the subsequent tick
        spellAPI->setXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER, recalc_value);


        // the game automatically adds the spell to a source on cast
        // in previous implementation we controlled that, because the actual spell effect was related to the second spell type
        // and we decided ourselves whether we should apply the spell which provides bonus armor modifier to a target/source
        // however, when we use a single spell type, the shieldwall is automatically applied to the spellcaster

        // let's check whether the spell we're currently handling is the first instance of shieldwall over the target
        // and if that's true, we'll add bonus modifier to spellcaster's armor rating
        // to do so we should obtain the index of a Shieldwall spell affecting the target

        uint16_t spell_index_current = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox, source_index, SHIELD_WALL_UNIVERSAL_LINE, spell_index);

        // getSpellIndexOfType returns the spell index of a spell with specific Spell Type
        // it returns the spell index it gets first, if there are more than one instance of specific Spell Type

        // it's possible to make it ignore the specific spell index when it's searching for the spell
        // so it would return 0, if the target is affected only with the current instance of Shieldwall

        if (spell_index_current == 0)
            // if spell_index_current returns 0, it means there were no other instances of Shieldwall over a creature
            // it indicates that the instance we're currently handling is new
            // we're allowed to apply the spell logic to spellcaster
            {
                figureAPI->addBonusMultToStatistic(_this->SF_CGdFigure, ARMOR, source_index, recalc_value);
            }

        else
            // in case spell_index_current returns other value than 0, it means we're working with the additional instance of Shieldwall
            // we shouldn't add bonus modifier to the spellcaster's armor rating
            // but since the spell is treated as already applied to the source by game engine, we should remove it from the source's UI
            // however, we shouldn't stop the spell overall
            // it will continue working with other creatures in area through iterator
            {
                toolboxAPI->removeSpellFromList(_this->SF_CGdFigureToolBox, source_index, spell_index);
            }

        // we go as usual with adding area visual effect centered on spellcaster
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


        // we make sure the iterator doesn't return us the spellcaster, because the spellcaster was already buffed at the start of the tick
        while (target_index == source_index)
            {
                target_index = iteratorAPI->getNextFigure(&figure_iterator);
            }

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
                // checkCanApply triggers the refresh handler for a spell specified via its spell_index (SHIELDWALL UNIVERSAL in this case)
                // if checkCanApply returns false, it means the SHIELDWALL already affects the target
                spell->target.entity_index = target_index;
                // checkCanApply addresses the same spell which we're currently working with (SHIELDWALL UNIVERSAL)
                // however, we have to know the index of the target which the iterator returned for us
                // because we can't pass target index directly, we change target index within the spell itself
                    if (spellAPI->checkCanApply(_this, spell_index))
                        {
                            // checkCanApply returned 1 here, so we add the spell to a long validated target
                            // spell_data.params[3] stands for SHIELDWALL spell data id which we're going to apply to the target
                            // _this->OpaqueClass->current_step stands for the spell starting tick (meaning game ticks, not spell tick)
                            // it's unknown what's the last parameter is standing for
                            toolboxAPI->addSpellToFigure(_this->SF_CGdFigureToolBox, target_index, spell_index);

                            figureAPI->addBonusMultToStatistic(_this->SF_CGdFigure, ARMOR, target_index, recalc_value);

                            spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitTarget, &unused, &relative_data, _this->OpaqueClass->current_step, 0x19, &hit_area);
                            // we spent one usage of the spell, hence we decrease the possible limit by one
                            figure_count--;
                        }
                }
                // we search for the next target with iterator API function
                target_index = iteratorAPI->getNextFigure(&figure_iterator);
            }
            // all figures in radius were checked or all usages of spell was spent, we finish the SHIELDWALL UNIVERSAL spell
            // we release the memory which we allocated for iterator
            iteratorAPI->disposeFigureIterator(figure_iterator);
            // we disable the spell effect from being triggered until specified amount of time passes
            uint16_t ticks_interval = spell_data.params[3];
            _this->active_spell_list[spell_index].to_do_count = (uint16_t)((ticks_interval * 10) / 1000);
        }
        else
        // current_tick == 1, it's time to end the spell
        {
            // we recall armor rating bonus modifier from spell XData
            // because this value is the same for every target affected, we should do it here instead of doing it within the loop below
            uint16_t recalc_value = spellAPI->getXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER);

            // we check through all figures for the Shieldwall effect
            // we can't use the iterator to determine search area, because targets might move too far away from the source which provided them with the Shieldwall
            for (uint16_t target_index = 0; target_index <= _this->SF_CGdFigure->max_used; target_index++)
                {
                    // we launch the same check as we did in the tick 0
                    // we want to be sure that the Shieldwall over the target is the same Shieldwall as we're planning to remove
                    uint16_t spell_index_current = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox, target_index, SHIELD_WALL_UNIVERSAL_LINE, spell_index);

                        // we check whether the target is affected with the Shieldwall
                        if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, SHIELD_WALL_UNIVERSAL_LINE))
                            // if the target is affected with the Shieldwall and 'getSpellIndexOfType' returned 0, it means the target is affected with the spell we're looking for
                            if (spell_index_current == 0)
                                {
                                    // we remove the spell from the target
                                    toolboxAPI->removeSpellFromList(_this->SF_CGdFigureToolBox, target_index, spell_index);
                                    // we substract bonus modifier from the target's armor rating
                                    figureAPI->addBonusMultToStatistic(_this->SF_CGdFigure, ARMOR, target_index, -recalc_value);
                                }
                }

        // we finish the spell and remove it from the active spells list
        spellAPI->removeDLLNode(_this, spell_index);
        spellAPI->setEffectDone(_this, spell_index, 0);
        }
}


// we declare refresh handler for AoE spell
// this handler is called whenever we're casting shieldwall universal, and will return 0 if the spell is already present or 1 when it's not
// this handler can be used as a snippet for any other similar spell which can be refreshed

int __thiscall shield_wall_universal_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index) // we casted shieldwall group again before the previous expired
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    // we declare target index with value which we stored into SHIELDWALL UNIVERSAL spell above
    uint16_t target_index = spell->target.entity_index;

    // we check whether the figure has the Shieldwall applied to it already

    if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, SHIELD_WALL_UNIVERSAL_LINE))
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
    SFSpell *shield_wall_universal_spell = registrationAPI->registerSpell(SHIELD_WALL_UNIVERSAL_LINE);
    registrationAPI->linkTypeHandler(shield_wall_universal_spell, &shield_wall_universal_type_handler);
    registrationAPI->linkEffectHandler(shield_wall_universal_spell, SHIELD_WALL_UNIVERSAL_JOB, &shield_wall_universal_effect_handler);
    registrationAPI->linkRefreshHandler(shield_wall_universal_spell, &shield_wall_universal_refresh_handler);
    registrationAPI->linkEndHandler(shield_wall_universal_spell, &shield_wall_universal_end_handler);
}

/***
 * This function MUST be present in your code with the exact declaration
 * otherwise the framework is unable to describe your mod in logs and mod info menu (menu not yet implemented)
 ***/
extern "C" __declspec(dllexport) SFMod *RegisterMod(SpellforceSpellFramework *framework)
{
    return framework->createModInfo("Shieldwall Mod", "1.0.0", "Teekius", "A mod designed to demonstrate spell providing temporary AoE buff to player-controlled units. This version implements AoE spell using the single Spell Type instead of two interrelated spells.");
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
