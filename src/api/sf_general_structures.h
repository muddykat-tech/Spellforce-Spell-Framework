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
} SFMod;

typedef void (*log_function_ptr)(const char *);
typedef struct __attribute__((packed))
{
    log_function_ptr logError;
    log_function_ptr logWarning;
    log_function_ptr logInfo;
} SFLog;

/* |-========== General Structures ==========-| */

typedef struct __attribute__((packed))
{
    uint16_t X;
    uint16_t Y;
} SF_Coord;

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
    uint32_t unknown1;
    uint32_t unknown2;
    uint32_t unknown3;
    uint16_t unknown4;
} CGdFigureWeaponStats;

typedef struct __attribute__((packed))
{
    uint16_t figure_index;
    uint16_t agro_value;
    uint16_t hate_value;
} CGdFigureHateEntry;

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
    SF_Coord position;
    SF_Coord destination;
    uint16_t to_do_count;
    uint16_t to_do_count_remainder;
    uint16_t anim_length;
    uint16_t anim_length_remainder;
    uint8_t activity;
    uint8_t unknown1;
    uint16_t building;
    uint32_t flags; // Actual Type CdFigureFlags
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
    uint8_t equipment[20]; // No idea how this works, may need a class (undefined2[16] in ghidra, but is 20 bytes FigureStatistic is 6 bytes)
    uint16_t head;         // Not sure what this does
    uint32_t unknown2[3];  // three 4 byte data points in a row, no name known for these.
    uint8_t unknown3[167]; // Many 1 byte sections in a row
    uint32_t unknown4[7];  // Many 4 byte sections in a row
    uint16_t unknown5;
    uint8_t unknown6[2];
    CGdFigureWeaponStats weapon_stats;
    uint8_t unknown7[12];
    uint8_t good; // I assume perhaps alignment?
    uint8_t direction;
    uint8_t unknown8;
    uint8_t path_dir;
    uint8_t path_wish_dir;
    uint32_t path_bits;
    uint8_t unknown9;
    uint32_t unknown10[3];
    uint16_t unknown11;
    CGdFigureHateEntry hate_entry;
    uint8_t debug_flags;
    uint8_t unknown12;
    uint16_t formation;
    uint8_t faction;
    uint8_t unknown13;
    uint32_t clan_relations;
    uint8_t unknown14[170]; // Skipping some variable sections, contains differences Also Check what AutoClass24 is, it's used multiple times in this section
    uint8_t dwarf_rank;
    uint8_t set_type;
} GdFigure;

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

/* |-========== Spell Start ==========-| */
typedef struct __attribute__((packed))
{
    uint16_t to_do_count;
    uint16_t spell_id;
    uint16_t spell_line; // aka spell type in shovel's editor
    uint16_t spell_job;  // aka spell line in older code
    SF_CGdTargetData source;
    SF_CGdTargetData target;
    uint8_t xdata_key; // IDK (Seems to be used in ref for things, seen usage in getting target data and others)
    uint8_t undefinded1;
    uint8_t DLLNode;
    uint8_t underfined2;
    uint8_t flags;
} SF_GdSpell;

typedef struct __attribute__((packed))
{
    void *SF_CGdAiMain;
    AutoClass14 *OpaqueClass; // For Random
    void *SF_CGdBuilding;
    void *SF_CGdBuildingToolbox;
    void *SF_CGdDoubleLinkedList;
    void *SF_CGdEffect;
    void *unkn1;
    SF_CGdFigure *SF_CGdFigure;
    void *SF_CGdFigureJobs;
    void *SF_CGdFigureToolBox;
    void *SF_CGdFormation;
    void *unkn2; // Seems to be used as first param for GetChanceToResistSpell
    void *SF_CGdObject;
    void *SF_CGdObjectToolBox;
    void *SF_CGdPlayer;
    void *SF_CGdResource;
    void *unkn3;
    void *unkn4;
    void *SF_CGdWorld;
    void *SF_CGdWorldToolBox;
    void *SF_CGdXDataList;
    uint16_t max_used;
    SF_GdSpell active_spell_list[800];
    void *unkn5;
    uint32_t unkn6;
} SF_CGdSpell;

/* |-========== Internal Structures ==========-| */

typedef struct __attribute__((packed))
{
    uint32_t vftable_ptr;
    uint8_t CAppMenu_data[0x688];
} CAppMenu;

typedef struct __attribute__((packed))
{
    uint32_t vftablePTR;
    uint8_t CMnuBase_data[0x208];
    uint8_t CMnuVisControl_data[0x9C];
    uint8_t CMnuLabel_data[0xc0];
} CMnuLabel;

typedef struct __attribute__((packed))
{
    uint32_t vftablePTR;
    uint8_t CMnuBase_data[0x208];
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

typedef enum
{
    EFFECT_EFFECT_INDEX = 0x06,
    EFFECT_SPELL_INDEX = 0x11,
    EFFECT_SPELL_ID = 0x09,
    EFFECT_SUBSPELL_ID = 0x1C,
    EFFECT_ENTITY_INDEX = 0x2F,
    EFFECT_ENTITY_INDEX2 = 0x1A,
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
typedef void(__thiscall *construct_default_sf_string_ptr)(SF_String *_this);

typedef void(__thiscall *construct_start_menu_ptr)(CUiStartMenu *_this, uint32_t p1);

typedef void(__thiscall *message_box_ptr)(uint32_t CMnuScreen_ptr, SF_String *string_ptr);

typedef void(__fastcall *original_menu_func_ptr)(uint32_t param1);

typedef void *(__cdecl *new_operator_ptr)(uint32_t param_1);
typedef void(__thiscall *container_add_control_ptr)(CMnuContainer *_this, void *CMnuBase, uint8_t c1, uint8_t c2, uint32_t p4);
typedef void(__thiscall *menu_label_set_data_ptr)(CMnuLabel *_this, uint32_t maybe_font, uint32_t maybe_color, uint32_t unkn1, uint8_t unknchar);

/* |-========== Macros ==========-| */
// Here comes a better method for setting up our exposed functions, to define functions also check sf_hooks.h
#define DECLARE_FUNCTION(type, name, ...) \
    typedef type(__thiscall *name##_ptr)(__VA_ARGS__);

#define DECLARE_FUNCTION_GROUP(group, ...) \
    typedef struct                         \
    {                                      \
        __VA_ARGS__                        \
    } group##Functions;
