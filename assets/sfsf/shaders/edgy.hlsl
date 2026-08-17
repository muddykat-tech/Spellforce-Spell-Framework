// Edge Detection
sampler2D uTexture : register(s0);
float4    uParams  : register(c0);

float4 main(float2 uv : TEXCOORD0) : COLOR0
{
    float2 texel = 1.0 / uParams.xy;
    float3 c  = tex2D(uTexture, uv).rgb;
    float3 cx = tex2D(uTexture, uv + float2(texel.x, 0.0)).rgb;
    float3 cy = tex2D(uTexture, uv + float2(0.0, texel.y)).rgb;

    float3 edge = abs(c - cx) + abs(c - cy);
    float luma = dot(edge, float3(0.299, 0.587, 0.114));
    return float4(luma.xxx * 3.0, 1.0);
}
