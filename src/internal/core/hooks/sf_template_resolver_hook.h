/* Whole-function replacement of the client template resolver FUN_007e0aa0
 * (sf-resolver-hook). Declares the resolver key layout and the pure,
 * standalone-compilable decision helpers (unit harness lives outside the
 * shipped tree); the engine-side replacement and its
 * install routine live in sf_template_resolver_hook.c. */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Vanilla key dword, byte-verified in the resolver prologue
 * (0x7E0AB3 mov edi,[ebp+8] / 0x7E0AC0 mov ecx,[edi] / 0x7E0AC2 movzx eax,cl:
 * cat = byte at +0, eid = word at +1). The +3 byte IS read by the resolver's
 * cat6 branch (0x7E0EDF mov esi,ecx / shr esi,8 - no movzx) and lands in
 * bits 16..23 of the search key; rtfx_pack_key zero-fills it, so cat6 goes
 * through rtfx_load_key_dword (the raw dword read) instead. */
typedef struct __attribute__((packed))
{
    uint8_t cat;   /* +0x0: render category (disasm 0x7E0AC2 byte[edi]) */
    uint16_t eid;  /* +0x1: record id (disasm 0x7E0AD6 shr ecx,8) */
} SF_ResolverKey;

static_assert(offsetof(SF_ResolverKey, cat) == 0,
              "SF_ResolverKey.cat must sit at +0x0 (disasm 0x7E0AC0 movzx eax,cl)");
static_assert(offsetof(SF_ResolverKey, eid) == 1,
              "SF_ResolverKey.eid must sit at +0x1 (disasm 0x7E0AD6 shr ecx,8)");
static_assert(sizeof(SF_ResolverKey) == 3,
              "SF_ResolverKey must be 3 bytes (packed cat+eid, no padding)");

/* Per-unit threshold of the projectile cascade: cmp word[..+0x28], 0x4C7 / jbe
 * at 0x7E0E2A..0x7E0E2F (unsigned). udid above it takes the vanilla per-unit
 * branch; at or below it the additive override applies. */
#define RTFX_PERUNIT_MAX_UNIT_DATA_ID 1223u

/* Effect-record render category of the resolver map (subtree lib + 4*8 ==
 * lib + 0x20, disasm 0x7E0EF2 / 0x7E0F24 - same subtree). */
#define RTFX_CAT_EFFECT 4u

/* kGdEffectProjectile (0x7E0DB8 cmp bh, 0x18). */
#define RTFX_KGD_EFFECT_PROJECTILE 0x18u

/* Packs the key into the vanilla scratch dword layout [cat][eid_lo][eid_hi][0]
 * (the layout FUN_0086ec60 decodes: cat = *key & 0xff, eid = *key >> 8). */
static inline uint32_t rtfx_pack_key(const SF_ResolverKey *key)
{
    return (uint32_t)key->cat | ((uint32_t)key->eid << 8);
}

/* Inverse of rtfx_pack_key. */
static inline SF_ResolverKey rtfx_unpack_key(uint32_t packed)
{
    SF_ResolverKey key;
    key.cat = (uint8_t)(packed & 0xFFu);
    key.eid = (uint16_t)((packed >> 8) & 0xFFFFu);
    return key;
}

/* Raw key dword exactly as the engine loads it (0x7E0AC0 mov ecx,[edi]):
 * all 4 bytes at the key pointer, NOT the pack output - the +3 byte must
 * survive into bits 16..23 of the cat6 search key (review R-2). */
static inline uint32_t rtfx_load_key_dword(const SF_ResolverKey *key)
{
    return *(const uint32_t *)(const void *)key;
}

/* cat3 variant byte (0x7E0C61 cvttss2si + 0x7E0C65 movzx esi,al): the scaled
 * product is truncated to int and then to u8, exactly like vanilla (review
 * R-1) - bits above 7 are dropped, no full-int key bits. */
static inline uint32_t rtfx_cat3_variant(float product)
{
    return (uint32_t)(uint8_t)(int)product;
}

/* Pure decision core of the projectile line cascade (the single
 * additive delta of the hook):
 *   hero_flag != 0           -> eq3_line (ranged equipment slot 3 item id);
 *   udid > 1223              -> udid (vanilla per-unit branch);
 *   udid <= 1223, slot found -> udid (ADDITIVE per-unit override);
 *   otherwise                -> ws1_type (vanilla weapon_stats[1].wpn_type).
 * eq3_line is the hero branch value passed in by the caller. */
static inline uint16_t rtfx_projectile_line(uint16_t udid, int hero_flag, uint16_t eq3_line,
                                            uint16_t ws1_type, int per_unit_found)
{
    if (hero_flag != 0)
    {
        return eq3_line;
    }
    if (udid > RTFX_PERUNIT_MAX_UNIT_DATA_ID)
    {
        return udid;
    }
    if (per_unit_found != 0)
    {
        return udid;
    }
    return ws1_type;
}

/* Installs the whole-function E9 detour on the resolver entry
 * (RVA 0x3E0AA0). Called from initialize_beta_hooks(). */
void initialize_template_resolver_hooks(void);
