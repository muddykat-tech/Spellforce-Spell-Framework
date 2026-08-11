/**
 * @defgroup CampaignModule SFSF Custom Campaign Module
 * @ingroup Core
 * @brief Core mod that discovers, validates and registers custom campaigns.
 *
 * Custom campaigns are pure data: every .json under sfsf\campaigns\ is loaded
 * here, checked, and handed to register_campaign(). The module owns its own
 * SFMod entry so that a broken campaign shows up in the in-game mod list with a
 * readable reason, exactly like a misbehaving .sfm would.
 *
 * @addtogroup CampaignModule
 * @{
 */

#ifndef CAMPAIGN_MODULE_H
#define CAMPAIGN_MODULE_H

#include "../../api/sfsf.h"

/** The core mod that owns every JSON-defined campaign. */
extern SFMod *g_campaign_mod;

/**
 * @brief Creates the module's mod entry and loads sfsf\campaigns\*.json.
 *
 * Safe to call when the folder does not exist - custom campaigns are simply
 * disabled in that case. Must run before the main menu is built, since
 * sf_menu_hook only offers the campaign button when g_campaign_count > 0.
 */
void initialize_campaign_module();

/** @} */
#endif // CAMPAIGN_MODULE_H
