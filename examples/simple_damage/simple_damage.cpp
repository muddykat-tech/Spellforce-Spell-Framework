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
EffectFunctions *effectAPI;

#define STATIC_SPELL_LINE 0xf3

void __thiscall static_sub_effect_handler(SF_CGDEffect *_this, uint16_t effect_index)
{
    sfsf->logAPI->logInfo("Subeffect triggered");
    uint16_t spell_id = effectAPI->getEffectXData(_this, effect_index, EFFECT_SUBSPELL_ID);
    uint16_t figure_index1 = effectAPI->getEffectXData(_this, effect_index, EFFECT_ENTITY_INDEX);
    uint8_t figure_type1;
    if (figure_index1)
    {
        figure_type1 = effectAPI->getEffectXData(_this, effect_index, EFFECT_ENTITY_TYPE);
    }
    uint16_t figure_index2 = effectAPI->getEffectXData(_this, effect_index, EFFECT_ENTITY_INDEX2);
    uint8_t figure_type2 = 0;
    if (figure_index2)
    {
        figure_type2 = effectAPI->getEffectXData(_this, effect_index, EFFECT_ENTITY_TYPE2);
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
    spellAPI->addSpell(_this->CGdSpell, spell_id, _this->OpaqueClass->current_step, &source, &target, 0);
}

void __thiscall static_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    _this->active_spell_list[spell_index].spell_job = 0xf1;
    spellAPI->setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall simple_damage_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    _this->active_spell_list[spell_index].spell_job = 0xf2;
}

void __thiscall static_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    sfsf->logAPI->logInfo("Static shock alive");
    SF_CGdResourceSpell spell_data;
    spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);
    uint32_t tick_current = spellAPI->getXData(_this, spell_index, SPELL_TICK_COUNT_AUX);
    uint16_t ticks_passed = spellAPI->addToXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 1);
    uint16_t ticks_total = spell_data.params[2];
    uint16_t ticks_interval = spell_data.params[3];
    uint16_t target_index = spell->target.entity_index;
    uint16_t source_index = spell->source.entity_index;
    if (tick_current == 0)
    {
        SF_CGdTargetData sourceData = {1, source_index, {0, 0}};
        SF_CGdTargetData targetData = {1, target_index, {0, 0}};
        SF_Rectangle aux_data;
        aux_data.partA = 0;
        aux_data.partB = 0;
        uint16_t effect_id = effectAPI->addEffect(_this->SF_CGdEffect, kGdEffectSpellDOTHitTarget, &sourceData, &targetData, _this->OpaqueClass->current_step, (ticks_total * ticks_interval), &aux_data);
    }
    if (ticks_passed < ticks_total)
    {
        _this->active_spell_list[spell_index].to_do_count = (uint16_t)((ticks_interval * 10) / 1000);
    }
    else
    {
        spellAPI->setEffectDone(_this, spell_index, 0);
    }
}
void __thiscall simple_damage_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    uint16_t target_index = spell->target.entity_index;
    uint16_t source_index = spell->source.entity_index;
    SF_SpellEffectInfo effect_info;
    SF_CGdResourceSpell spell_data;
    effect_info.spell_id = spell->spell_id;
    effect_info.job_id = spell->spell_job;
    spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, effect_info.spell_id);
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
                SF_Rectangle aux_data;
                aux_data.partA = 0;
                aux_data.partB = 0;
                spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellHitTarget, &unused, &relative_data, _this->OpaqueClass->current_step, 0x19, &aux_data);
                if (figureAPI->isAlive(_this->SF_CGdFigure, target_index))
                {
                    uint16_t damage = spell_data.params[0];
                    if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, STATIC_SPELL_LINE))
                    {
                        damage *= 2;
                        sfsf->logAPI->logInfo("Static shock found");
                        uint16_t static_spell_id = toolboxAPI->getSpellIndexOfType(_this->SF_CGdFigureToolBox, target_index, STATIC_SPELL_LINE, spell_index);
                        spellAPI->setEffectDone(_this, static_spell_id, 0);
                    }
                    else
                    {
                        SF_CGdTargetData sourceData = {1, source_index, {0, 0}};
                        SF_CGdTargetData targetData = {1, target_index, {0, 0}};

                        // lets say effect lasts for 1000 ticks
                        uint16_t effect_id = effectAPI->addEffect(_this->SF_CGdEffect, kGdEffectSpellDOTHitTarget, &sourceData, &targetData, _this->OpaqueClass->current_step, 5, &aux_data);
                        effectAPI->setEffectXData(_this->SF_CGdEffect, effect_id, EFFECT_SPELL_INDEX, spell_index);
                        effectAPI->setEffectXData(_this->SF_CGdEffect, effect_id, EFFECT_SPELL_ID, spell->spell_id);
                        effectAPI->setEffectXData(_this->SF_CGdEffect, effect_id, EFFECT_SUBSPELL_ID, spell_data.params[4]);
                        effectAPI->setEffectXData(_this->SF_CGdEffect, effect_id, EFFECT_ENTITY_INDEX, source_index);
                        effectAPI->setEffectXData(_this->SF_CGdEffect, effect_id, EFFECT_ENTITY_TYPE, 1);
                        effectAPI->setEffectXData(_this->SF_CGdEffect, effect_id, EFFECT_ENTITY_INDEX2, target_index);
                        effectAPI->setEffectXData(_this->SF_CGdEffect, effect_id, EFFECT_ENTITY_TYPE2, 1);
                    }
                    toolboxAPI->dealDamage(_this->SF_CGdFigureToolBox, source_index, target_index, damage, 1, 0, 0);
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
                SF_Rectangle aux_data;
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
    effectAPI = sfsf->effectAPI;

    // Here comes the spell type registration. Coose your spell_id and your spell_effect_id (spell_job);
    SFSpell *simple_damage_spell = registrationAPI->registerSpell(242);

    // Note: you need to pass pointer to function here
    registrationAPI->linkTypeHandler(simple_damage_spell, &simple_damage_handler);

    // Here comes the spell logic (effect)
    registrationAPI->linkEffectHandler(simple_damage_spell, 0xf2, &simple_damage_effect_handler);

    SFSpell *static_spell = registrationAPI->registerSpell(STATIC_SPELL_LINE);
    registrationAPI->linkTypeHandler(static_spell, &static_handler);
    registrationAPI->linkEffectHandler(static_spell, 0xf1, &static_effect_handler);
    registrationAPI->linkSubEffectHandler(static_spell, &static_sub_effect_handler);
}

/***
 * This function MUST be present in your code with the exact declaration
 * otherwise the framework is unable to describe your mod in logs and mod info menu (menu not yet implemented)
 ***/
extern "C" __declspec(dllexport) SFMod *RegisterMod(SpellforceSpellFramework *framework)
{
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