#include "Common.hlsl"

// =====================
// Macro Safety
// =====================
#ifndef LIGHTING_MODEL_GOURAUD
#define LIGHTING_MODEL_GOURAUD 0
#endif
#ifndef LIGHTING_MODEL_LAMBERT
#define LIGHTING_MODEL_LAMBERT 0
#endif
#ifndef LIGHTING_MODEL_PHONG
#define LIGHTING_MODEL_PHONG 0
#endif

// =====================
// Constant Buffer
// =====================
cbuffer StaticMeshBuffer : register(b2)
{
    float3 AmbientColor;
    float padding0;

    float3 DiffuseColor;
    float padding1;

    float3 SpecularColor;
    float Shininess;

    float2 ScrollUV;
    uint bHasDiffuseMap;
    uint bHasSpecularMap;

    float3 EmissiveColor;
    float padding2;
};

// =====================
// Textures
// =====================
Texture2D DiffuseMap : register(t0);
Texture2D AmbientMap : register(t1);
Texture2D SpecularMap : register(t2);
Texture2D BumpMap : register(t3);
SamplerState SampleState : register(s0);

// =====================
// IO
// =====================
struct VSInput
{
    float3 Position : POSITION;
    float4 Color : COLOR;
    float3 Normal : NORMAL;
    float2 UV : TEXCOORD;
};

struct PSInput
{
    float4 ClipPos : SV_POSITION;
    float3 WorldPos : TEXCOORD0;
    float3 WorldNormal : TEXCOORD1;
    float2 UV : TEXCOORD2;
    float3 Normal : NORMAL;
#if LIGHTING_MODEL_GOURAUD
    float3 GouraudColor : TEXCOORD3;
#endif
};

struct PSOutput
{
    float4 Color : SV_TARGET0;
    float4 Normal : SV_TARGET1;
    float4 WorldPos : SV_TARGET2;
};

// =====================
// VS
// =====================
PSInput mainVS(VSInput input)
{
    PSInput output;

    output.WorldPos = mul(float4(input.Position, 1.f), Model).xyz;
    output.WorldNormal = normalize(mul(input.Normal, (float3x3) WorldInvTrans));
    output.UV = input.UV + ScrollUV;
    output.ClipPos = ApplyMVP(input.Position);
    output.Normal = input.Normal;
    
#if LIGHTING_MODEL_GOURAUD
    float3 L = normalize(float3(1, 1, 1));
    float Intensity = max(0, dot(L, output.WorldNormal));
    
    output.GouraudColor = float3(Intensity, Intensity, Intensity);
#endif

    return output;
}

// =====================
// PS
// =====================
PSOutput mainPS(PSInput input)
{
    PSOutput output;

    // Diffuse texture
    float4 DiffuseTex = float4(1.f, 1.f, 1.f, 1.f);
    if ((bool) bHasDiffuseMap)
    {
        DiffuseTex = DiffuseMap.Sample(SampleState, input.UV);
        clip(DiffuseTex.a - 0.001f);
    }

    // Specular texture
    float3 SpecularTex = float3(1.f, 1.f, 1.f);
    if ((bool) bHasSpecularMap)
    {
        SpecularTex = SpecularMap.Sample(SampleState, input.UV).rgb;
    }

    // Emissive passthrough (Normal.a == 2 가 emissive 마커)
    if (any(EmissiveColor > 0.f))
    {
        output.Color = float4(EmissiveColor, 1.f) * DiffuseTex;
        output.Normal = float4(input.WorldNormal * 0.5f + 0.5f, 2.f);
        output.WorldPos = float4(input.WorldPos, 1.f);
        return output;
    }

    // Wireframe
    if (bIsWireframe > 0.5f)
    {
        output.Color = float4(WireframeRGB, 1.f);
        output.Normal = float4(input.WorldNormal * 0.5f + 0.5f, 1.f);
        output.WorldPos = float4(input.WorldPos, 1.f);
        return output;
    }

    // ── Lighting ────────────────────────────────────────────
    float3 FinalColor = float3(0, 0, 0);
    
/*
    I = ka * Ia + sum (kd * dot(L, N) * Id + ks * dot(R, V)^alpha * Is) 
*/

#if LIGHTING_MODEL_GOURAUD
    FinalColor = DiffuseColor * DiffuseTex.rgb;
    FinalColor = FinalColor * input.GouraudColor;
    
#elif LIGHTING_MODEL_LAMBERT
    float3 Albedo = DiffuseTex.rgb;
    
    float3 L = normalize(float3(1, 1, 1));
    float3 R = 2 * dot(L, input.WorldNormal) - L;
    
    float3 Diffuse = Albedo * max(0, dot(L, input.WorldNormal));

    FinalColor = Diffuse;

    
#elif LIGHTING_MODEL_PHONG    
    // Blinn-Phong
    float3 Albedo = DiffuseTex.rgb;
    
    // Directional Light
    float3 L = normalize(float3(1, 1, 1));
    
    // Directional Light    
    float3 V = float3(1, 0, 0);
    float3 H = normalize(L + V);
    
    float3 Diffuse = Albedo * max(0, dot(L, input.WorldNormal));
    float Specular = pow(dot(H, input.WorldNormal), 10);  // dot(V, R) 를 쓰면 그냥 Phong

    FinalColor = Diffuse + Specular;
    
    
    // Point Light
    float3 PointLightPos = float3(-1, 1, 1) * 1.5;

    float PointLightDist = distance(input.WorldPos, PointLightPos);
    float Phi = 3.141592;
    float Flux = 1000;
    float3 ObjectToPointDir = normalize(PointLightPos - input.WorldPos);
    float PointCos = max(0, dot(ObjectToPointDir, input.WorldNormal));
    Diffuse = Albedo * (Flux / (4 * Phi)) * (1 / (PointLightDist * PointLightDist)) * PointCos;
    
    FinalColor = Diffuse;
    
    // Cone Light
    float3 ConeLightPos = float3(1, 0, 0);
    float3 ConeLightDir = float3(-1, 0, 0);
    float3 ObjectToConeDir = normalize(ConeLightPos - input.WorldPos);
    float3 LightToObject = normalize(input.WorldPos - ConeLightPos);
    float CosAlpha = dot(LightToObject, ConeLightDir);
    
    
    float ConeFalloff = max(0, pow(CosAlpha, 2));
    
    float ConeLightIntensity = 10;
    PointCos = max(0, dot(ObjectToConeDir, input.WorldNormal));
    Diffuse = Albedo * (ConeLightIntensity * ConeFalloff) * (1 / (PointLightDist * PointLightDist)) * PointCos;
    FinalColor = Diffuse;
    if (CosAlpha >= 0.1)
    {
        float ConeFalloff = max(0, pow(CosAlpha, 10));
    
        float ConeLightIntensity = 10;
        PointCos = max(0, dot(ObjectToConeDir, input.WorldNormal));
        Diffuse = Albedo * (ConeLightIntensity * ConeFalloff) * (1 / (PointLightDist * PointLightDist)) * PointCos;
        FinalColor = Diffuse;
    }
    else
    {
        FinalColor = float3(0, 0, 0);
    }
#else
    // Unlit fallback
    FinalColor = DiffuseColor * DiffuseTex.rgb;
#endif
    // ────────────────────────────────────────────────────────
    output.Color = float4(FinalColor, 1.f);
    output.Normal = float4(input.WorldNormal * 0.5f + 0.5f, 1.f);
    output.WorldPos = float4(input.WorldPos, 1.f);
    return output;
}