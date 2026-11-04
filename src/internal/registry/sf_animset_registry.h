/**
 * @addtogroup AnimsetRegistry
 * @{
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Synthetic movie subkeys the animset bridge registers into a movie library
 * (one per shielded pair, at most 16 per library - the only section that
 * consumes them, TZ 7). */
#define SFSF_ANIMSET_SYNTH_FIRST 240
#define SFSF_ANIMSET_SYNTH_LAST 255

/** One shielded[] (synth pair) or solo[] (real subkey) film set of a
 * committed animset record. */
typedef struct
{
    uint16_t weapon;                    /* weapon type id */
    uint8_t anim_count;                 /* 1..8 */
    char anims[8][64];                  /* logical names, no extension; order = variant 0..N-1 */
    bool has_sound;
    char sound_name[64];                /* DrwSound.lua key */
    float sound_offset;                 /* seconds */
    float sound_length;                 /* seconds, 0 = unset */
} AnimsetFilmSetEntry;

/** One committed ranged[] set: aim registers under (71, weapon), shoot under
 * (74, weapon); both phases are mandatory (validated by the module). */
typedef struct
{
    uint16_t weapon;
    uint8_t aim_count;                  /* 1..8 */
    uint8_t shoot_count;                /* 1..8 */
    char aim[8][64];
    char shoot[8][64];
    bool has_sound;                     /* explicit sound lands on BOTH phases */
    char sound_name[64];
    float sound_offset;
    float sound_length;
} AnimsetRangedEntry;

/** One committed animset record (one entries[] element). */
typedef struct
{
    char figure[64];                    /* animset name, informational */
    char base_name[64];                 /* exact movie library match key */
    char source[64];                    /* json file name */
    uint8_t shielded_count;
    uint8_t solo_count;
    uint8_t ranged_count;
    AnimsetFilmSetEntry shielded[64];
    AnimsetFilmSetEntry solo[64];
    AnimsetRangedEntry ranged[64];
} AnimsetEntryRecord;

typedef enum
{
    ANIMSET_COMMIT_OK = 0,
    ANIMSET_COMMIT_CONFLICT /* at least one file was skipped over a base_name conflict */
} AnimsetCommitStatus;

/* Why a whole file was skipped by the catalog commit (unchanged v1 policy). */
typedef enum
{
    ANIMSET_SKIP_INTERNAL_DUPLICATE = 1, /* the same base_name twice inside one file */
    ANIMSET_SKIP_CROSS_FILE_DUPLICATE    /* the base_name also lives in another file */
} AnimsetSkipReason;

/** One per-file skip decision, for the module to log (one line per entry). */
typedef struct
{
    char source[64];      /* json file name that gets skipped */
    char other_source[64];/* conflict counterpart; empty for internal duplicates */
    AnimsetSkipReason reason;
} AnimsetSkipReport;

/** @brief Clears the animset catalog. */
void resetAnimsetRegistry();

/**
 * @brief Commits the animset catalog with the per-file skip policy.
 *
 * A movie library is matched by the exact base_name, so a duplicated
 * base_name must never register. Only the offending files die: a file with an
 * internal duplicate base_name is skipped whole, and when the same base_name
 * appears in two files BOTH files are skipped. Every other file registers
 * normally. One AnimsetSkipReport per skipped file is written to @p out_reports.
 */
AnimsetCommitStatus commitAnimsetCatalog(const AnimsetEntryRecord *entries, size_t count,
                                         size_t *out_registered, size_t *out_skipped,
                                         AnimsetSkipReport *out_reports, size_t reports_cap,
                                         size_t *out_report_count);

/** @brief The single registry sort (by base_name), called once after every scan. */
void finalizeAnimsets();

/** @brief True while no animset record is registered (no folder / empty catalog). */
bool animsetRegistryEmpty();

/**
 * @brief Exact-match lookup by recovered movie library base_name.
 * @return The record whose base_name equals @p base_name, or NULL.
 */
const AnimsetEntryRecord *findAnimsetEntry(const char *base_name);

/** @} */
