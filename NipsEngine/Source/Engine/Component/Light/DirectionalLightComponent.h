#pragma once
#include "LightComponent.h"

class UDirectionalLightComponent : public ULightComponent
{
public:
    DECLARE_CLASS(UDirectionalLightComponent, ULightComponent)

    UDirectionalLightComponent();
    ~UDirectionalLightComponent() override = default;

    void GetEditableProperties(TArray<FPropertyDescriptor>& OutProps) override;
    void Serialize(FArchive& Ar) override;
    void PostDuplicate(UObject* Original) override;
};
