/**
 * @addtogroup AnimsetLoader
 * @{
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>

#define SFSF_ANIMSET_NAME_MAX 64           /* 63 characters + NUL */
#define SFSF_ANIMSET_MAX_ANIMS 8           /* variants 0..7 of one film set */
#define SFSF_ANIMSET_MAX_SETS_PER_ENTRY 64 /* records per section of one entry */
#define SFSF_ANIMSET_MAX_ENTRIES 64

/**
 * Raw values of the optional shielded[]/solo[]/ranged[].sound object. A field
 * is only meaningful when its found_* flag is set; an absent sound object
 * leaves every flag clear.
 */
typedef struct
{
    char name[SFSF_ANIMSET_NAME_MAX]; /* DrwSound.lua key */
    float offset;                     /* sound start offset, seconds */
    float length;                     /* play range length, seconds (0 = unset) */
    bool found_name;
    bool found_offset;
    bool found_length;
} AnimsetSound;

/** Raw values of one shielded[] / solo[] element (TZ 4b). */
typedef struct
{
    int32_t weapon;
    char anims[SFSF_ANIMSET_MAX_ANIMS][SFSF_ANIMSET_NAME_MAX]; /* order = variant 0..N-1 */
    int32_t anim_count;
    AnimsetSound sound;
    bool found_weapon;
    bool found_anims;
} AnimsetFilmSet;

/** Raw values of one ranged[] element: aim and shoot are two separate film
 * lists; BOTH are mandatory at validation time (owner decision 14.2). */
typedef struct
{
    int32_t weapon;
    char aim[SFSF_ANIMSET_MAX_ANIMS][SFSF_ANIMSET_NAME_MAX];   /* job 71 variants */
    int32_t aim_count;
    char shoot[SFSF_ANIMSET_MAX_ANIMS][SFSF_ANIMSET_NAME_MAX]; /* job 74 variants */
    int32_t shoot_count;
    AnimsetSound sound;
    bool found_weapon;
    bool found_aim;
    bool found_shoot;
} AnimsetRangedSet;

/** Raw values of one entries[] element. */
typedef struct
{
    char figure[SFSF_ANIMSET_NAME_MAX];      /* animset name, for humans and logs */
    char base_name[SFSF_ANIMSET_NAME_MAX];   /* exact movie library match key */
    AnimsetFilmSet shielded[SFSF_ANIMSET_MAX_SETS_PER_ENTRY];
    int32_t shielded_count;
    AnimsetFilmSet solo[SFSF_ANIMSET_MAX_SETS_PER_ENTRY];
    int32_t solo_count;
    AnimsetRangedSet ranged[SFSF_ANIMSET_MAX_SETS_PER_ENTRY];
    int32_t ranged_count;
    bool found_figure;
    bool found_base_name;
} AnimsetEntry;

/**
 * One parsed sfsf\animsets\*.json file ({ "entries": [ ... ] }). The struct is
 * far too large for the stack - heap-allocate it and hand parse_animset_json_file
 * the pointer.
 */
typedef struct
{
    AnimsetEntry entries[SFSF_ANIMSET_MAX_ENTRIES];
    int32_t entry_count;
    /* Record-local limit breaches the loader dropped on the fly (TZ 4d:
     * >8 names per set, >64 sets per section, name >63 characters). The
     * module adds this to its skipped count. */
    int32_t dropped_record_count;
    /* Set when any entry carries the retired v1 "pairs" section: the module
     * then skips the file whole with the exact TZ 4c line. */
    bool rejected_v1_format;
} AnimsetCatalogFile;

/**
 * @brief Parses an animset v2 JSON file (solo / shielded / ranged sections).
 * @return true when the file is structurally valid; a broken file is skipped
 * whole by the caller. Unknown keys are skipped silently. Range, charset and
 * uniqueness validation is done by the module; the loader only drops single
 * records over their own limits (dropped_record_count).
 */
bool parse_animset_json_file(const char *path, AnimsetCatalogFile *out_file);

/** @} */
