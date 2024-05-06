#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sf_utility.h"
#include "sf_hooks.h"

void logWarning(const char *message) {
    // This will be used to hook into the game console later on
    // May also have warnings on a flag
    OutputDebugStringA(message);
    ConsoleLog(message);
}

void logInfo(const char *message) {
    // This will be used to hook into the game console later on
    // May also have warnings on a flag
    OutputDebugStringA(message);
    ConsoleLog(message);
}

void logError(const char* message) {
    OutputDebugStringA(message);
    ConsoleLog(message);
}
