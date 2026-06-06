#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "../sf_general_structures.h"

typedef struct __attribute__((packed))
{
    uint16_t figure_index;
    uint16_t agro_value;
    uint16_t hate_value;
} CGdFigureHateEntry;

/**
 * @struct SF_SGtFigureAction
 */
typedef struct __attribute__((packed))
{
    uint16_t type;
    uint16_t subtype;
    uint16_t unkn1;
    uint16_t unkn2;
    uint16_t unkn3;
    uint16_t unkn4;
} SF_SGtFigureAction;


typedef struct __attribute__((packed))
{
    uint16_t min_dmg;
    uint16_t max_dmg;
    uint16_t min_rng;
    uint16_t max_rng;
    uint16_t wpn_spd;
    uint16_t wpn_type;
    uint16_t wpn_mat;
} SF_CGdFigureWeaponStats;

typedef struct __attribute__((packed))
{
    uint16_t base_val;
    int16_t bonus_val;
    int8_t bonus_multiplier;
    uint8_t statistic_type;  // NOT CONFIRMED
} FigureStatistic;

typedef struct __attribute__((packed))
{
    uint16_t base_val;
    int16_t bonus_val;
    int8_t bonus_multiplier;
    uint8_t statistic_type;  // NOT CONFIRMED?
    uint16_t missing_val;    // No idea what this is
} FigureStatisticExt;

typedef struct __attribute__((packed))
{
    uint16_t GdJobId;
    uint8_t task;
    uint8_t padding;
    uint16_t flags;
    uint8_t pathing_mode;
    SF_CGdTargetData target;  // it is stored WEIRDLY
    SF_SGtFigureAction action;
    uint16_t xdata_key;
    uint8_t walking_mode;
    uint8_t padding2;
    SF_Coord walk_to_pos;
    uint8_t unknown[14];
    uint8_t current_waypoint;
} FigureJobData;

typedef struct __attribute__((packed))
{
    SF_Coord position;
    SF_Coord destination;
    uint16_t to_do_count;
    uint16_t to_do_count_remainder;
    uint16_t anim_length;
    uint16_t anim_length_remainder;
    uint8_t activity;
    uint8_t retreat_related;
    uint16_t building;
    uint32_t flags; // packed with GdFigureFlags
    uint8_t race;
    uint8_t level;
    uint16_t owner;
    uint8_t min_attack_range;
    uint8_t max_attack_range;
    uint16_t master_figure;
    uint16_t group_leader;
    uint32_t npc_id;
    uint16_t unit_data_id;
    FigureStatistic armor;
    FigureStatistic agility;
    FigureStatistic charisma;
    FigureStatistic dexterity;
    FigureStatisticExt health;
    FigureStatistic intelligence;
    FigureStatisticExt mana;
    FigureStatisticExt stamina;
    FigureStatistic strength;
    FigureStatistic wisdom;
    FigureStatistic resistance_fire;
    FigureStatistic resistance_ice;
    FigureStatistic resistance_mental;
    FigureStatistic resistance_black;
    FigureStatistic walk_speed;
    FigureStatistic fight_speed;
    FigureStatistic cast_speed;
    uint16_t equipment[16];  // No idea how this works, may need a class (undefined2[16] in ghidra, but is 20 bytes FigureStatistic is 6 bytes)
    uint16_t head;           // Not sure what this does
    SF_SGtFigureAction actions[15];
    GdFigureAbility abilties[10];
    SF_CGdFigureWeaponStats weapon_stats[2];
    uint8_t check_battle_sleeping;
    uint8_t good;  // good that worker is carrying -- wood, stone, etc
    uint8_t direction;
    uint8_t last_direction;
    uint8_t path_dir;
    uint8_t path_wish_dir;
    uint32_t path_bits;
    uint8_t path_wait_counter;
    uint32_t path_data_related_ptrs[3];
    uint16_t spell_job_start_node;
    CGdFigureHateEntry hate_entry[10];
    uint8_t debug_flags;
    uint8_t rune_id;
    uint16_t formation;
    uint8_t faction;
    uint8_t faction_flags;
    uint32_t clan_relations;
    uint16_t scaling;
    uint8_t formation_walkspeed_mod;
    uint16_t creo;
    uint8_t anim_type;
    FigureJobData current_job; // Current Active Job Data
    uint8_t padding[3];
    FigureJobData next_job; // Feign Death, Acting in Buildings? Seems to be about the Next Target Job Data?
    uint8_t padding2[3];
    FigureJobData prev_job;
    uint8_t padding3[3];
    uint32_t incoming_spells_vector[3];  // Skipping some variable sections, contains differences Also Check what FigureJobData is, it's used multiple times in this section
    uint8_t dwarf_rank;
    uint8_t set_type;
} GdFigure;

/**
 * @brief A structure for the global list of figures and related statistics for them
 */
struct __attribute__((packed)) SF_CGdFigure
{
    uint16_t max_used;
    GdFigure figures[2000];
};

struct __attribute__((packed)) SF_CGdFigureJobs
{
    uint32_t *CGdAStar;
    uint32_t *CGdAiBattle;
    AutoClass14 *OpaqueClass;
    SF_CGdBuilding *CGdBuilding;
    SF_CGdBuildingToolbox *CGdBuildingToolBox;
    uint32_t *CGdDoubleLinkList;
    SF_CGdEffect *CGdEffect;
    uint32_t *AutoClass30;
    SF_CGdFigure *CGdFigure;
    SF_CGdFigureToolbox *CGdFigureToolBox;
    uint32_t *CGdFormation;
    uint32_t *AutoClass34;
    uint32_t *CGdObject;
    uint32_t *CGdObjectToolBox;
    SF_CGdPlayer *CGdPlayer;
    uint32_t *AutoClass46;
    uint32_t *AutoClass47;
    uint32_t *CGdResource;
    SF_CGdSpell *CGdSpell;
    uint32_t *AutoClass48;
    uint32_t *AutoClass22;
    uint32_t *AutoClass50;
    uint32_t *CGdVisibility;
    SF_CGdWorld *CGdWorld;
    SF_CGdWorldToolBox *CGdWorldToolBox;
    uint32_t *CGdXDataList;
    uint8_t padding[572];
    uint32_t noManaUsage;
    void *unkn1;
    void *unkn2;
    uint32_t padding2;
    void *unkn3;
    uint32_t padding3;
    void *unkn4;
    uint32_t padding4;
};

typedef struct __attribute__((packed))
{
    uint16_t creo; //offset 0
    uint8_t level; //offset 2

    uint8_t unknwn; //offset 3

    uint8_t race; //offset 4

    uint16_t agility; //offset 5
    uint16_t dexterity; //offset 7
    uint16_t charisma; //offset 9
    uint16_t intelligence; //offset 11
    uint16_t stamina; //offset 13
    uint16_t strength; //offset 15
    uint16_t wisdom; //offset 17

    uint16_t unknwn2; //offset 19

    uint16_t fres; //offset 21
    uint16_t ires; //offset 23
    uint16_t bres; //offset 25
    uint16_t mres; //offset 27

    uint16_t wspeed; //offset 29
    uint16_t fspeed; //offset 31
    uint16_t cspeed; //offset 33
    uint16_t scaling; //offset 35

    uint16_t unknwn3;
    uint32_t unknwn4;

    uint8_t unit_flags; //offset 43
    uint16_t head; //offset 44

    uint8_t unknwn5; //offset 46

    GdFigureAbility abilities[10]; //offset 47
} CGdResourceUnitStats;
