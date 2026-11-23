/* Whole-function replacement of the client template resolver FUN_007e0aa0
 * (sf-resolver-hook).
 *
 * FUN_007e0AA0 (VA 0x7E0AA0..0x7E0FBD, __thiscall, args (key*, strict),
 * ret 8) picks the visual template {cat, eid} for every render consumer
 * (17 call sites, all through the entry).
 * This file reimplements the vanilla body 1:1 from the byte-verified decode
 * and adds exactly ONE semantic delta: the additive per-unit projectile
 * override. Vanilla delta is zero: no vanilla registration ever creates a
 * (24, udid <= 1223) slot.
 *
 * Engine structures/functions are touched only through VA typedef calls and
 * byte-verified packed views; the resolver map helpers
 * are called by VA, so their exact (and partially undecoded) semantics are
 * preserved bit-for-bit.
 *
 * The E9 detour is written during framework initialization, before the
 * render loop starts (SFSF hook pattern) - no execution races on the
 * patched bytes. */

#include "sf_template_resolver_hook.h"

#include "../../../asi/sf_asi.h"
#include "../sf_wrappers.h"
#include "../../../api/structures/sf_figure_structures.h"

#include <stddef.h>
#include <stdint.h>

/* ---- engine callees, resolved at install time --------------------------
 * Conventions byte-verified against the resolver call sites and the
 * *.dec.c decompilations. RVA literals follow the initialize_vanilla_fix_hooks
 * convention. */

/* 0-arg global getters: they read the CGdMain holder global only (dec:
 * GetCGdBuilding/GetCGdFigure/getCGdObject/getGdEffectPtr/FUN_007b1770),
 * ecx and any caller-pushed stack args are left untouched - __cdecl. */
typedef void *(__cdecl *rtfx_get_ptr)(void);

static rtfx_get_ptr rtfx_get_cgdbuilding;    /* VA 0x7B0FD0 */
static rtfx_get_ptr rtfx_get_cgfigure;       /* VA 0x7B1150 */
static rtfx_get_ptr rtfx_get_cgobject;       /* VA 0x7B12D0 */
static rtfx_get_ptr rtfx_get_effect_ptr;     /* VA 0x7B10D0 */
static rtfx_get_ptr rtfx_get_holder;         /* VA 0x7B1770 (CGdMain holder) */

typedef int (__thiscall *rtfx_is_flag_ptr)(void *figures, uint16_t figure_id); /* ret 4 */

static rtfx_is_flag_ptr rtfx_is_mainchar;    /* VA 0x679DA0 CGdFigure::IsMainChar */
static rtfx_is_flag_ptr rtfx_is_hero;        /* VA 0x5BE500 CGdFigure::IsHero */

typedef uint16_t (__thiscall *rtfx_get_equipment_ptr)(void *figures, uint16_t figure_id,
                                                      uint16_t slot); /* ret 8 */

static rtfx_get_equipment_ptr rtfx_get_equipment; /* VA 0x6B26A0 CGdFigure::GetEquipment */

/* REUSE from sf_weapon_type_hook.c (same VA 0x6B3050, ret 0xC); resolved in
 * install_select_job_animation_hook() during the data-hooks phase, which
 * initialize_framework() runs before initialize_beta_hooks(). */
typedef void *(__thiscall *rtfx_get_weapon_stats_ptr)(void *self, void *out, uint16_t figure_id,
                                                      uint8_t slot);
extern rtfx_get_weapon_stats_ptr getWeaponStatsInSlot; /* sf_weapon_type_hook.c:34 */

typedef uint32_t (__thiscall *rtfx_xdata_kind_ptr)(void *effects, uint16_t eid, int kind); /* ret 8 */

static rtfx_xdata_kind_ptr rtfx_has_xdata;   /* VA 0x6DE790 CGdEffect::hasXData */
static rtfx_xdata_kind_ptr rtfx_get_xdata;   /* VA 0x6DD730 (word[eff+eid*2C+5A]) */

typedef uint8_t (__thiscall *rtfx_get_race_ptr)(void *buildings, uint16_t building_id); /* ret 4 */

static rtfx_get_race_ptr rtfx_get_race;      /* VA 0x6C6820 CGdBuilding::GetRace */

