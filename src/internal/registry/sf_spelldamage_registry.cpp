#include "sf_spelldamage_registry.h"
#include "../core/sf_wrappers.h"
#include "../core/sf_modloader.h"
#include "../handlers/sf_spelldamage_handlers.h"

#include <iostream>
#include <map>
#include <cstdint>

static std::map<uint16_t, damage_handler_ptr> s_spelldamage_pre_handler_map;
static std::map<uint16_t, damage_handler_ptr> s_spelldamage_handler_map;
static std::map<uint16_t, damage_handler_ptr> s_spelldamage_post_handler_map;

void registerSpellDamageHandler(uint16_t spell_line_id, damage_handler_ptr handler, SpellDamagePhase phase)
{
    auto &phase_map = (phase == SpellDamagePhase::PRE)    ? s_spelldamage_pre_handler_map
                      : (phase == SpellDamagePhase::POST) ? s_spelldamage_post_handler_map
                                                          : s_spelldamage_handler_map;

    auto check = phase_map.find(spell_line_id);
    if (check != phase_map.end())
    {
        char message[256];
        sprintf(message, "%s (v%s) has replaced an Damage Handler [%d] (Was this on purpose?)", g_current_mod->mod_id, g_current_mod->mod_version, spell_line_id);
        log_warning(message);
    }
    phase_map[spell_line_id] = handler;
}

damage_handler_ptr get_spell_damage(const uint16_t spell_line_id, SpellDamagePhase phase)
{
    auto &phase_map = (phase == SpellDamagePhase::PRE)    ? s_spelldamage_pre_handler_map
                      : (phase == SpellDamagePhase::POST) ? s_spelldamage_post_handler_map
                                                          : s_spelldamage_handler_map;
    auto it = phase_map.find(spell_line_id);
    if (it == phase_map.end())
    {
        return NULL;
    }
    return it->second;
}

void __thiscall register_vanilla_spell_damage_handlers()
{
    registerSpellDamageHandler(0x06, &invulnerability_dmg_handler, SpellDamagePhase::PRE);
    registerSpellDamageHandler(0x97, &durability_dmg_handler, SpellDamagePhase::PRE);
    registerSpellDamageHandler(0x9b, &durability_dmg_handler, SpellDamagePhase::PRE);
    registerSpellDamageHandler(0x9d, &steelskin_dmg_handler, SpellDamagePhase::PRE);
    registerSpellDamageHandler(0xd5, &feedback_dmg_handler, SpellDamagePhase::PRE);

    registerSpellDamageHandler(0x0c, &dmg_shield_dmg_handler, SpellDamagePhase::DEFAULT);
    registerSpellDamageHandler(0x0f, &dmg_shield_dmg_handler, SpellDamagePhase::DEFAULT);
    registerSpellDamageHandler(0x2f, &dmg_shield_dmg_handler, SpellDamagePhase::DEFAULT);
    registerSpellDamageHandler(0xaf, &dmg_shield_dmg_handler, SpellDamagePhase::DEFAULT);
    registerSpellDamageHandler(0xb9, &dmg_shield_dmg_handler, SpellDamagePhase::DEFAULT);

    registerSpellDamageHandler(0x15, &hypnotize_dmg_handler, SpellDamagePhase::POST);
    registerSpellDamageHandler(0x20, &death_grasp_dmg_handler, SpellDamagePhase::POST);
    registerSpellDamageHandler(0x5d, &feign_death_dmg_handler, SpellDamagePhase::POST);
    registerSpellDamageHandler(0x8c, &conservation_dmg_handler, SpellDamagePhase::POST);
    registerSpellDamageHandler(0xa1, &hypnotize_dmg_handler, SpellDamagePhase::POST);
    registerSpellDamageHandler(0xa7, &hypnotize_dmg_handler, SpellDamagePhase::POST);
    registerSpellDamageHandler(0xd7, &hypnotize_dmg_handler, SpellDamagePhase::POST);
    registerSpellDamageHandler(0xda, &mana_shield_dmg_handler, SpellDamagePhase::POST);
}