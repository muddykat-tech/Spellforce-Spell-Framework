#include "../core/sf_hooks.h"
#include "../core/sf_wrappers.h"
#include "../core/sf_modloader.h"
#include "../handlers/sf_spellrefresh_handlers.h"

#include "sf_spellrefresh_registry.h"
#include "sf_spelltype_registry.h"
#include "sf_registry.h"
#include <map>
#include <cstdio>

std::map<uint16_t, refresh_handler_ptr> spellrefresh_handler_map;

void registerSpellRefreshHandler(uint16_t spell_line_id, refresh_handler_ptr handler)
{
    auto check = spellrefresh_handler_map.find(spell_line_id);
    if (check != spellrefresh_handler_map.end())
    {
        char message[256];
        sprintf(message, "%s (v%s) has replaced an Spell Refresh Handler [%d] (Was this on purpose?)", g_current_mod->mod_id, g_current_mod->mod_version, spell_line_id);
        log_warning(message);
    }

    spellrefresh_handler_map[spell_line_id] = handler;
}

refresh_handler_ptr get_spell_refresh(uint16_t spell_line_id)
{
    auto it = spellrefresh_handler_map.find(spell_line_id);
    if (it == spellrefresh_handler_map.end())
    {
        char message[256];
        sprintf(message, "Unknown Job ID [%d] for Spell Refresh Handler", spell_line_id);
        log_warning(message);
        it = spellrefresh_handler_map.emplace(spell_line_id, &first_block_refresh_handler).first;
    }
    return it->second;
}

void register_vanilla_spell_refresh_handlers()
{
    // Vanilla spell refresh handlers
    int firstblock_cases[] = {
        0x06, 0x0c, 0x0f, 0x15, 0x19, 0x20, 0x24, 0x29, 0x2f, 0x38, 0x48,
        0x4f, 0x8c, 0x95, 0x99, 0x9c, 0x9d, 0x9e, 0xa1, 0xa4, 0xa7, 0xbd, 0xd5, 0xdd, 0xde,};

    int vanilla_domination_cases[] = {0x2e, 0x6c, 0x78, 0x7a, 0xc5, 0xed};

    for (int i = 0; i < sizeof(firstblock_cases) / sizeof(firstblock_cases[0]); i++)
    {
        registerSpellRefreshHandler(firstblock_cases[i], &first_block_refresh_handler);
    }

    registerSpellRefreshHandler(0x04, &slowness_refresh_handler);
    registerSpellRefreshHandler(0x10, &decay_refresh_handler);
    registerSpellRefreshHandler(0x22, &inflexibility_refresh_handler);
    registerSpellRefreshHandler(0x23, &weaken_refresh_handler);
    registerSpellRefreshHandler(0x30, &quickness_refresh_handler);
    registerSpellRefreshHandler(0x32, &flexibility_refresh_handler);
    registerSpellRefreshHandler(0x34, &strength_refresh_handler);
    registerSpellRefreshHandler(0x40, &brilliance_refresh_handler);
    registerSpellRefreshHandler(0x41, &brilliance_refresh_handler);
    registerSpellRefreshHandler(0x63, &suffocation_refresh_handler); // 99 and 100 in ghidra switch case respectivly
    registerSpellRefreshHandler(0x64, &inablility_refresh_handler); // ^
    registerSpellRefreshHandler(0x65, &slow_fighting_refresh_handler);
    registerSpellRefreshHandler(0x74, &dexterity_refresh_handler);
    registerSpellRefreshHandler(0x75, &endurance_refresh_handler);
    registerSpellRefreshHandler(0x76, &fast_fighting_refresh_handler);
    registerSpellRefreshHandler(0x7d, &charisma_refresh_handler);
    registerSpellRefreshHandler(0x82, &enlightenment_refresh_handler);
    registerSpellRefreshHandler(0x87, &melt_resistance_refresh_handler);
    registerSpellRefreshHandler(0x8a, &chill_resistance_refresh_handler);
    registerSpellRefreshHandler(0xae, &black_almightness_refresh_handler);
    registerSpellRefreshHandler(0xaf, &white_almightness_refresh_handler);
    registerSpellRefreshHandler(0xb2, &black_almightness_refresh_handler);
    registerSpellRefreshHandler(0xb3, &white_almightness_refresh_handler);
    registerSpellRefreshHandler(0xc7, &mutation_refresh_handler); // 199 in ghidra switch case
    registerSpellRefreshHandler(0xe5, &eternity_refresh_handler);
    registerSpellRefreshHandler(0xe9, &mutation_refresh_handler);

    registerSpellRefreshHandler(0x94, &warcry_berserk_refresh_handler);
    registerSpellRefreshHandler(0x98, &warcry_berserk_refresh_handler);

    registerSpellRefreshHandler(0x96, &patronize_shelter_refresh_handler);
    registerSpellRefreshHandler(0x9a, &patronize_shelter_refresh_handler);

    registerSpellRefreshHandler(0x97, &endurance_durability_refresh_handler);
    registerSpellRefreshHandler(0x9b, &endurance_durability_refresh_handler);

    registerSpellRefreshHandler(0x17, &pestilence_refresh_handler);

    for (int i = 0; i < sizeof(vanilla_domination_cases) / sizeof(vanilla_domination_cases[0]); i++)
    {
        registerSpellRefreshHandler(vanilla_domination_cases[i], &domination_spell_refresh_handler);
    }

    registerSpellRefreshHandler(0xda, &case_da_refresh_handler);
}