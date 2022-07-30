#include "CSMarchingCubeParametors.hlsli"
#include "CSMarchingCubeFunctions.hlsli"

//シェーダ―リソースビュー
//インプットにのみ使う？
StructuredBuffer<ElemData> inputView : register(t0);

//入出力用バッファ(アクセスビュー)
//アウトプットするデータはこっち？
RWStructuredBuffer<ElemData> outOosData : register(u0);
//共有変数
groupshared float4 sharedPos;

//スレッド数
//並列して動かす単位
[numthreads(1, 1, 1)]//(Dispach()の数字に合わせて今回は使ってる)
void main( uint3 DTid : SV_DispatchThreadID )
{
    sharedPos = inputView[DTid.x].pos;
    GroupMemoryBarrierWithGroupSync();
    sharedPos.x = sin(g_paramf.x);
    GroupMemoryBarrierWithGroupSync();
    outOosData[DTid.x].pos = sharedPos;

}