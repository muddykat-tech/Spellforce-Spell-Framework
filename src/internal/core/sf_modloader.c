#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sf_wrappers.h"
#include "sf_modloader.h"

#include "../registry/sf_registry.h"

typedef void (*InitModuleFunc)(void *);
typedef SFMod *(*RegisterModFunc)(void *);
int g_mod_count = 0;
int g_error_count = 0;

void cleanup(void *modHandle)
{
    // Free resources (unload mod library using FreeLibrary)
    if (modHandle)
    {
        FreeLibrary((HMODULE)modHandle);
    }
}

// Function to extract filename from the path
const char *get_filename(const char *path)
{
    const char *filename = strrchr(path, '\\'); // Find the last occurrence of '\\' in the path
    return (filename) ? (filename + 1) : path;
}

void load_mod(const char *modPath, void *pFrameworkAPI)
{
    HMODULE modHandle = LoadLibrary(modPath);
    if (!modHandle)
    {
        log_error("| - Failed to load mod library (X_X)");
        return;
    }

    RegisterModFunc registerMod =
        (RegisterModFunc) GetProcAddress(modHandle, "RegisterMod");
    InitModuleFunc initModule =
        (InitModuleFunc) GetProcAddress(modHandle, "InitModule");

    if (!initModule)
    {
        log_warning(get_filename(modPath));
        log_error("| - Failed to get address of InitModule (X_X)");
        cleanup(modHandle);
        return;
    }

    if (!registerMod)
    {
        char warn[256];
        snprintf(warn, sizeof(warn),
                 "| - Failed to Initialize %s has erroneous mod data. (0_0)",
                 get_filename(modPath));
        log_warning(warn);
        log_error("| - Failed to get address of RegisterMod (X_X)");
        g_error_count += 1;
        return;
    }

    g_current_mod = registerMod(pFrameworkAPI);
    initModule(pFrameworkAPI);
    g_mod_count += 1;
    char infomsg[256];
    snprintf(infomsg, sizeof(infomsg), "| - [Initialized Mod: %s (Ver. %s)]",
             g_current_mod->mod_id, g_current_mod->mod_version);
    log_info(infomsg);
    return;
}

void load_all_mods(const char *subfolder, void *pFrameworkAPI)
{
    char currentDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDir);

    char modDirectory[MAX_PATH];
    snprintf(modDirectory, sizeof(modDirectory), "%s\\%s", currentDir,
             subfolder);

    WIN32_FIND_DATA findFileData;
    char searchPath[MAX_PATH];
    snprintf(searchPath, sizeof(searchPath), "%s\\*.sfm", modDirectory);

    HANDLE hFind = FindFirstFile(searchPath, &findFileData);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            char modPath[MAX_PATH];
            snprintf(modPath, sizeof(modPath), "%s\\%s", modDirectory,
                     findFileData.cFileName);
            load_mod(modPath, pFrameworkAPI);
        } while (FindNextFile(hFind, &findFileData) != 0);
        FindClose(hFind);
    }
    else
    {
        char msgbuf[MAX_PATH];
        snprintf(msgbuf, sizeof(msgbuf),
                 "| - Failed to find mods in directory: %s", modDirectory);
        log_error(msgbuf);
    }
}

void initialize_mods()
{
    load_all_mods("sfsf", &frameworkAPI);
    static char info_str[256];
    snprintf(info_str, sizeof(info_str),
             "| - %d Mods Initialized with %d error(s)", g_mod_count,
             g_error_count);
    log_info(info_str);
}
