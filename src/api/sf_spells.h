#pragma once
#include <stdint.h>

//XData Keys
typedef enum {
    SPELL_TICK_COUNT_AUX = 0x05,
    SPELL_TICK_COUNT = 0x12,
    SPELL_DOUBLE_DAMAGE = 0x26,
    SPELL_PESTILENCE_DAMAGE = 0x0E,
    EFFECT_EFFECT_INDEX = 0x06,
    SPELL_STAT_MUL_MODIFIER = 0x0A,
    SPELL_STAT_MUL_MODIFIER2 = 0x27,
    SPELL_STAT_MUL_MODIFIER3 = 0x2B,
    SPELL_STAT_MUL_MODIFIER4 = 0x2C,
    SPELL_CONSERVATION_SHIELD = 0x0B
} SpellDataKey;

typedef struct __attribute__((packed))
{
	uint16_t X;
	uint16_t Y;
} SF_Coord;

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
	uint8_t xdata_key; // IDK
	uint8_t undefinded1;
	uint8_t DLLNode;
	uint8_t underfined2; 
	uint8_t flags;
} SF_GdSpell;

typedef struct __attribute__((packed))
{
	uint8_t unkn1[12];
	uint32_t current_step;
	uint8_t unkn2[10];

} OpaqueClass1;

typedef struct __attribute__((packed))
{
	void *SF_CGdAiMain;
	OpaqueClass1 *OpaqueClass;
	void *SF_CGdBuilding;
	void *SF_CGdBuildingToolbox;
	void *SF_CGdDoubleLinkedList;
	void *SF_CGdEffect;
	void *unkn1;
	void *SF_CGdFigure;
	void *SF_CGdFigureJobs;
	void *SF_CGdFigureToolBox;
	void *SF_CGdFormation;
	void *unkn2;
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

typedef void (__thiscall *setXData_ptr)(SF_CGdSpell *, uint16_t, uint8_t, uint32_t);
typedef void (__thiscall *ConsolePrint_ptr)(uint32_t, SF_String*);
typedef uint16_t (__thiscall *get_spell_spell_line_ptr) (void *, uint16_t);
typedef uint32_t (__thiscall *figure_toolbox_get_unkn_ptr)(void *, uint16_t);
typedef void (__thiscall *figure_toolbox_add_spell_ptr)(void *, uint16_t, uint16_t);

typedef struct 
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

typedef void (__thiscall *initializeSpellDataFunc)(SF_CGdSpell* spell, uint16_t spell_id, SpellDataKey key);

typedef struct
{
	setXData_ptr setXData;
	initializeSpellDataFunc initializeSpellData;
} cgdspellfunctions;