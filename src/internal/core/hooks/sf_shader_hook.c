/**
 * @addtogroup ShaderHook
 * @{
 */

#include <windows.h>
#include <d3d9.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sf_shader_hook.h"
#include "../sf_shader_module.h"
#include "../sf_wrappers.h"

/* Preamble for a bit of context on this hackjob.
 * SpellForce imports no graphics DLL.
 *
 * D3DUtil loads System32\d3d9.dll by
 * absolute path and resolves Direct3DCreate9 with GetProcAddress. SFSF is
 * loaded after the display has already been built.
 *
 * So we just get the game engine to hand us the device instead.
 * The CALL to DSP_Display::BeginScene
 * in CAppMenu::RenderFrame is rewritten to point here:
 *
 *   00597bcc  MOV  ECX, dword ptr [PTR_00d28f5c]   ; the DSP_Display singleton
 *   00597bd2  CALL DSP_Display::BeginScene         ; -> 0x5cd180
 *
 * That gives us DSP_Display as `this`, on the render thread, inside a live
 * frame. From there we grab the device (see find_device) and Present and Reset
 * are patched in its vtable. Everything after that is ordinary D3D9.
 */

#define VT_IDIRECT3DDEVICE9_RESET   16
#define VT_IDIRECT3DDEVICE9_PRESENT 17

/** CALL DSP_Display::BeginScene inside CAppMenu::RenderFrame (Ghidra 0x597bd2). */
#define SFSF_ADDR_BEGINSCENE_CALL 0x197bd2

/** DSP_Display::BeginScene (Ghidra 0x5cd180). */
#define SFSF_ADDR_DSP_BEGINSCENE 0x1cd180

/**
 * IDirect3DDevice9 * inside DSP_Display, per DSP_Display::OpenResolution
 * (005cf21f) and DSP_Display::BeginScene (005cd180). Not populated when
 * the hook first runs
 */
#define SFSF_OFFSET_DSP_DEVICE 0x338

/**
 * First entry of the vertex buffer array. Not the device just some
 * IDirect3DResource9, which we can then 'GetDevice' on to get the device we need.
 */
#define SFSF_OFFSET_DSP_RESOURCE 0x490

/** INTZ: a depth-stencil format that is also sampleable. Not in d3d9.h. */
#define FOURCC_INTZ ((D3DFORMAT)MAKEFOURCC('I','N','T','Z'))

/** IDirect3DDevice9::SetDepthStencilSurface (vtable +0x9C). */
#define VT_IDIRECT3DDEVICE9_SETDEPTHSTENCIL 39

typedef HRESULT (WINAPI *PFN_SetDepthStencil)(IDirect3DDevice9 *, IDirect3DSurface9 *);

static IDirect3DTexture9 *s_depth_texture = NULL;
static IDirect3DSurface9 *s_depth_surface = NULL;
static bool s_depth_available = false;
static PFN_SetDepthStencil s_real_setdepthstencil = NULL;

/* |-========== World space ==========-|
 *
 * The engine drives its transforms through vertex shader constants - the
 * fixed-function VIEW/PROJECTION/WORLD matrices all read back as identity. The
 * view-projection lives in c3..c6, which was identified from a live dump:
 *
 *   c3  [ 1.000   0.000   0.000   179.200]
 *   c4  [ 0.000  -0.669  -0.743   -45.547]   c3..c5 are orthonormal (a rotation
 *   c5  [ 0.000  -0.743   0.669  -201.115]   with translation in .w, ~48 deg pitch)
 *   c6  [ 0.000  -0.743   0.669  -200.095]   same direction as c5, .w differs by
 *                                            1.02 -> the projection's w row,
 *                                            near plane 1.02
 *
 * Constants are last-write-wins and the last thing drawn each frame is UI, so
 * the matrix is snapshotted at the first draw call instead, when world geometry
 * is being rendered.
 */

/** IDirect3DDevice9::SetVertexShaderConstantF (vtable +0x178). */
#define VT_IDIRECT3DDEVICE9_SETVSCONSTF 94

/** IDirect3DDevice9::DrawIndexedPrimitive (vtable +0x148, per DSP_Display decomp). */
#define VT_IDIRECT3DDEVICE9_DRAWINDEXEDPRIM 82

/**
 * Registers to shadow. c3..c6 turned out to be a world-view-projection - it
 * changes per draw - so the pure view-projection is elsewhere and the range has
 * to be wide enough to find it.
 */
#define SFSF_VS_CONST_TRACK 96

/** First register of the view-projection matrix. */
#define SFSF_VS_VIEWPROJ_BASE 3

/* |-========== A note on the engine's render-data interfaces ==========-|
 *
 * FUN_005de200 registers a DSP_RenderDataInterface family - GetObjectToWorld,
 * GetObjectToClip, GetCameraWorld, GetCameraObject, SecondPass, LightPosition,
 * AmbientLight and the texture providers. Reading them looked promising, but
 * every global they source from - the transform block at 0xd28f68, the light
 * positions from 0xd29444 - reads as zeros while rendering, so the engine does
 * not drive that path in this build. There is no GetWorldToClip either, so a
 * pure view-projection is not known currently.
 *
 * We still have c3..c6, which is DSP_GetObjectToClip: a WORLD-view-projection
 * that changes per object. The view-projection is recovered from it by picking
 * a draw whose world matrix is identity - see consider_view_proj.
 */

static float s_camera_world[3] = { 0.0f, 0.0f, 0.0f };
static bool  s_camera_valid    = false;

typedef HRESULT (WINAPI *PFN_SetVSConstF)(IDirect3DDevice9 *, UINT, const float *, UINT);
typedef HRESULT (WINAPI *PFN_DrawIndexedPrimitive)(IDirect3DDevice9 *, D3DPRIMITIVETYPE,
                                                   INT, UINT, UINT, UINT, UINT);

static PFN_SetVSConstF          s_real_setvsconstf = NULL;
static PFN_DrawIndexedPrimitive s_real_drawindexed = NULL;

static float s_vs_constants[SFSF_VS_CONST_TRACK][4];
static bool  s_vs_seen[SFSF_VS_CONST_TRACK];

/**
 * Per-frame variance tracking. A register that changes between draws within one
 * frame is per-object data; one that holds still all frame but changes when the
 * camera moves is view state. That can help locate the pure
 * view-projection from the world-view-projection in c3..c6.
 */
static float s_vs_frame_start[SFSF_VS_CONST_TRACK][4];
static bool  s_vs_varies_in_frame[SFSF_VS_CONST_TRACK];

static float s_view_proj[4][4];
static bool  s_view_proj_valid     = false;
static bool  s_frame_snapshot_done = false;

/**
 * True while the main scene's depth target is bound. Shadow passes bind their
 * own smaller surfaces, so this distinguishes the scene camera's matrix from a
 * light's - without it, the snapshot picks up whichever pass happens to draw
 * first, which changes as shadow casters enter and leave the view.
 */
static bool s_main_pass_active = false;

/** Frames left to trace draw calls for. Armed by the F10 diagnostic. */
static int s_trace_draws = 0;
static int s_draw_index  = 0;

