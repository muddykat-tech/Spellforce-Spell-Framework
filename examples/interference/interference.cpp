#include "../../src/api/sfsf.h"
#include <windows.h>
#include <vector>
#include <algorithm>
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


uint16_t __thiscall interference_deal_damage_handler(SF_CGdFigureToolbox *_toolbox, uint16_t source, uint16_t target,
                                                     uint16_t current_damage, uint16_t is_spell_damage,
                                                     uint32_t is_ranged_damage, uint16_t spell_id)
{
    if (is_spell_damage)
    {
        SF_CGdResourceSpell spell_data;
        spellAPI->getResourceSpellData(_toolbox->CGdResource, &spell_data, spell_id);
        uint16_t reduction = (current_damage * spell_data.params[1]) / 100;
        (reduction > current_damage) ? (current_damage = 0) : (current_damage -= reduction);
        return current_damage;
    }

    return current_damage;
}

void __thiscall interference_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{

    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    uint16_t source_index = _this->active_spell_list[spell_index].source.entity_index;
    uint32_t current_tick = spellAPI->addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1);
    if (current_tick == 1)
    // spell start
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
                                      _this->OpaqueClass->current_step, 0x5, &aux_data);


            _this->SF_CGdFigure->figures[source_index].flags |= F_CHECK_SPELLS_BEFORE_JOB;
            _this->active_spell_list[spell_index].flags |= SpellFlagKey::CHECK_SPELLS_BEFORE_JOB2;
            uint32_t ticks_interval = spell_data.params[0];
            _this->active_spell_list[spell_index].to_do_count = (uint16_t)((ticks_interval * 10) / 1000);
            return;
        }
    }

    spellAPI->figTryClrCHkSPlBfrJob2(_this, spell_index);
    spellAPI->figClrChkSplBfrChkBattle(_this, spell_index, 0);
    spellAPI->setEffectDone(_this, spell_index, 0);
}



void ClearPatronizeInArea(SF_CGdSpell *_this, uint16_t spell_index)
{
    //indices start from 1(!)
    for (uint16_t target_index = 1; target_index <= _this->SF_CGdFigure->max_used; target_index++)
    {
        if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, kGdSpellLineAbilityPatronize) &&
            (figureAPI->getSpellJobStartNode(_this->SF_CGdFigure, target_index) != 0))
        {
            toolboxAPI->removeSpellFromList(_this->SF_CGdFigureToolBox, target_index, spell_index);
            spellAPI->tryClearCheckSpellsBeforeJob(_this, spell_index, target_index);
        }
    }
}

typedef struct
{
    uint16_t figure_id;
    uint16_t prio;
} patronize_entry;

uint16_t calculatePrio(SF_CGdSpell *_this, uint16_t figure_id)
{

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
    uint16_t source_index = _this->active_spell_list[spell_index].source.entity_index;
    uint32_t current_tick = spellAPI->addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1);

    if (current_tick == 1)
    {
        if (spellAPI->checkCanApply(_this, spell_index))
        {
            SF_CGdResourceSpell spell_data;
            spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);

            uint32_t ticks_interval = spell_data.params[0];
            uint16_t effect_radius = spell_data.params[2];
            uint16_t figure_count = spell_data.params[3];

            spell->flags |= SpellFlagKey::CHECK_SPELLS_BEFORE_JOB2;
            _this->SF_CGdFigure->figures[source_index].flags |= F_CHECK_SPELLS_BEFORE_JOB;
            _this->active_spell_list[spell_index].to_do_count = (uint16_t)((ticks_interval * 10) / 1000);

            SF_Coord cast_center;
            figureAPI->getPosition(_this->SF_CGdFigure, &cast_center, source_index);

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
            iteratorAPI->iteratorSetArea(&figure_iterator, &cast_center, effect_radius);

            std::vector<patronize_entry> targets;

            // let's do the iterator loop until we exhaust spell uses or there are no valid targets in radius anymore
            for (uint16_t target_index = iteratorAPI->getNextFigure(&figure_iterator); target_index != 0;
                 target_index = iteratorAPI->getNextFigure(&figure_iterator))
            {
                if (((int16_t)(_this->SF_CGdFigure->figures[target_index].owner) ==
                     (int16_t)(_this->SF_CGdFigure->figures[source_index].owner)) &&
                    (((uint8_t)(_this->SF_CGdFigure->figures[target_index].flags) & (IS_DEAD | RESESRVED_ONLY)) == 0) &&
                    (toolboxAPI->isTargetable(_this->SF_CGdFigureToolBox, target_index)))
                {
                    spell->target.entity_index = target_index;

                    if (spellAPI->checkCanApply(_this, spell_index) && (source_index != target_index))
                    {

                        patronize_entry entry;
                        entry.figure_id = target_index;
                        entry.prio = calculatePrio(_this, target_index);
                        targets.push_back(entry);
                    }
                }
            }
            iteratorAPI->disposeFigureIterator(&figure_iterator);

            std::sort(targets.begin(), targets.end(), [](const patronize_entry& a, const patronize_entry& b)
            {
                return a.prio > b.prio;
            });

            for (int i = 0; ((i < targets.size()) && (figure_count > 0)); i++)
            {
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

                toolboxAPI->addSpellToFigure(_this->SF_CGdFigureToolBox,  targets.at(i).figure_id, spell_index);
                figure_count--;
            }

        }
        logger->logDebug(DEBUG_HIGH, "Patronize activated");
        return;
    }
    logger->logDebug(DEBUG_HIGH, "Patronize done");
    ClearPatronizeInArea(_this, spell_index);
    spellAPI->setEffectDone(_this, spell_index, 0);
}

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

