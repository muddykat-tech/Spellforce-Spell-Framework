// 6. Ripple Water & Chromatic Aberration
sampler2D uTexture : register(s0);
float4    uParams  : register(c0);

float4 main(float2 uv : TEXCOORD0) : COLOR0
{
    float time = uParams.z;
    float2 center = uv - 0.5;
    float dist = length(center);

    // Radial ripple wave distortion
    float2 uvWarped = uv + center * (sin(dist * 35.0 - time * 4.0) * 0.025);

    // Chromatic aberration scaling with distance
    float aberration = dist * 0.04 * sin(time * 2.0);
    float r = tex2D(uTexture, uvWarped + float2(aberration, 0.0)).r;
    float g = tex2D(uTexture, uvWarped).g;
    float b = tex2D(uTexture, uvWarped - float2(aberration, 0.0)).b;

    return float4(r, g, b, 1.0);
}
