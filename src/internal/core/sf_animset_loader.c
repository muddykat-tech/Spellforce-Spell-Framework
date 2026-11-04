/**
 * @addtogroup AnimsetLoader
 * @{
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sf_animset_loader.h"
#include "sf_building_loader.h" /* readfile() */
#include "sf_wrappers.h"

#define JSMN_STATIC
#define JSMN_PARENT_LINKS
#include "jsmn.h"

/* Parse outcome of one section record: a record-local breach drops the
 * RECORD (per-record skip, TZ 4d), a structural breach kills the file. */
enum
{
    PARSE_OK = 0,
    PARSE_RECORD_DROP,
    PARSE_FILE_BROKEN
};

static int jsonint(const char *json, const jsmntok_t *token)
{
    char temp[32];
    int len = token->end - token->start;
    if (len >= (int)sizeof(temp))
    {
        len = (int)sizeof(temp) - 1;
    }
    strncpy(temp, json + token->start, len);
    temp[len] = '\0';
    return atoi(temp);
}

/** Locale-independent float parse (atof honours the CRT locale, the game may
 * have switched it away from "C" by the time the catalog is read). */
static float jsonfloat(const char *json, const jsmntok_t *token)
{
    const char *p = json + token->start;
    const char *end = json + token->end;
    float value = 0.0f;
    bool negative = false;

    if (p < end && (*p == '-' || *p == '+'))
    {
        negative = (*p == '-');
        p++;
    }
    while (p < end && *p >= '0' && *p <= '9')
    {
        value = value * 10.0f + (float)(*p - '0');
        p++;
    }
    if (p < end && *p == '.')
    {
        p++;
        float scale = 0.1f;
        while (p < end && *p >= '0' && *p <= '9')
        {
            value += (float)(*p - '0') * scale;
            scale *= 0.1f;
            p++;
        }
    }
    return negative ? -value : value;
}

static bool json_token_streq(const char *json, const jsmntok_t *token, const char *s)
{
    return token->type == JSMN_STRING &&
           (int)strlen(s) == token->end - token->start &&
           strncmp(json + token->start, s, token->end - token->start) == 0;
}

/** @brief Returns the index just past @p index and everything nested under it. */
static int skip_token_tree(const jsmntok_t *tokens, int token_count, int index)
{
    if (index >= token_count)
    {
        return index;
    }

    if (tokens[index].type == JSMN_OBJECT || tokens[index].type == JSMN_ARRAY)
    {
        int children_count = tokens[index].size;
        int next_index = index + 1;
        for (int i = 0; i < children_count; i++)
        {
            if (next_index >= token_count)
            {
                return token_count;
            }
            if (tokens[index].type == JSMN_OBJECT)
            {
                next_index = skip_token_tree(tokens, token_count, next_index); // key
                next_index = skip_token_tree(tokens, token_count, next_index); // value
            }
            else
            {
                next_index = skip_token_tree(tokens, token_count, next_index);
            }
        }
        return next_index;
    }

    return index + 1;
}

static bool copy_json_string(char *dst, size_t dst_len, const char *json, const jsmntok_t *token)
{
    int len = token->end - token->start;
    if (len >= (int)dst_len)
    {
        return false; /* do not silently truncate: the string lands in engine keys */
    }
    strncpy(dst, json + token->start, len);
    dst[len] = '\0';
    return true;
}

/**
 * @brief Reads one string token and enforces the name length limit.
 * @return 0 on success, PARSE_RECORD_DROP when the value is not a string or
 * longer than the limit (a record-local breach, TZ 4d).
 */
static int parse_name_value_checked(char *dst, size_t dst_len, const char *json, const jsmntok_t *token,
                                    const char *field, const char *path)
{
    if (token->type != JSMN_STRING)
    {
        log_warning("Animset JSON: \"%s\" is not a string, file %s", field, path);
        return PARSE_RECORD_DROP;
    }
    if (!copy_json_string(dst, dst_len, json, token))
    {
        log_warning("Animset JSON: \"%s\" is longer than %d characters, file %s", field,
                    (int)dst_len - 1, path);
        return PARSE_RECORD_DROP;
    }
    return PARSE_OK;
}

