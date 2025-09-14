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
    CGdFigureTask task;
    uint8_t padding;
    CGdFigureJobFlags flags;
    uint8_t pathing_mode;
    SF_CGdTargetData target;  // it is stored WEIRDLY
    uint8_t SGtFigureAction[0xc];
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
    uint8_t unknown1;
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
    uint32_t unknown2[3];    // three 4 byte data points in a row, no name known for these.
    uint8_t unknown3[168];   // Many 1 byte sections in a row
    uint32_t unknown4[7];    // Many 4 byte sections in a row
    uint16_t unknown5;
    SF_CGdFigureWeaponStats weapon_stats[2];
    uint8_t padding0;
    uint8_t good;  // I assume perhaps alignment?
    uint8_t direction;
    uint8_t unknown8;
    uint8_t path_dir;
    uint8_t path_wish_dir;
    uint32_t path_bits;
    uint8_t unknown9;
    uint32_t unknown10[3];
    uint16_t unknown11;
    CGdFigureHateEntry hate_entry[10];
    uint8_t debug_flags;
    uint8_t unknown12;
    uint16_t formation;
    uint8_t faction;
    uint8_t unknown13;
    uint32_t clan_relations;
    uint16_t unknown14;
    uint32_t padding1;
    FigureJobData current_job; // Current Active Job Data
    uint8_t padding2[3];
    FigureJobData next_job; // Feign Death, Acting in Buildings? Seems to be about the Next Target Job Data?
    uint8_t padding3[3];
    FigureJobData ac_4;
    uint8_t padding4[3];
    uint32_t unknown15[3];  // Skipping some variable sections, contains differences Also Check what FigureJobData is, it's used multiple times in this section
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
    SF_CGDEffect *CGdEffect;
    uint32_t *AutoClass30;
    SF_CGdFigure *CGdFigure;
    SF_CGdFigureToolbox *CGdFigureToolBox;
    uint32_t *CGdFormation;
    uint32_t *AutoClass34;
    uint32_t *CGdObject;
    uint32_t *CGdObjectToolBox;
    uint32_t *CGdPlayer;
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
