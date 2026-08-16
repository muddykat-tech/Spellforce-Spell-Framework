// Distance blur (crude depth of field).
//
// Blurs by depth: near geometry stays sharp, far geometry gets softer. The
// blur is a fixed 9-tap box whose radius scales with depth, which is cheap and
// good enough to see the effect working.
//
// Registers supplied by the framework:
//   s0 = colour capture
//   s1 = INTZ depth (value in .r)
//   c0 = (width, height, seconds, 0)

sampler2D uTexture : register(s0);
sampler2D uDepth   : register(s1);
float4    uParams  : register(c0);

// --- Tuning -----------------------------------------------------------------
// Depth is post-projection z/w and wildly nonlinear, so these are raw depth
// values, not metres. Start here and adjust by eye.
#define FOCUS_START  0.980   // everything nearer than this stays sharp
#define FOCUS_END    0.999   // at/after this, blur is at maximum
#define MAX_RADIUS   3.0     // max offset in pixels
// ----------------------------------------------------------------------------

float4 main(float2 uv : TEXCOORD0) : COLOR0
{
    float depth = tex2D(uDepth, uv).r;

    // Sky/cleared pixels sit at the far plane. Left unblurred so the horizon
    // doesn't smear; drop this line if you want the sky blurred too.
    float is_geometry = 1.0 - step(0.99999, depth);

    float blur = saturate((depth - FOCUS_START) / (FOCUS_END - FOCUS_START));
    blur *= is_geometry;

    float2 texel  = float2(1.0 / uParams.x, 1.0 / uParams.y);
    float2 radius = texel * MAX_RADIUS * blur;

    // 9-tap box. Centre weighted double so low-blur pixels stay crisp.
    float3 sum = tex2D(uTexture, uv).rgb * 2.0;
    sum += tex2D(uTexture, uv + float2(-radius.x, -radius.y)).rgb;
    sum += tex2D(uTexture, uv + float2( 0.0,      -radius.y)).rgb;
    sum += tex2D(uTexture, uv + float2( radius.x, -radius.y)).rgb;
    sum += tex2D(uTexture, uv + float2(-radius.x,  0.0     )).rgb;
    sum += tex2D(uTexture, uv + float2( radius.x,  0.0     )).rgb;
    sum += tex2D(uTexture, uv + float2(-radius.x,  radius.y)).rgb;
    sum += tex2D(uTexture, uv + float2( 0.0,       radius.y)).rgb;
    sum += tex2D(uTexture, uv + float2( radius.x,  radius.y)).rgb;

    return float4(sum / 10.0, 1.0);
}
