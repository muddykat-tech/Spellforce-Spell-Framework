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
static gameInfoGetU8_ptr pn_gi_get_kit_index;
static gameInfoFigureAbilities_ptr pn_gi_get_skill, pn_gi_get_subskill;
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
static appMenuReadSave_ptr pn_read_from_save;
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
static SF_String *pn_s_last_addon;       /* S_LAST_PLAYED_ADDON @ 0xd243d0       */
static SF_String *pn_s_last_sotp;        /* DAT_00d24480 @ 0xd24480              */
static SF_String *pn_s_last_coop;        /* DAT_00d23c90 @ 0xd23c90              */
CreateMnuHintExt_ptr CreateMnuHintExt;
CreateMenuPreMulti_ptr CreateMenuPreMulti;

static InitUnknownAvatar_0x54_ptr InitUnknownAvatar_0x54, DisposeUnknownAvatar_0x54;
static updatePreloadUnknown_0x54_ptr updatePreloadUnknown_0x54;
static InitAvatarInternal_ptr InitAvatarInternal;
importFromSave_ptr importFromSave;
static void install_preparenewgame_hook();

void initialize_preparenewgame_rewrite()
{
    pn_get_result_code       = (preloadGetResultCode_ptr)(ASI::AddrOf(0x5fad30));

    pn_gi_reset_avatar              = (gameInfoReset_ptr)(ASI::AddrOf(0x1a1830));
    pn_preload_get_avatar    = (preloadGetAvatar_ptr)(ASI::AddrOf(0x5fae00));
    pn_gi_set_avatar_equipdata         = (gameInfoSetAvatar_ptr)(ASI::AddrOf(0x1a1440));
    pn_gi_get_skill          = (gameInfoFigureAbilities_ptr)(ASI::AddrOf(0x19e7a0));
    pn_gi_get_subskill       = (gameInfoFigureAbilities_ptr)(ASI::AddrOf(0x19e7c0));
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
    pn_read_from_save           = (appMenuReadSave_ptr)(ASI::AddrOf(0x186520));
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

    CreateMenuPreMulti = (CreateMenuPreMulti_ptr)(ASI::AddrOf(0x193980));

    pn_s_cfg_key    = (SF_String *)(ASI::AddrOf(0x924120));
    pn_s_last_addon = (SF_String *)(ASI::AddrOf(0x9243d0));
    pn_s_last_sotp  = (SF_String *)(ASI::AddrOf(0x924480));
    pn_s_last_coop  = (SF_String *)(ASI::AddrOf(0x923c90));

    InitUnknownAvatar_0x54 = (InitUnknownAvatar_0x54_ptr)(ASI::AddrOf(0x17dd30));
    DisposeUnknownAvatar_0x54 = (InitUnknownAvatar_0x54_ptr)(ASI::AddrOf(0x17f110));

    updatePreloadUnknown_0x54 = (updatePreloadUnknown_0x54_ptr)(ASI::AddrOf(0x6097b0));
    InitAvatarInternal = (InitAvatarInternal_ptr)(ASI::AddrOf(0x17d920));
    importFromSave = (importFromSave_ptr)(ASI::AddrOf(0x18a140));
    install_preparenewgame_hook();
}

uint8_t __thiscall pn_preload_get_skip_tutorial(CUiMenuPreLoad *_this)
{
    uint8_t is_coop = (_this->CUiMenuPreLoad_data.game_info)->is_coop;
    if(is_coop == 0 && (_this->CUiMenuPreLoad_data).campaign_type == 0)
    {
        return (_this->CUiMenuPreLoad_data).skip_tutorial;
    }
    return 1;
}

SF_String * __thiscall getSavePath(CAppSession *_this, SF_String *output, uint32_t campaign_type);

