#include "CSMarchingCubeParametors.hlsli"
#include "CSMarchingCubeFunctions.hlsli"

//�V�F�[�_�\���\�[�X�r���[
//�C���v�b�g�ɂ̂ݎg���H
StructuredBuffer<ElemData> inputView : register(t0);

//���o�͗p�o�b�t�@(�A�N�Z�X�r���[)
//�A�E�g�v�b�g����f�[�^�͂������H
RWStructuredBuffer<ElemData> outOosData : register(u0);
//���L�ϐ�
groupshared float4 sharedPos;

//�X���b�h��
//���񂵂ē������P��
[numthreads(1, 1, 1)]//(Dispach()�̐����ɍ��킹�č���͎g���Ă�)
void main( uint3 DTid : SV_DispatchThreadID )
{
    sharedPos = inputView[DTid.x].pos;
    GroupMemoryBarrierWithGroupSync();
    sharedPos.x = sin(g_paramf.x);
    GroupMemoryBarrierWithGroupSync();
    outOosData[DTid.x].pos = sharedPos;

}