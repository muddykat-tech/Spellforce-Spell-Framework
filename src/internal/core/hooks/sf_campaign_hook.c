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
#include <sys/stat.h>

static gameInfoSetMapPathFull_ptr s_gameinfo_set_map_path; // deprecated I think
static appMenuEnterCampaignFlow_ptr s_enter_campaign_flow;

static videoSequenceStop_ptr s_video_sequence_stop;

static void hook_getsavepath();

static avatarInternalCopy_ptr s_avatar_internal_copy;
static avatarVectorsCopy_ptr s_avatar_vectors_copy;
static void hook_initfirstmap();

// used in prepare new game
startGame_ptr s_start_game;
playCampaignIntro_ptr s_play_campaign_intro;
getBasePathString_ptr s_get_base_path_string;
gameInfoSetAvatarType_ptr s_gameinfo_set_avatar_type;

typedef LPCSTR *(__cdecl *GetDataStorageLocation_ptr)(char **param_1, uint32_t type);
typedef void (__thiscall *prepareTransition_ptr)(CAppMenu *_this,uint32_t param_1,uint32_t param_2);
typedef SF_GameInfo *(__thiscall *initDefaultInfo_ptr)(SF_GameInfo *_this);
typedef void (__thiscall *AC82_Zero_ptr)(AutoClass82 *_this);
typedef void (__thiscall *some_vector_fun_ptr)(void *_this, uint32_t *size);
typedef uint32_t (__thiscall *CMnuBaseFlagGetter_ptr)(CMnuBase *_this);
typedef void (__thiscall *CUtlCallBackInit_ptr)(void *_this, void *param1, void *param2, uint32_t param3);
typedef void (__thiscall *CUiGameZero_ptr)(void *_this);
typedef void (__thiscall *CUiGame_009a1fd0_ptr)(void *_this, uint32_t param1);
typedef void (__thiscall *load_character_preset_ptr)(CAppMenu *_this, SF_String *name, void *AC11);
typedef SF_String *(__thiscall *getTemplate_ptr)(SF_GameInfo *_this);

static GetDataStorageLocation_ptr s_getDataStorageLocation;
CMnuBaseFlagGetter_ptr CMnuBaseIsVisible;
prepareTransition_ptr prepareTransition;
initDefaultInfo_ptr initDefaultInfo;
AC82_Zero_ptr AC82_Zero;
CUiGameZero_ptr CUiGameZero;
CUiGame_009a1fd0_ptr CUiGame_009a1fd0;
typedef SF_String *(__thiscall *AC95_get_figure_name_ptr)(void *AC95, SF_String *name_buffer, uint32_t figure_id);

extern AC95_get_figure_name_ptr AC95_get_figure_name;
load_character_preset_ptr load_character_preset;

some_vector_fun_ptr some_vector_init; //FUN_007264d0
some_vector_fun_ptr some_vector_dispose; // FUN_006115e0
CUtlCallBackInit_ptr CUtlCallBackInit;
getTemplate_ptr getTemplate;

// Declared in sf_campaign_hook.h so the core modules can validate their assets.
checkFileExists_ptr checkFileExists;

static void hook_qs_load();
static void hook_check_dirs();
static void hook_ql_helper();
static void hook_start_game();

