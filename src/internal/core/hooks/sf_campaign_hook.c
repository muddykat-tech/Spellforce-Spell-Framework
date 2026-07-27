/**
 * @addtogroup CampaignHook
 * @{
 */

#include "../sf_wrappers.h"
#include "../sf_ui_wrappers.h"
#include "sf_campaign_hook.h"
#include "../sf_hooks.h"

#include "sf_prepare_new_game.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>

static gameInfoSetMapPathFull_ptr s_gameinfo_set_map_path; // deprecated I think
static appMenuEnterCampaignFlow_ptr s_enter_campaign_flow;

static videoSequenceStop_ptr s_video_sequence_stop;

static void hook_getsavepath();

static avatarInternalCopy_ptr s_avatar_internal_copy;
static avatarVectorsCopy_ptr s_avatar_vectors_copy;
static void hook_initfirstmap();
static SF_String g_sfsf_load_dir;   /* "save\" when idle; "save\campaigns\<x>\" when active */
static void set_load_dir_for_campaign(const SFSF_CampaignDef *);
static void patch_load_dir_ref();

// used in prepare new game
startGame_ptr s_start_game;
playCampaignIntro_ptr s_play_campaign_intro;
getBasePathString_ptr s_get_base_path_string;
gameInfoSetAvatarType_ptr s_gameinfo_set_avatar_type;

typedef LPCSTR *(__cdecl *GetDataStorageLocation_ptr)(char **param_1, uint32_t type);
typedef uint32_t (__cdecl *checkFileExists_ptr)(SF_String *name);
typedef void (__thiscall *prepareTransition_ptr)(CAppMenu *_this,uint32_t param_1,uint32_t param_2);
typedef SF_GameInfo *(__thiscall *initDefaultInfo_ptr)(SF_GameInfo *_this);
typedef void (__thiscall *AC82_Zero_ptr)(AutoClass82 *_this);
static GetDataStorageLocation_ptr s_getDataStorageLocation;
prepareTransition_ptr prepareTransition;
initDefaultInfo_ptr initDefaultInfo;
AC82_Zero_ptr AC82_Zero;

//TODO -- make this function available for framework. Will need for proper loading screens and maps later on!!!
checkFileExists_ptr checkFileExists;

void initialize_campaign_hooks()
{

    s_getDataStorageLocation = (GetDataStorageLocation_ptr)(ASI::AddrOf(0x1ee9f0));
    s_gameinfo_set_map_path = (gameInfoSetMapPathFull_ptr)(ASI::AddrOf(0x1762d0));
    s_enter_campaign_flow   = (appMenuEnterCampaignFlow_ptr)(ASI::AddrOf(0x1936a0));
    s_play_campaign_intro   = (playCampaignIntro_ptr)(ASI::AddrOf(0x181e20));
    s_video_sequence_stop   = (videoSequenceStop_ptr)(ASI::AddrOf(0x3d7b90));
    s_start_game            = (startGame_ptr)(ASI::AddrOf(0x183560));
    s_get_base_path_string  = (getBasePathString_ptr)(ASI::AddrOf(0x61a700));

    s_gameinfo_set_avatar_type = (gameInfoSetAvatarType_ptr)(ASI::AddrOf(0x1a1a80));

    s_avatar_internal_copy = (avatarInternalCopy_ptr)(ASI::AddrOf(0x1759e0));
    s_avatar_vectors_copy  = (avatarVectorsCopy_ptr)(ASI::AddrOf(0x175860));
    checkFileExists = (checkFileExists_ptr)(ASI::AddrOf(0x4f7d50));
    prepareTransition = (prepareTransition_ptr)(ASI::AddrOf(0x199db0));
    initDefaultInfo = (initDefaultInfo_ptr)(ASI::AddrOf(0x175760));
    AC82_Zero = (AC82_Zero_ptr)(ASI::AddrOf(0x19e730));

    uiAPI.SFStringConstructor_char(&g_sfsf_load_dir, "save\\");
    hook_initfirstmap();
    hook_getsavepath();

    initialize_preparenewgame_rewrite();

    set_load_dir_for_campaign(NULL); //ensures default vanilla behavior - Replacement with actual rewrite
    patch_load_dir_ref(); // Same here

    // TEST CAMPAIGN - remove once registration comes from the mod registry
    SFSF_CampaignDef test_campaign = {};
    strncpy(test_campaign.name,        "testcampaign",      sizeof(test_campaign.name) - 1);
    strncpy(test_campaign.description, "Hook test",         sizeof(test_campaign.description) - 1);
    strncpy(test_campaign.start_map,   "000_liannon",   sizeof(test_campaign.start_map) - 1);
    strncpy(test_campaign.campaign_folder, "testcampaign",      sizeof(test_campaign.campaign_folder) - 1);
    strncpy(test_campaign.tutorial_map, "",                 sizeof(test_campaign.tutorial_map) - 1);
    test_campaign.avatar_type = 8;   // 8 + campaign_index convention
    register_campaign(&test_campaign);
}

