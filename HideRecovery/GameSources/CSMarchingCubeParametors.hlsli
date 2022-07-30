//�C���N���[�h�K�[�h
#ifndef __CSMARCHINGCUBEPARAMETORS_HLSL__ 
#define __CSMARCHINGCUBEPARAMETORS_HLSL__ 

struct VertexPositionNormalTexture
{
    float3 position;
    float3 normal;
    float2 texcode;
};

//--------------------------------------------------------------------------------------
// �R���X�^���g�o�b�t�@
//--------------------------------------------------------------------------------------
cbuffer CB : register(b0)
{
    float4 g_spheres;
    //�X�t�B�A�p�̃t���O
    //x == �g���X�t�B�A�̐�
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
// �G�������g�f�[�^
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
// �p�����[�^�f�[�^
//--------------------------------------------------------------------------------------



#endif