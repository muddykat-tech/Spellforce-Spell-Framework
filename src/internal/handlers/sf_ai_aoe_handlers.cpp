#include "sf_ai_aoe_handlers.h"
#include "../core/sf_wrappers.h"


uint32_t __thiscall raise_dead_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    CGdFigureIterator iter;
    iteratorAPI.figureIteratorInit(&iter, 0, 0, 0x3ff, 0x3ff);
    iteratorAPI.figureIteratorSetPointers(&iter, _this->battleData.CGdFigure,
                                          _this->battleData.autoclass22, _this->battleData.CGdWorld);

    iteratorAPI.iteratorSetArea(&iter, cast_position, spell_data->params[2]);
    uint16_t count = 0;
    uint32_t x_sum = 0;
    uint32_t y_sum = 0;
    SF_CGdFigure *sf_figures = _this->battleData.CGdFigure;

    uint16_t figure_index = iteratorAPI.getNextFigure(&iter);
    while (figure_index != 0)
    {
        if (sf_figures->figures[figure_index].owner != (uint16_t)(-1))
        {
            if (sf_figures->figures[figure_index].owner == sf_figures->figures[_this->battleData.current_figure].owner)
            {
                if ((sf_figures->figures[figure_index].flags & GdFigureFlags::IS_DEAD) != 0)
                {
                    if ((sf_figures->figures[figure_index].debug_flags & 0x2) == 0)
                    {
                        x_sum += sf_figures->figures[figure_index].position.X;
                        y_sum += sf_figures->figures[figure_index].position.Y;
                        count++;
                    }
                }
            }
        }
        figure_index = iteratorAPI.getNextFigure(&iter);
    }

    if (count < 3)
    {
        rank = 0;
    }
    else
    {
        x_sum = x_sum / count;
        y_sum = y_sum / count;
        cast_position->X = x_sum;
        cast_position->Y = y_sum;
    }
    iteratorAPI.disposeFigureIterator(&iter);
    return rank;
}

uint32_t __thiscall area_heal_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    if (((_this->battleData.figures_missing_hp * 100) / _this->battleData.figures_max_hp) >= 4)
    {
        rank = 0;
    }
    return rank;
}

uint32_t __thiscall revenge_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    if (((_this->battleData.figures_missing_hp * 100) / _this->battleData.figures_max_hp) < 5)
    {
        return 0;
    }
    CGdFigureIterator iter;
    iteratorAPI.figureIteratorInit(&iter, 0, 0, 0x3ff, 0x3ff);
    iteratorAPI.figureIteratorSetPointers(&iter, _this->battleData.CGdFigure,
                                          _this->battleData.autoclass22, _this->battleData.CGdWorld);

    iteratorAPI.iteratorSetArea(&iter, cast_position, spell_data->params[2]);
    uint16_t count = 0;
    SF_CGdFigure *sf_figures = _this->battleData.CGdFigure;
    uint16_t figure_index = iteratorAPI.getNextFigure(&iter);
    while (figure_index != 0)
    {
        if (sf_figures->figures[figure_index].owner != (uint16_t)(-1))
        {
            if (sf_figures->figures[figure_index].owner == sf_figures->figures[_this->battleData.current_figure].owner)
            {
                if ((sf_figures->figures[figure_index].flags & GdFigureFlags::IS_DEAD) != 0)
                {
                    if ((sf_figures->figures[figure_index].flags & GdFigureFlags::USED_FOR_REVENGE) != 0)
                    {
                        count++;
                    }
                }
            }
            if (count >= 5)
            {
                break;
            }
        }
        figure_index = iteratorAPI.getNextFigure(&iter);
    }

    if (count < 5)
    {
        rank = 0;
    }
    iteratorAPI.disposeFigureIterator(&iter);
    return rank;
}

uint32_t __thiscall torture_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    CGdFigureIterator iter;
    iteratorAPI.figureIteratorInit(&iter, 0, 0, 0x3ff, 0x3ff);
    iteratorAPI.figureIteratorSetPointers(&iter, _this->battleData.CGdFigure,
                                          _this->battleData.autoclass22, _this->battleData.CGdWorld);

    iteratorAPI.iteratorSetArea(&iter, cast_position, spell_data->params[2]);
    uint16_t count = 0;
    SF_CGdFigure *sf_figures = _this->battleData.CGdFigure;
    uint16_t figure_index = iteratorAPI.getNextFigure(&iter);
    while (figure_index != 0)
    {
        if (sf_figures->figures[figure_index].owner != (uint16_t)(-1))
        {
            if (sf_figures->figures[figure_index].owner != sf_figures->figures[_this->battleData.current_figure].owner)
            {
                if ((sf_figures->figures[figure_index].flags & GdFigureFlags::IS_DEAD) != 0)
                {
                    if ((sf_figures->figures[figure_index].flags & GdFigureFlags::USED_FOR_REVENGE) != 0)
                    {
                        count++;
                    }
                }
            }
            if (count >= 5)
            {
                break;
            }
        }
        figure_index = iteratorAPI.getNextFigure(&iter);
    }

    if (count < 5)
    {
        rank = 0;
    }
    iteratorAPI.disposeFigureIterator(&iter);
    return rank;
}

