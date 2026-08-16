// 3. Pixelation Grid (Retro)
sampler2D uTexture : register(s0);
float4    uParams  : register(c0);

float4 main(float2 uv : TEXCOORD0) : COLOR0
{
    float pixels = 384.0; // Adjust resolution scale
    float2 uvPixel = floor(uv * pixels) / pixels;
    float3 colour = tex2D(uTexture, uvPixel).rgb;
    return float4(colour, 1.0);
}
