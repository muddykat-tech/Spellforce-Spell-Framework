/**
 * @addtogroup MovieLibBridge
 * @{
 */
#include "../../../asi/sf_asi.h"
#include "../sf_wrappers.h"
#include "../sf_hooks.h" /* uiAPI (SF_String helpers) */
#include "../../registry/sf_animset_registry.h"
#include "../../registry/sf_weapon_type_registry.h" /* attack_style lookup of the v2 jobMap */
#include "../../../api/sf_general_structures.h"

#include <windows.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "sf_movie_lib_bridge.h"
#include "../sf_building_loader.h" /* readfile() */

/* The bridge turns catalog names into real films inside a figure's movie
 * library. Everything below follows the byte-verified engine layout: field
 * access goes through packed structures with static_asserts only, engine
 * callees are resolved once at install time from RVA literals. */

/* ---- engine layouts (local, packed; no SFSF headers exist for these) ---- */

/* MSVC std::_Tree node of the movie library tree. */
typedef struct __attribute__((packed)) SF_MovieTreeNode
{
    struct SF_MovieTreeNode *left;     /* +0x00 */
    struct SF_MovieTreeNode *parent;   /* +0x04 */
    struct SF_MovieTreeNode *right;    /* +0x08 */
    uint8_t color;                     /* +0x0C */
    uint8_t is_nil;                    /* +0x0D: head/sentinel marker */
    uint8_t unknown_0x0E[0x10 - 0x0E]; /* unmapped */
    uint32_t key;                      /* +0x10: job | subtype<<16 | variant<<24 */
    void *movie;                       /* +0x14: CDrwMovie* */
} SF_MovieTreeNode;

static_assert(offsetof(SF_MovieTreeNode, left) == 0x00, "SF_MovieTreeNode.left must sit at +0x00");
static_assert(offsetof(SF_MovieTreeNode, parent) == 0x04,
              "SF_MovieTreeNode.parent must sit at +0x04 (MovieTreeFindOrInsert 0x8AE810)");
static_assert(offsetof(SF_MovieTreeNode, right) == 0x08,
              "SF_MovieTreeNode.right must sit at +0x08 (CountMovieVariants 0x8AED60)");
static_assert(offsetof(SF_MovieTreeNode, is_nil) == 0x0D,
              "SF_MovieTreeNode.is_nil must sit at +0x0D (CountMovieVariants)");
static_assert(offsetof(SF_MovieTreeNode, key) == 0x10,
              "SF_MovieTreeNode.key must sit at +0x10 (job | subtype<<16 | variant<<24)");
static_assert(offsetof(SF_MovieTreeNode, movie) == 0x14, "SF_MovieTreeNode.movie must sit at +0x14");
static_assert(sizeof(SF_MovieTreeNode) == 0x18, "SF_MovieTreeNode must be 0x18 bytes");

/* CDrwMovieLibrary: only the tree head and the variant LCG state are mapped. */
typedef struct __attribute__((packed))
{
    void *vtable;                      /* +0x00 */
    SF_MovieTreeNode *tree_head;       /* +0x04: sentinel head, root = head->parent */
    uint8_t unknown_0x08[0x0C - 0x08]; /* unmapped */
    uint32_t lcg_state;                /* +0x0C: variant randomizer state (never rewritten here) */
} SF_CDrwMovieLibrary;

static_assert(offsetof(SF_CDrwMovieLibrary, tree_head) == 0x04,
              "SF_CDrwMovieLibrary.tree_head must sit at +0x04 (AddNewMovie/CountMovieVariants)");
static_assert(offsetof(SF_CDrwMovieLibrary, lcg_state) == 0x0C,
              "SF_CDrwMovieLibrary.lcg_state must sit at +0x0C");

/* CDrwMovie (0x30 bytes): only the three effect vectors are mapped. */
typedef struct __attribute__((packed))
{
    void *vtable;                       /* +0x00 */
    void *generic_effects_begin;        /* +0x04: rotation/scale/... vector (not read here) */
    uint8_t unknown_0x08[0x14 - 0x08];  /* +0x08..0x10 other data, not effects */
    void *anim_effects_begin;           /* +0x14: CDrwMovieEffectAnimation* vector */
    void *anim_effects_end;             /* +0x18 */
    void *anim_effects_cap;             /* +0x1C */
    void *sound_effects_begin;          /* +0x20: CDrwMovieEffectSound* vector */
    void *sound_effects_end;            /* +0x24 */
    void *sound_effects_cap;            /* +0x28 */
    uint8_t unknown_0x2C[0x30 - 0x2C];  /* unmapped */
} SF_CDrwMovie;

static_assert(offsetof(SF_CDrwMovie, generic_effects_begin) == 0x04,
              "SF_CDrwMovie.generic_effects_begin must sit at +0x04");
static_assert(offsetof(SF_CDrwMovie, anim_effects_begin) == 0x14,
              "SF_CDrwMovie.anim_effects_begin must sit at +0x14 (AddAnimation push_back)");
static_assert(offsetof(SF_CDrwMovie, anim_effects_end) == 0x18,
              "SF_CDrwMovie.anim_effects_end must sit at +0x18");
static_assert(offsetof(SF_CDrwMovie, sound_effects_begin) == 0x20,
              "SF_CDrwMovie.sound_effects_begin must sit at +0x20 (AddSound push_back)");
static_assert(sizeof(SF_CDrwMovie) == 0x30, "SF_CDrwMovie must be 0x30 bytes");

/* CDrwMovieEffectAnimation (0x50 bytes): only the name string is mapped. */
typedef struct __attribute__((packed))
{
    void *vtable;                       /* +0x00 */
    uint8_t unknown_0x04[0x34 - 0x04];  /* modes, selector, cache - written by the engine */
    SF_String name;                     /* +0x34: "animation\<base>_<...>.boa" */
    uint8_t unknown_0x44[0x50 - 0x44];  /* unmapped */
} SF_CDrwMovieEffectAnimation;

static_assert(offsetof(SF_CDrwMovieEffectAnimation, name) == 0x34,
              "SF_CDrwMovieEffectAnimation.name must sit at +0x34 (SetAnimationName 0x86B630)");
static_assert(sizeof(SF_CDrwMovieEffectAnimation) == 0x50,
              "SF_CDrwMovieEffectAnimation must be 0x50 bytes");

/* Engine play modes (constant registrar 0x7D5070). */
enum
{
    kDrwPlayOnce = 0,
    kDrwPlayLooped = 1,
    kDrwPlayBounce = 2,
    kDrwPlayClamped = 3,
    kDrwPlayStretched = 4, /* what the vanilla strike row uses */
    kDrwPlayContinous = 5
};

/* ---- engine callees, resolved at install time --------------------------- */

/* Max length of one film name buffer: "animation\" + 63 chars + ".boa",
 * with headroom. */
#define SFSF_ANIMSET_FILM_NAME_MAX 128

typedef void *(__thiscall *AddNewMovie_ptr)(void *lib, int job, int subtype, int variant); /* ret 12 */
typedef void *(__thiscall *AddAnimation_ptr)(void *movie, const char *name);               /* ret 4 */
typedef void *(__thiscall *SetRangeAndModes_ptr)(void *effect, float from, float to, int mode1, int mode2,
                                                 int mode3);                               /* ret 20 */
