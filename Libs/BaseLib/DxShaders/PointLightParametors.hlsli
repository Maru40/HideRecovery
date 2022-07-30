
//インクルードガード
#ifndef __PointLightParametors_HLSL__ 
#define __PointLightParametors_HLSL__ 

Texture2D<float4> Texture : register(t0);
sampler Sampler : register(s0);

struct PointLightParam {
    float4 diffuse : COLOR0;
    float4 specular : COLOR1;
    float4 position : SV_Position;
    float4 power;
};

struct TestParam {
    float4 position;
};

cbuffer PointLightParameters : register(b0)
{
    float4 DiffuseColor             : packoffset(c0);
    float3 EmissiveColor            : packoffset(c1);
    float3 SpecularColor            : packoffset(c2);
    float  SpecularPower : packoffset(c2.w);

    float3 LightDirection[3]        : packoffset(c3);
    float3 LightDiffuseColor[3]     : packoffset(c6);
    float3 LightSpecularColor[3]    : packoffset(c9);

    float3 EyePosition              : packoffset(c12);

    float3 FogColor                 : packoffset(c13);
    float4 FogVector                : packoffset(c14);

    float4x4 World                  : packoffset(c15);
    float3x3 WorldInverseTranspose  : packoffset(c19);
    float4x4 WorldViewProj          : packoffset(c22);

    uint4 Activeflags              : packoffset(c26);			//フラグ

    float4 LightPos                 : packoffset(c27);
    float4 EyePos                   : packoffset(c28);
    float4x4 LightView              : packoffset(c29);
    float4x4 LightProjection        : packoffset(c33);

    uint NumUsePointLight : packoffset(c37);
    PointLightParam PointLights[256] : packoffset(c38);

    float4x3 Bones[72] : packoffset(c1062);
};

#endif // __PointLightParametors_HLSL__ 