#include "api/sfsf.h"
//#include "../../src/api/sfsf.h"
#include <windows.h>
#include <stdio.h>

// this example shows another way to implement the shieldwall spell
// it implements shieldwall via single aoe spell instead of combination of two spells (AoE and individual)
// such implementation is closer to vanilla implementation of AoE buffs

// We declare macros for Shieldwall Spell Type and Spell Job
// Spell Type 0xf9 = 249, Spell Job 0xb0 = 176

// The custom Spell Type also must be defined within GameData.cff
// and provided with at least one spell corresponding to it

#define THROW_HAMMER_TOWER_LINE 0xf9
#define THROW_HAMMER_TOWER_JOB 0xb0


SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;
IteratorFunctions *iteratorAPI;
RegistrationFunctions *registrationAPI;
AiFunctions *aiAPI;
BuildingFunctions *buildingAPI;
SFLog *logger;

// we declare spell type handler for Shieldwall, it must initialize ticks and bonus modifier values

void __thiscall THROW_HAMMER_TOWER_type_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // we need only to associate spell type with a spell job and nothing more than that
    _this->active_spell_list[spell_index].spell_job = THROW_HAMMER_TOWER_JOB;
}

// we declare spell end handler for Shieldwall
// this handler would work in case a spell wasn't finished correctly
void __thiscall THROW_HAMMER_TOWER_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // since this spell is instant, we have only to get its index
    // and erase it from the list of active spells
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    spellAPI->removeDLLNode(_this, spell_index);
    spellAPI->setEffectDone(_this, spell_index, 0);
}



uint32_t __thiscall THROW_HAMMER_TOWER_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{


    if ((_this->battleData.current_figure == target_index) ||
        (_this->battleData.enemy_figures.entityCount == 0))
    {
        return 0;
    }


    struct {
        uint16_t figure_index = 0;
        uint16_t health = 9999;
           } lowest_hp_figure;


    for (uint16_t i = 0; i < _this->battleData.enemy_figures.entityCount; i++)
    {
        uint32_t min_radius = spell_data->min_range;
        uint32_t max_radius = spell_data->max_range;
        SF_Coord tower_position = _this->battleData.CGdFigure->figures[_this->battleData.current_figure].position;
        SF_Coord target_position = _this->battleData.CGdFigure->figures[target_index].position;
        uint32_t distance = toolboxAPI->getDistance (&tower_position, &target_position);

        if ((lowest_hp_figure.health < _this->battleData.CGdFigure->figures[target_index].health.base_val) && // we seek for the lower health
            (lowest_hp_figure.figure_index != _this->battleData.current_figure) && // we ignore tower's health, because we won't attack ourselves
            (distance >= min_radius) && (distance <= max_radius)) // we ignore all figures which don't fall into spell range
            {
                lowest_hp_figure.figure_index = i;
                lowest_hp_figure.health = _this->battleData.CGdFigure->figures[target_index].health.base_val;
            }

    }

    // if selected unit has the lowest health among targets in spell radius, we throw hammer at it
    if (lowest_hp_figure.figure_index == target_index)
    {
        return 2;
    }
    // if selected unit doesn't have the lowest health among targets in radius, we don't do anything
    else
    {
        return 0;
    }

}


// we declare the effect handler for tower spell

