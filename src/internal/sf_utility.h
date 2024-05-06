#ifndef UTILITY_H
#define UTILITY_H

#include "../api/sf_spells.h"

extern "C" void logWarning(const char *message);
extern "C" void logInfo(const char *message);
extern "C" void logError(const char *message);
extern "C" SF_String* convCharToString(const char *message);
#endif