// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "passplz.h"
#include "GameFramework/Character.h"
#include "Components/DecalComponent.h"
#include "CPP_Cobot_Car.generated.h"

UCLASS()
class PASSPLZ_API ACPP_Cobot_Car : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPP_Cobot_Car();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;
private:

	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* lavaCapsule;

	int mode;
};
