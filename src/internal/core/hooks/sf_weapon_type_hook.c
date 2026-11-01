/**
 * @addtogroup WeaponTypeHook
 * @{
 */
#include "../../../asi/sf_asi.h"
#include "../sf_wrappers.h"
#include "../sf_hooks.h"
#include "../../registry/sf_weapon_type_registry.h"
#include "../../../api/sf_general_structures.h"
#include "../../../api/structures/sf_figure_structures.h"

#include <windows.h>
#include <stdint.h>
#include <stddef.h>

#include "sf_weapon_type_hook.h"
#include "sf_vanilla_fix_hook.h"


/* Callees of the replaced function that the framework does not resolve
 * itself, resolved at install time. RVA literals follow the
 * initialize_vanilla_fix_hooks convention. Calling conventions are
 * byte-verified in the game binary (__thiscall callees pop their own
 * stack args: ret N / C2 imm16 epilogues). Two callees are reused from
 * the framework: GetWeaponType (sf_vanilla_fix_hook global, same RVA
 * 0x269260) and GetCurrentHealthPercent (figureAPI, RVA 0x2FDEB0). All
 * other reads go through the typed framework structures. */

typedef void *(__thiscall *getWeaponStatsInSlot_ptr)(void *self, void *out, uint16_t figure_id,
                                                              uint8_t slot); /* 0x6B3050, __thiscall, ret 0xC */
typedef uint8_t (__thiscall *getAnimationState_ptr)(void *self); /* 0x7D9EC0, __thiscall, ret */

getWeaponStatsInSlot_ptr getWeaponStatsInSlot;
getAnimationState_ptr getAnimationState;

/* DrwFigure layout, byte-verified in the game binary (no SFSF header exists).
 * Only the fields used by the weapon-type hook are mapped; gaps are sized
 * to keep the verified offsets (static_asserts below). */
typedef struct __attribute__((packed))
{
    uint32_t field_ref;                              /* +0x000: figure_id = field_ref >> 8 */
    uint8_t unknown_0x004[0x131 - 0x004];            /* unmapped */
    uint8_t ranged_anim;                             /* +0x131: cached ranged subtype */
    uint16_t strike_weapon;                          /* +0x132: strike weapon slot */
    uint8_t unknown_0x134[0x18C - 0x134];            /* unmapped */
    uint32_t frame_counter;                          /* +0x18C: LCG input together with figure_id */
} DrwFigure;

static_assert(offsetof(DrwFigure, field_ref) == 0x0,
              "DrwFigure.field_ref must sit at +0x0 (figure_id = field_ref >> 8)");
static_assert(offsetof(DrwFigure, ranged_anim) == 0x131,
              "DrwFigure.ranged_anim must sit at +0x131 (cached ranged subtype)");
static_assert(offsetof(DrwFigure, strike_weapon) == 0x132,
              "DrwFigure.strike_weapon must sit at +0x132 (strike weapon slot)");
static_assert(offsetof(DrwFigure, frame_counter) == 0x18C,
              "DrwFigure.frame_counter must sit at +0x18C (LCG input together with figure_id)");

/* The CGdMain global slot (VA 0xD75D38): the global holds a pointer to an
 * engine holder object whose first member is the CGdMain pointer (vanilla
 * reads *(*0xD75D38) at 0x7DB7C5..0x7DB7F5); the holder class has no SFSF
 * header, so only the slot read here is mapped. */
typedef struct __attribute__((packed))
{
    SF_CGdMain *CGdMain; /* +0x000: *(*0xD75D38) */
} GdMainHolder;

static_assert(offsetof(GdMainHolder, CGdMain) == 0x0,
              "GdMainHolder.CGdMain must sit at +0x0 (vanilla reads *(*0xD75D38))");

/* Pins the SF_CGdMain typing of the holder slot to the byte-verified
 * GetGdResource read ([this+0x64] @ 0x677441). */
