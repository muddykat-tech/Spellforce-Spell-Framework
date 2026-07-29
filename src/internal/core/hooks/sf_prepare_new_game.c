/**
 * @addtogroup CampaignHook
 * @{
 *
 * FULL reimplementation of CAppMenu::PrepareNewGame - PROTOTYPE.
 */

#include "../sf_wrappers.h"
#include "../sf_ui_wrappers.h"
#include "sf_campaign_hook.h"
#include "sf_prepare_new_game.h"
#include "../sf_hooks.h"
#include "sf_vanilla_fix_hook.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

static preloadGetResultCode_ptr pn_get_result_code;
static gameInfoReset_ptr pn_gi_reset_avatar;
static preloadGetAvatar_ptr pn_preload_get_avatar;
static gameInfoSetAvatar_ptr pn_gi_set_avatar_equipdata;
static gameInfoGetU8_ptr pn_gi_get_skill, pn_gi_get_subskill, pn_gi_get_kit_index;

static preloadGetU8_ptr pn_preload_get_kit_index;
static preloadGetU32_ptr pn_preload_get_sotp_side, pn_preload_get_campaign_type;


static giSetBoolU8_ptr pn_gi_set_premade_kit, pn_gi_apply_premade_kit;
static giSetU32_ptr pn_gi_set_start_mode2, pn_gi_set_game_mode,
                    pn_gi_set_flag_a, pn_gi_set_flag_b;
static giSetStr_ptr pn_gi_set_save_file_path, pn_gi_set_save_file_name;
static giSetTemplateName_ptr pn_gi_set_template_name;
static giVoid_ptr pn_gi_refresh_case7;
static giStarterKitReset_ptr pn_gi_starterkit_reset;
static preloadGetStr_ptr pn_preload_get_slot_name, pn_preload_get_avatar_name,
                         pn_preload_get_freegame_map, pn_preload_get_freegame_template;
static appMenuDeleteSave_ptr pn_delete_save;
static getPregameScreen_ptr pn_get_pregame_screen;
static getScreenName_ptr pn_screen_get_name;
static screenDeleteControl_ptr pn_screen_delete_control;
static screenSetActive_ptr pn_screen_set_active;
static cfgCtor_ptr pn_cfg_ctor;
static cfgDtor_ptr pn_cfg_dtor;
static cfgSetString_ptr pn_cfg_set_string;
static preloadUpdateKit_ptr pn_update_kit;
static preloadUpdateKit2_ptr pn_update_kit_2;

/* last-played globals best guesses */
static SF_String *pn_s_cfg_key;          /* DAT_00d24120 @ 0xd24120 (section?)   */
static SF_String *pn_s_last_sf1;         /* DAT_00d24320 @ 0xd24320              */
static SF_String *pn_s_last_addon;       /* S_LAST_PLAYED_ADDON @ 0xd243d0       */
static SF_String *pn_s_last_sotp;        /* DAT_00d24480 @ 0xd24480              */
static SF_String *pn_s_last_coop;        /* DAT_00d23c90 @ 0xd23c90              */
CreateMnuHintExt_ptr CreateMnuHintExt;

static void install_preparenewgame_hook();

