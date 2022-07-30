#include "ToonParametors.hlsli"
#include "ToonFunctions.hlsli"


VSOutput main( VSInput input )
{
    VSOutput vout;
	
	//�@���v�Z
    vout.NormalWS = normalize(mul(input.Normal, WorldInverseTranspose));
    vout.InputNormal = input.Normal;
    //�s�N�Z���|�W�V�����v�Z
    vout.PositionPS = mul(input.Position, WorldViewProj);
    //���[���h�|�W�V�����v�Z
    float fogFactor = saturate(dot(input.Position, FogVector)); //fogFactor���擾�B
    vout.PositionWS = float4(mul(input.Position, World).xyz, fogFactor);
    
    vout.TexCoord = input.TexCoord;
    
    //VS�v�Ztest
    CommonVSOutput commonVS = ComputeCommonVSOutputWithLighting(input.Position, input.Normal, Activeflags.x);
    vout.Diffuse = commonVS.Diffuse;
    vout.Specular = commonVS.Specular;
    vout.FogFactor = commonVS.FogFactor;
	
	return vout;
}