static_assert(offsetof(SF_CGdMain, data) + offsetof(CGdMain_data, CGdResource) == 0x64,
              "SF_CGdMain.data.CGdResource must sit at +0x64 (GetGdResource reads [this+0x64] @ 0x677441)");


SF_CGdMain * getCGdMain()
{
    GdMainHolder *holder = *(GdMainHolder **)ASI::AddrOf(0x975d38); /* CGdMain global (VA 0xD75D38) */
    return holder->CGdMain;
}

static uint16_t getDrwFigureID(DrwFigure *_this)
{
    return (uint16_t)((_this)->field_ref >> 8);
}

static uint16_t getSlotWeaponType(DrwFigure *_this, uint8_t slot)
{
    SF_CGdFigureWeaponStats stats;
    SF_CGdFigure *CGdFigure = getCGdMain()->data.CGdFigure;
    /* Verified against the tooltip fix: same CGdMain (same 0xD75D38 holder the
     * vanilla callee reads) and same data.CGdFigure field (tooltip getFigurePtr
     * reads +0x30); decompiled 0x6B3050 is a plain weapon_stats[slot] copy, no +2. */
    getWeaponStatsInSlot(CGdFigure, &stats, getDrwFigureID(_this), slot);
    return stats.wpn_type;
}

/* ---- lazy engine reads over the typed structures ----------------------- */

static bool isShieldEquipped(void *_this)
{
    /* vanilla 0x7DB7C9..0x7DB7F3: equipment slot 3 item resolves to weapon
     * type 9 (shield) through GetGdResource/GetWeaponType. */
    SF_CGdResource *CGdResource = getCGdMain()->data.CGdResource;

    uint16_t figure_index = getDrwFigureID((DrwFigure *)_this);
    uint16_t item_id = getCGdMain()->data.CGdFigure->figures[figure_index].equipment[3];

    return GetWeaponType(CGdResource, item_id) == 9;
}

static bool isCastTargetSelf(void *_this)
{
    /* cast target: current job target is the figure itself (entity type 1) */
    SF_CGdFigure *CGdFigure = getCGdMain()->data.CGdFigure;
    uint16_t figure_index = getDrwFigureID((DrwFigure *)_this);
    return ((CGdFigure->figures[figure_index].current_job.target.entity_type == 1) &&
            (CGdFigure->figures[figure_index].current_job.target.entity_index == figure_index));
}

int isStoopJob(uint16_t job_id)
{
  if ((job_id == kGdJobMinerStoopToDropGood) ||
      (job_id == kGdJobStoneMinerStoopToDropStone) ||
      (job_id == kGdJobWoodCutterStoopToDropLog))
  {
    return 1;
  }
  return 0;
}

uint32_t getFigureFlags(uint16_t figure_index)
{
    SF_CGdFigure * CGdFigure = getCGdMain()->data.CGdFigure;
    return CGdFigure->figures[figure_index].flags;
}

uint16_t getFigureJobFlags(uint16_t figure_index)
{
    SF_CGdFigure * CGdFigure = getCGdMain()->data.CGdFigure;
    return CGdFigure->figures[figure_index].current_job.flags;
}

/* ---- SelectJobAnimation decision core ---------------------------------- */

/* Named engine constants of the SelectJobAnimation reimplementation. Job
 * ids, figure record flags and job flags REUSE the framework declarations
 * (FigureJobs / GdFigureFlags / CGdFigureJobFlags,
 * sf_general_structures.h - values byte-verified against the game binary).
 * The kDrwAnim values are animation subkeys written to *anim outside
 * strikes (strike exits write the weapon type instead); the binary has no
 * enum for them, so they stay local literals. */

