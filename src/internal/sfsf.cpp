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

DebugLevel global_debug_level = DEBUG_INFO;
DebugLevel parse_debug_level(const char *levelStr)
{
    if (strcasecmp(levelStr, "INFO") == 0)
        return DEBUG_INFO;
    if (strcasecmp(levelStr, "LOW") == 0)
        return DEBUG_LOW;
    if (strcasecmp(levelStr, "MED") == 0)
        return DEBUG_MED;
    if (strcasecmp(levelStr, "HIGH") == 0)
        return DEBUG_HIGH;
    if (strcasecmp(levelStr, "ALL") == 0)
        return DEBUG_ALL;
    return DEBUG_INFO;
}

void create_default_ini_file(const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        log_error("Failed to create default config file: %s", filename);
        return;
    }

    fprintf(file,
            "; Default SFSF configuration\n"
            "[Logging]\n"
            "; (INFO, LOW, MED, HIGH, ALL)\n"
            "DebugLevel=INFO\n");
    fclose(file);

    log_info("Created default config file: %s", filename);
}

void load_debug_level_from_ini(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        log_warning("Config file not found: %s", filename);
        create_default_ini_file(filename);
        return;
    }

    char line[256];
    int inLoggingSection = 0;

    while (fgets(line, sizeof(line), file))
    {
        char *start = line;
        while (isspace((unsigned char)*start))
            ++start;

        if (*start == ';' || *start == '#' || *start == '\n' || *start == '\0')
            continue;

        if (*start == '[')
        {
            if (strncasecmp(start, "[Logging]", 9) == 0)
                inLoggingSection = 1;
            else
                inLoggingSection = 0;
        }
        else if (inLoggingSection)
        {
            char key[128], value[128];
            if (sscanf(start, " %127[^=]= %127s", key, value) == 2)
            {
                if (strcasecmp(key, "DebugLevel") == 0)
                {
                    global_debug_level = parse_debug_level(value);
                }
            }
        }
    }

    fclose(file);
}

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

                load_debug_level_from_ini(CONFIG_FILE);

                log_debug(DEBUG_INFO, "Loading Configs, Log Level is %s", debug_level_to_string(global_debug_level));

                initialize_framework();
                initialize_beta_hooks();
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
