// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Cobot_Car.h"

// Sets default values
ACPP_Cobot_Car::ACPP_Cobot_Car()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	lavaCapsule = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("lavaCapsule"));
	//Player1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Player1"));
	//Player2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Player2"));
	Player1CarFoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Player1CarFoot"));
	Player2CarFoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Player2CarFoot"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	lavaCapsule->SetupAttachment(GetCapsuleComponent());
	//Player1->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//Player2->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//Player1->SetupAttachment(GetCapsuleComponent());
	//Player2->SetupAttachment(GetCapsuleComponent());
	Player1CarFoot->SetupAttachment(GetCapsuleComponent());
	Player2CarFoot->SetupAttachment(GetCapsuleComponent());

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -80.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 1000.0f;
	lavaCapsule->SetRelativeLocation(FVector(27.f, 0.0f, 90.f));
	lavaCapsule->SetRelativeRotation(FRotator(75.f, 0.0f, 0.0f));

	//Player1->SetRelativeLocation(FVector(90.f, 165.f, 20.0f));
	//Player2->SetRelativeLocation(FVector(90.f, -165.f, 20.0f));
	Player1CarFoot->SetRelativeLocation(FVector(90.f, 165.f, -33.f));
	Player2CarFoot->SetRelativeLocation(FVector(90.f, -165.f, -33.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CAR(TEXT("/Game/K_Test/input/stage4_car.stage4_car"));
	if (SK_CAR.Succeeded()) {
		UE_LOG(LogTemp, Warning, TEXT("mesh ok"));

		GetMesh()->SetSkeletalMesh(SK_CAR.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_CARFOOT(TEXT("/Game/K_Test/input/CarFoot.CarFoot"));
	if (SM_CARFOOT.Succeeded()) {
		UE_LOG(LogTemp, Warning, TEXT("mesh ok"));

		Player1CarFoot->SetStaticMesh(SM_CARFOOT.Object);
		Player2CarFoot->SetStaticMesh(SM_CARFOOT.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_LAVACAPSULE(TEXT("/Game/K_Test/input/lavaCapsule.lavaCapsule"));
	if (SM_LAVACAPSULE.Succeeded()) {
		lavaCapsule->SetStaticMesh(SM_LAVACAPSULE.Object);
	}

	mode = 0;
}

// Called when the game starts or when spawned
void ACPP_Cobot_Car::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("SpawnActor"));


	isHaveCapsule = false;
	lavaCapsule->SetVisibility(false);

	/*Player1->SetChildActorClass(ACPP_Stage3Cobot::StaticClass());
	Player2->SetChildActorClass(ACPP_Stage3Cobot::StaticClass());
	Player1->RegisterComponent();
	Player2->RegisterComponent();
	Player1->CreateChildActor();
	Player2->CreateChildActor();
	lavaCapsule->GetStaticMesh();*/
}

// Called every frame
void ACPP_Cobot_Car::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("%p"), Player1);

	FVector v = GetMesh()->GetSocketLocation("Player1");
	UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), v.X, v.Y, v.Z);

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto P1 = GetWorld()->SpawnActor<ACPP_Stage3Cobot>(ACPP_Stage3Cobot::StaticClass(), FVector(0.0f, 0.0f, 1000.f), FRotator::ZeroRotator, SpawnInfo);
	auto P2 = GetWorld()->SpawnActor<ACPP_Stage3Cobot>(ACPP_Stage3Cobot::StaticClass(), FVector(0.0f, 0.0f, 1000.f), FRotator::ZeroRotator, SpawnInfo);

	CreatePlayer(P1, P2);

	SetActorTickEnabled(false);

}

// Called to bind functionality to input
void ACPP_Cobot_Car::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACPP_Cobot_Car::ChangAim(bool P1, bool P2)
{
	UE_LOG(LogTemp, Warning, TEXT("ChangAim"));

	Player1->isWalk = P1;
	Player2->isWalk = P2;
}

void ACPP_Cobot_Car::CreatePlayer(ACPP_Stage3Cobot* P1, ACPP_Stage3Cobot* P2)
{
	FName P1Socket(TEXT("Player1"));
	FName P2Socket(TEXT("Player2"));

	FVector v = GetMesh()->GetSocketLocation("Player1");
	UE_LOG(LogTemp, Warning, TEXT("%f %f %f"), GetMesh()->GetSocketLocation(TEXT("Player1")).X, GetMesh()->GetSocketLocation(P1Socket).Y, GetMesh()->GetSocketLocation(TEXT("Player1")).Z);



	if (nullptr != P1)
	{
		P1->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, P1Socket);
		P2->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, P2Socket);
		P1->SetOwner(this);
		P2->SetOwner(this);
		Player1 = P1;
		Player2 = P2;
		UE_LOG(LogTemp, Warning, TEXT("%p"), P1);
		UE_LOG(LogTemp, Warning, TEXT("%p"), Player1);


	}
}

