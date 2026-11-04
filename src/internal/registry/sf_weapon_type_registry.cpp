#include "sf_weapon_type_registry.h"

#include <algorithm>
#include <string.h>
#include <vector>

static std::vector<WeaponTypeEntry> s_weapon_type_registry;

void resetWeaponTypeRegistry()
{
    s_weapon_type_registry.clear();

    /* vanilla base entry — crossbow stays ranged with or without catalog */
    WeaponTypeEntry base;
    base.id = KDRWWT_2H_CROSSBOW;
    weaponTypeCopyBounded(base.name, sizeof(base.name), "2HCrossbow");
    base.attack_style = WEAPON_ATTACK_STYLE_RANGED;
    weaponTypeCopyBounded(base.source, sizeof(base.source), "vanilla");
    s_weapon_type_registry.push_back(base);
}

void weaponTypeCopyBounded(char *dst, size_t dst_len, const char *src)
{
    if (dst == NULL || dst_len == 0)
    {
        return;
    }
    strncpy(dst, src, dst_len - 1);
    dst[dst_len - 1] = '\0';
}

WeaponTypeCommitStatus commitWeaponTypeCatalog(const WeaponTypeEntry *entries, size_t count,
                                               size_t *out_registered, size_t *out_skipped,
                                               char *out_first_source, size_t first_source_len,
                                               char *out_second_source, size_t second_source_len)
{
    *out_registered = 0;
    *out_skipped = 0;
    if (out_first_source != NULL)
    {
        out_first_source[0] = '\0';
    }
    if (out_second_source != NULL)
    {
        out_second_source[0] = '\0';
    }
    if (entries == NULL || count == 0)
    {
        /* An empty catalog is a valid state, not an error. */
        return WT_COMMIT_OK;
    }

    /* Strict conflict policy, intentionally different from the ability
     * registry's last-wins: a weapon id lands in gamedata and savegames, so a
     * silent override is unacceptable. Any duplicate id or name deactivates
     * the whole catalog. */
    for (size_t i = 0; i < count; i++)
    {
        for (size_t j = i + 1; j < count; j++)
        {
            if (entries[i].id == entries[j].id || strcmp(entries[i].name, entries[j].name) == 0)
            {
                WeaponTypeCommitStatus status = (entries[i].id == entries[j].id)
                                                    ? WT_COMMIT_DUPLICATE_ID
                                                    : WT_COMMIT_DUPLICATE_NAME;
                weaponTypeCopyBounded(out_first_source, first_source_len, entries[i].source);
                weaponTypeCopyBounded(out_second_source, second_source_len, entries[j].source);
                *out_skipped = count;
                return status;
            }
        }
    }

    for (size_t i = 0; i < count; i++)
    {
        s_weapon_type_registry.push_back(entries[i]);
    }
    *out_registered = count;
    return WT_COMMIT_OK;
}

void finalizeWeaponTypes()
{
    std::sort(s_weapon_type_registry.begin(), s_weapon_type_registry.end(),
              [](const WeaponTypeEntry &a, const WeaponTypeEntry &b) { return a.id < b.id; });
}

size_t getWeaponTypeCount()
{
    return s_weapon_type_registry.size();
}

bool getWeaponTypeAt(size_t index, WeaponTypeEntry *out)
{
    if (index >= s_weapon_type_registry.size())
    {
        return false;
    }
    *out = s_weapon_type_registry[index];
    return true;
}

bool isOverridedRangedWeaponType(uint16_t weapon_type)
{
    /* The vector is id-sorted by finalizeWeaponTypes() after the catalog
     * commit; before finalize this lookup is not called yet (the hook only
     * runs once the game animates figures, after the Core Module Phase). */
    std::vector<WeaponTypeEntry>::const_iterator it = std::lower_bound(
        s_weapon_type_registry.begin(), s_weapon_type_registry.end(), weapon_type,
        [](const WeaponTypeEntry &entry, uint16_t id) { return entry.id < id; });
    return it != s_weapon_type_registry.end() && it->id == weapon_type &&
           it->attack_style == WEAPON_ATTACK_STYLE_RANGED;
}

bool getWeaponTypeAttackStyle(uint16_t weapon_type, WeaponAttackStyle *out_style)
{
    if (out_style == NULL)
    {
        return false;
    }
    std::vector<WeaponTypeEntry>::const_iterator it = std::lower_bound(
        s_weapon_type_registry.begin(), s_weapon_type_registry.end(), weapon_type,
        [](const WeaponTypeEntry &entry, uint16_t id) { return entry.id < id; });
    if (it == s_weapon_type_registry.end() || it->id != weapon_type)
    {
        return false;
    }
    *out_style = it->attack_style;
    return true;
}
