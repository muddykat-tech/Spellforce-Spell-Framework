#include "../../../api/sf_effect_functions.h"
#include "../../handlers/sf_sub_effect_handlers.h"
#include "../../core/sf_hooks.h"
#include "../../core/sf_wrappers.h"
#include "../../core/sf_modloader.h"

#include "../sf_registry.h"
#include <map>
#include <cstdio>

static std::map<uint16_t, sub_effect_handler_ptr> s_sub_effect_handler_map;

void registerSubEffectHandler(uint16_t spell_line,
                              sub_effect_handler_ptr handler)
{
    auto check = s_sub_effect_handler_map.find(spell_line);
    if (check != s_sub_effect_handler_map.end())
    {
        char message[256];
        sprintf(message,
                "%s (v%s) has replaced a Subeffect Handler [%d] (Was this on purpose?)",
                g_current_mod->mod_id, g_current_mod->mod_version, spell_line);
        log_warning(message);
    }

    s_sub_effect_handler_map[spell_line] = handler;
}

sub_effect_handler_ptr get_sub_effect_handler(uint16_t spell_line)
{
    auto it = s_sub_effect_handler_map.find(spell_line);
    if (it == s_sub_effect_handler_map.end())
    {
        // Element doesn't exist, insert the default value
        log_warning(
            "Unknown Job ID for Spell End Effect, Assigning a default handler.");
        it = s_sub_effect_handler_map.emplace(spell_line,
                                              &default_sub_effect_handler).first;
    }
    return it->second;
}

void register_vanilla_sub_effect_handlers()
{
    registerSubEffectHandler(0x01, &elemental_sub_effect_handler);
    registerSubEffectHandler(0x0e, &elemental_sub_effect_handler);
    registerSubEffectHandler(0xb6, &elemental_sub_effect_handler);
    registerSubEffectHandler(0xb7, &elemental_sub_effect_handler);
    registerSubEffectHandler(0xea, &elemental_sub_effect_handler);
    registerSubEffectHandler(0xeb, &elemental_sub_effect_handler);
    registerSubEffectHandler(0xef, &elemental_sub_effect_handler);

    registerSubEffectHandler(0xec, &elemental_chain_sub_effect_handler);
    registerSubEffectHandler(0x8b, &elemental_chain_sub_effect_handler);

    registerSubEffectHandler(0x02, &common_sub_effect_handler);
    registerSubEffectHandler(0x04, &common_sub_effect_handler);
    registerSubEffectHandler(0x13, &common_sub_effect_handler);
    registerSubEffectHandler(0x15, &common_sub_effect_handler);
    registerSubEffectHandler(0x22, &common_sub_effect_handler);
    registerSubEffectHandler(0x23, &common_sub_effect_handler);
    registerSubEffectHandler(0x30, &common_sub_effect_handler);
    registerSubEffectHandler(0x32, &common_sub_effect_handler);
    registerSubEffectHandler(0x34, &common_sub_effect_handler);
    registerSubEffectHandler(0x38, &common_sub_effect_handler);
    registerSubEffectHandler(0x3a, &common_sub_effect_handler);
    registerSubEffectHandler(0x41, &common_sub_effect_handler);
    registerSubEffectHandler(0x43, &common_sub_effect_handler);
    registerSubEffectHandler(0x63, &common_sub_effect_handler);
    registerSubEffectHandler(0x64, &common_sub_effect_handler);
    registerSubEffectHandler(0x65, &common_sub_effect_handler);
    registerSubEffectHandler(0x74, &common_sub_effect_handler);
    registerSubEffectHandler(0x75, &common_sub_effect_handler);
    registerSubEffectHandler(0x76, &common_sub_effect_handler);
    registerSubEffectHandler(0x92, &common_sub_effect_handler);
    registerSubEffectHandler(0x93, &common_sub_effect_handler);
    registerSubEffectHandler(0xa1, &common_sub_effect_handler);
    registerSubEffectHandler(0xa6, &common_sub_effect_handler);
    registerSubEffectHandler(0xa7, &common_sub_effect_handler);
    registerSubEffectHandler(0xa8, &common_sub_effect_handler);
    registerSubEffectHandler(0xa9, &common_sub_effect_handler);
    registerSubEffectHandler(0xaa, &common_sub_effect_handler);
    registerSubEffectHandler(0xac, &common_sub_effect_handler);
    registerSubEffectHandler(0xe5, &common_sub_effect_handler);
    registerSubEffectHandler(0xe6, &common_sub_effect_handler);
    registerSubEffectHandler(0xe7, &common_sub_effect_handler);
    registerSubEffectHandler(0xe8, &common_sub_effect_handler);
    registerSubEffectHandler(0xe9, &common_sub_effect_handler);
    registerSubEffectHandler(0xed, &common_sub_effect_handler);
    registerSubEffectHandler(0xee, &common_sub_effect_handler);
    registerSubEffectHandler(0xf0, &common_sub_effect_handler);
}
