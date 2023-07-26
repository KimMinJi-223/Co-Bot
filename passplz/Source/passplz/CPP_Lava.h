// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "passplz.h"
#include "GameFramework/Actor.h"
#include "Components/DecalComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "CPP_Lava.generated.h"

UCLASS()
class PASSPLZ_API ACPP_Lava : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Lava();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void MeltLavaFloor();
	UFUNCTION(BlueprintCallable)
		void MeltLavaFloorLocation();

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(BlueprintReadWrite)
		bool isExplosion;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UTextureRenderTarget2D* RenderTargetTexture;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USphereComponent* CollisionComponent;

private:
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* lavaBall;

	UPROPERTY(VisibleAnywhere)
	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(VisibleAnywhere)
		UMaterialInstanceDynamic* explosionDecalDynamicMaterial;
	UPROPERTY(VisibleAnywhere)
		UDecalComponent* explosionDecal;
	UPROPERTY(VisibleAnywhere)
		UNiagaraComponent* fireNiagara;

	float opacityValue;
	FVector crashLocation;
	UMaterialInterface* brushMaterial;
	UNiagaraSystem* lavaNiagaraSystemAsset;
};