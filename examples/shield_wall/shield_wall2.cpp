#include "api/sfsf.h"
#include <windows.h>
#include <stdio.h>

#define SHIELD_WALL_LINE 0xf3
#define SHIELD_WALL_JOB 0xaa
#define PARRY_LINE 0xf4
#define PARRY_JOB 0xab

SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;
IteratorFunctions *iteratorAPI;
RegistrationFunctions *registrationAPI;
SFLog *logger;

/* debug output example
Let it be here
char aliveInfo[256];
sprintf(aliveInfo, "Flags list: Target %hd \n", target_index);
sfsf->logInfo(aliveInfo);
*/

void __thiscall parry_type_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    _this->active_spell_list[spell_index].spell_job = PARRY_JOB;
    spellAPI->setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
    spellAPI->setXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER, 0);
}

void __thiscall parry_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    logger->logInfo("PARRY HAS ENDED");
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    uint16_t target_index = spell->target.entity_index;
    spellAPI->removeDLLNode(_this, spell_index);
    spellAPI->setEffectDone(_this, spell_index, 0); // this function actually ends a spell and can be used within any other handler in order to end a spell
    uint16_t recalc_value = spellAPI->getXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER);
    figureAPI->addBonusMultToStatistic(_this->SF_CGdFigure, ARMOR, target_index, -recalc_value);

    // legacy of flat out increase
    //_this->SF_CGdFigure->figures[target_index].armor.bonus_val += -recalc_value;
}

void __thiscall parry_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];

    uint16_t source_index = spell->source.entity_index;
    uint16_t target_index = spell->target.entity_index;

    uint32_t current_tick = spellAPI->getXData(_this, spell_index, SPELL_TICK_COUNT_AUX);
    spellAPI->addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1);
    SF_CGdResourceSpell spell_data;
    spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);
    uint16_t ticks_interval = spell_data.params[1];



    if (current_tick == 0)
    {
        uint16_t figure_ac = _this->SF_CGdFigure->figures[target_index].armor.base_val + _this->SF_CGdFigure->figures[target_index].armor.bonus_val;


        //uint16_t recalc_value = ((spell_data.params[0] - figure_ac) / figure_ac);

        //i don't really get it, but it rounds value to 0 unless we use intermediary float variable
        //also have to pass calculations one by one, or same bug occurs, gonna deal with it later

        double recalc_temp = spell_data.params[0] - figure_ac;
        recalc_temp = recalc_temp / figure_ac;
        recalc_temp *= 100;

        //uint16_t recalc_value = uint16_t((spell_data.params[0] - figure_ac) / figure_ac * 100);
        uint16_t recalc_value = uint16_t (recalc_temp);





        // due to engine limitations, maximum bonus multiplier is limited to 100% or -100%
        // let's nudge this value before it went overflow
        if (recalc_value > 100)
            recalc_value = 100;
        else
            if (recalc_value < - 100)
            recalc_value = -100;



        figureAPI->addBonusMultToStatistic(_this->SF_CGdFigure, ARMOR, target_index, recalc_value);

        // _this->SF_CGdFigure->figures[target_index].armor.bonus_val += recalc_value;
        // alternatively it could be possible to add value as flat bonus with bonus_val
        // but won't work properly, because this key directly overwritten by worn armor

        spellAPI->setXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER, recalc_value);
        _this->active_spell_list[spell_index].to_do_count = (uint16_t)((ticks_interval * 10) / 1000);
        // trick with todocount
        return;
    }
    else
    {
        uint16_t recalc_value = spellAPI->getXData(_this, spell_index, SPELL_STAT_MUL_MODIFIER);
        figureAPI->addBonusMultToStatistic(_this->SF_CGdFigure, ARMOR, target_index, -recalc_value);


        // keeping it to show how it could be possibly done in flat way
        // but it can't be actually used due to interference with other instances using bonus_val as well
        //_this->SF_CGdFigure->figures[target_index].armor.bonus_val += -recalc_value;

        char aliveInfo[256];
        sprintf(aliveInfo, "SUSBTRACTING PERCENTILE BONUS %hd \n", recalc_value);
        logger->logWarning(aliveInfo);
        spellAPI->setEffectDone(_this, spell_index, 0);
        return;
    }
}

