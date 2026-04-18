#pragma once
#include "Component/SceneComponent.h"
#include "Render/Common/RenderTypes.h"

class ULightComponentBase : public USceneComponent
{
public:
    DECLARE_CLASS(ULightComponentBase, USceneComponent)

    ULightComponentBase();
    ~ULightComponentBase() override = default;

    void GetEditableProperties(TArray<FPropertyDescriptor>& OutProps) override;
    void PostEditProperty(const char* PropertyName) override;

    void PostDuplicate(UObject* Original) override;

    void Serialize(FArchive& Ar) override;

    void BeginPlay() override;
    void EndPlay() override;

public:
    const FColor& GetLightColor() const { return LightColor; }
    float GetIntensity() const { return Intensity; }
    bool IsVisible() const { return bVisible; }

    void SetLightColor(const FColor& InColor) { LightColor = InColor; }
    void SetIntensity(float InIntensity) { Intensity = InIntensity; }
    void SetVisible(bool bInVisible) { bVisible = bInVisible; }

private:
    FColor LightColor = FColor(1.0f, 1.0f, 1.0f, 1.0f);
    float Intensity = 1.0f;
    bool bVisible = true;
};

class ULightComponent : public ULightComponentBase
{
public:
    DECLARE_CLASS(ULightComponent, ULightComponentBase)

    ULightComponent();
    ~ULightComponent() override = default;

    void Serialize(FArchive& Ar) override;
    void PostDuplicate(UObject* Original) override;

public:
    ELightType GetLightType() const { return LightType; }

protected:
    void SetLightType(ELightType InLightType) { LightType = InLightType; }

private:
    ELightType LightType = ELightType::Max;
};
