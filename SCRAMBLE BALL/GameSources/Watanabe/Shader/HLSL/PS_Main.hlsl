Texture2D g_mainRender : register(t0);
Texture2D g_depth : register(t1);

float4 main(float4 position : SV_Position) : SV_TARGET
{
    int3 sampleIndices = int3(position.xy, 0);

    float diffX = 1;
    float diffY = 1;
    float col00 = g_depth.Load(sampleIndices + half3(-diffX, -diffY, 0)).r;
    float col10 = g_depth.Load(sampleIndices + half3(0, -diffY,0)).r;
    float col01 = g_depth.Load(sampleIndices + half3(-diffX, 0,0)).r;
    float col11 = g_depth.Load(sampleIndices + half3(0, 0, 0)).r;

    float outlineValue = (col00 - col11) * (col00 - col11) + (col10 - col01) * (col10 - col01);

    return outlineValue - 0.00001f > 0 ? float4(1, 0, 0, 1) : g_mainRender.Load(sampleIndices);

}