/**
 * Per-frame search for the view-projection.
 *
 * The engine doesn't appear to upload a pure view-projection - world is multiplied in on
 * the CPU, so c3..c6 differs per object (every register from c2 to c93 tests as
 * per-object). What separates the real one is that the camera sits at the eye
 * as in position of camera itself means a true view-projection should map to w = 0:
 *
 *   residual = |dot(row3.xyz, cameraWorld) + row3.w|
 *
 * A world-view-projection only satisfies that if its world matrix fixes the
 * eye, which in practice means identity / the terrain. Every main-pass draw is
 * scored as it happens and the best is kept.
 */
static float s_best_matrix[4][4];
static int   s_best_prims    = 0;
static int   s_winning_prims = 0;
static bool  s_have_best     = false;

/**
 * @brief Keeps the matrix belonging to the frame's largest draw. (super hacky probably shouldn't do that)
 *
 * Terrain is drawn with an identity world matrix - a terrain-only map
 * reconstructs perfectly - and terrain chunks are much the biggest draws,
 * hundreds to thousands of primitives against tens for props. The largest draw
 * therefore carries a matrix whose world is identity, which is the
 * view-projection.
 *
 * This replaced a check to get the engine's DSP_GetCameraWorld
 * but when attempting to read it, the globals are all zero.
 */
static void consider_view_proj(const float matrix[4][4], UINT primitive_count)
{
    if ((int)primitive_count <= s_best_prims)
    {
        return;
    }

    memcpy(s_best_matrix, matrix, sizeof(s_best_matrix));
    s_best_prims = (int)primitive_count;
    s_have_best  = true;
}

/**
 * @brief Recovers the camera position from a view-projection.
 *
 * The eye is the one world point mapping to x = y = w = 0 in clip space, so
 * rows 0, 1 and 3 give three equations in three unknowns, solved by Cramer's
 * rule. This is how we acoid asking for camera data so far.
 */
static bool camera_from_view_proj(const float m[4][4], float out[3])
{
    const float a[3][3] = {
        { m[0][0], m[0][1], m[0][2] },
        { m[1][0], m[1][1], m[1][2] },
        { m[3][0], m[3][1], m[3][2] },
    };
    const float b[3] = { -m[0][3], -m[1][3], -m[3][3] };

    float det = a[0][0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1])
              - a[0][1] * (a[1][0] * a[2][2] - a[1][2] * a[2][0])
              + a[0][2] * (a[1][0] * a[2][1] - a[1][1] * a[2][0]);

    if (det > -1e-9f && det < 1e-9f)
    {
        return false;
    }

    for (int col = 0; col < 3; col++)
    {
        float c[3][3];
        memcpy(c, a, sizeof(c));
        c[0][col] = b[0];
        c[1][col] = b[1];
        c[2][col] = b[2];

        float d = c[0][0] * (c[1][1] * c[2][2] - c[1][2] * c[2][1])
                - c[0][1] * (c[1][0] * c[2][2] - c[1][2] * c[2][0])
                + c[0][2] * (c[1][0] * c[2][1] - c[1][1] * c[2][0]);

        out[col] = d / det;
    }

    return true;
}

/** @brief Promotes the frame's winner and recovers the camera from it. */
static void resolve_view_proj()
{
    if (s_have_best)
    {
        memcpy(s_view_proj, s_best_matrix, sizeof(s_view_proj));
        s_view_proj_valid = true;
        s_winning_prims   = s_best_prims;
        s_camera_valid    = camera_from_view_proj(s_view_proj, s_camera_world);
    }

    s_have_best  = false;
    s_best_prims = 0;
}

/* Minimal ID3DBlob
 * Declared here rather than pulling in all of d3dcommon.h
 */
typedef struct SFBlob SFBlob;
typedef struct
{
    HRESULT (WINAPI *QueryInterface)(SFBlob *, REFIID, void **);
    ULONG   (WINAPI *AddRef)(SFBlob *);
    ULONG   (WINAPI *Release)(SFBlob *);
    LPVOID  (WINAPI *GetBufferPointer)(SFBlob *);
    SIZE_T  (WINAPI *GetBufferSize)(SFBlob *);
} SFBlobVtbl;
struct SFBlob
{
    SFBlobVtbl *lpVtbl;
};

typedef HRESULT (WINAPI *PFN_D3DCompile)(LPCVOID src, SIZE_T src_size,
                                         LPCSTR source_name, const void *defines,
                                         void *include, LPCSTR entry_point,
                                         LPCSTR target, UINT flags1, UINT flags2,
                                         SFBlob **code, SFBlob **errors);

/** DSP_Display::BeginScene - __thiscall, returns non-zero on success. */
typedef int (__thiscall *PFN_DspBeginScene)(void *dsp_display);
typedef HRESULT (WINAPI *PFN_Present)(IDirect3DDevice9 *, const RECT *, const RECT *,
                                      HWND, const RGNDATA *);
typedef HRESULT (WINAPI *PFN_Reset)(IDirect3DDevice9 *, D3DPRESENT_PARAMETERS *);

typedef struct
{
    IDirect3DPixelShader9 *shader;
    char name[SHADER_NAME_LEN];
} CompiledPass;

typedef struct
{
    float x, y, z, rhw;
    float u, v;
} QuadVertex;

#define QUAD_FVF (D3DFVF_XYZRHW | D3DFVF_TEX1)

static CompiledPass s_passes[MAX_SHADER_ENTRIES];
static int s_pass_count = 0;

/**
 * Which passes actually run. Every declared pass is compiled regardless of its
 * "enabled" flag so it can be switched on at runtime; this array starts from
 * the manifest and is then driven by the F11 solo cycle.
 */
static bool s_pass_enabled[MAX_SHADER_ENTRIES];

/** -1 = follow the manifest, otherwise the index of the single soloed pass. */
static int s_solo_pass = -1;

static IDirect3DDevice9     *s_device          = NULL;
static IDirect3DTexture9    *s_capture_texture = NULL;
static IDirect3DSurface9    *s_capture_surface = NULL;
static IDirect3DStateBlock9 *s_state_block     = NULL;
static UINT s_capture_width  = 0;
static UINT s_capture_height = 0;

static bool  s_shaders_compiled = false;
static bool  s_pipeline_live    = false;
static bool  s_resources_ready  = false;
static bool  s_in_hook          = false;
static DWORD s_start_tick       = 0;

static PFN_Present    s_real_present = NULL;
static PFN_Reset      s_real_reset   = NULL;
static PFN_D3DCompile s_d3d_compile  = NULL;

static PFN_DspBeginScene s_real_dsp_beginscene = NULL;
static bool s_device_capture_done = false;

/**
 * @brief True when the whole span at @p address is committed and readable.
 */
static bool is_readable(const void *address, size_t length)
{
    if (address == NULL)
    {
        return false;
    }

    MEMORY_BASIC_INFORMATION info;
    if (VirtualQuery(address, &info, sizeof(info)) == 0)
    {
        return false;
    }

    if (info.State != MEM_COMMIT)
    {
        return false;
    }

    const DWORD readable = PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY |
                           PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE |
                           PAGE_EXECUTE_WRITECOPY;
    if ((info.Protect & readable) == 0 || (info.Protect & PAGE_GUARD) != 0)
    {
        return false;
    }

    const BYTE *region_end = (const BYTE *)info.BaseAddress + info.RegionSize;
    return ((const BYTE *)address + length) <= region_end;
}

/**
 * @brief True when @p pointer lies inside the loaded image of @p dll.
 *
 * Used to confirm a candidate device really is a d3d9 object before its vtable
 * is patched, just a safety net as this whole area isn't decompiled well, and is poorly understood.
 */
