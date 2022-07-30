#include "ToonParametors.hlsli"
#include "ToonFunctions.hlsli"

Texture2D<float3> _RampTex : register(t1);

samplerCUBE _samplCube : register(s1);

float4 main(VSOutput input) : SV_TARGET
{
    //return float4(input.InputNormal, 1.0f);
    //return float4(input.NormalWS, 1.0f);
    
    float3 eyeVector = normalize(EyePosition - input.PositionWS.xyz);
    float3x3 lightDirections = 0;
    float3x3 lightDiffuse = 0;
    float3x3 lightSpecular = 0;
    float3x3 halfVectors = 0;
    const int NumLight = Activeflags.x;
    
    float4 color = 1.0f;

    float4 Albedo = 0;
    
    //Textrueがあるなら
    if (Activeflags.y == 1) {
        Albedo = Texture.Sample(Sampler, input.TexCoord);
    }
    else {
        Albedo = DiffuseColor;
        Albedo = float4(0.45f, 0.45f, 0.45f, 1.0f);
        //Albedo = input.Diffuse;
    }
    
    [unroll]
    for (int i = 0; i < NumLight; i++)
    {
        lightDirections[i] = LightDirection[i];
        lightDiffuse[i] = LightDiffuseColor[i];
        lightSpecular[i] = LightSpecularColor[i];
    
        halfVectors[i] = normalize(eyeVector - lightDirections[i]);
    }
    
    float3 dotL = mul(-lightDirections, input.NormalWS);
    float3 dotH = mul(halfVectors, input.NormalWS);
    
    float3 zeroL = step(0, dotL);

    float3 diffuse = zeroL * dotL;
    float3 specular = pow(max(dotH, 0) * zeroL, SpecularPower) * dotL;

    half direction = dot(input.NormalWS, diffuse) * 0.5f + 0.5f;

    //Rampカラー取得
    float3 ramp = _RampTex.Sample(Sampler, float2(saturate(direction), 0.5f)).rgb;

    //リムの追加
    const float rimRate = CalculateRimRate(input.NormalWS, diffuse); // 輪郭らしさの指標
    const float3 rimColor = float3(1.0000000f, 0.9607844f, 0.8078432f); // 輪郭の色
    float3 rim = clamp(lerp(Albedo.xyz, rimColor, rimRate), 0.0f, 1.0f); // 色
    
    color.rgb = mul((Albedo.xyz * ramp), lightDiffuse) * rim;
    //color.rgb = (Albedo.xyz * ramp) * rim;
    //color.rgb = Albedo.xyz * mul(LightDiffuseColor[0], ramp);
    //color.rgb = Albedo.xyz * mul(lightDiffuse, ramp) + EmissiveColor;
    //color.rgb = Albedo.xyz * mul(lightDiffuse, ramp) * rim + EmissiveColor;
    
    AddSpecular(color, input.Specular * 0.5f);
    ApplyFog(color, input.PositionWS.w);
    
    return color;
}