void __thiscall shield_wall_type_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    _this->active_spell_list[spell_index].spell_job = SHIELD_WALL_JOB;
    spellAPI->setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall shield_wall_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];


    SF_SpellEffectInfo effect_info;
    SF_CGdResourceSpell spell_data;
    effect_info.spell_id = spell->spell_id;
    effect_info.job_id = spell->spell_job;
    SF_Rectangle hit_area;
    uint16_t source_index = _this->active_spell_list[spell_index].source.entity_index;
    SF_Coord cast_center;
    //SF_Coord cast_center = _this->active_spell_list[spell_index].source.position;
    figureAPI->getPosition(_this->SF_CGdFigure, &cast_center, source_index);

    // SF_Coord
    spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);
    SF_CGdTargetData relative_data;
    relative_data.position = {_this->active_spell_list[spell_index].source.position.X, _this->active_spell_list[spell_index].source.position.Y};
    relative_data.entity_type = 4;
    relative_data.entity_index = 0;
    uint32_t unused;
    spellAPI->getTargetsRectangle(_this, &hit_area, spell_index, spell_data.params[0], &cast_center);
    spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitWorld, &unused, &relative_data, _this->OpaqueClass->current_step, 0x19, &hit_area);

    uint16_t figure_count = spell_data.params[1];



    // Iterators are opaque from the user perspective.
    // Just give enough memory and don't bother what's inside
    CGdFigureIterator figure_iterator;
    iteratorAPI->setupFigureIterator(&figure_iterator, _this);
    iteratorAPI->iteratorSetArea(&figure_iterator, &cast_center, spell_data.params[0]); // radius should be really big, 150 higher
    uint16_t target_index = iteratorAPI->getNextFigure(&figure_iterator);

    while (target_index != 0 && figure_count != 0)
    {
        /*char aliveInfo[256];
        sprintf(aliveInfo, "ADDING SHIELDWALL TO FIGURE NUMBER %hd \n", target_index);
        logger->logWarning(aliveInfo);*/
        spellAPI->checkCanApply(_this, spell_index);

        //shield_wall_refresh_handler(_this, spell_index);
        uint16_t isAlive = figureAPI->isAlive(_this->SF_CGdFigure, target_index);
        uint16_t isTargetable = toolboxAPI->isTargetable(_this->SF_CGdFigureToolBox, target_index);
        uint16_t isOwner = _this->SF_CGdFigure->figures[source_index].owner - _this->SF_CGdFigure->figures[target_index].owner;

        if (((int16_t)(_this->SF_CGdFigure->figures[target_index].owner) == (int16_t)(_this->SF_CGdFigure->figures[source_index].owner)) &&
            (((uint8_t)(_this->SF_CGdFigure->figures[target_index].flags) & 0xa) == 0) &&
            (toolboxAPI->isTargetable(_this->SF_CGdFigureToolBox, target_index)))
        {
            SF_CGdTargetData source = {spell->source.entity_type, source_index, {0, 0}};
            SF_CGdTargetData target = {spell->target.entity_type, target_index, {0, 0}};
            spellAPI->addSpell(_this, spell_data.params[3], _this->OpaqueClass->current_step, &source, &target, 0);
            figure_count--;
        }
        target_index = iteratorAPI->getNextFigure(&figure_iterator);
    }
    spellAPI->setEffectDone(_this, spell_index, 0);

    iteratorAPI->disposeFigureIterator(figure_iterator);
}

int __thiscall shield_wall_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index) //we casted shieldwall again before the previous expired
{
 _this->active_spell_list[spell_index].spell_job = PARRY_JOB;
 SF_GdSpell *spell = &_this->active_spell_list[spell_index];
 uint16_t target_index = spell->target.entity_index;
 SF_CGdResourceSpell spell_data;
 spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);
 SF_CGdResourceSpell spell_data_2;
 spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data_2, spell_data.params[3]);

uint16_t spell_line = _this->active_spell_list[spell_index].spell_line;

 if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, spell_data_2.spell_line_id))
    {
        logger->logInfo("ALREADY HAS PARRY, REMOVING IT TO MAKE SPACE FOR NEW ONE");
        uint16_t parry_spell_id = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox, target_index, PARRY_LINE, spell_index);
        spellAPI->setEffectDone(_this, parry_spell_id, 0);
        return 0;

    }
 else
    {
        logger->logInfo("CAN APPLY PARRY TO FIGURE");
        return 1;
    }
}



/***
 * This function MUST be present in your code with the exact declaration
 * otherwise framework won't load your plugin
 ***/
extern "C" __declspec(dllexport) void InitModule(SpellforceSpellFramework *framework)
{
    sfsf = framework;
    spellAPI = sfsf->spellAPI;
    toolboxAPI = sfsf->toolboxAPI;
    figureAPI = sfsf->figureAPI;
    iteratorAPI = sfsf->iteratorAPI;
    registrationAPI = sfsf->registrationAPI;
    logger = sfsf->logAPI;

    SFSpell *shield_wall_spell = registrationAPI->registerSpell(SHIELD_WALL_LINE);
    registrationAPI->linkTypeHandler(shield_wall_spell, &shield_wall_type_handler);
    registrationAPI->linkEffectHandler(shield_wall_spell, SHIELD_WALL_JOB, &shield_wall_effect_handler);
    registrationAPI->linkRefreshHandler(shield_wall_spell, &shield_wall_refresh_handler);

    SFSpell *parry_spell = registrationAPI->registerSpell(PARRY_LINE);
    registrationAPI->linkTypeHandler(parry_spell, &parry_type_handler);
    registrationAPI->linkEffectHandler(parry_spell, PARRY_JOB, &parry_effect_handler);
    registrationAPI->linkEndHandler(parry_spell, &parry_end_handler);
}

/***
 * This function MUST be present in your code with the exact declaration
 * otherwise the framework is unable to describe your mod in logs and mod info menu (menu not yet implemented)
 ***/
extern "C" __declspec(dllexport) SFMod *RegisterMod(SpellforceSpellFramework *framework)
{
    return framework->createModInfo("Shield wall Mod", "1.0.0", "UnSchtalch", "A mod designed to provide an example combat ability");
}

// Required to be present, not required for any functionality
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        /* Code path executed when DLL is loaded into a process's address space. */
        break;

    case DLL_THREAD_ATTACH:
        /* Code path executed when a new thread is created within the process. */
        break;

    case DLL_THREAD_DETACH:
        /* Code path executed when a thread within the process has exited *cleanly*. */
        break;

    case DLL_PROCESS_DETACH:
        /* Code path executed when DLL is unloaded from a process's address space. */
        break;
    }

    return TRUE;
}