/* Engine anim-state constants, complete for the whole registered family:
 * values decoded from the lua constant registrar (lua_register_constant
 * call sites at 0x7CB0xx/0x7D5Cxx, pooled double literals per name) and
 * cross-checked against byte-verified SelectJobAnimation writes plus the
 * Original Scripts movie tables (hero-male Range1: Default/Special1 =
 * attackbow_aim_01/02; DwarfAnims RoundHit = whirlwind). The family ends
 * at 107 - nothing >= 108 is registered; values 18..99 of the subtype
 * space are weapon types (kDrwWt 1..21), not kDrwAnim members. */
enum
{
    kDrwAnimDefault = 0,
    kDrwAnimWar = 1,
    kDrwAnimRun = 2,
    kDrwAnimRetreat = 3,
    kDrwAnimSpecial1 = 4,
    kDrwAnimSpecial2 = 5,
    kDrwAnimSpecial3 = 6,
    kDrwAnimSpecial4 = 7,
    kDrwAnimSpecial5 = 8,
    kDrwAnimSpecial6 = 9,
    kDrwAnimFront = 10,
    kDrwAnimShoulder = 11,
    kDrwAnimMagic = 12,
    kDrwAnimFish = 13,
    kDrwAnimTalkIdle = 14,
    kDrwAnimTalk = 15,
    kDrwAnimBack = 16,
    kDrwAnimRight = 17,
    kDrwAnimLeft = 100,
    kDrwAnimShield = 101,
    kDrwAnimBoth = 102,
    kDrwAnimStaff = 103,
    kDrwAnimDual = 104,
    kDrwAnimScytheHit = 105,
    kDrwAnimRoundHit = 106,
    kDrwAnimDeathBlow = 107
};

/* Health percent threshold of the walk branch: cmp ax, 0x19 / jbe. */
#define KGDWALK_LOW_HEALTH_PERCENT 25

/** DrwFigure animation cache fields (rangedAnim +0x131, strikeWeapon +0x132). */
typedef struct
{
    uint8_t ranged_anim;
    uint16_t strike_weapon;
} sja_cache_t;

/*
 * Strike slot dispatch of the HitTarget branch. Byte-for-byte copy of the
 * vanilla jump table at 0x7DBB74 (index = slot0 - 2, bounds-checked against
 * 0x11 at 0x7DB803); group targets are the dword table at 0x7DBB54:
 *   0 -> 0x7DB81A punch          4 -> 0x7DB88C staff
 *   1 -> 0x7DB824 stab           5 -> 0x7DB85F rand-stab then both
 *   2 -> 0x7DB82E rand-stab+shield  6 -> 0x7DB884 both
 *   3 -> 0x7DB853 shield         7 -> default (falls to the HIT_LEFT check)
 */
enum
{
    HIT_GROUP_PUNCH = 0,
    HIT_GROUP_STAB = 1,
    HIT_GROUP_RAND_STAB_SHIELD = 2,
    HIT_GROUP_SHIELD = 3,
    HIT_GROUP_STAFF = 4,
    HIT_GROUP_RAND_STAB_BOTH = 5,
    HIT_GROUP_BOTH = 6,
    HIT_GROUP_DEFAULT = 7
};

static const uint8_t s_hit_group_by_slot[18] = {
    HIT_GROUP_PUNCH,           /* slot0 2  */
    HIT_GROUP_STAB,            /* slot0 3  */
    HIT_GROUP_RAND_STAB_SHIELD,/* slot0 4  */
    HIT_GROUP_SHIELD,          /* slot0 5  */
    HIT_GROUP_SHIELD,          /* slot0 6  */
    HIT_GROUP_SHIELD,          /* slot0 7  */
    HIT_GROUP_STAFF,           /* slot0 8  */
    HIT_GROUP_RAND_STAB_BOTH,  /* slot0 9  */
    HIT_GROUP_BOTH,            /* slot0 10 */
    HIT_GROUP_BOTH,            /* slot0 11 */
    HIT_GROUP_BOTH,            /* slot0 12 */
    HIT_GROUP_STAFF,           /* slot0 13 */
    HIT_GROUP_STAFF,           /* slot0 14 */
    HIT_GROUP_STAFF,           /* slot0 15 */
    HIT_GROUP_DEFAULT,         /* slot0 16 */
    HIT_GROUP_DEFAULT,         /* slot0 17 */
    HIT_GROUP_SHIELD,          /* slot0 18 (0x12, 1HClaw -> shield group) */
    HIT_GROUP_PUNCH            /* slot0 19 */
};

