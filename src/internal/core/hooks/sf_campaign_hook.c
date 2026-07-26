/**
 * @addtogroup CampaignHook
 * @{
 */

#include "../sf_wrappers.h"
#include "../sf_ui_wrappers.h"
#include "sf_campaign_hook.h"
#include "../sf_hooks.h"

#include "sf_vanilla_fix_hook.h" // This is a dirty way to nab a few things from it without paying our dues

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>

static gameInfoSetAvatarType_ptr s_gameinfo_set_avatar_type;
static gameInfoSetMapPathFull_ptr s_gameinfo_set_map_path;
static appMenuEnterCampaignFlow_ptr s_enter_campaign_flow;
static playCampaignIntro_ptr s_play_campaign_intro;
static videoSequenceStop_ptr s_video_sequence_stop;
static startGame_ptr s_start_game;
static getBasePathString_ptr s_get_base_path_string;

static SF_String *s_g_save_campaign2; /* "save\campaign2\" global */
static SF_String *s_g_save_campaign3; /* "save\campaign3\" global */
static SF_String *s_g_save_base;      /* "save\" global @ 0x923f60 */

static void getsavepath_hook();
static void install_intro_callsite_patch();

static avatarInternalCopy_ptr s_avatar_internal_copy;
static avatarVectorsCopy_ptr  s_avatar_vectors_copy;
static void install_initfirstmap_hook();

void initialize_campaign_hooks()
{
    s_gameinfo_set_map_path = (gameInfoSetMapPathFull_ptr)(ASI::AddrOf(0x1762d0));
    s_enter_campaign_flow   = (appMenuEnterCampaignFlow_ptr)(ASI::AddrOf(0x1936a0));
    s_play_campaign_intro   = (playCampaignIntro_ptr)(ASI::AddrOf(0x181e20));
    s_video_sequence_stop   = (videoSequenceStop_ptr)(ASI::AddrOf(0x3d7b90));
    s_start_game            = (startGame_ptr)(ASI::AddrOf(0x183560));
    s_get_base_path_string  = (getBasePathString_ptr)(ASI::AddrOf(0x61a700));

    s_g_save_campaign2 = (SF_String *)(ASI::AddrOf(0x9242f0));
    s_g_save_campaign3 = (SF_String *)(ASI::AddrOf(0x924580));
    s_g_save_base      = (SF_String *)(ASI::AddrOf(0x923f60));

    s_gameinfo_set_avatar_type = (gameInfoSetAvatarType_ptr)(ASI::AddrOf(0x1a1a80));

    s_avatar_internal_copy = (avatarInternalCopy_ptr)(ASI::AddrOf(0x1759e0));
    s_avatar_vectors_copy  = (avatarVectorsCopy_ptr)(ASI::AddrOf(0x175860));

    install_initfirstmap_hook();
    install_intro_callsite_patch();
    getsavepath_hook();

    // TEST CAMPAIGN - remove once registration comes from the mod registry
    SFSF_CampaignDef test_campaign = {};
    strncpy(test_campaign.name,        "testcampaign",      sizeof(test_campaign.name) - 1);
    strncpy(test_campaign.description, "Hook test",         sizeof(test_campaign.description) - 1);
    strncpy(test_campaign.start_map,   "000_liannon",   sizeof(test_campaign.start_map) - 1);
    strncpy(test_campaign.campaign_folder, "testcampaign",      sizeof(test_campaign.campaign_folder) - 1);
    strncpy(test_campaign.tutorial_map, "",                 sizeof(test_campaign.tutorial_map) - 1);
    test_campaign.engine_type = 0;   // masquerade as SF1. If we want custom campaign type we change this value.
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
    if (def->engine_type > 2)
    {
        log_error("Campaign '%s': engine_type must be 0..2 (vanilla masquerade)", def->name);
        return -1;
    }
    g_campaigns[g_campaign_count] = *def;
    log_info("Registered campaign %u: %s (map: %s, saves: %s, engine_type: %u)",
             g_campaign_count, def->name, def->start_map,
             def->campaign_folder, def->engine_type);
    return (int32_t)g_campaign_count++;
}

