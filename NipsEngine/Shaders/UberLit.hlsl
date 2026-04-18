#include "Util.hlsli"

#define NUM_POINT_LIGHT 4
#define NUM_SPOT_LIGHT 4
#define NUM_DECAL 32

struct FLightingResult
{
    float3 Diffuse;
    float3 Specular;
};

struct FAmbientLightInfo
{
    // TODO : 정렬 맞추십쇼
};

struct FDirectionalLightInfo
{
    // TODO : 정렬 맞추십쇼
    float3 DirectionInWorld;
    float Intensity;
    float3 Color;
    float Padding;
};

struct FPointLightInfo
{
    // TODO : 정렬 맞추십쇼
};

struct FSpotLightInfo
{
    // TODO : 정렬 맞추십쇼
};

cbuffer FrameBuffer : register(b0)
{
    row_major float4x4 View;
    row_major float4x4 Projection;
    float3 CameraPosition;
    float Padding1;
}

cbuffer Lighting : register(b1)
{
    FAmbientLightInfo Ambient;
    FDirectionalLightInfo Directional;
    FPointLightInfo PointLights[NUM_POINT_LIGHT];
    FSpotLightInfo SpotLights[NUM_SPOT_LIGHT];
}

cbuffer PerObject : register(b2)
{
    row_major float4x4 World;
    row_major float4x4 WorldInvTans;
}

Texture2D DiffuseMap : register(t0);
Texture2D NormalMap : register(t1);

SamplerState SampleState : register(s0);

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
#if LIGHTING_MODEL_GOURAUD
    float3 LightColor : TEXCOORD3;
#endif
};

FLightingResult CalculateDirectionalLight(FDirectionalLightInfo Light, float3 WorldPos, float3 N)
{
    FLightingResult result;
    result.Diffuse = float3(0, 0, 0);
    result.Specular = float3(0, 0, 0);
#if LIGHTING_MODEL_GOURAUD
    result.Diffuse = Light.Color * Light.Intensity * dot(Light.DirectionInWorld, N);
#elif LIGHTING_MODEL_LAMBERT
    result.Diffuse = Light.Color * Light.Intensity * max(dot(Light.DirectionInWorld, N), 0.0f);
#elif LIGHTING_MODEL_PHONG
    result.Diffuse = Light.Color * Light.Intensity * max(dot(Light.DirectionInWorld, N), 0.0f);
    float3 L = normalize(Light.DirectionInWorld);
    float3 V = normalize(CameraPosition - WorldPos);
    float3 H = normalize(L + V);
    float specular = pow(max(dot(normalize(N), H), 0.0f), 16.0f);
    result.Specular = Light.Color * (Light.Intensity * specular);
#endif
    return result;
}

FLightingResult CalculateLighting(float3 WorldPos, float3 N)
{
    FLightingResult totalLighting;
    totalLighting.Diffuse = float3(0, 0, 0);
    totalLighting.Specular = float3(0, 0, 0);

    FLightingResult directionalLight = CalculateDirectionalLight(Directional, WorldPos, N);
    totalLighting.Diffuse += directionalLight.Diffuse;
    totalLighting.Specular += directionalLight.Specular;

    // TODO: Ambient, Points, Spot Lights 계산 추가

    return totalLighting;
}


PSInput Uber_VS(VSInput Input)
{
    PSInput output;

    float4x4 MVP = mul(World, mul(View, Projection));

    float4 Pos4 = float4(Input.Position, 1.0f);
    output.ClipPos = ApplyMVP(Pos4, MVP);
    output.WorldPos = mul(Pos4, World);
    output.WorldNormal = ApplyMVP(Pos4, WorldInvTans);
    output.UV = Input.UV;

#if LIGHTING_MODEL_GOURAUD
    FLightingResult totalLighting = CalculateLighting(output.WorldPos, output.WorldNormal);
    output.LightColor = totalLighting.Diffuse;

#elif LIGHTING_MODEL_LAMBERT
    // Nothing to do
#elif LIGHTING_MODEL_PHONG
    // Nothing to do
#endif
    return output;
}

struct PSOutput
{
    float4 Color : SV_TARGET0;
    float4 Normal : SV_TARGET1;
    float4 WorldPos : SV_TARGET2;
};

PSOutput Uber_PS(PSInput Input)
{
    PSOutput output;

    float4 DiffuseColor = DiffuseMap.Sample(SampleState, Input.UV);

    // Primitive

    // Decal

    // Lighting
#if LIGHTING_MODEL_GOURAUD
    output.Color = float4(DiffuseColor.rgb * Input.LightColor, DiffuseColor.a);
#elif LIGHTING_MODEL_LAMBERT
    FLightingResult totalLighting = CalculateLighting(Input.WorldPos, Input.WorldNormal);
    output.Color = float4(DiffuseColor.rgb * totalLighting.Diffuse, DiffuseColor.a);
#elif LIGHTING_MODEL_PHONG
    FLightingResult totalLighting = CalculateLighting(Input.WorldPos, Input.WorldNormal);
    output.Color = float4(DiffuseColor.rgb * totalLighting.Diffuse + totalLighting.Specular, DiffuseColor.a);
#endif

    output.Normal = float4(Input.WorldNormal * 0.5f + 0.5f, 1.0f);
    output.WorldPos = float4(Input.WorldPos, 1.0f);
    return output;
}
