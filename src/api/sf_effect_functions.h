#pragma once
#include "sf_general_structures.h"

typedef struct __attribute__((packed))
{
    uint16_t CGdEffectType;
    SF_CGdTargetData source;
    SF_CGdTargetData target;
    SF_Coord position;
    uint32_t start_step;
    uint32_t unkn1;
    uint16_t length;
    uint16_t xdata_key; // IDK (Seems to be used in ref for things, seen usage in getting target data and others)
    uint16_t unkn2;
    uint16_t unkn3;
    uint16_t unkn4;
    uint16_t unkn5;
    SF_Coord position2;
} SF_GdEffect;

typedef struct __attribute__((packed))
{
    AutoClass14 *OpaqueClass;
    void *SF_CGdBuilding;
    void *SF_CGdBuildingToolbox;
    void *SF_CGdDoubleLinkedList;
    SF_CGdFigure *SF_CGdFigure;
    void *SF_CGdFigureToolBox;
    void *AutoClass34;
    void *SF_CGdObject;
    void *SF_CGdResource;
    void *CGdSpell;
    void *AutoClass22;
    void *AutoClass50;
    void *SF_CGdWorld;
    void *SF_CGdWorldToolBox;
    void *SF_CGdXDataList;
    SF_GdEffect active_effect_list[2000];
    uint16_t max_used;
} SF_CGDEffect;

DECLARE_FUNCTION(void, setEffectXData, SF_CGDEffect *_this, SpellDataKey key, uint32_t value);
DECLARE_FUNCTION(uint32_t, getEffectXData, SF_CGDEffect *_this, uint16_t effect_index, SpellDataKey key);
DECLARE_FUNCTION(uint16_t, addEffect, SF_CGDEffect *_this, CGdEffectType effect_id, SF_CGdTargetData *source,
                 SF_CGdTargetData *target, uint32_t tick_start, uint16_t tick_count, SF_Rectangle *param_6);
DECLARE_FUNCTION_GROUP(Effect,
                       setEffectXData_ptr setEffectXData;
                       addEffect_ptr addEffect;
                       getEffectXData_ptr getEffectXData;);