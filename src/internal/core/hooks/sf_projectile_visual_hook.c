/* Projectile visual anchor hook (waves 17/19).
 *
 * The reflected projectile's flight is rendered by the CLIENT per frame
 * inside CDrwEffectObject::vfunction15 (VA 0x8A9F70), which resolves two
 * anchor keys from the GD effect record and feeds them to FUN_008aa610
 * through the single call site at VA 0x8AA2F4 (RVA 0x4AA2F4):
 *   a3 = secondary anchor = record target pair (+0x02) — left untouched;
 *   a4 = primary anchor   = record source pair (+0x09) — substituted.
 * cloneEffect keeps the DONOR figure in the record's source pair (K12: the
 * client template resolver picks the donor's weapon for the visual), so the
 * donor would also anchor the rendered arc. This wrapper rewrites a4 to
 * keybuild(figure, reflector) for registered clones: the arc then flies
 * reflector -> attacker while the template still resolves from the donor
 * (engine precedent: the kGdSpellLineRockBulletChained branch of vf15
 * rebuilds its primary key from xdata the same way).
 *
 * Wave 19: FUN_008aa610 resolves the a4 key STATELESSLY on EVERY frame
 * (research O-F3), so the wave-17 consume-once substitution collapsed the
 * arc back to the attacker after the first rendered frame. The ring now
 * keys entries by the clone's record id and keeps them for the whole
 * flight (cs <= expire_cs = tick_start + tick_count, research O-F1/O-F5);
 * match is non-consuming and re-substitutes a4 on every frame. The
 * original callee is ALWAYS called; effect record, timings and positions
 * are not touched. */

#include "sf_projectile_visual_hook.h"
#include "../../../api/sfsf.h"
#include "../../../asi/sf_asi.h"
#include "../sf_wrappers.h"

/* Original callee of the hooked site (FUN_008aa610, VA 0x8AA610):
 * __thiscall, this + 7 stack arguments (record, view, a3, a4, X, Y, Z) —
 * lifted from the site byte context (wave-9d scan) and the vf15 decompile,
 * which forwards (this, record, record+0xC, secondary, primary, X, Y, Z). */
typedef void (__thiscall *pvfx_fun_8aa610_ptr)(void *_this, void *record, void *view,
                                               uint32_t a3, uint32_t a4,
                                               float x, float y, float z);

static pvfx_fun_8aa610_ptr pvfx_orig_8aa610;

/* Key builder (FUN_0086dd60, VA 0x86DD60): packs (category, index) into the
 * caller's scratch dword and returns it — precedents: sf_menu_hook.c
 * (fun_0086dd60_ptr) and vf15's own anchor key assembly. */
static fun_0086dd60_ptr pvfx_keybuild;

/* The CGdMain global slot (VA 0xD75D38): the global holds a holder whose
 * first member is the CGdMain pointer (same read as sf_weapon_type_hook.c
 * and the vanilla code at 0x7DB7C5..0x7DB7F5). */
typedef struct __attribute__((packed))
{
    SF_CGdMain *CGdMain; /* +0x000: *(*0xD75D38) */
} PvfxGdMainHolder;

static_assert(offsetof(PvfxGdMainHolder, CGdMain) == 0x0,
              "PvfxGdMainHolder.CGdMain must sit at +0x0 (vanilla reads *(*0xD75D38))");

static_assert(offsetof(SF_CGdMain, data) + offsetof(CGdMain_data, CGdEffect) == 0x28,
              "SF_CGdMain.data.CGdEffect must sit at +0x28 (effects manager)");

/* GD effect slot view (canon: the Add record array starts at CGdEffect+0x3C,
 * stride 0x2C; cloneEffect addresses records the same way). Wave-11
 * arithmetic kept: the view base is manager + eid*0x2C and the 0x3C array
 * gap stays INSIDE the view. */
typedef struct __attribute__((packed))
{
    uint8_t gap_0x00[0x3c]; /* +0x00: manager tail up to the record start */
    uint16_t type;          /* +0x3C: effect type (24 = kGdEffectProjectile) */
    uint8_t target_cat;     /* +0x3E: target entity category (a3 anchor) */
    uint16_t target_index;  /* +0x3F: target entity index */
    uint8_t gap_0x41[4];    /* +0x41 */
    uint8_t source_cat;     /* +0x45: source entity category (donor after K12) */
    uint16_t source_index;  /* +0x46: source entity index (donor figure) */
} PvfxEffectSlot;

