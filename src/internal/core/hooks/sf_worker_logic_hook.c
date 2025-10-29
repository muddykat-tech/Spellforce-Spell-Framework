#include "sf_worker_logic_hook.h"
#include "../sf_hooks.h"
#include "sf_endspell_hook.h"
#include "../sf_wrappers.h"

extern SpellFunctions spellAPI;
extern EffectFunctions effectAPI;
extern ToolboxFunctions toolboxAPI;
extern BuildingFunctions buildingAPI;
extern FigureFunctions figureAPI;
extern IteratorFunctions iteratorAPI;
extern AiFunctions aiAPI;
extern RegistrationFunctions registrationAPI;

void terminate_job_spells(SF_CGdFigureJobs *_this, uint16_t figure_id)
{
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

//TODO - use registry instead of hardcode
static uint8_t getRacialSmallHQ(uint8_t race)
{
    uint8_t result = 0;
    switch (race)
    {
        case 1:
            result = 2;
            break;
        case 2:
            result = 41;
            break;
        case 3:
            result = 20;
            break;
        case 4:
            result = 73;
            break;
        case 5:
            result = 58;
            break;
        case 6:
            result = 85;
            break;
        default:
            break;
    }
    return result;
}
//NOTE SF_Coord here is used as two uint16_t
typedef void (__thiscall *startWalkingTo_ptr)(SF_CGdFigureJobs *_this, uint16_t figure_id, SF_CGdTargetData *target,
                                              SF_Coord *ranges, uint8_t walk_mode, uint32_t param5);
typedef uint32_t *(__thiscall *getObjectQueue_ptr)(void *CGdObject, uint16_t object_index);
typedef void (__thiscall *addToObjectQueue_ptr)(uint32_t *_this, uint16_t figure_index);
startWalkingTo_ptr startWalkingTo;
getObjectQueue_ptr getObjectQueue;
addToObjectQueue_ptr addToObjectQueue;

void initialize_worker_logic_data_hooks()
{
    startWalkingTo = (startWalkingTo_ptr)ASI::AddrOf(0x2e3750);
    getObjectQueue = (getObjectQueue_ptr)ASI::AddrOf(0x31d7b0);
    addToObjectQueue = (addToObjectQueue_ptr)ASI::AddrOf(0x354be0);
}
static void setupWalkToMonument(SF_CGdFigureJobs *_this, uint16_t figure_id, uint16_t monument_id)
{
    SF_Coord ranges = {1,1};
    SF_CGdTargetData t_data;
    t_data.entity_index = monument_id;
    t_data.entity_type = 3;
    t_data.position = {0,0};
    toolboxAPI.setFigureXData(_this->CGdFigureToolBox, figure_id, EFFECT_ENTITY_INDEX2, monument_id);
    startWalkingTo(_this, figure_id, &t_data, &ranges, 0, 1);
    uint32_t *unknown_list = getObjectQueue(_this->CGdObject, monument_id);
    addToObjectQueue(unknown_list, figure_id);
}

bool __thiscall onWoodcutterFinishJob(SF_CGdFigureJobs *_this, uint16_t figure_id)
{
    uint16_t job_id = _this->CGdFigure->figures[figure_id].current_job.GdJobId;
    uint8_t figure_task = _this->CGdFigure->figures[figure_id].current_job.task;
    uint8_t target_building_type = 0;
    uint16_t target_building = 0;
    uint32_t target_job = kGdJobWoodCutterWalkHome;
    if (figure_task == CGdFigureTask::TASK_WORKER)
    {
        uint8_t race = _this->CGdFigure->figures[figure_id].race;
        target_building_type = getRacialSmallHQ(race);
        if (target_building_type != 0)
        {
            target_building = buildingAPI.findClosestBuilding(_this->CGdBuildingToolBox,
                                                              _this->CGdFigure->figures[figure_id].position,
                                                              target_building_type,
                                                              _this->CGdFigure->figures[figure_id].owner, 0x19);
        }
    }
    else
    {
        uint8_t race = _this->CGdFigure->figures[figure_id].race;
        target_building_type = buildingAPI.getRacialSawmill(race);
        if (target_building_type != 0)
        {
            uint16_t building_index = _this->CGdFigure->figures[figure_id].building;
            //NO, there's no invariant here!
            target_building = buildingAPI.findClosestBuilding(_this->CGdBuildingToolBox,
                                                              _this->CGdBuilding->buildings[building_index].position,
                                                              target_building_type,
                                                              _this->CGdFigure->figures[figure_id].owner, 0x19);
        }
    }
    if (target_building != 0)
    {
        target_job = kGdJobWoodCutterWalkToDeliverGood;
    }
    else
    {
        if (figure_task == CGdFigureTask::TASK_WORKER)
        {
            uint16_t monument_id, unused;
            monument_id = toolboxAPI.findClosestMonument(_this->CGdObjectToolBox, figure_id, &unused);
            if (monument_id == 0)
            {
                figureAPI.setJob(_this, figure_id, job_id);
                return false;
            }
            figureAPI.prepareJobTransition(_this, figure_id, job_id, target_job);
            terminate_job_spells(_this, figure_id);
            figureAPI.onStartJob(_this, figure_id, target_job);
            setupWalkToMonument(_this, figure_id, monument_id);
            return true;
        }
        else
        {
            target_building = _this->CGdFigure->figures[figure_id].building;
        }
    }
    figureAPI.prepareJobTransition(_this, figure_id, job_id, target_job);
    terminate_job_spells(_this, figure_id);
    figureAPI.onStartJob(_this, figure_id, target_job);
    if (target_job == kGdJobWoodCutterWalkToDeliverGood)
    {
        buildingAPI.addFigureToBuilding(_this->CGdBuildingToolBox, target_building, figure_id, 0);
    }
    toolboxAPI.setFigureXData(_this->CGdFigureToolBox, figure_id, WORKER_DELIVERY_BUILDING, target_building);
    toolboxAPI.setFigureXData(_this->CGdFigureToolBox, figure_id, EFFECT_ENTITY_INDEX2, 0);
    SF_Coord walk_pos = _this->CGdBuilding->buildings[target_building].position;
    figureAPI.setFigureWalkPos(_this, figure_id, &walk_pos, 0, 1);

    return true;
}

bool __thiscall onMinerFinishJob(SF_CGdFigureJobs *_this, uint16_t figure_id)
{
    uint16_t job_id = _this->CGdFigure->figures[figure_id].current_job.GdJobId;
    uint8_t figure_task = _this->CGdFigure->figures[figure_id].current_job.task;
    uint8_t target_building_type = 0;
    uint16_t target_building = 0;
    uint32_t target_job = kGdJobMinerWalkHome;
    if (figure_task == CGdFigureTask::TASK_WORKER)
    {
        uint8_t race = _this->CGdFigure->figures[figure_id].race;
        target_building_type = getRacialSmallHQ(race);
        if (target_building_type != 0)
        {
            target_building = buildingAPI.findClosestBuilding(_this->CGdBuildingToolBox,
                                                              _this->CGdFigure->figures[figure_id].position,
                                                              target_building_type,
                                                              _this->CGdFigure->figures[figure_id].owner, 0x19);
        }
    }
    else
    {
        uint8_t race = _this->CGdFigure->figures[figure_id].race;
        uint16_t building_index = _this->CGdFigure->figures[figure_id].building;
        if (buildingAPI.buildingIsMoonsilverMine(_this->CGdBuilding, building_index))
        {
            target_building = 0;
        }
        else
        {
            target_building_type = buildingAPI.getRacialSmelter(race);
            if (target_building_type != 0)
            {
                building_index = _this->CGdFigure->figures[figure_id].building;
                //NO, there's no invariant here!
                target_building = buildingAPI.findClosestBuilding(_this->CGdBuildingToolBox,
                                                                  _this->CGdBuilding->buildings[building_index].position,
                                                                  target_building_type,
                                                                  _this->CGdFigure->figures[figure_id].owner, 0x19);
            }
        }
    }
    if (target_building != 0)
    {
        target_job = kGdJobMinerWalkToDeliverGood;
    }
    else
    {
        if (figure_task == CGdFigureTask::TASK_WORKER)
        {
            uint16_t monument_id, unused;
            monument_id = toolboxAPI.findClosestMonument(_this->CGdObjectToolBox, figure_id, &unused);
            if (monument_id == 0)
            {
                figureAPI.setJob(_this, figure_id, job_id);
                return false;
            }
            figureAPI.prepareJobTransition(_this, figure_id, job_id, target_job);
            terminate_job_spells(_this, figure_id);
            figureAPI.onStartJob(_this, figure_id, target_job);
            setupWalkToMonument(_this, figure_id, monument_id);
            return true;
        }
        else
        {
            target_building = _this->CGdFigure->figures[figure_id].building;
        }
    }
    figureAPI.prepareJobTransition(_this, figure_id, job_id, target_job);
    terminate_job_spells(_this, figure_id);
    figureAPI.onStartJob(_this, figure_id, target_job);
    if (target_job == kGdJobMinerWalkToDeliverGood)
    {
        buildingAPI.addFigureToBuilding(_this->CGdBuildingToolBox, target_building, figure_id, 0);
    }
    toolboxAPI.setFigureXData(_this->CGdFigureToolBox, figure_id, WORKER_DELIVERY_BUILDING, target_building);
    toolboxAPI.setFigureXData(_this->CGdFigureToolBox, figure_id, EFFECT_ENTITY_INDEX2, 0);
    SF_Coord walk_pos = _this->CGdBuilding->buildings[target_building].position;
    figureAPI.setFigureWalkPos(_this, figure_id, &walk_pos, 0, 1);

    return true;
}

bool __thiscall onStoneMinerFinishJob(SF_CGdFigureJobs *_this, uint16_t figure_id)
{
    uint16_t job_id = _this->CGdFigure->figures[figure_id].current_job.GdJobId;
    uint8_t figure_task = _this->CGdFigure->figures[figure_id].current_job.task;
    uint8_t target_building_type = 0;
    uint16_t target_building = 0;
    uint32_t target_job = kGdJobStoneMinerWalkHome;
    if (figure_task == CGdFigureTask::TASK_WORKER)
    {
        uint8_t race = _this->CGdFigure->figures[figure_id].race;
        target_building_type = getRacialSmallHQ(race);
        if (target_building_type != 0)
        {
            target_building = buildingAPI.findClosestBuilding(_this->CGdBuildingToolBox,
                                                              _this->CGdFigure->figures[figure_id].position,
                                                              target_building_type,
                                                              _this->CGdFigure->figures[figure_id].owner, 0x19);
        }
    }
    else
    {
        uint8_t race = _this->CGdFigure->figures[figure_id].race;
        target_building_type = buildingAPI.getRacialStonecutter(race);
        if (target_building_type != 0)
        {
            uint16_t building_index = _this->CGdFigure->figures[figure_id].building;
            //NO, there's no invariant here!
            target_building = buildingAPI.findClosestBuilding(_this->CGdBuildingToolBox,
                                                              _this->CGdBuilding->buildings[building_index].position,
                                                              target_building_type,
                                                              _this->CGdFigure->figures[figure_id].owner, 0x19);
        }
    }
    if (target_building != 0)
    {
        target_job = kGdJobStoneMinerWalkToDeliverGood;
    }
    else
    {
        if (figure_task == CGdFigureTask::TASK_WORKER)
        {
            uint16_t monument_id, unused;
            monument_id = toolboxAPI.findClosestMonument(_this->CGdObjectToolBox, figure_id, &unused);
            if (monument_id == 0)
            {
                figureAPI.setJob(_this, figure_id, job_id);
                return false;
            }
            figureAPI.prepareJobTransition(_this, figure_id, job_id, target_job);
            terminate_job_spells(_this, figure_id);
            figureAPI.onStartJob(_this, figure_id, target_job);
            setupWalkToMonument(_this, figure_id, monument_id);
            return true;
        }
        else
        {
            target_building = _this->CGdFigure->figures[figure_id].building;
        }
    }
    figureAPI.prepareJobTransition(_this, figure_id, job_id, target_job);
    terminate_job_spells(_this, figure_id);
    figureAPI.onStartJob(_this, figure_id, target_job);
    if (target_job == kGdJobStoneMinerWalkToDeliverGood)
    {
        buildingAPI.addFigureToBuilding(_this->CGdBuildingToolBox, target_building, figure_id, 0);
    }
    toolboxAPI.setFigureXData(_this->CGdFigureToolBox, figure_id, WORKER_DELIVERY_BUILDING, target_building);
    toolboxAPI.setFigureXData(_this->CGdFigureToolBox, figure_id, EFFECT_ENTITY_INDEX2, 0);
    SF_Coord walk_pos = _this->CGdBuilding->buildings[target_building].position;
    figureAPI.setFigureWalkPos(_this, figure_id, &walk_pos, 0, 1);

    return true;
}
