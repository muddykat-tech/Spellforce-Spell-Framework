/**
 * @addtogroup WeaponTypeRegistry
 * @{
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Vanilla weapon type id referenced by the base ranged classification. */
#define KDRWWT_2H_CROSSBOW 17

typedef enum
{
    WEAPON_ATTACK_STYLE_MELEE = 0,
    WEAPON_ATTACK_STYLE_RANGED = 1
} WeaponAttackStyle;

/** @brief One custom weapon type as loaded from sfsf\weapon_types\*.json. */
typedef struct
{
    uint16_t id;
    char name[64];
    WeaponAttackStyle attack_style;
    char source[64];
} WeaponTypeEntry;

typedef enum
{
    WT_COMMIT_OK = 0,
    WT_COMMIT_DUPLICATE_ID,
    WT_COMMIT_DUPLICATE_NAME
} WeaponTypeCommitStatus;

/**
 * @brief Clears the catalog and re-seeds the base ranged classification.
 *
 * After a reset only KDRWWT_2H_CROSSBOW counts as ranged, so the
 * SelectJobAnimation replacement works with or without a catalog.
 */
void resetWeaponTypeRegistry();

/**
 * @brief Atomically activates a whole weapon type catalog.
 *
 * Stricter than the ability registry on purpose: weapon ids land in gamedata
 * and savegames, so a silent last-wins override is not acceptable here. If any
 * two entries share an id or a name, NOTHING is registered, @p out_registered
 * is 0, @p out_skipped is the full record count and the conflicting pair of
 * sources is copied to the out buffers for the caller to report.
 */
WeaponTypeCommitStatus commitWeaponTypeCatalog(const WeaponTypeEntry *entries, size_t count,
                                               size_t *out_registered, size_t *out_skipped,
                                               char *out_first_source, size_t first_source_len,
                                               char *out_second_source, size_t second_source_len);

/** @brief Bounded string copy for WeaponTypeEntry name/source fields. */
void weaponTypeCopyBounded(char *dst, size_t dst_len, const char *src);

/** @brief The single registry sort (by id), called once after every scan. */
void finalizeWeaponTypes();

size_t getWeaponTypeCount();

/** @param index catalog index, ordered by id after finalizeWeaponTypes(). */
bool getWeaponTypeAt(size_t index, WeaponTypeEntry *out);

/**
 * @brief Ranged classification for the SelectJobAnimation reimplementation.
 *
 * The table is {KDRWWT_2H_CROSSBOW} union every catalog id with
 * attack_style == WEAPON_ATTACK_STYLE_RANGED; membership does not pick the
 * combat branch (the job does), it picks crossbow-style animations (own
 * subtype) over the bow base {4,5} inside ranged jobs.
 */
bool isOverridedRangedWeaponType(uint16_t weapon_type);

/** @} */