static uint32_t s_ql_return_ok;
static uint32_t s_ql_return_fail;
static uint32_t s_hdr_result_ok;
static uint32_t s_sg_return;
void initialize_campaign_hooks()
{

    s_getDataStorageLocation = (GetDataStorageLocation_ptr)(ASI::AddrOf(0x1ee9f0));
    s_gameinfo_set_map_path = (gameInfoSetMapPathFull_ptr)(ASI::AddrOf(0x1762d0));
    s_enter_campaign_flow   = (appMenuEnterCampaignFlow_ptr)(ASI::AddrOf(0x1936a0));
    s_play_campaign_intro   = (playCampaignIntro_ptr)(ASI::AddrOf(0x181e20));
    s_video_sequence_stop   = (videoSequenceStop_ptr)(ASI::AddrOf(0x3d7b90));
    s_get_base_path_string  = (getBasePathString_ptr)(ASI::AddrOf(0x61a700));

    s_gameinfo_set_avatar_type = (gameInfoSetAvatarType_ptr)(ASI::AddrOf(0x1a1a80));

    s_avatar_internal_copy = (avatarInternalCopy_ptr)(ASI::AddrOf(0x1759e0));
    s_avatar_vectors_copy  = (avatarVectorsCopy_ptr)(ASI::AddrOf(0x175860));

    checkFileExists = (checkFileExists_ptr)(ASI::AddrOf(0x4f7d50));
    prepareTransition = (prepareTransition_ptr)(ASI::AddrOf(0x199db0));
    initDefaultInfo = (initDefaultInfo_ptr)(ASI::AddrOf(0x175760));
    AC82_Zero = (AC82_Zero_ptr)(ASI::AddrOf(0x19e730));
    s_getDataStorageLocation = (GetDataStorageLocation_ptr)(ASI::AddrOf(0x1ee9f0));
    s_start_game = (startGame_ptr)(ASI::AddrOf(0x183560));
    CMnuBaseIsVisible = (CMnuBaseFlagGetter_ptr)(ASI::AddrOf(0x5142c0));
    some_vector_init = (some_vector_fun_ptr)(ASI::AddrOf(0x3264d0));                     //FUN_007264d0
    some_vector_dispose = (some_vector_fun_ptr)(ASI::AddrOf(0x2115e0));

    CUtlCallBackInit = (CUtlCallBackInit_ptr)(ASI::AddrOf(0x5de450));
    CUiGameZero = (CUiGameZero_ptr)(ASI::AddrOf(0x5a1af0));
    CUiGame_009a1fd0 = (CUiGame_009a1fd0_ptr)(ASI::AddrOf(0x5a1fd0));
    load_character_preset = (load_character_preset_ptr)(ASI::AddrOf(0x18fc90));
    getTemplate = (getTemplate_ptr)(ASI::AddrOf(0x176010));

    s_ql_return_ok = ASI::AddrOf(0x5ef663);
    s_ql_return_fail = ASI::AddrOf(0x5ef74c);
    s_hdr_result_ok = ASI::AddrOf(0x5fb864);
    s_sg_return = ASI::AddrOf(0x199655);
    hook_initfirstmap();
    hook_getsavepath();
    hook_qs_load();
    hook_check_dirs();
    hook_ql_helper();
    hook_start_game();

    initialize_preparenewgame_rewrite();

    /* Campaigns themselves are registered by the Custom Campaign Module from
     * sfsf\campaigns\*.json - see initialize_campaign_module(). */
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
    if (g_campaigns[g_campaign_count].avatar_type == -1)
        g_campaigns[g_campaign_count].avatar_type = 8 + g_campaign_count;

    log_info("Registered campaign %u: %s (type %u, map: %s, saves: %s)",
             g_campaign_count, def->name,
             SFSF_CAMPAIGN_TYPE_BASE + g_campaign_count,
             def->start_map, def->campaign_folder);
    return (int32_t)g_campaign_count++;
}

/* Un'Schtalch's code block - updated and reworked a bit for you ~Muddykat*/

int isDirectoryExists(const char *path)
{
    struct stat stats;
    if (stat(path, &stats) == 0)
    {
        return S_ISDIR(stats.st_mode);
    }
    return 0;
}

void checkDirs(CAppSession *_this)
{
    SF_String base_dirs[5];
    char *paths[3];
    SF_String base_path;
    SF_String back_slash;

    uiAPI.SFStringConstructor_char(&back_slash, "\\");
    uiAPI.SFStringConstructor_char(&base_dirs[0], "save");
    uiAPI.SFStringConstructor_char(&base_dirs[1], "temp");
    uiAPI.SFStringConstructor_char(&base_dirs[2], "char");
    uiAPI.SFStringConstructor_char(&base_dirs[3], "campaign2");
    uiAPI.SFStringConstructor_char(&base_dirs[4], "campaign3");

    s_getDataStorageLocation(paths, 0);
    uiAPI.SFStringConstructor_char(&base_path, paths[0]);

    uiAPI.SFStringConcat(&base_path, &base_dirs[0]);
    char *real_path = uiAPI.SFStringCMbStr(&base_path);
    if (!isDirectoryExists(real_path))
    {
        _mkdir(real_path);
    }

    //vanilla campaigns
    for (int i = 1; i < 5; i++)
    {
        SF_String temp;
        uiAPI.SFStringConstructor_char(&temp, real_path);
        uiAPI.SFStringConcat(&temp, &back_slash);
        uiAPI.SFStringConcat(&temp, &base_dirs[i]);
        char *temp_path = uiAPI.SFStringCMbStr(&temp);
        if (!isDirectoryExists(temp_path))
        {
            _mkdir(temp_path);
        }
        uiAPI.SFStringDestructor(&temp);
    }

    for (uint32_t i = 0; i < g_campaign_count; i++)
    {
        const SFSF_CampaignDef *custom = NULL;
        custom = &g_campaigns[i];
        if (custom != NULL)
        {
            SF_String temp;
            SF_String camp_folder;
            uiAPI.SFStringConstructor_char(&temp, real_path);
            uiAPI.SFStringConstructor_char(&camp_folder, custom->campaign_folder);

            uiAPI.SFStringConcat(&temp, &back_slash);
            uiAPI.SFStringConcat(&temp, &camp_folder);
            char *temp_path = uiAPI.SFStringCMbStr(&temp);
            if (!isDirectoryExists(temp_path))
            {
                _mkdir(temp_path);
            }
            uiAPI.SFStringDestructor(&temp);
            uiAPI.SFStringDestructor(&camp_folder);

        }

    }

    for (int i = 0; i < 5; i++)
    {
        uiAPI.SFStringDestructor(&base_dirs[i]);
    }
    uiAPI.SFStringDestructor(&base_path);
    uiAPI.SFStringDestructor(&back_slash);
}


