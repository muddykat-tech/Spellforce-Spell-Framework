#include "../../src/api/sfsf.h"
#include <windows.h>
#include <vector>
#include <algorithm>
// we'd need vector and algorithm in order to designate AI priorities for Patronize vanilla spell

// We declare macros for Spell Type and Spell Job of both spells
// it is very unhandy to keep them in mind, so we just declare them here
// we use a new set of numbers to avoid interfering with previous examples

// New Spell Type will be 0xf5 (245), it must be defined within GameData.cff
// and provided with at least one spell corresponding each Spell Type
// new Spell Job will be 0xac (172) to avoid interfering with numbers used in previous examples

#define INTERFERENCE_LINE 0xf5
#define INTERFERENCE_JOB 0xac

SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;
IteratorFunctions *iteratorAPI;
RegistrationFunctions *registrationAPI;
SFLog *logger;


void __thiscall interference_type_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    _this->active_spell_list[spell_index].spell_job = INTERFERENCE_JOB;
    spellAPI->setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}


void __thiscall interference_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    spellAPI->figTryClrCHkSPlBfrJob2(_this, spell_index);
    spellAPI->figClrChkSplBfrChkBattle(_this, spell_index, 0);
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
                                                     uint16_t current_damage, uint16_t is_spell_damage,
                                                     uint32_t is_ranged_damage, uint16_t spell_id)
{

    // _toolbox stands for the unit which is affected with our spell and got damaged with any means
    // source specifies the figure_index of a damaging figure
    // target specifies the figure_index of a damaged figure
    // current_damage specifies the amount of damage
    // is_spell_damage and is_ranged_damage specify the damage origin
    // spell_id returns an ID of the spell which triggered the Damage handler
    
    if (is_spell_damage)
    {
        SF_CGdResourceSpell spell_data;
        spellAPI->getResourceSpellData(_toolbox->CGdResource, &spell_data, spell_id);
        // we decrease damage by the percentage stated in spell parameters and also make sure it doesn't get negative
        uint16_t reduction = (current_damage * spell_data.params[1]) / 100;
        (reduction > current_damage) ? (current_damage = 0) : (current_damage -= reduction);
        // we return the modified damage value and game engine uses the modified value as usual
        return current_damage;
    }

    // the damage wasn't caused with another spell, we return damage unchanged
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
    // we increase spell tick by 1 and record the new value
    // hence spell tick will be 1 at the start
    uint32_t current_tick = spellAPI->addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1);
    if (current_tick == 1)
    // spell start
    {
        // we make the game launch refresh handler for the spell
        // the refresh handler will check if another instance of the same spell exists over the target, and will decide whether new spell can be applied
        if (spellAPI->checkCanApply(_this, spell_index))
        {
            // we load the spell from GameData.cff 
            SF_CGdResourceSpell spell_data;
            spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);

            SF_CGdTargetData relative_data;
            relative_data.position = {0, 0};
            relative_data.entity_type = 1;
            relative_data.entity_index = source_index;
            uint32_t unused;

            SF_Rectangle aux_data;
            aux_data.partA = 0;
            aux_data.partB = 0;

            // we apply the visual effect filling the area which we specified above
            spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitTarget, &unused, &relative_data,
                                      _this->OpaqueClass->current_step, 0x5, &aux_data);
            // the parameter before the last stands for spell area, effects affecting single target usually have this parameter set to 0
            // but if we want to make the effect look cool, we could put the number above 0 and it will look like an AoE spell on activation

            
            // we have to activate the flag F_CHECK_SPELLS_BEFORE_JOB in order to make it possible for Deal Damage handler to trigger when the damage is received
            _this->SF_CGdFigure->figures[source_index].flags |= F_CHECK_SPELLS_BEFORE_JOB;
            // we activate the flag F_CHECK_SPELLS_BEFORE_BATTLE for the sake of optimization
            _this->active_spell_list[spell_index].flags |= SpellFlagKey::CHECK_SPELLS_BEFORE_JOB2;
            
            // we get spell duration from game data
            uint32_t ticks_interval = spell_data.params[0];
            // we disable the spell from being triggered for a specified number of internal game ticks, and after a specified in ticks_interval amount of time has passed, we can remove the spell
            _this->active_spell_list[spell_index].to_do_count = (uint16_t)((ticks_interval * 10) / 1000);
            return;
        }
    }

