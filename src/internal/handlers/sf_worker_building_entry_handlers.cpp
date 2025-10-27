#include "sf_worker_building_entry_handlers.h"
#include "../core/sf_wrappers.h"
#include "../core/hooks/sf_endspell_hook.h"

//FIXME
bool buildingIsMaster(uint16_t building_type)
{
    if ((building_type == 0x18) || (building_type == 0x1f) || (building_type == 0x0b))
    {
        return true;
    }
    return false;
}

//FIXME
bool buildingIsMoonsilverMine(uint16_t building_type)
{
    if ((building_type == 0x33) || (building_type == 0x59))
    {
        return true;
    }
    return false;
}


void __thiscall building_repair(SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id)
{
    uint16_t worker_building = toolboxAPI.getFigureXData(_this->CGdFigureToolBox, figure_id,
                                                         SpellDataKey::SPELL_CONSERVATION_SHIELD);
    if (worker_building != 0)
    {
        buildingAPI.releaseWorkers(_this->CGdBuilding, building_id, 1);
        uint16_t job_to = kGdJobBuilderSearchForWork;
        uint16_t job_from = _this->CGdFigure->figures[figure_id].current_job.GdJobId;
        figureAPI.prepareJobTransition(_this, figure_id, job_from, job_to);
        figureAPI.onStartJob(_this, figure_id, kGdJobBuilderSearchForWork);
        uint16_t spell_index = toolboxAPI.getSpellIndexOfType(_this->CGdFigureToolBox, figure_id,
                                                              kGdSpellLineInvisibility, 0);
        if (spell_index == 0)
        {
            spell_index = toolboxAPI.getSpellIndexOfType(_this->CGdFigureToolBox, figure_id,
                                                         kGdSpellLineFeignDeath, 0);
        }
        if (spell_index != 0)
        {
            sf_endspell_hook(_this->CGdSpell, spell_index);
        }
    }
}
typedef void (__thiscall *jobCriticalHit_ptr)(SF_CGdFigureJobs *_this,uint16_t figure_id);


void __thiscall default_entry_handler(SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id)
{
    if (building_id != 0)
    {
        buildingAPI.freeAssignedWorker(_this->CGdBuildingToolBox, figure_id);
    }
    jobCriticalHit_ptr jobCriticalHit = (jobCriticalHit_ptr)(ASI::AddrOf(0x2df980));
    jobCriticalHit(_this, figure_id);
}

void __thiscall hq_entry_handler(SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id)
{
    uint8_t good = _this->CGdFigure->figures[figure_id].good;
    if (good != 0)
    {
        figureAPI.setJob(_this, figure_id, kGdJobCarrierCheckDrop);
        toolboxAPI.setFigureXData(_this->CGdFigureToolBox, figure_id, EFFECT_ENTITY_INDEX2, building_id);
        return;
    }
    if (building_id != 0)
    {
        buildingAPI.freeAssignedWorker(_this->CGdBuildingToolBox, figure_id);
    }
    jobCriticalHit_ptr jobCriticalHit = (jobCriticalHit_ptr)(ASI::AddrOf(0x2df980));
    jobCriticalHit(_this, figure_id);
    //TODO -- JobCriticalHit(_this, figure_id);
}

void __thiscall woodcutter_entry_handler (SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id)
{
    uint8_t good = _this->CGdFigure->figures[figure_id].good;
    if (good == 0)
    {
        figureAPI.setJob(_this, figure_id, kGdJobWoodCutterSearchTree);
    }
    else
    {
        figureAPI.setJob(_this, figure_id, kGdJobWoodCutterCheckDrop);
        toolboxAPI.setFigureXData(_this->CGdFigureToolBox, figure_id, EFFECT_ENTITY_INDEX2, building_id);
    }
    toolboxAPI.setFigureXData(_this->CGdFigureToolBox, figure_id, WORKER_HOST_BUILDING_POS_X,
                              _this->CGdBuilding->buildings[building_id].position.X);
    toolboxAPI.setFigureXData(_this->CGdFigureToolBox, figure_id, WORKER_HOST_BUILDING_POS_Y,
                              _this->CGdBuilding->buildings[building_id].position.Y);
    toolboxAPI.equipArtisanArmour(_this->CGdFigureToolBox, figure_id);
    CGdFigureTask task = CGdFigureTask::TASK_WOODCUTTER;
    if (buildingIsMaster(_this->CGdBuilding->buildings[building_id].type))
    {
        task = CGdFigureTask::TASK_MASTER_WOODCUTTER;
    }
    figureAPI.setTask(_this->CGdFigure, figure_id, task);
    toolboxAPI.equipArtisanTools(_this->CGdFigureToolBox, figure_id, 5, 0);
    SF_SGtFigureAction action;
    action.type = 0;
    action.subtype = 1;
    action.unkn2 = 0;
    action.unkn3 = 0;
    action.unkn4 = 0;
    action.unkn5 = 0;
    figureAPI.addAction(_this->CGdFigure, figure_id, &action);
}

