// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <array>

#include "passplz.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_Elevator.h"
#include "CPP_Tutorial_Light.generated.h"

UCLASS()
class PASSPLZ_API ACPP_Tutorial_Light : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Tutorial_Light();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USpotLightComponent* spotLight;
private:
	UPROPERTY(VisibleAnywhere)
		UBoxComponent* boxCollision;

private:
	UFUNCTION()
		void OnComponentBeginOverlap_boxCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//UFUNCTION()
	//	void OnComponentEndOverlap_boxCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:
	//std::array<FVector, 3> lightLocation;
	TArray<FVector> lightLocation;
	int32 lightIndex;
};