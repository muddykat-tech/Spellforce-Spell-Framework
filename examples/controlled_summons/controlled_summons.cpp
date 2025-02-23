#include "../../src/api/sfsf.h"
#include <windows.h>
#include <stdio.h>

#define SUMMON_SPELL_LINE 0x14
#define SUMMON_SPELL_JOB 0x17

SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;
IteratorFunctions *iteratorAPI;
RegistrationFunctions *registrationAPI;
SFLog *logger;

bool __thiscall isTower(SF_CGdFigure *_this, uint16_t figure_index)
{
    return _this->figures[figure_index].flags >> 11 & 1;
}

bool __thiscall isInMapBounds(SF_CGdWorld *_this, SF_Coord *position)
{
    if ((position->X < _this->map_size) && (position->Y < _this->map_size))
    {
        return 1;
    }
    return 0;
}

uint16_t __thiscall getSector(SF_CGdWorld *_this, SF_Coord *position)
{
    uint32_t index = position->X + position->Y * 0x400;
    return _this->cells[index].sector;
}

uint16_t summonCreature(SF_CGdFigureToolbox *_this, uint16_t master_index, uint16_t creature_type)
{
    SF_Coord master_postion;
    SF_Coord summon_pos;
    uint16_t summon_index = 0;
    figureAPI->getPosition(_this->CGdFigure, &master_postion, master_index);
    summon_pos.X = master_postion.X;
    summon_pos.Y = master_postion.Y;
    if (isTower(_this->CGdFigure, master_index))
    {
        char message[256];
        sprintf(message, "Figure_id : %d is not tower", master_index);
        logger->logInfo(message);

        for (int i = 0; i < 9999; i++)
        {
            uint16_t someX = _this->CGdWorld->unknown1[i].uknwn1;
            uint16_t someY = _this->CGdWorld->unknown1[i].uknwn2;
            SF_Coord new_pos = {someX, someY};
            new_pos.X += master_postion.X;
            new_pos.Y += master_postion.Y;
            if (isInMapBounds(_this->CGdWorld, &new_pos) && getSector(_this->CGdWorld, &new_pos))
            {
                summon_pos.X = new_pos.X;
                summon_pos.Y = new_pos.Y;
                break;
            }
        }
    }
    char message[256];
    sprintf(message, "Summon pos %hd %hd", summon_pos.X, summon_pos.Y);
    logger->logInfo(message);

    uint16_t sector = getSector(_this->CGdWorld, &summon_pos);
    SF_Coord offset = {1, 4};
    SF_Coord real_pos = {0, 0};
    if (toolboxAPI->findClosestFreePosition(_this->CGdWorldToolBox, &summon_pos, &offset, sector, &real_pos))
    {
        uint16_t owner = _this->CGdFigure->figures[master_index].owner;
        summon_index = toolboxAPI->addUnit(_this, real_pos.X, real_pos.Y, owner, creature_type, 0x13, 0,  _this->CGdFigure->figures[master_index].faction, 0);
        //figureAPI->setTask
        _this->CGdFigure->figures[summon_index].race = _this->CGdFigure->figures[master_index].race;
    }
    return summon_index;
}

void __thiscall summon_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdResourceSpell spell_data;
    // we pull the pointer for this instance of spell
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);
    uint32_t tick_number = spellAPI->getXData(_this, spell_index, SPELL_TICK_COUNT_AUX);
    if (tick_number == 0)
    {
        uint16_t target_index = summonCreature(_this->SF_CGdFigureToolBox, spell->source.entity_index, spell_data.params[2]);
        if (target_index != 0)
        {
            toolboxAPI->addSpellToFigure(_this->SF_CGdFigureToolBox, target_index, spell_index);
            spellAPI->addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1);
            _this->active_spell_list[spell_index].to_do_count = (spell_data.params[0] * 10) / 1000;
            _this->active_spell_list[spell_index].target.entity_index = target_index;
            _this->active_spell_list[spell_index].target.entity_type = 1;
            _this->active_spell_list[spell_index].target.position.X = 0;
            _this->active_spell_list[spell_index].target.position.Y = 0;
            return;
        }
    }
    else
    {
        uint16_t current_mana = figureAPI->getCurrentMana(_this->SF_CGdFigure, spell->source.entity_index);
        if (current_mana >= spell_data.params[1])
        {
            _this->active_spell_list[spell_index].to_do_count = (spell_data.params[0] * 10) / 1000;
            figureAPI->subMana(_this->SF_CGdFigure, spell->source.entity_index, spell_data.params[1]);
            return;
        }
        else
        {
            SF_Rectangle some_rect = {0, 0};
            SF_CGdTargetData target;
            target.entity_index = spell->target.entity_index;
            target.position = {0, 0};
            target.entity_type = 1;
            uint32_t unused;
            spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitTarget,
                                      &unused, &target, _this->OpaqueClass->current_step, 0x19, &some_rect);
            toolboxAPI->dealDamage(_this->SF_CGdFigureToolBox, 0, spell->target.entity_index, 0x7FFF, 1, 0, 0);
            return;
        }
    }
    spellAPI->setEffectDone(_this, spell_index, 0);
}

/***
 * This function MUST be present in your code with the exact declaration
 * otherwise framework won't load your plugin
 ***/
extern "C" __declspec(dllexport) void InitModule(SpellforceSpellFramework *framework)
{
    // we pull pointers to framework methods and structures
    sfsf = framework;
    spellAPI = sfsf->spellAPI;
    toolboxAPI = sfsf->toolboxAPI;
    figureAPI = sfsf->figureAPI;
    iteratorAPI = sfsf->iteratorAPI;
    registrationAPI = sfsf->registrationAPI;
    logger = sfsf->logAPI;

    SFSpell *summon_spell = registrationAPI->registerSpell(SUMMON_SPELL_LINE);                  // just pull any summoning spell
    registrationAPI->linkEffectHandler(summon_spell, SUMMON_SPELL_JOB, &summon_effect_handler); // and overwrite it's effect handler which is common
}

/***
 * This function MUST be present in your code with the exact declaration
 * otherwise the framework is unable to describe your mod in logs and mod info menu (menu not yet implemented)
 ***/
extern "C" __declspec(dllexport) SFMod *RegisterMod(SpellforceSpellFramework *framework)
{
    return framework->createModInfo("Controlled Summons mod", "1.0.0", "Un'Schtalch", "Done specifically for General Kong");
}

// Required to be present by, not required for any functionality
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
