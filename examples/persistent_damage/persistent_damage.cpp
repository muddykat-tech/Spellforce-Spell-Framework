#include <windows.h>
#include <stdio.h>
#include "../../src/api/sfsf.h"
// NOTE sfsf.h includes the OTHER api files, but the other files are still required

SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;
RegistrationFunctions *registrationAPI;
SFLog *logger;
// This custom spell type and custom spell effect has to be setup both here (to provide logic)
// And in GameData.cff (to provide data and spell stats)

// Spell index is the ID for the TYPE of spell being cast
// Spell Job is the ID for the LOGIC (effect) handler that the spell uses when being cast.
void __thiscall ignite_spelltype_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // Effect ID is the spell_job; Free slots starts from a6. slots 1 to a5 are reserved for vanilla spells

    // here we link certain spell with its spell index to its spell_job
    _this->active_spell_list[spell_index].spell_job = 0xf2;
    // our spell affects the target multiple ticks (times),
    // each new tick happens after a delay defined in the spell data in milliseconds.
    // we need to make sure that our spell begins tracking its ticks starting at 0
    spellAPI->setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
    // here we just put message to log for debug purpose
    logger->logInfo("IGNITE SPELL ACTIVATED");
}


//could be imitated within spelleffect_handler, when the spell should end according to its logic
//It is not necessary in this case, but will play it's role in later chapters
void __thiscall ignite_spellend_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    logger->logInfo("IGNITE HAS ENDED");
    spellAPI->removeDLLNode(_this, spell_index); // this command removes spell from active spells list
    spellAPI->setEffectDone(_this, spell_index, 0); // this function actually ends a spell and can be used within any other handler in order to end a spell
}

void __thiscall ignite_spelleffect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    logger->logInfo("IGNITE EFFECT HANDLED");

    // we pull spell data from spell list.
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];

    // we store source of the spell (a spellcaster) and a target of the spell (a hostile creature)
    uint16_t target_index = spell->target.entity_index;
    uint16_t source_index = spell->source.entity_index;

    // grabbing spell id and the id of its job (logic)
    SF_SpellEffectInfo effect_info;
    effect_info.spell_id = spell->spell_id;
    effect_info.job_id = spell->spell_job;

    // declaring local structure for spell data
    SF_CGdResourceSpell spell_data;
    // we pull spell paraemters into spell_data from GameData.cff
    spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);

    // we declare local variable for damage dealt, we'll use it later
    uint16_t damage;

    // spell is persistent, we check how many ticks it should have and how long a single tick lasts (in milliseconds)
    uint16_t ticks_total = spell_data.params[2];
    uint16_t ticks_interval = spell_data.params[3];


    // we get current tick of our spell, it should be 0 at the start
    uint32_t tick_current = spellAPI->getXData(_this, spell_index, SPELL_TICK_COUNT_AUX);
    // addToXData adds a specified value to actual spell data
    // and immediately returns new value as an integer, so we can put it in local variable
    // we update amount of ticks passed, but then we return to our current tick, because we haven't executed logic for current tick yet
    uint16_t ticks_passed = spellAPI->addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1) - 1;


    // setting flags to check whether spell is allowed to be used on the target
    uint16_t isAlive = figureAPI->isAlive(_this->SF_CGdFigure, target_index);
    uint16_t isTargetable = toolboxAPI->isTargetable(_this->SF_CGdFigureToolBox, target_index);
    uint16_t isHostile = toolboxAPI->figuresCheckHostile(_this->SF_CGdFigureToolBox, source_index, target_index);
    // we also can check figure properties by directly accessing them instead of using a function
    uint16_t isOwner = _this->SF_CGdFigure->figures[target_index].owner;

    // we've got a lot of technical conditions which can prevent spell cast, so if we don't meet at least one of them, spell fails
    // target must be alive, must be targetable, must be hostile, must not be our own unit
    if (isAlive != 0 && isTargetable != 0 && isHostile != 0 && isOwner != -1)
    {
        // initial stage of spell
        if (tick_current == 0)
        {
            // roll only once for the resist
            uint32_t resist_chance = spellAPI->getChanceToResistSpell(_this->unkn2, source_index, target_index, effect_info);
            uint16_t random_roll = spellAPI->getRandom(_this->OpaqueClass, 100);
            if (resist_chance < random_roll)
            {
                uint32_t unused;
                SF_CGdTargetData relative_data;
                relative_data.position.X = 0;
                relative_data.position.Y = 0;
                relative_data.entity_type = 1;
                relative_data.entity_index = target_index;

                SF_Rectangle aux_data;
                aux_data.partA = 0;
                aux_data.partB = 0;
                spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitTarget, &unused, &relative_data, _this->OpaqueClass->current_step, 10, &aux_data);
                // all this data is necessary to center visual effect on the unit

                // we get initial damage of the spell
                damage = spell_data.params[0];
                // we apply damage to figure (an object which stores unit stats)
                toolboxAPI->dealDamage(_this->SF_CGdFigureToolBox, source_index, target_index, damage, 1, 0, 0);
                // we make hostile creature aggro after being damaged
                spellAPI->figureAggro(_this, spell_index, target_index);
            }
            else
            {
                //the spell was resisted, but creature notices it was attacked and must become aggroed
                spellAPI->figureAggro(_this, spell_index, target_index);
                uint32_t unused;
                SF_CGdTargetData relative_data;
                relative_data.position.X = 0;
                relative_data.position.Y = 0;
                relative_data.entity_type = 1;
                relative_data.entity_index = target_index;
                SF_Rectangle aux_data;
                aux_data.partA = 0;
                aux_data.partB = 0;
                spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellTargetResisted,
                                          &unused, &relative_data, _this->OpaqueClass->current_step, 10, &aux_data);
                // we stop the spell, so it doesn't linger on creature
                spellAPI->setEffectDone(_this, spell_index, 0);
             }
            return;
        }
        else
        {
            // every other time when spell ticks
            if (ticks_passed <= ticks_total)
            {
                uint32_t unused;
                SF_CGdTargetData relative_data;
                relative_data.position.X = 0;
                relative_data.position.Y = 0;
                relative_data.entity_type = 1;
                relative_data.entity_index = target_index;
                SF_Rectangle aux_data;
                aux_data.partA = 0;
                aux_data.partB = 0;
                spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellDOTHitTarget,
                                          &unused, &relative_data, _this->OpaqueClass->current_step, 10, &aux_data);
                // we pull persistent damage value from spell data, it's supposed to be lesser than initial damage was
                damage = spell_data.params[1];
                // with the next command we make the game to ignore the spell effect handler for a specified amount of internal ticks until it's a proper time for the next spell tick to happen
                // please note that internal ticks aren't the same as spell ticks (they're shorter by a magnitudes)
                _this->active_spell_list[spell_index].to_do_count = (uint16_t)((ticks_interval * 10) / 1000);
                toolboxAPI->dealDamage(_this->SF_CGdFigureToolBox, source_index, target_index, damage, 1, 0, 0);
            }
            else
            {
                // spell worked for specified amount of ticks plus one tick for applying initial damage, it should be stopped now
                // Last Param for spell effect done should always be 0
                spellAPI->setEffectDone(_this, spell_index, 0);
            }
        }
    }
}