typedef void *(__thiscall *rtfx_get_resource_ptr)(void *gd_main); /* 0-arg getter, ret */

static rtfx_get_resource_ptr rtfx_get_gd_resource; /* VA 0x677441 CGdMain::GetGdResource */

typedef uint16_t (__thiscall *rtfx_spell_line_ptr)(void *resource, uint16_t spell_id); /* ret 4 */

static rtfx_spell_line_ptr rtfx_get_spell_spellline; /* VA 0x66E100 CGdResource::GetSpellSpellline */

/* Resolver map ops (CDrwObjectLibrary subtrees; value = *(node + 0x14),
 * 0x7E0EFD/0x7E0F4E). Both write the node pointer into *out_node exactly
 * like the vanilla call sites; the caller mirrors the vanilla reads. */
typedef void (__thiscall *rtfx_map_find_ptr)(void *subtree, void **out_node, uint32_t *key); /* ret 8 */

static rtfx_map_find_ptr rtfx_map_find;          /* VA 0x7DFDB0 tail exact step */
static rtfx_map_find_ptr rtfx_map_find_or_insert; /* VA 0x7DFE40 ladder steps */

/* float scale of the cat3 variant block: thiscall(lib, key-pointer), result
 * in st(0) (fstp dword at 0x7E0C34); the precise xdata-amount formula stays
 * unknown - called by VA. The argument is the raw key pointer (the
 * value of [ebp-0xc] at 0x7E0C2A); its type stays void* here because the
 * packed SF_ResolverKey is not 4-byte aligned and the callee reads the dword
 * itself. */
typedef float (__thiscall *rtfx_scale_ptr)(void *lib, const void *key); /* ret 4 */

static rtfx_scale_ptr rtfx_object_scale;     /* VA 0x7E1020 */

/* FUN_0086dfd0 is __fastcall(uint *key) - the key POINTER travels in
 * ecx (site 0x7E0AFF mov ecx,edi; no push; eax at the call holds the variant
 * dword, which is NOT an argument - MS fastcall passes args in ecx/edx
 * only). Returns the byte result in al (FUN_0086ec60 decodes *key:
 * cat = &0xff, eid = >>8). void* for the same packed-alignment reason. */
typedef uint32_t (__fastcall *rtfx_key_group_ptr)(const void *key);

static rtfx_key_group_ptr rtfx_key_group_byte; /* VA 0x86DFD0 */

/* FUN_007b13d0 is __fastcall(holder) -> *(holder + 0x35c) (gd_main);
 * the push esi at 0x7E0CF6 belongs to GetSpellSpellline. */
typedef uint32_t (__fastcall *rtfx_holder_gd_main_ptr)(uint32_t holder);

static rtfx_holder_gd_main_ptr rtfx_holder_gd_main; /* VA 0x7B13D0 */

/* ---- byte-verified record views ---------------------------------------- */

/* Building record, stride 0x34 (imul 0x7E0AE1), race byte +0xA (0x7E0AE4). */
typedef struct __attribute__((packed))
{
    uint8_t gap_0x00[0xA]; /* unmapped */
    uint8_t race;          /* +0x0A: race byte (cat2 line) */
} RtfxBuildingSlot;

static_assert(offsetof(RtfxBuildingSlot, race) == 0xA,
              "RtfxBuildingSlot.race must sit at +0xA (disasm 0x7E0AE4 byte[esi+eax+0xa])");

/* Figure record, stride 0x2B3 (imul 0x7E0AF9/0x7E0E21): variant dword +0x16
 * (0x7E0B01) and unit_data_id +0x28 (0x7E0E2A). The SFSF GdFigure layout
 * puts unit_data_id at +0x26 - the engine reads +0x28, the +2 compensation
 * is canonical (same read as sf_onhit_hook hardcode units). */
typedef struct __attribute__((packed))
{
    uint8_t gap_0x00[0x16];    /* unmapped */
    uint32_t variant_dword;    /* +0x16: >>0xB & 0xFFFFFF01 -> cat1 variant bits */
    uint8_t gap_0x1A[0x28 - 0x1A]; /* unmapped */
    uint16_t unit_data_id;     /* +0x28: projectile cascade threshold field */
} RtfxFigureSlot;