static bool is_within_module(const void *pointer, const char *dll)
{
    HMODULE module = GetModuleHandleA(dll);
    if (module == NULL)
    {
        return false;
    }

    IMAGE_DOS_HEADER *dos = (IMAGE_DOS_HEADER *)module;
    if (dos->e_magic != IMAGE_DOS_SIGNATURE)
    {
        return false;
    }

    IMAGE_NT_HEADERS *nt = (IMAGE_NT_HEADERS *)((BYTE *)module + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE)
    {
        return false;
    }

    const BYTE *base = (const BYTE *)module;

    /* Past the PE headers deliberately. D3DUtil's first field is the HMODULE
     * for d3d9.dll, which IS the module base - so a plain range test lets that
     * struct pass as a COM object. */
    const BYTE *sections_begin = base + nt->OptionalHeader.SizeOfHeaders;

    return ((const BYTE *)pointer >= sections_begin &&
            (const BYTE *)pointer < base + nt->OptionalHeader.SizeOfImage);
}

/** @brief Swaps one slot of a COM object's vtable, returning the old pointer. */
static bool patch_vtable(void *com_object, int index, void *replacement, void **out_original)
{
    if (com_object == NULL)
    {
        return false;
    }

    void **vtable = *(void ***)com_object;
    DWORD old_protect = 0;

    if (!VirtualProtect(&vtable[index], sizeof(void *), PAGE_READWRITE, &old_protect))
    {
        log_error("| - Shader: could not unprotect vtable slot %d (%lu)",
                  index, GetLastError());
        return false;
    }

    *out_original = vtable[index];
    vtable[index] = replacement;
    VirtualProtect(&vtable[index], sizeof(void *), old_protect, &old_protect);
    FlushInstructionCache(GetCurrentProcess(), &vtable[index], sizeof(void *));
    return true;
}

/**
 * @brief General 4x4 inverse.
 *
 * Done on the CPU once per frame rather than in HLSL, where a full inverse
 * would cost far more than the whole post-process pass.
 *
 * @return false when the matrix is singular.
 */
static bool invert_4x4(const float m[4][4], float out[4][4])
{
    float a[16], inv[16];
    memcpy(a, m, sizeof(a));

    inv[0]  =  a[5]*a[10]*a[15] - a[5]*a[11]*a[14] - a[9]*a[6]*a[15]
             + a[9]*a[7]*a[14] + a[13]*a[6]*a[11] - a[13]*a[7]*a[10];
    inv[4]  = -a[4]*a[10]*a[15] + a[4]*a[11]*a[14] + a[8]*a[6]*a[15]
             - a[8]*a[7]*a[14] - a[12]*a[6]*a[11] + a[12]*a[7]*a[10];
    inv[8]  =  a[4]*a[9]*a[15] - a[4]*a[11]*a[13] - a[8]*a[5]*a[15]
             + a[8]*a[7]*a[13] + a[12]*a[5]*a[11] - a[12]*a[7]*a[9];
    inv[12] = -a[4]*a[9]*a[14] + a[4]*a[10]*a[13] + a[8]*a[5]*a[14]
             - a[8]*a[6]*a[13] - a[12]*a[5]*a[10] + a[12]*a[6]*a[9];
    inv[1]  = -a[1]*a[10]*a[15] + a[1]*a[11]*a[14] + a[9]*a[2]*a[15]
             - a[9]*a[3]*a[14] - a[13]*a[2]*a[11] + a[13]*a[3]*a[10];
    inv[5]  =  a[0]*a[10]*a[15] - a[0]*a[11]*a[14] - a[8]*a[2]*a[15]
             + a[8]*a[3]*a[14] + a[12]*a[2]*a[11] - a[12]*a[3]*a[10];
    inv[9]  = -a[0]*a[9]*a[15] + a[0]*a[11]*a[13] + a[8]*a[1]*a[15]
             - a[8]*a[3]*a[13] - a[12]*a[1]*a[11] + a[12]*a[3]*a[9];
    inv[13] =  a[0]*a[9]*a[14] - a[0]*a[10]*a[13] - a[8]*a[1]*a[14]
             + a[8]*a[2]*a[13] + a[12]*a[1]*a[10] - a[12]*a[2]*a[9];
    inv[2]  =  a[1]*a[6]*a[15] - a[1]*a[7]*a[14] - a[5]*a[2]*a[15]
             + a[5]*a[3]*a[14] + a[13]*a[2]*a[7] - a[13]*a[3]*a[6];
    inv[6]  = -a[0]*a[6]*a[15] + a[0]*a[7]*a[14] + a[4]*a[2]*a[15]
             - a[4]*a[3]*a[14] - a[12]*a[2]*a[7] + a[12]*a[3]*a[6];
    inv[10] =  a[0]*a[5]*a[15] - a[0]*a[7]*a[13] - a[4]*a[1]*a[15]
             + a[4]*a[3]*a[13] + a[12]*a[1]*a[7] - a[12]*a[3]*a[5];
    inv[14] = -a[0]*a[5]*a[14] + a[0]*a[6]*a[13] + a[4]*a[1]*a[14]
             - a[4]*a[2]*a[13] - a[12]*a[1]*a[6] + a[12]*a[2]*a[5];
    inv[3]  = -a[1]*a[6]*a[11] + a[1]*a[7]*a[10] + a[5]*a[2]*a[11]
             - a[5]*a[3]*a[10] - a[9]*a[2]*a[7] + a[9]*a[3]*a[6];
    inv[7]  =  a[0]*a[6]*a[11] - a[0]*a[7]*a[10] - a[4]*a[2]*a[11]
             + a[4]*a[3]*a[10] + a[8]*a[2]*a[7] - a[8]*a[3]*a[6];
    inv[11] = -a[0]*a[5]*a[11] + a[0]*a[7]*a[9] + a[4]*a[1]*a[11]
             - a[4]*a[3]*a[9] - a[8]*a[1]*a[7] + a[8]*a[3]*a[5];
    inv[15] =  a[0]*a[5]*a[10] - a[0]*a[6]*a[9] - a[4]*a[1]*a[10]
             + a[4]*a[2]*a[9] + a[8]*a[1]*a[6] - a[8]*a[2]*a[5];

    float det = a[0]*inv[0] + a[1]*inv[4] + a[2]*inv[8] + a[3]*inv[12];
    if (det == 0.0f)
    {
        return false;
    }

    det = 1.0f / det;
    for (int i = 0; i < 16; i++)
    {
        ((float *)out)[i] = inv[i] * det;
    }
    return true;
}

/**
 * @brief Logs what the engine's own depth-stencil looks like.
 *
 * MSAA must be NONE for the INTZ swap to work, and the surface should match the
 * backbuffer - a smaller one means we are looking at a shadow pass rather than
 * the main scene.
 */
static void log_engine_depth_surface()
{
    IDirect3DSurface9 *depth = NULL;
    if (FAILED(s_device->GetDepthStencilSurface(&depth)) || depth == NULL)
    {
        log_warning("| - Shader: the device has no depth-stencil surface bound");
        return;
    }

    D3DSURFACE_DESC desc;
    if (SUCCEEDED(depth->GetDesc(&desc)))
    {
        log_info("| - Shader: engine depth %ux%u format 0x%08X (%c%c%c%c) msaa %d",
                 desc.Width, desc.Height, (unsigned)desc.Format,
                 (char)((desc.Format      ) & 0xFF), (char)((desc.Format >>  8) & 0xFF),
                 (char)((desc.Format >> 16) & 0xFF), (char)((desc.Format >> 24) & 0xFF),
                 (int)desc.MultiSampleType);
    }

    depth->Release();
}