SFSF_CampaignDef g_campaigns[SFSF_MAX_CAMPAIGNS];
uint32_t g_campaign_count = 0;
int32_t g_active_custom_campaign = -1;
CAppMenu *g_campaign_app_menu = NULL;

int32_t register_campaign(const SFSF_CampaignDef *def)
{
    if (g_campaign_count >= SFSF_MAX_CAMPAIGNS || def == NULL)
    {
        log_error("Campaign registry full or NULL definition");
        return -1;
    }

    g_campaigns[g_campaign_count] = *def;
    if (g_campaigns[g_campaign_count].avatar_type == 0)
        g_campaigns[g_campaign_count].avatar_type = 8 + g_campaign_count;

    log_info("Registered campaign %u: %s (type %u, map: %s, saves: %s)",
             g_campaign_count, def->name,
             SFSF_CAMPAIGN_TYPE_BASE + g_campaign_count,
             def->start_map, def->campaign_folder);
    return (int32_t)g_campaign_count++;
}

/* Un'Schtalch's code block - updated and reworked a bit for you ~Muddykat*/


//TODO -- rename into getSavePath later on.
SF_String * getSavePath_1(SF_String *output, uint32_t campaign_type)
{
    char *paths[3];
    SF_String base_save;
    s_getDataStorageLocation(paths, 0);
    uiAPI.SFStringConstructor_char(output, paths[0]);
    uiAPI.SFStringConstructor_wchar(&base_save, L"save\\");
    uiAPI.SFStringConcat(output, &base_save);
    uiAPI.SFStringDestructor(&base_save);

    switch (campaign_type)
    {
        case 0:
        {
            //don't need to do anything
            break;
        }
        case 1:
        {
            SF_String campaign_path;
            uiAPI.SFStringConstructor_wchar(&campaign_path, L"campaign2\\");
            uiAPI.SFStringConcat(output, &campaign_path);
            uiAPI.SFStringDestructor(&campaign_path);

            break;
        }
        case 2:
        {
            SF_String campaign_path;
            uiAPI.SFStringConstructor_wchar(&campaign_path, L"campaign3\\");
            uiAPI.SFStringConcat(output, &campaign_path);
            uiAPI.SFStringDestructor(&campaign_path);
            break;
        }
        default:
        {
            //place to add for custom campaign stuff
            break;
        }
    }
    return output;
}
typedef SF_String *(__thiscall *AC95_get_figure_name_ptr)(void *AC95, SF_String *name_buffer, uint32_t figure_id);

extern AC95_get_figure_name_ptr AC95_get_figure_name;


