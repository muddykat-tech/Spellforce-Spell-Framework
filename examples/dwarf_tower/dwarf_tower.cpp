#include "api/sfsf.h"
//#include "../../src/api/sfsf.h"
#include <windows.h>
#include <stdio.h>

// this example shows code necessary to create custom building
// the building will be dwarf tower which throws hammers at enemies in range
// the tower will also be having tweaked AI
// it will aim at the most damaged targets


// actually, from the inside of game engine tower projectiles are implemented as spells, which towers cast at enemies during a fight
// hence we need to create unique spell type for a spell which is used to simulate projectile of our tower

// We declare macros for  Spell Type and Spell Job and Building Type
// Spell Type 0xf9 = 249, Spell Job 0xb0 = 176

// The custom Spell Type also must be defined within GameData.cff
// and provided with at least one spell corresponding to it

#define DWARF_HAMMER_TOWER_LINE 0xf9
#define DWARF_HAMMER_TOWER_JOB 0xb0


SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;
IteratorFunctions *iteratorAPI;
RegistrationFunctions *registrationAPI;
AiFunctions *aiAPI;
BuildingFunctions *buildingAPI;
SFLog *logger;

// we declare spell type handler for hammer throw
// the spell is instant
// it applies damage to a target on a hit, and stops after that
void __thiscall dwarf_hammer_tower_type_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // actually, from the inside of game engine tower projectiles are implemented as spells, which towers cast at enemies during a fight
    // hence we need to create unique spell type for a spell which is used to simulate projectile of our tower
    // we need only to associate spell type with a spell job and nothing more than that
    _this->active_spell_list[spell_index].spell_job = DWARF_HAMMER_TOWER_JOB;
}

// we declare spell end handler for hammer throw
// this handler would work in case a spell wasn't finished correctly
void __thiscall dwarf_hammer_tower_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // since this spell is instant, we have only to get its index
    // and erase it from the list of active spells
    spellAPI->removeDLLNode(_this, spell_index);
    spellAPI->setEffectDone(_this, spell_index, 0);
}


// we declare building done handler
// this handler gets triggered every time the construction of a building of specific type is finished

void __thiscall dwarf_hammer_tower_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    // the tower doesn't have its own AI
    // instead it gets unit attached to it
    // which actually runs tower AI
    // this unit exists only virtually, there is no need to create an actually existing instance of it and place that into game world
    // it's enough to just link tower with specific unit ID taken from GameData.cff
    buildingAPI->attachTowerUnit(_this, building_index, 0xbb5); // 0xbb5 in hex stands for 2997, the ID of unit which is going to simulate tower garrison
}


// we declare single target AI handler for Hammer Throw spell
// this handler is constantly called by all units which possess this spell
// it is triggered separately for every target (doesn't matter hostile or friendly) which the object has in the line of sight
// the handler will provide us with relevant data about object, targets in its LOS and some other info

