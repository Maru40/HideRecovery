//#include "BcINCParameters.hlsli"
#include "PointLightParametors.hlsli"
#include "BcINCStructs.hlsli"
#include "BcINCCommon.hlsli"
#include "BcINCLighting.hlsli"

VSOutputPixelLightingTx main(VSInputNmTx vin)
{
	VSOutputPixelLightingTx vout;

	CommonVSOutputPixelLighting cout = ComputeCommonVSOutputPixelLighting(vin.Position, vin.Normal);
	SetCommonVSOutputParamsPixelLighting;

	vout.Diffuse = float4(1, 1, 1, DiffuseColor.a);
	vout.TexCoord = vin.TexCoord;

	return vout;
}

//VSOutputTx main(VSInputNmTx vin)
//{
//	VSOutputTx vout;
//
//	CommonVSOutput cout;
//
//	if (Activeflags.x > 0) {
//		cout = ComputeCommonVSOutputWithLighting(vin.Position, vin.Normal, Activeflags.x);
//	}
//	else {
//		cout = ComputeCommonVSOutput(vin.Position);
//	}
//
//	SetCommonVSOutputParams;
//
//	return vout;
//}