static_assert(offsetof(RtfxFigureSlot, variant_dword) == 0x16,
              "RtfxFigureSlot.variant_dword must sit at +0x16 (disasm 0x7E0B01 dword[esi+eax+0x16])");
static_assert(offsetof(RtfxFigureSlot, unit_data_id) == 0x28,
              "RtfxFigureSlot.unit_data_id must sit at +0x28 (disasm 0x7E0E2A word[+0x28], +2 compensation)");
static_assert(offsetof(GdFigure, unit_data_id) == 0x26,
              "SFSF GdFigure.unit_data_id must sit at +0x26 (engine +0x28 = +2 compensation)");
static_assert(sizeof(GdFigure) == 0x2B3,
              "GdFigure must match the engine figure stride 0x2B3 (imul at 0x7E0AF9/0x7E0E21)");

/* Object record, stride 0x13 (imul 0x7E0B56/0x7E0EBC): object_data_id word
 * +0x00 (cat3 raw id 0x7E0B59 and the 0x15/0x16 two-step lookup 0x7E0EC2). */
typedef struct __attribute__((packed))
{
    uint16_t object_data_id; /* +0x00 */
} RtfxObjectSlot;

static_assert(offsetof(RtfxObjectSlot, object_data_id) == 0,
              "RtfxObjectSlot.object_data_id must sit at +0x0 (disasm 0x7E0B59 word[ecx+eax])");

/* Effect record, stride 0x2C (imul 0x7E0C94): type byte +0x3C, target pair
 * +0x3E/+0x3F, source pair +0x45/+0x46 - same record offsets as PvfxEffectSlot
 * in sf_projectile_visual_hook.h. */
typedef struct __attribute__((packed))
{
    uint8_t gap_0x00[0x3C];        /* unmapped */
    uint8_t type;                  /* +0x3C: dispatch byte (mov bh, byte at 0x7E0C97) */
    uint8_t gap_0x3D;              /* unmapped */
    uint8_t target_cat;            /* +0x3E */
    uint16_t target_index;         /* +0x3F */
    uint8_t gap_0x41[0x45 - 0x41]; /* unmapped */
    uint8_t source_cat;            /* +0x45 */
    uint16_t source_index;         /* +0x46 */
} RtfxEffectSlot;

static_assert(offsetof(RtfxEffectSlot, type) == 0x3C,
              "RtfxEffectSlot.type must sit at +0x3C (disasm 0x7E0C97 byte[ecx+eax+0x3c])");
static_assert(offsetof(RtfxEffectSlot, target_cat) == 0x3E,
              "RtfxEffectSlot.target_cat must sit at +0x3E (disasm 0x7E0CB2 byte[+0x3e])");
static_assert(offsetof(RtfxEffectSlot, target_index) == 0x3F,
              "RtfxEffectSlot.target_index must sit at +0x3F (disasm 0x7E0D87 word[+0x3f])");
static_assert(offsetof(RtfxEffectSlot, source_cat) == 0x45,
              "RtfxEffectSlot.source_cat must sit at +0x45 (disasm 0x7E0DD1 byte[+0x45])");
static_assert(offsetof(RtfxEffectSlot, source_index) == 0x46,
              "RtfxEffectSlot.source_index must sit at +0x46 (disasm 0x7E0DE5 word[+0x46])");

/* weapon_stats copy: wpn_type word at +0xA (0x7E0E6D word[eax+0xa]). */
static_assert(offsetof(SF_CGdFigureWeaponStats, wpn_type) == 0xA,
              "SF_CGdFigureWeaponStats.wpn_type must sit at +0xA (disasm 0x7E0E6D word[eax+0xa])");

/* Override log cap: per-unit override firings are capped at 30
 * lines like the pvfx probe. */
static uint32_t s_override_log_count = 0;

/* 0x7E0CBB..0x7E0D16, the shared xdata gate: every effect type except 0x14
 * (which has its own GetXData path) lands here - type 3 after its
 * target_cat adjustment, all others straight from the 0x14 miss (jne 0x7e0cbb
 * at 0x7E0D1E). Without xdata(kind 9) the record falls to the 0x15/0x16/
 * 0x1d/0x18 dispatch (je 0x7e0d4a at 0x7E0CD1). Returns 0 when the gate
 * fails, 1 with the assembled key in *key_out. */
