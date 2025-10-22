#include "sf_building_done_handlers.h"
#include "../core/sf_wrappers.h"

void __thiscall default_done_handler (SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    return;
}

// woodcutters, quaries, farms. BUT NOT MINES?!
void __thiscall multiworker_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.setBuildingXData(_this, building_index, SpellDataKey::BUILDING_POSITION_X,
                                 _this->CGdBuilding->buildings[building_index].position.X);
    buildingAPI.setBuildingXData(_this, building_index, SpellDataKey::BUILDING_POSITION_Y,
                                 _this->CGdBuilding->buildings[building_index].position.Y);
}

//resource doubling stuff: foodstores, smelteries, stonecutters, sawmills
void __thiscall singleworker_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.setBuildingXData(_this, building_index, SpellDataKey::BUILDING_QUEUE_COUNT, 0);
    buildingAPI.setBuildingXData(_this, building_index, SpellDataKey::BUILDING_ANIM_START, 0);
    buildingAPI.setBuildingXData(_this, building_index, SpellDataKey::BUILDING_ANIM_LENGTH, 0);
}

void __thiscall breeder_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    //it has different meaning in context of buildings. IDK which
    buildingAPI.setBuildingXData(_this, building_index, SpellDataKey::SPELL_TICK_COUNT_AUX, 0);
    buildingAPI.setBuildingXData(_this, building_index, SpellDataKey::SPELL_PESTILENCE_DAMAGE, 0);
}

void __thiscall army_worker_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.setBuildingXData(_this, building_index, SpellDataKey::SPELL_PESTILENCE_DAMAGE, 0);
}

void __thiscall human_crossbow_tower_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x341);
}

void __thiscall human_white_hand_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x33a);
}

void __thiscall elf_archer_tower_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x342);
}

void __thiscall elf_frostbringer_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x339);
}

void __thiscall orc_firestarter_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x48f);
}

void __thiscall troll_stonethrower_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x550);
}

void __thiscall norcaine_sorcery_tower_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x54e);
}

void __thiscall norcaine_mindbreaker_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
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
                                                 _this->OpaqueClass->current_step, 0, &rect);
    if (effect_index != 0)
    {
        buildingAPI.setBuildingXData(_this, building_index, SpellDataKey::BUILDING_EFFECT_ID, effect_index);
    }
}

void __thiscall npc_stonethrower_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x682);
}

void __thiscall npc_crossbow_tower_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x3d9);
}

void __thiscall wulfgar_crossbow_tower_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x430);
}

void __thiscall wulfgar_crossbow_tower_done_handler2(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x48b);
}

void __thiscall npc_firestarter_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x490);
}

void __thiscall undead_bone_tower_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x259);
}

void __thiscall npc_white_hand_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x51d);
}

void __thiscall blade_black_tower_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x551);
}

void __thiscall npc_spitflower_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x552);
}

void __thiscall npc_frostbringer_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x55a);
}

void __thiscall blade_black_tower_done_handler2(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x572);
}

void __thiscall blade_dark_crystal_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x581);
}

void __thiscall blade_dark_mananode_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x582);
}

void __thiscall npc_mindbreaker_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x5e7);
}

void __thiscall npc_holy_statue_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x62e);
}

void __thiscall npc_sun_tower_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x66f);
}

void __thiscall npc_moon_tower_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x670);
}

void __thiscall npc_spitflower_done_handler2(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x73d);
}

void __thiscall npc_sorcery_tower_done_handler(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x787);
}

void __thiscall npc_sorcery_tower_done_handler2(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x9b6);
}

void __thiscall undead_bone_tower_done_handler2(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x9b7);
}

void __thiscall undead_bone_tower_done_handler3(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x9b8);
}

void __thiscall npc_firestarter_done_handler2(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x9b9);
}

void __thiscall blade_black_tower_done_handler3(SF_CGdBuildingToolbox *_this,uint16_t building_index)
{
    buildingAPI.attachTowerUnit(_this, building_index, 0x9ba);
}