void __thiscall loadQuickSave(CAppMenu *_this)
{
    log_info("QuickSave Load run!");
    uint16_t player = _this->CAppMenu_data.CAppSession->data.controllerClient->data.current_player;

    bool inUse = (_this->CAppMenu_data.CAppSession->data.CGdMain->data.CGdPlayer->players[player].use != 0);
    uint16_t figure_id =
        _this->CAppMenu_data.CAppSession->data.CGdMain->data.CGdPlayer->players[player].avatar_figure_index;
    if (!inUse)
    {
        return;
    }
    SF_String avatar_name;
    SF_String tilda;
    SF_String quicksave;
    SF_String base_path;
    AC95_get_figure_name(_this->CAppMenu_data.AC95, &avatar_name, figure_id);
    uiAPI.SFStringFromWchar(&tilda, L'~', 1);
    uiAPI.SFStringConstructor_wchar(&quicksave, L"QUICKSAVE.SAV");
    uiAPI.SFStringConcat(&avatar_name, &tilda);
    uiAPI.SFStringConcat(&avatar_name, &quicksave);
    getSavePath_1(&base_path, _this->CAppMenu_data.campaign_type);
    uiAPI.SFStringConcat(&base_path, &avatar_name);
    if (checkFileExists(&base_path))
    {
        prepareTransition(_this, 1, 1);
        SF_GameInfo newInfo;
        initDefaultInfo(&newInfo);
        newInfo.unknown_0xf0 = _this->CAppMenu_data.game_info.unknown_0xf0;
        uiAPI.SFStringDeepCopy(&newInfo.filename, &avatar_name);
        newInfo.is_tutorial = 0;
        newInfo.unknown_0xf4 = 2;
        newInfo.start_mode = 0;
        SF_String dummy;
        uiAPI.SFStringConstructor(&dummy);
        s_start_game(_this, &newInfo, 100, 0, 0, 0, &dummy);
        uiAPI.SFStringDestructor(&dummy);

        AC82_Zero(&newInfo.AC82_1);
        uiAPI.SFStringDestructor(&newInfo.starter_kit_name);
        uiAPI.SFStringDestructor(&newInfo.template_name);
        uiAPI.SFStringDestructor(&newInfo.filename);
        AC82_Zero(&newInfo.AC82);
    }
    uiAPI.SFStringDestructor(&avatar_name);
    uiAPI.SFStringDestructor(&tilda);
    uiAPI.SFStringDestructor(&quicksave);
    uiAPI.SFStringDestructor(&base_path);
}

/**
 * @brief Proper map selection code for the future
 * @param _this Pointer to the game info structure that holds most of the info we need
 * @param skip_tutorial Totu
 * @param skill Primary skill for the newly created avatar
 * @param subskill Secondary skill for the newly created avatar
 * @param campaign_id Campaign ID we're loading. (0 - Order, 1 - Aryn, 2 - Phoenix)
 * @param is_shadowblade SotP side flag, used only for the third vanilla campaign
 */