/** @brief Parses one sound object. A missing sound object stays all-clear. */
static bool parse_sound_object(const char *json, const jsmntok_t *tokens, int token_count,
                               int object_index, AnimsetSound *out_sound, const char *path)
{
    int pairs_to_process = tokens[object_index].size;
    int pair_index = object_index + 1;

    for (int i = 0; i < pairs_to_process; i++)
    {
        if (pair_index + 1 >= token_count)
        {
            return false;
        }

        const jsmntok_t *key_token = &tokens[pair_index];
        const jsmntok_t *value_token = &tokens[pair_index + 1];

        if (json_token_streq(json, key_token, "name"))
        {
            if (parse_name_value_checked(out_sound->name, sizeof(out_sound->name), json, value_token,
                                         "sound.name", path) != PARSE_OK)
            {
                return false;
            }
            out_sound->found_name = true;
        }
        else if (json_token_streq(json, key_token, "offset"))
        {
            if (value_token->type != JSMN_PRIMITIVE)
            {
                log_warning("Animset JSON: \"sound.offset\" is not a number, file %s", path);
                return false;
            }
            out_sound->offset = jsonfloat(json, value_token);
            out_sound->found_offset = true;
        }
        else if (json_token_streq(json, key_token, "length"))
        {
            if (value_token->type != JSMN_PRIMITIVE)
            {
                log_warning("Animset JSON: \"sound.length\" is not a number, file %s", path);
                return false;
            }
            out_sound->length = jsonfloat(json, value_token);
            out_sound->found_length = true;
        }
        /* Unknown keys are skipped silently so future fields stay backwards compatible. */

        pair_index = skip_token_tree(tokens, token_count, pair_index); // key
        pair_index = skip_token_tree(tokens, token_count, pair_index); // value
    }
    return true;
}

/**
 * @brief Parses one name array (anims / aim / shoot) into @p names.
 * @return PARSE_RECORD_DROP when the record breaches its own limits (>8
 * names, a name >63 characters) - the caller drops the record, not the file.
 */
static int parse_name_array(const char *json, const jsmntok_t *tokens, int token_count,
                            int array_index, char (*names)[SFSF_ANIMSET_NAME_MAX],
                            int32_t *name_count, const char *field, const char *path)
{
    const jsmntok_t *value_token = &tokens[array_index];
    int anims_to_process = value_token->size;
    int anim_index = array_index + 1; /* first array element sits past the array token */
    for (int j = 0; j < anims_to_process; j++)
    {
        if (anim_index >= token_count)
        {
            return PARSE_FILE_BROKEN;
        }
        if (*name_count >= SFSF_ANIMSET_MAX_ANIMS)
        {
            log_warning("Animset JSON: \"%s\" has more than %d names, record skipped, file %s",
                        field, SFSF_ANIMSET_MAX_ANIMS, path);
            return PARSE_RECORD_DROP;
        }
        if (parse_name_value_checked(names[*name_count], sizeof(names[0]), json, &tokens[anim_index],
                                     field, path) != PARSE_OK)
        {
            return PARSE_RECORD_DROP;
        }
        (*name_count)++;
        anim_index = skip_token_tree(tokens, token_count, anim_index);
    }
    return PARSE_OK;
}

/** @brief Parses one shielded[] / solo[] element (weapon, anims, optional sound). */
static int parse_film_set_object(const char *json, const jsmntok_t *tokens, int token_count,
                                 int object_index, AnimsetFilmSet *out_set, const char *path)
{
    int pairs_to_process = tokens[object_index].size;
    int pair_index = object_index + 1;

    for (int i = 0; i < pairs_to_process; i++)
    {
        if (pair_index + 1 >= token_count)
        {
            return PARSE_FILE_BROKEN;
        }

        const jsmntok_t *key_token = &tokens[pair_index];
        const jsmntok_t *value_token = &tokens[pair_index + 1];

        if (json_token_streq(json, key_token, "weapon"))
        {
            if (value_token->type != JSMN_PRIMITIVE)
            {
                log_warning("Animset JSON: \"weapon\" is not a number, file %s", path);
                return PARSE_FILE_BROKEN;
            }
            out_set->weapon = jsonint(json, value_token);
            out_set->found_weapon = true;
        }
        else if (json_token_streq(json, key_token, "anims"))
        {
            if (value_token->type != JSMN_ARRAY)
            {
                log_warning("Animset JSON: \"anims\" is not an array, file %s", path);
                return PARSE_FILE_BROKEN;
            }
            out_set->found_anims = true;
            int status = parse_name_array(json, tokens, token_count, pair_index + 1, out_set->anims,
                                          &out_set->anim_count, "anims", path);
            if (status != PARSE_OK)
            {
                return status;
            }
        }
        else if (json_token_streq(json, key_token, "sound"))
        {
            if (value_token->type != JSMN_OBJECT)
            {
                log_warning("Animset JSON: \"sound\" is not an object, file %s", path);
                return PARSE_FILE_BROKEN;
            }
            if (!parse_sound_object(json, tokens, token_count, pair_index + 1, &out_set->sound, path))
            {
                return PARSE_FILE_BROKEN;
            }
        }
        /* Unknown keys are skipped silently so future fields stay backwards compatible. */

        pair_index = skip_token_tree(tokens, token_count, pair_index); // key
        pair_index = skip_token_tree(tokens, token_count, pair_index); // value
    }
    return PARSE_OK;
}

