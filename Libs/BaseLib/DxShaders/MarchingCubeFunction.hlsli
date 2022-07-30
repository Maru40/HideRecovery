//�C���N���[�h�K�[�h
#ifndef __MarchingCubeFunctions_HLSL__ 
#define __MarchingCubeFunctions_HLSL__ 

#include "MarchingCubeParametors.hlsli"

groupshared const float PI2 = 3.14f * 2.0f;
//��ŃR���X�^���g�o�b�t�@�s
groupshared float smoothUnionValue = 0.5f;

// �����t�������֐���Ԃ��֐�
// �T���v���Ŏ��ۂɎg�p���Ă�����̂��ڂ���̂͏璷�Ȃ��߁A�����ł͒P���ȋ��̋����֐����g�p���Ă���
float GetDistance(float3 p)
{
    return length(p) - 2.0f;
}

// �@����Ԃ��֐�
float3 GetNormal(float3 p)
{
    float e = 0.01f;
    return normalize(float3(
        GetDistance(p + float3(e, 0.0, 0.0)) - GetDistance(p - float3(e, 0.0, 0.0)),
        GetDistance(p + float3(0.0, e, 0.0)) - GetDistance(p - float3(0.0, e, 0.0)),
        GetDistance(p + float3(0.0, 0.0, e)) - GetDistance(p - float3(0.0, 0.0, e))
    ));
}

// v0, v1�̒l�����Ƃ�p0, p1���Ԃ����l��Ԃ�
float3 Interpolate(float3 p0, float3 p1, float v0, float v1)
{
    return lerp(p0, p1, -v0 / (v1 - v0));
}

//�_�����Z�p
int ModI(int x, int y)
{
    return x - y * (x / y);
}

//�_���a�p
int Or(int a, int b)
{
    int result = 0;
    int n = 1;
    for (int i = 0; i < 8; i++)
    {
        if ((ModI(a, 2) == 1) || (ModI(b, 2) == 1)) {
            result += n;
        }
        a = a / 2;
        b = b / 2;
        n = n * 2;
        if (!(a > 0 || b > 0)) {
            break;
        }
    }
    
    return result;
}

//���^�{�[���p�����֐�
float sphere(float3 p, float r)
{
    return length(p) - r;
}

//Smooth Union
float OpSmoothUnion(float d1, float d2, float k)
{
    float h = clamp(0.5f + 0.5f * (d2 - d1) / k, 0.0f, 1.0f);
    return lerp(d2, d1, h) - k * h * (1.0 - h);
}

//�����_���Ƀ��^�{�[���𓮂���
float randomBlobMove(float3 position, int i, float4 randomValues)
{
    float cPI2 = PI2;
    float time = (1.0f / 60.0f);

    float temp = time * 0.002f * (0.2f + randomValues.w) + randomValues.z * 100.0f;
    float t = fmod(1, cPI2);
    float3 translate = (randomValues.xyz * 2.0f - 1.0f) * 20.0f * sin(t);
    float r = 6.0f + randomValues.x * 6.0f;
    float l = 0.3f; //sellSize
    position -= translate;

    return sphere(position, r);
}

//�ŏI�I�ȋ����֐�
float GetLastDistance(float3 position)
{
    // �K���ɉ�]
    //float time = (1.0f / 60.0f);
    //float theta = fmod(time * 0.001f, PI2);
    //position = rotateVec3
    
    float4 randomValues[2] = {
        float4(0.5f, 1.0f, 0.0f, 1.0f),
        float4(0.5f, 0.8f, 0.0f, 0.0f)
    };
    
    float result = 0.0f;
    float distance;
    for (int i = 0; i < UseNumSpheres; i++)
    {
        distance = randomBlobMove(position, i, randomValues[i]); //Shpere�ɕύX�\��
        if (result == 0.0f)
        {
            result = distance;
        }
        
        result = OpSmoothUnion(result, distance, smoothUnionValue);
    }
    
    return result;
}

#endif