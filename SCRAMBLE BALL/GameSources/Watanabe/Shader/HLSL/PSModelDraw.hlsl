#include "IncDraw.hlsli"

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

Texture2D g_toonTexture : register(t1);

float4 main(PSPNTInput input) : SV_TARGET
{
    float3 lightdir = normalize(LightDir.xyz);
    float3 N1 = normalize(input.norm);
    float p = dot(N1, -lightdir);
    p = p * 0.5f + 0.5f;
    p = p * p;
    float4 Color = g_toonTexture.Sample(g_sampler, float2(p, 0.0f));
    Color = Color * Diffuse + Emissive;

    Color += input.specular;
    Color.a = Diffuse.a;

    if (Activeflags.x)
    {
        Color = g_texture.Sample(g_sampler, input.tex) * Color;
    }
    return Color;
}