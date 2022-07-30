#include "MarchingCubeParametors.hlsli"
#include "MarchingCubeFunction.hlsli"

uint TriTableRegister[256][16] : register(u0);
Texture2D<float4> TriTableTexture : register(t1);

VSOutput main(VSInput input)
{   
    VSOutput vout;
    
    ////法線計算
    //vout.NormalWS = normalize(mul(input.Normal, WorldInverseTranspose));
    ////ピクセルポジション計算
    //vout.PositionPS = mul(input.Position, WorldViewProj);
    ////ワールドポジション計算
    //float fogFactor = saturate(dot(input.Position, FogVector)); //fogFactorを取得。
    //vout.PositionWS = float4(mul(input.Position, World).xyz, fogFactor);
    
    //vout.Diffuse = DiffuseColor;
    //vout.TexCoord = input.TexCoord;
    
    //将来的にコンスタントバッファ行
    int3 u_marchingSpace = int3(5, 5, 5);
    int3 u_cellNum = int3(20, 20, 20); // セル数
    float3 u_cellSize = float3(5.0f / 20.0f, 5.0f / 20.0f, 5.0f / 20.0f); // セルの大きさ
    
    int cellId = floor(input.vertexID / 15.0f); // セルのID
    int vertexId = fmod(input.vertexID, 15.0f); // セル内での頂点のID
    
    int3 cellIdx = int3(
	    cellId % u_cellNum.x, 
        (cellId % (u_cellNum.x * u_cellNum.y)) / u_cellNum.x, 
        cellId / (u_cellNum.x * u_cellNum.y)
    );
    
    //セルの基準点(立方体の向かって左奥)
    float3 cellCorner = (0.5f * float3(u_cellNum) - float3(cellIdx)) * u_cellSize;

    //セルの各頂点
    float3 c0 = cellCorner;
    //float3 c1 = cellCorner + u_cellSize * float3(1.0, 0.0, 0.0);
    //float3 c2 = cellCorner + u_cellSize * float3(1.0, 1.0, 0.0);
    //float3 c3 = cellCorner + u_cellSize * float3(0.0, 1.0, 0.0);
    //float3 c4 = cellCorner + u_cellSize * float3(0.0, 0.0, 1.0);
    //float3 c5 = cellCorner + u_cellSize * float3(1.0, 0.0, 1.0);
    //float3 c6 = cellCorner + u_cellSize * float3(1.0, 1.0, 1.0);
    //float3 c7 = cellCorner + u_cellSize * float3(0.0, 1.0, 1.0);
    float3 c1 = c0 + u_cellSize * float3(1.0, 0.0, 0.0);
    float3 c2 = c0 + u_cellSize * float3(1.0, 0.0, 1.0);
    float3 c3 = c0 + u_cellSize * float3(0.0, 0.0, 1.0);
    float3 c4 = c0 + u_cellSize * float3(0.0, 1.0, 0.0);
    float3 c5 = c0 + u_cellSize * float3(1.0, 1.0, 0.0);
    float3 c6 = c0 + u_cellSize * float3(1.0, 1.0, 1.0);
    float3 c7 = c0 + u_cellSize * float3(0.0, 1.0, 1.0);
    
    // 現在のセルの各頂点の値を求める
    float v0 = GetDistance(c0);
    float v1 = GetDistance(c1);
    float v2 = GetDistance(c2);
    float v3 = GetDistance(c3);
    float v4 = GetDistance(c4);
    float v5 = GetDistance(c5);
    float v6 = GetDistance(c6);
    float v7 = GetDistance(c7);
    //float v0 = GetLastDistance(c0);
    //float v1 = GetLastDistance(c1);
    //float v2 = GetLastDistance(c2);
    //float v3 = GetLastDistance(c3);
    //float v4 = GetLastDistance(c4);
    //float v5 = GetLastDistance(c5);
    //float v6 = GetLastDistance(c6);
    //float v7 = GetLastDistance(c7);
    
    // セルの各頂点の値からルックアップテーブルを参照するためのインデックスを求める
    int cubeIdx = 0;
    if (v0 < 0.0) cubeIdx |= 1;
    if (v1 < 0.0) cubeIdx |= 2;
    if (v2 < 0.0) cubeIdx |= 4;
    if (v3 < 0.0) cubeIdx |= 8;
    if (v4 < 0.0) cubeIdx |= 16;
    if (v5 < 0.0) cubeIdx |= 32;
    if (v6 < 0.0) cubeIdx |= 64;
    if (v7 < 0.0) cubeIdx |= 128;
    
    //int tri = int(Texture.SampleLevel(Sampler, int2(cubeIdx, vertexId), 0).x);
    //float4 tempTri = Texture.SampleLevel(Sampler, int2(cubeIdx * 16 + vertexId, 0), 0);
    
    int tri = TriTableCb[cubeIdx][vertexId];
    float4 tempPosition = 0;
    float3 tempNormal = 0;
    uint v_discard = 0;

    if (tri == -1)
    {
		// -1のとき描画する三角形はないので、ピクセルを破棄するようにする
        tempPosition = float4(float3(0.0f, 0.0f, 0.0f), 1.0f);
        v_discard = 1;
    }
    else
    {
        float3 position = 0;

        if (tri == 0)
        {
            position = Interpolate(c0, c1, v0, v1);
        }
        else if (tri == 1)
        {
            position = Interpolate(c1, c2, v1, v2);
        }
        else if (tri == 2)
        {
            position = Interpolate(c2, c3, v2, v3);
        }
        else if (tri == 3)
        {
            position = Interpolate(c3, c0, v3, v0);
        }
        else if (tri == 4)
        {
            position = Interpolate(c4, c5, v4, v5);
        }
        else if (tri == 5)
        {
            position = Interpolate(c5, c6, v5, v6);
        }
        else if (tri == 6)
        {
            position = Interpolate(c6, c7, v6, v7);
        }
        else if (tri == 7)
        {
            //position = Interpolate(c7, c4, v7, v4);
            position = Interpolate(c4, c7, v4, v7);
        }
        else if (tri == 8)
        {
            position = Interpolate(c0, c4, v0, v4);
        }
        else if (tri == 9)
        {
            position = Interpolate(c1, c5, v1, v5);
        }
        else if (tri == 10)
        {
            position = Interpolate(c2, c6, v2, v6);
        }
        else if (tri == 11)
        {
            position = Interpolate(c3, c7, v3, v7);
        }

        tempPosition = float4(position, 1.0f);
        //float4 normal = (mul(u_normalMatrix, float4(getNormal(position), 0.0f))).xyz;
        tempNormal = float4(GetNormal(position), 0.0f).xyz;
        v_discard = 0;
    }
    
    //法線計算
    vout.NormalWS = normalize(mul(tempNormal, WorldInverseTranspose));
    //vout.NormalWS = normalize(mul(input.Normal, WorldInverseTranspose));
    //ピクセルポジション計算
    vout.PositionPS = mul(tempPosition, WorldViewProj);
    //vout.PositionPS = mul(input.Position, WorldViewProj);
    //vout.PositionPS = tempPosition;
    //ワールドポジション計算
    float fogFactor = saturate(dot(tempPosition, FogVector)); //fogFactorを取得。
    vout.PositionWS = float4(mul(tempPosition, World).xyz, fogFactor);
    //vout.PositionWS = float4(mul(input.Position, World).xyz, fogFactor);
    
    vout.Diffuse = DiffuseColor;
    vout.TexCoord = input.TexCoord;
    vout.vertexDiscard = 0;
    vout.vertexDiscard.w = v_discard;
    //vout.vertexDiscard.w = 0;
    
    return vout;
}