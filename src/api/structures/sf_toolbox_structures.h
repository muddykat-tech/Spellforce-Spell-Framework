#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "../sf_general_structures.h"

struct __attribute__((packed)) SF_CGdFigureToolbox
{
    uint32_t *CGdAIBattle;
    uint32_t *CGdAIMain;
    uint32_t *CGdAStar;
    AutoClass14 *maybe_random;
    SF_CGdBuilding *CGdBuilding;
    SF_CGdBuildingToolbox *CGdBuildingToolbox;
    uint32_t *CGdDoubleLinkedList;
    SF_CGdEffect *CGdEffect;
    uint32_t *autoclass30;
    SF_CGdFigure *CGdFigure;
    SF_CGdFigureJobs *CGdFigureJobs;
    uint32_t *CGdFormation;
    uint32_t *autoclass34;
    uint32_t *autoclass50;
    uint32_t *CGdInfluenceMap;
    uint32_t *autoclass36;
    uint32_t *CGdObject;
    uint32_t *CGdObjectToolBox;
    SF_CGdPlayer *CGdPlayer;
    uint32_t *autoclass46;
    SF_CGdResource *CGdResource;
    SF_CGdSpell *CGdSpell;
    uint32_t *autoclass22;
    uint32_t *CGdVisibility;
    SF_CGdWorld *CGdWorld;
    SF_CGdWorldToolBox *CGdWorldToolBox;
    uint32_t *CGdXDataList;
    uint32_t *undefined4_1;
    uint32_t *undefined4_2;
    uint32_t *undefined;
};

struct __attribute__((packed)) SF_CGdWorldToolBox
{
    uint32_t *CGdAStar;
    AutoClass14 *OpaqueClass;  // Unconfirmed
    SF_CGdBuilding *CGdBuilding;
    uint32_t *CGdBuildingToolbox;
    uint32_t *CGdDoubleLinkedList;
    SF_CGdEffect *CGdEffect;
    SF_CGdFigure *CGdFigure;
    uint32_t *CGdFigureJobs;
    uint32_t *autoclass47;
    uint32_t *CGdObject;
    uint32_t *CGdObjectToolBox;
    uint32_t *CGdResource;
    uint32_t *autoclass22;
    SF_CGdWorld *SF_CGdWorld;
    uint32_t *CGdXDataList;
    uint32_t data[10000];
    uint32_t uknown1;  // 4x uint8_t
    uint32_t unknown2;
};
