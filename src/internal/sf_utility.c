#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sf_utility.h"

void logWarning(const char *message) {
    // This will be used to hook into the game console later on
    // May also have warnings on a flag
    OutputDebugStringA(message);
}

void logError(const char* message) {
    char buffer[256]; // Max length of error message...
    snprintf(buffer, sizeof(buffer), "Error: %s\n", message);
    OutputDebugStringA(buffer);
}