#pragma once
#include "../core/sf_hero_loader.h"

/**
 * Registry of custom hero gear applied by createHeroFromRuneHook.
 *
 * Lifecycle mirrors the ability registry: population
 * (registerHeroGear*, duplicate creo_id entries are replaced with a logged
 * conflict, the last registration wins - an empty gear list from a file clears
 * the gear of a vanilla hero) -> reads (getHeroGear). The registry needs no
 * finalize step, std::map is ordered.
 */
void registerHeroGear(uint16_t creo_id, HeroGearEntry *gear_list, uint8_t list_len);
void registerHeroGearFromSource(uint16_t creo_id, HeroGearEntry *gear_list, uint8_t list_len,
                                const char *source);
bool getHeroGear(uint16_t creo_id, HeroGearEntry *out_gear_list);
