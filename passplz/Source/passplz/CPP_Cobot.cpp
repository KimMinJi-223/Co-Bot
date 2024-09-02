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
	Foot_left_Zone = CreateDefaultSubobject<UBoxComponent>(TEXT("Foot_left_Zone"));
	Foot_right_Zone = CreateDefaultSubobject<UBoxComponent>(TEXT("Foot_right_Zone"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	Foot_left_Zone->SetBoxExtent(FVector(8.f, 8.f, 4.f));
	Foot_right_Zone->SetBoxExtent(FVector(8.f, 8.f, 4.f));

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
	else {
		UE_LOG(LogTemp, Warning, TEXT("Animation NO"));
	}
	
	static ConstructorHelpers::FObjectFinder<USoundBase> colorChangeSoundAsset(TEXT("/Game/game_sound/stage_2/button_colorChange_click_Cue.button_colorChange_click_Cue"));
	if (colorChangeSoundAsset.Succeeded())
	{
		colorChangeSound = colorChangeSoundAsset.Object;
	}
	
	GetCharacterMovement()->MaxWalkSpeed = 100.f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->MaxAcceleration = FLT_MAX;
	GetCharacterMovement()->BrakingDecelerationWalking = 0.0f;

	bUseControllerRotationYaw = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = true;
	IsFast_run = false;

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

	Foot_left_Zone->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "left");
	Foot_right_Zone->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "right");

	GetMesh()->SetAnimInstanceClass(AnimInstanceClass);

	color = FVector(0.0f, 1.0f, 0.0f);
	GetMesh()->SetVectorParameterValueOnMaterials(TEXT("cobot_color"), color);
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

void ACPP_Cobot::AnimatiomChange(int mode)
{
	if (mode == 0) {
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;

		bUseControllerRotationYaw = false;
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bInheritRoll = true;

		Current_left = GetMesh()->GetSocketLocation("left");
		Start_left = Current_left;
		Target_left = Current_left;

		Current_right = GetMesh()->GetSocketLocation("right");
		Start_right = Current_right;
		Target_right = Current_right;


	}
	else if (mode == 1) {
		//GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		
		bUseControllerRotationYaw = true;
		//SpringArm->bUsePawnControlRotation = true;
		//SpringArm->bInheritPitch = true;
		//SpringArm->bInheritYaw = true;
		//SpringArm->bInheritRoll = true;
	}
}

void ACPP_Cobot::SetCollision()
{
	Foot_left_Zone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Foot_right_Zone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACPP_Cobot::RecvColor(int newcolor)
{
	UGameplayStatics::PlaySoundAtLocation(this, colorChangeSound, GetActorLocation(), GetActorRotation());

	switch (newcolor) {
	case 0: //����
		UE_LOG(LogTemp, Warning, TEXT("Cobot::Red"));
		color.X = 1.0;
		break;
	case 1: //�׸�
		UE_LOG(LogTemp, Warning, TEXT("Cobot::Green"));
		color.Y = 1.0;
		break;
	case 2: //���
		UE_LOG(LogTemp, Warning, TEXT("Cobot::Blue"));
		color.Z = 1.0;
		break;
	case 3: //��
		UE_LOG(LogTemp, Warning, TEXT("Cobot::Black"));
		color = FVector(0.0f, 0.0f, 0.0f);
		break;
	}

	GetMesh()->SetVectorParameterValueOnMaterials(TEXT("cobot_color"), color);
}