static_assert(offsetof(PvfxEffectSlot, type) == 0x3c,
              "PvfxEffectSlot.type must sit at +0x3C (record start inside the view)");
static_assert(offsetof(PvfxEffectSlot, source_cat) == 0x45,
              "PvfxEffectSlot.source_cat must sit at +0x45 (record source pair)");
static_assert(offsetof(PvfxEffectSlot, source_index) == 0x46,
              "PvfxEffectSlot.source_index must sit at +0x46 (record source pair)");

static PvfxAnchorRing g_anchor_ring = {0};

void pvfx_register_anchor(uint16_t clone_eid, uint16_t donor, uint16_t reflector, uint32_t expire_cs,
                          uint32_t now_cs)
{
    pvfx_anchor_put(&g_anchor_ring, clone_eid, donor, reflector, expire_cs, now_cs);
}

static SF_CGdEffect *pvfx_get_effects_manager()
{
    PvfxGdMainHolder *holder = *(PvfxGdMainHolder **)ASI::AddrOf(0x975d38); /* CGdMain global (VA 0xD75D38) */
    return holder->CGdMain->data.CGdEffect;
}

static void __thiscall pvfx_site_8aa2f4_wrapper(void *_this, void *record, void *view,
                                                uint32_t a3, uint32_t a4,
                                                float x, float y, float z)
{
    SF_CGdEffect *effects = pvfx_get_effects_manager();
    if (effects != 0)
    {
        /* eid from the client record header (probe-5/6: *(u32*)record >> 8),
         * the same derivation vf15 itself uses for the GD record address. */
        uint16_t eid = (uint16_t)(*(uint32_t *)record >> 8);
        PvfxEffectSlot *slot = (PvfxEffectSlot *)((uint8_t *)effects + (uint32_t)eid * 0x2c);
        if (slot->type == kGdEffectProjectile && slot->source_cat == 1) /* projectile + figure source */
        {
            uint32_t cs = effects->OpaqueClass->current_step;
            uint16_t reflector = 0;
            if (pvfx_anchor_match(&g_anchor_ring, eid, cs, &reflector))
            {
                /* a4 := keybuild(figure, reflector) on EVERY flight frame: the
                 * engine resolver is stateless and re-reads the record keys
                 * each call, the RockBulletChained branch rebuilds its primary
                 * anchor exactly this way. */
                uint32_t key_scratch = 0;
                a4 = *pvfx_keybuild(&key_scratch, 1, reflector);
                uint16_t anchor_donor = 0;
                if (pvfx_anchor_log_once(&g_anchor_ring, eid, &anchor_donor))
                {
                    static uint32_t s_log_count = 0;
                    if (++s_log_count <= 30)
                    {
                        log_info("[avfx] anchor eid=%u donor=%u -> reflector=%u",
                                 eid, anchor_donor, reflector);
                    }
                }
            }
        }
    }
    pvfx_orig_8aa610(_this, record, view, a3, a4, x, y, z);
}

void initialize_projectile_visual_hooks()
{
    pvfx_keybuild = (fun_0086dd60_ptr)(ASI::AddrOf(0x46dd60));     /* FUN_0086dd60 (VA 0x86DD60) */
    pvfx_orig_8aa610 = (pvfx_fun_8aa610_ptr)(ASI::AddrOf(0x4aa610)); /* FUN_008aa610 (VA 0x8AA610) */

    /* Single E8 call site of FUN_008aa610 inside vf15 (VA 0x8AA2F4):
     * precheck the site bytes, redirect to the wrapper, readback own write. */
    int site = ASI::AddrOf(0x4aa2f4);
    bool precheck_ok = (*(uint8_t *)site == 0xE8) &&
        ((site + 5 + *(int *)(site + 1)) == ASI::AddrOf(0x4aa610));
    if (!precheck_ok)
    {
        log_info("[avfx] anchor-site precheck MISMATCH, install skipped");
        return;
    }
    ASI::MemoryRegion site_mreg(site, 5);
    ASI::BeginRewrite(site_mreg);
    *(uint8_t *)site = 0xE8; /* CALL instruction */
    *(int *)(site + 1) = (int)(&pvfx_site_8aa2f4_wrapper) - (site + 5);
    ASI::EndRewrite(site_mreg);
    int readback = site + 5 + *(int *)(site + 1);
    log_info("[avfx] install anchor-site readback=%s",
             (readback == (int)(&pvfx_site_8aa2f4_wrapper)) ? "OK" : "MISMATCH");
}
