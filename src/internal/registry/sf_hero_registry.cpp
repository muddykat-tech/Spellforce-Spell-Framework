#include <map>
#include <list>
#include "sf_hero_registry.h"

std::map<uint16_t, std::list<HeroGearEntry> > s_custom_heroes_map;

void registerHeroGear(uint16_t creo_id, HeroGearEntry *gear_list, uint8_t list_len)
{
    std::list<HeroGearEntry> gear;
    for (int i = 0; i < list_len; i++)
    {
        gear.push_back(gear_list[i]);
    }
    s_custom_heroes_map.emplace(creo_id, gear);
}

bool getHeroGear(uint16_t creo_id, HeroGearEntry *out_gear_list)
{
    auto entry = s_custom_heroes_map.find(creo_id);
    if (entry != s_custom_heroes_map.end())
    {
        std::list<HeroGearEntry> gear_list = entry->second;
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
