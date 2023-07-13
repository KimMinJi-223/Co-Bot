// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "passplz.h"
#include "CPP_Stage3Cobot.h"
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

	void ChangAim(bool P1, bool P2);

	void CreatePlayer(ACPP_Stage3Cobot* P1, ACPP_Stage3Cobot* P2);

	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = Camera)
		UCameraComponent* Camera;

		ACPP_Stage3Cobot* Player1;
		ACPP_Stage3Cobot* Player2;

	UPROPERTY(BlueprintReadWrite)
		bool isHaveCapsule;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UStaticMeshComponent* lavaCapsule;

private:


	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Player1CarFoot;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Player2CarFoot;

	int mode;
};
