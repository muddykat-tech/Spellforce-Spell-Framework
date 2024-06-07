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
    console_log(modifiedMessage);
}

void log_info(const char *message) {
    // Logs an informational message to the console and the debug output
    static char modifiedMessage[256]; 
    snprintf(modifiedMessage, sizeof(modifiedMessage), "[SFSF] %s", message);
    console_log(modifiedMessage);
}

void log_error(const char* message) {
    // Logs an error message to the console and the debug output, including the last error code
    int lastError = GetLastError();
    static char modifiedMessage[256];
    snprintf(modifiedMessage, sizeof(modifiedMessage), "[ERROR] %s [Last Error: %d]", message, lastError);
    console_log(modifiedMessage);
}

SFLog sf_logger;
SFLog* setup_logger()
{
    sf_logger.logError = &log_error;
    sf_logger.logInfo = &log_info;
    sf_logger.logWarning = &log_warning;
    return &sf_logger;
}

void __thiscall setupFigureIterator(CGdFigureIterator *iterator, SF_CGdSpell *spell)
{
    iteratorAPI.figureIteratorInit(iterator, 0x0, 0x0, 0x3ff, 0x3ff);
    iteratorAPI.figureIteratorSetPointers(iterator, spell->SF_CGdFigure, spell->unkn3, spell->SF_CGdWorld);
}

// Some funky stuff to clean up Iterator memory, not 100% sure if correct
void __thiscall disposeFigureIterator(CGdFigureIterator iterator)
{
    uint32_t unused;
    FUN_0069eaf0(&iterator.data.offset_0x30, &unused, ((AutoClass69 *)iterator.data.offset_0x30.ac69_ptr1)->ac69_ptr1, iterator.data.offset_0x30.ac69_ptr1);
    fidFree(iterator.data.offset_0x30.ac69_ptr1);
}

void __thiscall addBonusMultToStatistic(SF_CGdFigure *figure, StatisticDataKey key, uint16_t target, int8_t value)
{
    bool invalid = FALSE;
    FigureStatistic statistic;
    switch (key)
    {
    case ARMOR:
        statistic = figure->figures[target].armor;
        break;
    case AGILITY:
        statistic = figure->figures[target].agility;
        break;
    case CHARISMA:
        statistic = figure->figures[target].charisma;
        break;
    case DEXTERITY:
        statistic = figure->figures[target].dexterity;
        break;
    case INTELLIGENCE:
        statistic = figure->figures[target].intelligence;
        break;
    case STRENGTH:
        statistic = figure->figures[target].strength;
        break;
    case WISDOM:
        statistic = figure->figures[target].wisdom;
        break;
    case RESISTANCE_FIRE:
        statistic = figure->figures[target].resistance_fire;
        break;
    case RESISTANCE_ICE:
        statistic = figure->figures[target].resistance_ice;
        break;
    case RESISTANCE_MENTAL:
        statistic = figure->figures[target].resistance_mental;
        break;
    case RESISTANCE_BLACK:
        statistic = figure->figures[target].resistance_black;
        break;
    case WALK_SPEED:
        statistic = figure->figures[target].walk_speed;
        break;
    case FIGHT_SPEED:
        statistic = figure->figures[target].fight_speed;
        break;
    case CAST_SPEED:
        statistic = figure->figures[target].cast_speed;
        break;
    default:
        // Handle default case if needed
        invalid = TRUE;
        break;
    }

    if (invalid)
    {
        log_warning("INVALID STATISTIC KEY");
        return;
    }

    figureAPI.addBonusMult(statistic, value);
}


void __thiscall spellClearFigureFlag(SF_CGdSpell* _this, uint16_t spell_id, SpellFlagKey key)
{
    switch(key)
    {
        case CHECK_SPELLS_BEFORE_CHECK_BATTLE:
            spellAPI.figClrChkSplBfrChkBattle(_this, spell_id, 0);
        break;
        case CHECK_SPELLS_BEFORE_JOB2:
            spellAPI.figTryClrCHkSPlBfrJob2(_this, spell_id, 0);
        break;
        case UNFREEZE:
            spellAPI.figTryUnfreeze(_this, spell_id, 0);
        break;
    }
}

/*
	char mod_id[64];
	char mod_version[24];
	char mod_description[128];
	char mod_author[128];
*/
SFMod *createModInfo(const char* mod_id, const char* mod_version, const char* mod_author, const char* mod_description){
    // Sanitize the mod info, no buffer overflows for us!
    SFMod *mod = (SFMod*)malloc(sizeof(SFMod));

    strncpy(mod->mod_id, mod_id, 63);
    mod->mod_id[63] = '\0';
 
    strncpy(mod->mod_version, mod_version, 23);
    mod->mod_version[23] = '\0';

    strncpy(mod->mod_description, mod_description, 127);
    mod->mod_description[127] = '\0';

    strncpy(mod->mod_author, mod_author, 127);
    mod->mod_author[127] = '\0';

    return mod;
}