SF_String * __thiscall getSavePath(CAppSession *_this, SF_String *output, uint32_t campaign_type)
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
            //log_info("Loading into getSavePath");
            const SFSF_CampaignDef *custom = NULL;
            int custom_idx = (int)campaign_type - SFSF_CAMPAIGN_TYPE_BASE;
            if (custom_idx >= 0 && custom_idx < (int32_t)g_campaign_count)
            {
                custom = &g_campaigns[custom_idx];
            }

            /* Unknown campaign type: fall back to the base save folder rather
             * than dereferencing a campaign that was never registered. */
            if (custom == NULL)
            {
                log_error("getSavePath: no registered campaign for type %u, using the base save folder",
                          campaign_type);
                break;
            }

            SF_String campaign_path;
            SF_String back_slash;

            uiAPI.SFStringConstructor_char(&campaign_path, custom->campaign_folder);
            uiAPI.SFStringConstructor_char(&back_slash, "\\");
            uiAPI.SFStringConcat(output, &campaign_path);
            uiAPI.SFStringConcat(output, &back_slash);
            uiAPI.SFStringDestructor(&campaign_path);
            uiAPI.SFStringDestructor(&back_slash);
            break;
        }
    }
    return output;
}


uint8_t __thiscall quickLoad_helper(SF_CUiMain *_this)
{
    uint16_t player_id = _this->CUiMain_data.CGdControllerClient->data.current_player;
    if (player_id == 0)
    {
        return 0;
    }

    SF_CGdPlayer *players = _this->CUiMain_data.CGdMain->data.CGdPlayer;
    if (players->players[player_id].use == 0)
    {
        return 0;
    }
    SF_String avatar_name;
    SF_String postfix;
    SF_String base_path;
    uiAPI.SFStringConstructor(&avatar_name);
    AC95_get_figure_name(_this->CUiMain_data.AC95,&avatar_name, players->players[player_id].avatar_figure_index);
    uiAPI.SFStringConstructor_char(&postfix, "~QUICKSAVE.sav");
    uiAPI.SFStringConcat(&avatar_name, &postfix);
    //Not a bug. First parameter is unused inside, since it's static member function.
    getSavePath((void *)_this, &base_path, _this->CUiMain_data.campaign_type);
    uiAPI.SFStringConcat(&base_path,&avatar_name);
    uint32_t shallContinue = false;
    if (checkFileExists(&base_path))
    {
        if ((CMnuBaseIsVisible((CMnuBase *)_this->CUiMain_data.CUiGame) == 1) &&
            (CMnuBaseIsVisible(*(CMnuBase **)&_this->CUiMain_data.unkn7[0x1128]) == 0))
        {
            shallContinue = true;
        }
    }

    uiAPI.SFStringDestructor(&avatar_name);
    uiAPI.SFStringDestructor(&postfix);
    uiAPI.SFStringDestructor(&base_path);

    if (!shallContinue)
    {
        return 0;
    }

    return 1;
}

//We have inlined and optimized getSavePath over there, so let's just plug-in

static void __declspec(naked) quickload_hook()
{
    asm ("mov %%esi, %%ecx   \n\t"  // Getting CUIMain
         "call %P0           \n\t"  // Calling the Hook Function
         "test %%eax, %%eax  \n\t"  // checking what have we returned
         "jne 1f             \n\t"
         "jmp *%2            \n\t"
         "1: jmp *%1         \n\t" : : "i" (quickLoad_helper),
         "o" (s_ql_return_ok),"o" (s_ql_return_fail) );
}

uint32_t __thiscall getHdrStringID(uint32_t camp_type)
{
    if (camp_type == 2)
    {
        return 0x1cb8;
    }
    if (camp_type == 1)
    {
        return 0x1bbf;
    }
    if (camp_type == 0)
    {
        return 0x1979;
    }

    int custom_idx = (int)camp_type - SFSF_CAMPAIGN_TYPE_BASE;
    if (g_campaigns[custom_idx].campaign_name_id)
    {
        return g_campaigns[custom_idx].campaign_name_id;
    }
    else
    {
        return 0x1979;
    }
}

static void __declspec(naked) hdr_helper()
{
    asm ("mov 0x3c4(%%ebx), %%eax   \n\t"   // Getting campaign type
         "mov %%eax, %%ecx          \n\t"
         "call %P0                  \n\t"  // Calling the Hook Function
         "mov %%eax, %%ecx          \n\t"  // saving the result
         "lea -0x330(%%ebp), %%eax  \n\t"
         "push %%eax                \n\t"
         "push %%ecx                \n\t"
         "jmp *%1         \n\t" : : "i" (getHdrStringID),
         "o" (s_hdr_result_ok) );
}

