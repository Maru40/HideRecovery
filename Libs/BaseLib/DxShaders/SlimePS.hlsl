
#include "SlimeParametors.hlsli"
#include "SlimeFunctions.hlsli"

// 出力データ用の構造体
PS_OutPut main(VSOutputPixelLightingTx input)
{   
    PS_OutPut outPut;

    float3 position = input.PositionWS.xyz;                      // レイの座標（ピクセルのワールド座標で初期化）
    const float3 rayDir = normalize(position.xyz - EyePosition); // レイの進行方向

    for (int i = 0; i < NumRayLoop; i++)
    {
        // positionと球との最短距離
        float dist = GetDistance(position);

        // 距離が指定距離以下になったら、色と深度を書き込んで処理終了
        if (dist < RayHitDistance) 
        {
            return CalculateRayMarchingPSOutPut(input, position, rayDir);
        }
        
        // レイの方向に行進
        position += dist * rayDir;
    }
    
    // 衝突判定がなかったら透明にする
    outPut.col = 0;
    
    return outPut;
}