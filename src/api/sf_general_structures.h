/**
 * @file sf_general_structures.h
 * @brief Group of Structures and Enums used throught SFSF
 * @ingroup API
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>

/* |-========== Mod Structures ==========-| */

typedef struct __attribute__((packed))
{
    char mod_id[64];
    char mod_version[24];
    char mod_description[128];
    char mod_author[128];
    char mod_errors[256]; //Large Buffer for all error reporting needs.
} SFMod;

typedef void (*log_function_ptr)(const char *);
typedef struct __attribute__((packed))
{
    log_function_ptr logError;
    log_function_ptr logWarning;
    log_function_ptr logInfo;
} SFLog;

typedef enum : uint16_t
{
    NONE = 0x0,
    SUMMON_SPELL = 0x1,
    DOMINATION_SPELL = 0x2,
    CHAIN_SPELL = 0x4,
    WHITE_AURA_SPELL = 0x8,
    BLACK_AURA_SPELL = 0x10,
    TARGET_ONHIT_SPELL = 0x20,
    COMBAT_ABILITY_SPELL = 0x40,
    AOE_SPELL = 0x80,
    SEIGE_AURA_SPELL = 0x100,
    AURA_SPELL = 0x200,
    STACKABLE_SPELL = 0x400,
    SPELL_TAG_COUNT = 12
} SpellTag;

/* |-========== General Structures ==========-| */

// Forward Declarations for looping struct declarations
typedef struct SF_CGdSpell SF_CGdSpell;
typedef struct SF_CGdFigureToolbox SF_CGdFigureToolbox;
typedef struct SF_GdEffect SF_GdEffect;
typedef struct SF_CGDEffect SF_CGDEffect;
typedef struct SF_CGdWorld SF_CGdWorld;
typedef struct SF_CGdWorldToolBox SF_CGdWorldToolBox;
typedef struct SF_CGdFigureJobs SF_CGdFigureJobs;
typedef struct SF_CGdBuilding SF_CGdBuilding;
typedef enum : uint8_t
{
    TASK_WORKER = 2,
    TASK_WOODCUTTER = 3,
    TASK_QUARRY = 4,
    TASK_MINE = 5,
    TASK_FORGE = 6,
    TASK_HERO = 9,
    TASK_MAINCHAR = 10,
    TASK_NPC = 11,
    TASK_PET = 12,
    TASK_HUNTING_LODGE = 14,
    TASK_MERCHANT = 17
} CGdFigureTask;

typedef enum
{
    PHASE_0 = 0,
    PHASE_1,
    PHASE_2,
    PHASE_3,
    PHASE_4,
    PHASE_5,
    OnHitEnd
} OnHitPhase;

// Game Formated them for bitwise operations, hence the magic numbers
typedef enum
{
    UNDEAD = 0x1,
    RESESRVED_ONLY = 0x2,
    AGGROED = 0x4,
    IS_DEAD = 0x8,
    REDO = 0x10,
    F_CHECK_SPELLS_BEFORE_JOB = 0x20,
    F_CHECK_SPELLS_BEFORE_CHECK_BATTLE = 0x40, // May need to be changed as it could be used inplace of a spell key perhaps?
    WALK_JOB_WAIT = 0x80,
    FREEZED = 0x100,
    HAS_LOOT = 0x200,
    HAS_DIALOG = 0x400,
    FEMALE = 0x800,
    GOT_AGGRO = 0x1000,
    RETREAT = 0x2000,
    NO_WAY_TO_TARGET = 0x4000,
    AURA_RUNNING = 0x8000,
    AI_BLOCKED = 0x10000,
    TOWER = 0x20000,
    IS_SWAPPING = 0x40000,
    CUR_ACTIVE_DIALOG = 0x80000,
    IS_IN_FIGHT = 0x100000,
    VIEW_MODE_1ST_3RD = 0x200000,
    IS_TALKING = 0x400000,
    IS_IMPORTANT_DIALOG = 0x800000,
    UNKILLABLE = 0x1000000,
    FOLLOW_MODE = 0x2000000,
    HIT_LEFT_HAND_NEXT = 0x4000000,
    FOREST_SPIRIT = 0x8000000,
    VIP = 0x10000000,
    ILLUSION = 0x20000000,
    SPAWN = 0x40000000,
    USED_FOR_REVENGE = 0x80000000
} GdFigureFlags;

