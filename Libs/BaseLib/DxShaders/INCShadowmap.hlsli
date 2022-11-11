//--------------------------------------------------------------------------------------
// File: ShadowmapParameters.hlsli
//
//--------------------------------------------------------------------------------------

cbuffer ShadowmapConstantBuffer : register( b0 )
{
	float4x4 World				: packoffset(c0);
	float4x4 View				: packoffset(c4);
	float4x4 Projection				: packoffset(c8);
    float DissolveAnimationRate : packoffset(c12.x);
    float4x3 Bones[100]              : packoffset(c13);
};

struct VS_INPUT
{
    float4 vPosition    : SV_POSITION;
    float2 TexCoord : TEXCOORD;
};

struct VS_INPUT_WEIGHTS
{
    float4 vPosition    : SV_POSITION;
    float3 Normal   : NORMAL;
    float2 TexCoord : TEXCOORD;
    uint4  Indices  : BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};

struct VS_INPUT_TANGENT_WEIGHTS
{
	float4 vPosition : SV_Position;
	float3 Normal   : NORMAL;
	float4 Tangent  : TANGENT;
    float2 TexCoord : TEXCOORD;
	uint4  Indices  : BLENDINDICES0;
	float4 Weights  : BLENDWEIGHT0;
};




struct VS_OUTPUT
{
    float4 vPosition    : SV_POSITION;
    float2 tex : TEXCOORD;
};