typedef void *(__thiscall *AddSound_ptr)(void *movie, int sound_id, float delay, float length); /* ret 12 */
typedef void *(__thiscall *SetSoundOffset_ptr)(void *effect_sound, float offset);          /* ret 4 */
typedef void **(__thiscall *LoadByName_ptr)(void *mgr, void **out_pair, SF_String *name);  /* ret 8 */
typedef void *(__thiscall *VcaManagerCtor_ptr)(void *mem); /* ret 0; Ghidra labels it __fastcall, with a
                                                            * single argument that is identical to __thiscall */
typedef void *(*EngineNew_ptr)(size_t size);               /* engine operator new, cdecl */

static AddNewMovie_ptr s_add_new_movie;
static AddAnimation_ptr s_add_animation;
static SetRangeAndModes_ptr s_set_range_and_modes;
static AddSound_ptr s_add_sound;
static SetSoundOffset_ptr s_set_sound_offset;
static LoadByName_ptr s_load_by_name;
static VcaManagerCtor_ptr s_vca_ctor;
static EngineNew_ptr s_engine_new;
static void **s_vca_manager_slot; /* global singleton slot (VA 0x00D751A4) */
static void *s_anim_effect_vftable;  /* CDrwMovieEffectAnimation::vftable, ASLR-resolved */
static void *s_sound_effect_vftable; /* CDrwMovieEffectSound::vftable, ASLR-resolved */

void install_movie_lib_bridge()
{
    s_add_new_movie = (AddNewMovie_ptr)(ASI::AddrOf(0x4aeb10));       /* CDrwMovieLibrary::AddNewMovie (VA 0x8AEB10) */
    s_add_animation = (AddAnimation_ptr)(ASI::AddrOf(0x46a7f0));      /* CDrwMovie::AddAnimation (VA 0x86A7F0) */
    s_set_range_and_modes =
        (SetRangeAndModes_ptr)(ASI::AddrOf(0x46b7d0));                /* CDrwMovieEffect::SetRangeAndModes (VA 0x86B7D0) */
    s_add_sound = (AddSound_ptr)(ASI::AddrOf(0x46a9e0));              /* CDrwMovie::AddSound (VA 0x86A9E0) */
    s_set_sound_offset =
        (SetSoundOffset_ptr)(ASI::AddrOf(0x46b8a0));                  /* CDrwMovieEffectSound::SetSoundOffset (VA 0x86B8A0) */
    s_load_by_name = (LoadByName_ptr)(ASI::AddrOf(0x3ba770));         /* VCA_SkeletalAnimationManager::LoadByName (VA 0x7BA770) */
    s_vca_ctor = (VcaManagerCtor_ptr)(ASI::AddrOf(0x3ba510));         /* manager constructor (VA 0x7BA510) */
    s_engine_new = (EngineNew_ptr)(ASI::AddrOf(0x675a9d));            /* engine operator new (VA 0xA75A9D) */
    s_vca_manager_slot = (void **)ASI::AddrOf(0x9751a4);              /* manager singleton global (VA 0x00D751A4) */
    s_anim_effect_vftable = (void *)ASI::AddrOf(0x7D9DE0); /* CDrwMovieEffectAnimation::vftable (VA 0xBD9DE0); beta-exe loads relocated (+0x60000 observed), absolute literals never match — pitfalls §1 */
    s_sound_effect_vftable = (void *)ASI::AddrOf(0x7D9E18); /* CDrwMovieEffectSound::vftable (VA 0xBD9E18); same relocation lesson as s_anim_effect_vftable */
}

uint16_t animsetJobForWeapon(uint16_t weapon_type)
{
    /* Full v2 jobMap (TZ 5.1): Punch for the fist-family, deterministic Stab
     * for the dagger, custom catalog types by attack_style, everything else
     * Strike - the registration job always equals the emission job of the
     * dispatcher, so the synth guard and the cascade step 1 agree. */
    if (weapon_type == 2 || weapon_type == 19)
    {
        return (uint16_t)kGdJobPunch;
    }
    if (weapon_type == 3)
    {
        return (uint16_t)kGdJobStab;
    }
    if ((weapon_type >= 22 && weapon_type <= 99) || (weapon_type >= 108 && weapon_type <= 239))
    {
        WeaponAttackStyle style;
        if (getWeaponTypeAttackStyle(weapon_type, &style))
        {
            if (style == WEAPON_ATTACK_STYLE_STAB)
            {
                return (uint16_t)kGdJobStab;
            }
            if (style == WEAPON_ATTACK_STYLE_PUNCH)
            {
                return (uint16_t)kGdJobPunch;
            }
        }
    }
    return (uint16_t)kGdJobStrike;
}

/* ---- resolve gate: probe a logical name with the engine loader ---------- */

/**
 * @brief Builds the engine-shaped probe string for a logical animation name.
 *
 * The probe form is "animation\<name>.boa": the engine's own consumer
 * (CDrwMovieEffectAnimation::vfunction2, VA 0x86ADF0) reads exactly this
 * string from the effect (+0x34, written by SetAnimationName 0x86B630 and
 * lua's AddAnimation(name..".boa")). LoadByName splits the INPUT string
 * (_splitpath semantics, FUN_005e6560) and rebuilds the path replacing only
 * the extension with "bob"/"bor" (_makepath semantics, FUN_005e6350): the
 * directory component comes from the input, so a bare name would probe
 * <root>\<name>.bob and always miss (research file, "Fix-pass R1/R2",
 * fact 1). The decorated form also makes the probe cache key identical to
 * the engine's playback key.
 */
static void buildProbeName(const char *name, char *out, size_t out_len)
{
    snprintf(out, out_len, "animation\\%s.boa", name);
}

/**
 * @brief Probes one logical animation name with the engine's own loader.
 *
 * LoadByName builds the path with the "bob" extension first and "bor" second,
 * pack-aware; a missing file returns 0 without an exception. A successful
 * probe also pre-caches the animation in the manager cache. The returned pair
 * belongs to the manager cache: only pair[0] is inspected, the owning counter
 * in pair[1] is deliberately left untouched (the cache lives for the whole
 * process, the catalog bounds the probe count to 230 names).
 *
 * Ownership of the probe string: the cache node stores a DEEP COPY of the key
 * (insert chain FUN_007ba460 -> FUN_007b9f40 -> FUN_007b9e00 calls
 * SF_String::copy_from 0x783720, which allocates its own buffer and memcpy's
 * the bytes; lookup FUN_007bb0b0 compares key contents) - the input string is
 * only read during the call itself. A stack probe destroyed right after the
 * call is therefore safe; no keep-alive pool is needed (research file,
 * "Fix-pass R1/R2", fact 2).
 */
static bool animNameResolves(const char *name)
{
    if (s_load_by_name == NULL || s_vca_manager_slot == NULL)
    {
        return false;
    }

    void *manager = *s_vca_manager_slot;
    if (manager == NULL)
    {
        /* Mirror the engine's lazy singleton: if (global == 0) { global =
         * ctor(new(8)); } - vfunction2 0x86ADF0 path. */
        void *memory = s_engine_new(8); /* 8-byte manager object */
        if (memory == NULL)
        {
            log_error("Animset bridge: could not build the animation manager, gate skipped");
            return false;
        }
        manager = s_vca_ctor(memory);
        *s_vca_manager_slot = manager;
    }

    char probe_name[SFSF_ANIMSET_FILM_NAME_MAX];
    buildProbeName(name, probe_name, sizeof(probe_name));

    SF_String probe;
    uiAPI.SFStringConstructor_char(&probe, probe_name);
    void *pair[2] = {NULL, NULL};
    s_load_by_name(manager, pair, &probe);
    uiAPI.SFStringDestructor(&probe);

    return pair[0] != NULL;
}

