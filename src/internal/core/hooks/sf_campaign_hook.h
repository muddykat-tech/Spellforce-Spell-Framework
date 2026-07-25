/**
 * @defgroup CampaignHook Custom Campaign Hook
 * @ingroup Hooks
 * @brief Custom Campaign selection screen and launch via the vanilla pre-game flow.
 *
 * @addtogroup CampaignHook
 * @{
 */

#ifndef SF_CAMPAIGN_HOOK_H
#define SF_CAMPAIGN_HOOK_H

#include "../../../api/sfsf.h"
#include "../../../asi/sf_asi.h"

#define SFSF_MAX_CAMPAIGNS 2

typedef struct __attribute__((packed))
{
    char name[64];          /**< Shown on the campaign button */
    char description[256];  /**< For the right-hand detail panel */
    char start_map[64];     /**< Map name WITHOUT path/extension, e.g. "mycampaign" */
    char tutorial_map[64];  /**< Optional tutorial map. */
    char save_folder[64];   /**< Folder for save separation and map loading, may need rename, e.g. "mycampaign" */
    char intro_video[64];   /**< Optional: "videos\\myintro" - empty = engine_type
                                 default. NOT implemented yet (needs CUiVideo replication of PlayCampaignIntroVideo). */
    uint32_t engine_type;   /**< 0/1/2 - vanilla campaign to masquerade as.
                                 Avoid 2 unless SotP avatar rules are wanted.
                                 it exposes result case 9 (BoW import with
                                 hardcoded P201/P202 maps so we'd need rewrite to fix). */
    uint32_t avatar_type;   /**< GdAvatar field should continue past a GameInfo reset in PrepareNewGame and is
                                 serialized into saves). Vanilla: 3 = SF1,
                                 4 = AddOn1, 7 = AddOn2, 1/5 = multiplayer.
                                 Custom campaigns: Currently setup to use 8 + campaign_index so
                                 saves are identified for each custom campaign load order issues might
                                 happen with differnt mods though will need extensive error handling / checking.
                                 */
} SFSF_CampaignDef;

typedef void (__thiscall *gameInfoSetAvatarType_ptr)(SF_GameInfo *_this, uint16_t avatar_type);
typedef void (__thiscall *gameInfoSetMapPathFull_ptr)(SF_GameInfo *_this, SF_String *map_path);
typedef void (__thiscall *appMenuEnterCampaignFlow_ptr)(CAppMenu *_this, uint32_t param_1);
typedef void (__fastcall *playCampaignIntro_ptr)(CAppMenu *_this);
typedef void (__thiscall *videoSequenceStop_ptr)(SF_CUiVideoSequence *_this);
typedef void (__thiscall *startGame_ptr)(CAppMenu *_this, SF_GameInfo *game_info,
                                         uint32_t param_2, uint32_t param_3,
                                         uint32_t param_4, uint32_t coord,
                                         SF_String *param_6);

typedef SF_String *(__thiscall *getSavePath_real_ptr)(SF_String *_this, int campaign_type);

typedef char **(__cdecl *getBasePathString_ptr)(void *out_string_obj);

extern SFSF_CampaignDef g_campaigns[SFSF_MAX_CAMPAIGNS];
extern uint32_t g_campaign_count;
extern int32_t g_active_custom_campaign; /**< -1 = vanilla / none active */
extern CAppMenu *g_campaign_app_menu;    /**< Stashed by sf_menu_hook on menu build untested and probably will crash us */

void initialize_campaign_hooks();

/** Registers a campaign definition. Returns campaign index or -1. */
int32_t register_campaign(const SFSF_CampaignDef *def);

/**
 * Lifecycle reset - MUST be called from sf_menu_hook every time the main menu
 * as this restores the swapped save-dir globals and clears the active
 * campaign.
 */
void campaign_hook_on_main_menu(CAppMenu *app_menu);

/** Main-menu button callback - builds and toggles the campaign screen. */
void __thiscall show_custom_campaign_screen(CMnuSmpButton *_this);

/** Per-campaign button callback. */
void __thiscall on_campaign_selected(CMnuSmpButton *_this);

/** @} */
#endif // SF_CAMPAIGN_HOOK_H
