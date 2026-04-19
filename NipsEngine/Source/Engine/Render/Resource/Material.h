#pragma once

#include "Object/Object.h"
#include "Texture.h"
#include "Shader.h"
#include "RenderResources.h"
#include <variant>

/**
 * Flag 에 따른 Material 관리를 위한 정보
 */
enum class EShadingModel : uint8
{
	LIGHTING_MODEL_NONE = 0,
    LIGHTING_MODEL_GOURAUD = 1,
    LIGHTING_MODEL_LAMBERT = 2,
    LIGHTING_MODEL_PHONG = 3,
};

struct FPermutationKey
{
    EShadingModel ShadingModel = EShadingModel::LIGHTING_MODEL_NONE;

	uint32 GetKey() const
	{
        uint32 Key = 0;

		Key |= (uint32)(ShadingModel);
		// Flag 추가 시 bit 연산을 통해 추가 고려
		// 예시
		// Key |= (uint32)(NewFlag) << 8;
        return Key;
	}

	FString GetKeyName() const
	{
        return std::to_string(GetKey());
	}
};

/**
 * @brief MTL 파일의 머테리얼 데이터를 표현하는 구조체.
 * Obj .mtl 포맷 기준으로 정의했습니다.
 */

struct FMaterial
{
    FString Name;

    FVector AmbientColor   = { 0.2f, 0.2f, 0.2f }; // Ka
    FVector DiffuseColor   = { 0.8f, 0.8f, 0.8f }; // Kd
    FVector SpecularColor  = { 0.0f, 0.0f, 0.0f }; // Ks
    FVector EmissiveColor  = { 0.0f, 0.0f, 0.0f }; // Ke

    float Shininess  = 0.0f; 
    float Opacity    = 1.0f; 
    int   IllumModel = 2;    

	// Texture 정보
    FString DiffuseTexPath;   // map_Kd
	bool	bHasDiffuseTexture = { false };
		 
    FString AmbientTexPath;   // map_Ka
	bool	bHasAmbientTexture = { false };

    FString SpecularTexPath;  // map_Ks
	bool	bHasSpecularTexture = { false };

	FString BumpTexPath;      // map_bump
	bool	bHasBumpTexture = { false };
};

enum class EMaterialParamType
{
	Bool,
	Int,
	UInt,
	Float,
	Vector2,
	Vector3,
	Vector4,
	Matrix4,
	Texture,
};

struct FMaterialParamValue
{
	FMaterialParamValue() : Type(EMaterialParamType::Float), Value(0.0f) {}
	FMaterialParamValue(bool InBool) : Type(EMaterialParamType::Bool), Value(InBool) {}
	FMaterialParamValue(int32 InInt) : Type(EMaterialParamType::Int), Value(InInt) {}
	FMaterialParamValue(uint32 InUInt) : Type(EMaterialParamType::UInt), Value(InUInt) {}
	FMaterialParamValue(float InScalar) : Type(EMaterialParamType::Float), Value(InScalar) {}
	FMaterialParamValue(const FVector2& InVector2) : Type(EMaterialParamType::Vector2), Value(InVector2) {}
	FMaterialParamValue(const FVector& InVector3) : Type(EMaterialParamType::Vector3), Value(InVector3) {}
	FMaterialParamValue(const FVector4& InVector4) : Type(EMaterialParamType::Vector4), Value(InVector4) {}
	FMaterialParamValue(const FMatrix& InMatrix4) : Type(EMaterialParamType::Matrix4), Value(InMatrix4) {}
	FMaterialParamValue(UTexture* InTexture) : Type(EMaterialParamType::Texture), Value(InTexture) {}

	EMaterialParamType Type;
	std::variant<bool, int32, uint32, float, FVector2, FVector, FVector4, FMatrix, UTexture*> Value;
};

class UMaterialInterface : public UObject
{
public:
	DECLARE_CLASS(UMaterialInterface, UObject)

	virtual const FString& GetName() const = 0;
	virtual FString& GetNameRef() = 0;
	virtual const FString& GetFilePath() const = 0;
	virtual FString& GetFilePathRef() = 0;
	
	virtual void Bind(ID3D11DeviceContext* Context) const = 0;
	virtual bool GetParam(const FString& Name, FMaterialParamValue& OutValue) const = 0;

	virtual void SetParam(const FString& Name, const FMaterialParamValue& Value) = 0;