/* ---- DrwSound.lua replica (sound name -> event id) ---------------------- */

/* The vanilla file holds 403 source entries / 402 distinct Data keys (measured
 * 2026-11-04, DrwSound.lua l.6-860); the cap must exceed that with headroom
 * for modded files. Static cost: 1024 x 64 B. */
#define SFSF_DRW_SOUND_MAX_KEYS 1024
#define SFSF_DRW_SOUND_NAME_MAX 64

static char s_drsound_keys[SFSF_DRW_SOUND_MAX_KEYS][SFSF_DRW_SOUND_NAME_MAX];
static int s_drsound_count = 0;
static bool s_drsound_parsed = false; /* one parse attempt per process */
static bool s_drsound_usable = false;

static int drwSoundKeyCompare(const void *a, const void *b)
{
    return strcmp((const char *)a, (const char *)b);
}

/** @brief Matches one "<identifier> = {" line of the Data table (brace depth
 * is checked by the caller). Both vanilla styles pass: the opening brace on
 * the same line ("key = {") or on the next line ("key =\n{", e.g.
 * battle_hit_1haxe l.557); leading whitespace is arbitrary (the file mixes
 * tabs with "space tab", e.g. spell_hit_rockbullet l.163); @p rest is the
 * cursor at the terminating '\n' of the current line. */
static bool parseDrwSoundKeyLine(const char *line, const char *line_end,
                                 const char *rest, char *out_key)
{
    while (line < line_end && (line[0] == ' ' || line[0] == '\t'))
    {
        line++;
    }

    const char *identifier = line;
    while (line < line_end && (isalnum((unsigned char)line[0]) || line[0] == '_'))
    {
        line++;
    }
    size_t identifier_len = (size_t)(line - identifier);
    if (identifier_len == 0 || identifier_len >= SFSF_DRW_SOUND_NAME_MAX)
    {
        return false;
    }

    while (line < line_end && (line[0] == ' ' || line[0] == '\t'))
    {
        line++;
    }
    if (line >= line_end || line[0] != '=')
    {
        return false;
    }
    line++;
    while (line < line_end && (line[0] == ' ' || line[0] == '\t'))
    {
        line++;
    }
    if (line < line_end)
    {
        if (line[0] != '{')
        {
            return false;
        }
    }
    else
    {
        /* Second vanilla style: the line ends right after '=' and the opening
         * brace sits on the NEXT line. rest points at the '\n' (or '\0' for
         * the last line, which has no brace and fails). */
        if (rest == NULL || rest[0] != '\n')
        {
            return false;
        }
        rest++;
        while (rest[0] == ' ' || rest[0] == '\t')
        {
            rest++;
        }
        if (rest[0] != '{')
        {
            return false;
        }
    }

    memcpy(out_key, identifier, identifier_len);
    out_key[identifier_len] = '\0';
    return true;
}

/** @brief Net brace delta of one line, ignoring Lua strings and -- comments. */
static int drwSoundNetBraces(const char *line, const char *line_end)
{
    int delta = 0;
    const char *p = line;
    char quote = '\0';
    for (; p < line_end; p++)
    {
        if (quote != '\0')
        {
            if (p[0] == '\\' && p + 1 < line_end)
            {
                p++;
            }
            else if (p[0] == quote)
            {
                quote = '\0';
            }
        }
        else if (p[0] == '"' || p[0] == '\'')
        {
            quote = p[0];
        }
        else if (p[0] == '-' && p + 1 < line_end && p[1] == '-')
        {
            break; /* comment runs to the end of the line */
        }
        else if (p[0] == '{')
        {
            delta++;
        }
        else if (p[0] == '}')
        {
            delta--;
        }
    }
    return delta;
}

/**
 * @brief Replicates DrwSoundId construction (DrwSound.lua lines 916-921):
 * the id of a sound is its 1-based position in the strcmp-sorted list of the
 * depth-1 keys of the Data table.
 */
static void loadDrwSoundTable()
{
    s_drsound_parsed = true;
    s_drsound_usable = false;
    s_drsound_count = 0;

    char current_directory[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, current_directory);

    char lua_path[MAX_PATH];
    snprintf(lua_path, sizeof(lua_path), "%s\\script\\DrwSound.lua", current_directory);

    char *lua = readfile(lua_path); /* extern "C", logs the reason itself */
    if (lua == NULL)
    {
        log_error("Animset bridge: DrwSound.lua not readable (%s), all pair sounds skipped", lua_path);
        return;
    }

    char key[SFSF_DRW_SOUND_NAME_MAX];
    const char *cursor = lua;
    int total_keys = 0;
    /* tkeys(Data) semantics: keys are the entries DIRECTLY inside the first
     * top-level table (vanilla "local Data = {" l.6, closes l.860; the
     * hits/misses tables l.863-912 belong to a separate "local BattleData").
     * Style-B blocks keep their interior fields (File/Volume/...) at ONE tab,
     * so indentation alone cannot separate keys from fields — track braces. */
    int depth = 0;
    bool in_data_table = false;
    int net_braces;
    while (*cursor != '\0')
    {
        const char *line = cursor;
        while (*cursor != '\0' && *cursor != '\n')
        {
            cursor++;
        }
        const char *line_end = cursor;
        if (line_end > line && line_end[-1] == '\r')
        {
            line_end--;
        }

        net_braces = drwSoundNetBraces(line, line_end);
        if (in_data_table && depth == 1 &&
            parseDrwSoundKeyLine(line, line_end, cursor, key))
        {
            total_keys++;
            if (s_drsound_count < SFSF_DRW_SOUND_MAX_KEYS)
            {
                memcpy(s_drsound_keys[s_drsound_count], key, strlen(key) + 1);
                s_drsound_count++;
            }
        }

        depth += net_braces;
        if (!in_data_table && depth >= 1)
        {
            in_data_table = true;
        }
        else if (in_data_table && depth <= 0)
        {
            break; /* Data closed: BattleData and the trailer are not scanned */
        }

        if (*cursor == '\n')
        {
            cursor++;
        }
    }
    free(lua);

    if (total_keys > SFSF_DRW_SOUND_MAX_KEYS)
    {
        /* The id is the position in the FULL sorted key list; with keys left
         * unread the computed ids would silently point at wrong sounds, so an
         * over-cap file is refused outright. */
        log_error("Animset bridge: DrwSound.lua has more than %d sound keys (%d found), all pair sounds skipped",
                  SFSF_DRW_SOUND_MAX_KEYS, total_keys);
        return;
    }

    if (s_drsound_count == 0)
    {
        log_error("Animset bridge: DrwSound.lua has no sound entries (%s), all pair sounds skipped",
                  lua_path);
        return;
    }

    qsort(s_drsound_keys, s_drsound_count, SFSF_DRW_SOUND_NAME_MAX, drwSoundKeyCompare);
    /* A Lua table key is unique: a name assigned twice in the Data constructor
     * (vanilla: battle_npc_gargoyle_scream l.252/283, battle_npc_deer_die
     * l.416/435) is ONE key — compact the sorted list so ids do not shift. */
    {
        int unique_count = 0;
        int i;
        for (i = 0; i < s_drsound_count; i++)
        {
            if (unique_count == 0 ||
                strcmp(s_drsound_keys[unique_count - 1], s_drsound_keys[i]) != 0)
            {
                if (unique_count != i)
                {
                    memcpy(s_drsound_keys[unique_count], s_drsound_keys[i],
                           SFSF_DRW_SOUND_NAME_MAX);
                }
                unique_count++;
            }
        }
        s_drsound_count = unique_count;
    }
    s_drsound_usable = true;
}

