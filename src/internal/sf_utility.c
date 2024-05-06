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
    //ConsoleLog(message);
}

void logInfo(const char *message) {
    // This will be used to hook into the game console later on
    // May also have warnings on a flag
    OutputDebugStringA(message);
    ConsoleLog(message);
}

void logError(const char* message) {
    OutputDebugStringA(message);
    //ConsoleLog(message);
}


SF_String* convCharToString(const char* str){
	// Allocate memory for SF_String
    SF_String* sf_string = (SF_String*)malloc(sizeof(SF_String));
    if (sf_string == NULL) {
        // Handle memory allocation failure
        return NULL;
    }

    // Calculate string length
    size_t len = strlen(str);

    // Allocate memory for data and copy the string
    sf_string->data = (char*)malloc(len + 1); // +1 for null terminator
    if (sf_string->data == NULL) {
        // Handle memory allocation failure
        free(sf_string);
        return NULL;
    }
    strcpy(sf_string->data, str);

    // Convert char* to wchar_t*
    size_t wlen = mbstowcs(NULL, str, 0); // Get required size
    if (wlen == (size_t)-1) {
        // Handle conversion error
        free(sf_string->data);
        free(sf_string);
        return NULL;
    }
    wchar_t* wstr = (wchar_t*)malloc((wlen + 1) * sizeof(wchar_t)); // +1 for null terminator
    if (wstr == NULL) {
        // Handle memory allocation failure
        free(sf_string->data);
        free(sf_string);
        return NULL;
    }
    mbstowcs(wstr, str, wlen + 1); // Perform conversion

    // Set members
    sf_string->raw_data = _wcsdup(wstr);
    sf_string->str_length = (uint8_t)len;
    sf_string->unknown_length_var = (uint8_t)len;

    // Free temporary wchar_t string
    free(wstr);

    return sf_string;
}
