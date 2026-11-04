/**
 * @addtogroup MovieLibBridge
 * @{
 */

#pragma once
#include <stdint.h>

/**
 * @brief Resolves the engine callees used by the animset bridge.
 *
 * Called once from install_select_job_animation_hook(), before the call-site
 * redirect goes live. Installs no hooks of its own and logs nothing.
 */
void install_movie_lib_bridge();

/**
 * @brief Staging job of a weapon+shield pair (full v2 jobMap, TZ 5.1).
 *
 * 2 (Hand) / 19 (Claw) -> kGdJobPunch; 3 (1HDagger) -> kGdJobStab; custom ids
 * of the weapon_types catalog follow their attack_style (stab -> Stab,
 * punch -> Punch, strike/staff/ranged and unknown ids -> Strike); every other
 * supported weapon -> kGdJobStrike (vanilla jump table 0x7DBB74 semantics).
 */
uint16_t animsetJobForWeapon(uint16_t weapon_type);

/**
 * @brief Looks up the synthetic movie subkey for (movie_lib, weapon_type).
 * @return The subkey 240..255 registered for the pair, or 0 when the library
 * carries no configured pair for this weapon (vanilla behaviour).
 *
 * First touch of a library walks its movie tree, recovers the base_name,
 * matches it against the sfsf\animsets catalog and registers the films
 * through the engine callees (lazy, single game thread).
 */
uint8_t lookupShieldSynth(void *movie_lib, uint16_t weapon_type);

/**
 * @brief Registration point of ALL animset v2 sections (TZ 5.2): find /
 * acquire / configure / verify one movie library.
 *
 * Called from the top of sf_select_job_animation (before the job switch), so
 * every melee, shield and ranged path touches its figure's library exactly
 * once per call and always BEFORE any emission. An empty catalog is a no-op;
 * a cached library of ANY state is re-verified against its tree signature on
 * every call (address reuse after a map reload must not keep a stale
 * verdict); a signature drift triggers a full reconfiguration.
 */
void ensureMovieLibConfigured(void *movie_lib);

/** @brief Ring-eviction counter of the movie lib cache (observability of the
 * TZ 5.2 p.7 policy; printed by the module's summary line as well). */
uint32_t animsetLibCacheEvictionCount();

/** @} */