void initialize_preparenewgame_rewrite()
{
    pn_get_result_code       = (preloadGetResultCode_ptr)(ASI::AddrOf(0x5fad30));

    pn_gi_reset_avatar              = (gameInfoReset_ptr)(ASI::AddrOf(0x1a1830));
    pn_preload_get_avatar    = (preloadGetAvatar_ptr)(ASI::AddrOf(0x5fae00));
    pn_gi_set_avatar_equipdata         = (gameInfoSetAvatar_ptr)(ASI::AddrOf(0x1a1440));
    pn_gi_get_skill          = (gameInfoGetU8_ptr)(ASI::AddrOf(0x19e7a0));
    pn_gi_get_subskill       = (gameInfoGetU8_ptr)(ASI::AddrOf(0x19e7c0));
    pn_gi_get_kit_index      = (gameInfoGetU8_ptr)(ASI::AddrOf(0x175f00));
    pn_preload_get_sotp_side = (preloadGetU32_ptr)(ASI::AddrOf(0x60a3d0));
    pn_preload_get_campaign_type  = (preloadGetU32_ptr)(ASI::AddrOf(0x5faa10));

    pn_preload_get_kit_index      = (preloadGetU8_ptr)(ASI::AddrOf(0x5fae20));

    pn_gi_set_premade_kit    = (giSetBoolU8_ptr)(ASI::AddrOf(0x176760));
    pn_gi_apply_premade_kit  = (giSetBoolU8_ptr)(ASI::AddrOf(0x1a1b10));
    pn_gi_set_start_mode2    = (giSetU32_ptr)(ASI::AddrOf(0x176780));
    pn_gi_set_game_mode      = (giSetU32_ptr)(ASI::AddrOf(0x1766f0));
    pn_gi_set_flag_a         = (giSetU32_ptr)(ASI::AddrOf(0x176740));
    pn_gi_set_flag_b         = (giSetU32_ptr)(ASI::AddrOf(0x176750));
    pn_gi_set_save_file_path = (giSetStr_ptr)(ASI::AddrOf(0x176320));
    pn_gi_set_save_file_name = (giSetStr_ptr)(ASI::AddrOf(0x176700));
    pn_gi_set_template_name  = (giSetTemplateName_ptr)(ASI::AddrOf(0x176790));
    pn_gi_refresh_case7      = (giVoid_ptr)(ASI::AddrOf(0x176480));
    pn_gi_starterkit_reset   = (giStarterKitReset_ptr)(ASI::AddrOf(0x1763c0));
    pn_preload_get_slot_name         = (preloadGetStr_ptr)(ASI::AddrOf(0x609520));
    pn_preload_get_avatar_name       = (preloadGetStr_ptr)(ASI::AddrOf(0x609600));
    pn_preload_get_freegame_map      = (preloadGetStr_ptr)(ASI::AddrOf(0x60a3e0));
    pn_preload_get_freegame_template = (preloadGetStr_ptr)(ASI::AddrOf(0x60a410));
    pn_delete_save           = (appMenuDeleteSave_ptr)(ASI::AddrOf(0x186520));
    pn_get_pregame_screen       = (getPregameScreen_ptr)(ASI::AddrOf(0x50f930));
    pn_screen_get_name          = (getScreenName_ptr)(ASI::AddrOf(0x510cf0));
    pn_screen_delete_control = (screenDeleteControl_ptr)(ASI::AddrOf(0x508560));
    pn_screen_set_active       = (screenSetActive_ptr)(ASI::AddrOf(0x512180));
    pn_cfg_ctor              = (cfgCtor_ptr)(ASI::AddrOf(0x389050));
    pn_cfg_dtor              = (cfgDtor_ptr)(ASI::AddrOf(0x387e20));
    pn_cfg_set_string        = (cfgSetString_ptr)(ASI::AddrOf(0x3897b0));
    pn_update_kit            = (preloadUpdateKit_ptr)(ASI::AddrOf(0x176760));
    pn_update_kit_2          = (preloadUpdateKit2_ptr)(ASI::AddrOf(0x1a1b10));
    CreateMnuHintExt         = (CreateMnuHintExt_ptr)(ASI::AddrOf(0x18eca0));

    pn_s_cfg_key    = (SF_String *)(ASI::AddrOf(0x924120));
    pn_s_last_sf1   = (SF_String *)(ASI::AddrOf(0x924320));
    pn_s_last_addon = (SF_String *)(ASI::AddrOf(0x9243d0));
    pn_s_last_sotp  = (SF_String *)(ASI::AddrOf(0x924480));
    pn_s_last_coop  = (SF_String *)(ASI::AddrOf(0x923c90));

    install_preparenewgame_hook();
}

uint8_t __thiscall pn_preload_get_skip_tutorial(CUiMenuPreLoad *_this, SF_GameInfo* game_info)
{
    log_info("check coop");
    uint8_t is_coop = (_this->CUiMenuPreLoad_data.game_info)->is_coop;
    log_info("got it?");
    if(is_coop == 0 && (_this->CUiMenuPreLoad_data).campaign_type == 0)
    {
        log_info("return skip tut in preload data");
        return (_this->CUiMenuPreLoad_data).skip_tutorial;
    }
    log_info("return 1");
    return 1;
}

