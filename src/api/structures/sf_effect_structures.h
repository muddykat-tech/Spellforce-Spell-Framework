#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "../sf_general_structures.h"


struct __attribute__((packed)) SF_GdEffect
{
    uint16_t CGdEffectType;
    SF_CGdTargetData source;
    SF_CGdTargetData target;
    SF_Coord position;
    uint32_t start_step;
    uint32_t unkn1;
    uint16_t length;
    uint16_t xdata_key;
    uint16_t unkn2;
    uint16_t unkn3;
    uint16_t unkn4;
    uint16_t unkn5;
    SF_Coord position2;
};

struct __attribute__((packed)) SF_CGDEffect
{
    AutoClass14 *OpaqueClass;
    SF_CGdBuilding *CGdBuilding;
    SF_CGdBuildingToolbox *SF_CGdBuildingToolbox;
    void *SF_CGdDoubleLinkedList;
    SF_CGdFigure *SF_CGdFigure;
    SF_CGdFigureToolbox *SF_CGdFigureToolBox;
    void *AutoClass34;
    void *SF_CGdObject;
    void *SF_CGdResource;
    SF_CGdSpell *CGdSpell;
    void *AutoClass22;
    void *AutoClass50;
    SF_CGdWorld *SF_CGdWorld;
    SF_CGdWorldToolBox *SF_CGdWorldToolBox;
    void *SF_CGdXDataList;
    SF_GdEffect active_effect_list[2000];
    uint16_t max_used;
};
