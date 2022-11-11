struct VSPNTBoneInput
{
    float4 position : SV_Position;
    float3 norm : NORMAL;
    float2 tex : TEXCOORD0;
    uint4 indices : BLENDINDICES0;
    float4 weights : BLENDWEIGHT0;
};

struct VSPNTInput
{
    float4 position : SV_POSITION;
    float3 norm : NORMAL;
    float2 tex : TEXCOORD;
};

struct VSOutlineInput
{
    float4 position : SV_POSITION;
    float3 norm : NORMAL;
};


struct VSBoneOutlineInput
{
    float4 position : SV_Position;
    float3 norm : NORMAL;
    uint4 indices : BLENDINDICES0;
    float4 weights : BLENDWEIGHT0;
};

struct PSOutlineInput
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD;
    float3 norm : NORMAL;
};

struct PSPNTInput
{
    float4 position : SV_POSITION;
    float3 norm : NORMAL;
    float4 specular : COLOR;
    float2 tex : TEXCOORD;
};

struct PSPNTInputShadow
{
    float4 position : SV_Position;
    float3 norm : NORMAL0;
    float3 lightRay : NORMAL1;
    float3 lightView : NORMAL2;
    float4 specular : COLOR;
    float2 tex : TEXCOORD0;
    float4 lightSpacePos : POSITION1;
};

cbuffer ParametersBuffer : register(b0)
{
    float4x4 World : packoffset(c0);
    float4x4 View : packoffset(c4);
    float4x4 Projection : packoffset(c8);
    float4 Emissive : packoffset(c12);
    float4 Diffuse : packoffset(c13);
    float4 Specular : packoffset(c14);
    float DissolveAnimationRate : packoffset(c15.x);
    float EnabledDissolve : packoffset(c15.y);
    float DissolveEdgeColor : packoffset(c16);
    float4 LightDir : packoffset(c17);
    float4 LightPos : packoffset(c18);
    float4 EyePos : packoffset(c19);
    float4x4 LightView : packoffset(c20);
    float4x4 LightProjection : packoffset(c24);
    float4x3 Bones[100] : packoffset(c28);
};

cbuffer OutlineParametersBuffer : register(b1)
{
    float4 OutlineColor : packoffset(c0);
    float OutlineWidth : packoffset(c1.x);
};