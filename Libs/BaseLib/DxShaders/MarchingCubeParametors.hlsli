//インクルードガード
#ifndef __MarchingCubeParametors_HLSL__ 
#define __MarchingCubeParametors_HLSL__ 

struct VSInput
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
    uint vertexID : SV_VertexID;
};

struct VSOutput
{
    float2 TexCoord : TEXCOORD0;
    float4 PositionWS : TEXCOORD1;
    float3 NormalWS : TEXCOORD2;
    float4 Diffuse : COLOR0;
    float4 PositionPS : SV_Position;
    uint4 vertexDiscard : COLOR1;
};

Texture2D<float4> Texture : register(t0);
sampler Sampler : register(s0);

//コンスタントバッファ
cbuffer MarchingCubeConstantBuffer : register(b0)
{
    float4 DiffuseColor : packoffset(c0);
    float3 EmissiveColor : packoffset(c1);
    float3 SpecularColor : packoffset(c2);
    float SpecularPower : packoffset(c2.w);

    float3 LightDirection[3] : packoffset(c3);
    float3 LightDiffuseColor[3] : packoffset(c6);
    float3 LightSpecularColor[3] : packoffset(c9);

    float3 EyePosition : packoffset(c12);

    float3 FogColor : packoffset(c13);
    float4 FogVector : packoffset(c14);

    float4x4 World : packoffset(c15);
    float3x3 WorldInverseTranspose : packoffset(c19);
    float4x4 WorldViewProj : packoffset(c22);

    uint4 Activeflags : packoffset(c26); //フラグ

    float4 LightPos : packoffset(c27);
    float4 EyePos : packoffset(c28);
    float4x4 LightView : packoffset(c29);
    float4x4 LightProjection : packoffset(c33);
    
    uint UseNumSpheres : packoffset(c37);
    float4 Spheres[256] : packoffset(c38);
    float4 Colors[256] : packoffset(c294);

    //uint NumUsePointLight : packoffset(c37);

    float4x4 ViewProj : packoffset(c550);
    
    float4x3 Bones[72] : packoffset(c554);
   
};


//コンスタントバッファ
cbuffer TriTableConstantBuffer : register(b1)
{
    int TriTableCb[256][16] : packoffset(c0);
}



#endif