void __thiscall initFirstMap(SF_GameInfo *_this, uint32_t skip_tutorial, uint8_t skill,
                             uint8_t subskill, uint32_t campaign_id, bool is_shadowblade)
{
    SF_String default_template;
    SF_String template_path;
    SF_String full_template;
    SF_String dot_map;
    SF_String campagn_path;
    SF_String intial_map_name;
    log_info("initFirstMap Hook: Starter Kit Start ");
    /* -- starter kit: figure_template\starterkit\SK_<skill><subskill>.des -- */
    uiAPI.SFStringConstructor_char(&dot_map, ".map");
    uiAPI.SFStringConstructor(&full_template);
    uiAPI.SFStringConstructor(&default_template);
    uiAPI.SFprintf(&default_template, L"SK_%02d%02d.des", skill, subskill);
    uiAPI.SFStringConstructor_char(&template_path, "figure_template\\starterkit\\");
    uiAPI.SFStringConcat(&full_template, &template_path);
    uiAPI.SFStringConcat(&full_template, &default_template);
    uiAPI.SFStringDeepCopy(&_this->starter_kit_name, &full_template);

    log_info("initFirstMap Hook: Avatar Snapshot to deep Copy");
    /* -- avatar snapshot AC82 -> AC82_1: FULL deep copy (vanilla parity).
     *    Downstream flows (restart map, PrepareNewGame) read AC82_1;
     *    the shallow field copies alone are NOT sufficient. -- */
    _this->AC82_1.unknown1  = _this->AC82.unknown1;
    log_info("initFirstMap Hook: Avatar Snapshot Ran: _this->AC82_1.unknown1  = _this->AC82.unknown1;");
    _this->AC82_1.unknown2  = _this->AC82.unknown2;
    log_info("initFirstMap Hook: Avatar Snapshot Ran: _this->AC82_1.unknown2  = _this->AC82.unknown2;");
    _this->AC82_1.unknown3  = _this->AC82.unknown3;
    log_info("initFirstMap Hook: Avatar Snapshot Ran: _this->AC82_1.unknown3  = _this->AC82.unknown3;");
    _this->AC82_1.kit_index = _this->AC82.kit_index;
    log_info("initFirstMap Hook: Avatar Snapshot Ran: _this->AC82_1.kit_index = _this->AC82.kit_index;");
    s_avatar_internal_copy(&_this->AC82_1.avatarData.internal, &_this->AC82.avatarData.internal);
    log_info(
        "initFirstMap Hook: Avatar Snapshot Ran: s_avatar_internal_copy(&_this->AC82_1.avatarData.internal, &_this->AC82.avatarData.internal);");
    s_avatar_vectors_copy(&_this->AC82_1.avatarData.begin, &_this->AC82.avatarData.begin);
    log_info(
        "initFirstMap Hook: Avatar Snapshot Ran: s_avatar_vectors_copy(&_this->AC82_1.avatarData.begin, &_this->AC82.avatarData.begin);");

    log_info("initFirstMap Hook: Check for Custom Campaign");
    /* -- active custom campaign? -- */
    const SFSF_CampaignDef *custom = NULL;
    int custom_idx = (int)campaign_id - SFSF_CAMPAIGN_TYPE_BASE;
    if (custom_idx >= 0 && custom_idx < (int32_t)g_campaign_count)
    {
        custom = &g_campaigns[custom_idx];
    }

    /* -- folder + start map: selected FIRST, tutorial branch only overrides
     *    the map NAME (vanilla order - vanilla tutorials live in their own
     *    campaign's folder, e.g. map\Campaign2\tutorial.map). -- */
    if (custom != NULL)
    {
        log_info("initFirstMap Hook: Attempting to load Custom Campaign");
        char folder[160];
        snprintf(folder, sizeof(folder), "map\\%s\\", custom->campaign_folder);
        uiAPI.SFStringConstructor_char(&campagn_path, folder);
        uiAPI.SFStringConstructor_char(&intial_map_name, custom->start_map);

        /* No tutorial map defined: force a direct start even if the player
         * left the tutorial checkbox on. */
        if (!skip_tutorial && custom->tutorial_map[0] == '\0')
        {
            skip_tutorial = 1;
        }
        log_info("Campaign '%s': maps from %s", custom->name, folder);
    }
    else
    {
        log_info("initFirstMap Hook: Not Custom - Deverting to Vanilla Flow");
        switch (campaign_id)
        {
            case 1:
                log_info("initFirstMap Hook: Engine Type 1");
                uiAPI.SFStringConstructor_char(&campagn_path, "map\\Campaign2\\");
                uiAPI.SFStringConstructor_wchar(&intial_map_name, L"P101_Mirraw_Thur");
                break;
            case 2:
                log_info("initFirstMap Hook: Engine Type 2");
                uiAPI.SFStringConstructor_char(&campagn_path, "map\\Campaign3\\");
                if (is_shadowblade)
                {
                    uiAPI.SFStringConstructor_wchar(&intial_map_name, L"P202_City_Of_Souls");
                }
                else
                {
                    uiAPI.SFStringConstructor_wchar(&intial_map_name, L"P201_Blackwater_Coast");
                }
                break;
            case 0:
            default:
                log_info("initFirstMap Hook: Engine Type 0");
                /* default falls back to Order so the strings are ALWAYS
                 * constructed - the epilogue destructors depend on it. */
                if (campaign_id > 2)
                {
                    log_error("initFirstMap: unexpected campaign_id %u, falling back to Order",
                              campaign_id);
                }
                uiAPI.SFStringConstructor_char(&campagn_path, "map\\Campaign\\");
                uiAPI.SFStringConstructor_wchar(&intial_map_name, L"000_Greyfell");
                break;
        }
    }

    log_info("initFirstMap Hook: Check Tutorial");
    if (skip_tutorial)
    {
        log_info("initFirstMap Hook: Skipping Tutorial");
        /* Direct start: Assign player their skill-derived starter kit (armor equipment ect). */
        uiAPI.SFStringDeepCopy(&_this->template_name, &_this->starter_kit_name);
        log_info("initFirstMap Hook; setting GameInfo");
        _this->start_mode  = 2;
        _this->is_tutorial = 0;
    }
    else
    {
        log_info("initFirstMap Hook: Loading Tutorial");
        /* Tutorial: swap the map NAME (folder already selected above),
         * fixed tutorial figure template. */
        uiAPI.SFStringDestructor(&intial_map_name);
        if (custom != NULL)
        {
            uiAPI.SFStringConstructor_char(&intial_map_name, custom->tutorial_map);
        }
        else
        {
            uiAPI.SFStringConstructor_wchar(&intial_map_name, L"tutorial");
        }

        log_info("initFirstMap Hook: Tutorial Kit Load");
        SF_String tutorial_template;
        SF_String tutorial_kit_name;
        SF_String tutorial_template_path;
        uiAPI.SFStringConstructor_wchar(&tutorial_template, L"FT_Tutorial.des");
        uiAPI.SFStringConstructor_char(&tutorial_template_path, "figure_template\\");
        uiAPI.SFStringConstructor(&tutorial_kit_name);
        uiAPI.SFStringConcat(&tutorial_kit_name, &tutorial_template_path);
        uiAPI.SFStringConcat(&tutorial_kit_name, &tutorial_template);
        uiAPI.SFStringDeepCopy(&_this->template_name, &tutorial_kit_name);
        uiAPI.SFStringDestructor(&tutorial_kit_name);
        uiAPI.SFStringDestructor(&tutorial_template);
        uiAPI.SFStringDestructor(&tutorial_template_path);

        log_info("initFirstMap Hook: Updating GameInfo");
        _this->start_mode  = 1;
        _this->is_tutorial = 1;
    }

    log_info("initFirstMap Hook: File Name Settings");
    /* -- filename = <folder><map>.map + branch-invariant state -- */
    uiAPI.SFStringConcat(&campagn_path, &intial_map_name);
    uiAPI.SFStringConcat(&campagn_path, &dot_map);
    uiAPI.SFStringDeepCopy(&_this->filename, &campagn_path);
    _this->unknown_0xf0 = 1;
    _this->unknown_0xf4 = 3;

    log_info("initFirstMap Hook: Cleanup");
    uiAPI.SFStringDestructor(&default_template);
    uiAPI.SFStringDestructor(&template_path);
    uiAPI.SFStringDestructor(&full_template);
    uiAPI.SFStringDestructor(&dot_map);
    uiAPI.SFStringDestructor(&campagn_path);
    uiAPI.SFStringDestructor(&intial_map_name);
}

