struct VSPCTInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
};

struct PSPCTInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
};

cbuffer ParametersBuffer : register(b0)
{
    float4x4 World : packoffset(c0);
    float4x4 View : packoffset(c4);
    float4x4 Projection : packoffset(c8);
    float4 Emissive : packoffset(c12);
    float4 Diffuse : packoffset(c13);
    float Ratio : packoffset(c6.x);
    float Threshold : packoffset(c6.y);
    uint UseGradientTextureFlg : packoffset(c6.z);
    uint IsBackground : packoffset(c6.w);
};