#include <map>
#include <list>
#include <string.h>
#include "sf_hero_registry.h"
#include "../core/sf_wrappers.h"

typedef struct HeroRegistryEntry
{
    char source[64];
    std::list<HeroGearEntry> gear;
} HeroRegistryEntry;

static std::map<uint16_t, HeroRegistryEntry> s_custom_heroes_map;

void registerHeroGearFromSource(uint16_t creo_id, HeroGearEntry *gear_list, uint8_t list_len,
                                const char *source)
{
    std::list<HeroGearEntry> gear;
    for (int i = 0; i < list_len; i++)
    {
        gear.push_back(gear_list[i]);
    }

    auto entry = s_custom_heroes_map.find(creo_id);
    if (entry != s_custom_heroes_map.end())
    {
        log_warning("Hero registry conflict: creo %u: '%s' -> '%s' (%u gear -> %u gear)",
                    creo_id, entry->second.source, source,
                    (unsigned)entry->second.gear.size(), (unsigned)gear.size());
        entry->second.gear = gear;
        strncpy(entry->second.source, source, sizeof(entry->second.source) - 1);
        entry->second.source[sizeof(entry->second.source) - 1] = '\0';
        return;
    }

    HeroRegistryEntry new_entry = {};
    strncpy(new_entry.source, source, sizeof(new_entry.source) - 1);
    new_entry.source[sizeof(new_entry.source) - 1] = '\0';
    new_entry.gear = gear;
    s_custom_heroes_map.emplace(creo_id, new_entry);
}

void registerHeroGear(uint16_t creo_id, HeroGearEntry *gear_list, uint8_t list_len)
{
    registerHeroGearFromSource(creo_id, gear_list, list_len, "vanilla");
}

bool getHeroGear(uint16_t creo_id, HeroGearEntry *out_gear_list)
{
    auto entry = s_custom_heroes_map.find(creo_id);
    if (entry != s_custom_heroes_map.end())
    {
        std::list<HeroGearEntry> gear_list = entry->second.gear;
        int i = 0;
        for (auto it = gear_list.crbegin(); it != gear_list.crend(); ++it)
        {
            out_gear_list[i].item_id = it->item_id;
            out_gear_list[i].slot = it->slot;
            i++;
        }
        return true;
    }
    return false;
}
