#include "../../src/api/sfsf.h"
#include <windows.h>
#include <stdio.h>

// We declare macros for Spell Type and Spell Job of both spells
// it is very unhandy to keep them in mind, so we just declare them here
// we use a new set of numbers to avoid interfering with previous examples

// New Spell Type will be 0xf5 (245), it must be defined within GameData.cff
// and provided with at least one spell corresponding each Spell Type
// new Spell Job will be 0xac (172) to avoid interfering with numbers used in previous examples

#define INTERFERENCE_LINE 0xf5
#define INTERFERENCE_JOB 0xac

// we also need to know Spell Types and Spell Jobs for vanilla spells which will be mutually exclusive with Interference
// let's get Spell Types from GameData.cff and write down as hexadecimals

#define PATRONIZE_LINE 0x96 // 150
#define SHELTER_LINE 0x9A   // 154

// spell jobs are obtained with reverse-engineering
// they can be checked at sf_spelleffect_registry.cpp within API internal code
// https://github.com/muddykat-tech/Spellforce-Spell-Framework/blob/master/src/internal/registry/sf_spelleffect_registry.cpp

#define PATRONIZE_JOB 0x6b // 107
#define SHELTER_JOB 0x6f   // 111

SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;
IteratorFunctions *iteratorAPI;
RegistrationFunctions *registrationAPI;
SFLog *logger;

// we declare spell type handler for Interference
void __thiscall interference_type_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // we link the specific spell type with its own spell job
    _this->active_spell_list[spell_index].spell_job = INTERFERENCE_JOB;
    // we ensure that a spell starting tick will be 0
    spellAPI->setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

// we declare spell end handler for Interference
// this handler would work in case a spell wasn't finished correctly
void __thiscall interference_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    spellAPI->removeDLLNode(_this, spell_index);    // we remove spell from the list of active spells affecting the target
    spellAPI->setEffectDone(_this, spell_index, 0); // we end a spell
}

// we are used to the fact that spell logic is implemented within the spell effect handler
// however, interference is very special, its main logic (reducing incoming damage) will be trigged within Damage handler
// it's declared in the following way
// the handler intercepts the incoming damage, we can modify it and return the new value
// then the game will proceed with applying the modified damage to the target's health as usual
// if we don't modify the damage due to some reasons (for example, the spell doesn't absorb damage from ranged attacks), we still should return the initial value with the function
// otherwise the damage will be lost

uint16_t __thiscall interference_deal_damage_handler(SF_CGdFigureToolbox *_toolbox, uint16_t source, uint16_t target,
                                                     uint16_t current_damage, uint16_t is_spell_damage, uint32_t is_ranged_damage, uint16_t spell_id)
{

    // _toolbox stands for the unit which is affected with our spell and got damaged with any means
    // source specifies the figure_index of a damaging figure
    // target specifies the figure_index of a damaged figure
    // current_damage specifies the amount of damage
    // is_spell_damage and is_ranged_damage specify the damage origin
    // spell_id returns an ID of the spell which triggered the Damage handler

    if (is_spell_damage)
    // we check if the damage which trigged Damage handler comes from the spell
    {
        // we load spell_data of Interference spell, spell_id stands for the ID of the spell which governs the Damage handler
        SF_CGdResourceSpell spell_data;
        spellAPI->getResourceSpellData(_toolbox->CGdResource, &spell_data, spell_id);

        // we decrease damage by the percentage stated in spell parameters
        current_damage += -(uint16_t)((current_damage * spell_data.params[1]) / 100);
        // we return the modified damage value and game engine uses the modified value as usual
        return current_damage;
    }

    // the damage wasn't caused with another spell, we return its value unchanged
    return current_damage;
}

// we declare effect handler for the Interference
// in this case effect handler will only govern the spell duration, and will remove the spell when the specified amount of time passes
// the bulk of spell logic is implemented in damage handler above

