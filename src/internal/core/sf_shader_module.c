/**
 * @addtogroup ShaderModule
 * @{
 */

#include <windows.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sf_shader_module.h"
#include "sf_shader_loader.h"
#include "sf_hooks.h"           /* createModInfo */
#include "sf_building_loader.h" /* readfile() */
#include "sf_wrappers.h"

#include "../registry/sf_error_registry.h"
#include "../registry/sf_registry.h"

SFMod *g_shader_mod = NULL;

static ShaderEntry s_entries[MAX_SHADER_ENTRIES];
static int s_entry_count = 0;

void report_shader_error(const char *format, ...)
{
    char buffer[512];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    report_mod_error(g_shader_mod, "%s", buffer);
}

/** @brief Builds sfsf\shaders\<file>. Shaders live on disk, not in the game VFS. */
static void build_shader_path(const char *file, char *out, size_t out_size)
{
    char current_dir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, current_dir);
    snprintf(out, out_size, "%s\\sfsf\\shaders\\%s", current_dir, file);
}

static bool shader_file_exists(const char *file)
{
    char path[MAX_PATH];
    build_shader_path(file, path, sizeof(path));
    FILE *handle = fopen(path, "rb");
    if (handle == NULL)
    {
        return false;
    }
    fclose(handle);
    return true;
}

static bool accept_shader_entry(const ShaderEntry *entry)
{
    if (entry->name[0] == '\0')
    {
        report_mod_error(g_shader_mod, "shaders.json: entry with no \"name\"");
        return false;
    }

    if (entry->fragment_file[0] == '\0')
    {
        report_mod_error(g_shader_mod, "shaders.json: \"%s\" has no \"fragment\" file", entry->name);
        return false;
    }

    for (int i = 0; i < s_entry_count; i++)
    {
        if (strcmp(s_entries[i].name, entry->name) == 0)
        {
            report_mod_error(g_shader_mod, "shaders.json: \"%s\" is declared twice, keeping the first",
                             entry->name);
            return false;
        }
    }

    if (!shader_file_exists(entry->fragment_file))
    {
        report_mod_error(g_shader_mod, "shaders.json: \"%s\" needs \"%s\", missing from sfsf\\shaders\\",
                         entry->name, entry->fragment_file);
        return false;
    }

    if (entry->vertex_file[0] != '\0' && !shader_file_exists(entry->vertex_file))
    {
        report_mod_error(g_shader_mod, "shaders.json: \"%s\" needs \"%s\", missing from sfsf\\shaders\\",
                         entry->name, entry->vertex_file);
        return false;
    }

    s_entries[s_entry_count] = *entry;
    s_entry_count++;
    return true;
}

bool read_shader_sources(const ShaderEntry *entry, char **out_vertex, char **out_fragment)
{
    char path[MAX_PATH];

    *out_vertex   = NULL;
    *out_fragment = NULL;

    build_shader_path(entry->fragment_file, path, sizeof(path));
    *out_fragment = readfile(path);
    if (*out_fragment == NULL)
    {
        report_shader_error("could not read \"%s\"", entry->fragment_file);
        return false;
    }

    if (entry->vertex_file[0] != '\0')
    {
        build_shader_path(entry->vertex_file, path, sizeof(path));
        *out_vertex = readfile(path);
        if (*out_vertex == NULL)
        {
            report_shader_error("could not read \"%s\"", entry->vertex_file);
            free(*out_fragment);
            *out_fragment = NULL;
            return false;
        }
    }

    return true;
}

const ShaderEntry *get_shader_entries(int *out_count)
{
    *out_count = s_entry_count;
    return s_entries;
}

int get_shader_pass_count()
{
    int count = 0;
    for (int i = 0; i < s_entry_count; i++)
    {
        if (s_entries[i].enabled)
        {
            count++;
        }
    }
    return count;
}

void initialize_shader_module()
{
    char version_tag_buffer[128];
    snprintf(version_tag_buffer, sizeof(version_tag_buffer), "%d.%d.%d-%s",
             SPELLFRAMEWORK_VERSION_MAJOR, SPELLFRAMEWORK_VERSION_MINOR,
             SPELLFRAMEWORK_VERSION_PATCH, SPELLFRAMEWORK_TAG);

    g_shader_mod = createModInfo("Shader Module (experimental)", version_tag_buffer,
                                 "Muddykat",
                                 "Runs HLSL post-process passes listed by sfsf\\shaders.json.\nUse f10 to dump debug info, f11 to cycle shaders (returns to default settings after full cycle) and f8 to recompile shaders");
    clear_mod_errors(g_shader_mod);
    register_mod_for_listing(g_shader_mod, MOD_TYPE_CORE);

    s_entry_count = 0;

    char current_dir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, current_dir);

    char path[MAX_PATH];
    snprintf(path, sizeof(path), "%s\\sfsf\\shaders.json", current_dir);

    ShaderEntry parsed[MAX_SHADER_ENTRIES];
    int parsed_count = 0;

    if (!parse_shaders_json_file(path, parsed, MAX_SHADER_ENTRIES, &parsed_count))
    {
        /* A missing shaders.json is a normal install, not a mod error. */
        return;
    }

    for (int i = 0; i < parsed_count; i++)
    {
        accept_shader_entry(&parsed[i]);
    }

    /* Insertion sort by declared order - list is tiny. */
    for (int i = 1; i < s_entry_count; i++)
    {
        ShaderEntry key = s_entries[i];
        int j = i - 1;
        while (j >= 0 && s_entries[j].order > key.order)
        {
            s_entries[j + 1] = s_entries[j];
            j--;
        }
        s_entries[j + 1] = key;
    }

    log_info("| - Shader Module: %d of %d declared shader(s) accepted",
             s_entry_count, parsed_count);
}

/** @} */
