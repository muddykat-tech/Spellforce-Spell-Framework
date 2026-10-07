#pragma once
#include "../core/sf_hero_loader.h"


void registerHeroGear(uint16_t creo_id, HeroGearEntry *gear_list, uint8_t list_len);
bool getHeroGear(uint16_t creo_id, HeroGearEntry *out_gear_list);
