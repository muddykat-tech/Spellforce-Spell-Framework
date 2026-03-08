#include "sf_phys_effect_hook.h"
#include "../../registry/spell_data_registries/sf_phys_effect_registry.h"
#include "../../registry/spell_data_registries/sf_onhit_registry.h"
#include <utility>
#include <list>

uint16_t __thiscall assistance_onhit_handler(SF_CGdFigureJobs *_this, uint16_t source_index, uint16_t target_index,
                                             uint16_t damage);

void __thiscall sf_phys_effect_hook(SF_CGDEffect *_this, uint16_t effect_id)
{
    uint32_t source_id = effectAPI.getEffectXData(_this, effect_id, EFFECT_ENTITY_INDEX);
    uint32_t source_type = effectAPI.getEffectXData(_this, effect_id, EFFECT_ENTITY_TYPE);
    uint32_t target_id = effectAPI.getEffectXData(_this, effect_id, EFFECT_ENTITY_INDEX2);
    uint32_t target_type = effectAPI.getEffectXData(_this, effect_id, EFFECT_ENTITY_TYPE2);
    uint32_t damage = effectAPI.getEffectXData(_this, effect_id, EFFECT_PHYSICAL_DAMAGE);

    //rain handlers
    if ((!effectAPI.getEffectXData(_this, effect_id, EFFECT_ADD_SUBSPELL)) &&
        (effectAPI.effectXDataExists(_this, effect_id, EFFECT_ADD_SUBSPELL)))
    {
        uint16_t spell_index = effectAPI.getEffectXData(_this, effect_id, EFFECT_SPELL_INDEX);
        uint16_t spell_line = spellAPI.getSpellLine(_this->CGdSpell, spell_index);
        sub_effect_handler_ptr rain_handler = get_rain_handler(spell_line);
        rain_handler(_this, effect_id);
    }
    else
    {
        bool isMagicDamage = 0;
        if (target_type == 1)
        {
            uint16_t spell_line = effectAPI.getEffectXData(_this, effect_id, EFFECT_SPELL_LINE);
            if (spell_line != 0)
            {
                log_debug(DEBUG_HIGH, "Spell line [%d], damage [%d]", spell_line, damage);
                phys_effect_handler_ptr single_handler = get_phys_effect_handler(spell_line);
                //spark handler by default
                damage = single_handler(_this, source_id, target_id, &isMagicDamage, damage);

            }
            if (toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_id, kGdSpellLineAssistance) &&
                (damage != 0x7fff))
            {
                std::list<std::pair<uint16_t, onhit_handler_ptr>> onhit_list = get_onhit_phase(OnHitPhase::PHASE_2);
                for (auto it = onhit_list.crbegin(); it != onhit_list.crend(); ++it)
                {
                    std::pair<uint16_t, onhit_handler_ptr> entry = *it;
                    uint16_t spell_line_id = entry.first;
                    if (spell_line_id == kGdSpellLineAssistance)
                    {
                        onhit_handler_ptr onhit_func = entry.second;
                        damage = onhit_func(_this->SF_CGdFigureToolBox->CGdFigureJobs, source_id, target_id, damage);
                    }
                }

            }
            toolboxAPI.dealDamage(_this->SF_CGdFigureToolBox, source_id, target_id, damage, isMagicDamage, 0, 0);
            //Monument protection handler
            uint32_t mana_cost = effectAPI.getEffectXData(_this, effect_id, EFFECT_MANA_COST);
            if (mana_cost != 0)
            {
                SF_CGdTargetData source;
                source.position = {0,0};
                source.entity_index = source_id;
                source.entity_type = source_type;

                SF_CGdTargetData target;
                target.position = {0,0};
                target.entity_index = target_id;
                target.entity_type = target_type;

                SF_Rectangle rect = {0,0};

                effectAPI.addEffect(_this, kGdEffectMonumentHitFigure, &source, &target,
                                    _this->OpaqueClass->current_step, 0x14, &rect);
                if (figureAPI.isAlive(_this->SF_CGdFigure, target_id))
                {
                    figureAPI.subMana(_this->SF_CGdFigure, target_id, mana_cost);
                }
            }
        }
        //Siege Aura Handler
        else
        {
            if (_this->CGdBuilding->buildings[target_id].health_current != 0)
            {
                buildingAPI.buildingDealDamage(_this->SF_CGdBuildingToolbox, source_id, target_id, damage, 0);
            }
        }
    }
}


