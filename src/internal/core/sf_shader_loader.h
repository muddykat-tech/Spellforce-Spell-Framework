/**
 * @defgroup ShaderLoader Shader JSON Loader
 * @ingroup Core
 * @brief Parses shaders.json into ShaderEntry records.
 *
 * Parsing only - discovery, validation, compilation and lookup are handled in the
 * Shader Module (sf_shader_module.h).
 *
 * @addtogroup ShaderLoader
 * @{
 */

#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <stdbool.h>
#include <stdint.h>

#define SHADER_NAME_LEN 64
#define SHADER_FILE_LEN 128

/** @brief One shader pass as described by shaders.json. */
typedef struct
{
    char name[SHADER_NAME_LEN];      /**< Lower-cased identifier, used in logs and lookups. */
    char vertex_file[SHADER_FILE_LEN];   /**< Relative to sfsf\shaders\. Empty = built-in passthrough. */
    char fragment_file[SHADER_FILE_LEN]; /**< Relative to sfsf\shaders\. Required. */
    bool enabled;                    /**< Defaults to true when the key is absent. */
    int32_t order;                   /**< Lower runs first. Defaults to declaration order. */
} ShaderEntry;

/**
 * @brief Reads a shaders.json: { "shaders": [ { ... }, ... ] }.
 *
 * @param path        Full path to the file.
 * @param out_entries Caller owned array, filled up to @p max_entries.
 * @param max_entries Capacity of @p out_entries.
 * @param out_count   Number of entries written.
 * @return false when the file is unreadable or is not the expected shape.
 */
bool parse_shaders_json_file(const char *path, ShaderEntry *out_entries,
                             int max_entries, int *out_count);

/** @} */
#endif // SHADER_LOADER_H
