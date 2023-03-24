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
	SpringArm->SetRelativeRotation(FRotator(-25.0f, 0.0f, 0.0f));
	GetCapsuleComponent()->SetCapsuleSize(30.f, 40.f);
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_COBOT(TEXT("/Game/model/PLAYER/cobot_test02.cobot_test02"));
	if (SK_COBOT.Succeeded()) {
		UE_LOG(LogTemp, Warning, TEXT("mesh ok"));

		GetMesh()->SetSkeletalMesh(SK_COBOT.Object);
	}
	
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> COBOT_ANIM(TEXT("/Game/Global_BP/Cobot_anim.Cobot_anim_C"));
	if (COBOT_ANIM.Succeeded()) {
		UE_LOG(LogTemp, Warning, TEXT("Animation OK"));
		AnimInstanceClass = COBOT_ANIM.Class;
		//GetMesh()->SetAnimInstanceClass(COBOT_ANIM.Class);
	}
	else{
		UE_LOG(LogTemp, Warning, TEXT("Animation NO"));
	}

	GetCharacterMovement()->MaxWalkSpeed = 100.f;

}

// Called when the game starts or when spawned
void ACPP_Cobot::BeginPlay()
{
	Super::BeginPlay();
	Current_left = GetMesh()->GetSocketLocation("left");
	Start_left = GetMesh()->GetSocketLocation("left");
	Target_left = GetMesh()->GetSocketLocation("left");
	Current_right = GetMesh()->GetSocketLocation("right");
	Start_right = GetMesh()->GetSocketLocation("right");
	Target_right = GetMesh()->GetSocketLocation("right");
	GetMesh()->SetAnimInstanceClass(AnimInstanceClass);
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