static int rtfx_xdata_spellline_key(void *effects, uint16_t eid, uint8_t bh, uint32_t *key_out)
{
    if (rtfx_has_xdata(effects, eid, 9) == 0) /* 0x7E0CCA, kind = 9 */
    {
        return 0;
    }
    uint16_t xdata = rtfx_get_xdata(effects, eid, 9); /* 0x7E0CE2 -> ax */
    void *holder = rtfx_get_holder();                 /* 0x7E0CEA FUN_007b1770 */
    void *gd_main = (void *)rtfx_holder_gd_main((uint32_t)holder); /* 0x7E0CF1 fastcall */
    void *resource = rtfx_get_gd_resource(gd_main);   /* 0x7E0CF9 CGdMain::GetGdResource */
    uint16_t line = rtfx_get_spell_spellline(resource, xdata); /* 0x7E0D00 -> ax */
    *key_out = ((uint32_t)bh << 16) + (uint32_t)line; /* 0x7E0D05..0x7E0D14 */
    return 1;
}

/* ---- the whole-function replacement ------------------------------------ */

/* ABI: void* __thiscall f(void* lib, SF_ResolverKey* key, void* strict),
 * callee cleans the two 4-byte stack arguments (vanilla ret 8 at 0x7E0FBB);
 * returns the template pointer in eax to every consumer. */
