
//インクルードガード
#ifndef __SlimeFunctions_HLSL__ 
#define __SlimeFunctions_HLSL__ 

#include "SlimeParametors.hlsli"
#include "SlimeFunctions.hlsli"

#define SetCommonVSOutputParamsPixelLighting \
    vout.PositionPS = cout.Pos_ps; \
    vout.PositionWS = float4(cout.Pos_ws, cout.FogFactor); \
    vout.NormalWS = cout.Normal_ws;

//フォグの計算
float ComputeFogFactor(const in float4 position)
{
    return saturate(dot(position, FogVector));
}

//頂点シェーダでライティングをする
CommonVSOutputPixelLighting ComputeCommonVSOutputPixelLighting(const in float4 position, const in float3 normal)
{
    CommonVSOutputPixelLighting vout;
    
    vout.Pos_ps = mul(position, WorldViewProj);
    vout.Pos_ws = mul(position, World).xyz;
    vout.Normal_ws = normalize(mul(normal, WorldInverseTranspose));
    vout.FogFactor = ComputeFogFactor(position);
    
    return vout;
}

// 球の距離関数
float SphereDistanceFunction(const in float4 sphere, const in float3 position)
{
    return length(sphere.xyz - position) - sphere.w;
}

// 深度計算
inline float GetDepth(const in float3 position)
{
    //現在のビュー行列×射影行列 == UNITY_MATRIX_VP
    const float4 newPosition = mul(float4(position, 1.0f), WorldViewProj);

    float z = newPosition.z / newPosition.w;

    return z;
}

// smooth min関数
float SmoothMin(const in float x1, const in float x2, uniform float k)
{
    return -log(exp(-k * x1) + exp(-k * x2)) / k; //exp == 指数を返す。
}

// いずれかの球との最短距離を返す
float GetDistance(const in float3 position)
{
    float dist = 10000.0f;
    
    for (int i = 0; i < UseNumSpheres; i++)
    {
        //dist = min(dist, SphereDistanceFunction(SphereDatas[i].positionRadius, position));
        dist = SmoothMin(dist, SphereDistanceFunction(SphereDatas[i].positionRadius, position), SmoothMinConnectValue);
    }
    return dist;
}

//ベースカラーの取得
float4 GetBaseColor(const in float3 position)
{
    float3 color = 0.0f;
    float weight = 0.01f;
    for (int i = 0; i < UseNumSpheres; i++)
    {
        const float4 sphere = SphereDatas[i].positionRadius;
        //色の影響度。
        const float influence = clamp((length(sphere.xyz - position) - sphere.w) * ColorDistinctness, 0, 1.0f); //距離が近いほど、色の影響度が増す。
        const float t = 1.0f - influence * influence * (3.0f - 2.0f * influence); //色を滑らかに補間するための値。(サインカーブのようになる。)
        color += t * SphereDatas[i].color.xyz;
        weight += t;
    }
    
    color /= weight;
    return float4(color, 1.0f);
}

// 法線の算出
float3 GetNormal(const in float3 position)
{
    float d = 0.0001f;
    return normalize(float3(
        GetDistance(position + float3(d, 0.0f, 0.0f)) - GetDistance(position + float3(-d, 0.0f, 0.0f)),
        GetDistance(position + float3(0.0f, d, 0.0f)) - GetDistance(position + float3(0.0f, -d, 0.0f)),
        GetDistance(position + float3(0.0f, 0.0f, d)) - GetDistance(position + float3(0.0f, 0.0f, -d))
    ));
}

//ライトを繁栄させる
ColorsPair ComputeLightsSlime(const in float3 eyeVector, const in float3 worldNormal, uniform int numLights)
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
    float3 specularBase = clamp(max(dotH, 0) * zeroL, 0.0f, abs(max(dotH, 0) * zeroL));
    float3 specular = pow(specularBase, SpecularPower) * dotL;

    ColorsPair result;
    
    result.Diffuse = mul(diffuse, lightDiffuse) * DiffuseColor.rgb + EmissiveColor;
    result.Specular = mul(specular, lightSpecular) * SpecularColor;

    return result;
}

//リムレートの計算
float CalculateRimRate(const in float3 norm, const in float3 rayDir)
{
    //輪郭らしさ == pow(1.0f - abs(dot(法線ベクトル, レイ方向)), 定数);
    //内角は輪郭に近づけば「0」に近く、輪郭から遠ければ「1」に近づく。
    const float rimClamp = clamp(1.0f - abs(dot(norm, rayDir)), 0.0f, 1.0f); // 定数
    const float rimRate = pow(rimClamp, RimPower); // 輪郭らしさの指標
    
    return rimRate;
}

//Rayがヒットした場合にPSOutPutデータを計算する処理
PS_OutPut CalculateRayMarchingPSOutPut(const in VSOutputPixelLightingTx input, const in float3 position, const in float3 rayDir)
{
    PS_OutPut outPut;
    
    float3 baseColor = GetBaseColor(position).xyz;
    float3 norm = GetNormal(position);
            
    const float rimRate = CalculateRimRate(norm, rayDir);  //リムカラーレートを計算
    const float3 rimColor = RimColor.xyz;                  //リムカラーを取得
            
    //ライトの設定
    float3 eyeVector = normalize(EyePosition - position.xyz);
    float3 worldNormal = normalize(input.NormalWS);
    ColorsPair lightResult = ComputeLightsSlime(eyeVector, norm, Activeflags.x);
            
    //カラー計算
    float3 halfDir = normalize(EyePosition - position.xyz);                   // ray方向の反対(ハイライトに必要な情報)
    //輪郭に近いほど明るくする。
    float3 color = clamp(lerp(baseColor.xyz, rimColor, rimRate), 0.0f, 1.0f); // 色
    //輪郭から離れるほど透明で、近いほど不透明にする。
    float alpha = clamp(lerp(MinRimRate, MaxRimRate, rimRate), 0.0f, 1.0f);   // 不透明度

    color *= lightResult.Diffuse;      //ライトの情報を繁栄
    color.rgb += lightResult.Specular; //ライトのスペキュラー計算
    color.rgb = lerp(color.rgb, FogColor, input.PositionWS.w);
    outPut.col = float4(color, alpha); //カラー書き込み
    
    return outPut;
}

#endif