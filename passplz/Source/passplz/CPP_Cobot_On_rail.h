// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "passplz.h"
#include "GameFramework/Actor.h"
#include "CPP_Cobot_On_rail.generated.h"

UCLASS()
class PASSPLZ_API ACPP_Cobot_On_rail : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Cobot_On_rail();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Cobot;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* rail;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* collision;
};
