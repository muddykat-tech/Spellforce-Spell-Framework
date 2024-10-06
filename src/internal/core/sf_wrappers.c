#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sf_wrappers.h"
#include "sf_hooks.h"

#include "hooks/sf_menu_hook.h"
#include "hooks/sf_console_hook.h"

static FUN_0069eaf0_ptr FUN_0069eaf0;
static fidfree_ptr fidFree;

SF_String_ctor_ptr g_create_sf_string;
SF_String_dtor_ptr g_destroy_sf_string;

void initialize_wrapper_data_hooks()
{
    FUN_0069eaf0 = (FUN_0069eaf0_ptr)(ASI::AddrOf(0x29EAF0));
    fidFree = (fidfree_ptr)(ASI::AddrOf(0x6B6E25));
}

void log_message(const char *filename, const char *message)
{
    FILE *file = fopen(filename, "a");
    if (file != NULL)
    {
        fprintf(file, "%s\n", message);
        fclose(file);
    }
}

void log_warning(const char *message)
{
    // Logs a warning message to the console and the debug output
    static char modifiedMessage[256];
    snprintf(modifiedMessage, sizeof(modifiedMessage), "[WARNING] %s", message);
    console_log(modifiedMessage);
}

void log_info(const char *message)
{
    // Logs an informational message to the console and the debug output
    static char modifiedMessage[256];
    snprintf(modifiedMessage, sizeof(modifiedMessage), "[SFSF] %s", message);
    console_log(modifiedMessage);
}

void log_error(const char *message)
{
    // Logs an error message to the console and the debug output, including the last error code
    int lastError = GetLastError();
    static char modifiedMessage[256];
    snprintf(modifiedMessage, sizeof(modifiedMessage), "[ERROR] %s [Last Error: %d]", message, lastError);
    console_log(modifiedMessage);
}

SFLog sf_logger;
SFLog *setup_logger()
{
    sf_logger.logError = &log_error;
    sf_logger.logInfo = &log_info;
    sf_logger.logWarning = &log_warning;
    return &sf_logger;
}

// TODO: Parse me through an as an ActionAPI Wrapped Function
bool __thiscall isActionMelee(SF_SGtFigureAction *_this)
{
    if ((_this->type == 10000) || (_this->type == 0x2711))
    {
        return 1;
    }
    return 0;
}

void __thiscall setupFigureIterator(CGdFigureIterator *iterator, SF_CGdSpell *spell)
{
    iteratorAPI.figureIteratorInit(iterator, 0x0, 0x0, 0x3ff, 0x3ff);
    iteratorAPI.figureIteratorSetPointers(iterator, spell->SF_CGdFigure, spell->unkn3, spell->SF_CGdWorld);
}