/* Un'Schtalch's code block - updated and reworked a bit for you ~Muddykat*/


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
    log_info("initFirstMap Hook: Avatar Snapshot Ran: s_avatar_internal_copy(&_this->AC82_1.avatarData.internal, &_this->AC82.avatarData.internal);");
    s_avatar_vectors_copy(&_this->AC82_1.avatarData.begin, &_this->AC82.avatarData.begin);
    log_info("initFirstMap Hook: Avatar Snapshot Ran: s_avatar_vectors_copy(&_this->AC82_1.avatarData.begin, &_this->AC82.avatarData.begin);");

    log_info("initFirstMap Hook: Check for Custom Campaign");
    /* -- active custom campaign? -- */
    const SFSF_CampaignDef *custom = NULL;
    if (g_active_custom_campaign >= 0 &&
        g_active_custom_campaign < (int32_t)g_campaign_count)
    {
        custom = &g_campaigns[g_active_custom_campaign];
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
                uiAPI.SFStringConstructor_char(&campagn_path, "map\\campaign2\\");
                uiAPI.SFStringConstructor_wchar(&intial_map_name, L"P101_Mirraw_Thur");
                break;
            case 2:
                log_info("initFirstMap Hook: Engine Type 2");
                uiAPI.SFStringConstructor_char(&campagn_path, "map\\campaign3\\");
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
                uiAPI.SFStringConstructor_char(&campagn_path, "map\\campaign\\");
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

static void install_initfirstmap_hook()
{
    uint32_t entry = ASI::AddrOf(0x176040);
    ASI::MemoryRegion mreg(entry, 5);
    ASI::BeginRewrite(mreg);
    *(unsigned char *)entry = 0xE9;
    *(int *)(entry + 1) = (int)(&initFirstMap) - (int)(entry + 5);
    ASI::EndRewrite(mreg);
    log_info("initFirstMap replacement hooked (entry JMP)");
}

/* Originals captured before the first swap so they can be restored verbatim. */
static SF_String s_orig_campaign2;
static SF_String s_orig_campaign3;
static SF_String s_orig_base;
static bool s_originals_captured = false;
static bool s_globals_swapped = false;

static void capture_save_globals()
{
    if (s_originals_captured || s_g_save_base == NULL)
    {
        return;
    }
    uiAPI.SFStringConstructor(&s_orig_campaign2);
    uiAPI.SFStringConstructor(&s_orig_campaign3);
    uiAPI.SFStringConstructor(&s_orig_base);
    uiAPI.SFStringCopy(&s_orig_campaign2, s_g_save_campaign2);
    uiAPI.SFStringCopy(&s_orig_campaign3, s_g_save_campaign3);
    uiAPI.SFStringCopy(&s_orig_base, s_g_save_base);
    s_originals_captured = true;
}

static void swap_save_globals(const SFSF_CampaignDef *def)
{
    if (s_g_save_base == NULL)
    {
        log_warning("SF_String_save address not set - save separation incomplete");
        return;
    }
    capture_save_globals();

    char dir[192];
    snprintf(dir, sizeof(dir), "save\\campaigns\\%s\\", def->campaign_folder);

    SF_String sf_dir;
    uiAPI.SFStringConstructor_char(&sf_dir, dir);

    /* All three constants point at the SAME folder while a custom campaign is
     * active e.g it means things should be redirected regardless of which constant it reads.

     System may need to be changed if we figure out proper custom campaign type (engine_type atm) but we'd need to
     call the same setup functions that the other camapgins do for the menu / ui systems
     */
    uiAPI.SFStringCopy(s_g_save_campaign2, &sf_dir);
    uiAPI.SFStringCopy(s_g_save_campaign3, &sf_dir);
    uiAPI.SFStringCopy(s_g_save_base, &sf_dir);
    uiAPI.SFStringDestructor(&sf_dir);
    s_globals_swapped = true;

    _mkdir("save");
    _mkdir("save\\campaigns");
    char full[256];
    snprintf(full, sizeof(full), "save\\campaigns\\%s", def->campaign_folder);
    _mkdir(full);

    log_info("Save dirs redirected to %s", dir);
}

static void restore_save_globals()
{
    if (!s_globals_swapped || !s_originals_captured)
    {
        return;
    }
    uiAPI.SFStringCopy(s_g_save_campaign2, &s_orig_campaign2);
    uiAPI.SFStringCopy(s_g_save_campaign3, &s_orig_campaign3);
    uiAPI.SFStringCopy(s_g_save_base, &s_orig_base);
    s_globals_swapped = false;
    log_info("Save dirs restored to vanilla");
}

void campaign_hook_on_main_menu(CAppMenu *app_menu)
{
    g_campaign_app_menu = app_menu;
    restore_save_globals();
    g_active_custom_campaign = -1;
}

/**
 * reimplementation of 0x1b89d0 + custom branch for custom campaigns ^_^
 */
SF_String * __stdcall hooked_getSavePath(SF_String *out, int campaign_type)
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
    if (g_active_custom_campaign >= 0 &&
        g_active_custom_campaign < (int32_t)g_campaign_count)
    {
        snprintf(custom_suffix, sizeof(custom_suffix), "save\\campaigns\\%s\\",
                 g_campaigns[g_active_custom_campaign].campaign_folder);
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

static void getsavepath_hook()
{
    ASI::MemoryRegion mreg(ASI::AddrOf(0x1b89d0), 5);
    ASI::BeginRewrite(mreg);
    *(unsigned char *)(ASI::AddrOf(0x1b89d0)) = 0xE9; /* JMP */
    *(int *)(ASI::AddrOf(0x1b89d1)) =
        (int)(&hooked_getSavePath) - ASI::AddrOf(0x1b89d5);
    ASI::EndRewrite(mreg);
    log_info("getSavePath replacement hooked");
}

/**
 * Replaces CALL PlayCampaignIntroVideo at the end of PrepareNewGame
 */
void __fastcall hooked_play_campaign_intro(CAppMenu *_this)
{
    // keeping it hooked, but we just essentially jump right back for now
    // Might be useful for custom videos, or may not be needed at all
    s_play_campaign_intro(_this);
}

static void install_intro_callsite_patch()
{
    /* CALL rel32 at 0x1960ac */
    ASI::MemoryRegion mreg(ASI::AddrOf(0x1960ac), 5);
    ASI::BeginRewrite(mreg);
    *(unsigned char *)(ASI::AddrOf(0x1960ac)) = 0xE8; /* CALL hijack*/
    *(int *)(ASI::AddrOf(0x1960ad)) =
        (int)(&hooked_play_campaign_intro) - ASI::AddrOf(0x1960b1);
    ASI::EndRewrite(mreg);
    log_info("Intro call-site patch installed (start-map override)");
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

static void campaign_launch_flow(int32_t campaign_index)
{
    CAppMenu *app_menu = g_campaign_app_menu;
    const SFSF_CampaignDef *def = &g_campaigns[campaign_index];

    if (app_menu == NULL || s_gameinfo_set_avatar_type == NULL)
    {
        log_error("Campaign launch unavailable: missing CAppMenu or engine addresses");
        return;
    }

    log_info("Launching campaign '%s' (flow, engine_type %u, avatar %u)",
             def->name, def->engine_type, def->avatar_type);

    stop_intro_video(app_menu);

    g_active_custom_campaign = campaign_index;
    swap_save_globals(def);

    app_menu->CAppMenu_data.campaign_type = def->engine_type; //current method is we pretend to be vanilla campaign, may not fly
    s_gameinfo_set_avatar_type(&app_menu->CAppMenu_data.game_info, (uint16_t)def->avatar_type);

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
