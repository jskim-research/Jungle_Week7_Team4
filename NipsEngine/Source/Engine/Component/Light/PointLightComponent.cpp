#include "PointLightComponent.h"
#include "Object/ObjectFactory.h"

DEFINE_CLASS(UPointLightComponent, ULightComponent)
REGISTER_FACTORY(UPointLightComponent)

UPointLightComponent::UPointLightComponent()
{
    SetLightType(ELightType::LightType_Point);
}

void UPointLightComponent::GetEditableProperties(TArray<FPropertyDescriptor>& OutProps)
{
    ULightComponent::GetEditableProperties(OutProps);

    OutProps.push_back({ "Attenuation Radius",     EPropertyType::Float, &AttenuationRadius,    0.0f,  10000.0f, 1.0f });
    OutProps.push_back({ "Light Falloff Exponent", EPropertyType::Float, &LightFalloffExponent, 0.01f, 16.0f,    0.01f });
}

void UPointLightComponent::Serialize(FArchive& Ar)
{
    ULightComponent::Serialize(Ar);

    Ar << "AttenuationRadius"    << AttenuationRadius;
    Ar << "LightFalloffExponent" << LightFalloffExponent;
}

void UPointLightComponent::PostDuplicate(UObject* Original)
{
    ULightComponent::PostDuplicate(Original);

    const UPointLightComponent* Orig = Cast<UPointLightComponent>(Original);
    if (!Orig) return;

    AttenuationRadius    = Orig->AttenuationRadius;
    LightFalloffExponent = Orig->LightFalloffExponent;
}
