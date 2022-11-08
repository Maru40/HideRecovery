#include "IncDraw.hlsli"

PSOutlineInput main(VSOutlineInput input)
{
    PSOutlineInput result;
    //���_�̈ʒu��ϊ�
	// �A�E�g���C���̕������@�������Ɋg�傷��
    float4 pos = float4(input.position.xyz + (input.norm * OutlineWidth), 1.0f);
	//���[���h�ϊ�
    pos = mul(pos, World);
	//�r���[�ϊ�
    pos = mul(pos, View);
	//�ˉe�ϊ�
    pos = mul(pos, Projection);
	//�s�N�Z���V�F�[�_�ɓn���ϐ��ɐݒ�
    result.position = pos;
	//���C�e�B���O
    result.norm = mul(input.norm, (float3x3) World);
    result.norm = normalize(result.norm);

	return result;
}