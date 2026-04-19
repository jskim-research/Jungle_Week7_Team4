#include "Material.h"
#include "Core/ResourceManager.h"

DEFINE_CLASS(UMaterialInterface, UObject)
DEFINE_CLASS(UMaterial, UMaterialInterface)
DEFINE_CLASS(UMaterialInstance, UMaterialInterface)


void UMaterial::Bind(ID3D11DeviceContext* Context) const
{
    UShader* Shader = GetShader(ShaderKey.GetKey());
    if (!Shader)
        return;

	Shader->Bind(Context);

	auto DSState = FResourceManager::Get().GetOrCreateDepthStencilState(DepthStencilType);
	auto BlendState = FResourceManager::Get().GetOrCreateBlendState(BlendType);
	auto RasterizerState = FResourceManager::Get().GetOrCreateRasterizerState(RasterizerType);
	auto Sampler = FResourceManager::Get().GetOrCreateSamplerState(SamplerType);
	
	// TODO: Render Pipeline Stalling 방지 추가 필요
	Context->OMSetDepthStencilState(DSState, 0);
	Context->OMSetBlendState(BlendState, nullptr, 0xFFFFFFFF);
	Context->RSSetState(RasterizerState);
	Context->PSSetSamplers(0, 1, &Sampler);

	ApplyParams(Context, MaterialParams);
}

UShader* UMaterial::GetShader(uint32 InKey) const
{
    auto It = ShaderMap.find(InKey);

    if (It == ShaderMap.end() || !It->second)
    {
        // 없으면 새로 생성
        D3D11_INPUT_ELEMENT_DESC NormalVertexInputLayout[4] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, static_cast<uint32>(offsetof(FNormalVertex, Position)), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, static_cast<uint32>(offsetof(FNormalVertex, Color)), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, static_cast<uint32>(offsetof(FNormalVertex, Normal)), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, static_cast<uint32>(offsetof(FNormalVertex, UVs)), D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

		D3D_SHADER_MACRO ShaderMacro[4];
        
		switch (ShaderKey.ShadingModel)
		{
        case EShadingModel::LIGHTING_MODEL_NONE:
            ShaderMacro[0] = { "LIGHTING_MODEL_GOURAUD", "0" };
            ShaderMacro[1] = { "LIGHTING_MODEL_PHONG", "0" };
            ShaderMacro[2] = { "LIGHTING_MODEL_LAMBERT", "0" };
            ShaderMacro[3] = { nullptr, nullptr };
            break;
        case EShadingModel::LIGHTING_MODEL_GOURAUD:
            ShaderMacro[0] = { "LIGHTING_MODEL_GOURAUD", "1" };
            ShaderMacro[1] = { "LIGHTING_MODEL_PHONG", "0" };
            ShaderMacro[2] = { "LIGHTING_MODEL_LAMBERT", "0" };
            ShaderMacro[3] = { nullptr, nullptr };
            break;

		case EShadingModel::LIGHTING_MODEL_PHONG:
            ShaderMacro[0] = { "LIGHTING_MODEL_GOURAUD", "0" };
            ShaderMacro[1] = { "LIGHTING_MODEL_PHONG", "1" };
            ShaderMacro[2] = { "LIGHTING_MODEL_LAMBERT", "0" };
            ShaderMacro[3] = { nullptr, nullptr };
            break;

		case EShadingModel::LIGHTING_MODEL_LAMBERT:
            ShaderMacro[0] = { "LIGHTING_MODEL_GOURAUD", "0" };
            ShaderMacro[1] = { "LIGHTING_MODEL_PHONG", "0" };
            ShaderMacro[2] = { "LIGHTING_MODEL_LAMBERT", "1" };
            ShaderMacro[3] = { nullptr, nullptr };
            break;
		}

		FString ShaderFilePath = "Shaders/UberLit.hlsl";
        // ShaderFilePath = "Shaders/ShaderStaticMesh.hlsl";
		
		// ShaderKey에 따라서 ShaderFilePath_ShaderKey.GetKeyName() 을 Key 로 새로 Shader Map
        bool bLoaded = FResourceManager::Get().LoadShaderUsingKey(ShaderFilePath, ShaderKey, "mainVS", "mainPS", NormalVertexInputLayout, ARRAYSIZE(NormalVertexInputLayout), ShaderMacro);
        assert(bLoaded && "Failed to load shader in material");

        ShaderMap[InKey] = FResourceManager::Get().GetShader(ShaderFilePath + "_" + ShaderKey.GetKeyName());
        return ShaderMap[InKey];
    }

    return It->second;
}