static void hook_qs_load()
{
    ASI::MemoryRegion mreg_qs(ASI::AddrOf(0x185c00), 5);
    ASI::BeginRewrite(mreg_qs);
    *(unsigned char *)(ASI::AddrOf(0x185c00)) = 0xE9; // JMP instruction
    *(int *)(ASI::AddrOf(0x185c01)) = (int)(&loadQuickSave) - ASI::AddrOf(0x185c05);
    ASI::EndRewrite(mreg_qs);
    log_info("QuickSave Load replacement hooked (entry JMP)");
}

static void hook_initfirstmap()
{
    //FIXME
    //Mate, optimizaton MIGHT kill this code. We have tried this already for stats overflow fix, please, DONT ~Un'Schtalch
    uint32_t entry = ASI::AddrOf(0x176040);
    ASI::MemoryRegion mreg(entry, 5);
    ASI::BeginRewrite(mreg);
    *(unsigned char *)entry = 0xE9;
    *(int *)(entry + 1) = (int)(&initFirstMap) - (int)(entry + 5);
    ASI::EndRewrite(mreg);
    log_info("initFirstMap replacement hooked (entry JMP)");
}

// basically hacky way to "override" quick save location.
// because campaign switch is checking for type == 1 {} else {if(type == 2){} basic behavior here }
// we override the string pointer to ours.
// e.g PUSH SF_String_save is now PUSH SFSF_Load_dir
// will this work... maybe
static void patch_load_dir_ref()
{
    // uint32_t site = ASI::AddrOf(0x185d3e);
    // ASI::MemoryRegion mreg(site, 5);
    // ASI::BeginRewrite(mreg);
    // *(unsigned char *)site = 0x68; //push should be unchanged essentially
    // *(int *)(site + 1) = (int)(&g_sfsf_load_dir) - (int) (site+5);
    // ASI::EndRewrite(mreg);
    // log_info("Hacky Patch for save location");
}