// This function initializes the module. We store subparts of framework in our own variables to have easier access to them.
// We also initialize spell handlers of our custom spell within this function.

extern "C" __declspec(dllexport) void InitModule(SpellforceSpellFramework *framework)
{
    // pulling framework and its subparts and storing it in our own variables
    sfsf = framework;
    spellAPI = sfsf->spellAPI;
    toolboxAPI = sfsf->toolboxAPI;
    figureAPI = sfsf->figureAPI;
    logger = sfsf->logAPI;
    registrationAPI = sfsf->registrationAPI;

    // We initialize three main spell handlers here. Each handler corresponds to a certain phase of a spell.
    // First we declare our custom spell and link it with custom spell type by its number (the number corresponds to spell type ID in GameData)
    SFSpell *ignite_spell = registrationAPI->registerSpell(242);

    // Here we link our custom spell to main handlers.
    // Spell type handler is called when a spell of a certain spell type was cast. It's called only once at the start of the spell.
    // Spell effect handler applies logic of the custom spell type to game world. Effect handler is repeatedly called as long as the spell remains active.
    // Spell end handler is called when the spell ends. This handler's functions can be imitated within spell effect handler.
    registrationAPI->linkTypeHandler(ignite_spell, &ignite_spelltype_handler);
    registrationAPI->linkEffectHandler(ignite_spell, 0xf2, &ignite_spelleffect_handler); // 0xf2 = 242 = custom spell type in Gamedata.cff
    registrationAPI->linkEndHandler(ignite_spell, &ignite_spellend_handler);
}


// This function registers the mod within the framework. It's critical for the mod to work.
// Basically framework call this function and one above when loading mods
extern "C" __declspec(dllexport) SFMod *RegisterMod(SpellforceSpellFramework *framework)
{
    return framework->createModInfo("SF First Spell", "1.0.0", "Teekius", "How-to guide to create the first custom Spellforce spell");
}

//This function is required for OS to recognize our mod as loadable library. Let it be.

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
