//#include "api/sfsf.h"
#include "../../src/api/sfsf.h"
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



SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;
IteratorFunctions *iteratorAPI;
RegistrationFunctions *registrationAPI;
AiFunctions *aiAPI;
BuildingFunctions *buildingAPI;
SFLog *logger;

void __thiscall orc_quarry_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI->setBuildingXData(_this, building_index, SpellDataKey::BUILDING_POSITION_X,
                                  _this->CGdBuilding->buildings[building_index].position.X);
    buildingAPI->setBuildingXData(_this, building_index, SpellDataKey::BUILDING_POSITION_Y,
                                  _this->CGdBuilding->buildings[building_index].position.Y);
}



void __thiscall quarry_entry_handler (SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id)
{
    uint8_t good = _this->CGdFigure->figures[figure_id].good;
    if (good == 0)
    {
        figureAPI->setJob(_this, figure_id, kGdJobStoneMinerSearchStone);
    }
    else
    {
        figureAPI->setJob(_this, figure_id, kGdJobStoneMinerCheckDrop);
        toolboxAPI->setFigureXData(_this->CGdFigureToolBox, figure_id, EFFECT_ENTITY_INDEX2, building_id);
    }
    toolboxAPI->setFigureXData(_this->CGdFigureToolBox, figure_id, WORKER_HOST_BUILDING_POS_X,
                               _this->CGdBuilding->buildings[building_id].position.X);
    toolboxAPI->setFigureXData(_this->CGdFigureToolBox, figure_id, WORKER_HOST_BUILDING_POS_Y,
                               _this->CGdBuilding->buildings[building_id].position.Y);
    toolboxAPI->equipArtisanArmour(_this->CGdFigureToolBox, figure_id);
    figureAPI->setTask(_this->CGdFigure, figure_id, CGdFigureTask::TASK_QUARRY);
    toolboxAPI->equipArtisanTools(_this->CGdFigureToolBox, figure_id, 6, 0);
    SF_SGtFigureAction action;
    action.type = 0;
    action.subtype = 3;
    action.target.entity_index = 0;
    action.target.entity_type = 0;
    action.target.position = {0, 0};
    action.unkn = 0;
    figureAPI->addAction(_this->CGdFigure, figure_id, &action);
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


    SFBuilding *orc_quarry = registrationAPI->registerBuilding(50);
    registrationAPI->linkBuildingJSON(orc_quarry, "orc_quarry");
    registrationAPI->linkBuildingDoneHandler(orc_quarry, &orc_quarry_done_handler);
    registrationAPI->linkBuildingEntryHandler(orc_quarry, &quarry_entry_handler);
    registrationAPI->applyBuildingTag(orc_quarry, BuildingTag::QUARRY_BUILDING);
    registrationAPI->applyBuildingTag(orc_quarry, BuildingTag::HABITABLE_BUILDING);
    orc_quarry->race = 5;

}

/***
 * This function MUST be present in your code with the exact declaration
 * otherwise the framework is unable to describe your mod in logs and mod info menu (menu not yet implemented)
 ***/
extern "C" __declspec(dllexport) SFMod *RegisterMod(SpellforceSpellFramework *framework)
{
    // it's critical for mod name to match with folder name where mod assets (such as building JSON data) stored
    // in this case mod name isn't purely cosmetic
    // game data of custom building added by this mod is going to be located in %Spellforce/sfsf/orc_quarry/buildings/ folder
    return framework->createModInfo("orc_quarry", "1.0.0", "Un'Schtalch",
                                    "A mod designed to demonstrate creation of custom building resource gathering building");
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
