#include "DirectionalLightComponent.h"
#include "Object/ObjectFactory.h"

DEFINE_CLASS(UDirectionalLightComponent, ULightComponent)
REGISTER_FACTORY(UDirectionalLightComponent)

UDirectionalLightComponent::UDirectionalLightComponent()
{
    SetLightType(ELightType::LightType_Directional);
}

void UDirectionalLightComponent::GetEditableProperties(TArray<FPropertyDescriptor>& OutProps)
{
    ULightComponent::GetEditableProperties(OutProps);
}

void UDirectionalLightComponent::Serialize(FArchive& Ar)
{
    ULightComponent::Serialize(Ar);
}

void UDirectionalLightComponent::PostDuplicate(UObject* Original)
{
    ULightComponent::PostDuplicate(Original);

    // const UDirectionalLightComponent* Orig = Cast<UDirectionalLightComponent>(Original);
}
