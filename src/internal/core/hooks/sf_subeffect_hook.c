/**
 * @defgroup SubEffectHook SubEffect Hook
 * @ingroup Hooks
 * @brief Used to inject sub effects into spellforce
 * @addtogroup SubEffectHook
 * @{
 */

#include "../../registry/spell_data_registries/sf_subeffect_registry.h"
#include "../sf_wrappers.h"
#include "../sf_hooks.h"

#include "sf_subeffect_hook.h"

void __thiscall sf_subeffect_hook(SF_CGDEffect *_this, uint16_t effect_id)
{
    uint16_t spell_id = effectAPI.getEffectXData(_this, effect_id,
                                                 EFFECT_SUBSPELL_ID);
    if (spell_id)
    {
        SF_CGdResourceSpell spell_data;
        spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data,
                                      spell_id);
        sub_effect_handler_ptr handler =
            get_sub_effect_handler(spell_data.spell_line_id);
        if (handler != NULL)
        {
            handler(_this, effect_id);
        }
    }
}

/** @} */

//just rewritten logic from ghidra to butcher forward
void __thiscall sf_phys_sub_effect_hook(SF_CGDEffect *_this, uint16_t effect_id)
{
    //STONE RAIN HANDLER
    if (effectAPI.getEffectXData(_this, effect_id, EFFECT_DO_NOT_ADD_SUBSPELL))
    {
        uint32_t pos_x = _this->active_effect_list[effect_id].position.X;
        uint32_t pos_y = _this->active_effect_list[effect_id].position.Y;
        for (uint32_t i = 9, j = 0; i > 0; i--, j+=7)
        {
            uint16_t w_x = _this->SF_CGdWorld->unknown1[j+pos_x];
            uint16_t w_y = _this->SF_CGdWorld->unknown1[j+pos_y];
            uint16_t map_size =  _this->SF_CGdWorld->map_size;
            if ((w_x < map_size) && (w_y < map_size))
            {
                uint32_t damage = effectAPI.getEffectXData(_this, effect_id, EFFECT_PHYSICAL_DAMAGE);
                uint32_t source_id = effectAPI.getEffectXData(_this, effect_id, EFFECT_ENTITY_INDEX);
                if ((_this->SF_CGdWorld->cells[w_y*0x400 + w_y].world_cell_flags & 0x10) != 0)
                {
                    uint16_t target_id = toolboxAPI.getFigureFromWorld(_this->SF_CGdWorldToolBox, w_x, w_y, 0);
                    if ((_this->SF_CGdFigure->figures[target_id].owner != (uint16_t)(-1)) &&
                        (_this->SF_CGdFigure->figures[target_id].flags & 0xa == 0) &&
                        (toolboxAPI.isTargetable(_this->SF_CGdFigureToolBox, target_id)))
                    {
                        bool isFriendly = toolboxAPI.figuresCheckFriendly(_this, source_id, target_id);
                        bool isNeutral = toolboxAPI.figuresCheckNeutral(_this, source_id, target_id);
                        if ((!isFriendly && !isNeutral) || (_this->SF_CGdFigure->figures[target_id].owner != 0))
                        {
                            //kGdSpellLineStone
                            uint16_t mult = toolboxAPI.getPhysDamageReduction(_this->SF_CGdFigureToolBox, source_id,
                                                                              target_id, 0x57);
                            damage = (mult * damage + 5000) / 10000;
                            if (_this->SF_CGdFigure->figures[source_id].set_type == 8)
                            {
                                damage *= 2;
                            }
                            if (figureAPI.isSiegeUnit(_this->SF_CGdFigure, target_id))
                            {
                                //BUGFIX
                                damage = (damage * 100 + 50) / 1000;
                            }
                            toolboxAPI.dealDamage(_this->SF_CGdFigureToolBox, source_id, target_id, damage, 0, 1, 0);
                        }
                    }
                }
            }
        }

    }
}
