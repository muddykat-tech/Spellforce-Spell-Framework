/** 
 * @defgroup AIHook AI Hooks
 * @ingroup Hooks
 * Hooks and Functions related to AI functionality.
 * Note that the main hook is the 
 * @ref rank_support_spell_hook
 * @addtogroup AIHook
 * @{
 */

#include "sf_ai_hook.h"

#include "../sf_wrappers.h"
#include "../sf_hooks.h"

#include "../../registry/ai_data_registries/sf_ai_avoidance_registry.h"
#include "../../registry/ai_data_registries/sf_ai_single_target_registry.h"

void clearAction(SF_SGtFigureAction *_this)
{
    _this->type = 0xffff;
    _this->subtype = 0;
    _this->unkn2 = 0;
    _this->unkn3 = 0;
    _this->unkn4 = 0;
    _this->unkn5 = 0;
}

bool isSpellAction(SF_SGtFigureAction *_this)
{
    if ((_this->type != 0) && (_this->type < 10000))
    {
        return true;
    }
    return false;
}

uint16_t __thiscall getSector(SF_CGdWorld *_this, SF_Coord *position)
{
    uint32_t index = position->X + position->Y * 0x400;
    return _this->cells[index].sector;
}

uint32_t GetEuclideanDistanceFromRing(int x1, int y1, int x2, int y2, int inner_radius, int outer_radius)
{
    int iVar1;

    iVar1 = (y1 - y2) * (y1 - y2) + (x1 - x2) * (x1 - x2);
    if (iVar1 < (inner_radius * inner_radius - 1))
    {
        return inner_radius * inner_radius - iVar1;
    }
    if ((outer_radius * outer_radius + 1) < iVar1)
    {
        return iVar1 - outer_radius * outer_radius;
    }
    return 0;
}

uint32_t signum(uint32_t param_1)
{
    return (param_1 ^ (int)param_1 >> 0x1f) - ((int)param_1 >> 0x1f);
}

/**
 * @brief Injects into the ranking system for a single target AI spell.
*/
uint32_t rank_support_spell_hook(SF_CGdBattleDevelopment *_this, uint16_t target_index, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    bool isStackable = hasSpellTag(spell_line, STACKABLE_SPELL);
    ai_single_handler_ptr handler = get_single_ai_handler(spell_line);
    uint32_t rank = handler(_this, target_index, spell_line, spell_data);
    if ((toolboxAPI.hasSpellOnIt(_this->battleData.CGdFigureToolBox, target_index, spell_line)) && (!isStackable))
    {
        rank = 0;
    }
    return rank;
}

/**
 * @}
 */