int __thiscall interference_patronize_shelter_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];

    uint16_t source_index = spell->source.entity_index;
    uint8_t scenario = 0;

    switch (spell->spell_job)
    {
        case INTERFERENCE_JOB:
        {
            if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, source_index, INTERFERENCE_LINE))
            {

                uint16_t pruned_spell_index = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox, source_index,
                                                                              INTERFERENCE_LINE, spell_index);
                if (spell_index != pruned_spell_index && pruned_spell_index != 0)
                {
                    spellAPI->removeDLLNode(_this, pruned_spell_index);
                    spellAPI->setEffectDone(_this, pruned_spell_index, 0);
                    logger->logDebug(DEBUG_HIGH, "INTERFERENCE WAS REFRESHED");
                }
            }

            if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, source_index, kGdSpellLineAbilityShelter))
            {
                uint16_t pruned_spell_index = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox, source_index,
                                                                              kGdSpellLineAbilityShelter, 0);
                spellAPI->removeDLLNode(_this, pruned_spell_index);
                spellAPI->setEffectDone(_this, pruned_spell_index, 0);
                logger->logDebug(DEBUG_HIGH, "SHELTER WAS OVERLAPPED WITH INTERFERENCE");
            }

            if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, source_index, kGdSpellLineAbilityPatronize))
            {
                uint16_t pruned_spell_index = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox,
                                                                              source_index,
                                                                              kGdSpellLineAbilityPatronize, 0);
                toolboxAPI->removeSpellFromList(_this->SF_CGdFigureToolBox, source_index, pruned_spell_index);
                logger->logDebug(DEBUG_HIGH, "PATRONIZE WAS OVERLAPPED WITH INTERFERENCE");
            }
            return 1;
        }
        case kGdSpellJobAbilityPatronize:
        {
            if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, spell->target.entity_index, INTERFERENCE_LINE))

            {
                logger->logDebug(DEBUG_HIGH, "CAN'T CAST PATRONIZE ON TARGET AFFECTED BY INTERFERENCE");
                return 0;
            }

            if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, spell->target.entity_index,
                                         kGdSpellLineAbilityShelter))
            {
                logger->logDebug(DEBUG_HIGH, "CAN'T CAST PATRONIZE ON TARGET AFFECTED BY SHELTER");
                return 0;
            }

            if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, spell->target.entity_index,
                                         kGdSpellLineAbilityPatronize))
            {
                uint16_t pruned_spell_index = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox,
                                                                              spell->target.entity_index,
                                                                              kGdSpellLineAbilityPatronize,
                                                                              spell_index);
                if (spell_index != pruned_spell_index && pruned_spell_index != 0)
                {
                    if (source_index == spell->target.entity_index)
                    {
                        ClearPatronizeInArea(_this, pruned_spell_index);
                        logger->logInfo("PATRONIZE WAS REFRESHED OVER THE SOURCE");
                    }
                    else
                    {
                        toolboxAPI->removeSpellFromList(_this->SF_CGdFigureToolBox, spell->target.entity_index,
                                                        spell_index);
                        logger->logDebug(DEBUG_HIGH, "PATRONIZE WAS REFRESHED OVER THE TARGET");
                    }
                }
            }
            return 1;
        }
        case kGdSpellJobAbilityShelter:
        {
            if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, source_index, INTERFERENCE_LINE))
            {
                logger->logDebug(DEBUG_HIGH, "CAN'T CAST SHELTER ON TARGET AFFECTED BY INTERFERENCE");
                return 0;
            }

            if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, source_index, kGdSpellLineAbilityShelter))
            {

                uint16_t pruned_spell_index = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox, source_index,
                                                                              kGdSpellLineAbilityShelter, spell_index);

                if (spell_index != pruned_spell_index && pruned_spell_index != 0)
                {
                    spellAPI->removeDLLNode(_this, pruned_spell_index);
                    spellAPI->setEffectDone(_this, pruned_spell_index, 0);
                    logger->logDebug(DEBUG_HIGH, "SHELTER WAS REFRESHED");
                }
            }

            if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, source_index, kGdSpellLineAbilityPatronize))
            {
                uint16_t pruned_spell_index = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox, source_index,
                                                                              kGdSpellLineAbilityPatronize, 0);
                ClearPatronizeInArea(_this, pruned_spell_index);
                logger->logDebug(DEBUG_HIGH, "PATRONIZE WAS OVERLAPPED WITH SHELTER");
            }
            return 1;
        }

        default:
        {
            break;
        }
    }

    return 1; // adding this line for safety, most likely it won't be ever triggered
}

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
    return framework->createModInfo("Interference mod", "1.0.0", "S'Baad",
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