void campaign_hook_on_main_menu(CAppMenu *app_menu)
{
    g_campaign_app_menu = app_menu;
    g_active_custom_campaign = -1;
    set_load_dir_for_campaign(NULL);
}

/**
 * reimplementation of 0x1b89d0 + custom branch for custom campaigns ^_^
 */
SF_String * __stdcall getSavePath(SF_String *out, int campaign_type)
{
    SF_String base;
    s_get_base_path_string(&base);

    char narrow[512];
    int len = 0;
    if (base.raw_data != NULL && base.str_length > 0)
    {
        len = WideCharToMultiByte(CP_ACP, 0, base.raw_data, base.str_length,
                                  narrow, (int)sizeof(narrow) - 1, NULL, NULL);
        if (len < 0)
        {
            len = 0;
        }
    }
    narrow[len] = '\0';
    uiAPI.SFStringDestructor(&base);

    const char *suffix;
    char custom_suffix[192];
    int idx = campaign_type - SFSF_CAMPAIGN_TYPE_BASE;
    if (idx >= 0 && idx < (int32_t)g_campaign_count)
    {
        snprintf(custom_suffix, sizeof(custom_suffix), "save\\campaigns\\%s\\",
                 g_campaigns[idx].campaign_folder);
        suffix = custom_suffix;
    }
    else if (campaign_type == 1)
    {
        suffix = "save\\campaign2\\";
    }
    else if (campaign_type == 2)
    {
        suffix = "save\\campaign3\\";
    }
    else
    {
        suffix = "save\\";
    }

    char full[768];
    snprintf(full, sizeof(full), "%s%s", narrow, suffix);

    uiAPI.SFStringConstructor_char(out, full);
    return out;
}

static void hook_getsavepath()
{
    ASI::MemoryRegion mreg(ASI::AddrOf(0x1b89d0), 5);
    ASI::BeginRewrite(mreg);
    *(unsigned char *)(ASI::AddrOf(0x1b89d0)) = 0xE9; /* JMP */
    *(int *)(ASI::AddrOf(0x1b89d1)) =
        (int)(&getSavePath) - ASI::AddrOf(0x1b89d5);
    ASI::EndRewrite(mreg);
    log_info("getSavePath replacement hooked");
}

static void stop_intro_video(CAppMenu *app_menu)
{
    SF_CUiVideoSequence *seq =
        (SF_CUiVideoSequence *)app_menu->CAppMenu_data.CUiVideoSequence_ptr;
    if (seq != NULL)
    {
        s_video_sequence_stop(seq);
        app_menu->CAppMenu_data.CUiVideoSequence_ptr = NULL;
    }
}

