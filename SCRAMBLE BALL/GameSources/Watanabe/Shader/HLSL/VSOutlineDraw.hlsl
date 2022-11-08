#include "IncDraw.hlsli"

PSOutlineInput main(VSOutlineInput input)
{
    PSOutlineInput result;
    //頂点の位置を変換
	// アウトラインの分だけ法線方向に拡大する
    float4 pos = float4(input.position.xyz + (input.norm * OutlineWidth), 1.0f);
	//ワールド変換
    pos = mul(pos, World);
	//ビュー変換
    pos = mul(pos, View);
	//射影変換
    pos = mul(pos, Projection);
	//ピクセルシェーダに渡す変数に設定
    result.position = pos;
	//ライティング
    result.norm = mul(input.norm, (float3x3) World);
    result.norm = normalize(result.norm);

	return result;
}