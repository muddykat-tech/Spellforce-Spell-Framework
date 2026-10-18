#include "sf_level_up_registry.h"
#include <vector>
#include <algorithm>

typedef struct AbilityLevelUpEntry
{
    uint8_t skill;
    uint8_t spec;
    uint8_t level;
    uint16_t ability_id;

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


uint16_t getLearnedAbility(uint8_t skill, uint8_t spec, uint8_t level)
{

    //TODO: move this call into specialized function
    std::sort(s_abilities_registry.begin(),  s_abilities_registry.end());

    AbilityLevelUpEntry search_entry = {skill, spec, level};
    auto it = std::lower_bound(s_abilities_registry.begin(), s_abilities_registry.end(), search_entry);
    if ((it != s_abilities_registry.end()) && (it->skill == skill) && (it->spec == spec) && (it->level == level))
    {
        return it->ability_id;
    }
    return 0;
}

void registerLearnedAbility(uint8_t skill, uint8_t spec, uint8_t level, uint16_t ability_id)
{
    AbilityLevelUpEntry new_entry = {skill, spec, level, ability_id};
    s_abilities_registry.push_back(new_entry);
}
