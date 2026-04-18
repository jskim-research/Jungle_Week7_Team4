#pragma once
#include "LightComponent.h"

class UAmbientLightComponent : public ULightComponent
{
public:
    DECLARE_CLASS(UAmbientLightComponent, ULightComponent)

    UAmbientLightComponent();
    ~UAmbientLightComponent() override = default;

    void GetEditableProperties(TArray<FPropertyDescriptor>& OutProps) override;
    void Serialize(FArchive& Ar) override;
    void PostDuplicate(UObject* Original) override;
};
