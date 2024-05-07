#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sf_utility.h"
#include "sf_hooks.h"

void logWarning(const char *message) {
    // This will be used to hook into the game console later on
    // May also have warnings on a flag
    char *str = strdup(message);
    OutputDebugStringA(str);
    ConsoleLog(str);
}

void logInfo(const char *message) {
    // This will be used to hook into the game console later on
    // May also have warnings on a flag
    char *str = strdup(message);
    OutputDebugStringA(str);
    ConsoleLog(str);
}

void logError(const char* message) {
    char *str = strdup(message);
    int debugstr = GetLastError();
    sprintf(str, " Last Error: %d\n", debugstr);

    OutputDebugStringA(str);
    ConsoleLog(str);
}
