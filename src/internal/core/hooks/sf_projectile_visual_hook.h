#pragma once

#include <stdint.h>
#include <stdbool.h>

/* Clone anchor ring (wave 19).
 *
 * cloneEffect registers one entry per reflected projectile CLONE; the
 * 8AA2F4 site wrapper (see sf_projectile_visual_hook.c) looks the record
 * id up in this ring and swaps the rendered arc's source anchor (a4) to
 * the reflector. The engine resolver inside FUN_008aa610 is STATELESS: it
 * re-resolves the a4 key from the record on EVERY rendered frame (research
 * O-F3), so a consume-once substitution collapsed the arc back to the
 * attacker after the first frame (wave-17 regression, research VERDICT).
 * The entry therefore stays live for the WHOLE flight: the client renders
 * frames cs in [start, start+length] (research O-F5), expire_cs is
 * tick_start + tick_count (research O-F1), and pvfx_anchor_match does NOT
 * consume — expired entries are swept instead, both in match and on put
 * (wave 21). Engine precedent: vf15's
 * kGdSpellLineRockBulletChained branch rebuilds its anchor key every frame
 * the same way.
 *
 * Pure logic, no engine dependencies: the unit harness in the task contour
 * compiles this header standalone. */
#define PVFX_ANCHOR_RING_CAPACITY 8

typedef struct
{
    uint16_t clone_eid;   /* GD effect record id of the reflected clone */
    uint16_t donor;       /* donor figure id (diagnostics: K12 template source) */
    uint16_t reflector;   /* figure the rendered arc starts from */
    uint32_t expire_cs;   /* last flight frame (tick_start + tick_count) */
    bool logged;          /* anchor diagnostics line already printed */
} PvfxAnchorEntry;

typedef struct
{
    PvfxAnchorEntry entries[PVFX_ANCHOR_RING_CAPACITY]; /* live entries [0..count) */
    uint8_t count;
} PvfxAnchorRing;

/* Registers the clone's anchor substitution until expire_cs. A zero clone
 * eid is refused; flights already over at now_cs are swept first; a
 * re-registration of a live eid replaces the entry (latest wins, re-arms
 * the one-shot log latch); a still-full ring drops the entry with the
 * EARLIEST expire_cs, so the longest remaining flight keeps its anchor. */
static inline bool pvfx_anchor_put(PvfxAnchorRing *ring, uint16_t clone_eid, uint16_t donor,
                                   uint16_t reflector, uint32_t expire_cs, uint32_t now_cs)
{
    uint8_t i;
    uint8_t j;
    uint8_t victim;
    if (clone_eid == 0)
    {
        return false;
    }
    /* Sweep flights already over (expire_cs < now_cs): reclaim capacity
     * before any eviction, so a dead entry never displaces a live one. */
    for (i = 0; i < ring->count;)
    {
        if (now_cs > ring->entries[i].expire_cs)
        {
            for (j = i + 1; j < ring->count; j++)
            {
                ring->entries[j - 1] = ring->entries[j];
            }
            ring->count--;
            continue;
        }
        i++;
    }
    for (i = 0; i < ring->count; i++)
    {
        if (ring->entries[i].clone_eid == clone_eid)
        {
            ring->entries[i].donor = donor;
            ring->entries[i].reflector = reflector;
            ring->entries[i].expire_cs = expire_cs;
            ring->entries[i].logged = false;
            return true;
        }
    }
    if (ring->count < PVFX_ANCHOR_RING_CAPACITY)
    {
        ring->entries[ring->count].clone_eid = clone_eid;
        ring->entries[ring->count].donor = donor;
        ring->entries[ring->count].reflector = reflector;
        ring->entries[ring->count].expire_cs = expire_cs;
        ring->entries[ring->count].logged = false;
        ring->count++;
        return true;
    }
    victim = 0;
    for (i = 1; i < PVFX_ANCHOR_RING_CAPACITY; i++)
    {
        if (ring->entries[i].expire_cs < ring->entries[victim].expire_cs)
        {
            victim = i;
        }
    }
    for (j = victim; j < PVFX_ANCHOR_RING_CAPACITY - 1; j++)
    {
        ring->entries[j] = ring->entries[j + 1];
    }
    ring->entries[PVFX_ANCHOR_RING_CAPACITY - 1].clone_eid = clone_eid;
    ring->entries[PVFX_ANCHOR_RING_CAPACITY - 1].donor = donor;
    ring->entries[PVFX_ANCHOR_RING_CAPACITY - 1].reflector = reflector;
    ring->entries[PVFX_ANCHOR_RING_CAPACITY - 1].expire_cs = expire_cs;
    ring->entries[PVFX_ANCHOR_RING_CAPACITY - 1].logged = false;
    return true;
}

/* NON-consuming lookup by clone eid: true while cs <= expire_cs, so every
 * flight frame of the clone re-resolves the a4 anchor. Entries whose flight
 * is over (cs > expire_cs) are swept first; on refusal *reflector_out is
 * untouched. */
static inline bool pvfx_anchor_match(PvfxAnchorRing *ring, uint16_t clone_eid, uint32_t cs,
                                     uint16_t *reflector_out)
{
    uint8_t i;
    uint8_t j;
    if (clone_eid == 0)
    {
        return false;
    }
    for (i = 0; i < ring->count;)
    {
        if (cs > ring->entries[i].expire_cs)
        {
            for (j = i + 1; j < ring->count; j++)
            {
                ring->entries[j - 1] = ring->entries[j];
            }
            ring->count--;
            continue;
        }
        i++;
    }
    for (i = 0; i < ring->count; i++)
    {
        if (ring->entries[i].clone_eid == clone_eid)
        {
            *reflector_out = ring->entries[i].reflector;
            return true;
        }
    }
    return false;
}

/* One-shot latch for the anchor diagnostics line: returns true exactly
 * once per live entry (the first call marks it logged and reports the
 * registered donor). */
static inline bool pvfx_anchor_log_once(PvfxAnchorRing *ring, uint16_t clone_eid, uint16_t *donor_out)
{
    uint8_t i;
    for (i = 0; i < ring->count; i++)
    {
        if (ring->entries[i].clone_eid == clone_eid && !ring->entries[i].logged)
        {
            ring->entries[i].logged = true;
            *donor_out = ring->entries[i].donor;
            return true;
        }
    }
    return false;
}

/* Called from cloneEffect (sf_phys_effect_hook.c) after the K12 source-pair
 * copy: queues the clone's anchor substitution for every flight frame until
 * expire_cs = tick_start + tick_count; now_cs is the engine current_step and
 * drives the put-side sweep and earliest-deadline eviction. */
void pvfx_register_anchor(uint16_t clone_eid, uint16_t donor, uint16_t reflector, uint32_t expire_cs,
                          uint32_t now_cs);

/* Installs the 8AA2F4 anchor-site wrapper; called from sf_hooks.c. */
void initialize_projectile_visual_hooks();