void __thiscall THROW_HAMMER_TOWER_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // we pull the pointer for this instance of spell
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    uint16_t target_index = spell->target.entity_index;
    uint16_t source_index = spell->source.entity_index;

    //grabbing spell and spell effect (logic) id
    SF_SpellEffectInfo effect_info;
    effect_info.spell_id = spell->spell_id;
    effect_info.job_id = spell->spell_job;

    //pulling spell parameters into local structure
    SF_CGdResourceSpell spell_data;
    spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);

    uint16_t isAlive = figureAPI->isAlive(_this->SF_CGdFigure, target_index);
    uint16_t isTargetable =  toolboxAPI->isTargetable(_this->SF_CGdFigureToolBox, target_index);
    uint16_t isHostile = toolboxAPI->figuresCheckHostile(_this->SF_CGdFigureToolBox, source_index, target_index);
    uint16_t isOwner = _this->SF_CGdFigure->figures[target_index].owner;


    // we've got a lot of technical conditions which can prevent spell cast, so if we don't meet at least one of them, spell fails
    if (isAlive != 0 || isTargetable != 0 || isHostile != 0 || isOwner != -1)
     {
        uint32_t resist_chance = spellAPI->getChanceToResistSpell(_this->unkn2, source_index, target_index, effect_info);
        uint16_t random_roll = spellAPI->getRandom(_this->OpaqueClass, 100);

        if (resist_chance >= random_roll) // we compare target spell resistance to random roll, and if resistance was higher, we stop function execution
            {
                uint32_t unused;
                SF_CGdTargetData relative_data;
                relative_data.position.X = 0;
                relative_data.position.Y = 0;
                relative_data.entity_type = 1;
                relative_data.entity_index = target_index;

                SF_Rectangle aux_data;
                aux_data.partA = 0;
                aux_data.partB = 0;
                spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellTargetResisted, &unused, &relative_data, _this->OpaqueClass->current_step, 10, &aux_data);
                spellAPI->setEffectDone(_this, spell_index, 0);
                return;
            }
        else //spell resistance failed, proceed to what happens on spell hit
            {
                uint32_t unused;
                SF_CGdTargetData relative_data;
                relative_data.position.X = 0;
                relative_data.position.Y = 0;
                relative_data.entity_type = 1;
                relative_data.entity_index = target_index;

                SF_Rectangle aux_data;
                aux_data.partA = 0;
                aux_data.partB = 0;
                spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitTarget, &unused, &relative_data, _this->OpaqueClass->current_step, 10, &aux_data);
                uint16_t damage = spell_data.params[0];
                toolboxAPI->dealDamage(_this->SF_CGdFigureToolBox, source_index, target_index, damage, 0, 0, 0);
                //spellAPI->figureAggro(_this, spell_index, target_index);
                spellAPI->setEffectDone(_this, spell_index, 0);
                return;
            }
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
    aiAPI = sfsf->aiAPI;
    buildingAPI = sfsf->buildingAPI;
    logger = sfsf->logAPI;


    // we register custom spell
    SFSpell *THROW_HAMMER_TOWER_spell = registrationAPI->registerSpell(THROW_HAMMER_TOWER_LINE);
    // we register building and link it with its json placed into /sfsf/%modname%/building folder
    SFBuilding *dwarf_tower = registrationAPI->registerBuilding("dwarf_tower_hammer");
    // we register handlers for custom spell
    registrationAPI->linkTypeHandler(THROW_HAMMER_TOWER_spell, &THROW_HAMMER_TOWER_type_handler);
    registrationAPI->linkEffectHandler(THROW_HAMMER_TOWER_spell, THROW_HAMMER_TOWER_JOB, &THROW_HAMMER_TOWER_effect_handler);
    registrationAPI->linkSingleTargetAIHandler(THROW_HAMMER_TOWER_spell, &THROW_HAMMER_TOWER_ai_handler);
    registrationAPI->linkEndHandler(THROW_HAMMER_TOWER_spell, &THROW_HAMMER_TOWER_end_handler);
    //registrationAPI->linkBuildingDoneHandler(dwarf_tower, &myDoneHandler);

}

/***
 * This function MUST be present in your code with the exact declaration
 * otherwise the framework is unable to describe your mod in logs and mod info menu (menu not yet implemented)
 ***/
extern "C" __declspec(dllexport) SFMod *RegisterMod(SpellforceSpellFramework *framework)
{
    return framework->createModInfo("Dwarf Tower", "1.0.0", "S'Baad", "A mod designed to demonstrate creation of custom building - Dwarf Tower controlled by an advanced AI.");
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
