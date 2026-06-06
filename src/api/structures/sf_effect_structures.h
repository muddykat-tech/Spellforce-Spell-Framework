#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "../sf_general_structures.h"


struct __attribute__((packed)) SF_GdEffect
{
    uint16_t type;
    SF_CGdTargetData source;
    SF_CGdTargetData target;
    SF_Coord position;
    uint32_t start_step;
    uint32_t unkn1;
    uint16_t length;
    uint16_t xdata_key;
    SF_Rectangle rect;
    SF_Coord position2;
};

struct __attribute__((packed)) SF_CGdEffect
{
    AutoClass14 *OpaqueClass;
    SF_CGdBuilding *CGdBuilding;
    SF_CGdBuildingToolbox *SF_CGdBuildingToolbox;
    void *SF_CGdDoubleLinkedList;
    SF_CGdFigure *SF_CGdFigure;
    SF_CGdFigureToolbox *SF_CGdFigureToolBox;
    void *AutoClass34;
    SF_CGdObject *SF_CGdObject;
    SF_CGdResource *SF_CGdResource;
    SF_CGdSpell *CGdSpell;
    void *SF_CGdTile;
    void *AutoClass50;
    SF_CGdWorld *SF_CGdWorld;
    SF_CGdWorldToolBox *SF_CGdWorldToolBox;
    void *SF_CGdXDataList;
    SF_GdEffect effects[2000];
    uint16_t max_used;
};
