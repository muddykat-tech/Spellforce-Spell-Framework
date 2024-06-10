
#include "sf_spellend_registry.h"
#include "../core/sf_wrappers.h"
#include "../handlers/sf_spellend_handlers.h"
#include "../core/sf_modloader.h"

#include <iostream>
#include <map>
#include <cstdint>

std::map<uint16_t, handler_ptr> spellend_handler_map;

void registerSpellEndHandler(uint16_t spell_line, handler_ptr handler)
{
    auto check = spellend_handler_map.find(spell_line);
    if (check != spellend_handler_map.end())
    {
        char message[256];
        sprintf(message, "%s (v%s) has replaced an End Handler [%d] (Was this on purpose?)", current_mod->mod_id, current_mod->mod_version, spell_line);
        log_warning(message);
    }

    spellend_handler_map[spell_line] = handler;
}

handler_ptr get_spell_end(uint16_t spell_line)
{
    auto it = spellend_handler_map.find(spell_line);
    if (it == spellend_handler_map.end())
    {
        // Element doesn't exist, insert the default value
        log_warning("Unknown Job ID for Spell End Effect, Assigning a default handler.");
        it = spellend_handler_map.emplace(spell_line, &default_end_handler).first;
    }
    return it->second;
}

void register_vanilla_spell_end_handlers()
{
    // As we define ALL of the end handlers ourself inside sf_spellend_handlers.cpp, we do NOT need to initialize them here. (unlike sf_spelleffect_registry.cpp)

    // Basic Spells that use OnSpellRemove and the default handlers
    registerSpellEndHandler(0x04, &command_handler_removespell);
    registerSpellEndHandler(0x10, &command_handler_removespell);
    registerSpellEndHandler(0x22, &command_handler_removespell);
    registerSpellEndHandler(0x23, &command_handler_removespell);
    registerSpellEndHandler(0x30, &command_handler_removespell);
    registerSpellEndHandler(0x32, &command_handler_removespell);
    registerSpellEndHandler(0x34, &command_handler_removespell);
    registerSpellEndHandler(0x40, &command_handler_removespell);
    registerSpellEndHandler(0x41, &command_handler_removespell);
    registerSpellEndHandler(0x63, &command_handler_removespell); // 99 and 100 in ghidra switch case respectivly
    registerSpellEndHandler(0x64, &command_handler_removespell); // ^
    registerSpellEndHandler(0x65, &command_handler_removespell);
    registerSpellEndHandler(0x74, &command_handler_removespell);
    registerSpellEndHandler(0x75, &command_handler_removespell);
    registerSpellEndHandler(0x76, &command_handler_removespell);
    registerSpellEndHandler(0x7d, &command_handler_removespell);
    registerSpellEndHandler(0x82, &command_handler_removespell);
    registerSpellEndHandler(0x87, &command_handler_removespell);
    registerSpellEndHandler(0x8a, &command_handler_removespell);
    registerSpellEndHandler(0xae, &command_handler_removespell);
    registerSpellEndHandler(0xb2, &command_handler_removespell);
    registerSpellEndHandler(0xc7, &command_handler_removespell); // 199 in ghidra switch case
    registerSpellEndHandler(0xe5, &command_handler_removespell);
    registerSpellEndHandler(0xe9, &command_handler_removespell);

    // invulnerability, illuminate, remediless, unkn (85) and demoralization
    registerSpellEndHandler(0x06, &common_handler_check_battle);
    registerSpellEndHandler(0xb, &common_handler_check_battle);
    registerSpellEndHandler(0x29, &common_handler_check_battle);
    registerSpellEndHandler(0x55, &common_handler_check_battle);
    registerSpellEndHandler(0x80, &common_handler_check_battle);

    // Freeze, Petrify
    registerSpellEndHandler(0x09, &common_handler_check_unfreeze);
    registerSpellEndHandler(0x19, &common_handler_check_unfreeze);

    registerSpellEndHandler(0x0a, &fog_end_handler);

    // Fireshield, Fiegn Death and Manashield
    registerSpellEndHandler(0xc, &common_handler_job_battle_check);
    registerSpellEndHandler(0x5d, &common_handler_job_battle_check);
    registerSpellEndHandler(0xda, &common_handler_job_battle_check);

    // Next block in switch
    registerSpellEndHandler(0xe, &common_handler_unfreeze);
    registerSpellEndHandler(0x16, &common_handler_unfreeze);
    registerSpellEndHandler(0x91, &common_handler_unfreeze);
    registerSpellEndHandler(0xa8, &common_handler_unfreeze);
    registerSpellEndHandler(0xa9, &common_handler_unfreeze);
    registerSpellEndHandler(0xeb, &common_handler_unfreeze);

    registerSpellEndHandler(0xf, &common_handler_job_battle_check2);
    registerSpellEndHandler(0x2f, &common_handler_job_battle_check2);
    registerSpellEndHandler(0x56, &common_handler_job_battle_check2);

    // Goto section in switch
    registerSpellEndHandler(0x11, &common_handler_done);
    registerSpellEndHandler(0x25, &common_handler_done);
    registerSpellEndHandler(0x26, &common_handler_done);
    registerSpellEndHandler(0x27, &common_handler_done);
    registerSpellEndHandler(0x28, &common_handler_done);
    registerSpellEndHandler(0x31, &common_handler_done);
    registerSpellEndHandler(0x33, &common_handler_done);
    registerSpellEndHandler(0x35, &common_handler_done);
    registerSpellEndHandler(0x49, &common_handler_done);
    registerSpellEndHandler(0x4a, &common_handler_done);
    registerSpellEndHandler(0x4b, &common_handler_done);
    registerSpellEndHandler(0x4c, &common_handler_done);
    registerSpellEndHandler(0xad, &common_handler_done);
    registerSpellEndHandler(0xbe, &common_handler_done);
    registerSpellEndHandler(0xc1, &common_handler_done);
    registerSpellEndHandler(0xc4, &common_handler_done);
    registerSpellEndHandler(0xc8, &common_handler_done); // 200 in ghidra switch case
    registerSpellEndHandler(0xc9, &common_handler_done);
    registerSpellEndHandler(0xca, &common_handler_done);
    registerSpellEndHandler(0xcc, &common_handler_done);
    registerSpellEndHandler(0xcd, &common_handler_done);
    registerSpellEndHandler(0xcf, &common_handler_done);
    registerSpellEndHandler(0xd0, &common_handler_done);
    registerSpellEndHandler(0xd4, &common_handler_done);
    registerSpellEndHandler(0xd6, &common_handler_done);
    registerSpellEndHandler(0xd7, &common_handler_done);
    registerSpellEndHandler(0xd8, &common_handler_done);
    registerSpellEndHandler(0xd9, &common_handler_done);

    registerSpellEndHandler(0x15, &common_handler_check_unfreeze2);
    registerSpellEndHandler(0xa1, &common_handler_check_unfreeze2);
    registerSpellEndHandler(0xa7, &common_handler_check_unfreeze2);

    // Case 0x48 (FUN_007b410) -> break;
    // Case 0x4f (FUN_007b360) -> break;
}