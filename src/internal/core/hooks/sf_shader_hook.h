/**
 * @defgroup ShaderHook Post-Process Shader Hook (Direct3D 9)
 * @ingroup Hooks
 * @brief Frame-end interception for running HLSL post-process passes.
 *
 * SpellForce imports no graphics DLL at all, it LoadLibrary's its renderer
 * at runtime.
 * Every hook here needs to be an inline patch installed from a worker thread once
 * the relevant module is resident.
 *
 * Chain: Direct3DCreate9 -> IDirect3D9::CreateDevice -> IDirect3DDevice9::Present.
 *
 * @addtogroup ShaderHook
 * @{
 */

#ifndef SF_SHADER_HOOK_H
#define SF_SHADER_HOOK_H

#include "../../../api/sfsf.h"
#include "../../../asi/sf_asi.h"

#include "../../registry/sf_error_registry.h"

/** Shader profile to compile against. ps_2_0 is a backup option. */
#define SFSF_SHADER_PROFILE "ps_3_0"
#define SFSF_SHADER_PROFILE_FALLBACK "ps_2_0"

/** How long the installer thread waits for d3d9.dll before giving up. */
#define SFSF_SHADER_INSTALL_TIMEOUT_MS 30000

/** @brief Starts the installer thread. */
void initialize_shader_hooks();

/** @brief True once a device was captured and at least one pass compiled. */
bool shader_pipeline_is_live();


/**
 * @brief How many references the shader hook holds on the D3D9 device.
 *
 * Counts objects created from the device: the capture texture, the state block,
 * the INTZ depth texture and one per compiled pixel shader. Surfaces fetched
 * with GetSurfaceLevel reference their parent texture rather than the device,
 * so they are not counted.
 *
 * Note this grows with the number of compiled passes - every declared shader is
 * compiled so it can be soloed at runtime, not just the enabled ones - so a
 * Release hook comparing against a fixed number will be wrong. Ask instead.
 */
int shader_hook_held_references();

/**
 * @brief Drops every device object the shader hook owns.
 *
 * Call this when the engine is tearing the device down, before it releases its
 * own reference. Leaves the hook inert: the device pointer is cleared and no
 * further capture is attempted.
 */
void shader_hook_release_all();

/**
 * @brief Same as shader_hook_release_all, but permanent.
 *
 * Nothing is re-captured afterwards. Call this on the way out of the process.
 */
void shader_hook_shutdown();

/** @} */
#endif // SF_SHADER_HOOK_H
