#include <string>
#include "SpellforceSpellFramework.h"

extern "C" __declspec(dllexport) void initializeModule(SpellforceSpellFramework* framework) {
    printf("Module initialized successfully!\n");
}