	void SetBool(const FString& Name, bool Value) { SetParam(Name, FMaterialParamValue(Value)); }
	void SetInt(const FString& Name, int32 Value) { SetParam(Name, FMaterialParamValue(Value)); }
	void SetUInt(const FString& Name, uint32 Value) { SetParam(Name, FMaterialParamValue(Value)); }
	void SetFloat(const FString& Name, float Value) { SetParam(Name, FMaterialParamValue(Value)); }
	void SetVector2(const FString& Name, const FVector2& Value) { SetParam(Name, FMaterialParamValue(Value)); }
	void SetVector3(const FString& Name, const FVector& Value) { SetParam(Name, FMaterialParamValue(Value)); }
	void SetVector4(const FString& Name, const FVector4& Value) { SetParam(Name, FMaterialParamValue(Value)); }
	void SetMatrix4(const FString& Name, const FMatrix& Value) { SetParam(Name, FMaterialParamValue(Value)); }
	void SetTexture(const FString& Name, UTexture* Value) { SetParam(Name, FMaterialParamValue(Value)); }

	virtual void GatherAllParams(TMap<FString, FMaterialParamValue>& OutParams) const = 0;
};

class UMaterial : public UMaterialInterface
{
public:
	DECLARE_CLASS(UMaterial, UMaterialInterface)

	FString Name;
	FString FilePath;

	FMaterial MaterialData;
	TMap<FString, FMaterialParamValue> MaterialParams;

	// Shader 가 없으면 새로 로드해야하는데 기존 ApplyParam 등이 const 함수이므로 내부에서 ShaderMap 수정 불가
	// 논리적 상수성을 지킨다는 전제 하에 mutable 로 선언하여 기존 인터페이스 수정 최소화
    mutable TMap<uint32, UShader*> ShaderMap;
    FPermutationKey ShaderKey;

	ESamplerType SamplerType = ESamplerType::EST_Linear;
	EDepthStencilType DepthStencilType = EDepthStencilType::Default;
	EBlendType BlendType = EBlendType::Opaque;
	ERasterizerType RasterizerType = ERasterizerType::SolidBackCull;
	D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	const FString& GetName() const override { return Name; }
	FString& GetNameRef() override { return Name; }
	const FString& GetFilePath() const override { return FilePath; }
	FString& GetFilePathRef() override { return FilePath; }

	// 아무 Flag 없는 Shader 세팅
	void SetShader(UShader* InShader)
	{
        if (InShader)
	        ShaderMap[0] = InShader;
	}

	UShader* GetShader(uint32 InKey) const;

	void SetParam(const FString& Name, const FMaterialParamValue& Value)
	{
		MaterialParams[Name] = Value;
	}
	virtual bool GetParam(const FString& Name, FMaterialParamValue& OutValue) const
	{
		auto It = MaterialParams.find(Name);
		if (It != MaterialParams.end())
		{
			OutValue = It->second;
			return true;
		}
		return false;
	}

	virtual void Bind(ID3D11DeviceContext* Context) const override;

	void ApplyParams(ID3D11DeviceContext* Context, const TMap<FString, FMaterialParamValue>& Params) const;

	void GatherAllParams(TMap<FString, FMaterialParamValue>& OutParams) const
	{
		for (const auto& [Key, Param] : MaterialParams)
		{
			OutParams[Key] = Param;
		}
	}
};

class UMaterialInstance : public UMaterialInterface
{
public:
	DECLARE_CLASS(UMaterialInstance, UMaterialInterface)

	FString Name;
	FString FilePath;

	UMaterial* Parent = nullptr;

	TMap<FString, FMaterialParamValue> OverridedParams;

	const FString& GetName() const override { return Name; }
	FString& GetNameRef() override { return Name; }
	const FString& GetFilePath() const override { return FilePath; }
	FString& GetFilePathRef() override { return FilePath; }

	static UMaterialInstance* Create(UMaterial* Material)
	{
		UMaterialInstance* Instance = new UMaterialInstance();
		Instance->Parent = Material;
		return Instance;
	}

	void SetParam(const FString& Name, const FMaterialParamValue& Value)
	{
		OverridedParams[Name] = Value;
	}
	bool GetParam(const FString& Name, FMaterialParamValue& OutValue) const override
	{
		auto It = OverridedParams.find(Name);
		if (It != OverridedParams.end())
		{
			OutValue = It->second;
			return true;
		}
		return Parent ? Parent->GetParam(Name, OutValue) : false;
	}

	void Bind(ID3D11DeviceContext* Context) const override;

	void GatherAllParams(TMap<FString, FMaterialParamValue>& OutParams) const
	{
		if (Parent)
		{
			Parent->GatherAllParams(OutParams);
		}

		for (const auto& [Key, Param] : OverridedParams)
		{
			OutParams[Key] = Param;
		}
	}
};