/** @brief Parses one ranged[] element (weapon, aim, shoot, optional sound). */
static int parse_ranged_set_object(const char *json, const jsmntok_t *tokens, int token_count,
                                   int object_index, AnimsetRangedSet *out_set, const char *path)
{
    int pairs_to_process = tokens[object_index].size;
    int pair_index = object_index + 1;

    for (int i = 0; i < pairs_to_process; i++)
    {
        if (pair_index + 1 >= token_count)
        {
            return PARSE_FILE_BROKEN;
        }

        const jsmntok_t *key_token = &tokens[pair_index];
        const jsmntok_t *value_token = &tokens[pair_index + 1];

        if (json_token_streq(json, key_token, "weapon"))
        {
            if (value_token->type != JSMN_PRIMITIVE)
            {
                log_warning("Animset JSON: \"weapon\" is not a number, file %s", path);
                return PARSE_FILE_BROKEN;
            }
            out_set->weapon = jsonint(json, value_token);
            out_set->found_weapon = true;
        }
        else if (json_token_streq(json, key_token, "aim"))
        {
            if (value_token->type != JSMN_ARRAY)
            {
                log_warning("Animset JSON: \"aim\" is not an array, file %s", path);
                return PARSE_FILE_BROKEN;
            }
            out_set->found_aim = true;
            int status = parse_name_array(json, tokens, token_count, pair_index + 1, out_set->aim,
                                          &out_set->aim_count, "aim", path);
            if (status != PARSE_OK)
            {
                return status;
            }
        }
        else if (json_token_streq(json, key_token, "shoot"))
        {
            if (value_token->type != JSMN_ARRAY)
            {
                log_warning("Animset JSON: \"shoot\" is not an array, file %s", path);
                return PARSE_FILE_BROKEN;
            }
            out_set->found_shoot = true;
            int status = parse_name_array(json, tokens, token_count, pair_index + 1, out_set->shoot,
                                          &out_set->shoot_count, "shoot", path);
            if (status != PARSE_OK)
            {
                return status;
            }
        }
        else if (json_token_streq(json, key_token, "sound"))
        {
            if (value_token->type != JSMN_OBJECT)
            {
                log_warning("Animset JSON: \"sound\" is not an object, file %s", path);
                return PARSE_FILE_BROKEN;
            }
            if (!parse_sound_object(json, tokens, token_count, pair_index + 1, &out_set->sound, path))
            {
                return PARSE_FILE_BROKEN;
            }
        }
        /* Unknown keys are skipped silently so future fields stay backwards compatible. */

        pair_index = skip_token_tree(tokens, token_count, pair_index); // key
        pair_index = skip_token_tree(tokens, token_count, pair_index); // value
    }
    return PARSE_OK;
}

/**
 * @brief Walks one section array (shielded / solo / ranged) into @p sets /
 * @p set_count. Excess records over the section cap are dropped one by one
 * (per-record, TZ 4d); the walk keeps consuming tokens either way.
 */
static int parse_section_array(const char *json, const jsmntok_t *tokens, int token_count,
                               int array_index, int set_stride, void *sets, int32_t *set_count,
                               int32_t max_sets, const char *section, const char *path,
                               int (*parse_one)(const char *, const jsmntok_t *, int, int, void *, const char *),
                               AnimsetCatalogFile *file)
{
    const jsmntok_t *value_token = &tokens[array_index];
    int elements_to_process = value_token->size;
    int element_index = array_index + 1;
    int file_status = PARSE_OK;

    for (int j = 0; j < elements_to_process; j++)
    {
        if (element_index >= token_count || tokens[element_index].type != JSMN_OBJECT)
        {
            log_warning("Animset JSON: \"%s\" element is not an object, file %s", section, path);
            return PARSE_FILE_BROKEN;
        }

        if (*set_count >= max_sets)
        {
            log_warning("Animset JSON: \"%s\" has more than %d records, record skipped, file %s",
                        section, max_sets, path);
            file->dropped_record_count++;
        }
        else
        {
            char *set_ptr = (char *)sets + (size_t)(*set_count) * (size_t)set_stride;
            memset(set_ptr, 0, (size_t)set_stride); /* clear leftovers of a dropped record */
            int status = parse_one(json, tokens, token_count, element_index, set_ptr, path);
            if (status == PARSE_RECORD_DROP)
            {
                file->dropped_record_count++;
            }
            else if (status != PARSE_OK)
            {
                return status;
            }
            else
            {
                (*set_count)++;
            }
        }
        element_index = skip_token_tree(tokens, token_count, element_index);
    }
    return file_status;
}

