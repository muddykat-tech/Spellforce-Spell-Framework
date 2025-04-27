#include <windows.h>
#include <stdio.h>
#include "../../src/api/sfsf.h"
#include "../../src/asi/sf_asi.h"
// NOTE sfsf.h includes the OTHER api files, but the other files are still required

typedef void (__thiscall *rescaleFigure_ptr)(SF_CGdFigureToolbox *_this,uint16_t figure_index);

SpellforceSpellFramework *sfsf;
SpellFunctions *spellAPI;
ToolboxFunctions *toolboxAPI;
FigureFunctions *figureAPI;
RegistrationFunctions *registrationAPI;
EffectFunctions *effectAPI;
SFLog *logger;

rescaleFigure_ptr rescaleFigureSpells;
rescaleFigure_ptr rescaleFigureDamage;

bool __thiscall buildingIsTower(SF_CGdBuilding *_this, uint16_t param_1)

{
    bool uVar1 = 0;
    switch(_this->buildings[param_1].type)
    {
        case 0xd:
        case 0x20:
        case 0x26:
        case 0x27:
        case 0x47:
        case 0x54:
        case 0x66:
        case 0x67:
        case 0x78:
        case 0x7b:
        case 0x7c:
        case 0x92:
        case 0x93:
        case 0x95:
        case 0x97:
        case 0xa0:
        case 0xa1:
        case 0xa3:
        case 0xa4:
        case 0xa5:
        case 0xa6:
        case 0xa7:
        case 0xa8:
        case 0xa9:
        case 0xaa:
        case 0xac:
        case 0xb2:
        case 0xd1:
        case 0xd2:
        case 0xd3:
        case 0xd4:
        case 0xd5:
            uVar1 = 1;
    }
    return uVar1;
}


void __thiscall rescaleFigureHook(SF_CGdFigureToolbox *_this, uint16_t figure_index)
{
    uint16_t building_index = _this->CGdFigure->figures[figure_index].building;
    if ((building_index != 0) && (buildingIsTower(_this->CGdBuilding, building_index)))
    {
        //DIRTY HACK
        _this->CGdFigure->figures[figure_index].level = _this->CGdBuilding->buildings[building_index].level;
        rescaleFigureSpells(_this, figure_index);
    }
    rescaleFigureDamage(_this, figure_index);
}

extern "C" __declspec(dllexport) void InitModule(SpellforceSpellFramework *framework)
{
    sfsf = framework;
    spellAPI = sfsf->spellAPI;
    toolboxAPI = sfsf->toolboxAPI;
    figureAPI = sfsf->figureAPI;
    registrationAPI = sfsf->registrationAPI;
    effectAPI = sfsf->effectAPI;

    ASI::MemoryRegion mreg_1(ASI::AddrOf(0x30110d), 5);
    ASI::BeginRewrite(mreg_1);
    *(unsigned char *)(ASI::AddrOf(0x30110d)) = 0xE8;
    *(int *)(ASI::AddrOf(0x30110e)) = (int)(&rescaleFigureHook) - ASI::AddrOf(0x301112);
    ASI::EndRewrite(mreg_1);

    rescaleFigureSpells = (rescaleFigure_ptr)ASI::AddrOf(0x2f7c2b);
    rescaleFigureDamage = (rescaleFigure_ptr)ASI::AddrOf(0x2f7cee);

}

extern "C" __declspec(dllexport) SFMod *RegisterMod(SpellforceSpellFramework *framework)
{
    return framework->createModInfo("SF Scaling Towers", "1.0.0", "Un'Schtalch",
                                    "Make enemy tower units scale with level");
}

//This function is required for OS to recognize our mod as loadable library. Let it be.

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
