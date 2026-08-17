// Noir
//
// Registers supplied by the framework:
// s0 = colour capture
// s1 = INTZ depth (value in .r)
// c0 = (width, height, seconds, aspect)
// c1-c4 = inverse view-projection matrix, one row per register
// c1 - c4 are not fully trustworthy, some hacks have been done to filter out object noise.

sampler2D uTexture : register(s0);
sampler2D uDepth : register(s1);
float4 uParams : register(c0);
float4 uInvViewProj[4] : register(c1);

#define EXPOSURE 0.88
#define CONTRAST 1.35
#define BLACK_LIFT 0.18
#define HIGHLIGHT_ROLL 0.62

#define VIBRANCE -0.35
#define SATURATION 0.68
#define WARM_HIGHLIGHT float3(1.08, 0.95, 0.80)
#define COOL_SHADOW float3(0.70, 0.75, 0.90)

#define SKY_STRENGTH 0.50
#define SKY_TOP float3(0.18, 0.22, 0.35)
#define SKY_HORIZON float3(0.45, 0.42, 0.40)
#define SKY_GRADIENT 1.65

#define HAZE_STRENGTH 0.48
#define HAZE_START 0.988
#define HAZE_END 0.9998
#define HAZE_COLOUR float3(0.50, 0.48, 0.45)

#define NEAR_DISTANCE 0.1         // where near grading starts (in normalized clip space)
#define NEAR_SATURATION 0.85      // boost saturation up close
#define NEAR_WARMTH float3(1.12, 1.05, 0.92)  // warmer tones up close

#define FAR_DISTANCE 0.95         // where far grading starts
#define FAR_SATURATION 0.45       // reduce saturation in distance
#define FAR_COOLNESS float3(0.75, 0.80, 1.05)  // cooler, bluer tones far away

#define ENABLE_FILM_GRAIN 1
#define GRAIN_STRENGTH 0.025
#define GRAIN_SCALE 0.9
#define ENABLE_CHROMATIC_ABERRATION 1
#define CHROMATIC_SHIFT 0.0035
#define ENABLE_VIGNETTE 1
#define VIGNETTE_AMOUNT 0.2
#define ENABLE_BLOOM 0

#define DEBUG_MODE 0

static const float3 LUMA = float3(0.2126, 0.7152, 0.0722);

float3 reconstruct_world_pos(float2 uv, float depth_sample)
{
    // Convert screen UV to clip-space coordinates [-1, 1]
    float4 clip_pos = float4(uv * 2.0 - 1.0, depth_sample, 1.0);

    // Multiply by inverse view-projection to get world position
    float4 world_pos;
    world_pos.x = dot(clip_pos, uInvViewProj[0]);
    world_pos.y = dot(clip_pos, uInvViewProj[1]);
    world_pos.z = dot(clip_pos, uInvViewProj[2]);
    world_pos.w = dot(clip_pos, uInvViewProj[3]);

    // Perspective divide
    world_pos.xyz /= world_pos.w;
    return world_pos.xyz;
}

// Calculate distance from camera
float calculate_camera_distance(float3 world_pos)
{
    return length(world_pos);
}

// Apply depth-based color grading
float3 apply_depth_grading(float3 colour, float normalized_distance)
{
    // normalized_distance: 0 = near, 1 = far

    // Interpolate saturation boost/reduction based on distance
    float depth_saturation = lerp(NEAR_SATURATION, FAR_SATURATION, normalized_distance);

    // Interpolate color tint based on distance
    float3 depth_tint = lerp(NEAR_WARMTH, FAR_COOLNESS, normalized_distance);

    // Apply saturation modulation
    float luma = dot(colour, LUMA);
    colour = lerp(luma.xxx, colour, depth_saturation);

    // Apply temperature tint
    colour *= depth_tint;

    return colour;
}

// Simple pseudo-random noise for film grain
float noise(float2 uv, float time)
{
    return frac(sin(dot(uv + time, float2(12.9898, 78.233))) * 43758.5453);
}

float3 apply_vibrance(float3 colour)
{
    float luma = dot(colour, LUMA);
    float maximum = max(colour.r, max(colour.g, colour.b));
    float minimum = min(colour.r, min(colour.g, colour.b));
    float sat = maximum - minimum;
    float3 vibrant = lerp(luma.xxx, colour, 1.0 + VIBRANCE * (1.0 - sat));
    return lerp(luma.xxx, vibrant, SATURATION);
}

float3 tone_curve(float3 colour)
{
    colour *= EXPOSURE;
    colour = max(colour, 0.0);
    colour = colour / (1.0 + colour * (1.0 - HIGHLIGHT_ROLL));
    colour = (colour - 0.5) * CONTRAST + 0.5;
    colour = BLACK_LIFT + colour * (1.0 - BLACK_LIFT);
    return saturate(colour);
}

float3 split_tone(float3 colour)
{
    float luma = dot(colour, LUMA);
    float3 tint = lerp(COOL_SHADOW, WARM_HIGHLIGHT, smoothstep(0.25, 0.75, luma));
    return colour * tint;
}

float4 main(float2 uv : TEXCOORD0) : COLOR0
{
    float3 colour = tex2D(uTexture, uv).rgb;
    float depth = tex2D(uDepth, uv).r;

    #if ENABLE_CHROMATIC_ABERRATION
    float3 colour_r = tex2D(uTexture, uv + float2(CHROMATIC_SHIFT, 0.0)).rgb;
    float3 colour_b = tex2D(uTexture, uv - float2(CHROMATIC_SHIFT, 0.0)).rgb;
    colour.r = colour_r.r;
    colour.b = colour_b.b;
    #endif

    // Standard D3D9 depth mapping
    float sky = step(0.99999, depth);
    float haze = saturate((depth - HAZE_START) / (HAZE_END - HAZE_START)) * (1.0 - sky);

    #if DEBUG_MODE == 1
    return float4(sky.xxx, 1.0);
    #elif DEBUG_MODE == 2
    return float4(haze.xxx, 1.0);
    #endif

    // Reconstruct 3D world position and calculate camera distance
    float3 world_pos = reconstruct_world_pos(uv, depth);
    float cam_distance = calculate_camera_distance(world_pos);

    // Normalize distance to 0-1 range
    // You may need to tune these based on your actual scene depth range
    float normalized_dist = saturate((cam_distance - 0.5) / 150.0);

    float3 sky_colour = lerp(SKY_HORIZON, SKY_TOP, pow(saturate(1.0 - uv.y), SKY_GRADIENT));
    colour = lerp(colour, lerp(HAZE_COLOUR, sky_colour, 0.5), haze * HAZE_STRENGTH);
    colour = lerp(colour, colour * 0.50 + sky_colour * 0.65, sky * SKY_STRENGTH);

    colour = tone_curve(colour);
    colour = apply_vibrance(colour);
    colour = split_tone(colour);

    // Apply depth-based color grading
    colour = apply_depth_grading(colour, normalized_dist);

    #if ENABLE_FILM_GRAIN
    // Add film grain
    float grain = noise(uv * GRAIN_SCALE, uParams.z);
    grain = grain * 2.0 - 1.0;
    colour += grain * GRAIN_STRENGTH;
    #endif

    #if ENABLE_VIGNETTE
    float2 centred = (uv - 0.5) * float2(uParams.w, 1.0);
    float falloff = 1.0 - saturate(dot(centred, centred) * VIGNETTE_AMOUNT * 2.0);
    colour *= lerp(1.0, falloff, 0.55);
    #endif

    return float4(saturate(colour), 1.0);
}
