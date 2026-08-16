// 5. CRT Scanlines & Vignette
sampler2D uTexture : register(s0);
float4    uParams  : register(c0);

float4 main(float2 uv : TEXCOORD0) : COLOR0
{
    float3 colour = tex2D(uTexture, uv).rgb;

    // Scanlines based on backbuffer height
    float scanline = sin(uv.y * uParams.y * 3.14159);
    colour *= (0.8 + 0.2 * scanline);

    // Vignette
    float2 center = uv - 0.5;
    float vignette = 1.0 - dot(center, center) * 1.5;
    colour *= saturate(vignette);

    return float4(colour, 1.0);
}
