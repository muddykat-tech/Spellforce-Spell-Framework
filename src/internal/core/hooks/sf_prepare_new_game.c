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
static gameInfoReset_ptr        pn_gi_reset;
static preloadGetAvatar_ptr     pn_preload_get_avatar;
static gameInfoSetAvatar_ptr    pn_gi_set_avatar;
static gameInfoGetU8_ptr        pn_gi_get_skill, pn_gi_get_subskill, pn_gi_get_kit_index;
static preloadGetU32_ptr        pn_preload_get_sotp_side, pn_preload_get_campaign_type,
                                pn_preload_get_skip_tutorial;
static giSetBoolU8_ptr          pn_gi_set_premade_kit, pn_gi_apply_premade_kit;
static giSetU32_ptr             pn_gi_set_start_mode2, pn_gi_set_game_mode,
                                pn_gi_set_flag_a, pn_gi_set_flag_b;
static giSetStr_ptr             pn_gi_set_save_file_path, pn_gi_set_save_file_name,
                                pn_gi_set_template_name;
static giVoid_ptr               pn_gi_refresh_case7;
static giStarterKitReset_ptr    pn_gi_starterkit_reset;
static preloadGetStr_ptr        pn_preload_get_slot_name, pn_preload_get_avatar_name,
                                pn_preload_get_freegame_map, pn_preload_get_freegame_template;
static appMenuDeleteSave_ptr    pn_delete_save;
static screenGetHint_ptr        pn_screen_get_hint;
static hintResolve_ptr          pn_hint_resolve;
static screenDeleteControl_ptr  pn_screen_delete_control;
static screenSetHint_ptr        pn_screen_set_hint;
static cfgCtor_ptr              pn_cfg_ctor;
static cfgDtor_ptr              pn_cfg_dtor;
static cfgSetString_ptr         pn_cfg_set_string;
static preloadGetU32_ptr        pn_preload_get_kit_index;

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
    pn_gi_reset              = (gameInfoReset_ptr)(ASI::AddrOf(0x1a1830));
    pn_preload_get_avatar    = (preloadGetAvatar_ptr)(ASI::AddrOf(0x5fae00));
    pn_gi_set_avatar         = (gameInfoSetAvatar_ptr)(ASI::AddrOf(0x1a1440));
    pn_gi_get_skill          = (gameInfoGetU8_ptr)(ASI::AddrOf(0x19e7a0));
    pn_gi_get_subskill       = (gameInfoGetU8_ptr)(ASI::AddrOf(0x19e7c0));
    pn_gi_get_kit_index      = (gameInfoGetU8_ptr)(ASI::AddrOf(0x175f00));
    pn_preload_get_sotp_side = (preloadGetU32_ptr)(ASI::AddrOf(0x60a3d0));
    pn_preload_get_campaign_type  = (preloadGetU32_ptr)(ASI::AddrOf(0x5faa10));
    pn_preload_get_skip_tutorial  = (preloadGetU32_ptr)(ASI::AddrOf(0x609780));
    pn_preload_get_kit_index      = (preloadGetU32_ptr)(ASI::AddrOf(0x5fae20));
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
    pn_screen_get_hint       = (screenGetHint_ptr)(ASI::AddrOf(0x50f930));
    pn_hint_resolve          = (hintResolve_ptr)(ASI::AddrOf(0x510cf0));
    pn_screen_delete_control = (screenDeleteControl_ptr)(ASI::AddrOf(0x508560));
    pn_screen_set_hint       = (screenSetHint_ptr)(ASI::AddrOf(0x512180));
    pn_cfg_ctor              = (cfgCtor_ptr)(ASI::AddrOf(0x389050));
    pn_cfg_dtor              = (cfgDtor_ptr)(ASI::AddrOf(0x387e20));
    pn_cfg_set_string        = (cfgSetString_ptr)(ASI::AddrOf(0x3897b0));
    CreateMnuHintExt         = (CreateMnuHintExt_ptr)(ASI::AddrOf(0x18eca0));

    pn_s_cfg_key    = (SF_String *)(ASI::AddrOf(0x924120));
    pn_s_last_sf1   = (SF_String *)(ASI::AddrOf(0x924320));
    pn_s_last_addon = (SF_String *)(ASI::AddrOf(0x9243d0));
    pn_s_last_sotp  = (SF_String *)(ASI::AddrOf(0x924480));
    pn_s_last_coop  = (SF_String *)(ASI::AddrOf(0x923c90));

    install_preparenewgame_hook();
}

