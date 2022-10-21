cbuffer AdvStageBase : register(b0)
{
    float4x4 view;
    float4x4 projection;
    float4x4 inverseViewProj;
    float viewNear;
    float viewFar;
    float2 viewPort;
    float4 lightAmbient;
};