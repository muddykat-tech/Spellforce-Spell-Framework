#pragma once
#include <stdint.h>
#include <stdbool.h>

//XData Keys
typedef enum {
    SPELL_TICK_COUNT_AUX = 0x05,
    SPELL_TICK_COUNT = 0x12,
    SPELL_DOUBLE_DAMAGE = 0x26,
	SPELL_TARGET = 0x33,
    SPELL_PESTILENCE_DAMAGE = 0x0E,
    EFFECT_EFFECT_INDEX = 0x06,
    SPELL_STAT_MUL_MODIFIER = 0x0A,
    SPELL_STAT_MUL_MODIFIER2 = 0x27,
    SPELL_STAT_MUL_MODIFIER3 = 0x2B,
    SPELL_STAT_MUL_MODIFIER4 = 0x2C,
    SPELL_CONSERVATION_SHIELD = 0x0B
} SpellDataKey;

typedef enum {
	ARMOR,
	AGILITY,
	CHARISMA,
	DEXTERITY,
	HEALTH,
	INTELLIGENCE,
	MANA_STUFF,
	STAMINA,
	STRENGHT,
	WISDOM,
	RESISTANCE_FIRE,
	RESISTANCE_ICE,
	RESISTANCE_MENTAL,
	RESISTANCE_BLACK,
	WALK_SPEED,
	FLIGHT_SPEED,
	CAST_SPEED,
} StatisticDataKey;
 
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
	uint8_t entity_type; //1 for figure, 2 for building, 3 for object
	uint16_t entity_index;
	SF_Coord position;
} SF_CGdTargetData;

typedef struct __attribute__((packed))
{
	uint16_t to_do_count;
	uint16_t spell_id;
	uint16_t spell_line; //aka spell type in shovel's editor
	uint16_t spell_job; //aka spell line in older code
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
	uint16_t missing_val; // No idea what this is
} FigureStatisticExt;

typedef struct __attribute__((packed)){
	uint32_t unknown1;
	uint32_t unknown2;
	uint32_t unknown3;
	uint16_t unknown4;
} CGdFigureWeaponStats;

typedef struct __attribute__((packed)){
	uint16_t figure_index;
	uint16_t agro_value;
	uint16_t hate_value;
} CGdFigureHateEntry;

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
	uint32_t flags; //Actual Type CdFigureFlags
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
	FigureStatistic flight_speed;
	FigureStatistic cast_speed;
	uint8_t equipment[20]; // No idea how this works, may need a class (undefined2[16] in ghidra, but is 20 bytes FigureStatistic is 6 bytes)
	uint16_t head; //Not sure what this does
	uint32_t unknown2[3]; // three 4 byte data points in a row, no name known for these.
	uint8_t unknown3[167]; // Many 1 byte sections in a row 
	uint32_t unknown4[7]; // Many 4 byte sections in a row
	uint16_t unknown5;
	uint8_t	unknown6[2];
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
	void *unkn2; //Seems to be used as first param for GetChanceToResistSpell 
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

typedef struct __attribute__((packed))
{
    void *raw_data;
    uint32_t str_length;
    uint32_t unknown_length_var;
    char *data;
} SF_String;

typedef void (__thiscall *ConsolePrint_ptr)(uint32_t, SF_String*);
typedef uint16_t (__thiscall *get_spell_spell_line_ptr) (void *, uint16_t);
typedef uint32_t (__thiscall *figure_toolbox_get_unkn_ptr)(void *, uint16_t);
typedef void (__thiscall *figure_toolbox_add_spell_ptr)(void *, uint16_t, uint16_t);
typedef bool (__thiscall *figure_toolbox_is_targetable_ptr)(void *CGdFigureToolbox, uint16_t figure_index);

typedef struct __attribute__((packed))
{
	uint16_t spell_id;
	uint16_t spell_line_id;
	uint8_t skill_requirements[12];
	uint16_t mana_cost;
	uint32_t cast_time;
	uint32_t recast_time;
	uint16_t min_range;
	uint16_t max_range;
	uint8_t cast_type1;
	uint8_t cast_type2;
	uint32_t params[10];
	uint16_t effect_power;
	uint16_t effect_range;

} SF_CGdResourceSpell;

