#include "sf_phys_effect_registry.h"
#include "../../core/sf_modloader.h"

#include <cstdint>
#include <list>
#include <map>
#include <utility>

static std::map<uint16_t, sub_effect_handler_ptr> s_rain_handlers_map;
static std::map<OnHitPhase, std::list<std::pair<uint16_t, phys_effect_handler_ptr>>> s_phys_effect_handler_map;


void __thiscall default_rain_handler(SF_CGdEffect *_this, uint16_t effect_index)
{
    return;
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

void registerPhysEffectHandler(uint16_t spell_line_id, phys_effect_handler_ptr handler, OnHitPhase phase)
{
    // Mirror of the on-hit registry policy: same line in the same phase replaces
    // the existing handler with a warning, a new line is appended to the phase list.
    auto &handler_list = s_phys_effect_handler_map[phase];

    for (auto &entry : handler_list)
    {
        if (entry.first == spell_line_id)
        {
            log_warning("%s (v%s) has replaced a Phys Effect Handler [%d] in phase %d (Was this on purpose?)",
                        g_current_mod->mod_id, g_current_mod->mod_version, spell_line_id,
                        static_cast<int>(phase));

            entry.second = handler;
            return;
        }
    }

    handler_list.emplace_back(spell_line_id, handler);
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

std::list<std::pair<uint16_t, phys_effect_handler_ptr>> get_phys_effect_phase(OnHitPhase phase)
{
    auto it = s_phys_effect_handler_map.find(phase);
    if (it == s_phys_effect_handler_map.end())
    {
        return {};
    }
    return it->second;
}
