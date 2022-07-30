
//インクルードガード
#ifndef __SlimeParametors_HLSL__ 
#define __SlimeParametors_HLSL__ 

//カラーペア(ディフューズ、スペキュラー)
struct ColorsPair
{
    float3 Diffuse : COLOR;
    float3 Specular : COLOR;
};

//頂点シェーダの引数データ
struct VSInputNmTx
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
};

//頂点シェーダでテクスチャのライティングを計算する場合の戻り値
struct VSOutputPixelLightingTx
{
    float2 TexCoord : TEXCOORD0;
    float4 PositionWS : TEXCOORD1;
    float3 NormalWS : TEXCOORD2;
    float4 Diffuse : COLOR0;
    float4 PositionPS : SV_Position;
};

//頂点シェーダーでライティングの計算する場合の戻り値
struct CommonVSOutputPixelLighting
{
    float4 Pos_ps;
    float3 Pos_ws;
    float3 Normal_ws;
    float FogFactor;
};

//頂点シェーダーの戻り値
struct VS_Output
{
    float2 TexCoord : TEXCOORD0;
    float4 PositionWS : TEXCOORD1;
    float3 NormalWS : TEXCOORD2;
    float4 Diffuse : COLOR0;
    float4 PositionPS : SV_Position;
};

//ピクセルシェーダーの戻り値
struct PS_OutPut
{
    float4 col : SV_Target0; // ピクセル色
    //float depth : SV_Depth;  // 深度
};

//スフィアデータ
struct SphereData
{
    float4 positionRadius;  //xyz == 位置情報, w == 半径
    float4 color;           //色
};

//コンスタントバッファ
cbuffer SlimeConstantBuffer : register(b0)
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
    
    int UseNumSpheres : packoffset(c37.x);           //使うスフィアの数
    int NumRayLoop : packoffset(c37.y);              //Rayのループ回数
    float RayHitDistance : packoffset(c37.z);        //RayHitDistanceを上げれば滑らかさが増す。(法線と深度の制度が下がる。)
    float SmoothMinConnectValue : packoffset(c37.w); //コネクト用の定数。大きければ滑らかに繋がり、小さければ通常のスフィアに近づく。
    
    float4 RimColor : packoffset(c38);               //リムの色
    float RimPower : packoffset(c39.x);              //リムの強さ
    float MinRimRate : packoffset(c39.y);            //最少リムレート
    float MaxRimRate : packoffset(c39.z);            //最大リムレート
    float ColorDistinctness : packoffset(c39.w);     //色の分離具合。小さいほど色が混ざり、大きいほど色がはっきりする。
    
    SphereData SphereDatas[256] : packoffset(c40);
    
    //float4x4 ViewProj : packoffset(c550);   
};

#endif