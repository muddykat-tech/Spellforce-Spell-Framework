#pragma once
#include <stdint.h>

typedef enum
{
    HERO_FIELD_CREO = 0,
    HERO_FIELD_SLOT,
    HERO_FIELD_ITEM_ID,
    HERO_FIELD_GEAR,
    HERO_FIELD_UNKNOWN,
} HeroFieldKey;

#define MAX_SLOTS 7

typedef struct __attribute__((packed))
{
    uint8_t slot;
    uint16_t item_id;
} HeroGearEntry;


typedef struct
{
    uint16_t creo_id;
    HeroGearEntry gear[MAX_SLOTS];
    uint8_t item_count;
} CustomHero;

bool parse_hero_json_file(const char *path, CustomHero *out_hero);
