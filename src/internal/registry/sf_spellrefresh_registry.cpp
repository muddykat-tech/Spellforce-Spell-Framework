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
        sprintf(message, "%s (v%s) has replaced an Spell Refresh Handler [%d] (Was this on purpose?)", current_mod->mod_id, current_mod->mod_version, spell_line_id);
        log_warning(message);
    }

    spellrefresh_handler_map[spell_line_id] = handler;
}

refresh_handler_ptr get_spell_refresh(uint16_t spell_line_id)
{
    auto it = spellrefresh_handler_map.find(spell_line_id);
    if (it == spellrefresh_handler_map.end())
    {
        // Element doesn't exist, insert the default value
        log_warning("Unknown Job ID for Spell Refresh Handler, Assigning the default handler.");
        it = spellrefresh_handler_map.emplace(spell_line_id, &first_block_refresh_handler).first;
    }
    return it->second;
}

void register_vanilla_spell_refresh_handlers()
{
    // Vanilla spell refresh handlers
    int firstblock_cases[] = {
        4, 6, 0xc, 0xf, 0x10, 0x15, 0x19, 0x20, 0x22, 0x23,
        0x24, 0x29, 0x2f, 0x30, 0x32, 0x34, 0x38, 0x40, 0x41, 0x48,
        0x4f, 99, 100, 0x65, 0x74, 0x75, 0x76, 0x7d, 0x82, 0x87,
        0x8a, 0x8c, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b,
        0x9c, 0x9d, 0x9e, 0xa1, 0xa4, 0xa7, 0xaf, 0xb3, 0xbd, 199,
        0xd5, 0xdd, 0xde, 0xe5, 0xe9};

    int secondblock_cases[] = {0x2e, 0x6c, 0x78, 0x7a, 0xc5, 0xed};

    for (int i = 0; i < sizeof(firstblock_cases) / sizeof(firstblock_cases[0]); i++)
    {
        registerSpellRefreshHandler(firstblock_cases[i], &first_block_refresh_handler);
    }

    registerSpellRefreshHandler(0x17, &case_17_refresh_handler);

    for (int i = 0; i < sizeof(secondblock_cases) / sizeof(secondblock_cases[0]); i++)
    {
        registerSpellRefreshHandler(secondblock_cases[i], &second_block_refresh_handler);
    }

    registerSpellRefreshHandler(0xda, &case_da_refresh_handler);

    // These are not Vanilla Entries, testing only
    registerSpellRefreshHandler(0x0e, &first_block_refresh_handler);
    registerSpellRefreshHandler(0xeb, &first_block_refresh_handler);
}