uint32_t __thiscall fog_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    uint16_t count = 0;
    ushort_list_node *enemy_list = &(_this->battleData.another_figure_list[_this->battleData.current_figure]);
    uint16_t length = ((uint32_t)enemy_list->data - (uint32_t)(enemy_list->first)) >> 1;

    for (uint16_t i = 0; i < length; i++)
    {
        uint16_t target_index = enemy_list->first[i];
        SF_Coord target_pos;
        target_pos.X = _this->battleData.CGdFigure->figures[target_index].position.X;
        target_pos.Y = _this->battleData.CGdFigure->figures[target_index].position.Y;
        uint16_t distance = getDistance(cast_position, &target_pos);
        if (figureAPI.getSpellJobStartNode(_this->battleData.CGdFigure, target_index))
        {
            if (spell_data->params[0] >= distance)
            {
                count++;
            }
        }
        else
        {
            if (!toolboxAPI.hasSpellOnIt(_this->battleData.CGdFigureToolBox, target_index, spell_line))
            {
                if (spell_data->params[0] >= distance)
                {
                    count++;
                }
            }
        }
        if (count > 2)
        {
            break;
        }
    }

    if (count < 3)
    {
        rank = 0;
    }

    return rank;
}

uint32_t __thiscall area_pain_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    return 1;
}

uint32_t __thiscall rain_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    uint16_t count = 0;
    ushort_list_node *enemy_list = &(_this->battleData.another_figure_list[_this->battleData.current_figure]);
    uint16_t length = ((uint32_t)enemy_list->data - (uint32_t)(enemy_list->first)) >> 1;
    uint16_t limit = (length + 2) / 3;
    for (uint16_t i = 0; i < length; i++)
    {
        uint16_t target_index = enemy_list->first[i];
        SF_Coord target_pos;
        target_pos.X = _this->battleData.CGdFigure->figures[target_index].position.X;
        target_pos.Y = _this->battleData.CGdFigure->figures[target_index].position.Y;
        uint16_t distance = getDistance(cast_position, &target_pos);
        if (spell_data->params[2] >= distance)
        {
            count++;
        }
        if (count >= limit)
        {
            break;
        }
    }
    if (count < limit)
    {
        rank = 0;
    }
    return rank;
}

uint32_t __thiscall area_freeze_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    uint16_t count = 0;
    ushort_list_node *enemy_list = &(_this->battleData.another_figure_list[_this->battleData.current_figure]);
    uint16_t length = ((uint32_t)enemy_list->data - (uint32_t)(enemy_list->first)) >> 1;

    for (uint16_t i = 0; i < length; i++)
    {
        uint16_t target_index = enemy_list->first[i];
        SF_Coord target_pos;
        target_pos.X = _this->battleData.CGdFigure->figures[target_index].position.X;
        target_pos.Y = _this->battleData.CGdFigure->figures[target_index].position.Y;
        uint16_t distance = getDistance(cast_position, &target_pos);
        if (figureAPI.getSpellJobStartNode(_this->battleData.CGdFigure, target_index))
        {
            if (spell_data->params[1] >= distance)
            {
                count++;
            }
        }
        else
        {
            if (!toolboxAPI.hasSpellOnIt(_this->battleData.CGdFigureToolBox, target_index, spell_line))
            {
                if (spell_data->params[1] >= distance)
                {
                    count++;
                }
            }
        }
        if (count > 2)
        {
            break;
        }
    }

    if (count < 3)
    {
        rank = 0;
    }

    return rank;
}

uint32_t __thiscall hypnotize_area_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    uint16_t count = 0;
    ushort_list_node *enemy_list = &(_this->battleData.another_figure_list[_this->battleData.current_figure]);
    uint16_t length = ((uint32_t)enemy_list->data - (uint32_t)(enemy_list->first)) >> 1;

    for (uint16_t i = 0; i < length; i++)
    {
        uint16_t target_index = enemy_list->first[i];
        SF_Coord target_pos;
        target_pos.X = _this->battleData.CGdFigure->figures[target_index].position.X;
        target_pos.Y = _this->battleData.CGdFigure->figures[target_index].position.Y;
        uint16_t distance = getDistance(cast_position, &target_pos);
        if (figureAPI.getSpellJobStartNode(_this->battleData.CGdFigure, target_index))
        {
            if (spell_data->params[3] >= distance)
            {
                count++;
            }
        }
        else
        {
            if (!toolboxAPI.hasSpellOnIt(_this->battleData.CGdFigureToolBox, target_index, spell_line))
            {
                if (spell_data->params[3] >= distance)
                {
                    count++;
                }
            }
        }
        if (count > 2)
        {
            break;
        }
    }

    if (count < 3)
    {
        rank = 0;
    }

    return rank;
}

uint32_t __thiscall default_aoe_offensive_ai_handler(SF_CGdBattleDevelopment *_this, SF_Coord *cast_position, uint16_t spell_line, SF_CGdResourceSpell *spell_data)
{
    uint32_t rank = 1;
    uint16_t count = 0;
    ushort_list_node *enemy_list = &(_this->battleData.another_figure_list[_this->battleData.current_figure]);
    uint16_t length = ((uint32_t)enemy_list->data - (uint32_t)(enemy_list->first)) >> 1;

    for (uint16_t i = 0; i < length; i++)
    {
        uint16_t target_index = enemy_list->first[i];
        SF_Coord target_pos;
        target_pos.X = _this->battleData.CGdFigure->figures[target_index].position.X;
        target_pos.Y = _this->battleData.CGdFigure->figures[target_index].position.Y;
        uint16_t distance = getDistance(cast_position, &target_pos);
        if (figureAPI.getSpellJobStartNode(_this->battleData.CGdFigure, target_index))
        {
            if (spell_data->params[2] >= distance)
            {
                count++;
            }
        }
        else
        {
            if (!toolboxAPI.hasSpellOnIt(_this->battleData.CGdFigureToolBox, target_index, spell_line))
            {
                if (spell_data->params[2] >= distance)
                {
                    count++;
                }
            }
        }
        if (count > 2)
        {
            break;
        }
    }

    if (count < 3)
    {
        rank = 0;
    }

    return rank;
}