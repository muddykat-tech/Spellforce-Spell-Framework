/**
 * @addtogroup WeaponTypeLoader
 * @{
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>

#define SFSF_MAX_WEAPON_TYPE_RECORDS 256

/**
 * Raw values of one weapon type record; the sfsf\weapon_types\*.json schema is
 * a single root object { "id", "name", "attack_style" }. Range and charset
 * validation is done by the module. A field is only meaningful when its
 * found_* flag is set.
 */
typedef struct
{
    int32_t id;
    char name[64];
    char attack_style[32];
    bool found_id;
    bool found_name;
    bool found_attack_style;
} WeaponTypeJsonRecord;

/**
 * @brief Parses a weapon type JSON file (a single root object) into @p out_record.
 * @return true when the file is structurally valid; a broken file is skipped
 * whole by the caller. Unknown keys are skipped silently.
 */
bool parse_weapon_type_json_file(const char *path, WeaponTypeJsonRecord *out_record);

/** @} */
