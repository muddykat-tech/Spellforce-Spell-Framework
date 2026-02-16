#include "sf_phys_effect_handlers.h"
#include "../core/sf_wrappers.h"
#include "../core/sf_hooks.h"

void __thiscall stone_rain_phys_handler(SF_CGDEffect *_this, uint16_t effect_id)
{
    uint32_t source_id = effectAPI.getEffectXData(_this, effect_id, EFFECT_ENTITY_INDEX);
    uint32_t damage = effectAPI.getEffectXData(_this, effect_id, EFFECT_PHYSICAL_DAMAGE);
    uint32_t pos_x = _this->active_effect_list[effect_id].position.X;
    uint32_t pos_y = _this->active_effect_list[effect_id].position.Y;

    for (uint32_t i = 9, j = 0; i > 0; i--, j+=7)
    {
        uint16_t w_x = _this->SF_CGdWorld->unknown1[j].uknwn1 + pos_x;
        uint16_t w_y = _this->SF_CGdWorld->unknown1[j].uknwn2 + pos_y;
        uint16_t map_size =  _this->SF_CGdWorld->map_size;
        if ((w_x < map_size) && (w_y < map_size))
        {
            if ((_this->SF_CGdWorld->cells[w_y*0x400 + w_x].world_cell_flags & 0x10) != 0)
            {
                uint16_t target_id = toolboxAPI.getFigureFromWorld(_this->SF_CGdWorldToolBox, w_x, w_y, 0);
                if ((_this->SF_CGdFigure->figures[target_id].owner != (uint16_t)(-1)) &&
                    ((_this->SF_CGdFigure->figures[target_id].flags & 0xa) == 0) &&
                    (toolboxAPI.isTargetable(_this->SF_CGdFigureToolBox, target_id)))
                {
                    bool isFriendly = toolboxAPI.figuresCheckFriendly(_this->SF_CGdFigureToolBox, source_id,
                                                                      target_id);
                    bool isNeutral = toolboxAPI.figuresCheckNeutral(_this->SF_CGdFigureToolBox, source_id,
                                                                    target_id);
                    if ((!isFriendly && !isNeutral) || (_this->SF_CGdFigure->figures[target_id].owner != 0))
                    {
                        //kGdSpellLineStone
                        uint16_t mult = toolboxAPI.getPhysDamageReduction(_this->SF_CGdFigureToolBox, source_id,
                                                                          target_id, kGdSpellLineStone);
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

uint16_t __thiscall spark_phys_handler (SF_CGDEffect *_this, uint16_t source, uint16_t target, bool *isSpellDamage,
                                        uint16_t damage)
{
    *isSpellDamage = true;
    return damage;
}

