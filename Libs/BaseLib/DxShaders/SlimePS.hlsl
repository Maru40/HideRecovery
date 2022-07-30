
#include "SlimeParametors.hlsli"
#include "SlimeFunctions.hlsli"

// �o�̓f�[�^�p�̍\����
PS_OutPut main(VSOutputPixelLightingTx input)
{   
    PS_OutPut outPut;

    float3 position = input.PositionWS.xyz;                      // ���C�̍��W�i�s�N�Z���̃��[���h���W�ŏ������j
    const float3 rayDir = normalize(position.xyz - EyePosition); // ���C�̐i�s����

    for (int i = 0; i < NumRayLoop; i++)
    {
        // position�Ƌ��Ƃ̍ŒZ����
        float dist = GetDistance(position);

        // �������w�苗���ȉ��ɂȂ�����A�F�Ɛ[�x����������ŏ����I��
        if (dist < RayHitDistance) 
        {
            return CalculateRayMarchingPSOutPut(input, position, rayDir);
        }
        
        // ���C�̕����ɍs�i
        position += dist * rayDir;
    }
    
    // �Փ˔��肪�Ȃ������瓧���ɂ���
    outPut.col = 0;
    
    return outPut;
}