/**
 * @brief Creates a depth-stencil that can also be sampled, when supported.
 *
 * D3D9 depth surfaces are opaque - they cannot be locked, copied or bound as a
 * texture. INTZ is the long-standing FOURCC that lifts that restriction, and is
 * supported on essentially every driver since ~2008. Failure here is not fatal:
 * depth passes are disabled and colour passes continue.
 */
static bool create_depth_resources(const D3DSURFACE_DESC *backbuffer_desc)
{
    s_depth_available = false;

    if (backbuffer_desc->MultiSampleType != D3DMULTISAMPLE_NONE)
    {
        report_shader_error("backbuffer is multisampled (%d) - INTZ depth cannot be "
                            "used, depth passes disabled",
                            (int)backbuffer_desc->MultiSampleType);
        return false;
    }

    IDirect3D9 *d3d9 = NULL;
    if (FAILED(s_device->GetDirect3D(&d3d9)) || d3d9 == NULL)
    {
        report_shader_error("could not reach IDirect3D9 to check INTZ support");
        return false;
    }

    D3DDEVICE_CREATION_PARAMETERS creation;
    D3DDISPLAYMODE mode;
    HRESULT hr = E_FAIL;

    if (SUCCEEDED(s_device->GetCreationParameters(&creation)) &&
        SUCCEEDED(d3d9->GetAdapterDisplayMode(creation.AdapterOrdinal, &mode)))
    {
        hr = d3d9->CheckDeviceFormat(creation.AdapterOrdinal, creation.DeviceType,
                                     mode.Format, D3DUSAGE_DEPTHSTENCIL,
                                     D3DRTYPE_TEXTURE, FOURCC_INTZ);
    }
    d3d9->Release();

    if (FAILED(hr))
    {
        report_shader_error("driver does not support INTZ - depth passes disabled");
        return false;
    }

    hr = s_device->CreateTexture(backbuffer_desc->Width, backbuffer_desc->Height, 1,
                                 D3DUSAGE_DEPTHSTENCIL, FOURCC_INTZ,
                                 D3DPOOL_DEFAULT, &s_depth_texture, NULL);
    if (FAILED(hr) || s_depth_texture == NULL)
    {
        report_shader_error("could not create the INTZ depth texture (hr 0x%08lX)",
                            (unsigned long)hr);
        return false;
    }

    if (FAILED(s_depth_texture->GetSurfaceLevel(0, &s_depth_surface)))
    {
        report_shader_error("could not get the INTZ depth surface");
        s_depth_texture->Release();
        s_depth_texture = NULL;
        return false;
    }

    s_depth_available = true;
    log_info("| - Shader: INTZ depth target created at %ux%u",
             backbuffer_desc->Width, backbuffer_desc->Height);
    return true;
}

/**
 * @brief Redirects main-scene depth binds to the INTZ surface.
 *
 * Only binds matching the backbuffer size are swapped. Shadow passes use their
 * own smaller surfaces and are left alone - redirecting those would both break
 * shadows and hand the shader a shadow map instead of scene depth.
 */
static HRESULT WINAPI sf_setdepthstencil_hook(IDirect3DDevice9 *device,
                                              IDirect3DSurface9 *surface)
{
    if (device == s_device)
    {
        if (surface == NULL)
        {
            s_main_pass_active = false;
        }
        else if (surface == s_depth_surface)
        {
            s_main_pass_active = true;
        }
        else
        {
            D3DSURFACE_DESC desc;
            bool is_main = SUCCEEDED(surface->GetDesc(&desc)) &&
                           desc.Width == s_capture_width &&
                           desc.Height == s_capture_height;

            s_main_pass_active = is_main;

            if (is_main && s_depth_available)
            {
                surface = s_depth_surface;
            }
        }
    }

    return s_real_setdepthstencil(device, surface);
}

/** @brief Shadows the vertex shader constants the engine sets. */
static HRESULT WINAPI sf_setvsconstf_hook(IDirect3DDevice9 *device, UINT start_register,
                                          const float *data, UINT count)
{
    if (device == s_device && data != NULL && start_register < SFSF_VS_CONST_TRACK)
    {
        UINT n = count;
        if (start_register + n > SFSF_VS_CONST_TRACK)
        {
            n = SFSF_VS_CONST_TRACK - start_register;
        }
        memcpy(&s_vs_constants[start_register][0], data, n * 4 * sizeof(float));
        for (UINT i = 0; i < n; i++)
        {
            UINT reg = start_register + i;
            s_vs_seen[reg] = true;

            if (memcmp(&s_vs_constants[reg][0], &s_vs_frame_start[reg][0],
                       4 * sizeof(float)) != 0)
            {
                s_vs_varies_in_frame[reg] = true;
            }
        }
    }

    return s_real_setvsconstf(device, start_register, data, count);
}

/**
 * @brief Snapshots the view-projection at the first draw of each frame.
 *
 * The constants are last-write-wins and the last thing drawn is UI, so the end
 * of frame state is the wrong matrix. World geometry goes first.
 */
static HRESULT WINAPI sf_drawindexed_hook(IDirect3DDevice9 *device, D3DPRIMITIVETYPE type,
                                          INT base_vertex, UINT min_index, UINT num_vertices,
                                          UINT start_index, UINT primitive_count)
{
    if (device == s_device)
    {
        s_draw_index++;

        /* Only the main scene pass carries the camera's matrix. */
        if (s_main_pass_active)
        {
            consider_view_proj((const float (*)[4])&s_vs_constants[SFSF_VS_VIEWPROJ_BASE][0],
                               primitive_count);
        }

        /* Bounded trace: enough to see which pass draws first, not enough to
         * flood the console. */
        if (s_trace_draws > 0 && s_draw_index <= 8)
        {
            log_info("| - Shader:   draw %d: %u prims, main_pass=%d, "
                     "c3=[%7.3f %7.3f %7.3f %9.3f] c6=[%7.3f %7.3f %7.3f %9.3f]",
                     s_draw_index, primitive_count, (int)s_main_pass_active,
                     s_vs_constants[3][0], s_vs_constants[3][1],
                     s_vs_constants[3][2], s_vs_constants[3][3],
                     s_vs_constants[6][0], s_vs_constants[6][1],
                     s_vs_constants[6][2], s_vs_constants[6][3]);
        }
    }

    return s_real_drawindexed(device, type, base_vertex, min_index, num_vertices,
                              start_index, primitive_count);
}

static bool load_compiler()
{
    if (s_d3d_compile != NULL)
    {
        return true;
    }

    static const char *k_compilers[] = {
        "d3dcompiler_47.dll", "d3dcompiler_46.dll", "d3dcompiler_43.dll", NULL
    };

    for (int i = 0; k_compilers[i] != NULL; i++)
    {
        HMODULE module = LoadLibraryA(k_compilers[i]);
        if (module == NULL)
        {
            continue;
        }
        s_d3d_compile = (PFN_D3DCompile)GetProcAddress(module, "D3DCompile");
        if (s_d3d_compile != NULL)
        {
            log_info("| - Shader: using %s to compile HLSL", k_compilers[i]);
            return true;
        }
    }

    report_shader_error("no D3D shader compiler found (d3dcompiler_47.dll and "
                        "friends are all missing) - HLSL sources cannot be built");
    return false;
}

