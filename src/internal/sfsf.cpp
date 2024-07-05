#include <windows.h>
#include "core/sf_hooks.h"
#include "core/sf_wrappers.h"
#include "registry/sf_registry.h"
#include "../asi/sf_asi.h"
#include <stdio.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        if (!ASI::Init(hModule))
            return FALSE;
        //! ASI::CheckSFVersion(ASI::SF_154) &&
        if (!ASI::CheckSFVersion(ASI::SF_BETA))
        {
            return FALSE;
        }
        else
        {
            FILE *file = fopen("sfsf.log", "w");
            fclose(file);

            log_info("Spellforce Version Accepted; Starting SFSF");
            // Initialize Framework -> see sf_registry.h
            initialize_framework();

            // Initialize Spellforce Hooks -> see sf_hooks.h
            initialize_beta_hooks();

            // Just wanted to align the debug log
            OutputDebugStringA("[SFSF] |======================| Injection Complete |======================|");
            break;
        }
        break;
    }
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}