typedef enum : uint16_t
{
    MANUAL_JOB_CHANGE = 1,
    SKIP_ONCE = 2,
    MANUAL_HIT_TARGET = 4,
    CORPSE_CANT_ROT = 8,
    START_WALK = 16,
    RUN_MODE = 32,
    WAR = 64,
    CHECK_BATTLE = 128,
    PATROL_MODE = 256,
    WAY_POINTS_READ_REVERSE = 512,
    SUPERIOR_PATHING = 1024,
    ROUND_HIT = 2048,
    DEATH_BLOW = 4096,
    START_WORK_AT_BUILDING_FORCE_JOB = 8192,
} CGdFigureJobFlags;

typedef struct __attribute__((packed))
{
    uint16_t X;
    uint16_t Y;
} SF_Coord;

typedef struct __attribute__((packed))
{
    uint32_t R;
    uint32_t G;
} SF_RGColor;

typedef struct __attribute__((packed))
{
    uint16_t spell_id;
    uint16_t job_id;
} SF_SpellEffectInfo;

typedef struct __attribute__((packed))
{
    uint8_t entity_type; // 1 for figure, 2 for building, 3 for object
    uint16_t entity_index;
    SF_Coord position;
} SF_CGdTargetData;

typedef struct __attribute__((packed))
{
    void *raw_data;
    uint32_t str_length;
    uint32_t unknown_length_var;
    char *data;
} SF_String;

typedef struct __attribute__((packed))
{
    uint32_t partA;
    uint32_t partB;
} SF_Rectangle;

/* |-========== Figure Start ==========-| */

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
    uint16_t bonus_val;
    uint8_t bonus_multiplier;
    uint8_t statistic_type; // NOT CONFIRMED
} FigureStatistic;

typedef struct __attribute__((packed))
{
    uint16_t base_val;
    uint16_t bonus_val;
    uint8_t bonus_multiplier;
    uint8_t statistic_type; // NOT CONFIRMED?
    uint16_t missing_val;   // No idea what this is
} FigureStatisticExt;

typedef struct __attribute__((packed))
{
    uint16_t type;
    uint16_t subtype;
    uint16_t unkn2;
    uint16_t unkn3;
    uint16_t unkn4;
    uint16_t unkn5;
} SF_SGtFigureAction;

typedef struct __attribute__((packed))
{
    uint16_t GdJobId;
    CGdFigureTask task;
    uint8_t padding;
    CGdFigureJobFlags flags;
    uint8_t pathing_mode;
    SF_CGdTargetData target; // it is stored WEIRDLY
    uint8_t SGtFigureAction[0xc];
    uint16_t xdata_key;
    uint8_t walking_mode;
    uint8_t padding2;
    SF_Coord walk_to_pos;
    uint8_t unknown[14];
    uint8_t current_waypoint;
} AutoClass24;

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
    GdFigureFlags flags;
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
    FigureStatisticExt mana_stuff;
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
    uint16_t equipment[16]; // No idea how this works, may need a class (undefined2[16] in ghidra, but is 20 bytes FigureStatistic is 6 bytes)
    uint16_t head;          // Not sure what this does
    uint32_t unknown2[3];   // three 4 byte data points in a row, no name known for these.
    uint8_t unknown3[168];  // Many 1 byte sections in a row
    uint32_t unknown4[7];   // Many 4 byte sections in a row
    uint16_t unknown5;
    uint8_t unknown6[2];
    SF_CGdFigureWeaponStats weapon_stats;
    uint8_t unknown7[13];
    uint8_t good; // I assume perhaps alignment?
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
    AutoClass24 ac_1;
    uint8_t padding2[3];
    AutoClass24 ac_2;
    uint8_t padding3[3];
    AutoClass24 ac_4;
    uint8_t padding4[3];
    uint32_t unknown15[3]; // Skipping some variable sections, contains differences Also Check what AutoClass24 is, it's used multiple times in this section
    uint8_t dwarf_rank;
    uint8_t set_type;
} GdFigure;

/**
 * @brief A structure for the global list of figures and related statistics for them
 */
typedef struct __attribute__((packed))
{
    uint16_t max_used;
    GdFigure figures[2000];
} SF_CGdFigure;

/* |-========== AutoClass Start ==========-| */
// These classes are currently only partially understood, and are not fully annotated

