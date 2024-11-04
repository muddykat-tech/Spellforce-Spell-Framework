#include "sf_ai_avoidance_handlers.h"
#include "../core/sf_hooks.h"
#include "../core/sf_wrappers.h"
#include <cstdio>

int __thiscall sf_ai_avoidance_hypnotize_handler(SF_CGdAiBattleDevelopement *_this, uint16_t figure_index, uint16_t spell_index)
{
    if (toolboxAPI.hasSpellOnIt(_this->CGdFigureToolBox, source_index, spell_index))
    {
        return 10000;
    }
    return 100;
}

int __thiscall sf_ai_avoidance_freeze_handler(SF_CGdAiBattleDevelopement *_this, uint16_t figure_index, uint16_t spell_index)
{
    if (toolboxAPI.hasSpellOnIt(_this->CGdFigureToolBox, source_index, spell_index))
    {
        return 500;
    }
    return 100;
}

int __thiscall sf_ai_avoidance_invulnerability_handler(SF_CGdAiBattleDevelopement *_this, uint16_t figure_index, uint16_t spell_index)
{
    if (toolboxAPI.hasSpellOnIt(_this->CGdFigureToolBox, source_index, spell_index))
    {
        return 250;
    }
    return 100;
}

int __thiscall sf_ai_avoidance_shield_handler(SF_CGdAiBattleDevelopement *_this, uint16_t figure_index, uint16_t spell_index)
{
    if (toolboxAPI.hasSpellOnIt(_this->CGdFigureToolBox, source_index, spell_index))
    {
        return 125;
    }
    return 100;
}

int __thiscall sf_ai_avoidance_default_handler(SF_CGdAiBattleDevelopement *_this, uint16_t figure_index, uint16_t spell_index)
{
    return 100;
}