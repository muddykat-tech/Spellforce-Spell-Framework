#include "../../src/api/sfsf.h"
#include <windows.h>
#include <stdio.h>
// For convenience only; You can put headers just near the file and remove ../api/
// NOTE sfsf.h includes the OTHER api files, the other files are still required

SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
FigureFunctions *figureAPI;
RegistrationFunctions *registrationAPI;
EffectFunctions *effectAPI;

// Id policy: max(vanilla)+1, owner directive 2026-11-04 (free holes are not
// picked anymore). The gamedata chunk 2002 record carries the Ice Comet as
// spell id 3554 (vanilla max 3553 + 1) with SpellLine=246 (vanilla max 245
// + 1). The engine resolves spell id -> line at cast time and the framework
// dispatches type handlers BY LINE (get_spell_handler(spell_line)), so
// registerSpell() below is fed the LINE, not the spell id; the spell id
// reaches the effect handler at runtime through active_spell_list[]
// .spell_id (getResourceSpellData reads params by it). Documentation only
// here - no code path needs the spell id constant at compile time.
#define ICE_COMET_SPELL_ID 3554
#define ICE_COMET_LINE 246

// Spell job: max(vanilla)+1, owner directive 2026-11-04 ("everything below
// is occupied"). NOTE: 172 == 0xac == INTERFERENCE_JOB of the interference
// example; the collision only bites when both mods are loaded at the same
// time (see the caveat in README.md).
#define ICE_COMET_JOB 172

/***
 * Type handler: assign the spell job and initialize the tick counter,
 * the vanilla blizzard pattern (blizzard_handler, sf_spelltype_handlers.cpp).
 */
void __thiscall ice_comet_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    _this->active_spell_list[spell_index].spell_job = ICE_COMET_JOB;
    spellAPI->initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

/***
 * One-shot effect: on tick 0 spawn a single falling comet with the blizzard
 * fall projectile semantics. The target cell is a snapshot taken at cast time
 * (figure -> its current position, world point -> the point itself, no homing).
 * The comet itself carries no damage: on impact the engine casts the vanilla
 * IceBurst subspell over hostile figures in the 3x3 cells around the landing
 * point (AddSpellFromEffect via EFFECT_SUBSPELL_ID/EFFECT_ADD_SUBSPELL).
 */
void __thiscall ice_comet_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    uint32_t tick_current = spellAPI->getXData(_this, spell_index, SPELL_TICK_COUNT_AUX);
    spellAPI->addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1);

    if (tick_current == 0)
    {
        // spell parameters from gamedata.cff (chunk 2002), needed in tick 0 only
        SF_CGdResourceSpell spell_data;
        spellAPI->getResourceSpellData(_this->CGdResource, &spell_data, spell->spell_id);

        // snapshot of the target cell at cast time
        SF_Coord fall_position;
        if (spell->target.entity_type == 1)
        {
            figureAPI->getPosition(_this->CGdFigure, &fall_position, spell->target.entity_index);
        }
        else if (spell->target.entity_type == 4)
        {
            fall_position = spell->target.position;
        }
        else
        {
            // any other target type (a building, a monument) carries no
            // usable world position: the comet would fall onto (0,0),
            // so the spell aborts instead of misfiring
            sfsf->logAPI->logWarning("ICE COMET ABORTED: unsupported target type %d",
                                     spell->target.entity_type);
            spellAPI->setEffectDone(_this, spell_index, 0);
            return;
        }
        SF_CGdTargetData fall_target = {4, 0, {fall_position.X, fall_position.Y}};
        SF_CGdTargetData source_data = {spell->source.entity_type, spell->source.entity_index, {0, 0}};
        SF_Rectangle rect;
        rect.partA = 0;
        rect.partB = 0;

        // flight: 20 ticks (2 s at 10 ticks/s), landing delayed by params[0] ms
        uint32_t tick_start = _this->OpaqueClass->current_step + ((spell_data.params[0] * 10) / 1000);
        uint16_t fall_effect_id = effectAPI->addEffect(_this->CGdEffect, kGdEffectBlizzardFall, &source_data, &fall_target, tick_start, 0x14, &rect);

        // the engine returns 0 when all 1999 effect slots are taken; without
        // this gate the xdata below would be written into slot 0 and the
        // comet would fizzle silently
        if (fall_effect_id == 0)
        {
            sfsf->logAPI->logWarning("ICE COMET ABORTED: no free effect slot for the falling comet");
            spellAPI->setEffectDone(_this, spell_index, 0);
            return;
        }

        // the vanilla xdata set of a blizzard fall projectile: the engine
        // dispatches on key presence, so no extra keys and no
        // EFFECT_PHYSICAL_DAMAGE on the comet itself
        effectAPI->setEffectXData(_this->CGdEffect, fall_effect_id, EFFECT_SPELL_INDEX, spell_index);
        effectAPI->setEffectXData(_this->CGdEffect, fall_effect_id, EFFECT_SPELL_ID, spell->spell_id);
        effectAPI->setEffectXData(_this->CGdEffect, fall_effect_id, EFFECT_SUBSPELL_ID, spell_data.params[4]);
        effectAPI->setEffectXData(_this->CGdEffect, fall_effect_id, EFFECT_EFFECT_TYPE, kGdEffectBlizzardHit);
        effectAPI->setEffectXData(_this->CGdEffect, fall_effect_id, EFFECT_ENTITY_INDEX, source_data.entity_index);
        effectAPI->setEffectXData(_this->CGdEffect, fall_effect_id, EFFECT_ENTITY_TYPE, source_data.entity_type);
        effectAPI->setEffectXData(_this->CGdEffect, fall_effect_id, EFFECT_ADD_SUBSPELL, 0);
    }

    // one-shot spell: finish right after the first comet, like the other
    // one-shot example effects
    spellAPI->setEffectDone(_this, spell_index, 0);
}

/***
 * This function MUST be present in your code with the exact declaration
 * otherwise framework won't load your plugin
 ***/
extern "C" __declspec(dllexport) void InitModule(SpellforceSpellFramework *framework)
{
    sfsf = framework;
    spellAPI = sfsf->spellAPI;
    figureAPI = sfsf->figureAPI;
    registrationAPI = sfsf->registrationAPI;
    effectAPI = sfsf->effectAPI;

    // spell registration: type handler sets the job, effect handler spawns the comet
    SFSpell *ice_comet_spell = registrationAPI->registerSpell(ICE_COMET_LINE);
    registrationAPI->linkTypeHandler(ice_comet_spell, &ice_comet_handler);
    registrationAPI->linkEffectHandler(ice_comet_spell, ICE_COMET_JOB, &ice_comet_effect_handler);
}

/***
 * This function MUST be present in your code with the exact declaration
 * otherwise the framework is unable to describe your mod in logs and mod info menu (menu not yet implemented)
 ***/
extern "C" __declspec(dllexport) SFMod *RegisterMod(SpellforceSpellFramework *framework)
{
    return framework->createModInfo("Ice Comet Example Mod", "1.0.0", "UnSchtalch", "A mod designed to provide an example of a single falling comet spell with a custom line visual and a vanilla subspell splash");
}

// Required to be present, not required for any functionality
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        /* Code path executed when DLL is loaded into a process' address space. */
        break;

    case DLL_THREAD_ATTACH:
        /* Code path executed when a new thread is created within the process. */
        break;

    case DLL_THREAD_DETACH:
        /* Code path executed when a thread within the process has exited 'cleanly'. */
        break;

    case DLL_PROCESS_DETACH:
        /* Code path executed when DLL is unloaded from a process' address space. */
        break;
    }

    return TRUE;
}
