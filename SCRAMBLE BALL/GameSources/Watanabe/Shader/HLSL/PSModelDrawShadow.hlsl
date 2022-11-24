#include "IncDraw.hlsli"

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

// [“xƒ}ƒbƒv
Texture2D g_DepthMap : register(t1);
SamplerComparisonState g_SamplerDepthMap : register(s1);
Texture2D g_toonTexture : register(t2);
Texture2D g_noiseTexture : register(t4);

float3 DplusS(float3 N, float3 L, float NdotL, float3 view);

float4 main(PSPNTInputShadow input) : SV_TARGET
{
    if (EnabledDissolve)
    {
        // ƒmƒCƒYƒeƒNƒXƒ`ƒƒ‚©‚ç‚‚³i•`”’¬•ªj‚ðŽæ“¾
        float4 noise = g_noiseTexture.Sample(g_sampler, input.tex);
        float height = grayScale(noise);

        if (height > DissolveAnimationRate + DissolveEdgeLength)
        {
            discard;
        }

        if (height > DissolveAnimationRate)
        {
            return DissolveEdgeColor;
        }
    }

    float3 lightdir = normalize(LightDir.xyz);
    float3 N1 = normalize(input.norm);
    float p = dot(N1, -lightdir);
    p = p * 0.5f + 0.5f;
    p = p * p;
    float4 Color = g_toonTexture.Sample(g_sampler, float2(p, 0.0f));
    Color = Color * Diffuse + Emissive;

    Color += input.specular;
    Color.a = Diffuse.a;

    Color = g_texture.Sample(g_sampler, input.tex) * Color;


	//‰e‚Ì”Z‚³
    const float3 ambient = float3(0.7f, 0.7f, 0.7f);
    float3 N = normalize(input.norm);
    float3 L = normalize(input.lightRay);
    float NdotL = dot(N, L);

    float2 shadowTexCoords;
    shadowTexCoords.x = 0.5f + (input.lightSpacePos.x / input.lightSpacePos.w * 0.5f);
    shadowTexCoords.y = 0.5f - (input.lightSpacePos.y / input.lightSpacePos.w * 0.5f);
    float pixelDepth = input.lightSpacePos.z / input.lightSpacePos.w;

    float lighting = 1;

    if ((saturate(shadowTexCoords.x) == shadowTexCoords.x) &&
		(saturate(shadowTexCoords.y) == shadowTexCoords.y) &&
		(pixelDepth > 0))
    {

        float margin = acos(saturate(NdotL));
        float epsilon = 0.0001 / margin;

        epsilon = clamp(epsilon, 0, 0.1);

        lighting = float(g_DepthMap.SampleCmpLevelZero(
			g_SamplerDepthMap,
			shadowTexCoords,
			pixelDepth + epsilon
			).r
			);
        if (lighting == 0.f)
        {
			//‰e‚Ì’†
            return float4(Color.xyz * ambient, Color.w);
        }
        else if (lighting < 1.0f)
        {
			//‰e‚Æ‰eˆÈŠO‚Ì‹«ŠEü
            float3 light = lighting * (ambient + DplusS(N, L, NdotL, input.lightView));
            float3 shadow = (1.0f - lighting) * ambient;
            return float4(Color.xyz * (light + shadow), Color.w);
        }
    }
    return float4(Color.xyz * (ambient + DplusS(N, L, NdotL, input.lightView)), Color.w);
}

float3 DplusS(float3 N, float3 L, float NdotL, float3 view)
{
    const float3 Kdiffuse = float3(.5f, .5f, .4f);
    const float3 Kspecular = float3(.2f, .2f, .3f);
    const float exponent = 3.f;

	// Compute the diffuse coefficient.
    float diffuseConst = saturate(NdotL);

	// Compute the diffuse lighting value.
    float3 diffuse = Kdiffuse * diffuseConst;

	// Compute the specular highlight.
    float3 R = reflect(-L, N);
    float3 V = normalize(view);
    float3 RdotV = dot(R, V);
    float3 specular = Kspecular * pow(saturate(RdotV), exponent);

    return (diffuse + specular);
}
