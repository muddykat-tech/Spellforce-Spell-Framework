#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "sf_general_structures.h"

typedef struct __attribute__((packed))
{
    uint16_t *first;
    uint16_t *data;
    uint16_t *post_last;
} ushort_list_node;

typedef struct __attribute((packed))
{
    uint8_t data[0xfa0];
    uint16_t entityCount;
} AutoClass60;

typedef struct __attribute__((packed))
{
    SF_CGdBuilding *CGdBuilding;
    void *CGdBuildingToolBox;
    void *autoclass30;
    SF_CGdFigure *CGdFigure;
    SF_CGdFigureJobs *CGdFigureJobs;
    SF_CGdFigureToolbox *CGdFigureToolBox;
    void *autoclass34;
    void *CGdFormation;
    void *CGdDoubleLinkList;
    void *CGdInfluenceMap;
    void *CGdObject;
    void *CGdObjectToolBox;
    void *CGdPlayer;
    void *CGdResource;
    SF_CGdSpell *CGdSpell;
    void *autoclass22;
    void *autoclass50;
    void *CGdVisibility;
    SF_CGdWorld *CGdWorld;
    SF_CGdWorldToolBox *CGdWorldToolBox;
    void *autoclass64;
    AutoClass60 figures_maybe;
    uint16_t unkn1;
    AutoClass60 figure_maybe2;
    uint16_t unkn2;
    AutoClass60 building_ally;
    uint16_t unkn3;
    AutoClass60 building_not_ally;
    uint16_t unkn4;
    uint32_t unkn5;
    SF_SGtFigureAction current_action;
    uint16_t current_action_min_rng;
    uint16_t current_action_max_rng;
    SF_String world_name;
    uint32_t can_be_target_maybe;
    uint16_t unkn6;
    uint8_t current_figure_sight_range_maybe;
    uint8_t unkn7;
    uint32_t figures_missing_hp;
    uint32_t current_figure_is_aggroed;
    uint32_t current_figure_is_not_mainchar_or_hero;
    uint32_t current_figure_has_amok;
    uint32_t unkn_flag1;
    uint32_t current_figure_is_mainchar;
    uint32_t current_figure_is_hero;
    uint32_t current_figure_has_owner;
    uint32_t manual_hit_target_maybe;
    uint32_t current_figure_is_tower;
    uint32_t current_figure_is_warrior;
    uint32_t something_related_to_ranking;
    uint8_t current_figure_target_type;
    uint16_t current_figure_target_index;
    SF_Coord current_figure_target_position;
    uint8_t unkn8;
    uint32_t current_hate;
    uint32_t current_ranking;
    uint8_t unkn_list[0xfa2];
    uint16_t unkn9;
    uint32_t current_figure_noaggroattack;
    uint32_t is_map_arena3;
    ushort_list_node some_figure_list[2000];
    ushort_list_node unkn_node;
    ushort_list_node another_figure_list[2000];
    uint32_t unkn_array[2000];
    uint16_t current_figure;
    SF_Coord current_figure_pos;
    uint16_t unkn10;
    uint32_t current_figure_is_combat_involved_maybe;
    uint32_t current_figure_has_aggro;
    uint32_t current_figure_has_demoralization;
    uint32_t unkn_flag2;
    uint32_t figures_max_hp;
    SF_CGdTargetData current_source_maybe;
    SF_CGdTargetData current_target;
    uint16_t unkn11;
    uint32_t force_process_figure_maybe;
    SF_CGdTargetData unknown_target;
    uint8_t unkn12;
    uint32_t some_ranking;
    uint32_t current_figure_is_pet;
    uint16_t current_figure_master;
    uint8_t unkn13;
    uint8_t unkn14;
    uint32_t figure_is_charmed_summon_maybe;
    uint32_t unkn_flag3;
    uint32_t current_target_level_possibly;
    uint32_t action_is_siege_aura;
} CGdAIBattleData;

typedef struct __attribute__((packed))
{
    void *vfTable;
    void *battleFactory;
    CGdAIBattleData BattleData;
} SF_CGdBattleDevelopment;

DECLARE_FUNCTION(SF_SGtFigureAction *, getTargetAction, SF_CGdFigure *figure, SF_SGtFigureAction *action, uint16_t figure_id);
DECLARE_FUNCTION(SF_SGtFigureAction *, getFigureAction, SF_CGdFigure *figure, SF_SGtFigureAction *action, uint16_t figure_id, uint8_t action_index);
DECLARE_FUNCTION(void, setAICurrentActionRanking, SF_CGdBattleDevelopment *_this, int rank);
DECLARE_FUNCTION(int, getAICurrentActionRanking, SF_CGdBattleDevelopment *_this);

DECLARE_FUNCTION(bool, canFigureDoAction, SF_CGdBattleDevelopment *_this, SF_SGtFigureAction *action);
DECLARE_FUNCTION(void, getActionStats, SF_CGdBattleDevelopment *_this, uint16_t *minRange, uint16_t *maxRange, SF_CGdResourceSpell *spellData);
// May be moved to General if it is not used anywhere else
DECLARE_FUNCTION(bool, isAIVectorEmpty, void *_this);
DECLARE_FUNCTION(uint16_t **, getAIVectorFirstElement, void *_this, uint16_t **pvalue);
DECLARE_FUNCTION(uint16_t **, getAIVectorGetCurrent, void *_this, uint16_t **value);
DECLARE_FUNCTION(int, getAIVectorLength, void *_this);
DECLARE_FUNCTION(uint16_t, getCastType, void* resource, uint16_t spellID);
DECLARE_FUNCTION(void *, AC60AddOrGetEntity, void *_autoclass60, uint16_t entity_index);
DECLARE_FUNCTION(SF_Coord *, getPositionToCastAlly, SF_CGdBattleDevelopment *_this,SF_Coord *param_1,void *std_vector_uint16_t);
DECLARE_FUNCTION(SF_Coord *, getPositionToCastEnemy, SF_CGdBattleDevelopment *_this,SF_Coord *param_1,void *std_vector_uint16_t);

DECLARE_FUNCTION(bool, isUnknownWorldFeature, SF_CGdWorldToolBox *, uint16_t figure_index, SF_Coord * param2, SF_Coord *param3, uint32_t param4, SF_Coord * param5, uint16_t param6);

DECLARE_FUNCTION_GROUP(Ai,
                       getTargetAction_ptr getTargetAction;
                       getFigureAction_ptr getFigureAction;
                       canFigureDoAction_ptr canFigureDoAction;
                       setAICurrentActionRanking_ptr setAICurrentActionRanking;
                       getAICurrentActionRanking_ptr getAICurrentActionRanking;
                       isAIVectorEmpty_ptr isAIVectorEmpty;
                       getAIVectorFirstElement_ptr getAIVectorFirstElement;
                       getAIVectorGetCurrent_ptr getAIVectorGetCurrent;
                       getAIVectorLength_ptr getAIVectorLength;
                       AC60AddOrGetEntity_ptr AC60AddOrGetEntity;
                       getActionStats_ptr getActionStats;
                       getCastType_ptr getCastType;
                       getPositionToCastAlly_ptr getPositionToCastAlly;
                       getPositionToCastEnemy_ptr getPositionToCastEnemy;
                       isUnknownWorldFeature_ptr isUnknownWorldFeature;
                       );