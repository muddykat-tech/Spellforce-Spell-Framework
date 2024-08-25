#include "../../api/sfsf.h"
#include <windows.h>
#include <stdio.h>


// We declare macros for Spell Type and Spell Job of both spells
// it is very unhandy to keep them in mind, so we just declare them here
// we use a new set of numbers to avoid interfering with previous examples

// New Spell Type will be 0xf5 (245), it must be defined within GameData.cff
// and provided with at least one spell corresponding each Spell Type
// new Spell Job will be 0xac (172) to avoid interfering with numbers used in previous examples

#define INTERFERENCE_LINE 0xf5
#define INTERFERENCE_JOB 0xac

// we also need to know Spell Types for vanilla spells which will be mutually exclusive with Interference
// let's get them from GameData.cff and write down as hexadecimals

#define PATRONIZE_LINE 0x96 // 150
#define SHELTER_LINE 0x9A // 154



SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;
IteratorFunctions *iteratorAPI;
RegistrationFunctions *registrationAPI;
SFLog *logger;


//we declare spell type handler for Interference
void __thiscall interference_type_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // we link the specific spell type with its own spell job
    _this->active_spell_list[spell_index].spell_job = INTERFERENCE_JOB;
    // we initialize spell duration timer
    spellAPI->setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

// we declare spell end handler for Interference
// this handler would work in case a spell wasn't finished correctly
void __thiscall interference_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    spellAPI->removeDLLNode(_this, spell_index); // we remove spell from the list of active spells affecting the target
    spellAPI->setEffectDone(_this, spell_index, 0); // we end a spell
}


// we are used to the fact that spell logic is implemented within the spell effect handler
// however, interference is very special, its main logic (reducing incoming damage) will be trigged within Damage handler
// it's declared in the following way
// the handler intercepts the incoming damage, we can modify it and return the new value
// then the game will proceed with applying the modified damage to the target's health as usual
// if we don't modify the damage due to some reasons (for example, the spell doesn't absorb damage from ranged attacks), we still should return the initial value with the function
// otherwise the damage will be lost

uint16_t __thiscall interference_deal_damage_handler(SF_CGdFigureToolbox *_toolbox, uint16_t source, uint16_t target,
                                    uint16_t current_damage, uint16_t is_spell_damage, uint32_t is_ranged_damage, uint16_t spell_id)
{

// _toolbox stands for the unit which is affected with our spell and got damaged with any means
// source specifies the figure_index of a damaging figure
// target specifies the figure_index of a damaged figure
// current_damage specifies the amount of damage
// is_spell_damage and is_ranged_damage specify the damage origin
// spell_id returns an ID of the spell which triggered the Damage handler

    if (is_spell_damage)
        //we check if the damage which trigged Damage handler comes from the spell
    {
        // we load spell_data of Interference spell, spell_id stands for the ID of the spell which governs the Damage handler
        SF_CGdResourceSpell spell_data;
        spellAPI->getResourceSpellData(_toolbox->CGdResource, &spell_data, spell_id);


        // we decrease damage by the percentage stated in spell parameters
        current_damage += -(uint16_t)((current_damage * spell_data.params[0]) / 100);
        // we return the modified damage value and game engine uses the modified value as usual
        logger->logInfo("INTERFERENCE REDUCED DAMAGE");
        return current_damage;
    }

    // made it 0 for all kinds of damage just for debug purpose
    //current_damage = 0;

    logger->logInfo("INTERFERENCE REGISTERED FULL DAMAGE");

    // the damage wasn't caused with the spell, we return damage value to game engine unchanged
    return current_damage;
}

// we declare effect handler for the Interference
// in this case effect handler will only govern the spell duration, and will remove the spell when the specified amount of time passes
// the bulk of spell logic is implemented in damage handler above

void __thiscall interference_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // we pull the pointer for this instance of spell
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];

    // we store index of the spellcaster, we won't need target index for this spell
    uint16_t source_index = _this->active_spell_list[spell_index].source.entity_index;

    // we have to activate the flag F_CHECK_SPELLS_BEFORE_JOB in order to make it possible for Deal Damage handler to trigger when the damage is received
    _this->SF_CGdFigure->figures[source_index].flags |= F_CHECK_SPELLS_BEFORE_JOB;
    //bool check_spells_before_job = figureAPI->isFlagSet(figureToolbox->CGdFigure, source_index, F_CHECK_SPELLS_BEFORE_JOB);

    // we get the current tick of the spell, should be 0 at the beginning, and 1 in the end
    uint32_t current_tick = spellAPI->getXData(_this, spell_index, SPELL_TICK_COUNT_AUX);
    // we increase amount of ticks passed by 1
    spellAPI->addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1);


    // we load the spell from GameData.cff in order to get tick_interval
    SF_CGdResourceSpell spell_data;
    spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);



    if (current_tick == 0)
        //spell start
        {
            // let's trigger refresh for interference or other vanilla spells which work in the same way
            spellAPI->checkCanApply(_this, spell_index);
            // the refresh handler will clear them from the target if either of them is currently affecting the target


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
            spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitWorld, &unused, &relative_data, _this->OpaqueClass->current_step, 0x5, &aux_data);


            // let's get spell duration from game data
            uint16_t ticks_interval = spell_data.params[3];
            // we disable the spell from being triggered for a specified number of internal game ticks, and after a specified in ticks_interval amount of time has passed, we may remove the spell
            _this->active_spell_list[spell_index].to_do_count = (uint16_t)((ticks_interval * 10) / 1000);
            logger->logInfo("INTERFERENCE ACTIVATED");
        }
    else
        // spell end, main spell logic is implemented with another handlerso we have only to clear flag which allowed triggering damage handler and stop the spell in this block
        {
            // it's better to clear two of the following flags for the sake of optimization
            spellAPI->figTryClrCHkSPlBfrJob2(_this, spell_index);
            spellAPI->figClrChkSplBfrChkBattle(_this, spell_index, 0);
            spellAPI->setEffectDone(_this, spell_index, 0); // we end a spell
            logger->logInfo("INTERFERENCE FINISHED");
        }
}