/** @return The DrwSound event id of @p name (1-based), or 0 when unknown. */
static int drwSoundIdFor(const char *name)
{
    if (!s_drsound_parsed)
    {
        loadDrwSoundTable();
    }
    if (!s_drsound_usable)
    {
        return 0;
    }

    char key[SFSF_DRW_SOUND_NAME_MAX];
    strncpy(key, name, sizeof(key) - 1);
    key[sizeof(key) - 1] = '\0';

    char *found = (char *)bsearch(key, s_drsound_keys, s_drsound_count, SFSF_DRW_SOUND_NAME_MAX,
                                  drwSoundKeyCompare);
    /* bsearch returns a pointer to the ELEMENT (stride NAME_MAX bytes), the id
     * is its element index + 1 - not the byte offset. */
    return found != NULL ? (int)((found - s_drsound_keys[0]) / SFSF_DRW_SOUND_NAME_MAX) + 1 : 0;
}

/* ---- movie tree walk: names, node count, leftmost key ------------------- */

#define SFSF_ANIMSET_LIB_FILM_CAP 512   /* vanilla libraries hold a few dozen films */

typedef struct
{
    char (*names)[SFSF_ANIMSET_FILM_NAME_MAX]; /* NULL = signature-only walk */
    int names_cap;
    int name_count;
    int node_count;
    uint32_t leftmost_key; /* key of the first in-order node (smallest key) */
    bool first_node_seen;
} LibWalkResult;

/** @brief Converts the effect's engine string to a plain C string, ASCII-clamped. */
static bool animEffectNameToChar(const SF_String *name, char *out, size_t out_len)
{
    if (name->raw_data == NULL || name->str_length == 0)
    {
        return false;
    }
    size_t count = name->str_length;
    if (count >= out_len)
    {
        count = out_len - 1;
    }
    for (size_t i = 0; i < count; i++)
    {
        wchar_t wide = name->raw_data[i];
        out[i] = (wide >= 0x20 && wide <= 0x7F) ? (char)wide : '?';
    }
    out[count] = '\0';
    return true;
}

/**
 * @brief Strips the "animation\" prefix and the ".boa" suffix of a stored
 * effect name, leaving the bare "<base>_<...>" film name.
 */
static void stripAnimNameDecoration(const char *full_name, char *out, size_t out_len)
{
    static const char k_prefix[] = "animation\\";
    static const char k_suffix[] = ".boa";

    const char *start = full_name;
    size_t length = strlen(full_name);

    if (strncmp(start, k_prefix, strlen(k_prefix)) == 0)
    {
        start += strlen(k_prefix);
        length -= strlen(k_prefix);
    }
    if (length > strlen(k_suffix) && strcmp(start + length - strlen(k_suffix), k_suffix) == 0)
    {
        length -= strlen(k_suffix);
    }
    if (length >= out_len)
    {
        length = out_len - 1;
    }
    memcpy(out, start, length);
    out[length] = '\0';
}

/** @brief Collects the bare film names of one movie's animation effects. */
static void collectMovieNames(const SF_CDrwMovie *movie, LibWalkResult *walk)
{
    if (movie == NULL || walk->names == NULL)
    {
        return;
    }

    SF_CDrwMovieEffectAnimation *const *effect = (SF_CDrwMovieEffectAnimation *const *)movie->anim_effects_begin;
    SF_CDrwMovieEffectAnimation *const *effect_end = (SF_CDrwMovieEffectAnimation *const *)movie->anim_effects_end;
    for (; effect < effect_end; effect++)
    {
        SF_CDrwMovieEffectAnimation *candidate = *effect;
        if (*(void **)candidate != s_anim_effect_vftable) /* CDrwMovieEffectAnimation::vftable (ASLR-resolved at install) */
        {
            continue; /* polymorphic vector: only animation effects carry a name here */
        }

        char full_name[SFSF_ANIMSET_FILM_NAME_MAX];
        char bare_name[SFSF_ANIMSET_FILM_NAME_MAX];
        if (!animEffectNameToChar(&candidate->name, full_name, sizeof(full_name)))
        {
            continue;
        }
        if (strncmp(full_name, "animation\\", strlen("animation\\")) != 0)
        {
            continue; /* not a lua-registered film name; keeps the LCP honest */
        }

        stripAnimNameDecoration(full_name, bare_name, sizeof(bare_name));
        if (bare_name[0] == '\0' || walk->name_count >= walk->names_cap)
        {
            continue;
        }
        strncpy(walk->names[walk->name_count], bare_name, SFSF_ANIMSET_FILM_NAME_MAX - 1);
        walk->names[walk->name_count][SFSF_ANIMSET_FILM_NAME_MAX - 1] = '\0';
        walk->name_count++;
    }
}

/** @brief In-order walk of the movie tree (the first node seen is leftmost). */
static void walkMovieTreeNode(const SF_MovieTreeNode *node, LibWalkResult *walk)
{
    if (node == NULL || node->is_nil != 0)
    {
        return;
    }

    walkMovieTreeNode(node->left, walk);

    walk->node_count++;
    if (!walk->first_node_seen)
    {
        walk->leftmost_key = node->key;
        walk->first_node_seen = true;
    }
    collectMovieNames((const SF_CDrwMovie *)node->movie, walk);

    walkMovieTreeNode(node->right, walk);
}

static void walkMovieLibrary(void *movie_lib, LibWalkResult *walk)
{
    SF_CDrwMovieLibrary *library = (SF_CDrwMovieLibrary *)movie_lib;
    const SF_MovieTreeNode *root = library->tree_head != NULL ? library->tree_head->parent : NULL;
    walkMovieTreeNode(root, walk);
}

/* ---- base_name recovery: LCP of film names, cut at the last '_' --------- */

/**
 * @brief Recovers the library's full base_name from its film names.
 * @param out_example First collected film name, for the evidence log line.
 * @return false when the longest common prefix carries no '_' component edge.
 */
static bool recoverBaseName(const char (*names)[SFSF_ANIMSET_FILM_NAME_MAX], int name_count,
                            char *out_base_name, size_t out_len, const char **out_example)
{
    *out_example = names[0];

    size_t common = strlen(names[0]);
    for (int i = 1; i < name_count; i++)
    {
        size_t j = 0;
        while (j < common && j < strlen(names[i]) && names[0][j] == names[i][j])
        {
            j++;
        }
        common = j;
    }

    /* Cut at the last '_' inside the common prefix: "figure_dwarf_" and
     * "figure_dwarf_walk" both yield "figure_dwarf". */
    size_t base_length = 0;
    for (size_t j = common; j > 0; j--)
    {
        if (names[0][j - 1] == '_')
        {
            base_length = j - 1;
            break;
        }
    }
    if (base_length == 0 || base_length >= out_len)
    {
        return false;
    }

    memcpy(out_base_name, names[0], base_length);
    out_base_name[base_length] = '\0';
    return true;
}

/* ---- per-library cache with a validity guard ----------------------------- */

