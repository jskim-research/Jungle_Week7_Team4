#include "GameFramework/PrimitiveActors.h"

#include "Component/DecalComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/TextRenderComponent.h"
#include "Component/HeightFogComponent.h"
#include "Component/SkyAtmosphereComponent.h"
#include "Component/Movement/RotatingMovementComponent.h"
#include "Component/Light/DirectionalLightComponent.h"
#include "Component/Light/AmbientLightComponent.h"
#include "Component/Light/PointLightComponent.h"
#include "Component/Light/SpotLightComponent.h"
#include "Component/HeightFogComponent.h"
#include "Component/BillboardComponent.h"
#include "Component/SubUVComponent.h"
#include "Core/ResourceManager.h"
#include <format>

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

DEFINE_CLASS(ASkyAtmosphereActor, AActor)
REGISTER_FACTORY(ASkyAtmosphereActor)

DEFINE_CLASS(AHeightFogActor, AActor)
REGISTER_FACTORY(AHeightFogActor)

void ASceneActor::InitDefaultComponents()
{
	auto SceneRoot = AddComponent<USceneComponent>();
	SetRootComponent(SceneRoot);

	UBillboardComponent* Billboard = AddComponent<UBillboardComponent>();
	Billboard->AttachToComponent(SceneRoot);
	Billboard->SetIsVisualizationComponent(true);
	Billboard->SetTexturePath("Asset/Texture/Icons/EmptyActor.PNG");
}

void AStaticMeshActor::InitDefaultComponents()
{
	auto* StaticMesh = AddComponent<UStaticMeshComponent>();
	SetRootComponent(StaticMesh);
}

void ASubUVActor::InitDefaultComponents()
{
	SetTickInEditor(true); // Editor Tick을 받도록 변경

    auto* SubUV = AddComponent<USubUVComponent>();
    SetRootComponent(SubUV);
	SubUV->SetParticle(FName("Explosion"));
	SubUV->SetSpriteSize(2.0f, 2.0f);
	SubUV->SetFrameRate(30.f);
}

void ATextRenderActor::InitDefaultComponents()
{
	UTextRenderComponent* Text = AddComponent<UTextRenderComponent>();
	SetRootComponent(Text);
	Text->SetFont(FName("Default"));
	Text->SetText("TextRender");
}

void ABillboardActor::InitDefaultComponents()
{
	UBillboardComponent* Billboard = AddComponent<UBillboardComponent>();
	SetRootComponent(Billboard);
	Billboard->SetTexturePath(("Asset/Texture/Pawn_64x.png"));
}

void ADecalActor::InitDefaultComponents()
{
	UDecalComponent* Decal = AddComponent<UDecalComponent>();
	SetRootComponent(Decal);

	UBillboardComponent* Billboard = AddComponent<UBillboardComponent>();
	Billboard->AttachToComponent(Decal);
	Billboard->SetIsVisualizationComponent(true);
	Billboard->SetTexturePath("Asset/Texture/Icons/S_DecalActorIcon.PNG");
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

void ASkyAtmosphereActor::InitDefaultComponents()
{
	USkyAtmosphereComponent* SkyAtmosphere = AddComponent<USkyAtmosphereComponent>();
	SetRootComponent(SkyAtmosphere);
}

void AHeightFogActor::InitDefaultComponents()
{
	UHeightFogComponent* HeightFog = AddComponent<UHeightFogComponent>();
	SetRootComponent(HeightFog);
}