/** @brief Highest profile this device can actually run, or NULL if none. */
static const char *pick_profile()
{
    D3DCAPS9 caps;
    memset(&caps, 0, sizeof(caps));

    if (FAILED(s_device->GetDeviceCaps(&caps)))
    {
        log_warning("| - Shader: GetDeviceCaps failed, assuming %s",
                    SFSF_SHADER_PROFILE_FALLBACK);
        return SFSF_SHADER_PROFILE_FALLBACK;
    }

    log_info("| - Shader: device pixel shader version %d.%d",
             (caps.PixelShaderVersion >> 8) & 0xFF, caps.PixelShaderVersion & 0xFF);

    if (caps.PixelShaderVersion < D3DPS_VERSION(2, 0))
    {
        return NULL;
    }
    if (caps.PixelShaderVersion < D3DPS_VERSION(3, 0))
    {
        return SFSF_SHADER_PROFILE_FALLBACK;
    }
    return SFSF_SHADER_PROFILE;
}

static bool compile_pass(const ShaderEntry *entry, const char *source,
                         const char *profile, CompiledPass *out_pass)
{
    SFBlob *code = NULL;
    SFBlob *errors = NULL;

    HRESULT hr = s_d3d_compile(source, strlen(source), entry->fragment_file,
                               NULL, NULL, "main", profile, 0, 0, &code, &errors);

    if (errors != NULL)
    {
        const char *text = (const char *)errors->lpVtbl->GetBufferPointer(errors);
        log_debug(FAILED(hr) ? DEBUG_INFO : DEBUG_MED, "| - Shader: %s: %s",
                  entry->name, text ? text : "(no message)");
        errors->lpVtbl->Release(errors);
    }

    if (FAILED(hr) || code == NULL)
    {
        report_shader_error("%s failed to compile (hr 0x%08lX)",
                            entry->name, (unsigned long)hr);
        if (code != NULL)
        {
            code->lpVtbl->Release(code);
        }
        return false;
    }

    IDirect3DPixelShader9 *shader = NULL;
    hr = s_device->CreatePixelShader((const DWORD *)code->lpVtbl->GetBufferPointer(code),
                                     &shader);
    code->lpVtbl->Release(code);

    if (FAILED(hr) || shader == NULL)
    {
        report_shader_error("%s compiled but CreatePixelShader failed (hr 0x%08lX)",
                            entry->name, (unsigned long)hr);
        return false;
    }

    out_pass->shader = shader;
    strncpy(out_pass->name, entry->name, sizeof(out_pass->name) - 1);
    out_pass->name[sizeof(out_pass->name) - 1] = '\0';
    return true;
}

static bool compile_all_passes()
{
    s_shaders_compiled = true;

    const char *profile = pick_profile();
    if (profile == NULL)
    {
        report_shader_error("device does not support pixel shader 2.0 - shaders disabled");
        return false;
    }

    if (!load_compiler())
    {
        return false;
    }

    int entry_count = 0;
    const ShaderEntry *entries = get_shader_entries(&entry_count);

    for (int i = 0; i < entry_count; i++)
    {
        /* Compiled even when disabled, so F11 can switch to it without a
         * restart. s_pass_enabled decides what actually runs. */
        char *vertex_source = NULL;
        char *pixel_source = NULL;
        if (!read_shader_sources(&entries[i], &vertex_source, &pixel_source))
        {
            continue; /* the module has already reported it */
        }

        if (vertex_source != NULL)
        {
            report_mod_warning(g_shader_mod,
                               "shaders.json: \"%s\" declares a vertex file, which the "
                               "D3D9 path ignores - the fullscreen quad is pre-transformed",
                               entries[i].name);
            free(vertex_source);
        }

        if (compile_pass(&entries[i], pixel_source, profile, &s_passes[s_pass_count]))
        {
            s_pass_enabled[s_pass_count] = entries[i].enabled;
            log_info("| - Shader: compiled pass \"%s\" as %s%s", entries[i].name, profile,
                     entries[i].enabled ? "" : " (declared disabled)");
            s_pass_count++;
        }

        free(pixel_source);
    }

    return (s_pass_count > 0);
}

/* |-========== Device resources ==========-| */

/** @brief Releases everything in D3DPOOL_DEFAULT. Must run before a Reset. */
static void release_device_resources()
{
    if (s_capture_surface != NULL)
    {
        s_capture_surface->Release();
        s_capture_surface = NULL;
    }
    if (s_capture_texture != NULL)
    {
        s_capture_texture->Release();
        s_capture_texture = NULL;
    }
    if (s_state_block != NULL)
    {
        s_state_block->Release();
        s_state_block = NULL;
    }
    if (s_depth_surface != NULL)
    {
        s_depth_surface->Release();
        s_depth_surface = NULL;
    }
    if (s_depth_texture != NULL)
    {
        s_depth_texture->Release();
        s_depth_texture = NULL;
    }

    s_capture_width   = 0;
    s_capture_height  = 0;
    s_depth_available = false;
    s_resources_ready = false;
}

static void log_device_creation_flags()
{
    D3DDEVICE_CREATION_PARAMETERS creation;
    if (FAILED(s_device->GetCreationParameters(&creation)))
    {
        return;
    }

    log_info("| - Shader: device flags 0x%08lX (%s%s%s%s)",
             (unsigned long)creation.BehaviorFlags,
             (creation.BehaviorFlags & D3DCREATE_PUREDEVICE) ? "PURE " : "",
             (creation.BehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING) ? "HWVP " : "",
             (creation.BehaviorFlags & D3DCREATE_SOFTWARE_VERTEXPROCESSING) ? "SWVP " : "",
             (creation.BehaviorFlags & D3DCREATE_MIXED_VERTEXPROCESSING) ? "MIXED" : "");
}

static bool create_device_resources()
{
    IDirect3DSurface9 *backbuffer = NULL;
    if (FAILED(s_device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer)) ||
        backbuffer == NULL)
    {
        report_shader_error("could not get the backbuffer");
        return false;
    }

    D3DSURFACE_DESC desc;
    HRESULT hr = backbuffer->GetDesc(&desc);
    backbuffer->Release();

    if (FAILED(hr))
    {
        report_shader_error("could not describe the backbuffer");
        return false;
    }

    hr = s_device->CreateTexture(desc.Width, desc.Height, 1, D3DUSAGE_RENDERTARGET,
                                 desc.Format, D3DPOOL_DEFAULT, &s_capture_texture, NULL);
    if (FAILED(hr))
    {
        report_shader_error("could not create a %ux%u capture target (hr 0x%08lX)",
                            desc.Width, desc.Height, (unsigned long)hr);
        return false;
    }

    if (FAILED(s_capture_texture->GetSurfaceLevel(0, &s_capture_surface)))
    {
        report_shader_error("could not get the capture surface");
        release_device_resources();
        return false;
    }

    if (FAILED(s_device->CreateStateBlock(D3DSBT_ALL, &s_state_block)))
    {
        report_shader_error("could not create a state block");
        release_device_resources();
        return false;
    }

    s_capture_width   = desc.Width;
    s_capture_height  = desc.Height;

    static bool logged_once = false;
    if (!logged_once)
    {
        logged_once = true;
        log_device_creation_flags();
        log_engine_depth_surface();
    }

    /* Depth is optional: colour passes work without it. */
    create_depth_resources(&desc);

    s_resources_ready = true;

    log_debug(DEBUG_MED, "| - Shader: capture target is %ux%u", desc.Width, desc.Height);
    return true;
}