/* "<base><dir><avatar>~<slot>.sav" for case 6. */
void build_load_path(CAppMenu *_this, void *preload, SF_String *out)
{
    SF_String base;
    s_get_base_path_string(&base);
    char narrow[512]; int len = 0;
    if (base.raw_data && base.str_length > 0)
        len = WideCharToMultiByte(CP_ACP, 0, base.raw_data, base.str_length,
                                  narrow, sizeof(narrow) - 1, NULL, NULL);
    if (len < 0) len = 0;
    narrow[len] = '\0';
    uiAPI.SFStringDestructor(&base);

    int ct = _this->CAppMenu_data.campaign_type;
    int idx = ct - SFSF_CAMPAIGN_TYPE_BASE;
    char dir[192];
    if (idx >= 0 && idx < (int32_t)g_campaign_count)
        snprintf(dir, sizeof(dir), "save\\campaigns\\%s\\", g_campaigns[idx].campaign_folder);
    else if (ct == 1) snprintf(dir, sizeof(dir), "save\\campaign2\\");
    else if (ct == 2) snprintf(dir, sizeof(dir), "save\\campaign3\\");
    else              snprintf(dir, sizeof(dir), "save\\");

    SF_String avatar_name, slot_name;
    uiAPI.SFStringConstructor(&avatar_name);
    uiAPI.SFStringConstructor(&slot_name);
    pn_preload_get_avatar_name(preload, &avatar_name);
    pn_preload_get_slot_name(preload, &slot_name);
    char an[128] = {0}, sn[128] = {0};
    if (avatar_name.raw_data && avatar_name.str_length > 0)
        WideCharToMultiByte(CP_ACP, 0, avatar_name.raw_data, avatar_name.str_length,
                            an, sizeof(an) - 1, NULL, NULL);
    if (slot_name.raw_data && slot_name.str_length > 0)
        WideCharToMultiByte(CP_ACP, 0, slot_name.raw_data, slot_name.str_length,
                            sn, sizeof(sn) - 1, NULL, NULL);
    uiAPI.SFStringDestructor(&avatar_name);
    uiAPI.SFStringDestructor(&slot_name);

    char full[1024];
    snprintf(full, sizeof(full), "%s%s%s~%s.sav", narrow, dir, an, sn);
    uiAPI.SFStringConstructor_char(out, full);
}

/* "last played" epilogue - vanilla types; custom campaigns skip this for now */
void write_last_played(CAppMenu *_this, void *preload, bool is_custom)
{
    if (is_custom) { return; }

    SF_String name;
    uiAPI.SFStringConstructor(&name);
    pn_preload_get_avatar_name(preload, &name);
    if (name.str_length == 0) { uiAPI.SFStringDestructor(&name); return; }

    SF_String *key_str;
    int ct = _this->CAppMenu_data.campaign_type;
    if (ct == 1)      key_str = pn_s_last_addon;
    else if (ct == 2) key_str = pn_s_last_sotp;
    else key_str = (_this->CAppMenu_data.game_info.is_coop != 0) ? pn_s_last_coop
                                                                 : pn_s_last_sf1;
    uint8_t cfg[0x40];
    SF_String empty;
    uiAPI.SFStringConstructor(&empty);
    pn_cfg_ctor(cfg, NULL);
    pn_cfg_set_string(cfg, uiAPI.SFStringCMbStr(pn_s_cfg_key),
                      uiAPI.SFStringCMbStr(key_str), &name, &empty);
    /* ghidra has SF_String::destructor(cfg+0x1c) then dtor, replicating here */
    uiAPI.SFStringDestructor((SF_String *)(cfg + 0x1c));
    pn_cfg_dtor(cfg);
    uiAPI.SFStringDestructor(&empty);
    uiAPI.SFStringDestructor(&name);
}

