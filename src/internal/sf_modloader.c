#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../api/sfsf.h"
#include "sf_modloader.h"

extern SpellforceSpellFramework frameworkAPI;

typedef void (*InitModuleFunc)(void*);

void cleanup(void* modHandle) {
    // Free resources (unload mod library using FreeLibrary)
    FreeLibrary((HMODULE)modHandle);
}

void log_error(const char* message) {
    char buffer[256]; // Max length of error message...
    snprintf(buffer, sizeof(buffer), "Error: %s\n", message);
    OutputDebugStringA(buffer);
}


void load_mod(const char* modPath, void* frameworkAPI) {
    // Using same method for loading a library we load the mods other people make
    HMODULE modHandle = LoadLibrary(modPath);
    if (modHandle) {
        // Obtain the address of the initializeModule function within the mod
        InitModuleFunc initModule = (InitModuleFunc)GetProcAddress(modHandle, "InitModule");
        if (initModule) {
            // Execute the initializeModule function with the framework API
            initModule(frameworkAPI);
            // Cleanup
        } else {
            log_error("Failed to get address of InitModule");
            cleanup(modHandle);

        }
    } else {
        log_error("Failed to load mod library");
    }
}

void load_all_mods(const char* subfolder, void* frameworkAPI) {
    // Grab the current working directory
    char currentDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDir);

    // Construct the directory path with subfolder
    char modDirectory[MAX_PATH];
    sprintf(modDirectory, "%s\\%s", currentDir, subfolder);

    // Find all .sfm (DLL) files in the specified directory
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(strcat(modDirectory, "\\*.sfm"), &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            // Load each mod found
            char modPath[MAX_PATH];
            sprintf(modDirectory, "%s\\%s", currentDir, subfolder);
            sprintf(modPath, "%s\\%s", modDirectory, findFileData.cFileName);
            load_mod(modPath, frameworkAPI);
        } while (FindNextFile(hFind, &findFileData) != 0);
        FindClose(hFind);
    } else {
        log_error("Failed to find mods in specified directory");
    }
}


void initMods(){
    // load all the mods, we'll also need to at some point use this section to provide framework api functions to the mod as well?
    load_all_mods("sfsf", &frameworkAPI);
}