<<<<<<< HEAD
=======
    // spell end, main spell logic is implemented with another handler, so we have only to clear flag which allowed triggering damage handler and stop the spell in this block
    // we should clear the flags we raised during the tick 1 for the sake of optimization
>>>>>>> 0eb79b3b663124ead8ee4e40c289baf96b71f3c6
    spellAPI->figTryClrCHkSPlBfrJob2(_this, spell_index);
    spellAPI->figClrChkSplBfrChkBattle(_this, spell_index, 0);
    spellAPI->setEffectDone(_this, spell_index, 0);
}


<<<<<<< HEAD
=======
// removing the Patronize requires running a loop to clear the spell from all targets affected
// because we remove the Patronize in different handlers (Spell Effect, Spell Refresh)
// it's more convenient to pack it in a function and call this function whenever we want to remove the Patronize
>>>>>>> 0eb79b3b663124ead8ee4e40c289baf96b71f3c6

void ClearPatronizeInArea(SF_CGdSpell *_this, uint16_t spell_index)
{
    //indices start from 1(!)
    for (uint16_t target_index = 1; target_index <= _this->SF_CGdFigure->max_used; target_index++)
<<<<<<< HEAD
    {
        if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, kGdSpellLineAbilityPatronize) &&
            (figureAPI->getSpellJobStartNode(_this->SF_CGdFigure, target_index) != 0))
=======
    // we run a loop through all figures in game and check whether they're affected with the Patronize
    // we can't use an iterator, because affected figures might move outside a specific radius around the spellcaster
    {
        if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, kGdSpellLineAbilityPatronize) &&
            (figureAPI->getSpellJobStartNode(_this->SF_CGdFigure, target_index) != 0))
        // we check whether the target has Patronize on them
        // we check whether this spell is actually present in the list of spells affecting the creature

        // however, we will remove only the specific instance of Patronize which we know by spell_index argument
        // if the creature was under the Patronize from another source, that Patronize won't be removed            
>>>>>>> 0eb79b3b663124ead8ee4e40c289baf96b71f3c6
        {
            toolboxAPI->removeSpellFromList(_this->SF_CGdFigureToolBox, target_index, spell_index);
            spellAPI->tryClearCheckSpellsBeforeJob(_this, spell_index, target_index);
        }
    }
<<<<<<< HEAD
}

typedef struct
{
    uint16_t figure_id;
    uint16_t prio;
} patronize_entry;

uint16_t calculatePrio(SF_CGdSpell *_this, uint16_t figure_id)
{
=======
}


// we declare structure which will be used to govern targets priroty for Patronize
typedef struct
{
    uint16_t figure_id;
    uint16_t prio;
} patronize_entry;

