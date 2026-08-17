// 7. Psychedelic Effect or just the "trippy" shader

sampler2D uTexture : register(s0);
float4    uParams  : register(c0);

float4 main(float2 uv : TEXCOORD0) : COLOR0
{
    float time = uParams.z;

    float2 uvWarped = uv;
    uvWarped.x += sin(uv.y * 15.0 + time * 2.5) * 0.015;
    uvWarped.y += cos(uv.x * 15.0 + time * 2.0) * 0.015;

    float aberration = 0.008 * sin(time * 1.5);

    float r = tex2D(uTexture, uvWarped + float2(aberration, 0.0)).r;
    float g = tex2D(uTexture, uvWarped).g;
    float b = tex2D(uTexture, uvWarped - float2(aberration, 0.0)).b;

    float3 colour = float3(r, g, b);

    float3 tint = 0.5 + 0.5 * cos(time + float3(0.0, 2.0, 4.0) + uv.xyx * 3.0);
    colour *= tint;

    return float4(colour, 1.0);
}
