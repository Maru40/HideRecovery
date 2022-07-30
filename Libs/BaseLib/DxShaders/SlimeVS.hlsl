#include "SlimeParametors.hlsli"
#include "SlimeFunctions.hlsli"

VSOutputPixelLightingTx main(VSInputNmTx input)
{
    //VS_OutPut vOut;
    VSOutputPixelLightingTx vout;
    
    //’¸“_‚ÌˆÊ’u‚ð•ÏŠ·
    float4 pos = float4(input.Position.xyz, 1.0f);
	
    CommonVSOutputPixelLighting cout = ComputeCommonVSOutputPixelLighting(input.Position, input.Normal);

    vout.PositionPS = cout.Pos_ps;
    vout.PositionWS = float4(cout.Pos_ws, cout.FogFactor);
    vout.NormalWS = cout.Normal_ws;
    vout.Diffuse = DiffuseColor;
    vout.TexCoord = input.TexCoord;
    
    return vout;
}