#include "GameFramework/PrimitiveActors.h"

#include "Component/DecalComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/TextRenderComponent.h"
#include "Component/HeightFogComponent.h"
#include "Component/Movement/RotatingMovementComponent.h"
#include "Component/Light/DirectionalLightComponent.h"
#include "Component/Light/AmbientLightComponent.h"
#include "Component/Light/PointLightComponent.h"
#include "Component/Light/SpotLightComponent.h"
#include "Core/ResourceManager.h"
#include <format>
#include <Component/SubUVComponent.h>

DEFINE_CLASS(ASceneActor, AActor)
REGISTER_FACTORY(ASceneActor)

DEFINE_CLASS(AStaticMeshActor, AActor)
REGISTER_FACTORY(AStaticMeshActor)

DEFINE_CLASS(ASubUVActor, AActor)
REGISTER_FACTORY(ASubUVActor)

DEFINE_CLASS(ATextRenderActor, AActor)
REGISTER_FACTORY(ATextRenderActor)

DEFINE_CLASS(ABillboardActor, AActor)
REGISTER_FACTORY(ABillboardActor)

DEFINE_CLASS(ADecalActor, AActor)
REGISTER_FACTORY(ADecalActor)

DEFINE_CLASS(ADirectionalLightActor, AActor)
REGISTER_FACTORY(ADirectionalLightActor)

DEFINE_CLASS(AAmbientLightActor, AActor)
REGISTER_FACTORY(AAmbientLightActor)

DEFINE_CLASS(APointLightActor, AActor)
REGISTER_FACTORY(APointLightActor)

DEFINE_CLASS(ASpotLightActor, AActor)
REGISTER_FACTORY(ASpotLightActor)

void ASceneActor::InitDefaultComponents()
{
	auto SceneRoot = AddComponent<USceneComponent>();
	SetRootComponent(SceneRoot);
}

void AStaticMeshActor::InitDefaultComponents()
{
	auto* StaticMesh = AddComponent<UStaticMeshComponent>();;
	SetRootComponent(StaticMesh);

	// Text attached directly to Root
	auto* Text = AddComponent<UTextRenderComponent>();
	Text->AttachToComponent(StaticMesh);
	Text->SetFont(FName("Default"));
	Text->SetText("UUID: " + std::to_string(GetUUID()));
	Text->SetTransient(true);
	Text->SetEditorOnly(true);

	FVector Extent = StaticMesh->GetWorldAABB().GetExtent();
	Text->SetRelativeLocation(FVector(0.0f, 0.0f, Extent.Z * 2.0f));
}

void ASubUVActor::InitDefaultComponents()
{
	SetTickInEditor(true); // Editor Tick을 받도록 변경

    auto* SubUV = AddComponent<USubUVComponent>();
    SetRootComponent(SubUV);
	SubUV->SetParticle(FName("Explosion"));
	SubUV->SetSpriteSize(2.0f, 2.0f);
	SubUV->SetFrameRate(30.f);

    auto* Text = AddComponent<UTextRenderComponent>();
    Text->AttachToComponent(SubUV);
    Text->SetFont(FName("Default"));
    Text->SetText("UUID: " + std::to_string(GetUUID()));
	Text->SetTransient(true);
	Text->SetEditorOnly(true);

    FVector Extent = SubUV->GetWorldAABB().GetExtent();
    Text->SetRelativeLocation(FVector(0.0f, 0.0f, Extent.Y * 1.4f));
}

void ATextRenderActor::InitDefaultComponents()
{
	UTextRenderComponent* Text = AddComponent<UTextRenderComponent>();
	SetRootComponent(Text);
	Text->SetFont(FName("Default"));
	Text->SetText("TextRender");

    auto* TextUUID = AddComponent<UTextRenderComponent>();
    TextUUID->AttachToComponent(Text);
    TextUUID->SetFont(FName("Default"));
    TextUUID->SetText("UUID: " + std::to_string(GetUUID()));
	TextUUID->SetTransient(true);
	TextUUID->SetEditorOnly(true);

    FVector Extent = TextUUID->GetWorldAABB().GetExtent();
    TextUUID->SetRelativeLocation(FVector(0.0f, 0.0f, Extent.Y * 0.6f));
}

void ABillboardActor::InitDefaultComponents()
{
	UBillboardComponent* Billboard = AddComponent<UBillboardComponent>();
	SetRootComponent(Billboard);
	Billboard->SetTextureName(("Asset/Texture/Pawn_64x.png"));
	//Billboard->SetTextureName();

    auto* TextUUID = AddComponent<UTextRenderComponent>();
    TextUUID->AttachToComponent(Billboard);
    TextUUID->SetFont(FName("Default"));
    TextUUID->SetText("UUID: " + std::to_string(GetUUID()));
	TextUUID->SetTransient(true);
	TextUUID->SetEditorOnly(true);

    FVector Extent = TextUUID->GetWorldAABB().GetExtent();
    TextUUID->SetRelativeLocation(FVector(0.0f, 0.0f, Extent.Y * 0.6f));
}

void ADecalActor::InitDefaultComponents()
{
	UDecalComponent* Decal = AddComponent<UDecalComponent>();
	SetRootComponent(Decal);

	UBillboardComponent* Billboard = AddComponent<UBillboardComponent>();
	Billboard->AttachToComponent(Decal);
	Billboard->SetEditorOnly(true);
	Billboard->SetTextureName(("Asset/Texture/DecalActor_64.png"));

	auto* TextUUID = AddComponent<UTextRenderComponent>();
	TextUUID->AttachToComponent(Decal);
	TextUUID->SetFont(FName("Default"));
	TextUUID->SetText("UUID: " + std::to_string(GetUUID()));
	TextUUID->SetTransient(true);
	TextUUID->SetEditorOnly(true);
	FVector Extent = TextUUID->GetWorldAABB().GetExtent();
	TextUUID->SetRelativeLocation(FVector(0.0f, 0.0f, Extent.Y * 0.6f));
}

void ADirectionalLightActor::InitDefaultComponents()
{
	UDirectionalLightComponent* DirLight = AddComponent<UDirectionalLightComponent>();
	SetRootComponent(DirLight);
}

void AAmbientLightActor::InitDefaultComponents()
{
	UAmbientLightComponent* AmbientLight = AddComponent<UAmbientLightComponent>();
	SetRootComponent(AmbientLight);
}

void APointLightActor::InitDefaultComponents()
{
	UPointLightComponent* PointLight = AddComponent<UPointLightComponent>();
	SetRootComponent(PointLight);
}

void ASpotLightActor::InitDefaultComponents()
{
	USpotLightComponent* SpotLight = AddComponent<USpotLightComponent>();
	SetRootComponent(SpotLight);
}