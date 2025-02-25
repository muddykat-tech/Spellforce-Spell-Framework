#include "../../src/api/sfsf.h"
#include "../../src/asi/sf_asi.h"
// NOTE sfsf.h includes the OTHER api files, but other libraries are still required
#include <windows.h>
#include <stdio.h>
#include "multiple_auras.h"

SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;
RegistrationFunctions *registrationAPI;
EffectFunctions *effectAPI;

getAutoClass24_ptr getAutoClass24;
FUN_006f8c06_ptr FUN_006f8c06;
FUN_006fa3ce_ptr FUN_006fa3ce;
setFigureAction_ptr setFigureAction;

void __thiscall default_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    spellAPI->removeDLLNode(_this, spell_index);
    spellAPI->setEffectDone(_this, spell_index, 0);
}


uint32_t __thiscall onSpellCastHook(SF_CGdFigureToolbox *_this, uint16_t figure_id, uint16_t spell_id, uint32_t param3, uint8_t target_type, SF_Coord position, uint32_t param6, uint32_t param7)
{
    AutoClass24 *ac24 = getAutoClass24(_this->CGdFigure, figure_id);
    if (!FUN_006f8c06(_this, figure_id, spell_id, param3, target_type, position, param6, param7, 0))
    {
        return 0;
    }
    else
    {
        FUN_006fa3ce(_this, figure_id, 0);
        ac24->flags |= MANUAL_JOB_CHANGE;
        ac24->GdJobId = kGdJobManualWalkToTarget;
        ac24->target.entity_index = figure_id;
        ac24->target.entity_type = target_type;
        ac24->target.position.X = position.X;
        ac24->target.position.Y = position.Y;
        SF_CGdResourceSpell spell_data;
        spellAPI->getResourceSpellData(_this->CGdResource, &spell_data, spell_id);

        SF_SGtFigureAction action;
        action.type = spell_data.spell_line_id;
        action.subtype = spell_id;
        action.unkn2 = 0;
        action.unkn3 = 0;
        action.unkn4 = 0;
        action.unkn5 = 0;

        setFigureAction(ac24, &action);
        return 1;
    }
}

void __thiscall aura_end_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    SF_CGdFigure *sf_figures = _this->SF_CGdFigure;
    uint16_t source_index = _this->active_spell_list[spell_index].source.entity_index;
    if ((sf_figures->figures[source_index].owner != (uint16_t)(-1)) &&
        ((sf_figures->figures[source_index].flags & GdFigureFlags::REDO) == 0))
    {     

        /*
        This should do the same thing as the code below, but as a callback.
        */
        // spellAPI->spellEffectCallback(_this, source_index, spell_index, 
        //     [](SF_CGdSpell* spell, uint16_t spell_index, uint16_t walked_index) -> bool {
        //         uint16_t spell_line = spellAPI->getSpellLine(spell, walked_index);
        //         return spellAPI->hasSpellTag(spell_line, AURA_SPELL);
        //     },
        //     [](SF_CGdSpell* spell, uint16_t src_idx, uint16_t walked_index, uint16_t spell_index) -> void {
        //         if( walked_index != spell_index ) {
        //             SF_CGdFigure *sf_figures = spell->SF_CGdFigure;
        //             sf_figures->figures[src_idx].flags = static_cast<GdFigureFlags>(sf_figures->figures[src_idx].flags & 
        //                 (~static_cast<unsigned int>(GdFigureFlags::AURA_RUNNING)));
        //         }
        //     });

        uint16_t node_id = figureAPI->getSpellJobStartNode(_this->SF_CGdFigure, source_index);
        bool should_stop = true;
        while (node_id != 0)
        {
            uint16_t index = toolboxAPI->getSpellIndexFromDLL(_this->SF_CGdDoubleLinkedList, node_id);
            if (index != spell_index)
            {
                uint16_t spell_line = spellAPI->getSpellLine(_this, index);
                if (spellAPI->hasSpellTag(spell_line, AURA_SPELL))
                {
                    should_stop = false;
                    break;
                }
            }
            node_id = toolboxAPI->getNextNode(_this->SF_CGdDoubleLinkedList, node_id);
        }
        if (should_stop)
        {
            sf_figures->figures[source_index].flags = static_cast<GdFigureFlags>(sf_figures->figures[source_index].flags & (~static_cast<unsigned int>(GdFigureFlags::AURA_RUNNING)));
        }
    }
    
    uint16_t effect_index = spellAPI->getXData(_this, spell_index, EFFECT_EFFECT_INDEX);
    if (effect_index != 0)
    {
        effectAPI->tryEndEffect(_this->SF_CGdEffect, effect_index);
    }
    default_end_handler(_this, spell_index);
}

void registerSpells()
{
    SFSpell *aura_1 = registrationAPI->registerSpell(kGdSpellLineAuraFlexibility);
    SFSpell *aura_2 = registrationAPI->registerSpell(kGdSpellLineAuraWeakness);
    registrationAPI->linkEndHandler(aura_1, &aura_end_handler);
    registrationAPI->linkEndHandler(aura_2, &aura_end_handler);

}

extern "C" __declspec(dllexport) void InitModule(SpellforceSpellFramework *framework)
{
    sfsf = framework;
    spellAPI = sfsf->spellAPI;
    toolboxAPI = sfsf->toolboxAPI;
    figureAPI = sfsf->figureAPI;
    registrationAPI = sfsf->registrationAPI;

    getAutoClass24 = (getAutoClass24_ptr)ASI::AddrOf(0x2b5cb0);
    FUN_006f8c06 = (FUN_006f8c06_ptr)ASI::AddrOf(0x2f8c06);
    FUN_006fa3ce = (FUN_006fa3ce_ptr)ASI::AddrOf(0x2fa3ce);
    setFigureAction = (setFigureAction_ptr)ASI::AddrOf(0x2c55d0);

    ASI::MemoryRegion mreg_1(ASI::AddrOf(0x356247), 5);
    ASI::BeginRewrite(mreg_1);
    *(unsigned char *)(ASI::AddrOf(0x356247)) = 0xE8;
    *(int *)(ASI::AddrOf(0x356248)) = (int)(&onSpellCastHook) - ASI::AddrOf(0x35624c);
    ASI::EndRewrite(mreg_1);

    registerSpells();
}

extern "C" __declspec(dllexport) SFMod *RegisterMod(SpellforceSpellFramework *framework)
{
    return framework->createModInfo("Multiple Auras Mod", "1.0.0", "UnSchtalch", "This mod is and experiment to make multiple auras possible using SFSF");
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        ASI::Init(hinstDLL);
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
