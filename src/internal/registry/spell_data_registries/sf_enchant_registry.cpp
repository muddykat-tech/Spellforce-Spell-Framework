#include "sf_enchant_registry.h"
#include "../../core/sf_hooks.h"
#include "../../core/sf_modloader.h"
#include "../../core/sf_wrappers.h"

#include <map>
#include <utility>

static std::map<uint16_t, enchant_handler_ptr> s_enchant_handlers_map;

uint32_t __thiscall default_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t dex_current = figureAPI.getCurrentStat(_this, figure_id, DEXTERITY);
    uint16_t int_current = figureAPI.getCurrentStat(_this, figure_id, INTELLIGENCE);
    return dex_current * 5 + int_current * 5 + 500;
}

uint32_t __thiscall poison_enchant_handler(SF_CGdFigure *_this, uint16_t figure_id)
{
    uint16_t dex_current = figureAPI.getCurrentStat(_this, figure_id, DEXTERITY);
    uint16_t int_current = figureAPI.getCurrentStat(_this, figure_id, INTELLIGENCE);
    return dex_current * 8 + int_current * 5 + 1400;
}

void registerEnchantHandler(uint16_t spell_line, enchant_handler_ptr handler)
{
    auto check = s_enchant_handlers_map.find(spell_line);
    if (check != s_enchant_handlers_map.end())
    {
        log_warning("%s (v%s) has replaced a Enchantment Chance Handler [%d] (Was this on purpose?)",
                    g_current_mod->mod_id, g_current_mod->mod_version, spell_line);
    }
    s_enchant_handlers_map[spell_line] = handler;
}

enchant_handler_ptr get_enchant_handler(uint16_t spell_line)
{
    auto it = s_enchant_handlers_map.find(spell_line);
    if (it == s_enchant_handlers_map.end())
    {
        //log_debug(DEBUG_HIGH, "Using default handler for enchant [%d]", spell_line);
        it = s_enchant_handlers_map.emplace(spell_line, &default_enchant_handler).first;
    }
    return it->second;
}

void register_vanilla_enchants_handlers()
{
    registerEnchantHandler(kGdSpellLinePoison, &poison_enchant_handler);
}
