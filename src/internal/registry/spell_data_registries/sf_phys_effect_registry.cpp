#include "sf_phys_effect_registry.h"
#include "../../core/sf_modloader.h"

#include <map>

static std::map<uint16_t, sub_effect_handler_ptr> s_rain_handlers_map;
static std::map<uint16_t, phys_effect_handler_ptr> s_phys_effect_handlers_map;


void __thiscall default_rain_handler(SF_CGdEffect *_this, uint16_t effect_index)
{
    return;
}

uint16_t __thiscall default_phys_effect_handler (SF_CGdEffect *_this, uint16_t source, uint16_t target,
                                                 bool *isSpellDamage, uint16_t damage)
{
    return damage;
}

void registerPhysRainHandler(uint16_t spell_line_id, sub_effect_handler_ptr handler)
{
    auto check = s_rain_handlers_map.find(spell_line_id);
    if (check != s_rain_handlers_map.end())
    {
        log_warning("%s (v%s) has replaced a Phys Rain Handler [%d] (Was this on purpose?)",
                    g_current_mod->mod_id, g_current_mod->mod_version, spell_line_id);
    }

    s_rain_handlers_map[spell_line_id] = handler;
}

void registerPhysEffectHandler(uint16_t spell_line_id, phys_effect_handler_ptr handler)
{
    auto check = s_phys_effect_handlers_map.find(spell_line_id);
    if (check != s_phys_effect_handlers_map.end())
    {
        log_warning("%s (v%s) has replaced a Phys Effect Handler [%d] (Was this on purpose?)",
                    g_current_mod->mod_id, g_current_mod->mod_version, spell_line_id);
    }
    s_phys_effect_handlers_map[spell_line_id] = handler;
}


sub_effect_handler_ptr get_rain_handler(uint16_t spell_line_id)
{
    auto it = s_rain_handlers_map.find(spell_line_id);
    if (it == s_rain_handlers_map.end())
    {
        log_warning("Unknown Spell Line ID [%d] for Phys Rain Handler", spell_line_id);
        it = s_rain_handlers_map.emplace(spell_line_id, &default_rain_handler).first;
    }
    return it->second;
}

phys_effect_handler_ptr get_phys_effect_handler(uint16_t spell_line_id)
{
    auto it = s_phys_effect_handlers_map.find(spell_line_id);
    if (it == s_phys_effect_handlers_map.end())
    {
        log_warning("Unknown Spell Line ID [%d] for Phys Effect Handler", spell_line_id);
        it = s_phys_effect_handlers_map.emplace(spell_line_id, &default_phys_effect_handler).first;
    }
    return it->second;
}
