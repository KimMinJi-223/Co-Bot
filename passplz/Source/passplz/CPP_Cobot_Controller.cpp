// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Cobot_Controller.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"

void ACPP_Cobot_Controller::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ACPP_Cobot_Controller::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	UE_LOG(LogTemp, Warning, TEXT("OnPossess"));
}

void ACPP_Cobot_Controller::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis(TEXT("MOVE_FORWARD"), this, &ACPP_Cobot_Controller::Move_Forward);
	InputComponent->BindAxis(TEXT("ROTATE"), this, &ACPP_Cobot_Controller::Rotate);
}

void ACPP_Cobot_Controller::Move_Forward(float NewAxisValue)
{
	APawn* player = GetPawn();

	if (!player)
		return;

	FRotator rotator_controller = GetControlRotation();
	FRotator rotator_forward = UKismetMathLibrary::MakeRotator(0.0f, 0.0f, rotator_controller.Yaw);
	FVector forward_vector = UKismetMathLibrary::GetForwardVector(rotator_forward);

	player->AddMovementInput(forward_vector, NewAxisValue);
}

void ACPP_Cobot_Controller::Rotate(float NewAxisValue)
{
	float delta_time = GetWorld()->GetDeltaSeconds();
	AddYawInput(delta_time * NewAxisValue * 20.0f);
}
