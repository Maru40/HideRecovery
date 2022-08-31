#include "ToonParametors.hlsli"
#include "ToonFunctions.hlsli"


VSOutput main( VSInput input )
{
    VSOutput vout;
	
	//法線計算
    vout.NormalWS = normalize(mul(input.Normal, WorldInverseTranspose));
    vout.InputNormal = input.Normal;
    //ピクセルポジション計算
    vout.PositionPS = mul(input.Position, WorldViewProj);
    //ワールドポジション計算
    float fogFactor = saturate(dot(input.Position, FogVector)); //fogFactorを取得。
    vout.PositionWS = float4(mul(input.Position, World).xyz, fogFactor);
    
    vout.TexCoord = input.TexCoord;
    
    //VS計算test
    CommonVSOutput commonVS = ComputeCommonVSOutputWithLighting(input.Position, input.Normal, Activeflags.x);
    vout.Diffuse = commonVS.Diffuse;
    vout.Specular = commonVS.Specular;
    vout.FogFactor = commonVS.FogFactor;
	
	return vout;
}