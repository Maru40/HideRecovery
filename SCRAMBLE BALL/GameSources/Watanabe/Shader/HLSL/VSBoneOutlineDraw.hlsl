#include "IncDraw.hlsli"

void Skin(inout VSBoneOutlineInput vin, uniform int boneCount)
{
    float4x3 skinning = 0;

	[unroll]
    for (int i = 0; i < boneCount; i++)
    {
        skinning += Bones[vin.indices[i]] * vin.weights[i];
    }
    vin.position.xyz = mul(vin.position, skinning);
    vin.norm = mul(vin.norm, (float3x3) skinning);
}

PSOutlineInput main(VSBoneOutlineInput input)
{
    PSOutlineInput result;

    Skin(input, 4);

	//頂点の位置を変換
    float4 pos = float4(input.position.xyz + (input.norm * OutlineWidth), 1.0f);
	//ワールド変換
    pos = mul(pos, World);
	//ビュー変換
    pos = mul(pos, View);
	//射影変換
    pos = mul(pos, Projection);
	//ピクセルシェーダに渡す変数に設定
    result.position = pos;

    result.norm = mul(input.norm, (float3x3) World);
    result.norm = normalize(result.norm);

    return result;
}
