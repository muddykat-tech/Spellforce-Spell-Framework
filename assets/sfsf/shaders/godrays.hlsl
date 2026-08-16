// Failed Volumetric light shafts test, just makes hard to read UI
//
// Idea was to complete marches from each pixel toward a screen-space sun position, accumulating
// light from sky pixels along the way. Which would be blocked by depth detected objects... nada
//
// Registers supplied by the framework:
//   s0 = colour capture
//   s1 = INTZ depth (value in .r)
//   c0 = (width, height, seconds, aspect)
//
// All tuning lives here. The framework compiles HLSL at runtime, so changing
// any of it means editing this file and restarting the game - no rebuild.

sampler2D uTexture : register(s0);
sampler2D uDepth   : register(s1);
float4    uParams  : register(c0);

// Sun position in screen UV. (0,0) is top-left, (1,1) bottom-right. No worldspace coords just yet.
// Testing these soon.
// outside 0..1 are fine - shafts still aim correctly from off-screen.
#define SUN_POS      float2(0.50, 0.05)

#define NUM_SAMPLES  24      // taps along the ray. More = smoother, slower.
#define DENSITY      0.85    // how far along the ray to march (1.0 = to the sun)
#define DECAY        0.96    // per-sample falloff; lower = shorter shafts
#define WEIGHT       0.30    // brightness of each sample
#define EXPOSURE     0.55    // final strength of the whole effect

#define DEBUG_MASK   0

float sky_mask(float2 uv)
{
    float depth = tex2D(uDepth, uv).r;
    return step(0.99999, depth);
}

float4 main(float2 uv : TEXCOORD0) : COLOR0
{
    float3 scene = tex2D(uTexture, uv).rgb;

#if DEBUG_MASK
    return float4(sky_mask(uv).xxx, 1.0);
#endif
    float2 delta = uv - SUN_POS;
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

        float3 tap = tex2D(uTexture, sample_uv).rgb * sky_mask(sample_uv) * inside;

        shafts += tap * decay * WEIGHT;
        decay  *= DECAY;
    }

    return float4(scene + shafts * EXPOSURE, 1.0);
}