static int hitTargetGroup(uint16_t slot0)
{
    /* vanilla 0x7DB800: add eax,-2; cmp eax,0x11; ja default */
    if (slot0 < 2 || slot0 > 19)
    {
        return HIT_GROUP_DEFAULT;
    }
    return s_hit_group_by_slot[slot0 - 2];
}

/** vanilla 0x7DB82E / 0x7DB85F: bit 16 of the stab LCG picks Stab. */
static bool randPicksStab(uint32_t x)
{
    return ((x * 0x4E6Du + 0x614Eu) & 0x10000u) != 0;
}

static void selectJobAnimationHitTarget(DrwFigure *_this, FigureJobs *job, uint8_t *anim,
                                        uint8_t *extra_anim, sja_cache_t *cache, uint16_t slot0,
                                        uint16_t slot1, uint32_t x)
{
    uint16_t figure_index = getDrwFigureID(_this);

    *job = kGdJobStrike;           /* vanilla 0x7DB7B3: mov eax,0x95; mov [esi],ax */
    cache->strike_weapon = slot0;  /* vanilla 0x7DB7BD: mov [edi+0x132],cx */
    _this->strike_weapon = slot0;  /* cache store right where the branch writes it */
    *extra_anim = kDrwAnimDefault; /* vanilla 0x7DB7C6: mov byte [eax],0 */

    /* Equipment slot 3 item must resolve to weapon type 9 (shield).
     * vanilla 0x7DB7C9..0x7DB7F3 */
    bool shield_equipped = isShieldEquipped(_this);

    switch (hitTargetGroup(slot0))
    {
        case HIT_GROUP_PUNCH: /* vanilla 0x7DB81A */
        {
            *job = kGdJobPunch;
            break;
        }
        case HIT_GROUP_STAB: /* vanilla 0x7DB824 */
        {
            *job = kGdJobStab;
            break;
        }
        case HIT_GROUP_RAND_STAB_SHIELD: /* vanilla 0x7DB82E */
        {
            if (randPicksStab(x))
            {
                *job = kGdJobStab;
            }
            /* both rand branches converge in the shield handler at 0x7DB853 */
            /* fall through */
        }
        case HIT_GROUP_SHIELD: /* vanilla 0x7DB853 */
        {
            if (shield_equipped)
            {
                *extra_anim = kDrwAnimShield; /* 0x65 */
            }
            break;
        }
        case HIT_GROUP_STAFF: /* vanilla 0x7DB88C */
        {
            *extra_anim = kDrwAnimStaff; /* 0x67 */
            break;
        }
        case HIT_GROUP_RAND_STAB_BOTH: /* vanilla 0x7DB85F */
        {
            if (randPicksStab(x))
            {
                *job = kGdJobStab;
            }
            /* fall through to the Both write at 0x7DB884 */
            /* fall through */
        }
        case HIT_GROUP_BOTH: /* vanilla 0x7DB884 */
        {
            *extra_anim = kDrwAnimBoth; /* 0x66, written in both rand outcomes */
            break;
        }
        default: /* vanilla default: switch does nothing, extra stays Default */
        {
            break;
        }
    }

    /* vanilla 0x7DB8A7: test [record+0x16],0x4000000; jnz skip - the block
     * runs only while HIT_LEFT_HAND_NEXT is CLEAR. */
    uint32_t figure_flags = getFigureFlags(figure_index);
    if ((figure_flags & GdFigureFlags::HIT_LEFT_HAND_NEXT) == 0)
    {
        *extra_anim = kDrwAnimLeft;   /* 0x64, vanilla 0x7DB8B0 */
        cache->strike_weapon = slot1; /* vanilla 0x7DB8B5: strikeWeapon = slot1 */
        _this->strike_weapon = slot1; /* cache store right where the branch writes it */
    }

    *anim = (uint8_t)cache->strike_weapon; /* vanilla 0x7DB8BB: mov al,[edi+0x132] */

    /* Forced anims, checked in order, a later flag overwrites an earlier one.
     * vanilla 0x7DB8D1 (0x1000), 0x7DB8F0 (0x800), 0x7DB917 (0x2000). The
     * flag and anim names deliberately do not pair - registrar-proven: the
     * DEATH_BLOW flag writes 0x69 = kDrwAnimScytheHit and START_WORK writes
     * 0x6B = kDrwAnimDeathBlow. */
    uint16_t job_flags = getFigureJobFlags(figure_index);
    if (job_flags & CGdFigureJobFlags::DEATH_BLOW)
    {
        *anim = kDrwAnimScytheHit;
    }
    if (job_flags & CGdFigureJobFlags::ROUND_HIT)
    {
        *anim = kDrwAnimRoundHit;
    }
    if (job_flags & CGdFigureJobFlags::START_WORK_AT_BUILDING_FORCE_JOB)
    {
        *anim = kDrwAnimDeathBlow;
    }
}