// we calculate targets priority for Patronize spell
// this function will be run for every figure caught in area of Patronize spell
// this function will check how many enemies are around the figure in check, and move it in priority list respectively
uint16_t calculatePrio(SF_CGdSpell *_this, uint16_t figure_id)
{
    // we obtain figure's coordinates
    SF_Coord target_pos = _this->SF_CGdFigure->figures[figure_id].position;
    uint16_t dx = 7;
    uint16_t sec_prio = 0;
    // in game engine every tile is surrouned by 8 tiles, we have to check for all of them
    for (int i = 8; i> 0; i--)
    {
        //some serious bitfuckery
        uint16_t near_x = *(uint16_t *)((uint32_t)&_this->SF_CGdWorld->unknown1[0].uknwn1 + dx) + target_pos.X;
        uint16_t near_y = *(uint16_t *)((uint32_t)&_this->SF_CGdWorld->unknown1[0].uknwn2 + dx) + target_pos.Y;
        // we check whether the terrain tile is passable, otherwise no sense in checking it
        if (((*(uint8_t *)&_this->SF_CGdWorld->cells[near_y*0x400 + near_x].world_cell_flags) & 0x10) != 0)
        {
            // we get the target index of a figure occupying the surrounding tile
            uint16_t sec_target = toolboxAPI->getFigureFromWorld(_this->SF_CGdWorldToolBox, near_x, near_y, 1);

            // if there is a figure in a tile, we check whether the figure is hostile
            if ((sec_target != 0)
                && (toolboxAPI->figuresCheckHostile(_this->SF_CGdFigureToolBox, figure_id, sec_target)))
            {
                // we increase the priority by 1
                sec_prio++;
            }
        }
        dx += 7;
    }
    // if the friendly figure is melee, it needs Patronize more than ranged units, we double it's priority and add 1 to it
    if (toolboxAPI->isUnitMelee(_this->SF_CGdFigureToolBox, figure_id))
    {
        sec_prio *= 2;
        sec_prio +=1;
    }
    // we return total priroty for the figure
    return sec_prio;
}


// the custom implementation of vanilla spell effect handler for the Patronize
// it's necessary in order to make the custom refresh handler work
// otherwise custom refresh handler gets ignored due to a bug in vanilla code
>>>>>>> 0eb79b3b663124ead8ee4e40c289baf96b71f3c6

    SF_Coord target_pos = _this->SF_CGdFigure->figures[figure_id].position;
    uint16_t dx = 7;
    uint16_t sec_prio = 0;
    for (int i = 8; i> 0; i--)
    {
        //some serious bitfuckery
        uint16_t near_x = *(uint16_t *)((uint32_t)&_this->SF_CGdWorld->unknown1[0].uknwn1 + dx) + target_pos.X;
        uint16_t near_y = *(uint16_t *)((uint32_t)&_this->SF_CGdWorld->unknown1[0].uknwn2 + dx) + target_pos.Y;
        if (((*(uint8_t *)&_this->SF_CGdWorld->cells[near_y*0x400 + near_x].world_cell_flags) & 0x10) != 0)
        {
            uint16_t sec_target = toolboxAPI->getFigureFromWorld(_this->SF_CGdWorldToolBox, near_x, near_y, 1);

            if ((sec_target != 0)
                && (toolboxAPI->figuresCheckHostile(_this->SF_CGdFigureToolBox, figure_id, sec_target)))
            {
                sec_prio++;
            }
        }
        dx += 7;
    }
    if (toolboxAPI->isUnitMelee(_this->SF_CGdFigureToolBox, figure_id))
    {
        sec_prio *= 2;
        sec_prio +=1;
    }
    return sec_prio;
}

void __thiscall patronize_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
<<<<<<< HEAD
    uint16_t source_index = _this->active_spell_list[spell_index].source.entity_index;
=======
    // we store index of the spellcaster, we won't need target index for this spell
    uint16_t source_index = _this->active_spell_list[spell_index].source.entity_index;
    // we increase the current tick by 1 and get the new value
>>>>>>> 0eb79b3b663124ead8ee4e40c289baf96b71f3c6
    uint32_t current_tick = spellAPI->addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1);

    if (current_tick == 1)
    {
        if (spellAPI->checkCanApply(_this, spell_index))
        {
<<<<<<< HEAD
            SF_CGdResourceSpell spell_data;
            spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);

=======
            // we load spell data
            SF_CGdResourceSpell spell_data;
            spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);

            // we pull spell parameters from GameData.cff
