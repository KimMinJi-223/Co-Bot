// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPP_Cobot_Controller.generated.h"

/**
 * 
 */
UCLASS()
class PASSPLZ_API ACPP_Cobot_Controller : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;
protected:
	virtual void SetupInputComponent() override;

private:
	void Move_Forward(float NewAxisValue);
	void Rotate(float NewAxisValue);
	
};
