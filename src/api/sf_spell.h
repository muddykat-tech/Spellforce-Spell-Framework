#pragma once
#include <stdint.h>

typedef struct __attribute__((packed))
{
	uint8_t X;
	uint8_t Y;
} SF_Coord;

typedef struct __attribute__((packed))
{
	uint8_t entity_type; //1 for figure, 2 for building, 3 for object
	uint8_t entity_index; 
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
	void *SF_CGdMain;
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