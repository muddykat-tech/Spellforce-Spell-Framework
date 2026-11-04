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
    SF_CGdBuildingToolbox *CGdBuildingToolbox;
    void *CGdDoubleLinkedList;
    SF_CGdFigure *CGdFigure;
    SF_CGdFigureToolbox *CGdFigureToolBox;
    void *CGdAutoClass34;
    SF_CGdObject *CGdObject;
    SF_CGdResource *CGdResource;
    SF_CGdSpell *CGdSpell;
    void *CGdTile;
    void *AutoClass50;
    SF_CGdWorld *CGdWorld;
    SF_CGdWorldToolBox *CGdWorldToolBox;
    void *CGdXDataList;
    SF_GdEffect effects[2000];
    uint16_t max_used;
};