void write_last_played(CAppMenu *_this, CUiMenuPreLoad *preload,  CUtlConfigFile* cfg, bool is_custom)
{
    //if (is_custom) { return; }
    log_info("Starting Write Chain");
    SF_String name;
    uiAPI.SFStringConstructor(&name);
    SF_String *name_ptr = pn_preload_get_avatar_name(preload, &name);
    if (name_ptr->str_length == 0) { uiAPI.SFStringDestructor(name_ptr); return; }

    log_info("Getting Config Data");
    SF_String *key_str;
    int ct = _this->CAppMenu_data.campaign_type;
    if (ct == 1)      key_str = pn_s_last_addon;
    else if (ct == 2) key_str = pn_s_last_sotp;
    else key_str = (_this->CAppMenu_data.game_info.is_coop != 0) ? pn_s_last_coop
                                                                 : pn_s_last_sf1;
    SF_String empty;
    uiAPI.SFStringConstructor(&empty);

    log_info("Set String - Fails missing control or some such from what I can trace");
    pn_cfg_set_string(cfg, uiAPI.SFStringCMbStr(pn_s_cfg_key), uiAPI.SFStringCMbStr(key_str), &empty, name_ptr);

    log_info("Cleaning up");
    uiAPI.SFStringDestructor(&cfg->name_maybe);
    pn_cfg_dtor(cfg);
    uiAPI.SFStringDestructor(&empty);
    uiAPI.SFStringDestructor(name_ptr);
    log_info("Returning");
}