>>>>>>> 0eb79b3b663124ead8ee4e40c289baf96b71f3c6
            uint32_t ticks_interval = spell_data.params[0];
            uint16_t effect_radius = spell_data.params[2];
            uint16_t figure_count = spell_data.params[3];

            spell->flags |= SpellFlagKey::CHECK_SPELLS_BEFORE_JOB2;
            _this->SF_CGdFigure->figures[source_index].flags |= F_CHECK_SPELLS_BEFORE_JOB;
            // we make the spell last a specified amount of in-game ticks (not the same as spell ticks, crudely saying it's something akin to fps)
            _this->active_spell_list[spell_index].to_do_count = (uint16_t)((ticks_interval * 10) / 1000);

            // we get XY coordinates of the spell center with API function
            // basically, we just center the spell around its spellcaster (source)
            SF_Coord cast_center;
            figureAPI->getPosition(_this->SF_CGdFigure, &cast_center, source_index);

            // we declare structures to store relative position of visual effect
            SF_CGdTargetData relative_data;
            relative_data.position = {0, 0};
            relative_data.entity_type = 1;
            relative_data.entity_index = source_index;
            uint32_t unused;

            SF_Rectangle hit_area;

            spellAPI->getTargetsRectangle(_this, &hit_area, spell_index, spell_data.params[0], &cast_center);

            spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitWorld, &unused, &relative_data,
                                      _this->OpaqueClass->current_step, 0x25, &hit_area);

            hit_area.partA = 0;
            hit_area.partB = 0;
            spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitTarget, &unused, &relative_data,
                                      _this->OpaqueClass->current_step, 0x96, &hit_area);

            CGdFigureIterator figure_iterator;
            iteratorAPI->setupFigureIterator(&figure_iterator, _this);
            // we set iterator area as a circle of specific radius (defined in spell data) with the spellcaster being at its center
            iteratorAPI->iteratorSetArea(&figure_iterator, &cast_center, effect_radius);

            // let's use a vector to store potential targets and their priorities
            std::vector<patronize_entry> targets;

            // let's check all targets in iterator area 
            for (uint16_t target_index = iteratorAPI->getNextFigure(&figure_iterator); target_index != 0;
                 target_index = iteratorAPI->getNextFigure(&figure_iterator))
            {
                if (((int16_t)(_this->SF_CGdFigure->figures[target_index].owner) ==
                     (int16_t)(_this->SF_CGdFigure->figures[source_index].owner)) &&
                    (((uint8_t)(_this->SF_CGdFigure->figures[target_index].flags) & (IS_DEAD | RESESRVED_ONLY)) == 0) &&
                    (toolboxAPI->isTargetable(_this->SF_CGdFigureToolBox, target_index)))
                {
                    spell->target.entity_index = target_index;

                    // we check whether the target doesn't have Patronize active already
                    if (spellAPI->checkCanApply(_this, spell_index) && (source_index != target_index))
                    {

                        // we declare temporary entity to store figure index and its priority value before adding the entity to the vector
                        patronize_entry entry;
                        entry.figure_id = target_index;
                        // we calculate priority for the target
                        entry.prio = calculatePrio(_this, target_index);
                        // we record the target within the vector
                        targets.push_back(entry);
                    }
                }
            }

            // we dispose iterator after all targets in area were evaluated
            iteratorAPI->disposeFigureIterator(&figure_iterator);

            // we sort the vector from highest priority to lowest
            std::sort(targets.begin(), targets.end(), [](const patronize_entry& a, const patronize_entry& b)
            {
                return a.prio > b.prio;
            });

            // we apply spell to the targets with highest priority until there are no more valid targets or we've exceeded figures limit defined by spell parameters
            for (int i = 0; ((i < targets.size()) && (figure_count > 0)); i++)
            {
                // we apply visual effect to every target
                SF_CGdTargetData relative_data;
                relative_data.position = {0, 0};
                relative_data.entity_type = 1;         // 1 stands for individual figure
                relative_data.entity_index = targets.at(i).figure_id;         //
                uint32_t unused;

                SF_Rectangle aux_data;
                aux_data.partA = 0;
                aux_data.partB = 0;

                spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitTarget, &unused, &relative_data,
                                          _this->OpaqueClass->current_step, 0x96, &aux_data);
                _this->SF_CGdFigure->figures[source_index].flags |= F_CHECK_SPELLS_BEFORE_JOB;

                // we add a spell to each figure
                toolboxAPI->addSpellToFigure(_this->SF_CGdFigureToolBox,  targets.at(i).figure_id, spell_index);
                figure_count--;
            }

        }
        logger->logDebug(DEBUG_HIGH, "Patronize activated");
        return;
    }
    logger->logDebug(DEBUG_HIGH, "Patronize done");
    // when there is a new tick, we clear spell from all targets affected
    // we do it with a function implemented above 
    ClearPatronizeInArea(_this, spell_index);
    // after that we stop the spell
    spellAPI->setEffectDone(_this, spell_index, 0);
}


