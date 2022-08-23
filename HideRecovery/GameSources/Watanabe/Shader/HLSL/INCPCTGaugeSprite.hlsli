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

cbuffer ConstantBuffer : register(b0)
{
    row_major float4x4 MatrixTransform : packoffset(c0);
    float4 Emissive : packoffset(c4);
    float4 Diffuse : packoffset(c5);
    float Ratio : packoffset(c6.x);
    float Threshold : packoffset(c6.y);
    uint UseGradientTextureFlg : packoffset(c6.z);
    uint IsBackground : packoffset(c6.w);
    float Maximum : packoffset(c7.x);
};