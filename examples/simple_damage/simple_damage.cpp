#include "../../src/api/sfsf.h"
#include <windows.h>
#include <stdio.h>
// For convenience only; You can put headers just near the file and remove ../api/
// NOTE sfsf.h includes the OTHER api files, the other files are still required

// Another convenience: you can ommit functions you won't need, or call everything from the framework structure
SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;
RegistrationFunctions *registrationAPI;

void __thiscall simple_damage_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    _this->active_spell_list[spell_index].spell_job = 0xf2;
}

typedef struct
{
    uint32_t partA;
    uint32_t partB;
} effect_aux_data;

void __thiscall simple_damage_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    uint16_t target_index = spell->target.entity_index;
    uint16_t source_index = spell->source.entity_index;
    /***
     * If entity_type is 1 (unit) and index is not 0
     * and owner is not -1 (unit is not reserved one)
     * and unit is not dead (probaly?)
     * and is not special type unit..?
     */

    if ((spell->target.entity_type == 1) && (target_index != 0))
        if (((int16_t)(_this->SF_CGdFigure->figures[target_index].owner) != -1) &&
            (((uint8_t)(_this->SF_CGdFigure->figures[target_index].flags) & 0xa) == 0) &&
            (toolboxAPI->isTargetable(_this->SF_CGdFigureToolBox, target_index)))
        {
            SF_SpellEffectInfo effect_info;
            SF_CGdResourceSpell spell_data;
            effect_info.spell_id = spell->spell_id;
            effect_info.job_id = spell->spell_job;
            spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, effect_info.spell_id);

            uint16_t random_roll = spellAPI->getRandom(_this->OpaqueClass, 100);
            uint32_t resist_chance = spellAPI->getChanceToResistSpell(_this->unkn2, source_index, target_index, effect_info);
            if ((uint32_t)resist_chance < random_roll)
            {
                uint32_t unused;

                // IDK, if this right, though
                SF_CGdTargetData relative_data;
                relative_data.position.X = 0;
                relative_data.position.Y = 0;
                relative_data.entity_type = 1;
                relative_data.entity_index = target_index;
                // IDK what that structure
                effect_aux_data aux_data;
                aux_data.partA = 0;
                aux_data.partB = 0;
                spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitTarget, &unused, &relative_data, _this->OpaqueClass->current_step, 0x19, &aux_data);
                if (figureAPI->isAlive(_this->SF_CGdFigure, target_index))
                {
                    toolboxAPI->dealDamage(_this->SF_CGdFigureToolBox, source_index, target_index, spell_data.params[0], 1, 0, 0);
                }
            }
            else
            {
                spellAPI->figureAggro(_this, spell_index, target_index);
                uint32_t unused;
                SF_CGdTargetData relative_data;
                relative_data.position.X = 0;
                relative_data.position.Y = 0;
                relative_data.entity_type = 1;
                relative_data.entity_index = target_index;
                // IDK what that structure
                effect_aux_data aux_data;
                aux_data.partA = 0;
                aux_data.partB = 0;
                spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellTargetResisted, &unused, &relative_data, _this->OpaqueClass->current_step, 10, &aux_data);
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
    sfsf = framework;
    spellAPI = sfsf->spellAPI;
    toolboxAPI = sfsf->toolboxAPI;
    figureAPI = sfsf->figureAPI;
    registrationAPI = sfsf->registrationAPI;

    // Here comes the spell type registration. Coose your spell_id and your spell_effect_id (spell_job);
    registrationAPI->registerSpell(242, 0xf2);
    
    // Note: you need to pass pointer to function here
    registrationAPI->linkTypeHandler(242, &simple_damage_handler);

    // Here comes the spell logic (effect)
    registrationAPI->linkEffectHandler(242, &simple_damage_effect_handler);
}

/***
 * This function MUST be present in your code with the exact declaration
 * otherwise the framework is unable to describe your mod in logs and mod info menu (menu not yet implemented)
 ***/
extern "C" __declspec(dllexport) SFMod* RegisterMod(SpellforceSpellFramework* framework) {
    return framework->createModInfo("Simple Damage Example Mod", "1.0.0", "UnSchtalch", "A mod designed to provide an example of a simple damage spell");
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