struct VSPNTBoneInput
{
    float4 position : SV_Position;
    float3 norm : NORMAL;
    float2 tex : TEXCOORD0;
    uint4 indices : BLENDINDICES0;
    float4 weights : BLENDWEIGHT0;
};

struct PSPNTInput
{
    float4 position : SV_POSITION;
    float3 norm : NORMAL;
    float4 specular : COLOR;
    float2 tex : TEXCOORD;
};

cbuffer ParametersBuffer : register(b0)
{
    float4x4 World : packoffset(c0);
    float4x4 View : packoffset(c4);
    float4x4 Projection : packoffset(c8);
    float4 Emissive : packoffset(c12);
    float4 Diffuse : packoffset(c13);
    float4 Specular : packoffset(c14);
    uint4 Activeflags : packoffset(c15); //ƒtƒ‰ƒO
    float4 LightDir : packoffset(c16);
    float4 LightPos : packoffset(c17);
    float4 EyePos : packoffset(c18);
    float4x4 LightView : packoffset(c19);
    float4x4 LightProjection : packoffset(c23);
    float4x3 Bones[100] : packoffset(c27);
};
