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
};