static void run_post_process()
{
    IDirect3DSurface9 *backbuffer = NULL;
    if (FAILED(s_device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer)) ||
        backbuffer == NULL)
    {
        return;
    }

    /* A state block covers neither render targets nor the depth surface, so
     * those are saved by hand. */
    IDirect3DSurface9 *saved_target = NULL;
    IDirect3DSurface9 *saved_depth  = NULL;
    s_device->GetRenderTarget(0, &saved_target);
    s_device->GetDepthStencilSurface(&saved_depth);

    s_state_block->Capture();

    const float width  = (float)s_capture_width;
    const float height = (float)s_capture_height;

    /* Half-texel offset: without it every pass softens the image by half a
     * pixel */
    QuadVertex quad[4] = {
        { -0.5f,        -0.5f,          0.0f, 1.0f, 0.0f, 0.0f },
        { width - 0.5f, -0.5f,          0.0f, 1.0f, 1.0f, 0.0f },
        { -0.5f,        height - 0.5f,  0.0f, 1.0f, 0.0f, 1.0f },
        { width - 0.5f, height - 0.5f,  0.0f, 1.0f, 1.0f, 1.0f },
    };

    s_device->SetRenderTarget(0, backbuffer);
    s_device->SetDepthStencilSurface(NULL);

    s_device->SetRenderState(D3DRS_ZENABLE, FALSE);
    s_device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    s_device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    s_device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    s_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    s_device->SetRenderState(D3DRS_LIGHTING, FALSE);
    s_device->SetRenderState(D3DRS_FOGENABLE, FALSE);
    s_device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
    s_device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
    s_device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
    s_device->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0F);

    s_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    s_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    s_device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
    s_device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    s_device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
    s_device->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, FALSE);

    if (s_depth_available)
    {
        s_device->SetTexture(1, s_depth_texture);
        s_device->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        s_device->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
        s_device->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
        s_device->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        s_device->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
    }

    s_device->SetVertexShader(NULL);
    s_device->SetFVF(QUAD_FVF);

    const float elapsed = (float)(GetTickCount() - s_start_tick) / 1000.0f;
    const float constants[4] = { width, height, elapsed, width / height };
    s_device->SetPixelShaderConstantF(0, constants, 1);

    /* c1..c4: inverse view-projection, for shaders that reconstruct world
     * position from depth. Left untouched when the matrix is unavailable. */
    float inverse[4][4];
    if (s_view_proj_valid && invert_4x4(s_view_proj, inverse))
    {
        s_device->SetPixelShaderConstantF(1, &inverse[0][0], 4);
    }

    /* c5: camera world position, c6: light 0 world position. w flags if it's valid so
     * a shader can use a fall back value. */
    const float camera_constant[4] = { s_camera_world[0], s_camera_world[1],
                                       s_camera_world[2], s_camera_valid ? 1.0f : 0.0f };
    /* Light 0 stays zero: DSP_LightPosition's globals is all zeros I may have wrong global or
     * bad read time, so shaders should treat .w = 0 as "no light data". for now */
    const float light_constant[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    s_device->SetPixelShaderConstantF(5, camera_constant, 1);
    s_device->SetPixelShaderConstantF(6, light_constant, 1);

    /* c7..c10: the forward view-projection, for projecting a world position
     * into screen space (the light, for one). */
    if (s_view_proj_valid)
    {
        s_device->SetPixelShaderConstantF(7, &s_view_proj[0][0], 4);
    }

    for (int i = 0; i < s_pass_count; i++)
    {
        if (!s_pass_enabled[i])
        {
            continue;
        }

        /* Grab whatever is in the backbuffer, including the previous pass's
         * output, so passes chain. */
        if (FAILED(s_device->StretchRect(backbuffer, NULL, s_capture_surface,
                                         NULL, D3DTEXF_NONE)))
        {
            break;
        }

        s_device->SetTexture(0, s_capture_texture);
        s_device->SetPixelShader(s_passes[i].shader);
        s_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, quad, sizeof(QuadVertex));
    }

    s_device->SetPixelShader(NULL);
    s_device->SetTexture(0, NULL);
    s_device->SetTexture(1, NULL);

    s_device->SetRenderTarget(0, saved_target);
    s_device->SetDepthStencilSurface(saved_depth);

    s_state_block->Apply();

    if (saved_target != NULL)
    {
        saved_target->Release();
    }
    if (saved_depth != NULL)
    {
        saved_depth->Release();
    }
    backbuffer->Release();
}

/* Runtime controls
 *
 * F10  dump constants, camera, light and the matrix residual
 * F11  cycle which pass is soloed (manifest -> pass 0 -> pass 1 -> ... -> manifest)
 * F12  recompile every pass from disk
 *
 * HLSL file can be edited and the result seen without
 * restarting the game
 */

/** @brief Edge-detected key test, so should avoid double presses */
static bool key_pressed(int virtual_key, bool *was_down)
{
    bool is_down = (GetAsyncKeyState(virtual_key) & 0x8000) != 0;
    bool pressed = is_down && !*was_down;
    *was_down = is_down;
    return pressed;
}

/** @brief Restores the manifest's enabled flags. */
static void apply_manifest_pass_state()
{
    int entry_count = 0;
    const ShaderEntry *entries = get_shader_entries(&entry_count);

    for (int i = 0; i < s_pass_count && i < entry_count; i++)
    {
        s_pass_enabled[i] = entries[i].enabled;
    }
}

/** @brief Advances the solo cycle: manifest, then each pass in turn. */
static void cycle_solo_pass()
{
    if (s_pass_count == 0)
    {
        return;
    }

    s_solo_pass++;
    if (s_solo_pass >= s_pass_count)
    {
        s_solo_pass = -1;
    }

    if (s_solo_pass < 0)
    {
        apply_manifest_pass_state();
        log_info("| - Shader: following shaders.json");
        return;
    }

    for (int i = 0; i < s_pass_count; i++)
    {
        s_pass_enabled[i] = (i == s_solo_pass);
    }
    log_info("| - Shader: soloing pass %d/%d \"%s\"",
             s_solo_pass + 1, s_pass_count, s_passes[s_solo_pass].name);
}

/**
 * @brief Drops every compiled shader so the next frame rebuilds from disk.
 *
 * read_shader_sources reads the files fresh each time, so edited HLSL is picked
 * up. The manifest itself is not re-read - adding a new shader to json still needs a
 * restart, changing an existing one does not.
 */
static void reload_shaders()
{
    for (int i = 0; i < s_pass_count; i++)
    {
        if (s_passes[i].shader != NULL)
        {
            s_passes[i].shader->Release();
            s_passes[i].shader = NULL;
        }
    }

    int previous_solo = s_solo_pass;

    s_pass_count       = 0;
    s_shaders_compiled = false;
    s_pipeline_live    = false;
    s_solo_pass        = -1;

    clear_mod_errors(g_shader_mod);
    log_info("| - Shader: reloading shaders from disk");

    /* Recompile immediately so failures are reported now rather than being
     * blamed on whatever happens next frame. */
    if (compile_all_passes())
    {
        log_info("| - Shader: %d pass(es) recompiled", s_pass_count);

        /* Re-solo whatever was soloed before, if it still exists. */
        if (previous_solo >= 0 && previous_solo < s_pass_count)
        {
            s_solo_pass = previous_solo - 1;
            cycle_solo_pass();
        }
    }
    else
    {
        log_warning("| - Shader: nothing compiled - previous passes are gone, "
                    "fix the HLSL and press F12 again");
    }
}

