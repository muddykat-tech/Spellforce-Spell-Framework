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

/**
 * TEST DEPTH Stuff
 *
 */

/** INTZ: a depth-stencil format that is also sampleable. Not in d3d9.h. */
#define FOURCC_INTZ ((D3DFORMAT)MAKEFOURCC('I','N','T','Z'))

/** IDirect3DDevice9::SetDepthStencilSurface (vtable +0x9C). */
#define VT_IDIRECT3DDEVICE9_SETDEPTHSTENCIL 39

typedef HRESULT (WINAPI *PFN_SetDepthStencil)(IDirect3DDevice9 *, IDirect3DSurface9 *);

static IDirect3DTexture9 *s_depth_texture = NULL;
static IDirect3DSurface9 *s_depth_surface = NULL;
static bool s_depth_available = false;
static PFN_SetDepthStencil s_real_setdepthstencil = NULL;

/** END */

/* Minimal ID3DBlob
 * Declared here rather than pulling in d3dcommon.h
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
    if (s_depth_available && device == s_device &&
        surface != NULL && surface != s_depth_surface)
    {
        D3DSURFACE_DESC desc;
        if (SUCCEEDED(surface->GetDesc(&desc)) &&
            desc.Width == s_capture_width && desc.Height == s_capture_height)
        {
            surface = s_depth_surface;
        }
    }

    return s_real_setdepthstencil(device, surface);
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
        if (!entries[i].enabled)
        {
            continue;
        }

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
            log_info("| - Shader: compiled pass \"%s\" as %s", entries[i].name, profile);
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

    if (s_depth_surface != NULL) { s_depth_surface->Release(); s_depth_surface = NULL; }
    if (s_depth_texture != NULL) { s_depth_texture->Release(); s_depth_texture = NULL; }


    s_capture_width   = 0;
    s_capture_height  = 0;

    s_depth_available = false;
    s_resources_ready = false;
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

    /* Depth is optional: colour passes work without it. */
    static bool logged_depth_once = false;
    if (!logged_depth_once)
    {
        logged_depth_once = true;
        log_engine_depth_surface();
    }
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

    s_device->SetPixelShaderConstantF(0, constants, 1);

    for (int i = 0; i < s_pass_count; i++)
    {
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

                if (SUCCEEDED(s_device->BeginScene()))
                {
                    run_post_process();
                    s_device->EndScene();
                }

                IDirect3DSurface9 *depth = NULL;
                if (SUCCEEDED(s_device->GetDepthStencilSurface(&depth)) && depth != NULL)
                {
                    D3DSURFACE_DESC desc;
                    depth->GetDesc(&desc);
                    log_info("| - Shader: depth %ux%u fmt 0x%X (%c%c%c%c) msaa %d",
                             desc.Width, desc.Height, desc.Format,
                             (desc.Format      ) & 0xFF, (desc.Format >>  8) & 0xFF,
                             (desc.Format >> 16) & 0xFF, (desc.Format >> 24) & 0xFF,
                             (int)desc.MultiSampleType);
                    depth->Release();
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

/* Declared locally rather than linking dxguid, which is not in every MinGW
 * distribution. These are the documented D3D9 interface IDs. */
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
 * @brief Asks a d3d9 resource which device owns it.
 *
 * GetDevice is inherited by every texture, surface and vertex buffer, so any
 * one of them names the engine's device without us needing a device pointer of
 * our own.
 *
 * @return The owning device with its reference already released, or NULL.
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
 * @brief Locates the device and patches Present / Reset on it. One shot.
 *
 * Retried across frames rather than latched on the first attempt, in case the
 * display does not own a device yet on the earliest RenderFrame calls.
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
}

/**
 * @brief Stands in for DSP_Display::BeginScene at one call site.
 *
 * Plain __thiscall, so the patched CALL needs no trampoline or naked asm.
 */
static int __thiscall sf_dsp_beginscene_hook(void *dsp_display)
{
    if (!s_device_capture_done)
    {
        capture_device_from_dsp(dsp_display);
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
        log_debug(DEBUG_MED, "| - Shader: nothing declared in shaders.json, hook not installed");
        return;
    }

    s_real_dsp_beginscene = (PFN_DspBeginScene)ASI::AddrOf(SFSF_ADDR_DSP_BEGINSCENE);

    ASI::MemoryRegion beginscene_mreg(ASI::AddrOf(SFSF_ADDR_BEGINSCENE_CALL), 5);
    ASI::BeginRewrite(beginscene_mreg);
    *(unsigned char *)(ASI::AddrOf(SFSF_ADDR_BEGINSCENE_CALL)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(SFSF_ADDR_BEGINSCENE_CALL + 1)) =
        (int)(&sf_dsp_beginscene_hook) - ASI::AddrOf(SFSF_ADDR_BEGINSCENE_CALL + 5);
    ASI::EndRewrite(beginscene_mreg);

    log_info("| - Shader: hooked DSP_Display::BeginScene call site, "
             "waiting for the first rendered frame");
}

/** @} */
