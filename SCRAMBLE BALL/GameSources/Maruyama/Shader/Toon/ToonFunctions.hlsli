//�C���N���[�h�K�[�h
#ifndef __TOONFUNCTIONS_HLSL__ 
#define __TOONFUNCTIONS_HLSL__ 

#include "ToonParametors.hlsli"

//�J���[�y�A(�f�B�t���[�Y�A�X�y�L�����[)
struct ColorPair
{
    float3 Diffuse;   //�f�B�t���[�Y
    float3 Specular;  //�X�y�L�����[
};

//�t�H�O�̌v�Z�����ĕԂ�
float ComputeFogFactor(float4 position)
{
    return saturate(dot(position, FogVector));
}

//�n�����J���[�ɃX�y�L�����[�J���[��ǉ�����B
void AddSpecular(inout float4 color, float3 specular)
{
    color.rgb += specular * color.a;
}

//�n�����J���[�Ƀt�H�O��ǉ�����B
void ApplyFog(inout float4 color, float fogFactor)
{
    color.rgb = lerp(color.rgb, FogColor * color.a, fogFactor);
}

//���C�e�B���O�̌v�Z�����ĕԂ��B
ColorPair ComputeLights(float3 eyeVector, float3 worldNormal, uniform int numLights)
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
    float3 specular = pow(max(dotH, 0) * zeroL, SpecularPower) * dotL;

    ColorPair result;
    
    result.Diffuse = mul(diffuse, lightDiffuse) * DiffuseColor.rgb + EmissiveColor;
    result.Specular = mul(specular, lightSpecular) * SpecularColor;

    return result;
}

//���_�V�F�[�_�Ń��C�e�B���O�����ĕԂ�
CommonVSOutput ComputeCommonVSOutputWithLighting(float4 position, float3 normal, uniform int numLights)
{
    CommonVSOutput vout;
    
    float4 pos_ws = mul(position, World);
    float3 eyeVector = normalize(EyePosition - pos_ws.xyz);
    float3 worldNormal = normalize(mul(normal, WorldInverseTranspose));

    ColorPair lightResult = ComputeLights(eyeVector, worldNormal, numLights);
    
    vout.PositionPS = mul(position, WorldViewProj);
    vout.Diffuse = float4(lightResult.Diffuse, DiffuseColor.a);
    vout.Specular = lightResult.Specular;
    vout.FogFactor = ComputeFogFactor(position);
    
    return vout;
}

//�������[�g�̌v�Z
float CalculateRimRate(const in float3 norm, const in float3 rayDir)
{
    //�֊s�炵�� == pow(1.0f - abs(dot(�@���x�N�g��, ���C����)), �萔);
    //���p�͗֊s�ɋ߂Â��΁u0�v�ɋ߂��A�֊s���牓����΁u1�v�ɋ߂Â��B
    const float RimPower = 0.75f; // �萔
    const float rimClamp = clamp(1.0f - abs(dot(norm, rayDir)), 0.0f, 1.0f); // �萔
    const float rimRate = pow(rimClamp, RimPower); // �֊s�炵���̎w�W
    
    return rimRate;
}

#endif