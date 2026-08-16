// 1. Grayscale
// Constant register c0 is supplied by the framework:
//   x = backbuffer width in pixels
//   y = backbuffer height in pixels
//   z = seconds since the pipeline started
//   w = reserved
sampler2D uTexture : register(s0);
float4    uParams  : register(c0);

float4 main(float2 uv : TEXCOORD0) : COLOR0
{
    float3 colour = tex2D(uTexture, uv).rgb;
    float luma = dot(colour, float3(0.299, 0.587, 0.114));
    return float4(luma.xxx, 1.0);
}
