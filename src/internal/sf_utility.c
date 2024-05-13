#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sf_utility.h"
#include "sf_hooks.h"

void logWarning(const char *message) {
    // This will be used to hook into the game console later on
    // May also have warnings on a flag
    static char modifiedMessage[256]; // Assuming maximum message length
    snprintf(modifiedMessage, sizeof(modifiedMessage), "[WARNING] %s", message);
    OutputDebugStringA(modifiedMessage);
    ConsoleLog(modifiedMessage);
}

void logInfo(const char *message) {
    // This will be used to hook into the game console later on
    // May also have warnings on a flag
    char *str = strdup(message);
    OutputDebugStringA(str);
    ConsoleLog(str);
}

void logError(const char* message) {
    int lastError = GetLastError();
    static char modifiedMessage[256]; // Assuming maximum message length
    snprintf(modifiedMessage, sizeof(modifiedMessage), "[ERROR] %s [Last Error: %d]", message, lastError);
    OutputDebugStringA(modifiedMessage);
    ConsoleLog(modifiedMessage);
}
