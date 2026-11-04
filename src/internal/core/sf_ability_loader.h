/**
 * @addtogroup AbilityLoader
 * @{
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>

#define SFSF_MAX_ABILITY_RECORDS 256

typedef enum
{
    ABILITY_FIELD_SKILL = 0,
    ABILITY_FIELD_SPEC,
    ABILITY_FIELD_LEVEL,
    ABILITY_FIELD_ABILITY_ID,
    ABILITY_FIELD_UNKNOWN,
} AbilityFieldKey;

/**
 * Raw values of one ability record; range validation is done by the module.
 * A field is only meaningful when its found_* flag is set.
 */
typedef struct
{
    int32_t skill;
    int32_t spec;
    int32_t level;
    int32_t ability_id;
    bool found_skill;
    bool found_spec;
    bool found_level;
    bool found_ability_id;
} AbilityJsonRecord;

/**
 * @brief Parses an abilities JSON file (a single array of records) into @p out_records.
 * @return true when the file is structurally valid; individual broken records
 * are skipped with a log_error and do not fail the whole file.
 */
bool parse_ability_json_file(const char *path, AbilityJsonRecord *out_records, int max_records,
                             int *out_count);

/** @} */
