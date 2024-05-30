#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sf_wrappers.h"
#include "sf_hooks.h"

void log_warning(const char *message) {
    // Logs a warning message to the console and the debug output
    static char modifiedMessage[256]; 
    snprintf(modifiedMessage, sizeof(modifiedMessage), "[WARNING] %s", message);
    OutputDebugStringA(modifiedMessage);
    console_log(modifiedMessage);
}

void log_info(const char *message) {
    // Logs an informational message to the console and the debug output
    static char modifiedMessage[256]; 
    snprintf(modifiedMessage, sizeof(modifiedMessage), "[SFSF] %s", message);
    OutputDebugStringA(modifiedMessage);
    console_log(modifiedMessage);
}

void log_error(const char* message) {
    // Logs an error message to the console and the debug output, including the last error code
    int lastError = GetLastError();
    static char modifiedMessage[256];
    snprintf(modifiedMessage, sizeof(modifiedMessage), "[ERROR] %s [Last Error: %d]", message, lastError);
    OutputDebugStringA(modifiedMessage);
    console_log(modifiedMessage);
}