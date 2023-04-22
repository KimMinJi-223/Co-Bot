// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "passplz.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "CPP_Time_Color_Button.generated.h"

UCLASS()
class PASSPLZ_API ACPP_Time_Color_Button : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Time_Color_Button();

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
		UBoxComponent* timeColorFootholdCollision;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* timeColorFoothold;
};