void __thiscall hooked_prepare_new_game(CAppMenu *_this, void *preload)
{
    log_info("Prepare new game");
    SF_GameInfo *gi = &_this->CAppMenu_data.game_info;
    // some cursed pointer walking for now
    uint32_t result = pn_get_result_code(preload);
    *(uint32_t *)((uint8_t *)&_this->CAppMenu_data + 0x158) = result;

    /* ---- avatar transfer prologue ----
     * avatar type (our 8+idx marker)
     * +0x10+0xd2 offset asm-verified. */
    uint16_t avatar_type = *(uint16_t *)((uint8_t *)gi + 0x10 + 0xd2);
    pn_gi_reset(gi, 0);
    uint8_t av_buf1[0x100]; uint8_t av_buf2[0x100];
    GdAvatarData *av1 = pn_preload_get_avatar(preload, av_buf1);
    /* first u16 of the vector's first element -> lands in AC82+0 via setAvatar
     * (asm-confirmed destination). Null-guarded: vanilla derefs blindly. */
    void *vec_data = *(void **)((uint8_t *)av1 + 0xD4);
    uint16_t first_elem_u16 = vec_data ? *(uint16_t *)vec_data : 0;
    GdAvatarData *av2 = pn_preload_get_avatar(preload, av_buf2);
    pn_gi_set_avatar(gi, av2, first_elem_u16);
    s_gameinfo_set_avatar_type(gi, avatar_type);

    uint8_t skill    = pn_gi_get_skill(gi);
    uint8_t subskill = pn_gi_get_subskill(gi);

    /* splash hint cleanup (cosmetic; conventions inferred) */
    void *splash = _this->CAppMenu_data.splash_screen;
    int hint = pn_screen_get_hint(splash);
    if (hint != 0)
    {
        pn_screen_delete_control(splash, pn_hint_resolve(pn_screen_get_hint(splash)));
        pn_screen_set_hint(splash, 0);
    }
    CreateMnuHintExt(_this);

    int ct = _this->CAppMenu_data.campaign_type;   /* REAL type; asm: CAppMenu+0x49C */
    int custom_idx = ct - SFSF_CAMPAIGN_TYPE_BASE;
    bool is_custom = (custom_idx >= 0 && custom_idx < (int32_t)g_campaign_count);
    bool handled = true;

    SF_String dot_map;
    uiAPI.SFStringConstructor_char(&dot_map, ".map");

    switch (result)
    {
    case 0: /* new game, created avatar */
    case 2: /* new game, premade avatar */
    {
        uint32_t skip_tut  = pn_preload_get_skip_tutorial(preload); /* engine forces skip for coop / campaign!=0 */
        uint8_t  kit_index = (uint8_t)pn_preload_get_kit_index(preload);

        if (!is_custom && ct == 2)
        {
            uint32_t side = pn_preload_get_sotp_side(preload);
            initFirstMap(gi, skip_tut & 0xff, skill, subskill,
                         pn_preload_get_campaign_type(preload), (bool)(uint8_t)side);
            pn_gi_set_premade_kit(gi, result == 2, kit_index);
            /* PDC3_<kit+1><p|s>.des predefined template */
            char t[80];
            uint8_t kit = pn_gi_get_kit_index(gi);
            snprintf(t, sizeof(t), "figure_template\\predefined\\PDC3_%02d%s.des",
                     (kit & 0xff) + 1, side == 0 ? "p" : "s");
            SF_String tmpl;
            uiAPI.SFStringConstructor_char(&tmpl, t);
            pn_gi_set_start_mode2(gi, 2);
            pn_gi_set_template_name(gi, &tmpl);
            uiAPI.SFStringDestructor(&tmpl);
        }
        else
        {
            initFirstMap(gi, skip_tut & 0xff, skill, subskill,
                         pn_preload_get_campaign_type(preload), false);
            pn_gi_set_premade_kit(gi, result == 2, kit_index);
            pn_gi_apply_premade_kit(gi, result == 2, kit_index);
        }
        uiAPI.SFStringCopy((SF_String *)((uint8_t *)&_this->CAppMenu_data + 0xac), &dot_map);
        /* intro seam lives here now (custom intro_video hooks in later) */
        s_play_campaign_intro(_this);
        break;
    }

    case 1: /* name conflict paths - reset to starter kit + feedback dialog */
    case 3:
    {
        pn_gi_starterkit_reset(gi, skill, subskill);
        uint8_t kit_index = (uint8_t)pn_preload_get_kit_index(preload);
        pn_gi_set_premade_kit(gi, result == 3, kit_index);
        pn_gi_apply_premade_kit(gi, result == 3, kit_index);
        uiAPI.SFStringCopy((SF_String *)((uint8_t *)&_this->CAppMenu_data + 0xac), &dot_map);

        log_info("PrepareNewGame: case %u (name conflict?) - dialog suppressed not sure what's going on in decomp here", result);
        break;
    }

    case 6: /* load selected save */
    {
        SF_String path;
        build_load_path(_this, preload, &path);
        log_info("Loading save: %ls", path.raw_data);
        pn_gi_set_save_file_path(gi, &path);
        SF_String empty;
        uiAPI.SFStringConstructor(&empty);
        s_start_game(_this, gi, 100, 0, 0, 0, &empty);
        uiAPI.SFStringDestructor(&empty);
        uiAPI.SFStringDestructor(&path);
        break;
    }

    case 7: /* delete save */
    {
        SF_String slot, dot_sav;
        uiAPI.SFStringConstructor(&slot);
        pn_preload_get_slot_name(preload, &slot);
        uiAPI.SFStringConstructor_char(&dot_sav, ".sav");
        uiAPI.SFStringConcat(&slot, &dot_sav);
        uiAPI.SFStringDestructor(&dot_sav);
        pn_delete_save(_this, &slot);
        uiAPI.SFStringDestructor(&slot);
        pn_gi_refresh_case7(gi);
        uiAPI.SFStringCopy((SF_String *)((uint8_t *)&_this->CAppMenu_data + 0xac), &dot_map);
        log_info("PrepareNewGame: case 7 (delete) some stuff missing here I think");
        break;
    }

    case 8: /* free game with template */
    {
        pn_gi_set_flag_a(gi, 1);
        pn_gi_set_flag_b(gi, 1);
        pn_gi_set_game_mode(gi, 3);
        SF_String map;
        uiAPI.SFStringConstructor(&map);
        pn_preload_get_freegame_map(preload, &map);
        pn_gi_set_save_file_name(gi, &map);
        uiAPI.SFStringDestructor(&map);
        pn_gi_set_start_mode2(gi, 1);
        SF_String tmpl, full_tmpl, prefix;
        uiAPI.SFStringConstructor(&tmpl);
        pn_preload_get_freegame_template(preload, &tmpl);
        uiAPI.SFStringConstructor_char(&prefix, "figure_template\\mission\\");
        uiAPI.SFStringConstructor(&full_tmpl);
        uiAPI.SFStringConcat(&full_tmpl, &prefix);
        uiAPI.SFStringConcat(&full_tmpl, &tmpl);
        pn_gi_set_template_name(gi, &full_tmpl);
        uiAPI.SFStringDestructor(&full_tmpl);
        uiAPI.SFStringDestructor(&prefix);
        uiAPI.SFStringDestructor(&tmpl);
        SF_String empty;
        uiAPI.SFStringConstructor(&empty);
        s_start_game(_this, gi, 100, 0, 0, 0, &empty);
        uiAPI.SFStringDestructor(&empty);
        break;
    }

    case 9: /* TODO */
        log_error("PrepareNewGame rewrite: case 9 (SotP import) not implemented");
        handled = false;
        break;

    default:
        handled = false;
        break;
    }

    uiAPI.SFStringDestructor(&dot_map);

    if (handled)
    {
        write_last_played(_this, preload, is_custom);
    }
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