void __thiscall sawmill_entry_handler (SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id)
{
    figureAPI.setJob(_this, figure_id, kGdJobCarpenterSearchForWork);
    toolboxAPI.equipArtisanArmour(_this->CGdFigureToolBox, figure_id);
    figureAPI.setTask(_this->CGdFigure, figure_id, CGdFigureTask::TASK_CARPENTER);
    uint8_t race = _this->CGdBuilding->buildings[building_id].race;
    if (race == 2)
    {
        toolboxAPI.equipArtisanTools(_this->CGdFigureToolBox, figure_id, 9, 17);
    }
    else
    {
        toolboxAPI.equipArtisanTools(_this->CGdFigureToolBox, figure_id, 0, 0);
    }
}

void __thiscall cattle_breeder_entry_handler (SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id)
{
    figureAPI.setJob(_this, figure_id, kGdJobCattleBreederSearchForWork);
    toolboxAPI.equipArtisanArmour(_this->CGdFigureToolBox, figure_id);
    figureAPI.setTask(_this->CGdFigure, figure_id, CGdFigureTask::TASK_CATTLE_BREEDER);
    toolboxAPI.equipArtisanTools(_this->CGdFigureToolBox, figure_id, 0, 0);
}

void __thiscall quarry_entry_handler (SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id)
{
    uint8_t good = _this->CGdFigure->figures[figure_id].good;
    if (good == 0)
    {
        figureAPI.setJob(_this, figure_id, kGdJobStoneMinerSearchStone);
    }
    else
    {
        figureAPI.setJob(_this, figure_id, kGdJobStoneMinerCheckDrop);
        toolboxAPI.setFigureXData(_this->CGdFigureToolBox, figure_id, EFFECT_ENTITY_INDEX2, building_id);
    }
    toolboxAPI.setFigureXData(_this->CGdFigureToolBox, figure_id, WORKER_HOST_BUILDING_POS_X,
                              _this->CGdBuilding->buildings[building_id].position.X);
    toolboxAPI.setFigureXData(_this->CGdFigureToolBox, figure_id, WORKER_HOST_BUILDING_POS_Y,
                              _this->CGdBuilding->buildings[building_id].position.Y);
    toolboxAPI.equipArtisanArmour(_this->CGdFigureToolBox, figure_id);
    figureAPI.setTask(_this->CGdFigure, figure_id, CGdFigureTask::TASK_QUARRY);
    toolboxAPI.equipArtisanTools(_this->CGdFigureToolBox, figure_id, 6, 0);
    SF_SGtFigureAction action;
    action.type = 0;
    action.subtype = 3;
    action.unkn2 = 0;
    action.unkn3 = 0;
    action.unkn4 = 0;
    action.unkn5 = 0;
    figureAPI.addAction(_this->CGdFigure, figure_id, &action);
}

void __thiscall foodstore_entry_handler (SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id)
{
    figureAPI.setJob(_this, figure_id, kGdJobFoodWorkerSearchForWork);
    toolboxAPI.equipArtisanArmour(_this->CGdFigureToolBox, figure_id);
    uint8_t race = _this->CGdBuilding->buildings[building_id].race;
    if (race == 2 || race == 4 )
    {
        toolboxAPI.equipArtisanTools(_this->CGdFigureToolBox, figure_id, 0x12, 0);
    }
    else
    {
        toolboxAPI.equipArtisanTools(_this->CGdFigureToolBox, figure_id, 0, 0);
    }
    figureAPI.setTask(_this->CGdFigure, figure_id, CGdFigureTask::TASK_STOREKEEPER);
}

typedef void (__thiscall *updateHunterList_ptr)(SF_CGdFigureJobs *_this,uint16_t figure_id);

