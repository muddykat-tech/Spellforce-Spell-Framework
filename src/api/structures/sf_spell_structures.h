#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "../sf_general_structures.h"

struct __attribute__((packed)) SF_SpellEffectInfo
{
    uint16_t spell_id;
    uint16_t job_id;
};

typedef struct __attribute__((packed))
{
    uint16_t to_do_count;
    uint16_t spell_id;
    uint16_t spell_line;
    uint16_t spell_job;
    SF_CGdTargetData source;
    SF_CGdTargetData target;
    uint16_t xdata_key;
    uint16_t DLLNode;
    uint8_t flags;
} SF_GdSpell;

struct __attribute__((packed)) SF_CGdSpell
{
    void *CGdAiMain;
    AutoClass14 *OpaqueClass;  // For Random (Rename to Global Tick?)
    SF_CGdBuilding *CGdBuilding;
    SF_CGdBuildingToolbox *CGdBuildingToolbox;
    void *CGdDoubleLinkedList;
    SF_CGdEffect *CGdEffect;
    void *CGdAutoClass30;
    SF_CGdFigure *CGdFigure;
    SF_CGdFigureJobs *CGdFigureJobs;
    SF_CGdFigureToolbox *CGdFigureToolBox;
    void *CGdFormation;
    void *CGdAutoClass34;  // Seems to be used as first param for GetChanceToResistSpell
    SF_CGdObject *CGdObject;
    void *CGdObjectToolBox;
    SF_CGdPlayer *CGdPlayer;
    SF_CGdResource *CGdResource;
    void *CGdTile;
    void *CGdAutoClass50;
    SF_CGdWorld *CGdWorld;
    SF_CGdWorldToolBox *CGdWorldToolBox;
    void *CGdXDataList;
    uint16_t max_used;
    SF_GdSpell active_spell_list[800];
    void *unkn5;
    uint32_t unkn6;
};