static void ensure_campaign_dirs(const SFSF_CampaignDef *def)
{
    SF_String base;
    s_get_base_path_string(&base);          /* Documents\<GameFolder>\ */
    char narrow[512];
    int len = 0;
    if (base.raw_data && base.str_length > 0)
        len = WideCharToMultiByte(CP_ACP, 0, base.raw_data, base.str_length,
                                  narrow, sizeof(narrow) - 1, NULL, NULL);
    if (len < 0)
        len = 0;
    narrow[len] = '\0';
    uiAPI.SFStringDestructor(&base);

    char path[768];
    snprintf(path, sizeof(path), "%ssave", narrow);
    CreateDirectoryA(path, NULL);
    snprintf(path, sizeof(path), "%ssave\\campaigns", narrow);
    CreateDirectoryA(path, NULL);
    snprintf(path, sizeof(path), "%ssave\\campaigns\\%s", narrow, def->campaign_folder);
    CreateDirectoryA(path, NULL);
}

static bool s_load_dir_constructed = false;

static void set_load_dir(const char *dir)
{
    SF_String tmp;
    uiAPI.SFStringConstructor_char(&tmp, dir);
    if (!s_load_dir_constructed)
    {
        uiAPI.SFStringConstructor(&g_sfsf_load_dir);
        s_load_dir_constructed = true;
    }

    uiAPI.SFStringCopy(&g_sfsf_load_dir, &tmp);
    uiAPI.SFStringDestructor(&tmp);
}

/** Point the patched load-dir at a campaign's save folder, or back at
 *  vanilla ("save\") when def == NULL. */
void set_load_dir_for_campaign(const SFSF_CampaignDef *def)
{
    if (def == NULL)
    {
        set_load_dir("save\\");
        return;
    }
    char dir[192];
    snprintf(dir, sizeof(dir), "save\\campaigns\\%s\\", def->campaign_folder);
    set_load_dir(dir);
    log_info("Quickload dir -> %s", dir);
}

static void campaign_launch_flow(int32_t campaign_index)
{
    CAppMenu *app_menu = g_campaign_app_menu;
    const SFSF_CampaignDef *def = &g_campaigns[campaign_index];

    if (app_menu == NULL || s_gameinfo_set_avatar_type == NULL)
    {
        log_error("Campaign launch unavailable: missing CAppMenu or engine addresses");
        return;
    }
    stop_intro_video(app_menu);
    g_active_custom_campaign = campaign_index;              /* UI state only */

    log_info("Launching campaign '%s' (flow, engine_type %u, avatar %u)",
             def->name, g_active_custom_campaign, def->avatar_type);

    ensure_campaign_dirs(def);
    set_load_dir_for_campaign(def);                         /* g_sfsf_load_dir -> save\campaigns\<folder>\ */

    app_menu->CAppMenu_data.campaign_type = SFSF_CAMPAIGN_TYPE_BASE + campaign_index;
    s_gameinfo_set_avatar_type(&app_menu->CAppMenu_data.game_info,
                               (uint16_t)def->avatar_type);
    s_enter_campaign_flow(app_menu, 1);
}

static bool s_screen_exists = false;
static bool s_screen_visible = false;
static CMnuContainer *s_campaign_screen = NULL;
static CMnuSmpButton *s_campaign_buttons[SFSF_MAX_CAMPAIGNS];

void __thiscall on_campaign_selected(CMnuSmpButton *_this)
{
    for (uint32_t i = 0; i < g_campaign_count; i++)
    {
        if (s_campaign_buttons[i] == _this)
        {
            campaign_launch_flow((int32_t)i);
            return;
        }
    }
    log_error("Campaign button not found in registry");
}

static void close_campaign_screen_callback(CMnuSmpButton *button)
{
    uiAPI.setContainerVisible(s_campaign_screen, false, 0);
    s_screen_visible = false;
}

