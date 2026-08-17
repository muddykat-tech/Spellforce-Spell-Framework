// Atmospheric - e.g high fantasy ish (just pushed up saturation and a few other things)
//
// Uses colour and depth only
// all numbers used are just guesswork and "feel" rather than specific tweaking, this can likely be made far better

// Registers supplied by the framework:
//   s0 = colour capture
//   s1 = INTZ depth (value in .r)
//   c0 = (width, height, seconds, aspect)

sampler2D uTexture : register(s0);
sampler2D uDepth   : register(s1);
float4    uParams  : register(c0);

#define EXPOSURE      1.06    // overall brightness before grading
#define CONTRAST      1.18    // 1.0 = off. Pivots around mid grey.
#define BLACK_LIFT    0.015   // stops shadows crushing to pure black
#define HIGHLIGHT_ROLL 0.88   // <1 compresses highlights; reduces clipping

#define VIBRANCE      0.55    // 0 = off, 1 = very strong
#define SATURATION    1.10    // flat saturation on top of vibrance
#define WARM_HIGHLIGHT float3(1.045, 1.010, 0.955)  // sunlight tint
#define COOL_SHADOW    float3(0.955, 0.985, 1.070)  // sky bounce in shadow

#define SKY_STRENGTH  0.55    // how much of the graded sky colour to blend in
#define SKY_TOP       float3(0.16, 0.42, 0.86)   // zenith
#define SKY_HORIZON   float3(0.62, 0.80, 0.96)   // near the horizon
#define SKY_GRADIENT  1.35    // >1 pushes the deep blue further down the frame

#define HAZE_STRENGTH 0.30    // how strongly far geometry fades to haze
#define HAZE_START    0.9955  // raw depth where haze begins
#define HAZE_END      0.9999  // raw depth where haze is at full strength
#define HAZE_COLOUR   float3(0.66, 0.79, 0.93)

#define ENABLE_BLOOM  1
#define BLOOM_THRESHOLD 0.68
#define BLOOM_STRENGTH  0.22
#define BLOOM_RADIUS    2.5   // pixels

#define VIGNETTE      0.22    // 0 = off

// 1 = show the sky mask, 2 = show the haze factor, 0 = normal
#define DEBUG_MODE    0

static const float3 LUMA = float3(0.2126, 0.7152, 0.0722);

float3 apply_vibrance(float3 colour)
{
    float luma    = dot(colour, LUMA);
    float maximum = max(colour.r, max(colour.g, colour.b));
    float minimum = min(colour.r, min(colour.g, colour.b));
    float sat     = maximum - minimum;

    // Weight by how unsaturated the pixel already is, so skin and stone look
    // richer while banners and magic effects don't burn out your eyes
    float3 vibrant = lerp(luma.xxx, colour, 1.0 + VIBRANCE * (1.0 - sat));
    return lerp(luma.xxx, vibrant, SATURATION);
}

float3 tone_curve(float3 colour)
{
    colour *= EXPOSURE;
    colour = max(colour, 0.0);

    // Highlight rolloff
    colour = colour / (1.0 + colour * (1.0 - HIGHLIGHT_ROLL));

    // Contrast around mid grey.
    colour = (colour - 0.5) * CONTRAST + 0.5;

    // Lift the floor so shadows keep some detail and colour.
    colour = BLACK_LIFT + colour * (1.0 - BLACK_LIFT);

    return saturate(colour);
}

float3 split_tone(float3 colour)
{
    float luma = dot(colour, LUMA);
    float3 tint = lerp(COOL_SHADOW, WARM_HIGHLIGHT, smoothstep(0.25, 0.75, luma));
    return colour * tint;
}

// debug command
#define REVERSED_DEPTH  0 // Set to 1 enable a reversed-Z depth buffer approach (e.g sky = 0.0)

float4 main(float2 uv : TEXCOORD0) : COLOR0
{
    float3 colour = tex2D(uTexture, uv).rgb;
    float  depth  = tex2D(uDepth, uv).r;

    float sky = 0.0;
    float haze = 0.0;

    #if REVERSED_DEPTH
        sky  = step(depth, 0.00001);
        haze = saturate((HAZE_START - depth) / (HAZE_START - HAZE_END)) * (1.0 - sky);
    #else
        sky  = step(0.99999, depth);
        haze = saturate((depth - HAZE_START) / (HAZE_END - HAZE_START)) * (1.0 - sky);
    #endif

#if DEBUG_MODE == 1
    return float4(sky.xxx, 1.0);
#elif DEBUG_MODE == 2
    return float4(haze.xxx, 1.0);
#endif

    float3 sky_colour = lerp(SKY_HORIZON, SKY_TOP, pow(saturate(1.0 - uv.y), SKY_GRADIENT));

    colour = lerp(colour, lerp(HAZE_COLOUR, sky_colour, 0.5), haze * HAZE_STRENGTH);
    colour = lerp(colour, colour * 0.45 + sky_colour * 0.75, sky * SKY_STRENGTH);

    colour = tone_curve(colour);
    colour = apply_vibrance(colour);
    colour = split_tone(colour);

#if ENABLE_BLOOM
    // Scale bloom offset dynamically based on screen resolution (normalized to 1080p baseline)
    float2 resolutionScale = uParams.xy / float2(1920.0, 1080.0);
    float2 texel = (BLOOM_RADIUS * resolutionScale) / uParams.xy;

    float3 glow  = 0.0;
    glow += tex2D(uTexture, uv + float2(-texel.x, -texel.y)).rgb;
    glow += tex2D(uTexture, uv + float2( texel.x, -texel.y)).rgb;
    glow += tex2D(uTexture, uv + float2(-texel.x,  texel.y)).rgb;
    glow += tex2D(uTexture, uv + float2( texel.x,  texel.y)).rgb;
    glow *= 0.25;

    float glow_luma = dot(glow, LUMA);
    glow *= saturate((glow_luma - BLOOM_THRESHOLD) / max(1.0 - BLOOM_THRESHOLD, 0.001));
    colour += glow * BLOOM_STRENGTH;
#endif

#if ENABLE_VIGNETTE
    float2 centred = (uv - 0.5) * float2(uParams.w, 1.0);
    float  falloff = 1.0 - saturate(dot(centred, centred) * VIGNETTE_AMOUNT * 2.0);
    colour *= lerp(1.0, falloff, 0.65);
#endif

    return float4(saturate(colour), 1.0);
}
