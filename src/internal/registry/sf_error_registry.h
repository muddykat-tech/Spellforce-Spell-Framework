/**
 * @defgroup ErrorRegistry Mod Error and Conflict Registry
 * @ingroup Registry
 * @brief Central bookkeeping for mod errors, warnings and ID conflicts.
  *
 * @addtogroup ErrorRegistry
 * @{
 */

#ifndef SF_ERROR_REGISTRY_H
#define SF_ERROR_REGISTRY_H

#include "../../api/sfsf.h"
#include "../core/sf_wrappers.h"

#include <list>

/**
 * @brief Namespaces for conflict checking.
 *
 */
typedef enum
{
    CONFLICT_SPELL_ID = 0,
    CONFLICT_SPELL_EFFECT_ID,
    CONFLICT_BUILDING_ID,
    CONFLICT_CAMPAIGN_FOLDER,
    CONFLICT_CAMPAIGN_NAME,
    CONFLICT_CAMPAIGN_AVATAR_TYPE,
    CONFLICT_DOMAIN_COUNT
} ConflictDomain;

/**
 * @brief What sort of mod an entry in the mod list is.
 *
 * The order matters: entries are kept grouped by type, lowest first, so the
 * framework always heads the list, core modules follow it, and everything a
 * player dropped in sfsf\ comes after. Within a group, registration order wins.
 */
typedef enum
{
    MOD_TYPE_FRAMEWORK = 0, /**< The Spellforce Spell Framework itself. */
    MOD_TYPE_CORE,          /**< Shipped with the framework, e.g. the Custom Campaign Module. */
    MOD_TYPE_EXTERNAL,      /**< A third party .sfm loaded from sfsf\. */
} ModType;

/** @brief A mod as shown in the in-game mod list. */
typedef struct
{
    SFMod *mod;
    ModType type;
} RegisteredMod;

/** @brief Tag rendered next to a mod in the list. Empty for external mods. */
const char *get_mod_type_label(ModType type);

/**
 * @brief Appends a line to a mod's error buffer without logging it.
 *
 * Used when the log message and the player-facing message need to differ, or
 * when the message belongs to a different mod than the one being processed.
 * The buffer is bounded; once full, further appends are dropped.
 */
void append_mod_error(SFMod *mod, const char *format, ...) LOG_PRINTF_FMT(2, 3);

/**
 * @brief Logs an error, appends it to the mod's error buffer and counts it.
 */
void report_mod_error(SFMod *mod, const char *format, ...) LOG_PRINTF_FMT(2, 3);

/**
 * @brief Logs a warning against a mod at DEBUG_INFO, i.e. always shown.
 *
 * Warnings never touch the mod's error buffer or the error count - they are
 * advisory and must not fail a load.
 */
void report_mod_warning(SFMod *mod, const char *format, ...) LOG_PRINTF_FMT(2, 3);

/**
 * @brief Logs a warning against a mod, shown only at @p level or more verbose.
 *
 * Used for expected-but-noteworthy events during registration, such as a mod
 * taking over a vanilla ID, which are normal enough that they should not be in
 * the log by default.
 */
void report_mod_warning(SFMod *mod, DebugLevel level, const char *format, ...) LOG_PRINTF_FMT(3, 4);

/** @brief Empties a mod's error buffer. */
void clear_mod_errors(SFMod *mod);

/**
 * @brief Claims a numeric ID within a domain for a mod.
 *
 * @param domain      Which namespace to claim in.
 * @param id          The ID being claimed.
 * @param mod         The mod claiming it.
 * @param vanilla_max IDs below this are engine-owned: taking one over is
 *                    reported as a warning rather than a mod conflict. Pass 0
 *                    for domains with no vanilla range.
 * @param id_label    Human readable domain name for the report, e.g. "Spell ID".
 * @return true when the ID was free, false when it was already claimed.
 *
 * @note On conflict the claim still succeeds in the sense that @p mod becomes
 * the recorded owner, e.g the last mod to register will overtake the claim.
 */
bool claim_numeric_id(ConflictDomain domain, uint32_t id, SFMod *mod,
                      uint32_t vanilla_max, const char *id_label);

/**
 * @brief Claims a string key within a domain for a mod. Comparison is
 * case-insensitive, since the keys are Windows paths and menu labels.
 *
 * @return true when the key was free, false when it was already claimed.
 */
bool claim_string_id(ConflictDomain domain, const char *key, SFMod *mod,
                     const char *id_label);

/** @brief Drops every claim in a domain. Intended for reloads and tests. */
void reset_conflict_domain(ConflictDomain domain);

/**
 * @brief Adds a mod to the list rendered by the in-game mod list screen.
 *
 * Registration is idempotent. Mods that register no spells (the Custom
 * Campaign Module, for instance) rely on this to be visible at all.
 */
void register_mod_for_listing(SFMod *mod, ModType type);

/**
 * @brief The mods known to the framework, grouped framework -> core -> external.
 *
 * Ordering is enforced on insertion, so the list is correct no matter what
 * order the subsystems happen to register in.
 */
const std::list<RegisteredMod> &get_registered_mods();

/** @} */
#endif // SF_ERROR_REGISTRY_H