// Has same issues as mod menu, it works fine on cold boot, but once in game then back out, causes crash
// likely bad methodology on how we initalize the data

void __thiscall show_custom_campaign_screen(CMnuSmpButton *_this)
{
    CMnuContainer *parent = (CMnuContainer *)_this->CMnuBase_data.param_2_callback;

    if (s_screen_exists)
    {
        s_screen_visible = !s_screen_visible;
        uiAPI.setContainerVisible(s_campaign_screen, s_screen_visible, 0);
        return;
    }

    log_info("Building custom campaign screen (%u campaigns)", g_campaign_count);

    s_campaign_screen = uiAPI.createContainer(0, 0, 1024, 768,
                                              "ui_bgr_landscape_bg.msb", "", 0.99f);
    CMnuContainer *frame = uiAPI.createContainer(11, 6, 1008, 757,
                                                 "ui_bgr_pregame_border_transparency.msb",
                                                 "ui_bgr_pregame_border.msb", 0.5f);
    CMnuContainer *list_panel = uiAPI.createContainer(59, 50, 443, 619,
                                                      "ui_bgr_pregame_border_left_transparency.msb",
                                                      "ui_bgr_pregame_border_left.msb", 0.5f);
    CMnuContainer *detail_panel = uiAPI.createContainer(502, 50, 443, 619,
                                                        "ui_bgr_pregame_border_right_transparency.msb",
                                                        "ui_bgr_pregame_border_right.msb", 0.5f);

    if (!s_campaign_screen || !frame || !list_panel || !detail_panel)
    {
        log_error("Unable to create Campaign Menu containers");
        return;
    }

    uiAPI.containerAddControl(parent, (CMnuBase *)s_campaign_screen, '\x01', '\x01', 0);
    uiAPI.containerAddControl(s_campaign_screen, (CMnuBase *)frame, '\x01', '\x01', 0);
    uiAPI.containerAddControl(frame, (CMnuBase *)list_panel, '\x01', '\x01', 0);
    uiAPI.containerAddControl(frame, (CMnuBase *)detail_panel, '\x01', '\x01', 0);

    char title[32] = "Custom Campaigns";
    CMnuLabel *title_label = uiAPI.attachLabel(NULL, frame, title, 6, 468, 16, 128, 16);
    uiAPI.setMenuID(title_label, 0x6);
    uiAPI.setLabelColour(title_label, 0.85f, 0.64f, 0.12f, '\0');
    uiAPI.setLabelColour(title_label, 0.85f, 0.64f, 0.12f, '\x01');

    char btn_default[32]  = "ui_mainmenu_button_default.msh";
    char btn_pressed[32]  = "ui_mainmenu_button_pressed.msh";
    char btn_disabled[32] = "ui_mainmenu_button_disabled.msh";
    char btn_load[1]      = "";

    const int LIST_X = 100;
    const int LIST_Y_START = 90;
    const int LIST_Y_PITCH = 44;

    for (uint32_t i = 0; i < g_campaign_count; i++)
    {
        s_campaign_buttons[i] = uiAPI.attachNewButton(
            list_panel,
            btn_default, btn_pressed, btn_load, btn_disabled,
            g_campaigns[i].name,
            7,
            LIST_X, LIST_Y_START + (LIST_Y_PITCH * i),
            227, 36,
            32 + i,
            (uint32_t)&on_campaign_selected);
    }

    char back_default[32]  = "ui_btn_nav_back_default.msh";
    char back_pressed[32]  = "ui_btn_nav_back_pressed.msh";
    char back_disabled[32] = "ui_btn_nav_back_disabled.msh";
    char back_load[1]  = "";
    char back_label[1] = "";
    uiAPI.attachNewButton(s_campaign_screen,
                          back_default, back_pressed, back_load, back_disabled,
                          back_label, 7, 52, 678, 48, 48, 31,
                          (uint32_t)&close_campaign_screen_callback);

    s_screen_exists = true;
    s_screen_visible = true;
}

/** @} */
