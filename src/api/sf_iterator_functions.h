#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "sf_general_structures.h"

typedef struct __attribute__((packed))
{
	uint32_t offset_0x0;
	uint32_t offset_0x4;
	uint16_t offset_0x8;
	uint16_t offset_0xa;
	uint16_t offset_0xc;
	uint16_t offset_0xe;
	uint16_t offset_0x10;
	uint16_t offset_0x12;
	uint16_t offset_0x14;
	uint16_t offset_0x16;
	uint16_t offset_0x18;
	uint16_t offset_0x1a;
	uint32_t offset_0x1c;
	uint16_t offset_0x20;
	uint32_t offset_0x22;
	uint8_t offset_0x26;
	uint8_t offset_0x27;
	uint32_t offset_0x28;
	uint16_t offset_0x2c;
	uint8_t offset_0x2e;
	uint8_t offset_0x2f;
	AutoClass69 offset_0x30;
} CGdTileIterator_data;

typedef struct __attribute__((packed))
{
	uint32_t ftable_ptr;
	CGdTileIterator_data data;
} CGdFigureIterator;

typedef struct __attribute__((packed))
{
	uint32_t btable_ptr;
	CGdTileIterator_data data;
} CGdBuildingIterator;

//Declare the function pointers for IteratorFunctions group
DECLARE_FUNCTION(void, figureIteratorInit, CGdFigureIterator *iterator, uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end);
DECLARE_FUNCTION(void, figureIteratorSetPointers, CGdFigureIterator *iterator, SF_CGdFigure * figure, void * AutoClass22, void * CGdWorld);
DECLARE_FUNCTION(void, iteratorSetArea, CGdFigureIterator *iterator, SF_Coord *position, uint16_t radius);
DECLARE_FUNCTION(uint16_t, getNextFigure, CGdFigureIterator *iterator);
DECLARE_FUNCTION(void, setup_figure_iterator, CGdFigureIterator *iterator, SF_CGdSpell *spell);
DECLARE_FUNCTION(void, disposeFigureIterator, CGdFigureIterator iterator);

DECLARE_FUNCTION(void, buildingIteratorInit, CGdBuildingIterator *iterator, uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end);
DECLARE_FUNCTION(void, buildingIteratorSetPointers, CGdBuildingIterator *iterator, void *SF_CGdBuilding, void *AutoClass22, void *CGdWorld);

DECLARE_FUNCTION(uint16_t, getNextBuilding, CGdBuildingIterator * _this);


// Functions in this group may be renamed soon
DECLARE_FUNCTION_GROUP(Iterator,
    figureIteratorInit_ptr figureIteratorInit;
	figureIteratorSetPointers_ptr figureIteratorSetPointers;
	iteratorSetArea_ptr iteratorSetArea;
	getNextFigure_ptr getNextFigure;
	setup_figure_iterator_ptr setup_figure_iterator; 
	disposeFigureIterator_ptr disposeFigureIterator;

	getNextBuilding_ptr getNextBuilding;
	buildingIteratorInit_ptr buildingIteratorInit;
	buildingIteratorSetPointers_ptr buildingIteratorSetPointers;
);
