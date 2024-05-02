#include <windows.h>
#include "modloader.h"
#include "SpellforceSpellHandler.h"
#include "hooks.h"

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
        	// Setup Vanilla Spells
			initSpellMap();
			
			// Attempt to load all mods!
			initializeMods();

            hookBetaVersion();
            break;
        }
     break;
    }
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}