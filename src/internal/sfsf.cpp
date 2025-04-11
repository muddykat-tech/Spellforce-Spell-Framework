/**
 * @mainpage Spellforce Spell Framework
 * - @ref API "SFSF Api"
 * - @ref Internal "SFSF Internals"
 */

/**
 * @defgroup Internal
 */

#include <windows.h>
#include "core/sf_hooks.h"
#include "core/sf_wrappers.h"
#include "registry/sf_registry.h"
#include "../asi/sf_asi.h"
#include <stdio.h>

/**
 * @brief Entry point for the DLL.
 *
 * This function is called by the operating system when the DLL is loaded or unloaded.
 * It handles initialization and cleanup based on the reason for the call.
 *
 * @param hModule Handle to the DLL module. This value is used for resource management.
 * @param ul_reason_for_call The reason the function is being called. Possible values include:
 * - `DLL_PROCESS_ATTACH`: The DLL is being loaded into the process's address space.
 * - `DLL_THREAD_ATTACH`: A new thread is being created in the process (not handled here).
 * - `DLL_THREAD_DETACH`: A thread is exiting cleanly (not handled here).
 * - `DLL_PROCESS_DETACH`: The DLL is being unloaded from the process's address space.
 * @param lpReserved If `ul_reason_for_call` is `DLL_PROCESS_ATTACH`, this parameter is `NULL` for dynamic loads
 * and non-NULL for static loads. If `ul_reason_for_call` is `DLL_PROCESS_DETACH`, this parameter is `NULL` if
 * the DLL is being unloaded by a call to `FreeLibrary`, or non-NULL if the process is terminating.
 *
 * @return Returns `TRUE` if successful; `FALSE` otherwise. If `FALSE` is returned during `DLL_PROCESS_ATTACH`,
 * the DLL will not be loaded.
 * @see ASI::Init(), ASI::CheckSFVersion(), initialize_framework(), initialize_beta_hooks()
 **/
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
                      LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
            if (!ASI::Init(hModule))
                return FALSE;
            if (!ASI::CheckSFVersion(ASI::SF_BETA))
            {
                return FALSE;
            }
            else
            {
                FILE *file = fopen("sfsf.log", "w");
                fclose(file);

                log_info("Spellforce Version Accepted; Starting SFSF");
                initialize_framework();
                initialize_beta_hooks();
                OutputDebugStringA(
                    "[SFSF] |======================| Injection Complete |======================|");
                break;
            }
            break;
        }
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}