void __thiscall interference_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // we pull the pointer for this instance of spell
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];

    // we store index of the spellcaster, we won't need target index for this spell
    uint16_t source_index = _this->active_spell_list[spell_index].source.entity_index;


    // bool check_spells_before_job = figureAPI->isFlagSet(figureToolbox->CGdFigure, source_index, F_CHECK_SPELLS_BEFORE_JOB);

    // we get the current tick of the spell, should be 0 at the beginning, and 1 in the end
    uint32_t current_tick = spellAPI->getXData(_this, spell_index, SPELL_TICK_COUNT_AUX);
    // we increase amount of ticks passed by 1
    spellAPI->addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1);

    // we load the spell from GameData.cff in order to get tick_interval
    SF_CGdResourceSpell spell_data;
    spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);

    if (current_tick == 0)
    // spell start
    {
        if (spellAPI->checkCanApply(_this, spell_index))
        {
            // let's trigger refresh for interference or other vanilla spells which work in the same way
            // the refresh handler will clear them from the target if either of them is currently affecting the target

            // we declare structures to store relative position of visual effect
            SF_CGdTargetData relative_data;
            relative_data.position = {0, 0};
            relative_data.entity_type = 1;
            relative_data.entity_index = source_index;
            uint32_t unused;

            SF_Rectangle aux_data;
            aux_data.partA = 0;
            aux_data.partB = 0;

            // we apply the visual effect filling the area which we specified above
            spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitTarget, &unused, &relative_data, _this->OpaqueClass->current_step, 0x5, &aux_data);
            // the parameter before the last stands for spell area, effects affecting single target usually have this parameter set to 0
            // but if we want to make the effect look cool, we could put the number above 0 and it will look like an AoE spell on activation

            // we have to activate the flag F_CHECK_SPELLS_BEFORE_JOB in order to make it possible for Deal Damage handler to trigger when the damage is received
            _this->SF_CGdFigure->figures[source_index].flags |= F_CHECK_SPELLS_BEFORE_JOB;

            // we activate the flag F_CHECK_SPELLS_BEFORE_BATTLE for the sake of optimization
            _this->active_spell_list[spell_index].flags |= 2;

            // let's get spell duration from game data
            uint16_t ticks_interval = spell_data.params[0];
            // we disable the spell from being triggered for a specified number of internal game ticks, and after a specified in ticks_interval amount of time has passed, we may remove the spell
            _this->active_spell_list[spell_index].to_do_count = (uint16_t)((ticks_interval * 10) / 1000);
        }
    }
    else
    // spell end, main spell logic is implemented with another handlerso we have only to clear flag which allowed triggering damage handler and stop the spell in this block
    {
        // we should clear the flags we raised during tick 0 for the sake of optimization
        spellAPI->figTryClrCHkSPlBfrJob2(_this, spell_index);
        spellAPI->figClrChkSplBfrChkBattle(_this, spell_index, 0);
        spellAPI->setEffectDone(_this, spell_index, 0); // we end a spell
    }
}


// removing the Patronize requires running a loop to clear the spell from all targets affected
// because we remove the Patronize in different handlers (Spell Effect, Spell Refresh)
// it's more convenient to pack it in a function and call this function whenever we want to remove the Patronize

void ClearPatronizeInArea(SF_CGdSpell *_this, uint16_t spell_index)
{
    for (uint16_t target_index = 0; target_index <= _this->SF_CGdFigure->max_used; target_index++)
        // we run a loop through all figures in game and check whether they're affected with the Patronize
        // we can't use an iterator, because affected figures might move outside a specific radius around the spellcaster
    {
        if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, PATRONIZE_LINE) &&
            (figureAPI->getSpellJobStartNode(_this->SF_CGdFigure, target_index) != 0))
            // we check whether the target has Patronize on them
            // we check whether this spell is actually present in the list of spells affecting the creature

            // however, we will remove only the specific instance of Patronize which we know by spell_index argument
            // if the creature was under the Patronize from another source, that Patronize won't be removed
                {
                    toolboxAPI->removeSpellFromList(_this->SF_CGdFigureToolBox, target_index, spell_index);

                    /*char aliveInfo[256];
                    sprintf(aliveInfo, "PATRONIZE WAS REMOVED FROM FIGURE %hd \n", target_index);
                    logger->logInfo(aliveInfo);*/
                }
    }

    // we also can finish the entire spell within this function, to avoid copy-pasting this block into all places which call the function
    spellAPI->removeDLLNode(_this, spell_index);
    spellAPI->setEffectDone(_this, spell_index, 0);
}


