/**
 * @defgroup ShaderModule SFSF Shader Module
 * @ingroup Core
 * @brief Core mod that owns JSON-declared post-process shaders.
 *
 * @addtogroup ShaderModule
 * @{
 */

#ifndef SHADER_MODULE_H
#define SHADER_MODULE_H

#include "../../api/sfsf.h"
#include "sf_wrappers.h"
#include "sf_shader_loader.h"

#define MAX_SHADER_ENTRIES 16

extern SFMod *g_shader_mod;

/** @brief Loads and validates sfsf\shaders.json. Must run before the first frame. */
void initialize_shader_module();

/** @brief Validated entries, in run order. */
const ShaderEntry *get_shader_entries(int *out_count);

/** @brief Number of enabled entries, for deciding whether to install the hook. */
int get_shader_pass_count();

/**
 * @brief Reads a pass's GLSL off disk. Caller frees both buffers.
 * @param out_vertex NULL when the entry declares no vertex file (use the built-in).
 */
bool read_shader_sources(const ShaderEntry *entry, char **out_vertex, char **out_fragment);

/** @brief report_mod_error() bound to the shader module, callable from the hook. */
void report_shader_error(const char *format, ...) LOG_PRINTF_FMT(1, 2);

/** @} */
#endif // SHADER_MODULE_H