void __thiscall hunter_entry_handler (SF_CGdFigureJobs *_this, uint16_t figure_id, uint16_t building_id)
{
    figureAPI.setJob(_this, figure_id, kGdJobHunterSearchForWork);
    toolboxAPI.equipArtisanArmour(_this->CGdFigureToolBox, figure_id);
    CGdFigureTask task = CGdFigureTask::TASK_HUNTER;
    if (buildingIsMaster(_this->CGdBuilding->buildings[building_id].type))
    {
        task = CGdFigureTask::TASK_HUNTING_LODGE;
    }
    figureAPI.setTask(_this->CGdFigure, figure_id, task);
    toolboxAPI.equipArtisanTools(_this->CGdFigureToolBox, figure_id, 0, 8);
    updateHunterList_ptr updateHunterList = (updateHunterList_ptr)ASI::AddrOf(0x2e1f40);
    updateHunterList(_this, figure_id);
}

void __thiscall fisher_entry_handler (SF_CGdFigureJobs *_this, uint16_t figure_id, uint16_t building_id)
{
    figureAPI.setJob(_this, figure_id, kGdJobFisherSearchForWork);
    toolboxAPI.equipArtisanArmour(_this->CGdFigureToolBox, figure_id);
    CGdFigureTask task = CGdFigureTask::TASK_FISHER;
    if (buildingIsMaster(_this->CGdBuilding->buildings[building_id].type))
    {
        task = CGdFigureTask::TASK_MASTER_FISHER;
    }
    figureAPI.setTask(_this->CGdFigure, figure_id, task);
    toolboxAPI.equipArtisanTools(_this->CGdFigureToolBox, figure_id, 3, 0);
}

void __thiscall farmer_entry_handler (SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id)
{
    figureAPI.setJob(_this, figure_id, kGdJobFarmerWaitForWork);
    toolboxAPI.equipArtisanArmour(_this->CGdFigureToolBox, figure_id);
    figureAPI.setTask(_this->CGdFigure, figure_id, CGdFigureTask::TASK_FARMER);
    toolboxAPI.equipArtisanTools(_this->CGdFigureToolBox, figure_id, 0, 0);
}

void __thiscall miner_entry_handler (SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id)
{
    figureAPI.setJob(_this, figure_id, kGdJobMinerSearchResource);
    toolboxAPI.setFigureXData(_this->CGdFigureToolBox, figure_id, WORKER_HOST_BUILDING_POS_X,
                              _this->CGdBuilding->buildings[building_id].position.X);
    toolboxAPI.setFigureXData(_this->CGdFigureToolBox, figure_id, WORKER_HOST_BUILDING_POS_Y,
                              _this->CGdBuilding->buildings[building_id].position.Y);
    toolboxAPI.equipArtisanArmour(_this->CGdFigureToolBox, figure_id);
    figureAPI.setTask(_this->CGdFigure, figure_id, CGdFigureTask::TASK_MINE);
    toolboxAPI.equipArtisanTools(_this->CGdFigureToolBox, figure_id, 6, 0);
    SF_SGtFigureAction action;
    action.type = 0;
    action.subtype = 8;
    if (buildingIsMoonsilverMine(_this->CGdBuilding->buildings[building_id].type))
    {
        action.subtype = 6;
    }
    action.unkn2 = 0;
    action.unkn3 = 0;
    action.unkn4 = 0;
    action.unkn5 = 0;
    figureAPI.addAction(_this->CGdFigure, figure_id, &action);
}

void __thiscall smelter_entry_handler (SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id)
{
    figureAPI.setJob(_this, figure_id, kGdJobSmelterSearchForWork);
    toolboxAPI.equipArtisanArmour(_this->CGdFigureToolBox, figure_id);
    figureAPI.setTask(_this->CGdFigure, figure_id, CGdFigureTask::TASK_SMELTER);
    toolboxAPI.equipArtisanTools(_this->CGdFigureToolBox, figure_id, 0xe, 0);
}

//aria and lenya
void __thiscall shrine_entry_handler (SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id)
{
    figureAPI.setJob(_this, figure_id, kGdJobShrineWorkerSearchForWork);
    toolboxAPI.equipArtisanArmour(_this->CGdFigureToolBox, figure_id);
    figureAPI.setTask(_this->CGdFigure, figure_id, CGdFigureTask::TASK_SHRINE);
    uint8_t race = _this->CGdBuilding->buildings[building_id].race;
    if (race == 1 || race == 6 )
    {
        toolboxAPI.equipArtisanTools(_this->CGdFigureToolBox, figure_id, 0x0f, 0);
    }
    else
    {
        toolboxAPI.equipArtisanTools(_this->CGdFigureToolBox, figure_id, 0, 0);
    }
}