// the custom implementation of vanilla spell effect handler for the Patronize
// it's necessary in order to make the custom refresh handler work
// otherwise custom refresh handler gets ignored due to a bug in vanilla code

// worth of noting, that the Patronize is also AoE spell similar to the Shieldwall from the previous example
// however, the Patronize is made as as a single spell rather than as combination of two spells

void __thiscall patronize_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // we pull the pointer for this instance of spell
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];

    // we store index of the spellcaster, we won't need target index for this spell
    uint16_t source_index = _this->active_spell_list[spell_index].source.entity_index;

    // we get the current tick of the spell, should be 0 at the beginning, and 1 in the end
    uint32_t current_tick = spellAPI->getXData(_this, spell_index, SPELL_TICK_COUNT_AUX);
    // we increase amount of ticks passed by 1
    spellAPI->addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1);

    if (current_tick == 0)
    {
        // spell start
        if (spellAPI->checkCanApply(_this, spell_index))
        {


            // we load the spell from GameData.cff in order to get tick_interval
            SF_CGdResourceSpell spell_data;
            spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);

            // key for spell radius of Patronize is 2
            // key for amount of figures affected is 3

            uint16_t effect_radius = spell_data.params[2];
            uint16_t figure_count = spell_data.params[3]; // the effect is automatically applied to the target, we don't need to check it within iterator

            SF_Coord cast_center;
            // we get XY coordinates of the spell center with API function
            // basically, we just center the spell around its spellcaster (source)
            figureAPI->getPosition(_this->SF_CGdFigure, &cast_center, source_index);

            // we declare structures to store relative position of visual effect
            SF_CGdTargetData relative_data;
            relative_data.position = {0, 0};
            relative_data.entity_type = 1;
            relative_data.entity_index = source_index;
            uint32_t unused;

            SF_Rectangle hit_area;

            // we get coordinates of area affected and record them as a squared circle
            spellAPI->getTargetsRectangle(_this, &hit_area, spell_index, spell_data.params[0], &cast_center);

            // we apply the visual effect filling the area which we specified above
            spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitWorld, &unused, &relative_data, _this->OpaqueClass->current_step, 0x25, &hit_area);


            hit_area.pathA = 0;
            hit_area.pathB = 0;
            spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitTarget, &unused, &relative_data, _this->OpaqueClass->current_step, 0x96, &aux_data);

            CGdFigureIterator figure_iterator;
            iteratorAPI->setupFigureIterator(&figure_iterator, _this);
            // we set iterator area as a circle of specific radius (defined in spell data) with the spellcaster being at its center
            iteratorAPI->iteratorSetArea(&figure_iterator, &cast_center, effect_radius);

            uint16_t target_index = iteratorAPI->getNextFigure(&figure_iterator);

            // the Patronize is automatically added to a target on spell cast, we don't have to add it via iterator loop
            // if the iterator picked the spellcaster as a the first target, let's get the next target
            while (target_index == source_index)
            {
                target_index = iteratorAPI->getNextFigure(&figure_iterator);
            }

            // let's do the iterator loop until we exhaust spell uses or there are no valid targets in radius anymore
            while (figure_count != 0 && target_index != 0)
            {
                // let's check whether the figure belongs to the spellcaster team, whether it's alive and whether it's targetable
                if (((int16_t)(_this->SF_CGdFigure->figures[target_index].owner) == (int16_t)(_this->SF_CGdFigure->figures[source_index].owner)) &&
                    (((uint8_t)(_this->SF_CGdFigure->figures[target_index].flags) & 0xa) == 0) &&
                    (toolboxAPI->isTargetable(_this->SF_CGdFigureToolBox, target_index)))

                    // we should pass target index to spell data, because otherwise refresh handler won't be able to access it
                    spell->target.entity_index = target_index;

                    // let's check whether the spell can be applied to a target
                    // or call for refresh it it's already present
                    if (spellAPI->checkCanApply(_this, spell_index))
                        // the refresh handler allowed to apply the spell to a target
                        {
                        // we have to apply the visual effect to each target separately
                        SF_CGdTargetData relative_data;
                        relative_data.position = {0, 0};
                        relative_data.entity_type = 1; // 1 stands for individual figure

                        // entity_index must be set to correspond the target_index, otherwise the effect won't be persistent over a creature
                        relative_data.entity_index = target_index; //
                        uint32_t unused;

                        SF_Rectangle aux_data;
                        aux_data.partA = 0;
                        aux_data.partB = 0;


                        // because we don't pass 0 for duration (we pass 0x96 as in vanilla implementation), it will create lingering effect below the creature
                        // so we will see it while the target is affected with the Patronize
                        spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitTarget, &unused, &relative_data, _this->OpaqueClass->current_step, 0x96, &aux_data);

                        // we extend the current spell to a target
                        toolboxAPI->addSpellToFigure(_this->SF_CGdFigureToolBox, target_index, spell_index);

                        // we decrease the spell uses amount by 1
                        figure_count--;
                    /*char aliveInfo[256];
                    sprintf(aliveInfo, "Patronize added to target index: %hd \n", target_index);
                    logger->logInfo(aliveInfo);*/
                }
                // we search for the next figure in radius
                target_index = iteratorAPI->getNextFigure(&figure_iterator);
            }

            // the loop has ended either because we exhausted all uses or all targets in radius
            // let's dispose of figures iterator
            iteratorAPI->disposeFigureIterator(figure_iterator);

            // let's get spell duration from game data, for Patronize key for spell duration is 0
            uint16_t ticks_interval = spell_data.params[0];

            // we disable the spell from being triggered for a specified number of internal game ticks, and after a specified in ticks_interval amount of time has passed, we may remove the spell
            _this->active_spell_list[spell_index].to_do_count = (uint16_t)((ticks_interval * 10) / 1000);
            logger->logInfo("PATRONIZE ACTIVATED");
        }
        else
        // the Patronize can't be applied to the spellcaster due to some reasons
        // let's finish the spell to avoid it going for the tick 1
        {
            spellAPI->setEffectDone(_this, spell_index, 0);
            logger->logInfo("PATRONIZE CAN'T BE APPLIED");
        }
    }
    else
    // spell end, main spell logic is implemented within another functions, let's just clear the spell from targets
    {
        // we call for function which will clear the spell from the spellcaster and all targets affected
        // we pass the current spell spell_index to identify it and not mistake with any other instances of Patronize which might be present over nearest characters
        ClearPatronizeInArea(_this, spell_index);
        logger->logInfo("PATRONIZE FINISHED");
    }
}