/* A library pointer could be reused by the allocator after the libraries are
 * rebuilt (a map reload rebuilds them; ConTestAnims.lua rebuilds them live).
 * Every cache touch therefore verifies the signature captured at configure
 * time: the leftmost tree key plus the node count (both recomputed cheaply
 * from the tree itself). v2 (TZ 5.2 p.7): the signature is captured for EVERY
 * final state and re-verified for every state, and the cache is finite -
 * 128 slots with a deterministic ring eviction (oldest INERT first, then the
 * oldest CONFIGURED/FAILED; a monotonic acquisition counter provides the
 * "oldest" order, no timestamps, no hashing). */
enum
{
    LIB_NEW = 0,
    LIB_CONFIGURED,
    LIB_INERT,
    LIB_FAILED
};

#define SFSF_ANIMSET_LIB_CACHE_MAX 128
#define SFSF_ANIMSET_MAX_PAIRS_PER_LIB 16 /* synthetic subkeys 240..255 */

typedef struct
{
    void *lib;
    uint32_t leftmost_key; /* signature component 1 */
    uint32_t node_count;   /* signature component 2 */
    uint32_t acquire_seq;  /* ring-eviction age order (monotonic) */
    uint8_t state;
    uint8_t pair_count;
    struct
    {
        uint16_t weapon;
        uint8_t synth;
    } pairs[SFSF_ANIMSET_MAX_PAIRS_PER_LIB];
} LibCacheEntry;

static LibCacheEntry s_lib_cache[SFSF_ANIMSET_LIB_CACHE_MAX];
static uint32_t s_lib_acquire_seq = 0;
static uint32_t s_lib_evictions = 0;
static bool s_lib_eviction_warned = false; /* the cache-full WARN is one-shot */

static LibCacheEntry *findLibCacheEntry(void *movie_lib)
{
    for (int i = 0; i < SFSF_ANIMSET_LIB_CACHE_MAX; i++)
    {
        if (s_lib_cache[i].state != LIB_NEW && s_lib_cache[i].lib == movie_lib)
        {
            return &s_lib_cache[i];
        }
    }
    return NULL;
}

/** @brief Policy observability (owner decision 14.11): a DEBUG snapshot of
 * the cache occupancy, printed at every configure completion and eviction.
 * Visible at the sfsf.ini debug level; INFO installs see nothing of it. */
static void logCacheSnapshot(const char *event, const LibCacheEntry *entry)
{
    int used = 0, configured = 0, inert = 0, failed = 0;
    for (int i = 0; i < SFSF_ANIMSET_LIB_CACHE_MAX; i++)
    {
        switch (s_lib_cache[i].state)
        {
            case LIB_CONFIGURED:
                configured++;
                used++;
                break;
            case LIB_INERT:
                inert++;
                used++;
                break;
            case LIB_FAILED:
                failed++;
                used++;
                break;
            default:
                break;
        }
    }
    log_debug(DEBUG_MED,
              "animset cache: %s lib %p -> state %d, signature key 0x%08x nodes %u; "
              "slots used %d/%d (configured %d, inert %d, failed %d), evictions %u",
              event, entry != NULL ? entry->lib : NULL, entry != NULL ? entry->state : 0,
              entry != NULL ? entry->leftmost_key : 0, entry != NULL ? entry->node_count : 0, used,
              SFSF_ANIMSET_LIB_CACHE_MAX, configured, inert, failed, s_lib_evictions);
}

static LibCacheEntry *acquireLibCacheEntry(void *movie_lib)
{
    for (int i = 0; i < SFSF_ANIMSET_LIB_CACHE_MAX; i++)
    {
        if (s_lib_cache[i].state == LIB_NEW)
        {
            s_lib_cache[i].lib = movie_lib;
            s_lib_cache[i].acquire_seq = ++s_lib_acquire_seq;
            return &s_lib_cache[i];
        }
    }

    /* Cache full (TZ 5.2 p.7): deterministic ring eviction instead of the old
     * "disabled for the rest". The oldest INERT slot dies first (a foreign
     * library costs one re-evaluation on its next touch); with no INERT slot
     * left the oldest CONFIGURED/FAILED dies (its next touch reconfigures it
     * - AddNewMovie replacement is idempotent). Every touching library gets
     * configured, so an eviction always frees a slot that can recover. */
    LibCacheEntry *oldest_inert = NULL;
    LibCacheEntry *oldest_other = NULL;
    for (int i = 0; i < SFSF_ANIMSET_LIB_CACHE_MAX; i++)
    {
        if (s_lib_cache[i].state == LIB_NEW)
        {
            continue;
        }
        if (s_lib_cache[i].state == LIB_INERT)
        {
            if (oldest_inert == NULL ||
                s_lib_cache[i].acquire_seq < oldest_inert->acquire_seq)
            {
                oldest_inert = &s_lib_cache[i];
            }
        }
        else if (oldest_other == NULL ||
                 s_lib_cache[i].acquire_seq < oldest_other->acquire_seq)
        {
            oldest_other = &s_lib_cache[i];
        }
    }
    LibCacheEntry *victim = oldest_inert != NULL ? oldest_inert : oldest_other;
    if (victim == NULL)
    {
        return NULL;
    }

    s_lib_evictions++;
    log_debug(DEBUG_MED,
              "animset cache: evicting %s slot (lib %p, seq %u, state %d) for lib %p; evictions %u",
              victim->state == LIB_INERT ? "INERT" : "CONFIGURED/FAILED", victim->lib,
              victim->acquire_seq, victim->state, movie_lib, s_lib_evictions);
    if (!s_lib_eviction_warned)
    {
        log_warning("Animset bridge: movie lib cache full (%d slots), evicting the oldest entry "
                    "(%u eviction(s) so far)",
                    SFSF_ANIMSET_LIB_CACHE_MAX, s_lib_evictions);
        s_lib_eviction_warned = true;
    }

    uint32_t seq = ++s_lib_acquire_seq;
    memset(victim, 0, sizeof(*victim));
    victim->lib = movie_lib;
    victim->acquire_seq = seq;
    return victim;
}

/* ---- default swing sound: donor cascade (variant B) -------------------- */

/* CDrwMovieEffectSound (0x44 bytes): only the fields cloned by the default
 * swing sound are mapped. */
typedef struct __attribute__((packed)) SF_CDrwMovieEffectSound
{
    void *vtable;                       /* +0x00 */
    uint8_t unknown_0x04[0x34 - 0x04];  /* range/mode fields + engine data, written by the engine */
    float offset;                       /* +0x34: SetSoundOffset target, seconds */
    uint32_t id;                        /* +0x38: DrwSound event id */
    float delay;                        /* +0x3C: seconds */
    float length;                       /* +0x40: seconds */
} SF_CDrwMovieEffectSound;

static_assert(offsetof(SF_CDrwMovieEffectSound, offset) == 0x34,
              "SF_CDrwMovieEffectSound.offset must sit at +0x34 (SetSoundOffset 0x86B8A0)");
static_assert(offsetof(SF_CDrwMovieEffectSound, id) == 0x38,
              "SF_CDrwMovieEffectSound.id must sit at +0x38 (AddSound 0x86A9E0)");
static_assert(offsetof(SF_CDrwMovieEffectSound, delay) == 0x3C,
              "SF_CDrwMovieEffectSound.delay must sit at +0x3C");
static_assert(offsetof(SF_CDrwMovieEffectSound, length) == 0x40,
              "SF_CDrwMovieEffectSound.length must sit at +0x40");
static_assert(sizeof(SF_CDrwMovieEffectSound) == 0x44,
              "SF_CDrwMovieEffectSound must be 0x44 bytes");

