#include "IncDraw.hlsli"

Texture2D g_texture : register(t0);
Texture2D g_texture2 : register(t2);
SamplerState g_sampler : register(s0);

Texture2D g_noiseTexture : register(t4);

float4 main(PSPNTInput input) : SV_TARGET
{
    if (EnabledDissolve)
    {
        float4 noise = g_noiseTexture.Sample(g_sampler, input.tex);
        // ノイズテクスチャから高さ（黒～白成分）を取得
        float height = grayScale(noise);

        if (height > DissolveAnimationRate + 0.05f || DissolveAnimationRate == 0)
        {
            discard;
        }

        if (height > DissolveAnimationRate)
        {
            return DissolveEdgeColor;
        }
    }

    float4 auraArea = g_texture.Sample(g_sampler, input.tex);
    float4 whiteAura = g_texture2.Sample(g_sampler, input.tex);

    // アルファブレンド[dst = a * src1 + (1 - a) * src2]
    float a = whiteAura.a;
    float4 src1 = whiteAura;
    float4 src2 = auraArea * Diffuse;
    float4 color = a * src1 + (1 - a) * src2;

    return color;
}