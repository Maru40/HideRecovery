#include "IncDraw.hlsli"

void Skin(inout VSBoneOutlineInput vin, uniform int boneCount)
{
    float4x3 skinning = 0;

	[unroll]
    for (int i = 0; i < boneCount; i++)
    {
        skinning += Bones[vin.indices[i]] * vin.weights[i];
    }
    vin.position.xyz = mul(vin.position, skinning);
    vin.norm = mul(vin.norm, (float3x3) skinning);
}

PSOutlineInput main(VSBoneOutlineInput input)
{
    PSOutlineInput result;

    Skin(input, 4);

	//���_�̈ʒu��ϊ�
    float4 pos = float4(input.position.xyz + (input.norm * OutlineWidth), 1.0f);
	//���[���h�ϊ�
    pos = mul(pos, World);
	//�r���[�ϊ�
    pos = mul(pos, View);
	//�ˉe�ϊ�
    pos = mul(pos, Projection);
	//�s�N�Z���V�F�[�_�ɓn���ϐ��ɐݒ�
    result.position = pos;

    result.norm = mul(input.norm, (float3x3) World);
    result.norm = normalize(result.norm);

    return result;
}
