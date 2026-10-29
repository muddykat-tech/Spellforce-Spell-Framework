#pragma once
#include "stdint.h"

/**
 * Shared ability registry for both skill level-ups and avatar level-ups.
 * Field order follows registerLearnedAbility(): (skill, spec, level, ability_id).
 * Entries with skill == 0 && spec == 0 are avatar level-up records: level is the
 * avatar level, and the matching ability is granted to the spell book by the
 * avatar level-up hook (sf_avatar_level_hook.cpp). ability_id == 0 disables the
 * ability for the key (hooks treat 0 as "grant nothing").
 *
 * Lifecycle: population (registerLearnedAbility*, duplicate keys are replaced
 * with a logged conflict, the last registration wins) -> finalizeLearnedAbilities()
 * (the single sort) -> reads (getLearnedAbility, binary search). Any late
 * populator must call finalizeLearnedAbilities() again before reads are valid.
 */
void registerLearnedAbility(uint8_t skill, uint8_t spec, uint8_t level, uint16_t ability_id);
void registerLearnedAbilityFromSource(uint8_t skill, uint8_t spec, uint8_t level, uint16_t ability_id,
                                      const char *source);
void finalizeLearnedAbilities();
uint16_t getLearnedAbility(uint8_t skill, uint8_t spec, uint8_t level);