// we also have to manually re-implenet Shelter Effect handler in order to make it use custom refresh handler
void __thiscall shelter_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    uint16_t source_index = _this->active_spell_list[spell_index].source.entity_index;
    uint32_t current_tick = spellAPI->addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1);
    if (current_tick == 1)
    {
        if (spellAPI->checkCanApply(_this, spell_index))
        {
            SF_CGdResourceSpell spell_data;
            spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);


            SF_CGdTargetData relative_data;
            relative_data.position = {0, 0};
            relative_data.entity_type = 1;
            relative_data.entity_index = source_index;

            uint32_t unused;

            SF_Rectangle aux_data;
            aux_data.partA = 0;
            aux_data.partB = 0;

            spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitTarget, &unused, &relative_data,
                                      _this->OpaqueClass->current_step, 0x96, &aux_data);
            uint32_t ticks_interval = spell_data.params[0];

            spell->to_do_count = (uint16_t)((ticks_interval * 10) / 1000);
            spell->flags |= SpellFlagKey::CHECK_SPELLS_BEFORE_JOB2;
            _this->SF_CGdFigure->figures[source_index].flags |= F_CHECK_SPELLS_BEFORE_JOB;
            logger->logDebug(DEBUG_HIGH, "SHELTER ACTIVATED");
        }
        else
        {
            // the Shelter can't be applied to the spellcaster being under the Interference
            // let's finish the spell to avoid it going for the tick 1
            spellAPI->setEffectDone(_this, spell_index, 0); // we end a spell
            logger->logDebug(DEBUG_HIGH, "SHELTER ACTIVATED");
        }
        return;
    }
    if (figureAPI->isAlive(_this->SF_CGdFigure, source_index))
    {
        spellAPI->figTryClrCHkSPlBfrJob2(_this, spell_index);
    }
    spellAPI->setEffectDone(_this, spell_index, 0);     // we end a spell
    logger->logInfo("SHELTER FINISHED");
}