// depending on circumstances we have, we must return priority for a spell
// 0 stands for denying to cast the spell
// 2 stands for high priority, the tower will throw projectile at the given target
uint32_t __thiscall dwarf_hammer_tower_ai_handler(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{

    // Battle development Global Object provides us with relevant data about object which makes decision
    // and about all objects surrounding it
    // it provides object indexes, which are grouped into ally/neutral/enemy and figures/buildings categories
    // also it provides some convenient wrappers like entityCount
    // which we can use to determine whether category is empty or possesses some objects in it


    // first, let's cut off the most obvious dead-ends
    // if there are no enemies around, we don't need to consider the decision
    // let's terminate handler
    if ((_this->battleData.enemy_figures.entityCount == 0)||
        // as well as we don't need to cast spell
        // if supposed target has the same index as the object casting spell
        // since it means that the tower is trying to consider shooting itself
        (_this->battleData.current_figure == target_index))
    {
        // therefore we deny casting the spell
        // and don't make further calculations, as they're irrelevant
        return 0;
    }


    // now we're going to search for the figure with the lost health among all targets in sight
    // let's make a structure to store figure index and its health
    struct {
                uint16_t figure_index = 0; // 0 stands for unknown figure
                uint32_t health = 0x7fff;  // we put the highest possible number here
                                          //to make sure that real figures would always have less health than that
           } lowest_hp_figure;


    // we start searching through all figures in sight
    // provided to us with battle data
    for (uint16_t i = 0; i < _this->battleData.enemy_figures.entityCount; i++)
    {
        // battle data provides to us all figures in sight of the object
        // but tower radius could be much smaller than its range of view
        // because of that, we need to limit eligible targets with targets being in range
        uint32_t max_radius = spell_data->max_range;
        // also the tower potentially could have minimum radius, causing it to have "blind-spot" around it
        // hence, let's take minimum radius in consideration as well
        uint32_t min_radius = spell_data->min_range;

        // let's get tower coordinates
        // in battle data the object which is making decision, called current_figure
        SF_Coord tower_position = _this->battleData.CGdFigure->figures[_this->battleData.current_figure].position;

        // let's get target coordinates
        // it's going to be a bit complicated, because i stands for figure index within enemies list provided to us with Battle Data
        // and we need to transform it into index corresponding to figures global list
        // we can obtain global index of figure
        // by addressing property entity_index of the subsequent element of data array of enemy_figures list
        uint16_t enemy_figure = _this->battleData.enemy_figures.data[i].entity_index;
        // then we can reach target coordinates using Figures Global Object
        SF_Coord target_position = _this->battleData.CGdFigure->figures[enemy_figure].position;

        // let's use function-wrapper from toolbox group to obtain distance between two points in area
        uint32_t distance = toolboxAPI->getDistance (&tower_position, &target_position);
        // game engine doesn't allow us direct access to figure's current health
        // we have to calculate it via applying current health percent to base value
        uint16_t enemy_health = _this->battleData.CGdFigure->figures[enemy_figure].health.base_val * (figureAPI->getCurrentHealthPercent(_this->battleData.CGdFigure, enemy_figure)) / 100;

        if ((enemy_health < lowest_hp_figure.health) && // we seek for the figure with the health lower than the health we assumed as minimal
            (lowest_hp_figure.figure_index != _this->battleData.current_figure) && // we ignore tower's own health, because it's forbidden from attacking itself
            (distance >= min_radius) && (distance <= max_radius) &&  // we ignore all figures outside of spell range
            (figureAPI->isAlive(_this->battleData.CGdFigure, enemy_figure))) // we check for the target figure to be alive
            {
                // if we found figure with lower health, we remember its index
                lowest_hp_figure.figure_index = enemy_figure;
                // as well as its hit-points
                lowest_hp_figure.health = enemy_health;
                // and search for the next figure in the list, until it's out
            }

    }

    // if selected unit has the lowest health among targets in spell radius, we throw hammer at it
    if (lowest_hp_figure.figure_index == target_index)
    {
        // we return the highest priority to apply the spell
        return 2;
    }
    // if selected unit doesn't have the lowest health among targets in radius, we don't do anything
    else
    {
        // we deny casting the spell
        // and terminate the handler
        return 0;
    }

}


// we declare the effect handler for tower spell

void __thiscall dwarf_hammer_tower_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // we pull the pointer for this instance of spell
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    uint16_t target_index = spell->target.entity_index;
    uint16_t source_index = spell->source.entity_index;

    // we grab spell id and its spell job (spell type) id
    SF_SpellEffectInfo effect_info;
    effect_info.spell_id = spell->spell_id;
    effect_info.job_id = spell->spell_job;

    // we load spell parameters from game data and put them into structure
    SF_CGdResourceSpell spell_data;
    spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);

    // we've got a lot of technical conditions which can prevent spell cast, so if we don't meet at least one of them, spell fails
    if ((figureAPI->isAlive(_this->SF_CGdFigure, target_index))
        || (toolboxAPI->isTargetable(_this->SF_CGdFigureToolBox, target_index))
        || (toolboxAPI->figuresCheckHostile(_this->SF_CGdFigureToolBox, source_index, target_index))
        || ((int16_t)(_this->SF_CGdFigure->figures[target_index].owner) != -1))
     {

                uint32_t unused;

                // we declare geometry for spell visual effect
                SF_CGdTargetData relative_data;
                relative_data.position.X = 0;
                relative_data.position.Y = 0;
                relative_data.entity_type = 1;
                relative_data.entity_index = target_index;

                // since the spell isn't AoE, we declare its area of effect as 0
                SF_Rectangle aux_data;
                aux_data.partA = 0;
                aux_data.partB = 0;

                // we apply on hit visual effect to the target
                spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitTarget, &unused, &relative_data, _this->OpaqueClass->current_step, 10, &aux_data);


                // we obtain spell damage
                uint16_t damage = spell_data.params[0];
                // because hammer throws deals physical damage, it must be reduced by target's armor
                // let's get reduction percent with toolbox function getPhysDamageReduction
                uint16_t reduction_percent = toolboxAPI->getPhysDamageReduction(_this->SF_CGdFigureToolBox, source_index, target_index, DWARF_HAMMER_TOWER_LINE);
                // we adjust damage with damage reduction
                damage = ((uint32_t)(damage * reduction_percent + 5000)) / 10000;
                char aliveInfo[256];
                sprintf(aliveInfo, "DAMAGE REDUCTION PERCENTAGE IS %hd \n", reduction_percent);
                logger->logInfo(aliveInfo);

                sprintf(aliveInfo, "SOURCE INDEX IS %hd \n", source_index);
                logger->logInfo(aliveInfo);

                sprintf(aliveInfo, "TARGET INDEX IS %hd \n", target_index);
                logger->logInfo(aliveInfo);

                // then we apply spell damage to the target
                toolboxAPI->dealDamage(_this->SF_CGdFigureToolBox, source_index, target_index, damage, 0, 0, 0);



                // we terminate spell effect, because it's not supposed to continue
                spellAPI->setEffectDone(_this, spell_index, 0);
                return;
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


    // we register custom spell which is necessary to simulate tower projectiles
    SFSpell *dwarf_hammer_tower_spell = registrationAPI->registerSpell(DWARF_HAMMER_TOWER_LINE);

    // we have to register building type for game engine
    // we have to register it via code
    // because amount of building types which can be placed within GameData.cff is hardcoded to be 213

    // to register new building, we can use any number in range 214-256
    // of course, provided it's not already occupied by another mod
    // let's use number 214 for this example
    SFBuilding *dwarf_tower = registrationAPI->registerBuilding(222);

    // then we link building with its json placed into /sfsf/%modname%/building folder
    registrationAPI->linkBuildingJSON(dwarf_tower, "dwarf_tower_hammer"); // we pass filename without extension, '.json' is appended automatically
    // then we register handlers for custom spell as usual
    registrationAPI->linkTypeHandler(dwarf_hammer_tower_spell, &dwarf_hammer_tower_type_handler);
    registrationAPI->linkEffectHandler(dwarf_hammer_tower_spell, DWARF_HAMMER_TOWER_JOB, &dwarf_hammer_tower_effect_handler);
    registrationAPI->linkEndHandler(dwarf_hammer_tower_spell, &dwarf_hammer_tower_end_handler);
    // then we link single target AI handler to custom spell
    registrationAPI->linkSingleTargetAIHandler(dwarf_hammer_tower_spell, &dwarf_hammer_tower_ai_handler);
    // and finally we register new handler which catches the moment when the tower building is finished
    // and attaches unit running its AI to it
    registrationAPI->linkBuildingDoneHandler(dwarf_tower, &dwarf_hammer_tower_done_handler);

}

/***
 * This function MUST be present in your code with the exact declaration
 * otherwise the framework is unable to describe your mod in logs and mod info menu (menu not yet implemented)
 ***/
extern "C" __declspec(dllexport) SFMod *RegisterMod(SpellforceSpellFramework *framework)
{
    // it's critical for mod name to match with folder name where mod assets (such as building JSON data) stored
    // in this case mod name isn't purely cosmetic
    // game data of custom building added by this mod is going to be located in %Spellforce/sfsf/Dwarf Tower/buildings/ folder
    return framework->createModInfo("Dwarf Tower", "1.0.0", "Teekius", "A mod designed to demonstrate creation of custom building which is going to be Dwarf Tower controlled by an advanced AI.");
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
