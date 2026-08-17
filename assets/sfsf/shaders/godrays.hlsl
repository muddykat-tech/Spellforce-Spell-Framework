// Volumetric light shafts, aimed at the engine's actual light.
//
// Attempts to projects light based on data pulled from the engine...
// We attempt to use some random data I found that looked right in order to find where the sun is on screen.
// The shafts should track correctly as the camera moves and rotates.. but they don't.
//
// I've likely got either a bad intercept location, camera and light position are always zero'd, so it's that or I have
// unused data ¯\(ツ)/¯
//
// Registers supplied by the framework:
//   s0     = colour capture
//   s1     = INTZ depth (value in .r)
//   c0     = (width, height, seconds, aspect)
//   c1-c4  = inverse view-projection, one row per register
//   c5     = camera world position, .w = 1 when valid
//   c6     = light 0 world position, .w = 1 when valid
//   c7-c10 = forward view-projection, one row per register

sampler2D uTexture : register(s0);
sampler2D uDepth   : register(s1);
float4    uParams  : register(c0);
float4    uCamera  : register(c5);
float4    uLight   : register(c6);

float4 uVP0 : register(c7);
float4 uVP1 : register(c8);
float4 uVP2 : register(c9);
float4 uVP3 : register(c10);

#define NUM_SAMPLES  48
#define DENSITY      1.15
#define DECAY        0.98
#define WEIGHT       0.65
#define EXPOSURE     1.40

#define FALLBACK_SUN float2(0.50, 0.05)

// 1 = show the sky mask, 2 = mark the computed sun position, 3 = pure green rays, 0 = normal.
#define DEBUG_MODE   3

float sky_mask(float2 uv)
{
    return step(0.99999, tex2D(uDepth, uv).r);
}

// Projects a world position to screen UV. *SHOULD* Return false behind the camera. looks to be working... probably
bool light_screen_pos(out float2 uv)
{
    uv = FALLBACK_SUN;

    if (uLight.w < 0.5)
    {
        return false;
    }

    float4 world = float4(uLight.xyz, 1.0);
    float4 clip  = float4(dot(uVP0, world), dot(uVP1, world),
                          dot(uVP2, world), dot(uVP3, world));

    if (clip.w <= 0.0001)
    {
        return false;   // behind the eye
    }

    float2 ndc = clip.xy / clip.w;
    uv = float2(ndc.x * 0.5 + 0.5, 0.5 - ndc.y * 0.5);
    return true;
}

float4 main(float2 uv : TEXCOORD0) : COLOR0
{
    float3 scene = tex2D(uTexture, uv).rgb;

    float2 sun_uv;
    bool   sun_ok = light_screen_pos(sun_uv);

#if DEBUG_MODE == 1
    return float4(sky_mask(uv).xxx, 1.0);
#elif DEBUG_MODE == 2
    // Red cross at the computed sun position. We using real data when green, fallback is red.
    float2 d = abs(uv - sun_uv);
    float  cross_mark = 1.0 - saturate(min(d.x, d.y) * 200.0);
    float3 marker = sun_ok ? float3(1.0, 0.0, 0.0) : float3(0.0, 1.0, 0.0);
    return float4(lerp(scene, marker, cross_mark), 1.0);
#endif

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
        float2 inside2 = step(0.0, sample_uv) * step(sample_uv, 1.0);
        float  inside  = inside2.x * inside2.y;

        shafts += tex2D(uTexture, sample_uv).rgb * sky_mask(sample_uv) * inside
                  * decay * WEIGHT;
        decay  *= DECAY;
    }

#if DEBUG_MODE == 3
    return float4(float3(0.0, 1.0, 0.0) * dot(shafts, float3(0.299, 0.587, 0.114)) * EXPOSURE, 1.0);
#endif

    return float4(scene + shafts * EXPOSURE, 1.0);
}
