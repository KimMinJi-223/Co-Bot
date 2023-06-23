// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Cobot_Car.h"

// Sets default values
ACPP_Cobot_Car::ACPP_Cobot_Car()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	Decal = CreateDefaultSubobject<UDecalComponent>("Decal");

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	Sphere->SetupAttachment(GetCapsuleComponent());
	Decal->SetupAttachment(Sphere);


	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -80.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 1000.0f;

	Decal->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_COBOT(TEXT("/Game/K_Test/input/stage4_car_run.stage4_car_run"));
	if (SK_COBOT.Succeeded()) {
		UE_LOG(LogTemp, Warning, TEXT("mesh ok"));

		GetMesh()->SetSkeletalMesh(SK_COBOT.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SPHERE(TEXT("/Game/K_Test/input/Sphere.Sphere"));
	if (SM_SPHERE.Succeeded()) {
		Sphere->SetStaticMesh(SM_SPHERE.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("/Game/K_Test/lava/circle.circle"));
	if (DecalMaterialAsset.Succeeded())
	{
		Decal->SetDecalMaterial(DecalMaterialAsset.Object);
	}
}

// Called when the game starts or when spawned
void ACPP_Cobot_Car::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_Cobot_Car::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACPP_Cobot_Car::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

