#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sf_wrappers.h"
#include "sf_modloader.h"

extern SpellforceSpellFramework frameworkAPI;

typedef void (*InitModuleFunc)(void*);

void cleanup(void* modHandle) {
    // Free resources (unload mod library using FreeLibrary)
    if (modHandle) {
        FreeLibrary((HMODULE)modHandle);
    }
}

// Function to extract filename from the path
const char* get_filename(const char* path) {
    const char* filename = strrchr(path, '\\'); // Find the last occurrence of '\\' in the path
    return (filename) ? (filename + 1) : path;
}

void load_mod(const char* modPath, void* pFrameworkAPI) {
    HMODULE modHandle = LoadLibrary(modPath);
    if (modHandle) {
        InitModuleFunc initModule = (InitModuleFunc)GetProcAddress(modHandle, "InitModule");
        if (initModule) {
            initModule(pFrameworkAPI);
            char infomsg[256];
            snprintf(infomsg, sizeof(infomsg), "[Initialized Mod: %s]", get_filename(modPath));
            log_info(infomsg);
        } else {
            log_error("Failed to get address of InitModule");
            cleanup(modHandle);
        }
    } else {
        log_error("Failed to load mod library");
    }
}

void load_all_mods(const char* subfolder, void* pFrameworkAPI) {
    char currentDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDir);

    char modDirectory[MAX_PATH];
    snprintf(modDirectory, sizeof(modDirectory), "%s\\%s", currentDir, subfolder);

    WIN32_FIND_DATA findFileData;
    char searchPath[MAX_PATH];
    snprintf(searchPath, sizeof(searchPath), "%s\\*.sfm", modDirectory);

    HANDLE hFind = FindFirstFile(searchPath, &findFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            char modPath[MAX_PATH];
            snprintf(modPath, sizeof(modPath), "%s\\%s", modDirectory, findFileData.cFileName);
            load_mod(modPath, pFrameworkAPI);
        } while (FindNextFile(hFind, &findFileData) != 0);
        FindClose(hFind);
    } else {
        char msgbuf[MAX_PATH];
        snprintf(msgbuf, sizeof(msgbuf), "Failed to find mods in directory: %s", modDirectory);
        log_error(msgbuf);
    }
}

void initialize_mods() {    
    log_info("--- Mod Loading Phase Start ---");
    load_all_mods("sfsf", &frameworkAPI);
    log_info("--- Mod Loading Phase End ---");
}