// this is the common refresh handler for all 3 spells: Interference, Patronize, Shelter
// the elder spell removes lesser spells when applied
// the elder spells blocks lesser spells from being applied (e. g. target can't benefit from shelter if it's having interference currently)
// the new instance of the same spell removes the older instance and refreshes spell duration
// we use the same handler with switch-case construction because it's easier than making separate handlers with virtually the same logic
int __thiscall interference_patronize_shelter_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];

    uint16_t source_index = spell->source.entity_index;

    // we check which spell triggered the refresh handler
    switch (spell->spell_job)
    {
        // Interference logic
        case INTERFERENCE_JOB:
        {
            // if target already has Interference effect active
            if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, source_index, INTERFERENCE_LINE))
            {

                // we get the spell index of Interference spell by its Spell Type 
                // the function returns the first spell index of a spell with given Spell Type except from the specific spell index which we might pass to function
                // this spell index is passed as last argument in getSpellIndexOfType() 
                uint16_t pruned_spell_index = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox, source_index,
                                                                              INTERFERENCE_LINE, spell_index);
                if (spell_index != pruned_spell_index && pruned_spell_index != 0)
                {
                    // we remove the older instance of a spell
                    spellAPI->removeDLLNode(_this, pruned_spell_index);
                    spellAPI->setEffectDone(_this, pruned_spell_index, 0);
                    logger->logDebug(DEBUG_HIGH, "INTERFERENCE WAS REFRESHED");
                }
            }

            // if there is a Shelter effect active, we get its spell index and clear it
            if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, source_index, kGdSpellLineAbilityShelter))
            {
                uint16_t pruned_spell_index = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox, source_index,
                                                                              kGdSpellLineAbilityShelter, 0);
                spellAPI->removeDLLNode(_this, pruned_spell_index);
                spellAPI->setEffectDone(_this, pruned_spell_index, 0);
                logger->logDebug(DEBUG_HIGH, "SHELTER WAS OVERLAPPED WITH INTERFERENCE");
            }

            // if there is a Patronize effect active, we get its spell index and clear it
            if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, source_index, kGdSpellLineAbilityPatronize))
            {
                uint16_t pruned_spell_index = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox,
                                                                              source_index,
                                                                              kGdSpellLineAbilityPatronize, 0);
                toolboxAPI->removeSpellFromList(_this->SF_CGdFigureToolBox, source_index, pruned_spell_index);
                logger->logDebug(DEBUG_HIGH, "PATRONIZE WAS OVERLAPPED WITH INTERFERENCE");
            }
            // we return 1 to stop the refresh handler working and to allow to apply other logic of Spell Effect handler to a target
            return 1;
        }

        //Patronize logic
        case kGdSpellJobAbilityPatronize:
        {
            // Inteference effect active
            if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, spell->target.entity_index, INTERFERENCE_LINE))

            {
                // we forbid applying Patronize to a target, because there is an Interference active already
                // spellAPI->checkCanApply() returns false, and Spell Effect handler acts accordingly
                logger->logDebug(DEBUG_HIGH, "CAN'T CAST PATRONIZE ON TARGET AFFECTED BY INTERFERENCE");
                return 0;
            }

            // Shelter effect active
            if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, spell->target.entity_index,
                                         kGdSpellLineAbilityShelter))
            {
                // we forbid applying Patronize to a target, because there is a Shelter active already
                // spellAPI->checkCanApply() returns false, and Spell Effect handler acts accordingly
                logger->logDebug(DEBUG_HIGH, "CAN'T CAST PATRONIZE ON TARGET AFFECTED BY SHELTER");
                return 0;
            }

            // Another Patronize effect active
            if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, spell->target.entity_index,
                                         kGdSpellLineAbilityPatronize))
            {
                // we get spell index of an older Patronize instance    
                uint16_t pruned_spell_index = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox,
                                                                              spell->target.entity_index,
                                                                              kGdSpellLineAbilityPatronize,
                                                                              spell_index);
                // we stop older Patronize effect
                if (spell_index != pruned_spell_index && pruned_spell_index != 0)
                {
                    if (source_index == spell->target.entity_index)
                    {
                        // if we're making Refresh Handler check for the original spellcaster, we also trigger function to remove Patronize effect from all its targets
                        ClearPatronizeInArea(_this, pruned_spell_index);
                        logger->logInfo("PATRONIZE WAS REFRESHED OVER THE SOURCE");
                    }
                    else
                    {
                        // if we're making Refresh Handler check for the figure being affected by someone's other spell, we simply remove older instance of Patronize from it
                        toolboxAPI->removeSpellFromList(_this->SF_CGdFigureToolBox, spell->target.entity_index,
                                                        spell_index);
                        logger->logDebug(DEBUG_HIGH, "PATRONIZE WAS REFRESHED OVER THE TARGET");
                    }
                }
            }
            // we return true to allow applying new Patronize effect to a target
            return 1;
        }

        // Shelter logic
        case kGdSpellJobAbilityShelter:
        {
            // Interference effect active
            if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, source_index, INTERFERENCE_LINE))
            {
                // we forbid applying Shelter to a target, because there is an Interference active already
                // spellAPI->checkCanApply() returns false, and Spell Effect handler acts accordingly
                logger->logDebug(DEBUG_HIGH, "CAN'T CAST SHELTER ON TARGET AFFECTED BY INTERFERENCE");
                return 0;
            }

            // Shelter effect active
            if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, source_index, kGdSpellLineAbilityShelter))
            {
                // we remove older instance of Shelter
                uint16_t pruned_spell_index = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox, source_index,
                                                                              kGdSpellLineAbilityShelter, spell_index);

                if (spell_index != pruned_spell_index && pruned_spell_index != 0)
                {
                    spellAPI->removeDLLNode(_this, pruned_spell_index);
                    spellAPI->setEffectDone(_this, pruned_spell_index, 0);
                    logger->logDebug(DEBUG_HIGH, "SHELTER WAS REFRESHED");
                }
            }

            // Patronize effect active
            if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, source_index, kGdSpellLineAbilityPatronize))
            {
                // we remove Patronize from target, because it's overlapped with Shelter
                uint16_t pruned_spell_index = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox, source_index,
                                                                              kGdSpellLineAbilityPatronize, 0);
                // we also remove Patronize from all targets its might been affecting
                ClearPatronizeInArea(_this, pruned_spell_index);
                logger->logDebug(DEBUG_HIGH, "PATRONIZE WAS OVERLAPPED WITH SHELTER");
            }
            // we return true allowing Shelter to be applied to a target
            return 1;
        }

        // just in case we didn't get any of options above, we break switch-case cycle
        default:
        {
            break;
        }
    }

    return 1; // adding this line for safety, most likely it won't be ever triggered
}