/* Adapters so parse_section_array can serve both element types. */
static int parse_film_set_adapter(const char *json, const jsmntok_t *tokens, int token_count,
                                  int object_index, void *out_set, const char *path)
{
    return parse_film_set_object(json, tokens, token_count, object_index, (AnimsetFilmSet *)out_set, path);
}

static int parse_ranged_set_adapter(const char *json, const jsmntok_t *tokens, int token_count,
                                    int object_index, void *out_set, const char *path)
{
    return parse_ranged_set_object(json, tokens, token_count, object_index, (AnimsetRangedSet *)out_set, path);
}

/** @brief Parses one entries[] element (figure, base_name, three sections). */
static int parse_entry_object(const char *json, const jsmntok_t *tokens, int token_count,
                              int object_index, AnimsetEntry *out_entry, const char *path,
                              AnimsetCatalogFile *file)
{
    int pairs_to_process = tokens[object_index].size;
    int pair_index = object_index + 1;

    for (int i = 0; i < pairs_to_process; i++)
    {
        if (pair_index + 1 >= token_count)
        {
            return PARSE_FILE_BROKEN;
        }

        const jsmntok_t *key_token = &tokens[pair_index];
        const jsmntok_t *value_token = &tokens[pair_index + 1];

        if (json_token_streq(json, key_token, "figure"))
        {
            if (parse_name_value_checked(out_entry->figure, sizeof(out_entry->figure), json,
                                         value_token, "figure", path) != PARSE_OK)
            {
                return PARSE_FILE_BROKEN;
            }
            out_entry->found_figure = true;
        }
        else if (json_token_streq(json, key_token, "base_name"))
        {
            if (parse_name_value_checked(out_entry->base_name, sizeof(out_entry->base_name), json,
                                         value_token, "base_name", path) != PARSE_OK)
            {
                return PARSE_FILE_BROKEN;
            }
            out_entry->found_base_name = true;
        }
        else if (json_token_streq(json, key_token, "pairs"))
        {
            /* Retired v1 format (TZ 4c): the flag routes the file to the
             * module's exact skip line, the loader adds nothing here. */
            file->rejected_v1_format = true;
            return PARSE_FILE_BROKEN;
        }
        else if (json_token_streq(json, key_token, "shielded"))
        {
            if (value_token->type != JSMN_ARRAY)
            {
                log_warning("Animset JSON: \"shielded\" is not an array, file %s", path);
                return PARSE_FILE_BROKEN;
            }
            int status = parse_section_array(json, tokens, token_count, pair_index + 1,
                                             sizeof(AnimsetFilmSet), out_entry->shielded,
                                             &out_entry->shielded_count,
                                             SFSF_ANIMSET_MAX_SETS_PER_ENTRY, "shielded", path,
                                             parse_film_set_adapter, file);
            if (status != PARSE_OK)
            {
                return status;
            }
        }
        else if (json_token_streq(json, key_token, "solo"))
        {
            if (value_token->type != JSMN_ARRAY)
            {
                log_warning("Animset JSON: \"solo\" is not an array, file %s", path);
                return PARSE_FILE_BROKEN;
            }
            int status = parse_section_array(json, tokens, token_count, pair_index + 1,
                                             sizeof(AnimsetFilmSet), out_entry->solo,
                                             &out_entry->solo_count, SFSF_ANIMSET_MAX_SETS_PER_ENTRY,
                                             "solo", path, parse_film_set_adapter, file);
            if (status != PARSE_OK)
            {
                return status;
            }
        }
        else if (json_token_streq(json, key_token, "ranged"))
        {
            if (value_token->type != JSMN_ARRAY)
            {
                log_warning("Animset JSON: \"ranged\" is not an array, file %s", path);
                return PARSE_FILE_BROKEN;
            }
            int status = parse_section_array(json, tokens, token_count, pair_index + 1,
                                             sizeof(AnimsetRangedSet), out_entry->ranged,
                                             &out_entry->ranged_count,
                                             SFSF_ANIMSET_MAX_SETS_PER_ENTRY, "ranged", path,
                                             parse_ranged_set_adapter, file);
            if (status != PARSE_OK)
            {
                return status;
            }
        }
        /* Unknown keys are skipped silently so future fields stay backwards compatible. */

        pair_index = skip_token_tree(tokens, token_count, pair_index); // key
        pair_index = skip_token_tree(tokens, token_count, pair_index); // value
    }
    return PARSE_OK;
}

