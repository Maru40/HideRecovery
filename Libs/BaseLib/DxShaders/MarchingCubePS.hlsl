#include "MarchingCubeParametors.hlsli"
#include "MarchingCubeFunction.hlsli"

float4 main(VSOutput input) : SV_TARGET
{
    if (input.vertexDiscard.w == 1)
    {
        //discard;
    }
    else
    {
        float3 normal = normalize(input.NormalWS);
        float3 color = float3(0.3f, 0.3f, 1.0f) * max(0.0f, dot(LightDirection[0], normal));
        
        return 1.0f;
        //return float4(color, 1.0f);
    }
    
    return 0;
}