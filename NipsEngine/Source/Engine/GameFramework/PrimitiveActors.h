#pragma once

#include "AActor.h"

class UTextRenderComponent;
class UDecalComponent;

class ASceneActor : public AActor
{
public:
	DECLARE_CLASS(ASceneActor, AActor)
	ASceneActor() = default;

	void InitDefaultComponents();
};

class AStaticMeshActor : public AActor
{
public:
	DECLARE_CLASS(AStaticMeshActor, AActor)
	AStaticMeshActor() = default;

	void InitDefaultComponents();
};

class ASubUVActor : public AActor
{
public:
    DECLARE_CLASS(ASubUVActor, AActor)
    ASubUVActor() = default;

    void InitDefaultComponents();
};

class ATextRenderActor : public AActor
{
public:
    DECLARE_CLASS(ATextRenderActor, AActor)
    ATextRenderActor() = default;

    void InitDefaultComponents();
};

class ABillboardActor : public AActor
{
public:
    DECLARE_CLASS(ABillboardActor, AActor)
	ABillboardActor() = default;

    void InitDefaultComponents();
};

class ADecalActor : public AActor
{
public:
	DECLARE_CLASS(ADecalActor, AActor)
	ADecalActor() = default;

	void InitDefaultComponents();
};

class ADirectionalLightActor : public AActor {
public:
	DECLARE_CLASS(ADirectionalLightActor, AActor)
	ADirectionalLightActor() = default;

	void InitDefaultComponents();
};

class AAmbientLightActor : public AActor {
public:
	DECLARE_CLASS(AAmbientLightActor, AActor)
	AAmbientLightActor() = default;

	void InitDefaultComponents();
};

class APointLightActor : public AActor {
public:
	DECLARE_CLASS(APointLightActor, AActor)
	APointLightActor() = default;

	void InitDefaultComponents();
};

class ASpotLightActor : public AActor {
public:
	DECLARE_CLASS(ASpotLightActor, AActor)
	ASpotLightActor() = default;

	void InitDefaultComponents();
};
