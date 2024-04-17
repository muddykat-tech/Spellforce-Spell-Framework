typedef void (*InitModuleFunc)(SpellforceFramework*);

InitModuleFunc initializeModule;

SpellforceFramework() : initializeModule(SpellforceFramework* pfas) {}