/**
 * @brief Exact-key movie lookup: descends the library's std::_Tree the way the
 * engine's dword comparator does (left when target < key, right when target >
 * key); variant bits of @p target_key are part of the comparison.
 * @return The movie stored under @p target_key, or NULL when absent.
 */
static SF_CDrwMovie *findMovieByKey(SF_CDrwMovieLibrary *library, uint32_t target_key)
{
    if (library == NULL || library->tree_head == NULL)
    {
        return NULL;
    }
    const SF_MovieTreeNode *node = library->tree_head->parent; /* root */
    while (node != NULL && node->is_nil == 0)
    {
        if (target_key < node->key)
        {
            node = node->left;
        }
        else if (target_key > node->key)
        {
            node = node->right;
        }
        else
        {
            return (SF_CDrwMovie *)node->movie;
        }
    }
    return NULL;
}

/** @brief In-order count of the tree nodes of one (job, subkey) key prefix:
 * how many film variants already stand under the key (variant bits +24). */
static void countMovieVariantsNode(const SF_MovieTreeNode *node, uint32_t base_key, int *count)
{
    if (node == NULL || node->is_nil != 0)
    {
        return;
    }
    countMovieVariantsNode(node->left, base_key, count);
    if ((node->key & 0x00FFFFFFu) == base_key)
    {
        (*count)++;
    }
    countMovieVariantsNode(node->right, base_key, count);
}

static int countMovieVariants(SF_CDrwMovieLibrary *library, uint32_t job, uint32_t subkey)
{
    int count = 0;
    if (library == NULL || library->tree_head == NULL)
    {
        return 0;
    }
    countMovieVariantsNode(library->tree_head->parent, job | (subkey << 16), &count);
    return count;
}

/**
 * @brief Reads the donor swing sound of one movie: the FIRST element of its
 * sound effect vector.
 *
 * @return false when the vector is empty, the first slot is not a
 * CDrwMovieEffectSound (vtable mismatch), or the effect carries no sound
 * (id 0) - the caller then tries the next cascade step.
 */
static bool donorSoundFromMovie(const SF_CDrwMovie *movie, void *expected_vftable,
                                int *out_id, float *out_offset, float *out_length)
{
    if (movie == NULL || movie->sound_effects_begin == NULL ||
        movie->sound_effects_begin >= movie->sound_effects_end)
    {
        return false; /* no sound effect vector (or empty): not a donor */
    }

    const SF_CDrwMovieEffectSound *effect =
        *(SF_CDrwMovieEffectSound *const *)movie->sound_effects_begin;
    if (effect == NULL || *(void **)effect != expected_vftable)
    {
        return false; /* polymorphic vector: the first slot is not a sound effect */
    }
    if (effect->id == 0)
    {
        return false; /* effect without a usable sound reference */
    }

    *out_id = (int)effect->id;
    *out_offset = effect->offset;
    *out_length = effect->length;
    return true;
}

/**
 * @brief Donor cascade for a pair without an explicit sound (owner decision
 * 2026-11-04), job = jobMap(weapon), all lookups at variant 0:
 * 1. subkey = weapon - the racial per-weapon attack film;
 * 2. subkey = 1 (kDrwWtDefault) - the weapon-type default;
 * 3. subkey = 0 (kDrwAnimDefault) - the job default (hero-male carries its
 *    swing sound exactly there).
 * The first film with a usable sound effect wins.
 * @return false when the whole cascade missed; the caller logs ONE line.
 */