SF_String * build_load_path(CAppMenu *_this, void *preload, SF_String *out)
{
    SF_String base, avatar_name, slot_name;
    uiAPI.SFStringConstructor(&base);
    int ct = _this->CAppMenu_data.campaign_type;
    if (_this->CAppMenu_data.game_info.is_coop)
    {
        SF_String coop_str;
        uiAPI.SFStringConstructor_char(&coop_str, "CHAR\\");
        getSavePath(_this->CAppMenu_data.CAppSession, &base, 0);
        uiAPI.SFStringConcat(&base, &coop_str);
        uiAPI.SFStringDestructor(&coop_str);
    }
    else
    {
        getSavePath(_this->CAppMenu_data.CAppSession, &base, ct);
    }

    pn_preload_get_avatar_name(preload, &avatar_name);
    pn_preload_get_slot_name(preload, &slot_name);

    const wchar_t *bp = base.raw_data        ? base.raw_data        : L"";
    const wchar_t *an = avatar_name.raw_data ? avatar_name.raw_data : L"";
    const wchar_t *sn = slot_name.raw_data   ? slot_name.raw_data   : L"";


    uiAPI.SFStringConstructor(out);
    if (_this->CAppMenu_data.game_info.is_coop)
    {
        uiAPI.SFprintf(out, L"%ls%ls.sav", bp, sn);
    }
    else
    {
        uiAPI.SFprintf(out, L"%ls%ls~%ls.sav", bp, an, sn);
    }

    uiAPI.SFStringDestructor(&slot_name);
    uiAPI.SFStringDestructor(&avatar_name);
    uiAPI.SFStringDestructor(&base);
    return out;
}

