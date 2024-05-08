#include "sf_spelleffect_registry.h"
#include "sf_utility.h"

#include <iostream>
#include <map>
#include <cstdint>

std::map<uint16_t, handler_ptr> effect_handler_map;

void registerEffectHandler(uint16_t spell_job, handler_ptr handler) {
    auto check = effect_handler_map.find(spell_job);
    if (check != effect_handler_map.end()){ 
        char message[256];
        sprintf(message, "WARNING: An Effect Handler has been replaced! (Job ID == %d) (Was this on purpose?)", spell_job);
        logWarning(message);
    }

    effect_handler_map[spell_job] = handler;
}

handler_ptr get_spell_effect(uint16_t spell_job) {
    auto it = effect_handler_map.find(spell_job);
    if (it == effect_handler_map.end()) {
        logError("Unknown Job ID, No effect handler registered.");
        return NULL;
    }
    return it->second;
}