void UMaterial::ApplyParams(ID3D11DeviceContext* Context, const TMap<FString, FMaterialParamValue>& Params) const
{
    UShader* Shader = GetShader(ShaderKey.GetKey());
	TArray<uint8> CBufferData(Shader->GetCBufferSize());

	for (const auto& [Name, ParamValue] : Params)
	{
		FShaderVariableInfo VarInfo;
		if (Shader->GetShaderVariableInfo(Name, VarInfo))
		{
			switch (ParamValue.Type)
			{
			case EMaterialParamType::Bool:
			{
				uint32 BoolVal = std::get<bool>(ParamValue.Value) ? 1 : 0;
				std::memcpy(CBufferData.data() + VarInfo.Offset, &BoolVal, sizeof(uint32));
				break;
			}
			case EMaterialParamType::Int:
			{
				int32 Val = std::get<int32>(ParamValue.Value);
				std::memcpy(CBufferData.data() + VarInfo.Offset, &Val, sizeof(int32));
				break;
			}
			case EMaterialParamType::UInt:
			{
				uint32 UIntVal = std::get<uint32>(ParamValue.Value);
				std::memcpy(CBufferData.data() + VarInfo.Offset, &UIntVal, sizeof(uint32));
				break;
			}
			case EMaterialParamType::Float:
			{
				float Val = std::get<float>(ParamValue.Value);
				std::memcpy(CBufferData.data() + VarInfo.Offset, &Val, sizeof(float));
				break;
			}
			case EMaterialParamType::Vector2:
			{
				FVector2 Val = std::get<FVector2>(ParamValue.Value);
				std::memcpy(CBufferData.data() + VarInfo.Offset, &Val, sizeof(FVector2));
				break;
			}
			case EMaterialParamType::Vector3:
			{
				FVector Val = std::get<FVector>(ParamValue.Value);
				std::memcpy(CBufferData.data() + VarInfo.Offset, &Val, sizeof(FVector));
				break;
			}
			case EMaterialParamType::Vector4:
			{
				FVector4 Val = std::get<FVector4>(ParamValue.Value);
				std::memcpy(CBufferData.data() + VarInfo.Offset, &Val, sizeof(FVector4));
				break;
			}
			case EMaterialParamType::Matrix4:
			{
				FMatrix Val = std::get<FMatrix>(ParamValue.Value);
				std::memcpy(CBufferData.data() + VarInfo.Offset, &Val, sizeof(FMatrix));
				break;
			}
			default:
				break;
			}
		}
		else
		{
			if (ParamValue.Type == EMaterialParamType::Texture && std::holds_alternative<UTexture*>(ParamValue.Value))
			{
				int32 Slot = Shader->GetTextureBindSlot(Name);
				if (Slot >= 0)
				{
					UTexture* TextureAsset = std::get<UTexture*>(ParamValue.Value);
					if (TextureAsset)
					{
						ID3D11ShaderResourceView* SRV = TextureAsset->GetSRV();
						Context->PSSetShaderResources(Slot, 1, &SRV);
					}
				}
			}
		}
	}

	Shader->UpdateAndBindCBuffer(Context, CBufferData.data(), 2, static_cast<uint32>(CBufferData.size()));
}

void UMaterialInstance::Bind(ID3D11DeviceContext* Context) const
{
	if (!Parent) return;

	Parent->Bind(Context);

	TMap<FString, FMaterialParamValue> CombinedParams;
	Parent->GatherAllParams(CombinedParams);
	for (const auto& [Name, Value] : OverridedParams)
	{
		CombinedParams[Name] = Value;
	}

	Parent->ApplyParams(Context, CombinedParams);
}
