//インクルードガード
#ifndef __TOONFUNCTIONS_HLSL__ 
#define __TOONFUNCTIONS_HLSL__ 

#include "ToonParametors.hlsli"

//カラーペア(ディフューズ、スペキュラー)
struct ColorPair
{
    float3 Diffuse;   //ディフューズ
    float3 Specular;  //スペキュラー
};

//フォグの計算をして返す
float ComputeFogFactor(float4 position)
{
    return saturate(dot(position, FogVector));
}

//渡したカラーにスペキュラーカラーを追加する。
void AddSpecular(inout float4 color, float3 specular)
{
    color.rgb += specular * color.a;
}

//渡したカラーにフォグを追加する。
void ApplyFog(inout float4 color, float fogFactor)
{
    color.rgb = lerp(color.rgb, FogColor * color.a, fogFactor);
}

//ライティングの計算をして返す。
ColorPair ComputeLights(float3 eyeVector, float3 worldNormal, uniform int numLights)
{
    float3x3 lightDirections = 0;
    float3x3 lightDiffuse = 0;
    float3x3 lightSpecular = 0;
    float3x3 halfVectors = 0;
    
    [unroll]
    for (int i = 0; i < numLights; i++)
    {
        lightDirections[i] = LightDirection[i];
        lightDiffuse[i] = LightDiffuseColor[i];
        lightSpecular[i] = LightSpecularColor[i];
        
        halfVectors[i] = normalize(eyeVector - lightDirections[i]);
    }

    float3 dotL = mul(-lightDirections, worldNormal);
    float3 dotH = mul(halfVectors, worldNormal);
    
    float3 zeroL = step(0, dotL);

    float3 diffuse = zeroL * dotL;
    float3 specular = pow(max(dotH, 0) * zeroL, SpecularPower) * dotL;

    ColorPair result;
    
    result.Diffuse = mul(diffuse, lightDiffuse) * DiffuseColor.rgb + EmissiveColor;
    result.Specular = mul(specular, lightSpecular) * SpecularColor;

    return result;
}

//頂点シェーダでライティングをして返す
CommonVSOutput ComputeCommonVSOutputWithLighting(float4 position, float3 normal, uniform int numLights)
{
    CommonVSOutput vout;
    
    float4 pos_ws = mul(position, World);
    float3 eyeVector = normalize(EyePosition - pos_ws.xyz);
    float3 worldNormal = normalize(mul(normal, WorldInverseTranspose));

    ColorPair lightResult = ComputeLights(eyeVector, worldNormal, numLights);
    
    vout.PositionPS = mul(position, WorldViewProj);
    vout.Diffuse = float4(lightResult.Diffuse, DiffuseColor.a);
    vout.Specular = lightResult.Specular;
    vout.FogFactor = ComputeFogFactor(position);
    
    return vout;
}

//リムレートの計算
float CalculateRimRate(const in float3 norm, const in float3 rayDir)
{
    //輪郭らしさ == pow(1.0f - abs(dot(法線ベクトル, レイ方向)), 定数);
    //内角は輪郭に近づけば「0」に近く、輪郭から遠ければ「1」に近づく。
    const float RimPower = 0.75f; // 定数
    const float rimClamp = clamp(1.0f - abs(dot(norm, rayDir)), 0.0f, 1.0f); // 定数
    const float rimRate = pow(rimClamp, RimPower); // 輪郭らしさの指標
    
    return rimRate;
}

#endif