#include "sf_building_done_hook.h"
#include <stdlib.h>

extern BuildingFunctions buildingAPI;
typedef void (__thiscall *fun006c3650_ptr)(void *autoclass30, uint16_t unkn, uint16_t index);
typedef void (__thiscall *fun0074ca30_ptr)(uint32_t *god_object, uint32_t owner, uint32_t flag);
typedef uint32_t *(__thiscall *AC65_init_ptr)(void *autoclass65);

typedef uint16_t (__thiscall *XDataListExists_ptr)(void *SF_CGdXDataList, uint16_t index, uint8_t xdata_type);
typedef uint16_t (__thiscall *XDataListZero_ptr)(void *SF_CGdXDataList, uint16_t xdata_key, uint8_t xdata_type);
typedef void *(__cdecl *op_new_ptr)(uint32_t size);

AC65_init_ptr l_AC65_init;
fun006c3650_ptr l_fun006c3650;
fun0074ca30_ptr l_fun0074ca30;
XDataListZero_ptr XDataListZero;
XDataListExists_ptr XDataListExists;
op_new_ptr op_new;

void initialize_building_done_hooks()
{
    l_fun006c3650 = (fun006c3650_ptr)ASI::AddrOf(0x2c3650);
    l_AC65_init = (AC65_init_ptr)ASI::AddrOf(0x34c810);
    l_fun0074ca30 = (fun0074ca30_ptr)ASI::AddrOf(0x34ca30);
    XDataListZero = (XDataListZero_ptr) ASI::AddrOf(0x353ef0);
    XDataListExists = (XDataListExists_ptr) ASI::AddrOf(0x3549d0);
    op_new = (op_new_ptr)ASI::AddrOf(0x675A9D);
}

void include_owner_on_map(uint16_t owner, uint16_t building_index)
{
    uint32_t *object_ptr = (uint32_t *)ASI::AddrOf(0x00949fec);
    if (*object_ptr == 0)
    {

        uint32_t *temp_alloc = (uint32_t *)op_new(8);
        if (temp_alloc != 0)
        {
            *object_ptr = l_AC65_init(temp_alloc);
        }
    }
    l_fun0074ca30((uint32_t *)*object_ptr, building_index, 1);
}

void __thiscall sf_building_done_hook(SF_CGdBuildingToolbox *_this, uint16_t building_index)
{
    l_fun006c3650(_this->AutoClass30, 3, building_index);
    _this->CGdBuilding->buildings[building_index].build_up = 0;
    _this->CGdBuilding->buildings[building_index].flags |= 1;
    if (_this->CGdBuilding->buildings[building_index].owner != 0)
    {
        include_owner_on_map(_this->CGdBuilding->buildings[building_index].owner, building_index);
    }
    uint16_t building_xdata_key = _this->CGdBuilding->buildings[building_index].xdata_key;
    if (XDataListExists(_this->CGdXDataList, building_xdata_key, SpellDataKey::SPELL_CONSERVATION_SHIELD))
    {
        uint16_t value = XDataListZero(_this->CGdXDataList, building_xdata_key,
                                       SpellDataKey::SPELL_CONSERVATION_SHIELD);
        if (value != 0xffff)
        {
            _this->CGdBuilding->buildings[building_index].xdata_key = value;
        }
    }

    uint8_t building_type = _this->CGdBuilding->buildings[building_index].type;

    building_done_handler_ptr handler = get_building_done_handler (building_type);
    handler(_this, building_index);
}