bool parse_animset_json_file(const char *path, AnimsetCatalogFile *out_file)
{
    memset(out_file, 0, sizeof(*out_file));

    char *json_string = readfile(path);
    if (json_string == NULL)
    {
        // readfile has already logged the reason.
        return false;
    }

    jsmn_parser parser;
    jsmn_init(&parser);

    size_t json_length = strlen(json_string);
    int token_count = jsmn_parse(&parser, json_string, json_length, NULL, 0);
    if (token_count < 1)
    {
        log_warning("Animset JSON is not valid JSON: %s", path);
        log_debug(DEBUG_HIGH, "%s", json_string);
        free(json_string);
        return false;
    }

    jsmntok_t *tokens = (jsmntok_t *)calloc(token_count, sizeof(jsmntok_t));
    if (tokens == NULL)
    {
        log_error("Unable to allocate %d JSON tokens for: %s", token_count, path);
        free(json_string);
        return false;
    }

    jsmn_init(&parser);
    token_count = jsmn_parse(&parser, json_string, json_length, tokens, token_count);

    bool success = false;
    if (token_count < 1)
    {
        log_warning("Animset JSON failed to tokenize: %s", path);
        log_debug(DEBUG_HIGH, "%s", json_string);
    }
    else if (tokens[0].type != JSMN_OBJECT)
    {
        log_warning("Animset JSON must be a single JSON object: %s", path);
    }
    else
    {
        bool file_broken = false;

        int pairs_to_process = tokens[0].size;
        int pair_index = 1;

        for (int j = 0; j < pairs_to_process; j++)
        {
            if (pair_index + 1 >= token_count)
            {
                file_broken = true;
                break;
            }

            const jsmntok_t *key_token = &tokens[pair_index];
            const jsmntok_t *value_token = &tokens[pair_index + 1];

            if (json_token_streq(json_string, key_token, "entries"))
            {
                if (value_token->type != JSMN_ARRAY)
                {
                    log_warning("Animset JSON: \"entries\" is not an array, file %s", path);
                    file_broken = true;
                }
                else
                {
                    int elements_to_process = value_token->size;
                    /* First array element sits past the key token and the
                     * array token itself (same arithmetic as the section
                     * loops above). */
                    int element_index = pair_index + 2;
                    for (int k = 0; k < elements_to_process; k++)
                    {
                        if (element_index >= token_count ||
                            tokens[element_index].type != JSMN_OBJECT)
                        {
                            log_warning("Animset JSON: \"entries\" element is not an object, file %s",
                                        path);
                            file_broken = true;
                            break;
                        }
                        if (out_file->entry_count >= SFSF_ANIMSET_MAX_ENTRIES)
                        {
                            log_warning("Animset JSON: more than %d entries, file %s",
                                        SFSF_ANIMSET_MAX_ENTRIES, path);
                            file_broken = true;
                            break;
                        }
                        AnimsetEntry *entry = &out_file->entries[out_file->entry_count];
                        int status = parse_entry_object(json_string, tokens, token_count, element_index,
                                                        entry, path, out_file);
                        if (status != PARSE_OK)
                        {
                            file_broken = true;
                            break;
                        }
                        out_file->entry_count++;
                        element_index = skip_token_tree(tokens, token_count, element_index);
                    }
                }
            }
            /* Unknown keys are skipped silently so future fields stay backwards compatible. */

            if (file_broken)
            {
                break;
            }

            pair_index = skip_token_tree(tokens, token_count, pair_index); // key
            pair_index = skip_token_tree(tokens, token_count, pair_index); // value
        }

        success = !file_broken;
    }

    free(tokens);
    free(json_string);
    return success && !out_file->rejected_v1_format;
}

/** @} */
