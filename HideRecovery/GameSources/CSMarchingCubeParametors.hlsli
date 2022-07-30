//インクルードガード
#ifndef __CSMARCHINGCUBEPARAMETORS_HLSL__ 
#define __CSMARCHINGCUBEPARAMETORS_HLSL__ 

struct VertexPositionNormalTexture
{
    float3 position;
    float3 normal;
    float2 texcode;
};

//--------------------------------------------------------------------------------------
// コンスタントバッファ
//--------------------------------------------------------------------------------------
cbuffer CB : register(b0)
{
    float4 g_spheres;
    //スフィア用のフラグ
    //x == 使うスフィアの数
    uint g_sphereFlag;
    
    uint3 g_dimSize;
    
    float4 g_paramf;
};

//TriTable
cbuffer TriTableCB : register(b1)
{
    uint g_TriTable[256][16];
};

//EdgeTable
cbuffer EdgeTableCB : register(b2)
{
    uint g_EdgeTable[256];
};

////CubeTalbe
//cbuffer CubeTableCB : register(b3)
//{
//    int g_CubeTable[256];
//};

//--------------------------------------------------------------------------------------
// エレメントデータ
//--------------------------------------------------------------------------------------
struct ElemData
{
    float4 pos;
};


struct ElementData
{
    float3 position;
    float3 normal;
    float2 texcord;
    float2 indices;
    uint useVertex;
    uint useIndex;
};


//--------------------------------------------------------------------------------------
// パラメータデータ
//--------------------------------------------------------------------------------------



#endif