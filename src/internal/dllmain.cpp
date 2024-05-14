#include <windows.h>
#include "sf_hooks.h"
#include "sf_registry.h"
#include "../asi/sf_asi.h"

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        if (!ASI::Init(hModule))
            return FALSE;
        //!ASI::CheckSFVersion(ASI::SF_154) &&
        if (!ASI::CheckSFVersion(ASI::SF_BETA))
        {
            return FALSE;
        }
        else
        {
            // Initialize Framework -> see sf_registry.h
            initFramework();
            
            // Initialize Spellforce Hooks -> see sf_hooks.h
            initBetaHooks();
            break;
        }
     break;
    }
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}