static bool findDonorSwingSound(void *movie_lib, uint16_t weapon,
                                int *out_id, float *out_offset, float *out_length)
{
    /* Cascade order (owner decision 2026-11-04): the racial per-weapon film
     * first, then the weapon-type default (kDrwWtDefault = 1), then the job
     * default (kDrwAnimDefault = 0). All lookups are variant 0. */
    const uint16_t subkeys[3] = {weapon, 1, 0};
    uint32_t job = (uint32_t)animsetJobForWeapon(weapon);
    SF_CDrwMovieLibrary *library = (SF_CDrwMovieLibrary *)movie_lib;

    for (int i = 0; i < 3; i++)
    {
        SF_CDrwMovie *movie = findMovieByKey(library, job | ((uint32_t)subkeys[i] << 16));
        if (donorSoundFromMovie(movie, s_sound_effect_vftable, out_id, out_offset, out_length))
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief Ranged donor cascade for a shoot phase without an explicit sound
 * (owner decision 14.6, "analogous to melee"): (74, weapon) first, then
 * (74, 0) - the vanilla fire-arrow sound sits on the Range2 job default
 * (ofil l.1380). Aim phases never inherit a sound (vanilla aims silently).
 */
static bool findDonorRangedSound(void *movie_lib, uint16_t weapon,
                                 int *out_id, float *out_offset, float *out_length)
{
    const uint16_t subkeys[2] = {weapon, 0};
    uint32_t job = (uint32_t)kGdJobHitTargetRange2;
    SF_CDrwMovieLibrary *library = (SF_CDrwMovieLibrary *)movie_lib;

    for (int i = 0; i < 2; i++)
    {
        SF_CDrwMovie *movie = findMovieByKey(library, job | ((uint32_t)subkeys[i] << 16));
        if (donorSoundFromMovie(movie, s_sound_effect_vftable, out_id, out_offset, out_length))
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief Attaches one pair sound to a registered film (vanilla l.171-181
 * pattern), shared by the explicit json sound and the donor cascade.
 *
 * The sound source is resolved ONCE per pair by the caller
 * (registerPairFilms); an unresolved or unusable source has already been
 * logged there, so this function stays silent.
 */
static void attachPairSound(void *movie, int sound_id, float offset, float length)
{
    if (sound_id == 0)
    {
        return;
    }

    void *sound = s_add_sound(movie, sound_id, 0.0f, 0.0f);
    if (sound == NULL)
    {
        return;
    }
    s_set_sound_offset(sound, offset);
    if (length > 0.0f)
    {
        s_set_range_and_modes(sound, 0.0f, length, kDrwPlayLooped, 0, 0);
    }
}

/* Which default-sound cascade applies to a film set (TZ 8). */
typedef enum
{
    FILM_DONOR_NONE = 0,  /* ranged aim: explicit sound only (vanilla aims silently) */
    FILM_DONOR_MELEE,     /* shielded/solo: (jobMap(w), w|1|0) cascade */
    FILM_DONOR_RANGED     /* ranged shoot: (74, w) -> (74, 0) cascade */
} FilmDonorMode;

/* View of a record's sound definition shared by all three sections. */
typedef struct
{
    bool has_sound;
    const char *name;
    float offset;
    float length;
} FilmSetSoundRef;

/**
 * @brief Registers the films of one set under (job, subkey) - the common
 * engine path of all three v2 sections (generalizes the v1 pair registrar).
 *
 * @param synth non-NULL only for the shielded section: the synthetic subkey
 * 240..255 is allocated lazily with the *synth_used precheck (a fully skipped
 * set occupies no slot); when NULL, @p subkey (the weapon type itself for
 * solo, the phase's job key space for ranged) is used directly.
 *
 * Resolve gate, SetRangeAndModes(Stretched) and AddSound follow v1 §5.2 p.2
 * unchanged. The donor cascade resolves lazily after the first name passed
 * the gate, once per set.
 *
 * @return true when at least one film was registered.
 */
static bool registerFilmSet(void *movie_lib, int job, uint8_t subkey,
                            const char (*names)[64], int name_count,
                            const FilmSetSoundRef *sound, FilmDonorMode donor,
                            uint16_t donor_weapon, uint8_t *synth, uint8_t *synth_used)
{
    bool registered_any = false;
    int variant = 0;
    /* M<N snapshot (TZ 12.8), taken lazily at the first AddNewMovie of this
     * set: how many film variants already stand under the key. For the
     * shielded section the snapshot lands after the lazy synth allocation,
     * so it consults the REAL (job, synth) key - never the vanilla
     * (job, 0) default (review R2-1). */
    int variants_before = -1;

    /* Resolve the set's sound ONCE, before the film loop: one set with an
     * unknown name logs exactly one line, and an unusable DrwSound table
     * logs nothing here (its aggregate line already said everything). A set
     * without an explicit sound defers to its donor cascade inside the loop
     * (after the first name passed the resolve gate). */
    int sound_id = 0;
    float sound_offset = 0.0f;
    float sound_length = 0.0f;
    bool sound_checked = sound->has_sound;
    if (sound->has_sound)
    {
        sound_id = drwSoundIdFor(sound->name);
        if (sound_id == 0 && s_drsound_usable)
        {
            log_error("Animset bridge: sound \"%s\" not found in DrwSound.lua, skipped",
                      sound->name);
        }
        sound_offset = sound->offset;
        sound_length = sound->length;
    }

    for (int j = 0; j < name_count; j++)
    {
        const char *name = names[j];
        if (!animNameResolves(name))
        {
            log_error("Animset bridge: animation \"%s\" did not resolve, skipped", name);
            continue;
        }

        /* Default swing sound: resolve the donor after the first name passed
         * the gate, so a fully skipped set logs nothing here; exactly one
         * line per set when the whole cascade misses. */
        if (!sound_checked)
        {
            sound_checked = true;
            bool donor_found = false;
            if (donor == FILM_DONOR_MELEE)
            {
                donor_found = findDonorSwingSound(movie_lib, donor_weapon, &sound_id,
                                                  &sound_offset, &sound_length);
            }
            else if (donor == FILM_DONOR_RANGED)
            {
                donor_found = findDonorRangedSound(movie_lib, donor_weapon, &sound_id,
                                                   &sound_offset, &sound_length);
            }
            /* FILM_DONOR_NONE (ranged aim): intentionally silent, no cascade,
             * no miss line - the vanilla aim phases carry no sound either. */
            if (donor != FILM_DONOR_NONE && !donor_found)
            {
                /* Expected outcome for libs without a vanilla swing sound on
                 * the cascade keys (exotic NPC libs, soundless Punch blocks):
                 * a silent film is the documented README result - INFO, not
                 * an anomaly (review R2-3). */
                log_info("Animset bridge: no donor swing sound for weapon %d, film silent",
                         (int)donor_weapon);
            }
        }

        if (synth != NULL && *synth == 0)
        {
            *synth = SFSF_ANIMSET_SYNTH_FIRST + *synth_used;
            (*synth_used)++;
        }

        char boa_name[SFSF_ANIMSET_FILM_NAME_MAX];
        snprintf(boa_name, sizeof(boa_name), "%s.boa", name);
        int use_subkey = (synth != NULL) ? (int)*synth : (int)subkey;

        if (variants_before < 0)
        {
            variants_before = countMovieVariants((SF_CDrwMovieLibrary *)movie_lib,
                                                 (uint32_t)job, (uint32_t)use_subkey);
        }

        void *movie = s_add_new_movie(movie_lib, job, use_subkey, variant);
        if (movie == NULL)
        {
            log_error("Animset bridge: AddNewMovie failed for \"%s\", skipped", name);
            continue;
        }
        void *effect = s_add_animation(movie, boa_name);
        if (effect != NULL)
        {
            s_set_range_and_modes(effect, 0.0f, 1.0f, kDrwPlayStretched, 0, 0);
        }
        attachPairSound(movie, sound_id, sound_offset, sound_length);

        variant++;
        registered_any = true;
    }

    /* M<N mixing diagnostic (TZ 12.8), one INFO line per key, and ONLY when
     * the registration really leaves foreign variants behind: AddNewMovie
     * replaces variants 0..N-1, so films standing BEYOND our own count
     * (variants_before > variant) survive and will mix. A re-registration
     * over our own films (cache eviction -> reconfigure) counts exactly its
     * own variants and stays silent; for shielded the vanilla tree never
     * occupies the synth key, so the line is unreachable there (review
     * R2-1). */
    if (variants_before > variant)
    {
        log_info("Animset bridge: key (job %d, subkey %d) carries %d film(s), registering %d, "
                 "variants will mix after registration",
                 job, (int)((synth != NULL) ? *synth : subkey), variants_before, variant);
    }
    return registered_any;
}

/**
 * @brief Registers both phases of one ranged set: aim under (71, weapon)
 * without a donor (vanilla aims silently), shoot under (74, weapon) with the
 * ranged donor cascade; an explicit sound of the record lands on BOTH phases.
 * @return true when at least one film of either phase registered.
 */
static bool registerRangedFilms(void *movie_lib, const AnimsetRangedEntry *set)
{
    FilmSetSoundRef sound;
    sound.has_sound = set->has_sound;
    sound.name = set->sound_name;
    sound.offset = set->sound_offset;
    sound.length = set->sound_length;

    bool aim_ok = registerFilmSet(movie_lib, (int)kGdJobHitTargetRange1, (uint8_t)set->weapon,
                                  set->aim, (int)set->aim_count, &sound, FILM_DONOR_NONE,
                                  set->weapon, NULL, NULL);
    bool shoot_ok = registerFilmSet(movie_lib, (int)kGdJobHitTargetRange2, (uint8_t)set->weapon,
                                    set->shoot, (int)set->shoot_count, &sound, FILM_DONOR_RANGED,
                                    set->weapon, NULL, NULL);
    return aim_ok || shoot_ok;
}

/**
 * @brief Identifies the library against the catalog and registers its sets
 * (all three sections).
 *
 * The signature (leftmost key + node count) is captured at EVERY completion
 * for ANY final state (TZ 5.2 p.7): the next touch re-verifies INERT and
 * FAILED slots exactly like CONFIGURED ones, so an allocator-reused address
 * never keeps a stale verdict. Every inert outcome is INFO - with the v2
 * registration point every figure type of the map passes here once, a
 * library without a catalog record is the normal case, not an anomaly.
 */
static void configureLibEntry(LibCacheEntry *entry)
{
    /* Static instead of stack: 512 x 128 bytes = 64 KiB would be the largest
     * frame in the UpdateAnims call chain. The bridge is strictly
     * single-threaded (every touch comes from the main game loop - research
     * facts 1.2 p.9), so one shared buffer is safe. */
    static char names[SFSF_ANIMSET_LIB_FILM_CAP][SFSF_ANIMSET_FILM_NAME_MAX];
    LibWalkResult walk;
    memset(&walk, 0, sizeof(walk));
    walk.names = names;
    walk.names_cap = SFSF_ANIMSET_LIB_FILM_CAP;
    walkMovieLibrary(entry->lib, &walk);

    uint32_t signature_key = walk.leftmost_key;
    uint32_t signature_nodes = (uint32_t)walk.node_count;
    char base_name[SFSF_ANIMSET_FILM_NAME_MAX];
    const char *example_name = NULL;
    const AnimsetEntryRecord *record = NULL;
    bool registered_any = false;
    uint8_t synth_used = 0;
    bool taken[64]; /* registry records hold up to 64 shielded pairs */
    memset(taken, 0, sizeof(taken));
    memset(base_name, 0, sizeof(base_name));

    entry->pair_count = 0;
    entry->state = LIB_INERT;

    if (walk.name_count == 0)
    {
        log_debug(DEBUG_MED,
                  "Animset bridge: movie lib %p has no named films, animsets disabled for it",
                  entry->lib);
        goto finish;
    }
    if (walk.name_count == 1)
    {
        log_debug(DEBUG_MED,
                  "Animset bridge: movie lib %p has a single named film, identification skipped",
                  entry->lib);
        goto finish;
    }

    if (!recoverBaseName(names, walk.name_count, base_name, sizeof(base_name), &example_name))
    {
        log_debug(DEBUG_MED,
                  "Animset bridge: movie lib %p base_name could not be recovered, animsets disabled for it",
                  entry->lib);
        goto finish;
    }

    record = findAnimsetEntry(base_name);
    if (record == NULL)
    {
        log_debug(DEBUG_MED,
                  "Animset bridge: no animset record for movie lib %p (base_name \"%s\")",
                  entry->lib, base_name);
        goto finish;
    }

    /* Atomic refusal before the first AddNewMovie: 16 synthetic subkeys
     * (240..255) exist per library, the whole record is skipped when its
     * shielded section alone needs more (solo/ranged consume no synth). */
    if (record->shielded_count > SFSF_ANIMSET_MAX_PAIRS_PER_LIB)
    {
        entry->state = LIB_FAILED;
        log_error("Animset bridge: animset \"%s\" needs %d pairs, only %d synthetic subkeys exist",
                  record->figure, (int)record->shielded_count, SFSF_ANIMSET_MAX_PAIRS_PER_LIB);
        goto finish;
    }

    /* Shielded pairs in ascending weapon order, so subkey assignment is
     * deterministic; only this section carries the lazy synth allocator. */
    for (int round = 0; round < record->shielded_count; round++)
    {
        int next = -1;
        for (int i = 0; i < record->shielded_count; i++)
        {
            if (!taken[i] &&
                (next < 0 || record->shielded[i].weapon < record->shielded[next].weapon))
            {
                next = i;
            }
        }
        taken[next] = true;

        const AnimsetFilmSetEntry *pair = &record->shielded[next];
        FilmSetSoundRef sound;
        sound.has_sound = pair->has_sound;
        sound.name = pair->sound_name;
        sound.offset = pair->sound_offset;
        sound.length = pair->sound_length;

        uint8_t synth = 0;
        if (registerFilmSet(entry->lib, (int)animsetJobForWeapon(pair->weapon), 0,
                            pair->anims, (int)pair->anim_count, &sound, FILM_DONOR_MELEE,
                            pair->weapon, &synth, &synth_used))
        {
            entry->pairs[entry->pair_count].weapon = pair->weapon;
            entry->pairs[entry->pair_count].synth = synth;
            entry->pair_count++;
            registered_any = true;
        }
    }

    /* Solo sets: real subkeys (jobMap(w), w), no synth. */
    for (int i = 0; i < record->solo_count; i++)
    {
        const AnimsetFilmSetEntry *set = &record->solo[i];
        FilmSetSoundRef sound;
        sound.has_sound = set->has_sound;
        sound.name = set->sound_name;
        sound.offset = set->sound_offset;
        sound.length = set->sound_length;

        if (registerFilmSet(entry->lib, (int)animsetJobForWeapon(set->weapon),
                            (uint8_t)set->weapon, set->anims, (int)set->anim_count, &sound,
                            FILM_DONOR_MELEE, set->weapon, NULL, NULL))
        {
            registered_any = true;
        }
    }

    /* Ranged sets: two film sets per record (71 aim / 74 shoot). */
    for (int i = 0; i < record->ranged_count; i++)
    {
        if (registerRangedFilms(entry->lib, &record->ranged[i]))
        {
            registered_any = true;
        }
    }

    if (!registered_any)
    {
        goto finish; /* every name was skipped above, one line per name already logged */
    }

    /* Capture the signature AFTER registration: the guard must verify
     * against the steady state that includes the registered films. */
    {
        LibWalkResult signature;
        memset(&signature, 0, sizeof(signature));
        walkMovieLibrary(entry->lib, &signature);
        signature_key = signature.leftmost_key;
        signature_nodes = (uint32_t)signature.node_count;
    }
    entry->state = LIB_CONFIGURED;

    log_info("Animset bridge: movie lib %p matched animset \"%s\" (base_name \"%s\", e.g. film \"%s\")",
             entry->lib, record->figure, base_name, example_name);

finish:
    entry->leftmost_key = signature_key;
    entry->node_count = signature_nodes;
    logCacheSnapshot("configure", entry);
}

void ensureMovieLibConfigured(void *movie_lib)
{
    /* Empty registry (no sfsf\animsets folder, empty catalog) is a normal
     * install: short-circuit before any tree walk, log line or cache slot,
     * so every figure of a vanilla install costs nothing and stays silent. */
    if (animsetRegistryEmpty())
    {
        return;
    }
    if (movie_lib == NULL || s_add_new_movie == NULL)
    {
        return;
    }

    LibCacheEntry *entry = findLibCacheEntry(movie_lib);
    if (entry == NULL)
    {
        entry = acquireLibCacheEntry(movie_lib);
        if (entry == NULL)
        {
            return;
        }
        configureLibEntry(entry);
        return;
    }

    /* Re-verify EVERY state (TZ 5.2 p.7): the registration point touches all
     * figure types of the map, and a reused library address with a stale
     * INERT/FAILED verdict would silently lose the feature. The failure
     * direction stays safe - a re-configure only re-runs the registration. */
    LibWalkResult now;
    memset(&now, 0, sizeof(now));
    walkMovieLibrary(movie_lib, &now);
    if (now.leftmost_key != entry->leftmost_key || (uint32_t)now.node_count != entry->node_count)
    {
        log_debug(DEBUG_MED,
                  "animset cache: signature drift at lib %p (key 0x%08x nodes %u -> key 0x%08x nodes %u), re-evaluating",
                  movie_lib, entry->leftmost_key, entry->node_count, now.leftmost_key,
                  (uint32_t)now.node_count);
        log_info("Animset bridge: movie lib %p changed, reconfiguring animsets", movie_lib);
        configureLibEntry(entry);
    }
}

uint8_t lookupShieldSynth(void *movie_lib, uint16_t weapon_type)
{
    /* Empty registry (no sfsf\animsets folder, empty catalog) is a normal
     * install: short-circuit before any tree walk, log line or cache slot,
     * so vanilla shield bearers cost nothing and stay silent. */
    if (animsetRegistryEmpty())
    {
        return 0;
    }
    if (movie_lib == NULL || s_add_new_movie == NULL)
    {
        return 0;
    }

    /* Find/acquire/verify - idempotent over a warm cache; the shield branch
     * of the hook keeps its exact v1 contract. */
    ensureMovieLibConfigured(movie_lib);

    LibCacheEntry *entry = findLibCacheEntry(movie_lib);
    if (entry == NULL || entry->state != LIB_CONFIGURED)
    {
        return 0;
    }
    for (int i = 0; i < entry->pair_count; i++)
    {
        if (entry->pairs[i].weapon == weapon_type)
        {
            return entry->pairs[i].synth;
        }
    }
    return 0;
}

uint32_t animsetLibCacheEvictionCount()
{
    return s_lib_evictions;
}

/** @} */
