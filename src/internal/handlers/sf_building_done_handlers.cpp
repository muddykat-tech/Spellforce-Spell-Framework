#include "sf_building_done_handlers.h"
#include "../core/sf_wrappers.h"

void default_done_handler (SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    return;
}

// woodcutters, quaries, farms. BUT NOT MINES?!
void multiworker_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.setBuildingXData(_this, building_index, SpellDataKey::BUILDING_POSITION_X, 0);
    buildingAPI.setBuildingXData(_this, building_index, SpellDataKey::BUILDING_POSITION_Y, 0);
}

//resource doubling stuff: foodstores, smelteries, stonecutters, sawmills
void singleworker_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.setBuildingXData(_this, building_index, SpellDataKey::BUILDING_QUEUE_COUNT, 0);
    buildingAPI.setBuildingXData(_this, building_index, SpellDataKey::BUILDING_ANIM_START, 0);
    buildingAPI.setBuildingXData(_this, building_index, SpellDataKey::BUILDING_ANIM_LENGTH, 0);
}

void breeder_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    //it has different meaning in context of buildings. IDK which
    buildingAPI.setBuildingXData(_this, building_index, SpellDataKey::SPELL_TICK_COUNT_AUX, 0);
    buildingAPI.setBuildingXData(_this, building_index, SpellDataKey::SPELL_PESTILENCE_DAMAGE, 0);
}

void army_worker_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.setBuildingXData(_this, building_index, SpellDataKey::SPELL_PESTILENCE_DAMAGE, 0);
}

void mindbreaker_done(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x528);
    SF_CGdTargetData source;
    source.entity_index = building_index;
    source.entity_type = 2;
    source.position = {0,0};
    SF_CGdTargetData target;
    target.entity_index = building_index;
    target.entity_type = 2;
    target.position = {0,0};
    SF_Rectangle rect = {0,0};

    uint16_t effect_index =  effectAPI.addEffect(_this->CGdEffect, kGdEffectMentalTowerIdle,&source,&target,
                                                 _this->OpaqueClass.current_step, 0, &rect);
    if (effect_index != 0)
    {
        buildingAPI.setBuildingXData(_this, building_index, SpellDataKey::BUILDING_EFFECT_ID, effect_index);
    }
}