void __thiscall hooked_prepare_new_game(CAppMenu *_this, CUiMenuPreLoad *preload)
{
    log_info("Preparing New Game");
    CUtlConfigFile configFile;

    log_info("Check 1 loc: %x vs %x", (uint32_t)(_this->CAppMenu_data.CUiMenuPreload), (uint32_t)(preload));
    log_info("Check 2 loc: %x vs %x", (uint32_t)(&_this->CAppMenu_data.game_info), (uint32_t)(preload->CUiMenuPreLoad_data.game_info));
    pn_cfg_ctor(&configFile, (char*)0x0);
    log_info("Check 2");
    uint32_t result_code = pn_get_result_code(preload);
    SF_GameInfo *game_info = &(_this->CAppMenu_data).game_info;
    (_this->CAppMenu_data).pregame_load_result = result_code;
    GdAvatarData *avatar_data = &(game_info->AC82).avatarData;
    GdAvatarInternal *internal_avatar = &avatar_data->internal;
    uint16_t avatar_type = internal_avatar->avatar_type;
    log_info("Check 2");
    pn_gi_reset_avatar(game_info, 0); // might be internal_avatar init
    avatar_data = pn_preload_get_avatar(preload, avatar_data);
    log_info("Check 3");
    //uint16_t avatar_equipment_data_maybe =;
    avatar_data = pn_preload_get_avatar(preload, avatar_data);
    log_info("Check 4");
    pn_gi_set_avatar_equipdata(game_info, internal_avatar,  avatar_data->begin);
    s_gameinfo_set_avatar_type(game_info, avatar_type);

    log_info("Check 5");
    uint8_t skill_id = pn_gi_get_skill(game_info);
    uint8_t subskill_spec = pn_gi_get_subskill(game_info);

    SF_String *screen_name;
    CMnuScreen *pregame_screen = pn_get_pregame_screen((_this->CAppMenu_data).splash_screen);
    log_info("Check 6");
    if(pregame_screen != (CMnuScreen *)0x0)
    {
        log_info("Check 7");
        pregame_screen = pn_get_pregame_screen((_this->CAppMenu_data).splash_screen);
        screen_name = pn_screen_get_name(pregame_screen);

        log_info("Check 8");
        pn_screen_delete_control((_this->CAppMenu_data).splash_screen, screen_name);
        log_info("Check 9");
        pn_screen_set_active((_this->CAppMenu_data).splash_screen, (CMnuScreen *)0x0);
    }
    log_info("Check 10");
    CreateMnuHintExt(_this);
    log_info("Check 11");
    SF_String dot_map;
    uiAPI.SFStringConstructor_char(&dot_map, ".map");

    switch((_this->CAppMenu_data).pregame_load_result)
    {
        case 0:
        case 2:
        {
            log_info("Case 0 || 2");
            if((_this->CAppMenu_data).campaign_type == 2)
            {
                log_info("Load Vanillia Campaign 2");
                uint32_t SotPSide = pn_preload_get_sotp_side(preload);
                uint32_t campaign_type = pn_preload_get_campaign_type(preload);
                uint32_t skip_tutorial = pn_preload_get_skip_tutorial(preload, game_info);
                initFirstMap(game_info, skip_tutorial, skill_id, subskill_spec, campaign_type, SotPSide);
                SF_String template_predefined;
                uiAPI.SFStringConstructor(&template_predefined);
                //sotp_side =
            }
            else
            {
                log_info("Confirming Campaign Type: %d", (_this->CAppMenu_data).campaign_type);
                if((_this->CAppMenu_data).campaign_type == 0)
                {
                    log_info("Loading Vanillia Campaign 0");
                    uint32_t campaign_type = pn_preload_get_campaign_type(preload);
                    uint32_t skip_tutorial = pn_preload_get_skip_tutorial(preload, game_info);
                    initFirstMap(game_info, skip_tutorial, skill_id, subskill_spec, campaign_type, false);
                    uint8_t premade_kit_index = pn_preload_get_kit_index(preload);
                    pn_update_kit(game_info, (_this->CAppMenu_data).pregame_load_result == 2, premade_kit_index);
                    premade_kit_index = pn_preload_get_kit_index(preload);
                    pn_update_kit_2(game_info, ((_this->CAppMenu_data).pregame_load_result == 2), premade_kit_index);
                    uiAPI.SFStringCopy(&(_this->CAppMenu_data).pregrame_dotmap_string, &dot_map);
                    s_play_campaign_intro(_this);
                    uiAPI.SFStringDestructor(screen_name);
                    break;
                }
                else
                {
                    log_info("Load Custom Campaign");
                    uint32_t campaign_type = pn_preload_get_campaign_type(preload);
                    uint32_t skip_tutorial = pn_preload_get_skip_tutorial(preload, game_info);

                    log_info("init First Map Test?");
                    initFirstMap(game_info, skip_tutorial, skill_id, subskill_spec, campaign_type, false);
                    log_info("Get kit index?");
                    uint8_t premade_kit_index = (preload->CUiMenuPreLoad_data).premade_kit_index;
                    log_info("update kit index?");
                    pn_update_kit(game_info, (_this->CAppMenu_data).pregame_load_result == 2, premade_kit_index);
                    log_info("refresh kit index");
                    premade_kit_index = (preload->CUiMenuPreLoad_data).premade_kit_index;
                    log_info("update kit index 2");
                    pn_update_kit_2(game_info, ((_this->CAppMenu_data).pregame_load_result == 2), premade_kit_index);

                    log_info("copy dotmap?");
                    uiAPI.SFStringCopy(&(_this->CAppMenu_data).pregrame_dotmap_string, &dot_map);
                    log_info("play intro");
                    s_play_campaign_intro(_this);
                    break;
                }
            }
            break;
        }

        case 7:
        {
        log_info("Entered Case 7");
        }
        case 8:
        {
        log_info("Entered Case 8");
        }
        case 9:
        {
        log_info("Entered Case 9");
        }
        case 1:
        case 3:
        {
            log_info("Entered Case 3");
            break;
        }
        case 6:
        {
            log_info("Entered Case 1 || 6");

            uiAPI.SFStringDestructor(screen_name);
            break;
        }
        default:
            break;
    }

    log_info("clean up dotmap");
    uiAPI.SFStringDestructor(&dot_map);
    log_info("write to config");

    screen_name = pn_preload_get_avatar_name(preload, screen_name);
    bool is_screen_empty = screen_name->str_length == 0;
    char screen_flag = '\0';
    if(is_screen_empty)
    {
        screen_flag = '\x01';
    }

    if(screen_flag != '\0')
    {
        uint32_t campaign_type = (_this->CAppMenu_data).campaign_type;
        if(is_screen_empty)
        {
            uiAPI.SFStringConstructor_char(screen_name, "");
            screen_name = pn_preload_get_avatar_name(preload, screen_name);
        }
        char* section = uiAPI.SFStringCMbStr(pn_s_cfg_key);
        char* key = uiAPI.SFStringCMbStr(pn_s_last_sf1);
        // idk much about this area, I suspect this will cause issues
        log_info("set string");
        pn_cfg_set_string(&configFile, section, key, screen_name, screen_name);
    }

    log_info("cleanup");


    write_last_played(_this, preload, &configFile, false);
    uiAPI.SFStringDestructor(&configFile.name_maybe);
    pn_cfg_dtor(&configFile);

    log_info("done");
}

void install_preparenewgame_hook()
{
    ASI::MemoryRegion mreg(ASI::AddrOf(0x195e10), 5);
    ASI::BeginRewrite(mreg);
    *(unsigned char *)(ASI::AddrOf(0x195e10)) = 0xE9;
    *(int *)(ASI::AddrOf(0x195e11)) = (int)(&hooked_prepare_new_game) - (int)(ASI::AddrOf(0x195e15));
    ASI::EndRewrite(mreg);
    log_info("PrepareNewGame replacement hooked (entry JMP @ 0x595e10)");
}

/** @} */
