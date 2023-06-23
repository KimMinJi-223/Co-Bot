// Fill out your copyright notice in the Description page of Project Settings.

//기존 컨트롤러에서 했던거 패킷 처리 필요

#include "CPP_Cobot_Car_Controller.h"
#include "CPP_Cobot_Car.h"
//#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

ACPP_Cobot_Car_Controller::ACPP_Cobot_Car_Controller()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>DEFAULT_CONTEXT
	(TEXT("/Game/K_Test/input/stage3IMCTest.stage3IMCTest"));
	if (DEFAULT_CONTEXT.Succeeded())
		DefaultContext = DEFAULT_CONTEXT.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_ROTATE
	(TEXT("/Game/K_Test/input/Move.Move"));
	if (IA_ROTATE.Succeeded())
		Move1 = IA_ROTATE.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_ROTATE2
	(TEXT("/Game/K_Test/input/Move_2.Move_2"));
	if (IA_ROTATE2.Succeeded())
		Move2 = IA_ROTATE2.Object;
}

ACPP_Cobot_Car_Controller::~ACPP_Cobot_Car_Controller()
{
}

void ACPP_Cobot_Car_Controller::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		SubSystem->AddMappingContext(DefaultContext, 0);

	player = Cast<ACPP_Cobot_Car>(GetPawn());
	if (!player)
		return;
}

void ACPP_Cobot_Car_Controller::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ACPP_Cobot_Car_Controller::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	UE_LOG(LogTemp, Warning, TEXT("OnPossess"));
}

void ACPP_Cobot_Car_Controller::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(Move1, ETriggerEvent::Triggered, this, &ACPP_Cobot_Car_Controller::CarInput);
	}
}

void ACPP_Cobot_Car_Controller::CarInput(const FInputActionValue& Value)
{
	//서버 : 여기서 키를 누르거나 떼면 여길 들어오는데 이때 서버에 패킷 보내야함
	//서버에는 각 클라 키에 대한 bool값을 가진다.
	UE_LOG(LogTemp, Warning, TEXT("%f"), Value.Get<float>());
}

void ACPP_Cobot_Car_Controller::CarForward()
{
	UE_LOG(LogTemp, Warning, TEXT("CarForward"));

	FRotator rotator_controller = GetControlRotation();
	FRotator rotator_forward = UKismetMathLibrary::MakeRotator(0.0f, 0.0f, rotator_controller.Yaw);
	FVector forward_vector = UKismetMathLibrary::GetForwardVector(rotator_forward);

	player->AddMovementInput(forward_vector, 1.0);
}

void ACPP_Cobot_Car_Controller::CarRotation(float rotationValue)
{
	FRotator control_rotation =  GetControlRotation();
	control_rotation.Yaw += rotationValue;
	SetControlRotation(control_rotation);
}