// we declare refresh handler for group of three spells: Interference, Patronize, Shelter
// they provide the same effect, hence we have to prevent them from stacking
// in the previous example we implemented that the mechanics according to which spell blocks the casting if it's already affecting the target
// here we will remove already affecting spells and make the figure clear for applying new spell
// the handler is the same for all three spells: custom one, and shelter and patronize from vanilla

int __thiscall interference_patronize_shelter_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index) //we casted shieldwall group again before the previous expired
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];

    // the spell is cast on self, we can use the source index
    uint16_t source_index = spell->source.entity_index;
    logger->logInfo("TRIPLE CHECK ACTIVATED");

    // below we'll check spells over the target one by one and remove them
    // function hasSpellOnIt requires Spell Type ID to identify the spell
    // we have Spell Types defind with macros, so we don't need any additional steps to get it
    if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, source_index, INTERFERENCE_LINE))
       // the INTERFERENCE spell already exists on the target
        {
            // here comes the magic
            // we can get the spell index from the list of active spells affecting the target by knowing this spell's spell job id
            // we pass spell_index given with the refresh handler as the last known spell index for this figure, but this value is insignificant (used for optimizing search purposes?)

            uint16_t pruned_spell_index = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox, source_index, INTERFERENCE_LINE, spell_index);

            // we finish the INTERFERENCE spell using its spell index
            // we should remove the spell correctly, so we remove both Effect and DLLNode here
            spellAPI->removeDLLNode(_this, pruned_spell_index);
            spellAPI->setEffectDone(_this, pruned_spell_index, 0);
            logger->logInfo("INTERFERENCE WAS REFRESHED");
        }

    if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, source_index, PATRONIZE_LINE))
       // the PATRONIZE spell already exists on the target
        {
            uint16_t pruned_spell_index = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox, source_index, PATRONIZE_LINE, spell_index);
            spellAPI->removeDLLNode(_this, pruned_spell_index);
            spellAPI->setEffectDone(_this, pruned_spell_index, 0);
            logger->logInfo("PATRONIZE WAS REFRESHED");
        }

    if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, source_index, SHELTER_LINE))
       // the SHELTER spell already exists on the target
        {
            uint16_t pruned_spell_index = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox, source_index, SHELTER_LINE, spell_index);
            spellAPI->removeDLLNode(_this, pruned_spell_index);
            spellAPI->setEffectDone(_this, pruned_spell_index, 0);
            logger->logInfo("SHELTER WAS REFRESHED");
        }

    // we return 1 to show that the refresh function allows to apply the new spell to the target
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
    logger = sfsf->logAPI;


    // we register handlers for the new custom spell
    // in this example we introduce new handler - deal damage handler
    // this handler will be called to implement game logic for situations when we cast spell on a unit which is already affected by this spell
    SFSpell *interference_spell = registrationAPI->registerSpell(INTERFERENCE_LINE);
    registrationAPI->linkTypeHandler(interference_spell, &interference_type_handler);
    registrationAPI->linkEffectHandler(interference_spell, INTERFERENCE_JOB, &interference_effect_handler);
    registrationAPI->linkRefreshHandler(interference_spell, &interference_patronize_shelter_refresh_handler);
    registrationAPI->linkDealDamageHandler(interference_spell, &interference_deal_damage_handler, SpellDamagePhase::PRE);
    registrationAPI->linkEndHandler(interference_spell, &interference_end_handler);

    // SpellDamagePhase::PRE controls when damage is registered
    // there are three possible phases: PRE, default, post
    // PRE stands for initial damage, not modified by any other spells
    // default stands for damage modified by other spells
    // post stands for the damage in the end of all calculations, after other spells and armor reductions were applied


    // the Interference might interfere (pun was not intended) with the effect provided with Shelter or Patronize vanilla spells
    // those spells already have own refresh handler, which can be checked in
    // https://github.com/muddykat-tech/Spellforce-Spell-Framework/blob/master/src/internal/handlers/sf_spellrefresh_handlers.cpp

    // however, this handler won't check for the Interference spell
    // to fix this omission, we must use the custom refresh handler for the interferring vanilla spells
    // because all three spells will adhere to the same logic, we can link the same refresh handler which we linked above
    // we don't have to touch all other handlers like Spell Type, it's enough only to link only the refresh handler
    // it's worth of mentioning that the custom refresh handler will replace the existing vanilla handler, they won't work simultaneously

    SFSpell *patronize_spell = registrationAPI->registerSpell(PATRONIZE_LINE);
    registrationAPI->linkRefreshHandler(patronize_spell, &interference_patronize_shelter_refresh_handler);

    SFSpell *shelter_spell = registrationAPI->registerSpell(SHELTER_LINE);
    registrationAPI->linkRefreshHandler(shelter_spell, &interference_patronize_shelter_refresh_handler);
}

/***
 * This function MUST be present in your code with the exact declaration
 * otherwise the framework is unable to describe your mod in logs and mod info menu (menu not yet implemented)
 ***/
extern "C" __declspec(dllexport) SFMod *RegisterMod(SpellforceSpellFramework *framework)
{
    return framework->createModInfo("Interference mod", "1.0.0", "S'Baad", "This mod provides an example of an Interference which negates 20% of incoming damage from all spells.");
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