uint32_t getDistance(SF_Coord *pointA, SF_Coord *pointB)
{

    uint32_t delta;
    uint32_t uVar1;
    uint32_t uVar2;
    uint32_t uVar3;
    uint32_t uVar4;

    delta = (uint32_t)(uint16_t)pointA->X - (uint32_t)(uint16_t)pointB->X;
    uVar2 = (int)delta >> 0x1f;
    uVar2 = (delta ^ uVar2) - uVar2;
    uVar4 = uVar2 & 0xffff;
    delta = (uint32_t)(uint16_t)pointA->Y - (uint32_t)(uint16_t)pointB->Y;
    uVar3 = (int)delta >> 0x1f;
    uVar3 = (delta ^ uVar3) - uVar3;
    uVar1 = uVar3 & 0xffff;
    delta = uVar1;
    if ((uint16_t)uVar2 < (uint16_t)uVar3)
    {
        delta = uVar4;
        uVar4 = uVar1;
    }
    return ((delta * 0xd) >> 5) + uVar4;
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
    FigureStatistic *statistic;
    switch (key)
    {
    case ARMOR:
        statistic = &(figure->figures[target].armor);
        break;
    case AGILITY:
        statistic = &(figure->figures[target].agility);
        break;
    case CHARISMA:
        statistic = &(figure->figures[target].charisma);
        break;
    case DEXTERITY:
        statistic = &(figure->figures[target].dexterity);
        break;
    case INTELLIGENCE:
        statistic = &(figure->figures[target].intelligence);
        break;
    case STRENGTH:
        statistic = &(figure->figures[target].strength);
        break;
    case WISDOM:
        statistic = &(figure->figures[target].wisdom);
        break;
    case RESISTANCE_FIRE:
        statistic = &(figure->figures[target].resistance_fire);
        break;
    case RESISTANCE_ICE:
        statistic = &(figure->figures[target].resistance_ice);
        break;
    case RESISTANCE_MENTAL:
        statistic = &(figure->figures[target].resistance_mental);
        break;
    case RESISTANCE_BLACK:
        statistic = &(figure->figures[target].resistance_black);
        break;
    case WALK_SPEED:
        statistic = &(figure->figures[target].walk_speed);
        break;
    case FIGHT_SPEED:
        statistic = &(figure->figures[target].fight_speed);
        break;
    case CAST_SPEED:
        statistic = &(figure->figures[target].cast_speed);
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

void __thiscall spellClearFigureFlag(SF_CGdSpell *_this, uint16_t spell_id, SpellFlagKey key)
{
    switch (key)
    {
    case CHECK_SPELLS_BEFORE_CHECK_BATTLE:
        spellAPI.figClrChkSplBfrChkBattle(_this, spell_id, 0);
        break;
    case CHECK_SPELLS_BEFORE_JOB2:
        spellAPI.figTryClrCHkSPlBfrJob2(_this, spell_id);
        break;
    case UNFREEZE:
        spellAPI.figTryUnfreeze(_this, spell_id, 0);
        break;
    }
}

// Temp
typedef void(__thiscall *vfunction_ptr)(void *label, char *p1);
typedef void(__thiscall *vfunction12_ptr)(void *container, void *test, char *p1);

vfunction_ptr vfunction176;
vfunction_ptr vfunction25;
vfunction12_ptr vfunction12;

void __thiscall attach_new_label(CMnuContainer *parent, char *label_chars, uint8_t font_index, uint16_t x_pos, uint16_t y_pos, uint16_t width, uint16_t height)
{
    SF_String m_label_string;
    CMnuLabel *new_label;
    SF_FontStruct *fonts = g_get_smth_fonts();

    SF_String *label_string = g_create_sf_string(&m_label_string, label_chars);
    new_label = (CMnuLabel *)g_new_operator(0x368);

    if (font_index > 32)
    {
        log_error("Invalid font index 0~32, defaulting to font 6");
        font_index = 6;
    }

    SF_Font *selected_font = g_get_font(fonts, font_index);

    g_menu_label_constructor(new_label);

    // Start setting flags to tell Spellforce what this label is used for.
    g_set_label_flags(new_label, 7);

    g_init_menu_element(new_label, x_pos, y_pos, width, height, label_string);

    vfunction176 = (vfunction_ptr)(ASI::AddrOf(0x52f520));
    vfunction176(new_label, (char *)0x1);

    vfunction25 = (vfunction_ptr)(ASI::AddrOf(0x511ae0));
    vfunction25(new_label, (char *)0x0);

    g_menu_label_set_font(new_label, selected_font);

    g_container_add_control(parent, new_label, (char *)0x01, (char *)0x01, 0);

    vfunction12 = (vfunction12_ptr)(ASI::AddrOf(0x511ae0));
    vfunction12(parent, new_label, (char *)0x0);
    g_menu_label_set_string(new_label, label_string);

    g_destroy_sf_string(label_string);
}

uint16_t __thiscall sf_get_spell_id(SF_CGdSpell *_this, uint16_t spell_index)
{
    return _this->active_spell_list[spell_index].spell_id;
}

/*
    char mod_id[64];
    char mod_version[24];
    char mod_description[128];
    char mod_author[128];
*/
SFMod *createModInfo(const char *mod_id, const char *mod_version, const char *mod_author, const char *mod_description)
{
    // Sanitize the mod info, no buffer overflows for us!
    SFMod *mod = (SFMod *)malloc(sizeof(SFMod));

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