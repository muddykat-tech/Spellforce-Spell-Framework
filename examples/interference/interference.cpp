#include "api/sfsf.h"
#include <windows.h>
#include <stdio.h>


// We declare macros for Spell Type and Spell Job of both spells
// it is very unhandy to keep them in mind, so we just declare them here
// we use a new set of numbers to avoid interfering with previous examples
// 0xf3 = 243, 0xaa = 170, 0xf4 = 244, 0xab = 171

// The custom Spell Types (243 and 244) also must be defined within GameData.cff
// and provided with at least one spell corresponding each Spell Type

#define INTERFERENCE_LINE 0xf5
#define INTERFERENCE_JOB 0xac

// we also need to know Spell Types for vanilla spells which will be mutually exclusive with interference
// let's get them from GameData.cff and write down as hexadecimals

#define PATRONIZE_LINE 0x96 // 150
#define SHELTER_LINE 0x9A // 154



SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;
IteratorFunctions *iteratorAPI;
RegistrationFunctions *registrationAPI;

//SFLog *logger

/* debug output example
Let it be here
char aliveInfo[256];
sprintf(aliveInfo, "Flags list: Target %hd \n", target_index);
logger->logInfo(aliveInfo);
*/

//we declare Parry Spell Type handler
void __thiscall interference_type_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // we link the specific spell type with its own spell job
    _this->active_spell_list[spell_index].spell_job = INTERFERENCE_JOB;
    // we initialize ticks counter
    spellAPI->setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

// we declare spell end handler for Parry
// this handler would work in case a spell wasn't finished correctly
void __thiscall interference_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    spellAPI->removeDLLNode(_this, spell_index); // we remove spell from the list of active spells over the target
    spellAPI->setEffectDone(_this, spell_index, 0); // we end a spell
}


// we used that spell logic is implemented within the spell effect handler
// however, interference is very special, its main logic will be trigged in deal_damage handler

void __thiscall interference_damage_handler(SF_CGdFigureToolbox *_toolbox, uint16_t source, uint16_t target,
                                    uint16_t current_damage, uint16_t is_spell_damage, uint32_t is_ranged_damage, uint16_t spell_id)
{

    if (is_spell_damage)
    {
        SF_CGdResourceSpell spell_data;
        spellAPI.getResourceSpellData(_this->CGdResource, &spell_data, spell_id);
        current_damage = (uint16_t)((current_damage * spell_data.params[1]) / 100);
        return current_damage;
    }

    return current_damage;
}

// we declare spell effect handler which implements spell affecting an area
// the AoE checks for a specified amount of targets in a certain radius around the spellcaster
// if the spell happens to affect again before the previous instance expired, it resets its duration
// spell effect is simulated by individual instances of SHIELDWALL spell applied to every target affected
// this code can be used as snippet for any melee group ability that affects the caster and up to N targets nearby

void __thiscall interference_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // we pull the pointer for this instance of spell
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];

    // we store index of the spellcaster, we won't need target index for this spell
    uint16_t source_index = _this->active_spell_list[spell_index].source.entity_index;

    // we get the current tick of the spell, should be 0 at the beginning, and 1 in the end
    uint32_t current_tick = spellAPI->getXData(_this, spell_index, SPELL_TICK_COUNT_AUX);
    // we increase amount of ticks passed by 1
    spellAPI->addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1);


    // we load the spell parameters from GameData.cff
    // we'll use them later
    SF_CGdResourceSpell spell_data;
    spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);




    if (tick_current == 0)
        //spell start
        {
            // let's trigger refresh for interference or other vanilla spells which work in the same way
            // the refresh handler will clear them from the target if either of them is currently affecting the target
            spellAPI->checkCanApply(_this, spell_index);

            // we declare structures to store relative position of visual effect
            SF_CGdTargetData relative_data;
            figureAPI->getPosition(_this->SF_CGdFigure, &relative_data.position, source_index);
            relative_data.entity_type = 4;
            relative_data.entity_index = 0;
            uint32_t unused;

            SF_Rectangle aux_data;
            aux_data.partA = 0;
            aux_data.partB = 0;

            // we apply the visual effect filling the area which we specified above
            spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitWorld, &unused, &relative_data, _this->OpaqueClass->current_step, 0x19, &aux_data);


            // let's get spell duration from game data
            ticks_interval = spell_data.params[1];
            // we disable the spell from being triggered for a specified number of internal game ticks, and after a specified in ticks_interval amount of time has passed, we may remove the spell
            _this->active_spell_list[spell_index].to_do_count = (uint16_t)((ticks_interval * 10) / 1000);
        }
    else
        //spell end, main spell logic is implemented with another handler, so in this block we have only to stop the spell
        {
            spellAPI->setEffectDone(_this, spell_index, 0); // we end a spell
        }


    spellAPI->setEffectDone(_this, spell_index, 0);
}