/* ---- the whole-function replacement ------------------------------------ */

void __thiscall sf_select_job_animation(DrwFigure *_this, FigureJobs *job, uint8_t *anim,
                                        uint8_t *extra_anim)
{
    sja_cache_t cache;
    cache.ranged_anim = _this->ranged_anim;
    cache.strike_weapon = _this->strike_weapon;

    /* Both weapon stats are read up front, exactly like the vanilla prologue:
     * slot 1 first (0x7DB611), then slot 0 (0x7DB632). */
    uint16_t slot1 = getSlotWeaponType(_this, 1);
    uint16_t slot0 = getSlotWeaponType(_this, 0);
    uint16_t figure_index = getDrwFigureID(_this);
    uint32_t x = figure_index + _this->frame_counter;
    uint32_t figure_flags = getFigureFlags(figure_index);
    uint16_t job_flags = getFigureJobFlags(figure_index);

    switch (*job)
    {
        case kGdJobGroupNothing:
        {
            uint8_t state = getAnimationState(_this); /* DrwFigure::GetStateAnimation (0x7D9EC0) */
            if (state != 0)
            {
                *anim = state;
                return;
            }
            if (figure_flags & GdFigureFlags::IS_TALKING)
            {
                *anim = kDrwAnimTalk;
                return;
            }
            if (figure_flags & GdFigureFlags::CUR_ACTIVE_DIALOG)
            {
                *anim = kDrwAnimTalkIdle;
                return;
            }
            if (job_flags & CGdFigureJobFlags::WAR)
            {
                *anim = kDrwAnimWar;
                return;
            }
            /* Special1..6: rand = ((0xBC614E - x*0x3E39B193) >> 16 & 0x7FFF) % 6,
             * +4 (vanilla 0x7DB6F2..0x7DB727, 32-bit wrapping arithmetic). */
            *anim = (uint8_t)(((0xBC614Eu - x * 0x3E39B193u) >> 16 & 0x7FFFu) % 6 + 4);
            return;
        }
        case kGdJobHitTargetRange1:
        {
            if (isOverridedRangedWeaponType(slot1))
            {
                *anim = (uint8_t)slot1;
                cache.ranged_anim = (uint8_t)slot1;
            }
            else
            {
                /* bow base {4,5}: LCG ((x*0x41C64E6D + 0xBC614E) >> 16) & 1, +4 */
                uint8_t bow_anim = (uint8_t)((((x * 0x41C64E6Du + 0xBC614Eu) >> 16) & 1) + 4);
                *anim = bow_anim;
                cache.ranged_anim = bow_anim;
            }
            _this->ranged_anim = cache.ranged_anim; /* cache store right where the branch writes it */
            return;
        }
        case kGdJobHitTargetRange2:
        {
            *anim = cache.ranged_anim; /* cache read, never written here */
            return;
        }
        case kGdJobHitTarget:
        {
            selectJobAnimationHitTarget(_this, job, anim, extra_anim, &cache, slot0, slot1, x);
            return;
        }
        case kGdJobCastResolve:
        {
            if (isCastTargetSelf(_this))
            {
                *anim = kDrwAnimWar;
                return;
            }
            return;
        }
        case kGdJobGroupWalk:
        {
            if (figure_flags & GdFigureFlags::RETREAT)
            {
                *anim = kDrwAnimRetreat;
                return;
            }
            uint16_t health_percent = figureAPI.getCurrentHealthPercent(getCGdMain()->data.CGdFigure, figure_index);
            if (health_percent <= KGDWALK_LOW_HEALTH_PERCENT)
            {
                *anim = kDrwAnimRetreat;
                return;
            }
            if (job_flags & CGdFigureJobFlags::WAR)
            {
                if (health_percent > KGDWALK_LOW_HEALTH_PERCENT)
                {
                    *anim = kDrwAnimWar;
                    return;
                }
            }
            if (job_flags & CGdFigureJobFlags::RUN_MODE)
            {
                if (health_percent > KGDWALK_LOW_HEALTH_PERCENT)
                {
                    *anim = kDrwAnimRun;
                    return;
                }
            }
            uint8_t state = getAnimationState(_this); /* vanilla 0x7DBA80 */
            if (state == 0)
            {
                return; /* no write, straight to the epilogue (0x7DBB41) */
            }
            *anim = state; /* vanilla 0x7DBA90 -> write tail 0x7DBB3F */
            return;
        }
        case kGdJobWalkBack:
        {
            *job = kGdJobGroupWalk;
            *anim = kDrwAnimBack;
            return;
        }
        case kGdJobStrafeRight:
        {
            *job = kGdJobGroupWalk;
            *anim = kDrwAnimRight;
            return;
        }
        case kGdJobStrafeLeft:
        {
            *job = kGdJobGroupWalk;
            *anim = kDrwAnimLeft; /* 100 */
            return;
        }
        default:
        {
            if (!isStoopJob(*job))
            {
                return;
            }
            *job = kGdJobStoop;
            *anim = getAnimationState(_this);
            return;
        }
    }
}