void __thiscall hooked_prepare_new_game(CAppMenu *_this, CUiMenuPreLoad *preload)
{
    CUtlConfigFile configFile;
    pn_cfg_ctor(&configFile, (char *)0x0);

    uint8_t avatar_buf_a[224];   // Ghidra's local_1f8
    uint8_t avatar_buf_b[224];   // Ghidra's auStack_11c
    uint32_t result_code = preload->CUiMenuPreLoad_data.offset_0x8c;
    SF_GameInfo *game_info = &(_this->CAppMenu_data).game_info;
    (_this->CAppMenu_data).pregame_load_result = result_code;

    GdAvatarData *avatar_data = &(game_info->AC82).avatarData;

    GdAvatarInternal *internal_avatar = &avatar_data->internal;
    uint16_t saved_avatar_type = internal_avatar->avatar_type;

    pn_gi_reset_avatar(game_info, 0); // might be internal_avatar init
    GdAvatarData *a = pn_preload_get_avatar(preload, (GdAvatarData *)avatar_buf_a);
    uint16_t equip_word = *(uint16_t *)((uint8_t *)a + 0xd4);   // read BEFORE 2nd call
    GdAvatarData *b = pn_preload_get_avatar(preload, (GdAvatarData *)avatar_buf_b);

    pn_gi_set_avatar_equipdata(game_info, (GdAvatar *)b, equip_word);
    game_info->AC82.avatarData.internal.avatar_type = saved_avatar_type;

    uint8_t skill_id = pn_gi_get_skill(game_info, 0);
    uint8_t subskill_spec = pn_gi_get_subskill(game_info, 1);

    CMnuScreen *pregame_screen = pn_get_pregame_screen((_this->CAppMenu_data).splash_screen);

    if (pregame_screen != (CMnuScreen *)0)
    {
        SF_String *ctrl_name = pn_screen_get_name(pregame_screen);
        pn_screen_delete_control((_this->CAppMenu_data).splash_screen, ctrl_name);
        pn_screen_set_active((_this->CAppMenu_data).splash_screen, (CMnuScreen *)0);
    }

    CreateMnuHintExt(_this);

    char store_last_played = 0;

    /* REVIEW: dot_map isn't destroyed, I've forgotten if the game handles this
     * or if we need to destroy it ourselves, the Deep Copy may handle things
     * tho? The same thing applies for predefined_template, which only gets
     * destroyed in the campaign_type == 2 branch of case 0/2 and in case 8 -
     * cases 1/3, 6, 7, 9 and default skip it. Can you take a look as it may be
     * a leak. */
    SF_String dot_map, predefined_template;
    uiAPI.SFStringConstructor_char(&predefined_template, "figure_template\\predefined\\");
    uiAPI.SFStringConstructor_char(&dot_map, "*.map");
    SF_String *nm = NULL;
    switch((_this->CAppMenu_data).pregame_load_result)
    {
        case 0:
        case 2:
        {
            if ((_this->CAppMenu_data).campaign_type == 2)
            {
                uint32_t sotp_side     = pn_preload_get_sotp_side(preload);
                uint32_t campaign_type = pn_preload_get_campaign_type(preload);
                uint32_t skip_tutorial = pn_preload_get_skip_tutorial(preload);

                initFirstMap(game_info, skip_tutorial & 0xff, skill_id, subskill_spec,
                             campaign_type, (uint8_t)sotp_side != 0);

                uint8_t kit = pn_preload_get_kit_index(preload);
                pn_update_kit(game_info, (_this->CAppMenu_data).pregame_load_result == 2, kit);

                SF_String des_name;
                uiAPI.SFStringConstructor(&des_name);                 /* 0x00783900 */

                uint32_t side_again = pn_preload_get_sotp_side(preload);
                uint8_t kit_idx    = pn_gi_get_kit_index(game_info);  /* 0x00575F00 */

                uiAPI.SFprintf(&des_name,
                               (side_again == 0) ? L"PDC3_%02dp.des"
                                                : L"PDC3_%02ds.des",
                               (kit_idx & 0xff) + 1);              /* 0x00784170 */

                game_info->start_mode = 2;

                SF_String tmpl;
                SF_String *full = uiAPI.SFStringConcatMulti(&predefined_template, &tmpl, &des_name);
                pn_gi_set_template_name(game_info, full);             /* 0x00576790 */

                uiAPI.SFStringDestructor(&tmpl);
                uiAPI.SFStringDestructor(&des_name);
                uiAPI.SFStringDestructor(&predefined_template);

            }
            else
            {
                uint32_t campaign_id   = pn_preload_get_campaign_type(preload);
                uint32_t skip_tutorial = pn_preload_get_skip_tutorial(preload);

                initFirstMap(game_info, skip_tutorial & 0xff, skill_id, subskill_spec,
                             campaign_id, false);

                uint8_t kit = pn_preload_get_kit_index(preload);
                pn_update_kit(game_info,
                              (_this->CAppMenu_data).pregame_load_result == 2, kit);

                kit = pn_preload_get_kit_index(preload);
                pn_update_kit_2(game_info,
                                (uint32_t)((_this->CAppMenu_data).pregame_load_result == 2), kit);
            }

            uiAPI.SFStringDeepCopy(&(_this->CAppMenu_data).pregrame_dotmap_string, &dot_map);
            s_play_campaign_intro(_this);
            break;
        }

        case 7:
        {
            SF_String save_path;
            SF_String dot_sav;
            uiAPI.SFStringConstructor_wchar(&dot_sav, L".sav");
            pn_preload_get_slot_name(preload, &save_path);
            uiAPI.SFStringConcat(&save_path, &dot_sav);

            pn_read_from_save(_this, &save_path);
            uiAPI.SFStringDestructor(&save_path);
            uiAPI.SFStringDestructor(&dot_sav);

            pn_gi_refresh_case7(game_info); 
            uiAPI.SFStringDeepCopy(&(_this->CAppMenu_data).pregrame_dotmap_string, &dot_map);

            SF_String avatar_name;
            SF_String extra;

            uiAPI.SFStringConstructor_wchar(&avatar_name,game_info->AC82.avatarData.internal.name);
            uiAPI.SFStringConstructor_char(&extra, "");

            CreateMenuPreMulti(_this, 0, 0, 6, 0, 0, 2, 6942, &extra, &avatar_name);

            uiAPI.SFStringDestructor(&extra);
            uiAPI.SFStringDestructor(&avatar_name);
            break;
        }
        case 8:
        {
            game_info->unknown_0xf0 = 1;
            game_info->unknown_0xf4 = 3;
            game_info->unknown4 = 1;
            SF_String map_name;
            pn_preload_get_freegame_map(preload, &map_name);
            pn_gi_set_save_file_name(game_info, &map_name);
            uiAPI.SFStringDestructor(&map_name);

            game_info->start_mode = 1;
            SF_String template_name;
            pn_preload_get_freegame_template(preload, &template_name);
            uiAPI.SFStringConcat(&predefined_template, &template_name);
            pn_gi_set_template_name(game_info, &predefined_template);
            uiAPI.SFStringDestructor(&predefined_template);
            SF_String extra;
            uiAPI.SFStringConstructor_char(&extra, "");
            s_start_game(_this, game_info, 100, 0,0, 0, &extra);
            uiAPI.SFStringDestructor(&extra);
            break;
        }
        case 9:
        {
            InitUnknownAvatar_0x54(&avatar_buf_b[0x54]);

            uiAPI.SFStringConstructor((SF_String *)(&avatar_buf_b[0xc4]));
            if (updatePreloadUnknown_0x54(preload, &avatar_buf_b[0x54]))
            {
                GdAvatarInternal *ava_int = InitAvatarInternal(&avatar_buf_a[0x8]);
                if (importFromSave(_this, &avatar_buf_b[0x54], ava_int))
                {
                    pn_gi_set_avatar_equipdata(game_info, ava_int, 0);
                    uint32_t sotp_side     = avatar_buf_b[0xd4];
                    SF_String initial_map;
                    if (sotp_side)
                    {
                        uiAPI.SFStringConstructor_char(&initial_map, "map\\Campaign3\\P202_City_Of_Souls.map");
                    }
                    else
                    {
                        uiAPI.SFStringConstructor_char(&initial_map, "map\\Campaign3\\P201_Blackwater_Coast.map");
                    }
                    game_info->unknown_0xf0 = 1;
                    game_info->unknown_0xf4 = 3;
                    game_info->is_tutorial = 0;
                    game_info->start_mode = 3;
                    uiAPI.SFStringDeepCopy(&game_info->filename, &initial_map);
                    uiAPI.SFStringDestructor(&initial_map);
                    s_play_campaign_intro(_this);
                }
                DisposeUnknownAvatar_0x54(&avatar_buf_b[0x54]);
            }
            break;
        }
        case 1:
        case 3:
        {
            pn_gi_starterkit_reset(game_info, skill_id, subskill_spec);
            uint8_t kit_index = pn_preload_get_kit_index (preload);
            pn_update_kit(game_info, (_this->CAppMenu_data).pregame_load_result == 3, kit_index);
            pn_gi_apply_premade_kit(game_info, (_this->CAppMenu_data).pregame_load_result == 3, kit_index);


            uiAPI.SFStringDeepCopy(&_this->CAppMenu_data.pregrame_dotmap_string, &dot_map);

            SF_String avatar_name;
            SF_String extra;
            uiAPI.SFStringConstructor_char(&extra, "");
            pn_preload_get_avatar_name(preload, &avatar_name);

            CreateMenuPreMulti(_this, 0, 0, 6, 0, 0, 2, 6942, &extra, &avatar_name);

            uiAPI.SFStringDestructor(&extra);
            uiAPI.SFStringDestructor(&avatar_name);
            break;
        }
        case 6:
        {
            SF_String save_path;                       /* local_210 */

            build_load_path(_this, preload, &save_path);

            pn_gi_set_save_file_path(game_info, &save_path);

            {
                SF_String empty;                        /* local_220 */
                uiAPI.SFStringConstructor(&empty);

                s_start_game(_this, game_info, 100, 0, 0, 0, &empty);

                uiAPI.SFStringDestructor(&empty);
            }

            uiAPI.SFStringDestructor(&save_path);
            break;
        }
        default:
            goto skip_name_lookup;
            break;
    }

    SF_String avatar_name;
    nm = pn_preload_get_avatar_name(preload, &avatar_name);
    store_last_played = (nm->str_length == 0) ? 0 : 1;
    uiAPI.SFStringDestructor(&avatar_name);

skip_name_lookup:
    if (store_last_played != 0)
    {
        SF_String empty;
        SF_String name;

        int ct = (_this->CAppMenu_data).campaign_type;

        char key_buf[64];
        if (ct == 1)
            strncpy(key_buf, "LastPlayedAddon", sizeof(key_buf));
        else if (ct == 2)
            strncpy(key_buf, "LastPlayedAddOn", sizeof(key_buf));
        else if (game_info->is_coop == 1)
            strncpy(key_buf, "LastPlayedFree", sizeof(key_buf));
        else
            strncpy(key_buf, "LastPlayed", sizeof(key_buf));
        key_buf[sizeof(key_buf) - 1] = '\0';

        uiAPI.SFStringConstructor_char(&empty, "");
        pn_preload_get_avatar_name(preload, &name);

        char stored_buf[32];
        strncpy(stored_buf, "Stored", sizeof(stored_buf));
        stored_buf[sizeof(stored_buf) - 1] = '\0';
        pn_cfg_set_string(&configFile, stored_buf, key_buf, &name, &empty);
        uiAPI.SFStringDestructor(&name);
        uiAPI.SFStringDestructor(&empty);
    }
    
    /* REVIEW: we destroy name_maybe and then call the engine's CUtlConfigFile
     * destructor right after - if pn_cfg_dtor already releases that string this
     * is a double free. Worth a check in Ghidra at 0x387e20. */
    uiAPI.SFStringDestructor(&configFile.name_maybe);
    pn_cfg_dtor(&configFile);
}

void install_preparenewgame_hook()
{
    ASI::MemoryRegion mreg(ASI::AddrOf(0x195e10), 5);
    ASI::BeginRewrite(mreg);
    *(unsigned char *)(ASI::AddrOf(0x195e10)) = 0xE9;
    *(int *)(ASI::AddrOf(0x195e11)) = (int)(&hooked_prepare_new_game) - (int)(ASI::AddrOf(0x195e15));
    ASI::EndRewrite(mreg);
}

/** @} */
