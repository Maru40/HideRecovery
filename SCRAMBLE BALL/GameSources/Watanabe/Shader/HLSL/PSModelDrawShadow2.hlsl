#include "IncDraw.hlsli"

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

// 深度マップ
Texture2D g_DepthMap : register(t1);
SamplerState g_SamplerDepthMap : register(s1);
Texture2D g_toonTexture : register(t2);
Texture2D g_noiseTexture : register(t4);

float4 main(PSPNTInputShadow input) : SV_TARGET
{
    // ノイズテクスチャから高さ（黒～白成分）を取得
    float height = g_noiseTexture.Sample(g_sampler, input.tex).r;
    if (height > DissolveAnimationRate)
    {
        discard;
    }

    float shadowColor = 1.0f;
	// テクセルを計算
    float2 texel = float2(
		input.lightSpacePos.x / input.lightSpacePos.w * 0.5f + 0.5f,
		input.lightSpacePos.y / input.lightSpacePos.w * -0.5f + 0.5f
		);
    float depth = 0;
    float sm = 0;
    depth = input.lightSpacePos.z / input.lightSpacePos.w - 0.00005f;
    sm = g_DepthMap.Sample(g_SamplerDepthMap, texel).x;
    if (texel.x >= 0 && texel.x <= 1 && texel.y >= 0 && texel.y <= 1)
    {
		// 現在の深度値と深度マップ上の深度値を比較
        if (sm > depth)
        {
            shadowColor = 1.0f;
        }
        else
        {
            shadowColor = 0.7f;
        }
    }

    float3 lightdir = normalize(LightDir.xyz);
    float3 N1 = normalize(input.norm);
    float p = dot(N1, -lightdir);
    p = p * 0.5f + 0.5f;
    p = p * p;
    float4 Color = g_toonTexture.Sample(g_sampler, float2(p, 0.0f));
    Color = Color * Diffuse + Emissive;

    Color += input.specular;
    Color.a = Diffuse.a;

    Color = g_texture.Sample(g_sampler, input.tex) * Color;
	//影を足す
    Color.rgb = Color.rgb * shadowColor;

    return Color;
}