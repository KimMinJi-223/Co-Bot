// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "passplz.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "CPP_Forklift_Button.generated.h"

UCLASS()
class PASSPLZ_API ACPP_Forklift_Button : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Forklift_Button();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
		UArrowComponent* center;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* redCollision;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* red;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* greenCollision;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* green;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* blueCollision;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* blue;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* blackCollision;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* black;

	class ACPP_Color_Forklift* OutActor;

private:
	UFUNCTION()
		void OnComponentBeginOverlap_redCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnComponentBeginOverlap_greenCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnComponentBeginOverlap_blueCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnComponentBeginOverlap_blackCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
