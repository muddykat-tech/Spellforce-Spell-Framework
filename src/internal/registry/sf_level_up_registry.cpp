#include "sf_level_up_registry.h"
#include "../core/sf_wrappers.h"
#include <vector>
#include <algorithm>
#include <string.h>

typedef struct AbilityLevelUpEntry
{
    uint8_t skill;
    uint8_t spec;
    uint8_t level;
    uint16_t ability_id;
    char source[64];

    bool operator<(const AbilityLevelUpEntry& other) const
    {
        if (skill != other.skill)
        {
            return skill < other.skill;
        }
        if (spec != other.spec)
        {
            return spec < other.spec;
        }
        return level < other.level;
    }
} AbilityLevelUpEntry;

static std::vector<AbilityLevelUpEntry> s_abilities_registry;

void registerLearnedAbilityFromSource(uint8_t skill, uint8_t spec, uint8_t level, uint16_t ability_id,
                                      const char *source)
{
    for (AbilityLevelUpEntry &entry : s_abilities_registry)
    {
        if (entry.skill == skill && entry.spec == spec && entry.level == level)
        {
            log_warning("Ability registry conflict: (skill %u, spec %u, level %u): '%s' -> '%s' "
                        "(ability %u -> %u)",
                        skill, spec, level, entry.source, source, entry.ability_id, ability_id);
            entry.ability_id = ability_id;
            strncpy(entry.source, source, sizeof(entry.source) - 1);
            entry.source[sizeof(entry.source) - 1] = '\0';
            return;
        }
    }

    AbilityLevelUpEntry new_entry;
    memset(&new_entry, 0, sizeof(new_entry));
    new_entry.skill = skill;
    new_entry.spec = spec;
    new_entry.level = level;
    new_entry.ability_id = ability_id;
    strncpy(new_entry.source, source, sizeof(new_entry.source) - 1);
    new_entry.source[sizeof(new_entry.source) - 1] = '\0';
    s_abilities_registry.push_back(new_entry);
}

void registerLearnedAbility(uint8_t skill, uint8_t spec, uint8_t level, uint16_t ability_id)
{
    registerLearnedAbilityFromSource(skill, spec, level, ability_id, "vanilla");
}

void finalizeLearnedAbilities()
{
    std::sort(s_abilities_registry.begin(), s_abilities_registry.end());
}

uint16_t getLearnedAbility(uint8_t skill, uint8_t spec, uint8_t level)
{
    /* The registry must be finalized (see the header lifecycle note); duplicate
     * keys were replaced at registration time, so the keys are unique. */
    AbilityLevelUpEntry search_entry;
    memset(&search_entry, 0, sizeof(search_entry));
    search_entry.skill = skill;
    search_entry.spec = spec;
    search_entry.level = level;

    auto it = std::lower_bound(s_abilities_registry.begin(), s_abilities_registry.end(), search_entry);
    if ((it != s_abilities_registry.end()) && (it->skill == skill) && (it->spec == spec) &&
        (it->level == level))
    {
        return it->ability_id;
    }
    return 0;
}