typedef struct __attribute__((packed))
{
    uint32_t *ac69_ptr1;
    uint32_t *ac69_ptr2;
    uint32_t *ac69_ptr3;
    uint8_t unkn1;
    uint8_t unkn2;
    uint16_t figure_index;
} AutoClass69;

// This class is required for the RANDOM function, AutoClass14 also seems to hold some relevance to the game world as well
// This class is initialized in the GameInit function, so it is likely very important.
typedef struct __attribute__((packed))
{
    uint16_t unknown_field_0;
    uint16_t unknown_field_2;
    uint16_t unknown_field_4;
    uint8_t unknown1[6];
    uint32_t current_step;
    uint8_t unknown_field_10;
    uint8_t unknown2[3];
    uint32_t unknown_field_14;
    uint8_t unknown_field_18;
    uint8_t unknown_field_19;
} AutoClass14;

typedef struct __attribute__((packed))
{
    uint16_t uknwn1;
    uint16_t uknwn2;
    uint16_t uknwn3;
    uint8_t uknwn4;
} SF_world_unkn_1;

typedef struct __attribute__((packed))
{
    uint16_t world_cell_flags;
    uint16_t sector;
    uint8_t uknwn2;
    uint8_t tile_type;
} SF_world_unkn_2;

typedef struct __attribute__((packed))
{
    uint8_t uknwn1[10];
    uint8_t block_value;
    uint8_t uknwn2[3];
} SF_world_unkn_3;

typedef struct __attribute__((packed))
{
    SF_Rectangle rect;
    uint8_t unkn1;
} SF_world_unkn_4;

/* |-========== Spell Start ==========-| */
typedef struct __attribute__((packed))
{
    uint16_t to_do_count;
    uint16_t spell_id;
    uint16_t spell_line; // aka spell type in shovel's editor
    uint16_t spell_job;  // aka spell line in older code
    SF_CGdTargetData source;
    SF_CGdTargetData target;
    uint16_t xdata_key; // IDK (Seems to be used in ref for things, seen usage in getting target data and others)
    uint16_t DLLNode;
    uint8_t flags;
} SF_GdSpell;

