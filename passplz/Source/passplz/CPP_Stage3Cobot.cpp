// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Stage3Cobot.h"

// Sets default values
ACPP_Stage3Cobot::ACPP_Stage3Cobot()
{
	UE_LOG(LogTemp, Warning, TEXT("ACPP_Stage3Cobot::ACPP_Stage3Cobot"));

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -40.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetCapsuleComponent()->SetCapsuleSize(30.f, 40.f);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_COBOT(TEXT("/Game/K_Test/input/stage3Cobot.stage3Cobot"));
	if (SK_COBOT.Succeeded()) {
		UE_LOG(LogTemp, Warning, TEXT("mesh ok"));

		GetMesh()->SetSkeletalMesh(SK_COBOT.Object);
	}
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> COBOT_ANIM(TEXT("/Game/K_Test/input/stage3CobotAnim.stage3CobotAnim_C"));
	if (COBOT_ANIM.Succeeded()) {
		UE_LOG(LogTemp, Warning, TEXT("Animation OK"));
		AnimInstanceClass = COBOT_ANIM.Class;
		//GetMesh()->SetAnimInstanceClass(COBOT_ANIM.Class);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Animation NO"));
	}

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));

	isWalk = false;
}

// Called when the game starts or when spawned
void ACPP_Stage3Cobot::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("ACPP_Stage3Cobot::BeginPlay()"));

	GetMesh()->SetAnimInstanceClass(AnimInstanceClass);
}

// Called every frame
void ACPP_Stage3Cobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACPP_Stage3Cobot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