// adding AI handler to interference to make units actively use the spell
// AI handlers will be explained in more details in later chapters
uint32_t __thiscall interference_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line,
                                            SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    if ((_this->battleData.current_figure != target_index) ||
        ((_this->battleData.enemy_figures.entityCount == 0) &&
         (_this->battleData.building_not_ally.entityCount == 0)))
    {
        rank = 0;
    }
    return rank;
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
    registrationAPI->linkDealDamageHandler(interference_spell, &interference_deal_damage_handler,
                                           SpellDamagePhase::PRE);
    registrationAPI->linkSingleTargetAIHandler(interference_spell, &interference_ai_handler);
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

    SFSpell *patronize_spell = registrationAPI->registerSpell(kGdSpellLineAbilityPatronize);
    registrationAPI->linkEffectHandler(patronize_spell, kGdSpellJobAbilityPatronize, &patronize_effect_handler);
    registrationAPI->linkRefreshHandler(patronize_spell, &interference_patronize_shelter_refresh_handler);

    SFSpell *shelter_spell = registrationAPI->registerSpell(kGdSpellLineAbilityShelter);
    registrationAPI->linkEffectHandler(shelter_spell, kGdSpellJobAbilityShelter, &shelter_effect_handler);
    registrationAPI->linkRefreshHandler(shelter_spell, &interference_patronize_shelter_refresh_handler);
}

/***
 * This function MUST be present in your code with the exact declaration
 * otherwise the framework is unable to describe your mod in logs and mod info menu (menu not yet implemented)
 ***/
extern "C" __declspec(dllexport) SFMod *RegisterMod(SpellforceSpellFramework *framework)
{
    return framework->createModInfo("Interference mod", "1.2.0", "S'Baad, Un'Schtalch",
                                    "This mod provides an example of an Interference which negates 20% of incoming damage from all spells.");
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
