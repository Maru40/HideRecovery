
//�C���N���[�h�K�[�h
#ifndef __SlimeParametors_HLSL__ 
#define __SlimeParametors_HLSL__ 

//�J���[�y�A(�f�B�t���[�Y�A�X�y�L�����[)
struct ColorsPair
{
    float3 Diffuse : COLOR;
    float3 Specular : COLOR;
};

//���_�V�F�[�_�̈����f�[�^
struct VSInputNmTx
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
};

//���_�V�F�[�_�Ńe�N�X�`���̃��C�e�B���O���v�Z����ꍇ�̖߂�l
struct VSOutputPixelLightingTx
{
    float2 TexCoord : TEXCOORD0;
    float4 PositionWS : TEXCOORD1;
    float3 NormalWS : TEXCOORD2;
    float4 Diffuse : COLOR0;
    float4 PositionPS : SV_Position;
};

//���_�V�F�[�_�[�Ń��C�e�B���O�̌v�Z����ꍇ�̖߂�l
struct CommonVSOutputPixelLighting
{
    float4 Pos_ps;
    float3 Pos_ws;
    float3 Normal_ws;
    float FogFactor;
};

//���_�V�F�[�_�[�̖߂�l
struct VS_Output
{
    float2 TexCoord : TEXCOORD0;
    float4 PositionWS : TEXCOORD1;
    float3 NormalWS : TEXCOORD2;
    float4 Diffuse : COLOR0;
    float4 PositionPS : SV_Position;
};

//�s�N�Z���V�F�[�_�[�̖߂�l
struct PS_OutPut
{
    float4 col : SV_Target0; // �s�N�Z���F
    //float depth : SV_Depth;  // �[�x
};

//�X�t�B�A�f�[�^
struct SphereData
{
    float4 positionRadius;  //xyz == �ʒu���, w == ���a
    float4 color;           //�F
};

//�R���X�^���g�o�b�t�@
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

    uint4 Activeflags : packoffset(c26); //�t���O

    float4 LightPos : packoffset(c27);
    float4 EyePos : packoffset(c28);
    float4x4 LightView : packoffset(c29);
    float4x4 LightProjection : packoffset(c33);
    
    int UseNumSpheres : packoffset(c37.x);           //�g���X�t�B�A�̐�
    int NumRayLoop : packoffset(c37.y);              //Ray�̃��[�v��
    float RayHitDistance : packoffset(c37.z);        //RayHitDistance���グ��Ί��炩���������B(�@���Ɛ[�x�̐��x��������B)
    float SmoothMinConnectValue : packoffset(c37.w); //�R�l�N�g�p�̒萔�B�傫����Ί��炩�Ɍq����A��������Βʏ�̃X�t�B�A�ɋ߂Â��B
    
    float4 RimColor : packoffset(c38);               //�����̐F
    float RimPower : packoffset(c39.x);              //�����̋���
    float MinRimRate : packoffset(c39.y);            //�ŏ��������[�g
    float MaxRimRate : packoffset(c39.z);            //�ő僊�����[�g
    float ColorDistinctness : packoffset(c39.w);     //�F�̕�����B�������قǐF��������A�傫���قǐF���͂����肷��B
    
    SphereData SphereDatas[256] : packoffset(c40);
    
    //float4x4 ViewProj : packoffset(c550);   
};

#endif