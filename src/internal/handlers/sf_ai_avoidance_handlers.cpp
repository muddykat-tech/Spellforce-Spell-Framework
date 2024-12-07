#include "sf_ai_avoidance_handlers.h"
#include "../core/sf_hooks.h"
#include "../core/sf_wrappers.h"
#include <cstdio>

uint32_t __thiscall sf_ai_avoidance_hypnotize_handler(CGdAIBattleData *_this, uint16_t figure_index, uint16_t spell_index)
{
    if (toolboxAPI.hasSpellOnIt(_this->CGdFigureToolBox, figure_index, spell_index))
    {
        return 10000;
    }
    return 100;
}

uint32_t __thiscall sf_ai_avoidance_freeze_handler(CGdAIBattleData *_this, uint16_t figure_index, uint16_t spell_index)
{
    if (toolboxAPI.hasSpellOnIt(_this->CGdFigureToolBox, figure_index, spell_index))
    { 
        return 500;
    }
    return 100;
}

uint32_t __thiscall sf_ai_avoidance_invulnerability_handler(CGdAIBattleData *_this, uint16_t figure_index, uint16_t spell_index)
{
    if (toolboxAPI.hasSpellOnIt(_this->CGdFigureToolBox, figure_index, spell_index))
    {
        return 250;
    }
    return 100;
}

uint32_t __thiscall sf_ai_avoidance_shield_handler(CGdAIBattleData *_this, uint16_t figure_index, uint16_t spell_index)
{
    if (toolboxAPI.hasSpellOnIt(_this->CGdFigureToolBox, figure_index, spell_index))
    {
        return 125;
    }
    return 100;
}

uint32_t __thiscall sf_ai_avoidance_default_handler(CGdAIBattleData *_this, uint16_t figure_index, uint16_t spell_index)
{
    return 100;
}