// the custom implementation of vanilla spell effect handler for the Shelter
// it's necessary in order to make the custom refresh handler work
// otherwise custom refresh handler gets ignored due to a bug in vanilla code

void __thiscall shelter_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // we pull the pointer for this instance of spell
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];

    // we store index of the spellcaster, we won't need target index for this spell
    uint16_t source_index = _this->active_spell_list[spell_index].source.entity_index;

    // we get the current tick of the spell, should be 0 at the beginning, and 1 in the end
    uint32_t current_tick = spellAPI->getXData(_this, spell_index, SPELL_TICK_COUNT_AUX);

    // we increase amount of ticks passed by 1
    spellAPI->addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1);

    if (current_tick == 0)
    // spell start
    {
        // let's check whether we can apply the shelter to a target
        // whether we should refresh it
        // and whether we should clear the instance of Patronize over a target
        if (spellAPI->checkCanApply(_this, spell_index))
        {
            // the check returned TRUE, let's apply the spell and its visuals

            // we load the spell from GameData.cff in order to get tick_interval
            SF_CGdResourceSpell spell_data;
            spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);


            // we declare structures to store relative position of visual effect

            SF_CGdTargetData relative_data;
            relative_data.position = {0, 0};
            relative_data.entity_type = 1;

            // it's important to assign the source (target) index as an entity index for relative data
            // otherwise the visual effect won't linger over a spellcaster for the spell duration
            relative_data.entity_index = source_index;
            uint32_t unused;

            SF_Rectangle aux_data;
            aux_data.partA = 0;
            aux_data.partB = 0;

            // we apply the visual effect filling the area which we specified above
            spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitTarget, &unused, &relative_data, _this->OpaqueClass->current_step, 0x96, &aux_data);

            // let's get spell duration from game data
            // shelter uses 0 for spell duration
            uint16_t ticks_interval = spell_data.params[0];
            // we disable the spell from being triggered for a specified number of internal game ticks, and after a specified in ticks_interval amount of time has passed, we may remove the spell
            _this->active_spell_list[spell_index].to_do_count = (uint16_t)((ticks_interval * 10) / 1000);
            logger->logInfo("SHELTER ACTIVATED");
        }
        else
        {
        // the Shelter can't be applied to the spellcaster being under the Interference
        // let's finish the spell to avoid it going for the tick 1
        spellAPI->setEffectDone(_this, spell_index, 0); // we end a spell
        logger->logInfo("SHELTER CAN'T BE APPLIED");
        }
    }
    else
    // spell stops, main spell logic is implemented within another functions, so let's just end the spell
    {
        spellAPI->setEffectDone(_this, spell_index, 0); // we end a spell
        logger->logInfo("SHELTER FINISHED");
    }
}

