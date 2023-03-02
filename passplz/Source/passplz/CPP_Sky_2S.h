// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "passplz.h"
#include "GameFramework/Actor.h"
#include "CPP_Sky_2S.generated.h"

UCLASS()
class PASSPLZ_API ACPP_Sky_2S : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Sky_2S();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;	

private:
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* CenterSky;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* FrontSky;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* BackSky;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Button;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* Trigger_Button;

	UPROPERTY(VisibleAnywhere)
		int32 move_value;

	UFUNCTION()
		void OnButtonOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
