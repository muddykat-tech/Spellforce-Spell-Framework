#include "sf_sub_effect_handlers.h"
#include "../core/sf_hooks.h"
#include "../core/sf_wrappers.h"

void __thiscall elemental_sub_effect_handler(SF_CGDEffect *_this, uint16_t effect_index)
{
    uint16_t spell_id = effectAPI.getEffectXData(_this, effect_index, EFFECT_SUBSPELL_ID);
    uint16_t figure_index1 = effectAPI.getEffectXData(_this, effect_index, EFFECT_ENTITY_INDEX);
    uint8_t figure_type1;
    if (figure_index1)
    {
        figure_type1 = effectAPI.getEffectXData(_this, effect_index, EFFECT_ENTITY_TYPE);
    }
    uint16_t sub_spell_flag = effectAPI.getEffectXData(_this, effect_index, EFFECT_DO_NOT_ADD_SUBSPELL);
    if (sub_spell_flag != 1)
    {
        // here comes the black magic
        // CGdWorld structure is annotated badly, so offsets we use
        uint32_t world = *(uint32_t *)_this->SF_CGdWorld;
        uint16_t map_size = *(uint16_t *)(world + 0x11187);
        for (uint16_t i = 0, j = 9; j != 0; j--, i += 7)
        {
            uint16_t cell_x = *(uint16_t *)(world + i) + _this->active_effect_list[effect_index].position.X;
            uint16_t cell_y = *(uint16_t *)(world + i + 2) + _this->active_effect_list[effect_index].position.Y;
            if ((cell_x < map_size) && (cell_y < map_size))
            {
                uint8_t flags = *(uint8_t *)(world + 0x1118A + (cell_y * 0x400 + cell_x) * 6);
                if ((flags & 0x10) != 0)
                {
                    uint16_t figure_id = toolboxAPI.getFigureFromWorld(_this->SF_CGdWorldToolBox, cell_x, cell_y, 0);
                    if (figureAPI.isAlive(_this->SF_CGdFigure, figure_id) && toolboxAPI.isTargetable(_this->SF_CGdFigureToolBox, figure_id) && (toolboxAPI.figuresCheckHostile(_this->SF_CGdFigureToolBox, figure_index1, figure_id)))
                    {
                        SF_CGdTargetData target = {1, figure_id, {0, 0}};
                        SF_CGdTargetData source = {1, figure_index1, {0, 0}};
                        spellAPI.addSpell(_this->CGdSpell, spell_id, _this->OpaqueClass->current_step, &source, &target, 0);
                    }
                }
            }
        }
    }
    else
    {
        uint16_t figure_index2 = effectAPI.getEffectXData(_this, effect_index, EFFECT_ENTITY_INDEX2);
        uint8_t figure_type2 = 0;
        if (figure_index2)
        {
            figure_type2 = effectAPI.getEffectXData(_this, effect_index, EFFECT_ENTITY_TYPE2);
        }
        if (!figure_type2 || !figure_index2)
        {
            return;
        }
        SF_CGdTargetData source = {figure_type1, figure_index1, {0, 0}};
        SF_CGdTargetData target = {figure_type2, figure_index2, {0, 0}};
        spellAPI.addSpell(_this->CGdSpell, spell_id, _this->OpaqueClass->current_step, &source, &target, 0);
    }
}

void __thiscall elemental_chain_sub_effect_handler(SF_CGDEffect *_this, uint16_t effect_index)
{
    uint16_t spell_id = effectAPI.getEffectXData(_this, effect_index, EFFECT_SUBSPELL_ID);
    uint16_t figure_index1 = effectAPI.getEffectXData(_this, effect_index, EFFECT_ENTITY_INDEX);
    uint8_t figure_type1;
    if (figure_index1)
    {
        figure_type1 = effectAPI.getEffectXData(_this, effect_index, EFFECT_ENTITY_TYPE);
    }
    uint16_t figure_index2 = effectAPI.getEffectXData(_this, effect_index, EFFECT_ENTITY_INDEX2);
    uint16_t figure_index3 = effectAPI.getEffectXData(_this, effect_index, EFFECT_ENTITY_INDEX3);
    uint8_t figure_type2 = 0;
    if (figure_index2)
    {
        figure_type2 = effectAPI.getEffectXData(_this, effect_index, EFFECT_ENTITY_TYPE2);
    }
    if (!figure_type2 || !figure_index2)
    {
        return;
    }
    SF_CGdTargetData source = {figure_type1, figure_index1, {0, 0}};
    SF_CGdTargetData target = {figure_type2, figure_index2, {0, 0}};
    spellAPI.addSpell(_this->CGdSpell, spell_id, _this->OpaqueClass->current_step, &source, &target, figure_index3);
}

void __thiscall common_sub_effect_handler (SF_CGDEffect *_this, uint16_t effect_index)
{
    uint16_t spell_id = effectAPI.getEffectXData(_this, effect_index, EFFECT_SUBSPELL_ID);
    uint16_t figure_index1 = effectAPI.getEffectXData(_this, effect_index, EFFECT_ENTITY_INDEX);
    uint8_t figure_type1;
    if (figure_index1)
    {
        figure_type1 = effectAPI.getEffectXData(_this, effect_index, EFFECT_ENTITY_TYPE);
    }
    uint16_t figure_index2 = effectAPI.getEffectXData(_this, effect_index, EFFECT_ENTITY_INDEX2);
    uint8_t figure_type2 = 0;
    if (figure_index2)
    {
        figure_type2 = effectAPI.getEffectXData(_this, effect_index, EFFECT_ENTITY_TYPE2);
    }
    if (!figure_type2 || !figure_index2)
    {
        return;
    }
    if (!figure_type1 || !figure_index1)
    {
        return;
    }
    SF_CGdTargetData source = {figure_type1, figure_index1, {0, 0}};
    SF_CGdTargetData target = {figure_type2, figure_index2, {0, 0}};
    spellAPI.addSpell(_this->CGdSpell, spell_id, _this->OpaqueClass->current_step, &source, &target, 0);
}