struct __attribute__((packed)) SF_CGdFigureJobs
{
    uint32_t *CGdAStar;
    uint32_t *CGdAiBattle;
    AutoClass14 *OpaqueClass;
    SF_CGdBuilding *CGdBuilding;
    uint32_t *CGdBuildingToolBox;
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

struct __attribute__((packed)) SF_CGdFigureToolbox
{
    uint32_t *CGdAIBattle;
    uint32_t *CGdAIMain;
    uint32_t *CGdAStar;
    AutoClass14 *maybe_random; // Unconfirmed
    SF_CGdBuilding *CGdBuilding;
    uint32_t *CGdBuildingToolbox;
    uint32_t *CGdDoubleLinkedList;
    SF_CGDEffect *CGdEffect;
    uint32_t *autoclass30;
    SF_CGdFigure *CGdFigure;
    uint32_t *CGdFigureJobs;
    uint32_t *CGdFormation;
    uint32_t *autoclass34;
    uint32_t *autoclass50;
    uint32_t *CGdInfluenceMap;
    uint32_t *autoclass36;
    uint32_t *CGdObject;
    uint32_t *CGdObjectToolBox;
    uint32_t *CGdPlayer;
    uint32_t *autoclass46;
    uint32_t *CGdResource;
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

struct __attribute__((packed)) SF_CGdWorld
{
    SF_world_unkn_1 unknown1[10001];
    SF_String map_name;
    uint16_t map_size;
    uint8_t unknown2;
    SF_world_unkn_2 cells[1048576];
    uint16_t heightmap[1048576];
    uint16_t unknown3;
    uint8_t unknown4[131070];
    SF_world_unkn_4 unknown5[100];
    uint16_t change_count;
    SF_world_unkn_3 unknown6[255];
    uint8_t unknown7[63];
};

struct __attribute__((packed)) SF_CGdWorldToolBox
{
    uint32_t *CGdAStar;
    AutoClass14 *OpaqueClass; // Unconfirmed
    SF_CGdBuilding *CGdBuilding;
    uint32_t *CGdBuildingToolbox;
    uint32_t *CGdDoubleLinkedList;
    SF_CGDEffect *CGdEffect;
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
    uint32_t uknown1; // 4x uint8_t
    uint32_t unknown2;
};

struct __attribute__((packed)) SF_CGdSpell
{
    void *SF_CGdAiMain;
    AutoClass14 *OpaqueClass; // For Random
    SF_CGdBuilding *CGdBuilding;
    void *SF_CGdBuildingToolbox;
    void *SF_CGdDoubleLinkedList;
    SF_CGDEffect *SF_CGdEffect;
    void *unkn1;
    SF_CGdFigure *SF_CGdFigure;
    void *SF_CGdFigureJobs;
    SF_CGdFigureToolbox *SF_CGdFigureToolBox;
    void *SF_CGdFormation;
    void *unkn2; // Seems to be used as first param for GetChanceToResistSpell
    void *SF_CGdObject;
    void *SF_CGdObjectToolBox;
    void *SF_CGdPlayer;
    void *SF_CGdResource;
    void *unkn3;
    void *unkn4;
    SF_CGdWorld *SF_CGdWorld;
    SF_CGdWorldToolBox *SF_CGdWorldToolBox;
    void *SF_CGdXDataList;
    uint16_t max_used;
    SF_GdSpell active_spell_list[800];
    void *unkn5;
    uint32_t unkn6;
};

struct __attribute__((packed)) SF_GdEffect
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
};

struct __attribute__((packed)) SF_CGDEffect
{
    AutoClass14 *OpaqueClass;
    SF_CGdBuilding *CGdBuilding;
    void *SF_CGdBuildingToolbox;
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
/* |-============= Buildings Structures ==========- |*/

typedef struct __attribute__((packed))
{
    SF_Coord position;
    uint16_t owner;
    uint8_t type;
    uint8_t race;
    uint8_t flags;
    uint16_t build_up;
    uint16_t build_max;
    uint16_t health_current;
    uint16_t life_current_max;
    uint16_t health_max;
    uint16_t DLLNode;
    uint16_t xdata_key;
    uint16_t angle;
    uint32_t unknown[5];
    uint8_t worker_count;
    uint8_t worker_count_max;
    uint8_t level;
    uint32_t unknown2;
} GdBuilding;

struct __attribute__((packed)) SF_CGdBuilding
{
    AutoClass14 *autoClass14;
    GdBuilding buildings[1000];
    uint16_t max_used;
    void *astruct_31;
    uint32_t unknown1;
    void *astruct_39;
    uint32_t unknown2;
};


/* |-========== Internal Structures ==========-| */

typedef struct __attribute__((packed))
{
    uint8_t font_data[0x1fa0];
} SF_Font;

typedef struct __attribute__((packed))
{
    SF_Font *smth_font[32];
    uint8_t unkn_data[0x8];
} SF_FontStruct;

typedef struct __attribute__((packed))
{
    uint32_t R;
    uint32_t G;
    uint32_t B;
} SF_Color;

typedef struct __attribute__((packed))
{  
    uint32_t unkn;
    uint32_t param_1_callback;
    uint32_t param_2_callback;
    uint32_t param_3_callback;
    uint8_t data[0x118];
    uint32_t parent_ptr;
    uint8_t data2[0xdc];
} CMnuBase_data;

typedef struct __attribute__((packed))
{
    uint32_t vftable_ptr;
    CMnuBase_data data;
} CMnuBase;

typedef struct __attribute__((packed))
{
    uint32_t vftablePTR;
    uint8_t unknown_data2[0xcc];
} CUiOption;

typedef struct __attribute__((packed))
{
    uint32_t vftablePTR;
    CMnuBase_data CMnuBase_data;
    uint8_t CMnuVisControl_data[0x9C];
    uint8_t CMnuLabel_data[0xc0];
} CMnuLabel;


typedef struct __attribute__((packed))
{
    uint8_t toggle; // toggle used to track if a button is enabled or not.
    uint32_t index;  // Used to hold the index of loaded mods for the showmod page
    CMnuLabel *title_label; // Title Label 
    CMnuLabel *desc_label; // Description Label 
    CMnuLabel *page_label; // Page Index Label 
    CMnuLabel *error_label; // Error Info Label 
} SFSF_ModlistStruct;

typedef struct __attribute__((packed))
{
    uint32_t vtable_ptr;
    uint32_t param_ptr;
    uint32_t callback_func;
} CUtlCallback2;

typedef struct __attribute__((packed))
{
    uint8_t padding_start[0x13];
    uint32_t button_ptr;
    uint8_t padding_end[0x119];
} CMnuButton_data;

typedef struct __attribute__((packed))
{
    uint32_t vftablePTR;                    
    CMnuBase_data CMnuBase_data;            
    uint8_t CMnuVisControl_data[0x9c];
    CMnuButton_data CMnuButton_data;
    uint8_t CMnuSmpButton_data[0x50];
} CMnuSmpButton;

typedef struct __attribute__((packed))
{
    uint32_t vftablePTR;
    CMnuBase_data CMnuBase_data;
    uint8_t CMnuVisControl_data[0x9C];
    uint8_t CMnuContainer_data[0x98];
} CMnuContainer;

typedef struct __attribute__((packed))
{
    uint8_t unkn1[0x8];
    uint8_t offset_0x8;
    uint8_t unkn2[0x3];
    uint32_t offset_0xc;
    uint32_t offset_0x10;
    uint8_t offset_0x14;
    uint8_t unkn3[0x3];
    uint32_t offset_0x18;
    uint32_t offset_0x1c;
    uint8_t unknown[0x44];
    SF_String unknown_string;
} data_CUiStartMenu;

typedef struct __attribute__((packed))
{
    uint32_t vftablePTR;
    uint8_t CMnuBase_data[0x208];
    uint8_t CMnuVisControl_data[0x9C];
    uint8_t CMnuContainer_data[0x98];
    data_CUiStartMenu CUiStartMenu_data;
} CUiStartMenu;

typedef struct __attribute__((packed))
{
    uint16_t actionType_id;
    uint16_t actionSubtype_id;
    uint8_t unknown_data[12];
    uint8_t unknown_flag;
    uint8_t padding_maybe;
    uint8_t unknown_config_param;
} SF_UIElement;

typedef struct __attribute__((packed))
{
    uint8_t CUiMain_data[0x8658];
} CUiMain_data;

typedef struct __attribute__((packed))
{
    uint32_t CUiMain_cftable_ptr;
    CMnuBase_data CMnuBaseData;
    uint8_t CMnuVisControl_data[0x9C];
    uint8_t CMnuContainer_data[0x98];
    CUiMain_data CUiMain_data;
} SF_CUiMain;

typedef struct __attribute__((packed))
{
    uint8_t unkn[4];
    uint32_t offset_0x4;
    uint32_t offset_0x8;
    SF_String string1;
    SF_String string2;
    uint32_t offset_0x2c;
} SF_CUiVideoSequence_data;

typedef struct __attribute__((packed))
{
    uint32_t vtable_ptr;
    CMnuBase_data baseData;
    uint8_t CMnuVisControl_data[0x9C];
    uint8_t CMnuContainer_data[0x98];
    SF_CUiVideoSequence_data videoData;
} SF_CUiVideoSequence;

typedef struct __attribute__((packed))
{
    uint32_t vtable_ptr;
    CMnuBase_data baseData;
    uint8_t CMnuVisControl_data[0x9C];
    uint8_t CMnuContainer_data[0x98];
    uint8_t videoData[0xc];
} SF_CUiVideo;

typedef struct __attribute__((packed))
{
    uint8_t data[0x28];
    SF_CUiVideoSequence* CUiVideoSequence_ptr;
    CMnuBase* CMnuBase_ptr;
    uint8_t more_data[0x658];
} CAppMenu_data;

typedef struct __attribute__((packed))
{
    uint32_t vftable_ptr;
    CAppMenu_data CAppMenu_data;
} CAppMenu;

typedef enum
{
    EFFECT_EFFECT_INDEX = 0x06,
    EFFECT_SPELL_INDEX = 0x11,
    EFFECT_SPELL_ID = 0x09,
    EFFECT_ENTITY_INDEX2 = 0x1A,
    EFFECT_SUBSPELL_ID = 0x1C,
    EFFECT_PHYSICAL_DAMAGE = 0x1E,
    EFFECT_ENTITY_INDEX = 0x2F,
    EFFECT_ENTITY_INDEX3 = 0x2D,
    EFFECT_ENTITY_TYPE = 0x30,
    EFFECT_ENTITY_TYPE2 = 0x13,
    EFFECT_DO_NOT_ADD_SUBSPELL = 0x33,
    SPELL_TICK_COUNT_AUX = 0x05,
    SPELL_TICK_COUNT = 0x12,
    SPELL_DOUBLE_DAMAGE = 0x26,
    SPELL_TARGET = 0x33,
    SPELL_PESTILENCE_DAMAGE = 0x0E,
    SPELL_STAT_MUL_MODIFIER = 0x0A,
    SPELL_STAT_MUL_MODIFIER2 = 0x27,
    SPELL_STAT_MUL_MODIFIER3 = 0x2B,
    SPELL_STAT_MUL_MODIFIER4 = 0x2C,
    SPELL_CONSERVATION_SHIELD = 0x0B
} SpellDataKey;

typedef enum
{
    PRE,
    DEFAULT,
    POST,
    COUNT
} SpellDamagePhase;

typedef enum
{
    kGdEffectNone = 0,
    kGdEffectSpellCast = 1,
    kGdEffectSpellHitWorld = 2,
    kGdEffectSpellHitTarget = 3,
    kGdEffectSpellDOTHitTarget = 4,
    kGdEffectSpellMissTarget = 5,
    kGdEffectSpellResolve = 6,
    kGdEffectSummonWorker = 7,
    kGdEffectWorkerAppears = 8,
    kGdEffectSummonHero = 9,
    kGdEffectHeroAppears = 10,
    kGdEffectSpellTargetResisted = 11,
    kGdEffectSpellResolveSelf = 12,
    kGdEffectMeteorFall = 13,
    kGdEffectMeteorHit = 14,
    kGdEffectBlizzardFall = 15,
    kGdEffectBlizzardHit = 16,
    kGdEffectStoneFall = 17,
    kGdEffectStoneHit = 18,
    kGdEffectPetAppears = 19,
    kGdEffectTest = 20,
    kGdEffectMonumentClaimed = 21,
    kGdEffectMonumentWorking = 22,
    kGdEffectAuraResolve = 23,
    kGdEffectProjectile = 24,
    kGdEffectBuilding = 25,
    kGdEffectPlayerBind = 26,
    kGdEffectSummonMainChar = 27,
    kGdEffectMainCharAppears = 28,
    kGdEffectTitanProduction = 29,
    kGdEffectTitanAppears = 30,
    kGdEffectMentalTowerCast = 31,
    kGdEffectMentalTowerIdle = 32,
    kGdEffectMonumentBullet = 33,
    kGdEffectMonumentHitFigure = 34,
    kGdEffectSpellAssistanceHitFigure = 35,
    kGdEffectChainResolve = 36,
    kGdEffectSpellVoodooHitFigure = 37,
    kGdEffectSpellManaShieldHitFigure = 38,
    kGdEffectMax = 39
} CGdEffectType;

/* |-========== Internal Functions ==========-| */
// These functions are used in SFSF internally, and may be moved.

typedef void(__thiscall *console_print_ptr)(uint32_t, const char *);
typedef uint16_t(__thiscall *get_spell_spell_line_ptr)(void *, uint16_t);
typedef uint32_t(__thiscall *figure_toolbox_get_unkn_ptr)(void *, uint16_t);
typedef void(__thiscall *figure_toolbox_add_spell_ptr)(void *, uint16_t, uint16_t);
typedef bool(__thiscall *figure_toolbox_is_targetable_ptr)(void *CGdFigureToolbox, uint16_t figure_index);
typedef uint32_t(__thiscall *FUN_0069eaf0_ptr)(void *ac69, void *ac69_2, void *ac69_3, void *ac69_4);
typedef void (*fidfree_ptr)(uint32_t *memory_ptr);
typedef void(__thiscall *menu_label_ptr)(CMnuLabel *_this);
typedef void(__thiscall *menu_label_set_string_ptr)(CMnuLabel *_this, SF_String *string);
typedef void(__thiscall *initialize_menu_container_ptr)(CMnuContainer *_this);
typedef SF_String *(__thiscall *construct_default_sf_string_ptr)(SF_String *_this);

typedef void(__thiscall *construct_start_menu_ptr)(CUiStartMenu *_this, uint32_t p1);

typedef void(__thiscall *mnu_label_init_data_ptr)(void *_this, float xpos, float ypos, float width, float height, SF_String *string);
typedef void(__thiscall *message_box_ptr)(uint32_t CAppMenu, uint16_t description_id, SF_String *string_ptr, uint16_t hasOffset);

typedef void(__thiscall *menu_label_constructor_ptr)(CMnuLabel *_this);
typedef void(__thiscall *set_label_flags_ptr)(CMnuLabel *_this, uint32_t flags);

typedef void(__fastcall *original_menu_func_ptr)(uint32_t param1);

typedef void *(__cdecl *new_operator_ptr)(uint32_t param_1);
typedef void(__thiscall *container_add_control_ptr)(CMnuContainer *_this, CMnuBase *CMnubase, char c1, char c2, uint32_t p4);
typedef void(__thiscall *menu_label_set_data_ptr)(CMnuLabel *_this, uint32_t color_red, uint32_t color_green, uint32_t color_blue, uint8_t unknchar);
typedef void(__thiscall *get_sf_color_ptr)(SF_String *_this, uint32_t color_id);
typedef SF_FontStruct *(__thiscall *get_smth_fonts_ptr)(void);
typedef SF_Font *(__thiscall *get_font_ptr)(SF_FontStruct *_this, uint32_t font_id);
typedef void(__thiscall *menu_label_set_font_ptr)(void *_this, SF_Font *font);
typedef CUiOption* (__thiscall *create_option_ptr)(CUiOption *_this);

// SF Menu / GUI Hooks for Auras
typedef void(__thiscall *autoclass113_fun_00a27530_ptr)(void *_this_autoclass113);
typedef void(__thiscall *fun_0086dd60_ptr)(void *_this,uint8_t p1, uint32_t p2);
typedef void(__thiscall *autoclass113_fun_00a278c0_ptr)(void *_this_autoclass113, uint32_t p1);

// Weird __cdecl class here, blind passthrough and hope it works.
typedef uint16_t(__cdecl *fun_00a2a1d0_ptr)(uint32_t *some_ptr, void *some_ptr_2);

typedef uint32_t(__thiscall *fun_006a0140_ptr)(void *_this, uint16_t p1, uint32_t p2_ptr, uint8_t p3, uint8_t p4);
typedef void(__thiscall *fun_009a2790_ptr)(void* _p1, uint32_t p2);
typedef uint32_t(__thiscall *fun_0069f8d0_ptr)(void* _this, uint32_t p1);
typedef uint32_t(__thiscall *fun_0069fb90_ptr)(void* _this, uint16_t figure_id, uint8_t po2, uint16_t unkn_p3, SF_CGdTargetData *data, uint32_t p5, uint32_t p6);

typedef uint32_t(__thiscall *cuiVideoSequence_constructor_ptr)(SF_CUiVideo* _this, SF_String *p1);
typedef void(__thiscall *CMnuScreen_attach_control_ptr)(void *_CMnuScreen_ptr, CMnuBase* base, char flag);

extern void attachVideo(CAppMenu * CAppMenu_ptr,  CMnuContainer *parent, char *video_loc_and_name_charsC);
extern CMnuLabel * __thiscall attach_new_meshed_label(CMnuLabel *label_ptr,CMnuContainer *parent, char *mesh_name, char *label_text, uint8_t font_index, uint16_t x_pos, uint16_t y_pos, uint16_t width, uint16_t height);
extern CMnuLabel * __thiscall attach_new_label(CMnuLabel *label_ptr,CMnuContainer *parent, char *label_text, uint8_t font_index, uint16_t x_pos, uint16_t y_pos, uint16_t width, uint16_t height);
extern void __thiscall attach_new_button(CMnuContainer *parent, char *button_mesh_default, char *button_mesh_pressed, char *button_mesh_highlight, char *button_mesh_disabled,  char *label_char, uint8_t font_index, uint16_t x_pos, uint16_t y_pos, uint16_t width, uint16_t height, int button_index, uint32_t callback_function_pointer);

extern void __fastcall show_mod_list_callback(CMnuSmpButton *button, int32_t* cui_menu_ptr_maybe);

extern SFSF_ModlistStruct mod_struct;

/**
 * @brief Declares a function with the specified return type, name, and arguments.
 * 
 * This macro is used to define functions that we use for hooking into the game and to expose these to the API.
 * we use it to cut down on boilerplate code typedefs and structures.
 * 
 * @param return_type The return type of the function.
 * @param function_name The name of the function.
 * @param ... The function parameters and types.
 */
#define DECLARE_FUNCTION(type, name, ...) \
    typedef type(__thiscall *name##_ptr)(__VA_ARGS__);
