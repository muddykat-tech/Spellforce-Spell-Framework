/**
 * @addtogroup ErrorRegistry
 * @{
 */

#include "sf_error_registry.h"

#include "../core/sf_modloader.h"

#include <algorithm>
#include <map>
#include <string>

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static std::map<uint32_t, SFMod *> s_numeric_claims[CONFLICT_DOMAIN_COUNT];
static std::map<std::string, SFMod *> s_string_claims[CONFLICT_DOMAIN_COUNT];
static std::list<RegisteredMod> s_registered_mods;

/**
 * @brief Appends text to a bounded buffer starting at its current terminator.
 */
static void append_to_buffer(char *buffer, size_t buffer_size, const char *format, va_list args)
{
    size_t used = strnlen(buffer, buffer_size);
    if (used + 1 >= buffer_size)
    {
        return;
    }

    vsnprintf(buffer + used, buffer_size - used, format, args);
    buffer[buffer_size - 1] = '\0';
}

static std::string make_key(const char *key)
{
    std::string lowered(key != NULL ? key : "");
    std::transform(lowered.begin(), lowered.end(), lowered.begin(),
                   [](unsigned char c) { return (char)tolower(c); });
    return lowered;
}

void append_mod_error(SFMod *mod, const char *format, ...)
{
    if (mod == NULL)
    {
        return;
    }

    va_list args;
    va_start(args, format);
    append_to_buffer(mod->mod_errors, sizeof(mod->mod_errors), format, args);
    va_end(args);
}

void report_mod_error(SFMod *mod, const char *format, ...)
{
    char message[256];

    va_list args;
    va_start(args, format);
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);

    log_error("| - %s", message);

    if (mod != NULL)
    {
        append_mod_error(mod, "%s\n", message);
    }

    g_error_count = g_error_count + 1;
}

void report_mod_warning(SFMod *mod, const char *format, ...)
{
    (void)mod;

    char message[256];

    va_list args;
    va_start(args, format);
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);

    log_warning_level(DEBUG_INFO, "| - %s", message);
}

void report_mod_warning(SFMod *mod, DebugLevel level, const char *format, ...)
{
    (void)mod;

    char message[256];

    va_list args;
    va_start(args, format);
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);

    log_warning_level(level, "| - %s", message);
}

void clear_mod_errors(SFMod *mod)
{
    if (mod != NULL)
    {
        mod->mod_errors[0] = '\0';
    }
}

bool claim_numeric_id(ConflictDomain domain, uint32_t id, SFMod *mod,
                      uint32_t vanilla_max, const char *id_label)
{
    if (domain >= CONFLICT_DOMAIN_COUNT || mod == NULL)
    {
        return false;
    }

    std::map<uint32_t, SFMod *> &claims = s_numeric_claims[domain];
    std::map<uint32_t, SFMod *>::iterator existing = claims.find(id);
    bool was_free = (existing == claims.end());

    if (!was_free)
    {
        SFMod *previous_owner = existing->second;

        if (id < vanilla_max)
        {
            report_mod_warning(mod, DEBUG_MED,
                               "%s has Overwritten a vanilla %s [%u], this was previously registered by [%s]",
                               mod->mod_id, id_label, id, previous_owner->mod_id);
        }
        else
        {
            log_error("| - Mod Conflict Detected [%s]: %s [%u] is already registered by [%s]",
                      mod->mod_id, id_label, id, previous_owner->mod_id);

            // The error is shown on the mod that LOST the ID
            append_mod_error(previous_owner, "%s [%u] was overwritten by %s\n",
                             id_label, id, mod->mod_id);

            g_error_count = g_error_count + 1;
        }
    }

    claims[id] = mod;
    return was_free;
}

bool claim_string_id(ConflictDomain domain, const char *key, SFMod *mod,
                     const char *id_label)
{
    if (domain >= CONFLICT_DOMAIN_COUNT || mod == NULL || key == NULL)
    {
        return false;
    }

    std::map<std::string, SFMod *> &claims = s_string_claims[domain];
    std::string lookup = make_key(key);
    std::map<std::string, SFMod *>::iterator existing = claims.find(lookup);
    bool was_free = (existing == claims.end());

    if (!was_free)
    {
        SFMod *previous_owner = existing->second;

        log_error("| - Mod Conflict Detected [%s]: %s [%s] is already registered by [%s]",
                  mod->mod_id, id_label, key, previous_owner->mod_id);

        append_mod_error(previous_owner, "%s [%s] was overwritten by %s\n",
                         id_label, key, mod->mod_id);

        g_error_count = g_error_count + 1;
    }

    claims[lookup] = mod;
    return was_free;
}

void reset_conflict_domain(ConflictDomain domain)
{
    if (domain < CONFLICT_DOMAIN_COUNT)
    {
        s_numeric_claims[domain].clear();
        s_string_claims[domain].clear();
    }
}

const char *get_mod_type_label(ModType type)
{
    switch (type)
    {
        case MOD_TYPE_FRAMEWORK: return "[ FRAMEWORK ]";
        case MOD_TYPE_CORE: return "[ CORE MODULE ]";
        default: return " ";
    }
}

void register_mod_for_listing(SFMod *mod, ModType type)
{
    if (mod == NULL)
    {
        return;
    }

    for (const RegisteredMod &known : s_registered_mods)
    {
        if (known.mod == mod)
        {
            return;
        }
    }

    RegisteredMod entry;
    entry.mod = mod;
    entry.type = type;

    /* Insert ahead of the first entry of a later type. Groups therefore stay in
     * framework -> core -> external order while registration order is preserved
     * inside each group, whatever order the subsystems initialise in. */
    std::list<RegisteredMod>::iterator position = s_registered_mods.begin();
    while (position != s_registered_mods.end() && position->type <= type)
    {
        ++position;
    }

    s_registered_mods.insert(position, entry);
}

const std::list<RegisteredMod> &get_registered_mods()
{
    return s_registered_mods;
}

/** @} */