void *__thiscall sf_template_resolver(void *lib, SF_ResolverKey *key, void *strict)
{
    /* 0x7E0AB3..0x7E0AC9: key dword -> cat byte + eid word; cats outside
     * [1..6] go straight to the tail with esi = 0 (xor esi,esi 0x7E0AB8). */
    uint8_t cat = key->cat;                  /* movzx eax,cl @0x7E0AC2 */
    uint16_t eid = key->eid;                 /* shr ecx,8 / movzx esi,cx */
    uint32_t key_dword = rtfx_load_key_dword(key); /* raw dword[edi] @0x7E0AC0: the +3 byte survives for cat6 (R-2) */
    uint32_t esi = 0;

    switch (cat)
    {
        case 2: /* 0x7E0AD6 buildings */
        {
            void *base = rtfx_get_cgdbuilding(); /* 0x7E0ADC */
            RtfxBuildingSlot *slot =
                (RtfxBuildingSlot *)((uint8_t *)base + (uint32_t)eid * 0x34u);
            esi = slot->race; /* byte[base + eid*0x34 + 0xA] @0x7E0AE4 */
            break;
        }
        case 1: /* 0x7E0AEE figures */
        {
            void *base = rtfx_get_cgfigure(); /* 0x7E0AF4 */
            RtfxFigureSlot *slot =
                (RtfxFigureSlot *)((uint8_t *)base + (uint32_t)eid * 0x2B3u);
            uint32_t d = (slot->variant_dword >> 0xB) & 0xFFFFFF01u; /* 0x7E0B01..0x7E0B08 */
            /* 0x7E0AFF mov ecx,edi: the KEY pointer is the fastcall argument */
            uint8_t group = (uint8_t)rtfx_key_group_byte(key); /* 0x7E0B10 -> al */
            uint16_t line = rtfx_get_equipment(base, eid, 2); /* 0x7E0B1C..0x7E0B26 slot 2 */
            esi = (uint32_t)line + ((((uint32_t)group << 8) | (uint32_t)(uint8_t)d) << 16);
            break; /* 0x7E0B2B..0x7E0B42 */
        }
        case 3: /* 0x7E0B49 objects */
        {
            void *base = rtfx_get_cgobject(); /* 0x7E0B4F (pure getter; the vanilla
                                               * re-calls it before every range check,
                                               * 0x7E0B61..0x7E0C05 - one call suffices) */
            RtfxObjectSlot *slot =
                (RtfxObjectSlot *)((uint8_t *)base + (uint32_t)eid * 0x13u);
            uint32_t raw = slot->object_data_id; /* movzx edi,word @0x7E0B59 */
            uint32_t bucket;
            if (raw - 0x100u <= 0xFFu)        /* 0x7E0B62 */
            {
                bucket = 8;
            }
            else if (raw - 0x680u <= 0x7Fu)   /* 0x7E0B7B */
            {
                bucket = 8;
            }
            else if (raw - 0x600u <= 0x7Fu)   /* 0x7E0B90 */
            {
                bucket = 8;
            }
            else if (raw - 0x500u <= 0x7Fu)   /* 0x7E0BA5 */
            {
                bucket = 4;
            }
            else if (raw - 0x580u <= 0x7Fu)   /* 0x7E0BBD */
            {
                bucket = 8;
            }
            else if (raw - 0x80u <= 0x7Fu)    /* 0x7E0BCE */
            {
                bucket = 4;
            }
            else if (raw - 0x700u <= 0x7Fu)   /* 0x7E0BE3 */
            {
                bucket = 3;
            }
            else if (raw == 0x300u || raw == 0x302u) /* 0x7E0BFB..0x7E0C1E */
            {
                bucket = 0xBu;
            }
            else
            {
                esi = raw; /* 0x7E0C14: raw id, no variant block */
                break;
            }
            /* 0x7E0C2A: push [ebp-0xc] = the key POINTER value (cat3 never
             * rewrites [ebp-0xc]), ecx = lib; float via fstp. */
            float f = rtfx_object_scale(lib, key); /* 0x7E0C2F -> st(0) */
            float threshold = *(float *)ASI::AddrOf(0x78fe48); /* VA 0xB8FE48 comiss @0x7E0C3C */
            uint32_t variant;
            if (f > threshold)
            {
                /* 0x7E0C45..0x7E0C65: cvtdq2pd(bucket) + rounding-table entry
                 * selected by the bucket sign bit ([sign*8 + 0xB907F0], always
                 * entry 0 for buckets 3/4/8/0xB), *f, cvttss2si truncation.
                 * Each step stored to a float to pin per-op float32 rounding. */
                double rounding =
                    *(double *)ASI::AddrOf(0x7907f0 + 8u * (bucket >> 31)); /* VA 0xB907F0 */
                float scaled = (float)((double)bucket + rounding);
                float product = scaled * f;
                variant = rtfx_cat3_variant(product); /* cvttss2si @0x7E0C61 + movzx esi,al @0x7E0C65: u8 truncate (R-1) */
            }
            else
            {
                variant = 0; /* 0x7E0C75 jbe path */
            }
            esi = (variant << 16) + raw; /* 0x7E0C68..0x7E0C6B add esi,edi */
            break;
        }
        case 4: /* 0x7E0C87 effects */
        {
            void *effects = rtfx_get_effect_ptr(); /* 0x7E0C8D */
            RtfxEffectSlot *slot =
                (RtfxEffectSlot *)((uint8_t *)effects + (uint32_t)eid * 0x2Cu);
            uint8_t bh = slot->type; /* mov bh,byte[+0x3C] @0x7E0C97 */
            if (bh == 3)             /* 0x7E0C9B */
            {
                if (slot->target_cat != 1) /* 0x7E0CB2: byte[+0x3E] */
                {
                    bh = 2; /* 0x7E0CB9: spell-key category drops to 2 */
                }
            }
            else if (bh == 0x14) /* 0x7E0D1B: own GetXData path, no gate */
            {
                uint16_t xdata = rtfx_get_xdata(effects, eid, 9); /* 0x7E0D2F */
                esi = ((uint32_t)bh << 16) + (uint32_t)xdata;     /* 0x7E0D34..0x7E0D43 */
                break;
            }
            /* 0x7E0CBB shared gate (type 3 falls in after the bh fix, every
             * other non-0x14 type jumps here from 0x7E0D1E). */
            uint32_t xkey;
            if (rtfx_xdata_spellline_key(effects, eid, bh, &xkey))
            {
                esi = xkey;
                break;
            }
            /* 0x7E0D4A dispatch, reached only without xdata(kind 9) */
            if (bh == 0x15 || bh == 0x16) /* 0x7E0D4A..0x7E0D56 -> 0x7E0E9A */
            {
                uint16_t pair_index = slot->target_index;   /* word[+0x3F] @0x7E0EAA */
                void *obj_base = rtfx_get_cgobject();       /* 0x7E0EAF */
                uint16_t line =
                    *(uint16_t *)((uint8_t *)obj_base + (uint32_t)pair_index * 0x13u);
                esi = ((uint32_t)bh << 16) + (uint32_t)line; /* 0x7E0EB6..0x7E0EC9 */
                break;
            }
            if (bh == 0x1D) /* 0x7E0D5C */
            {
                if (slot->target_cat != 2) /* 0x7E0D8C */
                {
                    esi = ((uint32_t)bh << 16) + 0u; /* 0x7E0ECD line 0 */
                    break;
                }
                uint16_t bld_id = slot->target_index; /* word[+0x3F] @0x7E0D87 */
                void *bld_base = rtfx_get_cgdbuilding(); /* 0x7E0D96 */
                uint8_t line = rtfx_get_race(bld_base, bld_id); /* 0x7E0D9D -> al */
                esi = ((uint32_t)bh << 16) + (uint32_t)line;
                break;
            }
            if (bh != RTFX_KGD_EFFECT_PROJECTILE) /* 0x7E0DB8 cmp bh, 0x18 */
            {
                esi = ((uint32_t)bh << 16) + 0u; /* 0x7E0ECD: any other type -> line 0 */
                break;
            }
            /* type 0x18: projectile cascade, 0x7E0DB8..0x7E0E98 */
            uint8_t src_cat = slot->source_cat;       /* byte[+0x45] @0x7E0DD1 */
            uint16_t idx = slot->source_index;        /* word[+0x46] @0x7E0DE5 */
            if (src_cat != 1)                          /* 0x7E0DEA */
            {
                esi = ((uint32_t)bh << 16) + 0u;       /* 0x7E0ECD */
                break;
            }
            void *fig_base = rtfx_get_cgfigure();      /* 0x7E0DF4 */
            uint16_t line;
            if (rtfx_is_mainchar(fig_base, idx) ||     /* 0x7E0DFB */
                rtfx_is_hero(fig_base, idx))           /* 0x7E0E0C */
            {
                /* 0x7E0E78: hero/mainchar -> ranged equipment slot 3 item id */
                line = rtfx_get_equipment(fig_base, idx, 3); /* 0x7E0E82, push 3 */
            }
            else
            {
                RtfxFigureSlot *fig =
                    (RtfxFigureSlot *)((uint8_t *)fig_base + (uint32_t)idx * 0x2B3u);
                uint16_t udid = fig->unit_data_id; /* word[+0x28] @0x7E0E2A */
                if (udid <= RTFX_PERUNIT_MAX_UNIT_DATA_ID) /* 0x7E0E2A cmp 0x4C7 / jbe */
                {
                    /* === THE ADDITIVE OVERRIDE (the only semantic delta):
                     * exact-lookup (24 << 16 | udid) in the cat4 subtree lib+4*8
                     * (== lib+0x20) via the tail's find (0x7DFDB0), WITHOUT
                     * any ladder and WITHOUT any strict comparison - the tail's
                     * exact step itself returns a found non-null template. */
                    uint32_t k24 =
                        ((uint32_t)RTFX_KGD_EFFECT_PROJECTILE << 16) | (uint32_t)udid;
                    void *node = 0;
                    rtfx_map_find((uint8_t *)lib + RTFX_CAT_EFFECT * 8u, &node, &k24);
                    void *tmpl = *(void **)((uint8_t *)node + 0x14u); /* 0x7E0EFD read pattern */
                    if (tmpl != 0)
                    {
                        line = udid; /* per-unit slot wins */
                        ++s_override_log_count;
                        if (s_override_log_count <= 30u) /* cap 30 */
                        {
                            log_info("[rtfx] per-unit udid=%u line=%u", (unsigned)udid,
                                     (unsigned)udid);
                        }
                    }
                    else
                    {
                        /* no slot -> vanilla weapon branch 0x7E0E51 */
                        SF_CGdFigureWeaponStats stats;
                        getWeaponStatsInSlot(fig_base, &stats, idx, 1); /* 0x7E0E5F, reuse */
                        line = stats.wpn_type; /* word[+0xA] @0x7E0E6D */
                    }
                }
                else
                {
                    line = udid; /* 0x7E0E31..0x7E0E4C: udid > 1223 -> per-unit branch */
                }
            }
            esi = ((uint32_t)RTFX_KGD_EFFECT_PROJECTILE << 16) + (uint32_t)line;
            break; /* key = (0x18 << 16) | line */
        }
        case 6: /* 0x7E0EDF: mov esi,ecx; shr esi,8 - NO movzx: the +3 byte of
                 * the RAW key dword lands in bits 16..23, exactly as vanilla
                 * (rtfx_load_key_dword, not the pack output - R-2) */
        {
            esi = key_dword >> 8;
            break;
        }
        case 5:   /* 0x7E0EE4 via the table: esi stays 0 from the prologue */
        default:  /* cat ∉ [1..6] @0x7E0AC9 ja 0x7e0ee4 */
            break;
        }

    /* ---- tail 0x7E0EE4..0x7E0FA5, strict semantics ---- */
    /* Step 1: exact find in subtree lib + cat*8 (subtree read from the key
     * cat byte, 0x7E0EEF; node value at +0x14, 0x7E0EFD). t != 0 returns
     * IMMEDIATELY - without any comparison against strict (0x7E0F02 jne
     * epilogue fires before the cmp). */
    uint32_t k = esi;
    void *node = 0;
    rtfx_map_find((uint8_t *)lib + (uint32_t)cat * 8u, &node, &k); /* 0x7E0EF5 */
    void *t = *(void **)((uint8_t *)node + 0x14u);
    if (t != 0)
    {
        return t;
    }
    /* Step 2: t == 0 and strict != 0 -> return 0, the ladder never runs
     * (0x7E0F08 cmp [ebp+0xc], eax(0) / 0x7E0F0B jne epilogue). */
    if (strict != 0)
    {
        return 0;
    }
    /* Ladder (strict == 0), NO exclude comparisons anywhere. */
    if (cat == RTFX_CAT_EFFECT) /* 0x7E0F19 */
    {
        k = esi & 0xFF0000u; /* type-default, line -> 0 (0x7E0F1E) */
        node = 0;
        rtfx_map_find_or_insert((uint8_t *)lib + 0x20u, &node, &k); /* lib+0x20 == lib+cat*8 */
        t = *(void **)((uint8_t *)node + 0x14u);
        if (t != 0)
        {
            return t;
        }
    }
    else if (cat == 1) /* 0x7E0F2C */
    {
        k = esi & 0xFFFFFFu; /* without variant (0x7E0F36) */
        node = 0;
        rtfx_map_find_or_insert((uint8_t *)lib + 8u, &node, &k); /* subtree lib+8 */
        t = *(void **)((uint8_t *)node + 0x14u);
        if (t != 0)
        {
            return t;
        }
        k = (uint16_t)esi; /* line-only (0x7E0F55 movzx eax,si) */
        node = 0;
        rtfx_map_find_or_insert((uint8_t *)lib + (uint32_t)cat * 8u, &node, &k);
        t = *(void **)((uint8_t *)node + 0x14u);
        if (t != 0)
        {
            return t;
        }
    }
    else /* 0x7E0F6B: every other cat */
    {
        k = (uint16_t)esi; /* line-only */
        node = 0;
        rtfx_map_find_or_insert((uint8_t *)lib + (uint32_t)cat * 8u, &node, &k);
        t = *(void **)((uint8_t *)node + 0x14u);
        if (t != 0)
        {
            return t;
        }
    }
    /* Final step, unconditional (0x7E0F8B..0x7E0FA0): key 0 in lib+8 for
     * cat 1, in lib for everything else. */
    k = 0;
    node = 0;
    rtfx_map_find_or_insert((cat == 1) ? (void *)((uint8_t *)lib + 8u) : (void *)lib, &node, &k);
    return *(void **)((uint8_t *)node + 0x14u);
}

