#include "sf_building_entry_hook.h"
#include "../../handlers/sf_worker_building_entry_handlers.h"


extern BuildingFunctions buildingAPI;

typedef void (__thiscall *fun006c3ca0_ptr)(void *autoclass30, uint16_t owner,
                                           SF_CGdTargetData *source, SF_CGdTargetData *target);


bool isBuildingDamaged(SF_CGdBuilding *_this, uint16_t building_id)
{
    return (_this->buildings[building_id].health_current != _this->buildings[building_id].health_max);
}

void __thiscall job_start_working_at_building_hook (SF_CGdFigureJobs *_this, uint16_t figure_id)
{
    uint16_t building_id = _this->CGdFigure->figures[figure_id].building;
    if (((_this->CGdBuilding->buildings[building_id].flags & 0x1) == 0) ||
        (isBuildingDamaged(_this->CGdBuilding, building_id) &&
         (_this->CGdFigure->figures[figure_id].current_job.flags & 0x4000) == 0))
    {
        building_repair(_this,figure_id, building_id);
        return;
    }
    _this->CGdFigure->figures[figure_id].current_job.flags &= ~(0x4000);
    uint16_t owner = _this->CGdFigure->figures[figure_id].owner;
    SF_CGdTargetData source = {0, 0, {0, 0}};
    SF_CGdTargetData target = {1, figure_id, {0, 0}};
    fun006c3ca0_ptr fun006c3ca0 = (fun006c3ca0_ptr)ASI::AddrOf(0x2c3ca0);
    fun006c3ca0(_this->AutoClass30, owner, &source, &target);
    uint8_t building_type = _this->CGdBuilding->buildings[building_id].type;
    building_entry_handler_ptr handler = get_building_entry_handler(building_type);
    handler(_this, figure_id, building_id);
}
