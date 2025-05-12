#include "../../src/api/sfsf.h"

// NOTE sfsf.h includes the OTHER api files, but other libraries are still required
#include <windows.h>
#include <stdio.h>


SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;
EffectFunctions *effectAPI;
RegistrationFunctions *registrationAPI;

//TODO: export this function somewhere?
uint32_t getDistance(SF_Coord *pointA, SF_Coord *pointB)
{

    uint32_t delta;
    uint32_t uVar1;
    uint32_t uVar2;
    uint32_t uVar3;
    uint32_t uVar4;

    delta = (uint32_t)(uint16_t)pointA->X - (uint32_t)(uint16_t)pointB->X;
    uVar2 = (int)delta >> 0x1f;
    uVar2 = (delta ^ uVar2) - uVar2;
    uVar4 = uVar2 & 0xffff;
    delta = (uint32_t)(uint16_t)pointA->Y - (uint32_t)(uint16_t)pointB->Y;
    uVar3 = (int)delta >> 0x1f;
    uVar3 = (delta ^ uVar3) - uVar3;
    uVar1 = uVar3 & 0xffff;
    delta = uVar1;
    if ((uint16_t)uVar2 < (uint16_t)uVar3)
    {
        delta = uVar4;
        uVar4 = uVar1;
    }
    return ((delta * 0xd) >> 5) + uVar4;
}

bool isSiegeUnit (SF_CGdFigure *_this, uint16_t figure_index)
{
    if ((_this->figures[figure_index].unit_data_id == 2236) ||
        (_this->figures[figure_index].unit_data_id == 2238) ||
        (_this->figures[figure_index].unit_data_id == 2239) ||
        (_this->figures[figure_index].unit_data_id == 2244) ||
        (_this->figures[figure_index].unit_data_id == 2245) ||
        (_this->figures[figure_index].unit_data_id == 2249) )
    {
        return true;
    }
    return false;
}

void __thiscall rock_bullet_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_GdSpell *spell = &_this->active_spell_list[spell_index];
    SF_CGdResourceSpell spell_data;
    SF_SpellEffectInfo effect_info;

    effect_info.spell_id = spell->spell_id;
    effect_info.job_id = spell->spell_job;
    spellAPI->getResourceSpellData(_this->SF_CGdResource, &spell_data, spell->spell_id);


    uint16_t target_index = spell->target.entity_index;
    uint16_t source_index = spell->target.entity_index;

    //target is figure, target index has been passed through
    //figure is owned by any clan present on the map
    //figure is targetable
    //figure is not protected by special flags
    if ((spell->target.entity_type == 1) && (target_index != 0) &&
        (_this->SF_CGdFigure->figures[target_index].owner != (uint16_t)(-1)) &&
        (toolboxAPI->isTargetable(_this->SF_CGdFigureToolBox, target_index)) &&
        ((*(uint8_t *)(&_this->SF_CGdFigure->figures[target_index].flags) & 0xa) == 0))
    {
        uint16_t damage = spell_data.params[0];
        SF_CGdTargetData source_data;
        SF_CGdTargetData target_data;

        target_data.entity_index = target_index;
        target_data.entity_type = 1;
        target_data.position.X = spell->target.position.X;
        target_data.position.Y = spell->target.position.Y;

        source_data.entity_index = source_index;
        source_data.entity_type = 1;
        source_data.position.X = spell->source.position.X;
        source_data.position.X = spell->source.position.Y;

        //Wave form hadling
        uint16_t prev_figure_index = spellAPI->getXData(_this, spell_index, EFFECT_ENTITY_INDEX3);
        if ((prev_figure_index == 0) || (!figureAPI->isAlive(_this->SF_CGdFigure, prev_figure_index)))
        {
            prev_figure_index = source_index;
        }

        uint16_t resist_chance = spellAPI->getChanceToResistSpell(_this->AutoClass34, source_index, target_index,
                                                                  effect_info);

        uint16_t random = spellAPI->getRandom(_this->OpaqueClass, 100);

        if (resist_chance < random)
        {
            uint16_t distance = getDistance(&_this->SF_CGdFigure->figures[source_index].position,
                                            &_this->SF_CGdFigure->figures[target_index].position);

            distance = (distance * 1400) / 5000;
            if (distance == 0)
            {
                distance = 1; //we need it for non-zero flight time
            }
            if (!toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, kGdSpellLinePetrify))
            {
                uint16_t reduction_percent = toolboxAPI->getPhysDamageReduction(_this->SF_CGdFigureToolBox,
                                                                                source_index,
                                                                                target_index, kGdSpellLineRockBullet);
                damage = ((uint32_t)(damage * reduction_percent + 5000)) / 10000;
            }
            //if caster is alive
            if (figureAPI->isAlive(_this->SF_CGdFigure, source_index))
            {
                //double damage magic set handling
                if ((_this->SF_CGdFigure->figures[source_index].set_type == 8) &&
                    (spellAPI->getRandom(_this->OpaqueClass, 100) < 26))
                {
                    damage *= 2;
                }
            }
            if (isSiegeUnit(_this->SF_CGdFigure, target_index))
            {
                damage = ((uint16_t)(damage * 100) + 50) / 1000;
            }

            if (toolboxAPI->hasSpellOnIt(_this->SF_CGdFigureToolBox, target_index, kGdSpellLineDecay))
            {
                damage *= 3;
            }

            SF_Rectangle rect = {0, 0};
            target_data.entity_index = target_index;
            target_data.entity_type = 1;
            target_data.position.X = spell->target.position.X;
            target_data.position.Y = spell->target.position.Y;
            uint16_t effect_index = effectAPI->addEffect(_this->SF_CGdEffect, kGdEffectSpellHitTarget, &source_data,
                                                         &target_data, _this->OpaqueClass->current_step, distance,
                                                         &rect);
            effectAPI->setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_SPELL_INDEX, spell_index);
            effectAPI->setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_SPELL_ID,
                                      _this->active_spell_list[spell_index].spell_id);

            effectAPI->setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_PHYSICAL_DAMAGE, damage);
            effectAPI->setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_DO_NOT_ADD_SUBSPELL, 1);
            effectAPI->setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_ENTITY_INDEX, source_index);
            effectAPI->setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_ENTITY_TYPE, 1);
            effectAPI->setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_ENTITY_INDEX2, target_index);
            effectAPI->setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_ENTITY_TYPE2, 1);
            effectAPI->setEffectXData(_this->SF_CGdEffect, effect_index, EFFECT_ENTITY_INDEX3, prev_figure_index);
        }
        else
        {
            spellAPI->figureAggro(_this, spell_index, target_index);
            SF_Rectangle rect = {0,0};

            spellAPI->addVisualEffect(_this, spell_index, kGdEffectSpellTargetResisted, &source_data, &target_data,
                                      _this->OpaqueClass->current_step, 40, &rect);
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

    SFSpell *rock_bullet = registrationAPI->registerSpell(kGdSpellLineRockBullet);
    registrationAPI->linkEffectHandler(rock_bullet, kGdSpellLineRockBullet, &rock_bullet_effect_handler);
}


/***
 * This function MUST be present in your code with the exact declaration
 * otherwise the framework is unable to describe your mod in logs and mod info menu (menu not yet implemented)
 ***/

extern "C" __declspec(dllexport) SFMod *RegisterMod(SpellforceSpellFramework *framework)
{
    return framework->createModInfo("Rock Bullet Rewrite", "1.0.0", "UnSchtalch",
                                    "Example rewrite for Dark Ruler to work from");
}


// Required to be present by OS, not required for any functionality

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