/** @brief diagnostic dump, for the shadowed constants and the snapshotted matrix. */
static void dump_vs_constants()
{
    log_info("| - Shader: vertex shader constants  (VARIES = changed between draws "
             "this frame, so per-object; steady registers are view state)");
    for (int i = 0; i < SFSF_VS_CONST_TRACK; i++)
    {
        if (!s_vs_seen[i])
        {
            continue;
        }
        log_info("| -   c%-2d %-6s [%9.3f %9.3f %9.3f %9.3f]", i,
                 s_vs_varies_in_frame[i] ? "VARIES" : "steady",
                 s_vs_constants[i][0], s_vs_constants[i][1],
                 s_vs_constants[i][2], s_vs_constants[i][3]);
    }

    if (!s_view_proj_valid)
    {
        log_info("| - Shader: no view-projection snapshot yet");
        return;
    }

    log_info("| - Shader: view-projection from the frame's largest draw (%d prims); "
             "camera %s [%9.3f %9.3f %9.3f]",
             s_winning_prims, s_camera_valid ? "derived" : "unavailable",
             s_camera_world[0], s_camera_world[1], s_camera_world[2]);

    log_info("| - Shader: chosen view-projection");
    for (int r = 0; r < 4; r++)
    {
        log_info("| -   [%9.3f %9.3f %9.3f %9.3f]",
                 s_view_proj[r][0], s_view_proj[r][1],
                 s_view_proj[r][2], s_view_proj[r][3]);
    }

    /* Trace the next two frames' opening draws, so the pass that has the
     * matrix can be identified. */
    s_trace_draws = 2;
}

/**
 * @brief Runs the passes, then presents.
 */
static HRESULT WINAPI sf_present_hook(IDirect3DDevice9 *device, const RECT *source,
                                      const RECT *dest, HWND window_override,
                                      const RGNDATA *dirty_region)
{
    if (!s_in_hook && device != NULL && device == s_device &&
        s_device->TestCooperativeLevel() == D3D_OK)
    {
        s_in_hook = true;

        if (!s_shaders_compiled)
        {
            s_start_tick = GetTickCount();
            if (compile_all_passes())
            {
                log_info("| - Shader: %d pass(es) ready", s_pass_count);
            }
            else
            {
                log_warning("| - Shader: no usable passes, pipeline stays inactive");
            }
        }

        if (s_pass_count > 0)
        {
            if (!s_resources_ready)
            {
                create_device_resources();
            }

            if (s_resources_ready)
            {
                if (!s_pipeline_live)
                {
                    s_pipeline_live = true;
                    log_info("| - Shader: pipeline live");
                }

                static bool f10_down = false;
                static bool f11_down = false;
                static bool f8_down = false;

                if (key_pressed(VK_F10, &f10_down))
                {
                    dump_vs_constants();
                }
                if (key_pressed(VK_F11, &f11_down))
                {
                    cycle_solo_pass();
                }
                if (key_pressed(VK_F8, &f8_down))
                {
                    reload_shaders();
                }

                /* this finishes the frame search before the passes read it. */
                resolve_view_proj();

                if (SUCCEEDED(s_device->BeginScene()))
                {
                    run_post_process();
                    s_device->EndScene();
                }
            }
        }

        s_in_hook = false;
    }

    return s_real_present(device, source, dest, window_override, dirty_region);
}

/**
 * @brief Drops D3DPOOL_DEFAULT resources around a device reset.
 *
 * Pixel shaders survive a reset and are deliberately left alone; the capture
 * target and state block are rebuilt lazily on the next Present.
 */
static HRESULT WINAPI sf_reset_hook(IDirect3DDevice9 *device,
                                    D3DPRESENT_PARAMETERS *present_params)
{
    if (device != NULL && device == s_device)
    {
        log_debug(DEBUG_MED, "| - Shader: device reset, releasing resources");
        release_device_resources();
        s_pipeline_live = false;
    }

    return s_real_reset(device, present_params);
}

/**Quick method to capture a valid device
 * The DSP_Display isn't well understood, and is huge,
 * so it's quicker to just look for a correct pattern, then do proper decomp XD
 */

/** Upper bound for the read-only survey of DSP_Display. */
#define SFSF_DSP_SURVEY_LIMIT 0x4000

/** Frames to keep trying before giving up, in case the device appears late. */
#define SFSF_CAPTURE_MAX_ATTEMPTS 600

static int s_capture_attempts = 0;

/** @brief Reads a pointer-sized slot, or NULL when it is not safely readable. */
static void *safe_deref(const void *slot)
{
    if (!is_readable(slot, sizeof(void *)))
    {
        return NULL;
    }
    return *(void *const *)slot;
}

/** @brief True when @p candidate is an object whose vtable lives in d3d9.dll. */
static bool has_d3d9_vtable(const void *candidate)
{
    if (!is_readable(candidate, sizeof(void *)))
    {
        return false;
    }

    const void *vtable = *(const void *const *)candidate;
    if (!is_readable(vtable, sizeof(void *) * 64))
    {
        return false;
    }

    return is_within_module(vtable, "d3d9.dll");
}

/* Declared locally rather than linking dxguid, These are D3D9 interface IDs that worked out */
static const GUID k_iid_idirect3ddevice9 =
    { 0xd0223b96, 0xbf7a, 0x43fd, { 0x92, 0xbd, 0xa4, 0x3b, 0x0d, 0x82, 0xb9, 0xeb } };
static const GUID k_iid_idirect3dresource9 =
    { 0x05eec05d, 0x8f7d, 0x4362, { 0xb9, 0x99, 0xd1, 0xba, 0xf3, 0x57, 0xc7, 0x04 } };

/**
 * @brief Asks a candidate whether it is really an IDirect3DDevice9.
 *
 * Only call this on the render thread, on objects whose vtable has already been
 * shown to sit in a d3d9.dll section. Calling into these objects from another
 * thread, while the engine is still constructing them, crashes the game.
 */
static bool is_direct3d_device9(void *candidate)
{
    IUnknown *unknown = (IUnknown *)candidate;
    void *out = NULL;

    if (FAILED(unknown->QueryInterface(k_iid_idirect3ddevice9, &out)) || out == NULL)
    {
        return false;
    }

    ((IUnknown *)out)->Release();
    return true;
}

/**
 * @brief Asks a d3d9 resource which device it's on.
 *
 * GetDevice is inherited by every texture, surface and vertex buffer, so any
 * one of them names the engine's device without us needing a device pointer of
 * our own.
 *
 * @return The device with its reference already released, or NULL.
 */
static IDirect3DDevice9 *device_from_resource(void *candidate)
{
    IUnknown *unknown = (IUnknown *)candidate;
    void *resource_out = NULL;

    if (FAILED(unknown->QueryInterface(k_iid_idirect3dresource9, &resource_out)) ||
        resource_out == NULL)
    {
        return NULL;
    }

    IDirect3DResource9 *resource = (IDirect3DResource9 *)resource_out;
    IDirect3DDevice9 *device = NULL;
    HRESULT hr = resource->GetDevice(&device);
    resource->Release();

    if (FAILED(hr) || device == NULL)
    {
        return NULL;
    }

    /* GetDevice adds a reference. Because the engine owns this device and needs it to outlive us,
     * we drop the reference rather than hold onto it. */
    device->Release();
    return device;
}