void __thiscall temple_entry_handler (SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id)
{
    figureAPI.setJob(_this, figure_id, kGdJobPriestSearchForWork);
    toolboxAPI.equipArtisanArmour(_this->CGdFigureToolBox, figure_id);
    figureAPI.setTask(_this->CGdFigure, figure_id, CGdFigureTask::TASK_PRIEST);
    toolboxAPI.equipArtisanTools(_this->CGdFigureToolBox, figure_id, 0x0, 0);
    uint8_t race = _this->CGdBuilding->buildings[building_id].race;
    uint8_t bvar1 = 1;
    uint16_t owner = _this->CGdBuilding->buildings[building_id].owner;
    buildingAPI.updateProduction(_this->CGdPlayer, owner, race, bvar1, 1);
}

void __thiscall forester_entry_handler (SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id)
{
    figureAPI.setJob(_this, figure_id, kGdJobForesterSearchForWork);
    toolboxAPI.equipArtisanArmour(_this->CGdFigureToolBox, figure_id);
    figureAPI.setTask(_this->CGdFigure, figure_id, CGdFigureTask::TASK_FORESTER);
    toolboxAPI.equipArtisanTools(_this->CGdFigureToolBox, figure_id, 0, 0);
}

void __thiscall gatherer_entry_handler (SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id)
{
    figureAPI.setJob(_this, figure_id, kGdJobGathererSearchResource);
    toolboxAPI.equipArtisanArmour(_this->CGdFigureToolBox, figure_id);
    figureAPI.setTask(_this->CGdFigure, figure_id, CGdFigureTask::TASK_GATHERER);
    toolboxAPI.equipArtisanTools(_this->CGdFigureToolBox, figure_id, 0x0b, 0);
}

void __thiscall forge_entry_handler (SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id)
{
    figureAPI.setJob(_this, figure_id, kGdJobSmithSearchForWork);
    toolboxAPI.equipArtisanArmour(_this->CGdFigureToolBox, figure_id);
    figureAPI.setTask(_this->CGdFigure, figure_id, CGdFigureTask::TASK_FORGE);
    uint8_t race = _this->CGdBuilding->buildings[building_id].race;
    switch (race)
    {
        case 4:
        {
            toolboxAPI.equipArtisanTools(_this->CGdFigureToolBox, figure_id, 0, 0x12);
            break;
        }
        case 3:
        {
            toolboxAPI.equipArtisanTools(_this->CGdFigureToolBox, figure_id, 0x10, 0);
            break;
        }
        default:
        {
            toolboxAPI.equipArtisanTools(_this->CGdFigureToolBox, figure_id, 9, 0);
            break;
        }
    }
    uint8_t bvar1 = 0;
    uint16_t owner = _this->CGdBuilding->buildings[building_id].owner;
    buildingAPI.updateProduction(_this->CGdPlayer, owner, race, bvar1, 1);
}

void __thiscall mace_carver_entry_handler (SF_CGdFigureJobs *_this,uint16_t figure_id, uint16_t building_id)
{
    figureAPI.setJob(_this, figure_id, kGdJobClubMakerSearchForWork);
    toolboxAPI.equipArtisanArmour(_this->CGdFigureToolBox, figure_id);
    figureAPI.setTask(_this->CGdFigure, figure_id, CGdFigureTask::TASK_MACE_CARVER);
    toolboxAPI.equipArtisanTools(_this->CGdFigureToolBox, figure_id, 0, 0x12);

    uint8_t race = _this->CGdBuilding->buildings[building_id].race;
    uint8_t bvar1 = 2;
    uint16_t owner = _this->CGdBuilding->buildings[building_id].owner;
    buildingAPI.updateProduction(_this->CGdPlayer, owner, race, bvar1, 1);
}
typedef void (__thiscall *updateScavengerList_ptr)(SF_CGdFigureJobs *_this,uint16_t figure_id);

void __thiscall scavenger_entry_handler (SF_CGdFigureJobs *_this, uint16_t figure_id, uint16_t building_id)
{
    figureAPI.setJob(_this, figure_id, kGdJobCorpseCollectorSearchForWork);
    toolboxAPI.equipArtisanArmour(_this->CGdFigureToolBox, figure_id);
    CGdFigureTask task = CGdFigureTask::TASK_SCAVENGER;
    figureAPI.setTask(_this->CGdFigure, figure_id, task);
    toolboxAPI.equipArtisanTools(_this->CGdFigureToolBox, figure_id, 0, 8);
    updateScavengerList_ptr updateScavengerList = (updateScavengerList_ptr)ASI::AddrOf(0x2df340);
    updateScavengerList(_this, figure_id);
}
