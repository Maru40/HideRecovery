#include "IncDraw.hlsli"

SamplerState g_sampler : register(s0);
Texture2D g_noiseTexture : register(t0);

float4 main(PSOutlineInput input) : SV_TARGET
{
    if (EnabledDissolve)
    {
        // �m�C�Y�e�N�X�`�����獂���i���`�������j���擾
        float4 noise = g_noiseTexture.Sample(g_sampler, input.tex);
        float height = grayScale(noise);

        if (height > DissolveAnimationRate)
        {
            discard;
        }
    }

    return OutlineColor;
}