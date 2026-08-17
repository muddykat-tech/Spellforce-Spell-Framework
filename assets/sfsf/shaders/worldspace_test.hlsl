// World-space position reconstruction test.
//
// Unprojects each pixel using its depth and the inverse view-projection matrix
// the engine hands its vertex shaders, then visualises the result. If the
// matrix is correct the pattern is LOCKED TO THE TERRAIN... a pan or rotate of the
// camera and the colours / grid should stay glued to the ground.
//
// This works now, but trust me it's done badly, basically, we're getting like third hand data and we try to filter out
// Noise, works *mostly* alright, but UI is still included so we're getting bad UI bleed.
//
// Registers supplied by the framework:
//   s0    = colour capture
//   s1    = INTZ depth (value in .r)
//   c0    = (width, height, seconds, aspect)
//   c1-c4 = inverse view-projection, one row per register
//
// MODE 0 : world XY as a repeating grid  - best for checking lock-on
// MODE 1 : world height as a gradient    - should follow terrain elevation
// MODE 2 : raw world position as RGB     - smooth, mostly for sanity
// MODE 3 : contour lines every N units   - sharpest test of stability

#define MODE       3
#define GRID_SIZE  10.0   // world units per grid cell / contour band

sampler2D uTexture : register(s0);
sampler2D uDepth   : register(s1);
float4    uParams  : register(c0);

float4 uInvVP0 : register(c1);
float4 uInvVP1 : register(c2);
float4 uInvVP2 : register(c3);
float4 uInvVP3 : register(c4);

float3 world_from_depth(float2 uv, float depth)
{
    // UV to normalised device coords. Y flips: UV runs top-down, NDC bottom-up.
    float4 clip = float4(uv.x * 2.0 - 1.0,
                         1.0 - uv.y * 2.0,
                         depth,
                         1.0);

    // The engine's matrix is row-major with translation in .w, so transforming
    // is a dot product per row - same convention going back through the inverse.
    float4 pos = float4(dot(uInvVP0, clip),
                        dot(uInvVP1, clip),
                        dot(uInvVP2, clip),
                        dot(uInvVP3, clip));

    return pos.xyz / pos.w;
}

float4 main(float2 uv : TEXCOORD0) : COLOR0
{
    float3 scene = tex2D(uTexture, uv).rgb;
    float  depth = tex2D(uDepth, uv).r;

    // Sky has no meaningful world position - unprojecting it gives garbage at
    // the far plane, so leave it as-is.
    if (depth >= 0.99999)
    {
        return float4(scene, 1.0);
    }

    float3 world = world_from_depth(uv, depth);

#if MODE == 0
    float2 cell = frac(world.xy / GRID_SIZE);
    float3 tint = float3(cell, 0.5);
    return float4(lerp(scene, tint, 0.65), 1.0);

#elif MODE == 1
    float h = saturate(world.z / 50.0);
    return float4(lerp(scene, float3(h, h * 0.5, 1.0 - h), 0.7), 1.0);

#elif MODE == 2
    return float4(frac(world * 0.01), 1.0);

#else
    float2 g     = abs(frac(world.xy / GRID_SIZE) - 0.5);
    float  line_ = 1.0 - saturate(min(g.x, g.y) * 12.0);
    return float4(lerp(scene, float3(1.0, 0.2, 0.2), line_), 1.0);
#endif
}
