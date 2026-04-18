#include "AmbientLightComponent.h"
#include "Object/ObjectFactory.h"

DEFINE_CLASS(UAmbientLightComponent, ULightComponent)
REGISTER_FACTORY(UAmbientLightComponent)

UAmbientLightComponent::UAmbientLightComponent()
{
    SetLightType(ELightType::LightType_AmbientLight);
}

void UAmbientLightComponent::GetEditableProperties(TArray<FPropertyDescriptor>& OutProps)
{
    ULightComponent::GetEditableProperties(OutProps);
}

void UAmbientLightComponent::Serialize(FArchive& Ar)
{
    ULightComponent::Serialize(Ar);
}

void UAmbientLightComponent::PostDuplicate(UObject* Original)
{
    ULightComponent::PostDuplicate(Original);

    // const UAmbientLightComponent* Orig = Cast<UAmbientLightComponent>(Original);
}
