#include "INCPCTGaugeSprite.hlsli"

Texture2D g_texture : register(t0);
Texture2D g_colorTexture : register(t1);
SamplerState g_sampler : register(s0);

// 参考： [Shader Graphのremapノードが便利だった - なんかやる](https://nanka.hateblo.jp/entry/2019/04/12/035111)
// Remap関数
float remap(float val, float inMin, float inMax, float outMin, float outMax)
{
    return clamp(outMin + (val - inMin) * (outMax - outMin) / (inMax - inMin), outMin, outMax);
}

float4 main(PSPCTInput input) : SV_TARGET
{
    // ゲージ用グラデーションテクスチャの色を取得
    float4 gradient = g_texture.Sample(g_sampler, input.tex);
    // 割合の0～1を-Threshold～Maximumにマッピング
    float remapRatio = remap(Ratio, 0, 1, -Threshold, Maximum);

    float alpha = 0;
    // 元々が透明じゃない　かつ　割合が最大値以下のとき
    if (gradient.a > 0 && gradient.x <= Maximum)
    {
        // remapRatioとremapColor.xを比べる
        // alphaはremapRatio以下0、remapRatio + Threshold以上1
        alpha = smoothstep(remapRatio, remapRatio + Threshold, gradient.x);
        alpha = 1 - alpha;

        // ゲージの色
        float4 gaugeColor = Diffuse;
        // 色付きグラデーションテクスチャがある場合
        if (UseGradientTextureFlg)
        {
            // Ratioと元に色を取得
            gaugeColor = g_colorTexture.Sample(g_sampler, float2(Ratio, 0.0f));
        }
        // ゲージ部分の色
        float4 mainColor = alpha > 0.0f ? float4(gaugeColor.xyz, 1.0f) : float4(0.0f, 0.0f, 0.0f, (float) IsBackground);
        return mainColor;
    }
    return float4(0, 0, 0, 0);

}
