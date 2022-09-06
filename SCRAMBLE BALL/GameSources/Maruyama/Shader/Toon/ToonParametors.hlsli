//インクルードガード
#ifndef __TOONPARAMETORS_HLSL__ 
#define __TOONPARAMETORS_HLSL__ 

//頂点シェーダの引数
struct VSInput
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
};

//頂点シェーダの戻り値
struct VSOutput
{
    float2 TexCoord : TEXCOORD0;
    float4 PositionWS : TEXCOORD1;
    float3 NormalWS : TEXCOORD2;
    float3 InputNormal : TEXCOORD3;
    float4 PositionPS : SV_Position;
    float4 Diffuse : COLOR0;
    float3 Specular : COLOR1;
    float FogFactor : COLOR2;
};
    
//頂点シェーダーでライティングの計算する場合の戻り値
struct CommonVSOutput
{
    float4 PositionPS;
    float4 Diffuse;
    float3 Specular;
    float FogFactor;
};

Texture2D<float4> Texture : register(t0); //テクスチャリソース
sampler Sampler : register(s0);           //サンプラーステート

//コンスタントバッファ
cbuffer ToonParameters : register(b0)
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
    
    //リムカラー

    float4x3 Bones[72] : packoffset(c37);

};

#endif