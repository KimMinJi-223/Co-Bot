// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Cobot.h"

// Sets default values
ACPP_Cobot::ACPP_Cobot()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	UE_LOG(LogTemp, Warning, TEXT("ACPP_Cobot"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -40.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
	GetCapsuleComponent()->SetCapsuleSize(30.f, 40.f);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_COBOT(TEXT("/Game/model/PLAYER/cobot.cobot"));
	if (SK_COBOT.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_COBOT.Object);
	}

}

// Called when the game starts or when spawned
void ACPP_Cobot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_Cobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_Cobot::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ACPP_Cobot::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	UE_LOG(LogTemp, Warning, TEXT("PossessedBy"));
}

// Called to bind functionality to input
void ACPP_Cobot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

