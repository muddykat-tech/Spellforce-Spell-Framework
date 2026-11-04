#include "sf_animset_registry.h"

#include <algorithm>
#include <string.h>
#include <vector>

static std::vector<AnimsetEntryRecord> s_animset_registry;
static bool s_animset_sorted = false;

void resetAnimsetRegistry()
{
    s_animset_registry.clear();
    s_animset_sorted = false;
}

bool animsetRegistryEmpty()
{
    return s_animset_registry.empty();
}

/* Appends one skip report unless the source is already reported; reports are
 * bounded by the caller-provided buffer, skipping decisions are not. A NULL
 * report buffer or counter makes reporting a silent no-op. */
static void addSkipReport(AnimsetSkipReport *reports, size_t reports_cap, size_t *report_count,
                          const char *source, const char *other_source, AnimsetSkipReason reason)
{
    if (report_count == NULL)
    {
        return;
    }
    for (size_t i = 0; i < *report_count; i++)
    {
        if (strcmp(reports[i].source, source) == 0)
        {
            return;
        }
    }
    if (*report_count >= reports_cap)
    {
        return;
    }
    AnimsetSkipReport *report = &reports[(*report_count)++];
    strncpy(report->source, source, sizeof(report->source) - 1);
    report->source[sizeof(report->source) - 1] = '\0';
    strncpy(report->other_source, other_source != NULL ? other_source : "",
            sizeof(report->other_source) - 1);
    report->other_source[sizeof(report->other_source) - 1] = '\0';
    report->reason = reason;
}

/* Marks @p source as skipped in @p skipped_sources (deduplicated, order kept). */
static void markSourceSkipped(std::vector<const char *> &skipped_sources, const char *source)
{
    for (size_t i = 0; i < skipped_sources.size(); i++)
    {
        if (strcmp(skipped_sources[i], source) == 0)
        {
            return;
        }
    }
    skipped_sources.push_back(source);
}

static bool sourceIsSkipped(const std::vector<const char *> &skipped_sources, const char *source)
{
    for (size_t i = 0; i < skipped_sources.size(); i++)
    {
        if (strcmp(skipped_sources[i], source) == 0)
        {
            return true;
        }
    }
    return false;
}

AnimsetCommitStatus commitAnimsetCatalog(const AnimsetEntryRecord *entries, size_t count,
                                         size_t *out_registered, size_t *out_skipped,
                                         AnimsetSkipReport *out_reports, size_t reports_cap,
                                         size_t *out_report_count)
{
    *out_registered = 0;
    *out_skipped = 0;
    if (out_report_count != NULL)
    {
        *out_report_count = 0;
    }
    if (entries == NULL || count == 0)
    {
        /* An empty catalog is a valid state, not an error. */
        return ANIMSET_COMMIT_OK;
    }

    /* Per-file conflict policy (TZ 5.1 v1.6): the bridge matches a movie
     * library by the exact base_name, so a duplicated base_name must never
     * register, but only the offending files die - a duplicate inside one
     * file skips that file, the same base_name in two files skips both. */
    std::vector<const char *> skipped_sources;
    for (size_t i = 0; i < count; i++)
    {
        for (size_t j = i + 1; j < count; j++)
        {
            if (strcmp(entries[i].base_name, entries[j].base_name) != 0)
            {
                continue;
            }
            if (strcmp(entries[i].source, entries[j].source) == 0)
            {
                markSourceSkipped(skipped_sources, entries[i].source);
                addSkipReport(out_reports, reports_cap, out_report_count, entries[i].source, NULL,
                              ANIMSET_SKIP_INTERNAL_DUPLICATE);
            }
            else
            {
                markSourceSkipped(skipped_sources, entries[i].source);
                markSourceSkipped(skipped_sources, entries[j].source);
                addSkipReport(out_reports, reports_cap, out_report_count, entries[i].source,
                              entries[j].source, ANIMSET_SKIP_CROSS_FILE_DUPLICATE);
                addSkipReport(out_reports, reports_cap, out_report_count, entries[j].source,
                              entries[i].source, ANIMSET_SKIP_CROSS_FILE_DUPLICATE);
            }
        }
    }

    for (size_t i = 0; i < count; i++)
    {
        if (sourceIsSkipped(skipped_sources, entries[i].source))
        {
            (*out_skipped)++;
            continue;
        }
        s_animset_registry.push_back(entries[i]);
        (*out_registered)++;
    }
    return skipped_sources.empty() ? ANIMSET_COMMIT_OK : ANIMSET_COMMIT_CONFLICT;
}

void finalizeAnimsets()
{
    std::sort(s_animset_registry.begin(), s_animset_registry.end(),
              [](const AnimsetEntryRecord &a, const AnimsetEntryRecord &b)
              { return strcmp(a.base_name, b.base_name) < 0; });
    s_animset_sorted = true;
}

const AnimsetEntryRecord *findAnimsetEntry(const char *base_name)
{
    /* The vector is base_name-sorted by finalizeAnimsets() after the catalog
     * commit; before finalize this lookup is not called yet (the bridge only
     * runs once the game animates figures, after the Core Module Phase). */
    if (!s_animset_sorted)
    {
        return NULL;
    }

    std::vector<AnimsetEntryRecord>::const_iterator it = std::lower_bound(
        s_animset_registry.begin(), s_animset_registry.end(), base_name,
        [](const AnimsetEntryRecord &entry, const char *name) { return strcmp(entry.base_name, name) < 0; });
    if (it == s_animset_registry.end() || strcmp(it->base_name, base_name) != 0)
    {
        return NULL;
    }
    return &*it;
}