/* ---- install ------------------------------------------------------------ */

void initialize_template_resolver_hooks(void)
{
    /* Callee VA table (RVA = VA - 0x400000). */
    rtfx_get_cgdbuilding = (rtfx_get_ptr)(ASI::AddrOf(0x3b0fd0));       /* VA 0x7B0FD0 */
    rtfx_get_cgfigure = (rtfx_get_ptr)(ASI::AddrOf(0x3b1150));          /* VA 0x7B1150 */
    rtfx_get_cgobject = (rtfx_get_ptr)(ASI::AddrOf(0x3b12d0));          /* VA 0x7B12D0 */
    rtfx_get_effect_ptr = (rtfx_get_ptr)(ASI::AddrOf(0x3b10d0));        /* VA 0x7B10D0 */
    rtfx_get_holder = (rtfx_get_ptr)(ASI::AddrOf(0x3b1770));            /* VA 0x7B1770 */
    rtfx_is_mainchar = (rtfx_is_flag_ptr)(ASI::AddrOf(0x279da0));       /* VA 0x679DA0 */
    rtfx_is_hero = (rtfx_is_flag_ptr)(ASI::AddrOf(0x1be500));           /* VA 0x5BE500 */
    rtfx_get_equipment = (rtfx_get_equipment_ptr)(ASI::AddrOf(0x2b26a0)); /* VA 0x6B26A0 */
    /* getWeaponStatsInSlot reused from sf_weapon_type_hook.c (VA 0x6B3050) */
    rtfx_has_xdata = (rtfx_xdata_kind_ptr)(ASI::AddrOf(0x2de790));      /* VA 0x6DE790 */
    rtfx_get_xdata = (rtfx_xdata_kind_ptr)(ASI::AddrOf(0x2dd730));      /* VA 0x6DD730 */
    rtfx_get_race = (rtfx_get_race_ptr)(ASI::AddrOf(0x2c6820));         /* VA 0x6C6820 */
    rtfx_get_gd_resource = (rtfx_get_resource_ptr)(ASI::AddrOf(0x277441)); /* VA 0x677441 */
    rtfx_get_spell_spellline = (rtfx_spell_line_ptr)(ASI::AddrOf(0x26e100)); /* VA 0x66E100 */
    rtfx_map_find = (rtfx_map_find_ptr)(ASI::AddrOf(0x3dfdb0));         /* VA 0x7DFDB0 */
    rtfx_map_find_or_insert = (rtfx_map_find_ptr)(ASI::AddrOf(0x3dfe40)); /* VA 0x7DFE40 */
    rtfx_object_scale = (rtfx_scale_ptr)(ASI::AddrOf(0x3e1020));        /* VA 0x7E1020 */
    rtfx_key_group_byte = (rtfx_key_group_ptr)(ASI::AddrOf(0x46dfd0));  /* VA 0x86DFD0 */
    rtfx_holder_gd_main = (rtfx_holder_gd_main_ptr)(ASI::AddrOf(0x3b13d0)); /* VA 0x7B13D0 */

    /* Whole-function detour on the resolver ENTRY (VA 0x7E0AA0, RVA
     * 0x3E0AA0): every consumer enters through 0x7E0AA0 (17 xrefs, all
     * UNCONDITIONAL_CALL, no mid-function entry),
     * so a single E9 covers the whole call surface uniformly. */
    int entry = ASI::AddrOf(0x3e0aa0);
    bool precheck_ok = (*(uint8_t *)(entry + 0) == 0x55) &&
                       (*(uint8_t *)(entry + 1) == 0x8B) &&
                       (*(uint8_t *)(entry + 2) == 0xEC); /* push ebp; mov ebp,esp */
    if (!precheck_ok)
    {
        /* R-70: explicit refusal, the hook stays inactive. */
        log_info("[rtfx] install FAILED precheck/readback mismatch");
        return;
    }
    uint8_t saved[5];
    int i;
    for (i = 0; i < 5; ++i)
    {
        saved[i] = *(uint8_t *)(entry + i);
    }
    ASI::MemoryRegion entry_mreg(entry, 5);
    ASI::BeginRewrite(entry_mreg);
    *(uint8_t *)entry = 0xE9; /* JMP rel32 */
    *(int *)(entry + 1) = (int)(&sf_template_resolver) - (entry + 5);
    ASI::EndRewrite(entry_mreg);
    int readback = entry + 5 + *(int *)(entry + 1);
    if (readback != (int)(&sf_template_resolver))
    {
        /* The detour did not land - restore the saved vanilla bytes so
         * the hook stays inactive, refusal log like the precheck. */
        ASI::BeginRewrite(entry_mreg);
        for (i = 0; i < 5; ++i)
        {
            *(uint8_t *)(entry + i) = saved[i];
        }
        ASI::EndRewrite(entry_mreg);
        log_info("[rtfx] install FAILED precheck/readback mismatch");
        return;
    }
    log_info("[rtfx] install template-resolver whole-function at 7E0AA0 readback=%s",
             (readback == (int)(&sf_template_resolver)) ? "OK" : "MISMATCH");
}
