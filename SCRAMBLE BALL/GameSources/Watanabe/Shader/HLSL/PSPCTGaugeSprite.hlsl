#include "INCPCTGaugeSprite.hlsli"

Texture2D g_texture : register(t0);
Texture2D g_colorTexture : register(t1);
SamplerState g_sampler : register(s0);

// �Q�l�F [Shader Graph��remap�m�[�h���֗������� - �Ȃ񂩂��](https://nanka.hateblo.jp/entry/2019/04/12/035111)
// Remap�֐�
float remap(float val, float inMin, float inMax, float outMin, float outMax)
{
    return clamp(outMin + (val - inMin) * (outMax - outMin) / (inMax - inMin), outMin, outMax);
}

float4 main(PSPCTInput input) : SV_TARGET
{
    // �Q�[�W�p�O���f�[�V�����e�N�X�`���̐F���擾
    float4 gradient = g_texture.Sample(g_sampler, input.tex);
    // ������0�`1��-Threshold�`Maximum�Ƀ}�b�s���O
    float remapRatio = remap(Ratio, 0, 1, -Threshold, Maximum);

    float alpha = 0;
    // ���X����������Ȃ��@���@�������ő�l�ȉ��̂Ƃ�
    if (gradient.a > 0 && gradient.x <= Maximum)
    {
        // remapRatio��remapColor.x���ׂ�
        // alpha��remapRatio�ȉ�0�AremapRatio + Threshold�ȏ�1
        alpha = smoothstep(remapRatio, remapRatio + Threshold, gradient.x);
        alpha = 1 - alpha;

        // �Q�[�W�̐F
        float4 gaugeColor = Diffuse;
        // �F�t���O���f�[�V�����e�N�X�`��������ꍇ
        if (UseGradientTextureFlg)
        {
            // Ratio�ƌ��ɐF���擾
            gaugeColor = g_colorTexture.Sample(g_sampler, float2(Ratio, 0.0f));
        }
        // �Q�[�W�����̐F
        float4 mainColor = alpha > 0.0f ? float4(gaugeColor.xyz, 1.0f) : float4(0.0f, 0.0f, 0.0f, (float) IsBackground);
        return mainColor;
    }
    return float4(0, 0, 0, 0);

}
