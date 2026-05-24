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
    void *SF_CGdAiMain;
    AutoClass14 *OpaqueClass;  // For Random (Rename to Global Tick?)
    SF_CGdBuilding *CGdBuilding;
    SF_CGdBuildingToolbox *SF_CGdBuildingToolbox;
    void *SF_CGdDoubleLinkedList;
    SF_CGDEffect *SF_CGdEffect;
    void *AC30;
    SF_CGdFigure *SF_CGdFigure;
    SF_CGdFigureJobs *SF_CGdFigureJobs;
    SF_CGdFigureToolbox *SF_CGdFigureToolBox;
    void *SF_CGdFormation;
    void *AutoClass34;  // Seems to be used as first param for GetChanceToResistSpell
    SF_CGdObject *SF_CGdObject;
    void *SF_CGdObjectToolBox;
    SF_CGdPlayer *CGdPlayer;
    void *SF_CGdResource;
    void *SF_CGdTile;
    void *AC50;
    SF_CGdWorld *SF_CGdWorld;
    SF_CGdWorldToolBox *SF_CGdWorldToolBox;
    void *SF_CGdXDataList;
    uint16_t max_used;
    SF_GdSpell active_spell_list[800];
    void *unkn5;
    uint32_t unkn6;
};
