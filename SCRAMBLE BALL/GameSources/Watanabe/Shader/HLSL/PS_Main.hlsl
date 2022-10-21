Texture2D g_mainRender : register(t0);

float4 main(float4 position : SV_Position) : SV_TARGET
{
    int3 sampleIndices = int3(position.xy, 0);

    return g_mainRender.Load(sampleIndices);
}