/* ---- install ----------------------------------------------------------- */

void install_select_job_animation_hook()
{
    getWeaponStatsInSlot = (getWeaponStatsInSlot_ptr)(ASI::AddrOf(0x2b3050));
    getAnimationState = (getAnimationState_ptr)(ASI::AddrOf(0x3d9ec0));

    /* Base ranged classification {17} works with or without a catalog; the
     * weapon type module fills the catalog later in the Core Module Phase. */
    resetWeaponTypeRegistry();

    /* Redirect the call site. The original is fully superseded, no
     * trampoline is needed; the 5 rewritten bytes sit outside the function
     * body, which stays byte-for-byte untouched. */
    ASI::MemoryRegion call_site_mreg(ASI::AddrOf(0x3dbe64), 5); /* SelectJobAnimation call site in UpdateAnims (VA 0x7DBE64) */
    ASI::BeginRewrite(call_site_mreg);
    *(unsigned char *)(ASI::AddrOf(0x3dbe64)) = 0xE8; /* call instruction */
    *(int *)(ASI::AddrOf(0x3dbe65)) = (int)(&sf_select_job_animation) - ASI::AddrOf(0x3dbe69);
    ASI::EndRewrite(call_site_mreg);
    log_info("SelectJobAnimation replaced (whole function) at 7DB5F0");
}

/** @} */