/**
 * @brief Finds the engine's IDirect3DDevice9 through DSP_Display.
 *
 */
static IDirect3DDevice9 *find_device(BYTE *dsp_display, int *out_offset,
                                     const char **out_route)
{
    int inspected = 0;

    for (int offset = 0; offset < SFSF_DSP_SURVEY_LIMIT; offset += sizeof(void *))
    {
        void *candidate = safe_deref(dsp_display + offset);
        if (candidate == NULL)
        {
            continue;
        }

        if (!has_d3d9_vtable(candidate))
        {
            continue;
        }

        inspected++;

        if (is_direct3d_device9(candidate))
        {
            *out_offset = offset;
            *out_route = "direct";
            return (IDirect3DDevice9 *)candidate;
        }

        IDirect3DDevice9 *owner = device_from_resource(candidate);
        if (owner != NULL)
        {
            *out_offset = offset;
            *out_route = "via resource GetDevice";
            return owner;
        }

        log_debug(DEBUG_MED, "| - Shader: d3d9 object at +0x%X (%p) is neither the "
                  "device nor a resource", offset, candidate);
    }

    log_debug(DEBUG_MED, "| - Shader: inspected %d d3d9 object(s), no device yet",
              inspected);
    return NULL;
}

/**
 * @brief Locates the device and patches its vtable.
 *
 * Retried across frames rather than latched on the first attempt, in case the
 * display does not have a device yet on early RenderFrame calls.
 */
static void capture_device_from_dsp(void *dsp_display_ptr)
{
    if (s_device_capture_done || dsp_display_ptr == NULL)
    {
        return;
    }

    BYTE *dsp_display = (BYTE *)dsp_display_ptr;
    if (!is_readable(dsp_display, sizeof(void *)))
    {
        return;
    }

    s_capture_attempts++;

    IDirect3DDevice9 *device = NULL;
    const char *route = NULL;
    int offset = 0;

    void *direct = safe_deref(dsp_display + SFSF_OFFSET_DSP_DEVICE);
    if (direct != NULL && has_d3d9_vtable(direct) && is_direct3d_device9(direct))
    {
        device = (IDirect3DDevice9 *)direct;
        offset = SFSF_OFFSET_DSP_DEVICE;
        route  = "device pointer";
    }

    if (device == NULL)
    {
        void *resource = safe_deref(dsp_display + SFSF_OFFSET_DSP_RESOURCE);
        if (resource != NULL && has_d3d9_vtable(resource))
        {
            device = device_from_resource(resource);
            if (device != NULL)
            {
                offset = SFSF_OFFSET_DSP_RESOURCE;
                route  = "resource GetDevice";
            }
        }
    }

    if (device == NULL)
    {
        device = find_device(dsp_display, &offset, &route);
    }

    if (device == NULL)
    {
        if (s_capture_attempts == 1)
        {
            log_info("| - Shader: no device on the first frame, retrying (DSP_Display "
                     "at %p)", (void *)dsp_display);
        }
        else if (s_capture_attempts >= SFSF_CAPTURE_MAX_ATTEMPTS)
        {
            s_device_capture_done = true;
            log_error("| - Shader: no IDirect3DDevice9 reachable from DSP_Display "
                      "after %d frames. Shaders off.", s_capture_attempts);
        }
        return;
    }

    s_device_capture_done = true;
    s_device = device;
    log_info("| - Shader: device %p found at DSP_Display+0x%X (%s) on frame %d",
             (void *)device, offset, route, s_capture_attempts);

    if (!patch_vtable(device, VT_IDIRECT3DDEVICE9_PRESENT,
                      (void *)&sf_present_hook, (void **)&s_real_present))
    {
        log_error("| - Shader: could not hook Present, shaders disabled");
        s_device = NULL;
        return;
    }
    log_info("| - Shader: Present hooked");

    if (!patch_vtable(device, VT_IDIRECT3DDEVICE9_RESET,
                      (void *)&sf_reset_hook, (void **)&s_real_reset))
    {
        log_warning("| - Shader: Present hooked but Reset was not - resolution "
                    "changes may leak the capture target");
    }

    if (!patch_vtable(device, VT_IDIRECT3DDEVICE9_SETDEPTHSTENCIL,
                      (void *)&sf_setdepthstencil_hook, (void **)&s_real_setdepthstencil))
    {
        log_warning("| - Shader: SetDepthStencilSurface was not hooked - depth may "
                    "come from the engine's own surface instead of INTZ");
    }

    if (!patch_vtable(device, VT_IDIRECT3DDEVICE9_SETVSCONSTF,
                      (void *)&sf_setvsconstf_hook, (void **)&s_real_setvsconstf))
    {
        log_warning("| - Shader: SetVertexShaderConstantF was not hooked - no world "
                    "space reconstruction");
    }

    if (!patch_vtable(device, VT_IDIRECT3DDEVICE9_DRAWINDEXEDPRIM,
                      (void *)&sf_drawindexed_hook, (void **)&s_real_drawindexed))
    {
        log_warning("| - Shader: DrawIndexedPrimitive was not hooked - the "
                    "view-projection snapshot will pick up UI state instead");
    }
}

/**
 * @brief Stand in for DSP_Display::BeginScene.
 */
static int __thiscall sf_dsp_beginscene_hook(void *dsp_display)
{
    if (!s_device_capture_done)
    {
        capture_device_from_dsp(dsp_display);
    }

    s_have_best = false;
    s_frame_snapshot_done = false;
    s_draw_index = 0;
    memcpy(s_vs_frame_start, s_vs_constants, sizeof(s_vs_frame_start));
    memset(s_vs_varies_in_frame, 0, sizeof(s_vs_varies_in_frame));
    if (s_trace_draws > 0)
    {
        s_trace_draws--;
        log_info("| - Shader: --- frame trace ---");
    }

    if (s_depth_available && s_device != NULL)
    {
        s_device->SetDepthStencilSurface(s_depth_surface);
    }

    return s_real_dsp_beginscene(dsp_display);
}

bool shader_pipeline_is_live()
{
    return s_pipeline_live;
}

void initialize_shader_hooks()
{
    if (get_shader_pass_count() == 0)
    {
        log_debug(DEBUG_MED, "| - Shader: nothing enabled in shaders.json");
    }

    s_real_dsp_beginscene = (PFN_DspBeginScene)ASI::AddrOf(SFSF_ADDR_DSP_BEGINSCENE);

    ASI::MemoryRegion beginscene_mreg(ASI::AddrOf(SFSF_ADDR_BEGINSCENE_CALL), 5);
    ASI::BeginRewrite(beginscene_mreg);
    *(unsigned char *)(ASI::AddrOf(SFSF_ADDR_BEGINSCENE_CALL)) = 0xE8;
    *(int *)(ASI::AddrOf(0x197bd3)) =
        (int)(&sf_dsp_beginscene_hook) - ASI::AddrOf(0x197bd7);
    ASI::EndRewrite(beginscene_mreg);

    log_info("| - Shader: hooked DSP_Display::BeginScene call site, "
             "waiting for the first rendered frame");
}

/** @} */
