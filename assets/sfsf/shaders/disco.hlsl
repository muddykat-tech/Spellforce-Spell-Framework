// Pulsing Color Tint (Disco)
sampler2D uTexture : register(s0);
float4    uParams  : register(c0);

float4 main(float2 uv : TEXCOORD0) : COLOR0
{
    float3 colour = tex2D(uTexture, uv).rgb;
    float pulse = 0.5 + 0.5 * sin(uParams.z * 2.0);
    float3 tint = float3(1.0, pulse, 0.5);
    return float4(colour * tint, 1.0);
}
