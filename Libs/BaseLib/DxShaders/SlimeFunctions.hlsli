
//�C���N���[�h�K�[�h
#ifndef __SlimeFunctions_HLSL__ 
#define __SlimeFunctions_HLSL__ 

#include "SlimeParametors.hlsli"
#include "SlimeFunctions.hlsli"

#define SetCommonVSOutputParamsPixelLighting \
    vout.PositionPS = cout.Pos_ps; \
    vout.PositionWS = float4(cout.Pos_ws, cout.FogFactor); \
    vout.NormalWS = cout.Normal_ws;

//�t�H�O�̌v�Z
float ComputeFogFactor(const in float4 position)
{
    return saturate(dot(position, FogVector));
}

//���_�V�F�[�_�Ń��C�e�B���O������
CommonVSOutputPixelLighting ComputeCommonVSOutputPixelLighting(const in float4 position, const in float3 normal)
{
    CommonVSOutputPixelLighting vout;
    
    vout.Pos_ps = mul(position, WorldViewProj);
    vout.Pos_ws = mul(position, World).xyz;
    vout.Normal_ws = normalize(mul(normal, WorldInverseTranspose));
    vout.FogFactor = ComputeFogFactor(position);
    
    return vout;
}

// ���̋����֐�
float SphereDistanceFunction(const in float4 sphere, const in float3 position)
{
    return length(sphere.xyz - position) - sphere.w;
}

// �[�x�v�Z
inline float GetDepth(const in float3 position)
{
    //���݂̃r���[�s��~�ˉe�s�� == UNITY_MATRIX_VP
    const float4 newPosition = mul(float4(position, 1.0f), WorldViewProj);

    float z = newPosition.z / newPosition.w;

    return z;
}

// smooth min�֐�
float SmoothMin(const in float x1, const in float x2, uniform float k)
{
    return -log(exp(-k * x1) + exp(-k * x2)) / k; //exp == �w����Ԃ��B
}

// �����ꂩ�̋��Ƃ̍ŒZ������Ԃ�
float GetDistance(const in float3 position)
{
    float dist = 10000.0f;
    
    for (int i = 0; i < UseNumSpheres; i++)
    {
        //dist = min(dist, SphereDistanceFunction(SphereDatas[i].positionRadius, position));
        dist = SmoothMin(dist, SphereDistanceFunction(SphereDatas[i].positionRadius, position), SmoothMinConnectValue);
    }
    return dist;
}

//�x�[�X�J���[�̎擾
float4 GetBaseColor(const in float3 position)
{
    float3 color = 0.0f;
    float weight = 0.01f;
    for (int i = 0; i < UseNumSpheres; i++)
    {
        const float4 sphere = SphereDatas[i].positionRadius;
        //�F�̉e���x�B
        const float influence = clamp((length(sphere.xyz - position) - sphere.w) * ColorDistinctness, 0, 1.0f); //�������߂��قǁA�F�̉e���x�������B
        const float t = 1.0f - influence * influence * (3.0f - 2.0f * influence); //�F�����炩�ɕ�Ԃ��邽�߂̒l�B(�T�C���J�[�u�̂悤�ɂȂ�B)
        color += t * SphereDatas[i].color.xyz;
        weight += t;
    }
    
    color /= weight;
    return float4(color, 1.0f);
}

// �@���̎Z�o
float3 GetNormal(const in float3 position)
{
    float d = 0.0001f;
    return normalize(float3(
        GetDistance(position + float3(d, 0.0f, 0.0f)) - GetDistance(position + float3(-d, 0.0f, 0.0f)),
        GetDistance(position + float3(0.0f, d, 0.0f)) - GetDistance(position + float3(0.0f, -d, 0.0f)),
        GetDistance(position + float3(0.0f, 0.0f, d)) - GetDistance(position + float3(0.0f, 0.0f, -d))
    ));
}

//���C�g��ɉh������
ColorsPair ComputeLightsSlime(const in float3 eyeVector, const in float3 worldNormal, uniform int numLights)
{
    float3x3 lightDirections = 0;
    float3x3 lightDiffuse = 0;
    float3x3 lightSpecular = 0;
    float3x3 halfVectors = 0;
    
    [unroll]
    for (int i = 0; i < numLights; i++)
    {
        lightDirections[i] = LightDirection[i];
        lightDiffuse[i] = LightDiffuseColor[i];
        lightSpecular[i] = LightSpecularColor[i];
        
        halfVectors[i] = normalize(eyeVector - lightDirections[i]);
    }

    float3 dotL = mul(-lightDirections, worldNormal);
    float3 dotH = mul(halfVectors, worldNormal);
    
    float3 zeroL = step(0, dotL);

    float3 diffuse = zeroL * dotL;
    float3 specularBase = clamp(max(dotH, 0) * zeroL, 0.0f, abs(max(dotH, 0) * zeroL));
    float3 specular = pow(specularBase, SpecularPower) * dotL;

    ColorsPair result;
    
    result.Diffuse = mul(diffuse, lightDiffuse) * DiffuseColor.rgb + EmissiveColor;
    result.Specular = mul(specular, lightSpecular) * SpecularColor;

    return result;
}

//�������[�g�̌v�Z
float CalculateRimRate(const in float3 norm, const in float3 rayDir)
{
    //�֊s�炵�� == pow(1.0f - abs(dot(�@���x�N�g��, ���C����)), �萔);
    //���p�͗֊s�ɋ߂Â��΁u0�v�ɋ߂��A�֊s���牓����΁u1�v�ɋ߂Â��B
    const float rimClamp = clamp(1.0f - abs(dot(norm, rayDir)), 0.0f, 1.0f); // �萔
    const float rimRate = pow(rimClamp, RimPower); // �֊s�炵���̎w�W
    
    return rimRate;
}

//Ray���q�b�g�����ꍇ��PSOutPut�f�[�^���v�Z���鏈��
PS_OutPut CalculateRayMarchingPSOutPut(const in VSOutputPixelLightingTx input, const in float3 position, const in float3 rayDir)
{
    PS_OutPut outPut;
    
    float3 baseColor = GetBaseColor(position).xyz;
    float3 norm = GetNormal(position);
            
    const float rimRate = CalculateRimRate(norm, rayDir);  //�����J���[���[�g���v�Z
    const float3 rimColor = RimColor.xyz;                  //�����J���[���擾
            
    //���C�g�̐ݒ�
    float3 eyeVector = normalize(EyePosition - position.xyz);
    float3 worldNormal = normalize(input.NormalWS);
    ColorsPair lightResult = ComputeLightsSlime(eyeVector, norm, Activeflags.x);
            
    //�J���[�v�Z
    float3 halfDir = normalize(EyePosition - position.xyz);                   // ray�����̔���(�n�C���C�g�ɕK�v�ȏ��)
    //�֊s�ɋ߂��قǖ��邭����B
    float3 color = clamp(lerp(baseColor.xyz, rimColor, rimRate), 0.0f, 1.0f); // �F
    //�֊s���痣���قǓ����ŁA�߂��قǕs�����ɂ���B
    float alpha = clamp(lerp(MinRimRate, MaxRimRate, rimRate), 0.0f, 1.0f);   // �s�����x

    color *= lightResult.Diffuse;      //���C�g�̏���ɉh
    color.rgb += lightResult.Specular; //���C�g�̃X�y�L�����[�v�Z
    color.rgb = lerp(color.rgb, FogColor, input.PositionWS.w);
    outPut.col = float4(color, alpha); //�J���[��������
    
    return outPut;
}

#endif