void hook_ql_helper()
{
    ASI::MemoryRegion mreg_qs(ASI::AddrOf(0x5ef43a), 6);
    ASI::BeginRewrite(mreg_qs);
    *(unsigned char *)(ASI::AddrOf(0x5ef43a)) = 0x90; // NOP
    *(unsigned char *)(ASI::AddrOf(0x5ef43b)) = 0xE9; // JMP instruction
    *(int *)(ASI::AddrOf(0x5ef43c)) = (int)(&quickload_hook) - ASI::AddrOf(0x5ef440);
    ASI::EndRewrite(mreg_qs);

    ASI::MemoryRegion mreg_hdr(ASI::AddrOf(0x5fb827), 6);
    ASI::BeginRewrite(mreg_hdr);
    *(unsigned char *)(ASI::AddrOf(0x5fb827)) = 0x90;  // NOP
    *(unsigned char *)(ASI::AddrOf(0x5fb828)) = 0xE9;  // JMP instruction
    *(int *)(ASI::AddrOf(0x5fb829)) = (int)(&hdr_helper) - ASI::AddrOf(0x5fb82d);
    ASI::EndRewrite(mreg_hdr);

}


void __thiscall loadQuickSave(CAppMenu *_this, uint32_t unknown)
{
    CAppSession *session = _this->CAppMenu_data.CAppSession;
    SF_CGdMain *main = session->data.CGdMain;
    CGdControllerClient *client = session->data.controllerClient;
    SF_CGdPlayer *player = main->data.CGdPlayer;
    uint16_t player_id = client->data.current_player;

    bool inUse = (player->players[player_id].use != 0);
    uint16_t figure_id = player->players[player_id].avatar_figure_index;

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
    getSavePath(session, &base_path, _this->CAppMenu_data.campaign_type);
    //log_info("Base path %ls", base_path.raw_data);

    uiAPI.SFStringConcat(&base_path, &avatar_name);
    if (checkFileExists(&base_path))
    {
        //log_info("Full save path %ls", base_path.raw_data);

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

void hook_qs_load()
{
    ASI::MemoryRegion mreg_qs(ASI::AddrOf(0x185c00), 5);
    ASI::BeginRewrite(mreg_qs);
    *(unsigned char *)(ASI::AddrOf(0x185c00)) = 0xE9; // JMP instruction
    *(int *)(ASI::AddrOf(0x185c01)) = (int)(&loadQuickSave) - ASI::AddrOf(0x185c05);
    ASI::EndRewrite(mreg_qs);
}

static void hook_check_dirs()
{
    ASI::MemoryRegion mreg(ASI::AddrOf(0x1b6fa0), 5);
    ASI::BeginRewrite(mreg);
    *(unsigned char *)(ASI::AddrOf(0x1b6fa0)) = 0xE9; // JMP instruction
    *(int *)(ASI::AddrOf(0x1b6fa1)) = (int)(&checkDirs) - ASI::AddrOf(0x1b6fa5);
    ASI::EndRewrite(mreg);
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
    //log_info("initFirstMap Hook: Starter Kit Start ");

    //log_info("Values: %x %d %d %d %d %d", (uint32_t)_this, skip_tutorial, skill, subskill, campaign_id, is_shadowblade);

    //log_info("initFirstMap Hook: Check for Custom Campaign");
    /* -- active custom campaign? Resolved up front: the starter kit below
     *    depends on it. -- */
    const SFSF_CampaignDef *custom = NULL;
    int custom_idx = (int)campaign_id - SFSF_CAMPAIGN_TYPE_BASE;
    if (custom_idx >= 0 && custom_idx < (int32_t)g_campaign_count)
    {
        custom = &g_campaigns[custom_idx];
    }

    uiAPI.SFStringConstructor_char(&dot_map, ".map");
    uiAPI.SFStringConstructor(&full_template);

    uiAPI.SFStringConstructor(&default_template);
    uiAPI.SFprintf(&default_template, L"SK_%02d%02d.des", skill, subskill);

    uiAPI.SFStringConstructor_char(&template_path, "figure_template\\starterkit\\");
    uiAPI.SFStringConcat(&full_template, &template_path);
    uiAPI.SFStringConcat(&full_template, &default_template);
    uiAPI.SFStringDeepCopy(&_this->starter_kit_name, &full_template);

    //log_info("initFirstMap Hook: Avatar Snapshot to deep Copy");
    /* -- avatar snapshot AC82 -> AC82_1: FULL deep copy (vanilla parity).
     *    Downstream flows (restart map, PrepareNewGame) read AC82_1;
     *    the shallow field copies alone are NOT sufficient. -- */
    _this->AC82_1.unknown1  = _this->AC82.unknown1;
    _this->AC82_1.unknown2  = _this->AC82.unknown2;
    _this->AC82_1.unknown3  = _this->AC82.unknown3;
    _this->AC82_1.kit_index = _this->AC82.kit_index;
    s_avatar_internal_copy(&_this->AC82_1.avatarData.internal, &_this->AC82.avatarData.internal);
    uint32_t proper_offset = (uint32_t)(&_this->AC82.avatarData.begin) - 0x10;
    s_avatar_vectors_copy(&_this->AC82_1.avatarData.begin, proper_offset);

    /* -- folder + start map: selected FIRST, tutorial branch only overrides
     *    the map NAME (vanilla order - vanilla tutorials live in their own
     *    campaign's folder, e.g. map\Campaign2\tutorial.map). -- */
    if (custom != NULL)
    {
        //log_info("initFirstMap Hook: Attempting to load Custom Campaign");
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
        //log_info("Campaign '%s': maps from %s", custom->name, folder);
    }
    else
    {
        //log_info("initFirstMap Hook: Not Custom - Deverting to Vanilla Flow");
        switch (campaign_id)
        {
            case 1:
                //log_info("initFirstMap Hook: Engine Type 1");
                uiAPI.SFStringConstructor_char(&campagn_path, "map\\Campaign2\\");
                uiAPI.SFStringConstructor_wchar(&intial_map_name, L"P101_Mirraw_Thur");
                break;
            case 2:
                //log_info("initFirstMap Hook: Engine Type 2");
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
                //log_info("initFirstMap Hook: Campaign Type != (1||2)");
                /* default falls back to Order so the strings are ALWAYS
                 * constructed - the epilogue destructors depend on it.
                 *
                 * A campaign_id above 2 that reached this branch means the
                 * lookup above found no registered campaign for it, so there is
                 * no definition to read - fall through to Order rather than
                 * dereferencing a NULL custom. */
                if (campaign_id > 2)
                {
                    log_error("initFirstMap: campaign type %u has no registered campaign, starting Order instead",
                              campaign_id);
                }

                uiAPI.SFStringConstructor_char(&campagn_path, "map\\Campaign\\");
                uiAPI.SFStringConstructor_wchar(&intial_map_name, L"000_Greyfell");
                break;
        }
    }

    //log_info("initFirstMap Hook: Check Tutorial");
    if (skip_tutorial)
    {
        //log_info("initFirstMap Hook: Skipping Tutorial");
        /* Direct start: Assign player their skill-derived starter kit (armor equipment ect). */
        uiAPI.SFStringDeepCopy(&_this->template_name, &_this->starter_kit_name);
        //log_info("initFirstMap Hook; setting GameInfo");
        _this->start_mode  = 2;
        _this->is_tutorial = 0;
    }
    else
    {
        //log_info("initFirstMap Hook: Loading Tutorial");
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

        //log_info("initFirstMap Hook: Tutorial Kit Load");
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

        //log_info("initFirstMap Hook: Updating GameInfo");
        _this->start_mode  = 1;
        _this->is_tutorial = 1;
    }

    //log_info("initFirstMap Hook: prepping map file for loading");
    /* -- filename = <folder><map>.map + branch-invariant state -- */
    uiAPI.SFStringConcat(&campagn_path, &intial_map_name);
    uiAPI.SFStringConcat(&campagn_path, &dot_map);
    uiAPI.SFStringDeepCopy(&_this->filename, &campagn_path);
    _this->unknown_0xf0 = 1;
    _this->unknown_0xf4 = 3;

    //log_info("initFirstMap Hook; Cleanup");
    uiAPI.SFStringDestructor(&default_template);
    uiAPI.SFStringDestructor(&template_path);
    uiAPI.SFStringDestructor(&full_template);
    uiAPI.SFStringDestructor(&dot_map);
    uiAPI.SFStringDestructor(&campagn_path);
    uiAPI.SFStringDestructor(&intial_map_name);
    //log_info("initFirstMap Hook; Complete Returning");
}

void hook_initfirstmap()
{
    ASI::MemoryRegion mreg(ASI::AddrOf(0x176040), 5);
    ASI::BeginRewrite(mreg);
    *(unsigned char *)(ASI::AddrOf(0x176040)) = 0xE9;
    *(int *)(ASI::AddrOf(0x176041)) = (int)(&initFirstMap) - (int)(ASI::AddrOf(0x176045));
    ASI::EndRewrite(mreg);
}

/* Detail panel geometry, relative to the 443x619 right hand container. */
#define CAMPAIGN_PANEL_WIDTH 443
#define CAMPAIGN_NAME_Y      32
#define CAMPAIGN_NAME_H      48
#define CAMPAIGN_AUTHOR_Y    88
#define CAMPAIGN_AUTHOR_H    28
#define CAMPAIGN_DESC_Y      140
#define CAMPAIGN_DESC_H      360
#define CAMPAIGN_PLAY_X      108
#define CAMPAIGN_PLAY_Y      540
#define CAMPAIGN_PLAY_W      227
#define CAMPAIGN_PLAY_H      36

/** Character width the description wraps at, matching the mod info panel. */
#define CAMPAIGN_DESC_WRAP 40

/* One row per campaign; the registry caps at SFSF_MAX_CAMPAIGNS so they all fit
 * in the left panel without paging. */
#define CAMPAIGN_ROW_X       108
#define CAMPAIGN_ROW_Y_START 40
#define CAMPAIGN_ROW_PITCH   36
#define CAMPAIGN_ROW_W       227
#define CAMPAIGN_ROW_H       30

static bool s_screen_exists = false;
static bool s_screen_visible = false;
static CMnuContainer *s_campaign_screen = NULL;
static CMnuSmpButton *s_campaign_buttons[SFSF_MAX_CAMPAIGNS];
static CMnuLabel *s_campaign_name_label = NULL;
static CMnuLabel *s_campaign_author_label = NULL;
static CMnuLabel *s_campaign_desc_label = NULL;

/** Campaign shown in the detail panel, -1 before anything is picked. */
static int32_t s_selected_campaign = -1;

void campaign_hook_on_main_menu(CAppMenu *app_menu)
{
    g_campaign_app_menu = app_menu;
    g_active_custom_campaign = -1;
    s_screen_exists = false;
    s_screen_visible = false;
    s_campaign_screen = NULL;
    s_campaign_name_label = NULL;
    s_campaign_author_label = NULL;
    s_campaign_desc_label = NULL;
    s_selected_campaign = -1;
    memset(s_campaign_buttons, 0, sizeof(s_campaign_buttons));
}

static void hook_getsavepath()
{
    ASI::MemoryRegion mreg(ASI::AddrOf(0x1b89d0), 5);
    ASI::BeginRewrite(mreg);
    *(unsigned char *)(ASI::AddrOf(0x1b89d0)) = 0xE9; /* JMP */
    *(int *)(ASI::AddrOf(0x1b89d1)) = (int)(&getSavePath) - ASI::AddrOf(0x1b89d5);
    ASI::EndRewrite(mreg);
}

void stop_intro_video(CAppMenu *app_menu)
{
    SF_CUiVideoSequence *seq =
        (SF_CUiVideoSequence *)app_menu->CAppMenu_data.CUiVideoSequence_ptr;
    if (seq != NULL)
    {
        s_video_sequence_stop(seq);
        fidFree((uint32_t *)seq);
        app_menu->CAppMenu_data.CUiVideoSequence_ptr = NULL;
    }
}

void startGame_helper(CAppMenu *_this, SF_GameInfo *game_info)
{
    SF_String template_name;
    SF_String template_path;
    uint32_t unused;
    SF_String *starter_kit = getTemplate(&_this->CAppMenu_data.game_info);
    log_info("Starter kit %ls", starter_kit->raw_data);
    uiAPI.SFStringConstructor_char(&template_path, "figure_template\\starterkit\\");

    if (!game_info->is_coop)
    {
        uint32_t campaign_type = _this->CAppMenu_data.campaign_type;
        switch (campaign_type)
        {
            case 0:
            {
                uiAPI.SFStringConstructor_char(&template_name, "figure_template\\starterkit\\SK_ALL.des");
                load_character_preset(_this, &template_name, &unused);
                uiAPI.SFStringDestructor(&template_name);
                break;
            }
            case 1:
            {
                uiAPI.SFStringConstructor_char(&template_name,
                                               "figure_template\\starterkit\\SK_all_addoncampaign.des");
                load_character_preset(_this, &template_name, &unused);
                uiAPI.SFStringDestructor(&template_name);
                break;
            }
            case 2:
            {
                /* don't load general preset, it is handled in predefined files*/
                break;
            }
            default:
            {
                const SFSF_CampaignDef *custom = NULL;
                int custom_idx = (int)campaign_type - SFSF_CAMPAIGN_TYPE_BASE;
                if (custom_idx >= 0 && custom_idx < (int32_t)g_campaign_count)
                {
                    custom = &g_campaigns[custom_idx];
                }
                SF_String template_name;
                /* Unknown campaign type: fall back to the base save folder rather
                 * than dereferencing a campaign that was never registered. */
                if (custom == NULL)
                {
                    log_error("Start Game: no registered campaign for type %u, using default kits",
                              campaign_type);

                    uiAPI.SFStringConstructor_char(&template_name, "SK_ALL.des");
                    load_character_preset(_this, &template_name, &unused);
                    uiAPI.SFStringDestructor(&template_name);
                    break;
                }
                uiAPI.SFStringConstructor_char(&template_name, custom->starterkit);
                uiAPI.SFStringConcat(&template_path, &template_name);
                uiAPI.SFStringDestructor(&template_name);

                if (!checkFileExists(&template_path))
                {
                    log_error("Start Game: starter kit not found for camapign %u, using default kits",
                              campaign_type);
                    uiAPI.SFStringDestructor(&template_path);
                    uiAPI.SFStringConstructor_char(&template_name, "SK_ALL.des");
                    uiAPI.SFStringConstructor_char(&template_path, "figure_template\\starterkit\\");
                    uiAPI.SFStringConcat(&template_path, &template_name);
                }
                load_character_preset(_this, &template_path, &unused);
                uiAPI.SFStringDestructor(&template_path);
                break;
            }
        }
    }
    else
    {
        uiAPI.SFStringConstructor_char(&template_name, "SK_all_coop.des");
        uiAPI.SFStringConcat(&template_path, &template_name);
        uiAPI.SFStringDestructor(&template_name);
        load_character_preset(_this, &template_path, &unused);
        uiAPI.SFStringDestructor(&template_path);
    }

    load_character_preset(_this, starter_kit, &unused);

}

static void __declspec(naked) start_game_hook()
{
    asm (
        "push %%ebx         \n\t"  // Getting GameInfo
        "push %%esi         \n\t"  // Getting CAppMenu
        "call %P0           \n\t"  // Calling the Hook Function
        "pop %%esi          \n\t" // restoring data
        "pop %%ebx          \n\t"
        "1: jmp *%1         \n\t" : : "i" (startGame_helper),
        "o" (s_sg_return));
}

static void hook_start_game()
{
    ASI::MemoryRegion mreg(ASI::AddrOf(0x199525), 5);
    ASI::BeginRewrite(mreg);
    *(unsigned char *)(ASI::AddrOf(0x199525)) = 0x90; /* NOP */
    *(unsigned char *)(ASI::AddrOf(0x199526)) = 0x90; /* NOP */
    *(unsigned char *)(ASI::AddrOf(0x199527)) = 0xE9; /* JMP */
    *(int *)(ASI::AddrOf(0x199528)) = (int)(&start_game_hook) - ASI::AddrOf(0x19952c);
    ASI::EndRewrite(mreg);
}

void campaign_launch_flow(int32_t campaign_index)
{
    CAppMenu *app_menu = g_campaign_app_menu;
    const SFSF_CampaignDef *def = &g_campaigns[campaign_index];

    if (app_menu == NULL || s_gameinfo_set_avatar_type == NULL)
    {
        log_error("Campaign launch unavailable: missing CAppMenu or engine addresses");
        return;
    }

    stop_intro_video(app_menu);
    g_active_custom_campaign = campaign_index;
    app_menu->CAppMenu_data.game_info.is_coop = 0;
    app_menu->CAppMenu_data.campaign_type = SFSF_CAMPAIGN_TYPE_BASE + campaign_index;
    s_gameinfo_set_avatar_type(&app_menu->CAppMenu_data.game_info, (uint16_t)def->avatar_type);
    s_enter_campaign_flow(app_menu, 1);
}

/**
 * @brief Renders one campaign into the right hand detail panel.
 */
static void show_campaign_details(int32_t campaign_index)
{
    if (campaign_index < 0 || campaign_index >= (int32_t)g_campaign_count)
    {
        return;
    }

    s_selected_campaign = campaign_index;
    const SFSF_CampaignDef *def = &g_campaigns[campaign_index];

    char author[128];
    snprintf(author, sizeof(author), "by %s",
             (def->author[0] != '\0') ? def->author : "Unknown");

    char wrapped_description[1024] = {0};
    wrap_text(def->description, wrapped_description, CAMPAIGN_DESC_WRAP);

    set_centred_label_text(s_campaign_name_label, def->name,
                           CAMPAIGN_PANEL_WIDTH, CAMPAIGN_NAME_Y, CAMPAIGN_NAME_H);
    set_centred_label_text(s_campaign_author_label, author,
                           CAMPAIGN_PANEL_WIDTH, CAMPAIGN_AUTHOR_Y, CAMPAIGN_AUTHOR_H);
    set_centred_label_text(s_campaign_desc_label, wrapped_description,
                           CAMPAIGN_PANEL_WIDTH, CAMPAIGN_DESC_Y, CAMPAIGN_DESC_H);
}

/** @brief Left panel row callback */
void __thiscall on_campaign_selected(CMnuSmpButton *_this)
{
    for (uint32_t i = 0; i < g_campaign_count; i++)
    {
        if (s_campaign_buttons[i] == _this)
        {
            show_campaign_details((int32_t)i);
            return;
        }
    }
    log_error("Campaign button not found in registry");
}

/** @brief Play button callback - launches whatever the list has selected. */
void __thiscall on_campaign_play(CMnuSmpButton *_this)
{
    if (s_selected_campaign < 0 || s_selected_campaign >= (int32_t)g_campaign_count)
    {
        log_error("Play pressed with no campaign selected");
        return;
    }

    campaign_launch_flow(s_selected_campaign);
}

void close_campaign_screen_callback(CMnuSmpButton *button)
{
    if (s_campaign_screen != NULL)
    {
        uiAPI.setContainerVisible(s_campaign_screen, false, 0);
    }
    s_screen_visible = false;
}

/**
 * @brief Builds the campaign screen
 */
void __thiscall show_custom_campaign_screen(CMnuSmpButton *_this)
{
    CMnuContainer *parent = (CMnuContainer *)_this->CMnuBase_data.param_2_callback;

    if (s_screen_exists && s_campaign_screen != NULL)
    {
        s_screen_visible = !s_screen_visible;
        uiAPI.setContainerVisible(s_campaign_screen, s_screen_visible, 0);
        return;
    }

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

        if (s_campaign_screen)
        {
            uiAPI.destroyContainer(s_campaign_screen);
        }
        if (frame)
        {
            uiAPI.destroyContainer(frame);
        }
        if (list_panel)
        {
            uiAPI.destroyContainer(list_panel);
        }
        if (detail_panel)
        {
            uiAPI.destroyContainer(detail_panel);
        }

        s_campaign_screen = NULL;
        s_screen_exists = false;
        s_screen_visible = false;
        return;
    }

    uiAPI.containerAddControl(parent, (CMnuBase *)s_campaign_screen, '\x01', '\x01', 0);
    uiAPI.containerAddControl(s_campaign_screen, (CMnuBase *)frame, '\x01', '\x01', 0);
    uiAPI.containerAddControl(frame, (CMnuBase *)list_panel, '\x01', '\x01', 0);
    uiAPI.containerAddControl(frame, (CMnuBase *)detail_panel, '\x01', '\x01', 0);

    char title[32] = "Custom Campaigns";
    CMnuLabel *title_label = uiAPI.attachLabel(NULL, frame, title, 6, 468, 16, 128, 16);
    uiAPI.setMenuID(title_label, 0x6);
    apply_label_colour(title_label, 0.85f, 0.64f, 0.12f);

    char btn_default[32]  = "ui_mainmenu_button_default.msh";
    char btn_pressed[32]  = "ui_mainmenu_button_pressed.msh";
    char btn_disabled[32] = "ui_mainmenu_button_disabled.msh";
    char btn_load[1]      = "";

    for (uint32_t i = 0; i < g_campaign_count; i++)
    {
        s_campaign_buttons[i] = uiAPI.attachNewButton(
            list_panel,
            btn_default, btn_pressed, btn_load, btn_disabled,
            g_campaigns[i].name,
            7,
            CAMPAIGN_ROW_X, CAMPAIGN_ROW_Y_START + (CAMPAIGN_ROW_PITCH * i),
            CAMPAIGN_ROW_W, CAMPAIGN_ROW_H,
            32 + i,
            (uint32_t)&on_campaign_selected);
    }

    char placeholder[2] = " ";

    s_campaign_name_label = uiAPI.attachLabel(NULL, detail_panel, placeholder, 6,
                                              0, CAMPAIGN_NAME_Y,
                                              CAMPAIGN_PANEL_WIDTH, CAMPAIGN_NAME_H);
    uiAPI.setMenuID(s_campaign_name_label, 0x6);
    apply_label_colour(s_campaign_name_label, 0.85f, 0.64f, 0.12f);

    s_campaign_author_label = uiAPI.attachLabel(NULL, detail_panel, placeholder, 6,
                                                0, CAMPAIGN_AUTHOR_Y,
                                                CAMPAIGN_PANEL_WIDTH, CAMPAIGN_AUTHOR_H);
    uiAPI.setMenuID(s_campaign_author_label, 0x6);
    apply_label_colour(s_campaign_author_label, 0.45f, 0.72f, 0.95f);

    s_campaign_desc_label = uiAPI.attachLabel(NULL, detail_panel, placeholder, 11,
                                              0, CAMPAIGN_DESC_Y,
                                              CAMPAIGN_PANEL_WIDTH, CAMPAIGN_DESC_H);
    uiAPI.setMenuID(s_campaign_desc_label, 0x6);

    char play_label[8] = "PLAY";
    uiAPI.attachNewButton(detail_panel,
                          btn_default, btn_pressed, btn_load, btn_disabled,
                          play_label, 7,
                          CAMPAIGN_PLAY_X, CAMPAIGN_PLAY_Y,
                          CAMPAIGN_PLAY_W, CAMPAIGN_PLAY_H,
                          30,
                          (uint32_t)&on_campaign_play);

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

    show_campaign_details(0);
}

/** @} */
