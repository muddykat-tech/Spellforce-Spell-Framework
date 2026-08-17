// Volumetric light shafts from a world-space sun direction.
//
// A directional sun has no position - it is infinitely far away - so the trick
// is to place a virtual sun a long way along the sun direction FROM THE CAMERA
// and project that. Because the anchor moves with the camera, the shafts stay
// correct as you pan, rotate and zoom, which a fixed screen position cannot do.
//
// Two sources for the direction:
//   SUN_SOURCE 0 - the SUN_AZIMUTH/SUN_ELEVATION below (always works)
//   SUN_SOURCE 1 - c11, harvested from vertex register 16, which is consistently
//                  a unit vector with w ~0.93. Probably the engine's sun; falls
//                  back to the configured direction when it does not look valid.
//
// Registers supplied by the framework:
//   s0     = colour capture
//   s1     = INTZ depth (value in .r)
//   c0     = (width, height, seconds, aspect)
//   c1-c4  = inverse view-projection
//   c5     = camera world position, .w = 1 when valid
//   c6     = light 0 world position, .w = 1 when valid (unused, engine gives 0)
//   c7-c10 = forward view-projection
//   c11    = candidate sun direction, .w = 1 when it looks like a unit vector

sampler2D uTexture : register(s0);
sampler2D uDepth   : register(s1);
float4    uParams  : register(c0);
float4    uCamera  : register(c5);
float4    uSunDir  : register(c11);

float4 uVP0 : register(c7);
float4 uVP1 : register(c8);
float4 uVP2 : register(c9);
float4 uVP3 : register(c10);

// --- Sun placement ---------------------------------------------------------
#define SUN_SOURCE     1

// c16 holds the direction light TRAVELS (its z is negative - pointing down), so
// it must be negated to get the direction TO the sun. Without this the virtual
// sun ends up underground: no rays looking horizontally, and the screen washes
// out as the camera tilts toward it.
#define SUN_INVERT     1

// Used when SUN_SOURCE is 0, or when c11 is not valid.
// World axes: x and y are horizontal (tile coords), z is up.
#define SUN_AZIMUTH    135.0   // degrees around the vertical axis
#define SUN_ELEVATION   38.0   // degrees above the horizon
#define SUN_DISTANCE  4000.0   // how far to place the virtual sun

// --- Shafts ----------------------------------------------------------------
#define NUM_SAMPLES  64
#define DENSITY      1.20
#define DECAY        0.980
#define WEIGHT       0.40
#define EXPOSURE     0.225

#define SKY_EDGE_LO  0.65     // depth where sky starts to count
#define SKY_EDGE_HI  0.70

// Shafts fade out as the sun leaves the frame, rather than snapping off.
#define OFFSCREEN_FADE 0.60   // screen widths beyond the edge before fully gone

// 1 = sky mask, 2 = mark the sun position, 3 = rays only, 0 = normal
#define DEBUG_MODE   0
// ---------------------------------------------------------------------------

float sky_mask(float2 uv)
{
    return smoothstep(SKY_EDGE_LO, SKY_EDGE_HI, tex2D(uDepth, uv).r);
}

float3 sun_direction()
{
#if SUN_SOURCE == 1
    if (uSunDir.w > 0.5)
    {
    #if SUN_INVERT
        return normalize(-uSunDir.xyz);
    #else
        return normalize(uSunDir.xyz);
    #endif
    }
#endif

    float az = radians(SUN_AZIMUTH);
    float el = radians(SUN_ELEVATION);
    return normalize(float3(cos(az) * cos(el), sin(az) * cos(el), sin(el)));
}

/**
 * Screen position of the virtual sun, plus a 0..1 factor that fades the effect
 * as it goes off screen or behind the camera.
 */
float2 sun_screen_pos(out float visibility)
{
    visibility = 0.0;

    if (uCamera.w < 0.5)
    {
        return float2(0.5, 0.05);   // no camera - nothing sensible to aim at
    }

    float3 direction = sun_direction();

    /* A sun below the horizon means the direction convention is wrong, and the
     * symptom is the screen washing out when the camera tilts down. Bail rather
     * than render that. */
    if (direction.z <= 0.0)
    {
        return float2(0.5, 0.05);
    }

    float3 sun_world = uCamera.xyz + direction * SUN_DISTANCE;

    float4 world = float4(sun_world, 1.0);
    float4 clip  = float4(dot(uVP0, world), dot(uVP1, world),
                          dot(uVP2, world), dot(uVP3, world));

    if (clip.w <= 0.0001)
    {
        return float2(0.5, 0.05);   // behind the eye, visibility stays 0
    }

    float2 ndc = clip.xy / clip.w;
    float2 uv  = float2(ndc.x * 0.5 + 0.5, 0.5 - ndc.y * 0.5);

    // Distance outside the [0,1] box, in screen widths.
    float2 outside = max(max(-uv, uv - 1.0), 0.0);
    float  beyond  = length(outside);
    visibility = 1.0 - saturate(beyond / OFFSCREEN_FADE);

    return uv;
}

float4 main(float2 uv : TEXCOORD0) : COLOR0
{
    float3 scene = tex2D(uTexture, uv).rgb;

    float  visibility;
    float2 sun_uv = sun_screen_pos(visibility);

#if DEBUG_MODE == 1
    return float4(sky_mask(uv).xxx, 1.0);
#elif DEBUG_MODE == 2
    // Cross at the sun. Green = engine direction, red = configured fallback.
    // Brightness tracks visibility, so it dims as the sun leaves the frame.
    float2 d = abs(uv - sun_uv);
    float  cross_mark = 1.0 - saturate(min(d.x, d.y) * 200.0);
    float3 marker = (uSunDir.w > 0.5 && SUN_SOURCE == 1)
                    ? float3(0.0, 1.0, 0.0) : float3(1.0, 0.0, 0.0);
    marker *= 0.25 + 0.75 * visibility;
    return float4(lerp(scene, marker, cross_mark), 1.0);
#endif

    if (visibility <= 0.001)
    {
        return float4(scene, 1.0);
    }

    // Step toward the sun, aspect-corrected so shafts stay radial.
    float2 delta = uv - sun_uv;
    delta.x *= uParams.w;
    delta *= DENSITY / NUM_SAMPLES;
    delta.x /= uParams.w;

    float2 sample_uv = uv;
    float  decay     = 1.0;
    float3 shafts    = 0.0;

    for (int i = 0; i < NUM_SAMPLES; i++)
    {
        sample_uv -= delta;

        float2 edge2  = smoothstep(0.0, 0.15, sample_uv) * smoothstep(1.0, 0.85, sample_uv);
        float  inside = edge2.x * edge2.y;

        shafts += tex2D(uTexture, sample_uv).rgb * sky_mask(sample_uv) * inside
                  * decay * WEIGHT;
        decay  *= DECAY;
    }

    shafts *= visibility;

#if DEBUG_MODE == 3
    float ray_val = dot(shafts, float3(0.299, 0.587, 0.114)) * EXPOSURE;
    float pulse = 0.5 + 0.5 * sin(uParams.z * 3.0);
    ray_val = saturate((ray_val * pulse - 0.05) / 0.45);
    return float4(float3(0.0, 1.0, 0.0) * (0.2 + 0.8 * ray_val), 1.0);
#endif

    return float4(scene + shafts * EXPOSURE, 1.0);
}
