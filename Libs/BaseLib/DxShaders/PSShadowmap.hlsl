//--------------------------------------------------------------------------------------
// File: VSShadowmap.hlsl
//
//--------------------------------------------------------------------------------------


#include "INCShadowmap.hlsli"

Texture2D g_noiseTexture : register(t0);
SamplerState g_sampler : register(s0);

float4 main(VS_OUTPUT input) : SV_TARGET
{
    // ノイズテクスチャから高さ（黒～白成分）を取得
    float height = g_noiseTexture.Sample(g_sampler, input.tex).r;
    if (height > DissolveAnimationRate)
    {
        discard;
    }

    return input.vPosition;
}

