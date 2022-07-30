
#include "PointLightParametors.hlsli"

//平行ライトの計算
float4 ComputeParallelLightDiffuse(const float3 selfNormalWS,const uint numLight)
{
    float4 reDiffuse = 0;
    
    float3 nor = mul(selfNormalWS, (float3x4) World).xyz;
    nor = normalize(nor);
		
    for (uint i = 0; i < numLight; i++)
    {
        float3 col = saturate(dot(nor, (float3) LightDirection[i]));
        float3 tempCol = col * LightDiffuseColor[i] * LightSpecularColor[i];
        reDiffuse += float4(tempCol.xyz, 1);
    }
    
    return reDiffuse;
}

//ポイントライトの計算
float4 ComputePointLightDiffuse(const float4 selfPosWS, const uint numUseLight)
{
    float4 reDiffuse;
    
    //減衰値
	//将来的にはコンスタントバッファを利用して受け取りたい
    const float4 Attenuation = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    //ポイントライトの計算
    for (uint i = 0; i < numUseLight; i++)
    {
        float3 dir = PointLights[i].position.xyz - selfPosWS.xyz;
        float len = length(dir);
			
		//float diffuse = saturate(dot(normalize(pin.NormalWS.xyz), dir));
        float attenuation = saturate(1.0f / (Attenuation.x + Attenuation.y * len + Attenuation.z * len * len));
			
        reDiffuse += PointLights[i].diffuse * //float4(LightDiffuseColor[i], 1.0f) *
				DiffuseColor * attenuation * PointLights[i].power; //本来は DiffuseColor == pin.Diffuse
    }
    
    return reDiffuse;
}

//ベクターの便利関数
float3 GetPosition(const float4x4 world)
{
    float x = (float) World._41;
    float y = (float) World._42;
    float z = (float) World._43;
    
    return float3(x, y, z);
}