// we declare refresh handler for AoE spell
// this handler is called whenever we're casting shieldwall group, and will return 0 if the spell is already present or 1 when it's not
// this handler can be used as a snippet for any other similar spell which can be refreshed

int __thiscall interference_patronize_shelter_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index) //we casted shieldwall group again before the previous expired
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    // the spell is cast on self, we can use the source index
    uint16_t source_index = spell->source.entity_index;

    // we check whether the figure has the SHIELDWALL spell applied to it already
    // method hasSpellOnIt accepts spell_line_id property of spell data as argument in order to idenfity the spell
    // spell_line_id corresponds to Spell Type ID
    if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, source_index, INTERFERENCE_LINE))
       // the INTERFERENCE spell already exists on the target
        {
            // here comes the magic
            // we can get the spell index from the list of active spells affecting the target by knowing this spell's spell job id
            // we pass SHIELDWALL GROUP spell_index as the last known spell index for this figure, but this value is insignificant (used for optimizing search purposes?)

            uint16_t pruned_spell_index = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox, source_index, INTERFERENCE_LINE, spell_index);

            // we finish the SHIELDWALL spell using its spell index

            spellAPI->setEffectDone(_this, pruned_spell_index, 0);
        }

    if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, source_index, ENDURANCE_LINE))
       // the PATRONIZE spell already exists on the target
        {
            uint16_t pruned_spell_index = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox, source_index, PATRONIZE_LINE, spell_index);
            spellAPI->setEffectDone(_this, pruned_spell_index, 0);
        }

    if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, source_index, DURABILITY_LINE))
       // the SHELTER spell already exists on the target
        {
            uint16_t pruned_spell_index = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox, source_index, SHELTER_LINE, spell_index);
            spellAPI->setEffectDone(_this, pruned_spell_index, 0);
        }

    return 1;
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


    // we register handlers for AoE component of the spell
    // in this example we introduce new handler - deal damage handler
    // this handler will be called to implement game logic for situations when we cast spell on a unit which is already affected by this spell
    SFSpell *interference_spell = registrationAPI->registerSpell(INTERFERENCE_LINE);
    registrationAPI->linkTypeHandler(interference_spell, &interference_type_handler);
    registrationAPI->linkEffectHandler(interference_spell, INTERFERENCE_JOB, &melee_group_ability_effect_handler);
    registrationAPI->linkRefreshHandler(interference_spell, &interference_patronize_shelter_refresh_handler);
    registrationAPI->linkDamageHandler(damage_spell, &interference_damage_deal_handler);
    registrationAPI->linkEndHandler(interference_spell, &interference_end_handler);


    // the Parry might interfere with the effect bestowed by Endurance or Durability vanilla spells
    // we're not planning

    SFSpell *endurance_spell = registrationAPI->registerSpell(PATRONIZE_LINE);
    registrationAPI->linkRefreshHandler(endurance_spell, &interference_patronize_shelter_refresh_handler);

    SFSpell *durability_spell = registrationAPI->registerSpell(SHELTER_LINE);
    registrationAPI->linkRefreshHandler(endurance_spell, &interference_patronize_shelter_refresh_handler);


}

/***
 * This function MUST be present in your code with the exact declaration
 * otherwise the framework is unable to describe your mod in logs and mod info menu (menu not yet implemented)
 ***/
extern "C" __declspec(dllexport) SFMod *RegisterMod(SpellforceSpellFramework *framework)
{
    return framework->createModInfo("Parry mod", "1.0.0", "S'Baad", "This mod provides an example of a Parry spell which negates 20% of incoming damage to a target.");
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
