#include <stdio.h>
#include "SpellforceSpellFramework.h"

typedef void (*InitModuleFunc)(SpellforceSpellFramework*);

void log_error(const char* message) {
    FILE* error_log = fopen("error.log", "a");
    if (error_log) {
        fprintf(error_log, "%s\n", message);
        fclose(error_log);
    }
}

// Memory cleanup function
void cleanup(SpellforceSpellFramework* frameworkAPI, void* modHandle) {
    // Free resources
    // Unload the mod library
    if (modHandle) {
        FreeLibrary(modHandle);
    }
}

void load_mod(const char* modPath, SpellforceSpellFramework* frameworkAPI) {

    // Load the mod
    void* modHandle = LoadLibrary(modPath, RTLD_LAZY);
    if (!modHandle) {
        char error_message[256];
        snprintf(error_message, sizeof(error_message), "Error loading mod '%s'", modPath);
        log_error(error_message);
        return;
    }

    // Obtain the address of the initializeModule function within the mod
    InitModuleFunc initModuleFunc = (InitModuleFunc)dlsym(modHandle, "initializeModule");
    if (!initModuleFunc) {
        char error_message[256];
        snprintf(error_message, sizeof(error_message), "Error getting initializeModule function for mod '%s'", modPath);
        log_error(error_message);
        cleanup(frameworkAPI, modHandle);
        return;
    }

    // Initialize the mod with the framework API
    initModuleFunc(frameworkAPI);

    // Cleanup resources
    cleanup(frameworkAPI, modHandle);
}

void load_all_mods(const char* subfolder, SpellforceSpellFramework* frameworkAPI) {
    DIR* dir;
    struct dirent* ent;

    // Open the directory
    if ((dir = opendir(subfolder)) != NULL) {
        // Iterate through each file in the directory
        while ((ent = readdir(dir)) != NULL) {
            // Construct the full path of the file
            char modPath[256];
            snprintf(modPath, sizeof(modPath), "%s/%s", subfolder, ent->d_name);

            // Load the mod if it's a valid file
            if (strstr(ent->d_name, ".sfm") != NULL) {
                load_mod(modPath, frameworkAPI);
            }
        }
        closedir(dir);
    } else {
        // Error handling if unable to open directory
        perror("Error opening directory");
    }
}