// Here comes a better method for setting up our exposed functions, when need to define function, check sf_hooks.h
#define DECLARE_FUNCTION(type, name, ...) \
    typedef type (__thiscall *name##_ptr)(__VA_ARGS__);

#define DECLARE_FUNCTION_GROUP(group, ...) \
    typedef struct { \
        __VA_ARGS__ \
    } group##Functions;

// Declare the function pointers for the FigureFunctions group
DECLARE_FUNCTION(bool, isAlive, SF_CGdFigure* figure, uint16_t target);
DECLARE_FUNCTION(bool, setWalkSpeed, SF_CGdFigure* figure, uint16_t target, uint16_t value);
DECLARE_FUNCTION(bool, addAction, SF_CGdFigure* figure, uint16_t target, void* maybe_action);
DECLARE_FUNCTION(void, addBonusMultToStatistic, SF_CGdFigure* figure, StatisticDataKey key, uint16_t target, uint8_t value);
DECLARE_FUNCTION(uint8_t, addBonusMult, FigureStatistic statistic, uint8_t value);

// Declare the function pointers for the SpellFunctions group
DECLARE_FUNCTION(void, setXData, SF_CGdSpell * _this, uint16_t, uint8_t, uint32_t);
DECLARE_FUNCTION(void, initializeSpellData, SF_CGdSpell* _this, uint16_t spell_id, SpellDataKey key);
DECLARE_FUNCTION(void, setEffectDone, SF_CGdSpell* _this, uint16_t spell_id, uint16_t param_2);
DECLARE_FUNCTION(uint32_t, addToXDataList, void* , uint16_t, SpellDataKey, uint32_t);
DECLARE_FUNCTION(uint32_t, getChanceToResistSpell, void*, uint16_t, uint16_t, SF_SpellEffectInfo);
DECLARE_FUNCTION(uint16_t, getRandom, void*, uint16_t);
DECLARE_FUNCTION(void, addVisualEffect, SF_CGdSpell* _this, uint16_t spell_index, uint16_t effect_id, void * unused, SF_CGdTargetData *target, uint32_t tick_start, uint16_t tick_count, void* param7)
DECLARE_FUNCTION(void, figureAggro, SF_CGdSpell *_this, uint16_t spell_index, uint16_t target_index);
DECLARE_FUNCTION(SF_CGdResourceSpell*, getResourceSpellData, void *, SF_CGdResourceSpell* spellData, uint16_t index);

// Declare the function pointers for the ToolboxFunctions group
DECLARE_FUNCTION(void, dealDamage, void* CGdFigureToolbox, uint16_t, uint16_t, uint32_t, uint32_t, uint32_t, uint32_t);
DECLARE_FUNCTION(bool, isTargetable, void * CGdFigureToolbox, uint16_t figure_index);

// We define the call name and the ptr it uses, I hope to fine a better way to deal with this.
DECLARE_FUNCTION_GROUP(Figure,
	isAlive_ptr isAlive;
	setWalkSpeed_ptr setWalkSpeed;
	addAction_ptr addAction;
	addBonusMult_ptr addBonusMult;
	addBonusMultToStatistic_ptr addBonusMultToStatistic;
);

DECLARE_FUNCTION_GROUP(Spell,
	setXData_ptr setXData;
	initializeSpellData_ptr initializeSpellData;
	setEffectDone_ptr setEffectDone;
	addToXDataList_ptr addToXDataList;
	getChanceToResistSpell_ptr getChanceToResistSpell;
	getRandom_ptr getRandom;
	getResourceSpellData_ptr getResourceSpellData;
	addVisualEffect_ptr addVisualEffect;
	figureAggro_ptr figureAggro;
);

DECLARE_FUNCTION_GROUP(Toolbox,
	dealDamage_ptr dealDamage;
	isTargetable_ptr isTargetable;
);