// we declare refresh handler for group of three spells: Interference, Patronize, Shelter
// they provide the same effect, hence we have to prevent them from stacking
// in the previous example we implemented that the mechanics according to which spell blocks the casting if it's already affecting the target
// here we will remove already affecting spells and make the figure clear for applying new spell
// the handler is the same for all three spells: custom one, and shelter and patronize from vanilla

int __thiscall interference_patronize_shelter_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index) // we casted shieldwall group again before the previous expired
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];

    // except for patronize, the spell is cast on self, we can use the source index
    // because patronize is AoE spell, we will have to use target indice, but we will access them directly via spell pointer
    uint16_t source_index = spell->source.entity_index;


    // in hindsight, it will make more sense to implement three separate refresh handlers for each of spells we make
    // however, vanilla spells generally use a single refresh handler for mutually exclusive spells, and we're aiming to replicate this
    // let's just implement switcher to know which spell triggered the refresh handler now

    uint8_t scenario = 0;

    if (spell->spell_job == INTERFERENCE_JOB)
        scenario = 1;

    if (spell->spell_job == PATRONIZE_JOB)
        scenario = 2;

    if (spell->spell_job == SHELTER_JOB)
        scenario = 3;


    // below we'll check for which spells the target is currently affected with
    // the general idea is to remove all spells which are lower in hierarchy
    // or remove the previous instance of the spell itself to make the spell refreshable
    // if we encounter the spell which is higher in hierarchy, we return 0, which means we can't apply the current spell
    // if no higher spells were found, we return 1, which means the spell can be applied

    if (scenario == 1)
    // we run checks for interference spell
    {
        if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, source_index, INTERFERENCE_LINE))
        // the INTERFERENCE spell already exists on the target
        {
            // here comes the magic
            // we can get the spell index from the list of active spells affecting the target by knowing this spell's spell job id
            // we pass spell_index given with the refresh handler as the last known spell index for this figure, but this value is insignificant (used for optimizing search purposes?)

            uint16_t pruned_spell_index = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox, source_index, INTERFERENCE_LINE, spell_index);

            // the trick is getSpellIndexOfType would return 0 if the spell_index given to us with the refresh handler (current spell) corresponds to the spell index we found
            // it prevents us from removing the spell which we're trying to add
            // however, it's better to additionally check for whether new and old spell_indices don't match

            // we finish the INTERFERENCE spell using its spell index
            // we should remove the spell correctly, so we remove both Effect and DLLNode here

            if (spell_index != pruned_spell_index && pruned_spell_index != 0)
            {
                spellAPI->removeDLLNode(_this, pruned_spell_index);
                spellAPI->setEffectDone(_this, pruned_spell_index, 0);
                logger->logInfo("INTERFERENCE WAS REFRESHED");
            }
        }

        if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, source_index, SHELTER_LINE))
        // the SHELTER spell already exists on the target
        // it must be cleared, because the Interference takes the precedence
        {
            uint16_t pruned_spell_index = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox, source_index, SHELTER_LINE, 0);
            spellAPI->removeDLLNode(_this, pruned_spell_index);
            spellAPI->setEffectDone(_this, pruned_spell_index, 0);
            logger->logInfo("SHELTER WAS OVERLAPPED WITH INTERFERENCE");
        }

        if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, source_index, PATRONIZE_LINE))
        // the PATRONIZE spell already exists on the target
        // it must be cleared, because the Interference has the precedence over it
        {
            uint16_t pruned_spell_index = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox, spell->target.entity_index, PATRONIZE_LINE, 0);
            ClearPatronizeInArea(_this, pruned_spell_index);
            logger->logInfo("PATRONIZE WAS OVERLAPPED WITH INTERFERENCE");
        }

        // as you might notice, the Interference spell could never return 0
        // it would remove vanilla spells from target, or the old instance of itself either
        // but there are no conditions which might make the new instance of the Interference inapplicable
        // this scenario of the refresh handler will always return 1
        return 1;
    }
    else if (scenario == 2)
    // we run checks for patronize spell
    {
        if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, spell->target.entity_index, INTERFERENCE_LINE))
        // the INTERFERENCE spell already exists on the target
        // it blocks the Patronize spell from being applied
        {
            logger->logInfo("CAN'T CAST PATRONIZE ON TARGET AFFECTED BY INTERFERENCE");
            return 0;
        }

        if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, spell->target.entity_index, SHELTER_LINE))
        // the SHELTER spell already exists on the target
        // it blocks the Patronize spell from being applied
        {
            logger->logInfo("CAN'T CAST PATRONIZE ON TARGET AFFECTED BY SHELTER");
            return 0;
        }

        if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, spell->target.entity_index, PATRONIZE_LINE))
        // the PATRONIZE spell already exists on the target
        // we must remove the previous instance of the Patronize from the spellcaster and from the targets around it
        {
            uint16_t pruned_spell_index = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox, spell->target.entity_index, PATRONIZE_LINE, spell_index);
            if (spell_index != pruned_spell_index && pruned_spell_index != 0)
                // the Patronize was detected
                // let's check whether we remove the spell from the source or from the secondary target
                if (source_index == spell->target.entity_index)
                    // we update the spell over the source, and hence remove all secondary instances of this spell from other targets
                    {
                        ClearPatronizeInArea(_this, pruned_spell_index);
                        logger->logInfo("PATRONIZE WAS REFRESHED OVER THE SOURCE");
                    }
                else
                    // we remove the secondary instance of the spell from specific target, but we don't terminate the spell and don't touch other targets
                    {
                        toolboxAPI->removeSpellFromList(_this->SF_CGdFigureToolBox, target_index, spell_index);
                        logger->logInfo("PATRONIZE WAS REFRESHED OVER THE TARGET");
                    }
        }

        // the Patronize wasn't interruped, we return true for 'checkCanApply'
        return 1;
    }
    else if (scenario == 3)
    // we run checks for shelter spell
    {

        if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, source_index, INTERFERENCE_LINE))
        // the INTERFERENCE spell already exists on the target, it blocks the Shelter from being applied
        {
            logger->logInfo("CAN'T CAST SHELTER ON TARGET AFFECTED BY INTERFERENCE");
            return 0;
        }


        if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, source_index, SHELTER_LINE))
        // the SHELTER spell already exists on the target
        // we must remove the previous instance in order to refresh the spell
        {

            uint16_t pruned_spell_index = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox, source_index, SHELTER_LINE, spell_index);

            if (spell_index != pruned_spell_index && pruned_spell_index != 0)
                {
                    spellAPI->removeDLLNode(_this, pruned_spell_index);
                    spellAPI->setEffectDone(_this, pruned_spell_index, 0);
                    logger->logInfo("SHELTER WAS REFRESHED");
                }
        }

        if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, source_index, PATRONIZE_LINE))
        // the PATRONIZE spell already exists on the target
        // it must be cleared, because the Shelter has the precedence over it
        {
            uint16_t pruned_spell_index = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox, source_index, PATRONIZE_LINE, 0);
            ClearPatronizeInArea(_this, pruned_spell_index);
            logger->logInfo("PATRONIZE WAS OVERLAPPED WITH SHELTER");
        }

        // nothing interrupted the new instance of the SHELTER from being applied
        // we return 1 for 'checkCanApply'
        return 1;
    }

    return 1; // adding this line for safety, most likely it won't be ever triggered
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
    logger = sfsf->logAPI;

    // we register handlers for the new custom spell
    // in this example we introduce new handler - deal damage handler
    // this handler will be called to implement game logic for situations when we cast spell on a unit which is already affected by this spell
    SFSpell *interference_spell = registrationAPI->registerSpell(INTERFERENCE_LINE);
    registrationAPI->linkTypeHandler(interference_spell, &interference_type_handler);
    registrationAPI->linkEffectHandler(interference_spell, INTERFERENCE_JOB, &interference_effect_handler);
    registrationAPI->linkRefreshHandler(interference_spell, &interference_patronize_shelter_refresh_handler);
    registrationAPI->linkDealDamageHandler(interference_spell, &interference_deal_damage_handler, SpellDamagePhase::PRE);
    // SpellDamagePhase controls when damage is registered
    // there are three possible phases: PRE, default, post
    // PRE stands for initial damage, not modified by any other spells
    // default stands for damage modified by other spells
    // post stands for the damage in the end of all calculations, after other spells and armor reductions were applied
    registrationAPI->linkEndHandler(interference_spell, &interference_end_handler);

    // the Interference might interfere (pun was not intended) with the effect provided with Shelter or Patronize vanilla spells
    // those spells already have own refresh handler, which can be checked in
    // https://github.com/muddykat-tech/Spellforce-Spell-Framework/blob/master/src/internal/handlers/sf_spellrefresh_handlers.cpp

    // however, this handler won't check for the Interference spell
    // to fix this omission, we must use the custom refresh handler for the interferring vanilla spells
    // because all three spells will adhere to the same logic, we can link the same refresh handler which we linked above
    // we don't have to touch all other handlers like Spell Type, it's enough only to link only the refresh handler
    // it's worth of mentioning that the custom refresh handler will replace the existing vanilla handler, they won't work simultaneously

    SFSpell *patronize_spell = registrationAPI->registerSpell(PATRONIZE_LINE);
    registrationAPI->linkEffectHandler(patronize_spell, PATRONIZE_JOB, &patronize_effect_handler);
    registrationAPI->linkRefreshHandler(patronize_spell, &interference_patronize_shelter_refresh_handler);

    SFSpell *shelter_spell = registrationAPI->registerSpell(SHELTER_LINE);
    registrationAPI->linkEffectHandler(shelter_spell, SHELTER_JOB, &shelter_effect_handler);
    registrationAPI->linkRefreshHandler(shelter_spell, &interference_patronize_shelter_refresh_handler);
}

/***
 * This function MUST be present in your code with the exact declaration
 * otherwise the framework is unable to describe your mod in logs and mod info menu (menu not yet implemented)
 ***/
extern "C" __declspec(dllexport) SFMod *RegisterMod(SpellforceSpellFramework *framework)
{
    return framework->createModInfo("Interference mod", "1.0.0", "S'Baad", "This mod provides an example of an Interference which negates 20% of incoming damage from all spells.");
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
