#include "../../HLSL/AdvStageBase.hlsli"

cbuffer OutlineBuffer : register(b1)
{
    float2 fogCoefficient;
    uint windowWidth;
    uint windowHeight;
    float4 fogColor;
}

Texture2D depthTexture : register(t0);
RWTexture2D<float4> mainTexture : register(u0);

float3 ViewPositionToDepth(float2 tex, float depth)
{
    // スクリーン座標　左上(-1,+1) 右下(+1,-1)
    float2 spos = float2(2, -2) * tex + float2(-1, 1);

    // 位置(view座標系)
    float3 vpos;
    vpos.z = projection._m32 / (depth - projection._m22);
    vpos.xy = spos.xy * vpos.z / float2(projection._m00, projection._m11);

    return vpos;
}

[numthreads(1, 1, 1)]
void main(uint2 DTid : SV_DispatchThreadID)
{
    float diffX = 10;
    float diffY = 10;
    float col00 = depthTexture[DTid + half2(-diffX, -diffY)].r;
    float col10 = depthTexture[DTid + half2(0, -diffY)].r;
    float col01 = depthTexture[DTid + half2(-diffX, 0)].r;
    float col11 = depthTexture[DTid + half2(0, 0)].r;
    float outlineValue = (col00 - col11) * (col00 - col11) + (col10 - col01) * (col10 - col01);

    float depth = depthTexture[DTid].x;
    float2 tex = DTid / float2(windowWidth, windowHeight);
    float depthLength = length(ViewPositionToDepth(tex, depth));

    float coefficient = saturate(fogCoefficient.x + depthLength * fogCoefficient.y);

    mainTexture[DTid] = lerp(float4(fogColor.rgb, 1), mainTexture[DTid], coefficient);

    //if (outlineValue - 0.01f > 0)
    //{
    //    mainTexture[DTid] = float4(1, 0, 0, 1);
    //}
    //else
    //{
    //    mainTexture[DTid] = mainTexture[DTid];
    //}
}