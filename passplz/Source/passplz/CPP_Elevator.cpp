// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Elevator.h"

// Sets default values
ACPP_Elevator::ACPP_Elevator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	doorLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("doorLeft"));
	doorRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("doorRight"));
	doorFloor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("doorFloor"));
	boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("boxCollision"));
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("camera"));
	doorFloorForSeekButton = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("doorFloorForSeekButton"));

	RootComponent = boxCollision;

	doorFloor->SetupAttachment(RootComponent);
	doorLeft->SetupAttachment(RootComponent);
	doorRight->SetupAttachment(RootComponent);
	camera->SetupAttachment(RootComponent);
	doorFloorForSeekButton->SetupAttachment(doorFloor);

	boxCollision->SetBoxExtent(FVector(71.069857f, 84.826179f, 779.27683f));
	doorLeft->SetRelativeLocation(FVector(0.f, 130.f, 0.f));
	doorRight->SetRelativeLocation(FVector(0.f, 130.f, 0.f));
	doorRight->SetRelativeRotation(FRotator(0.f, -180.f, 0.f));
	camera->SetRelativeLocation(FVector(0.f, 680.f, 338.32f));
	camera->SetRelativeRotation(FRotator(-14.45f, -90.f, 0.f));
	doorFloorForSeekButton->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	doorFloorForSeekButton->SetRelativeRotation(FRotator(0.f, 180.f, 180.f));
	doorFloorForSeekButton->SetRelativeScale3D(FVector(1.f, 1.f, 1.241087f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_ELEVATOR_DOOR(TEXT("/Game/model/EV/ev_door.ev_door"));
	if (SM_ELEVATOR_DOOR.Succeeded()) {
		doorLeft->SetStaticMesh(SM_ELEVATOR_DOOR.Object);
		doorRight->SetStaticMesh(SM_ELEVATOR_DOOR.Object);
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_ELEVATOR_FLOOR(TEXT("/Game/model/EV/ev_floor.ev_floor"));
	if (SM_ELEVATOR_FLOOR.Succeeded()) {
		doorFloor->SetStaticMesh(SM_ELEVATOR_FLOOR.Object);
		doorFloorForSeekButton->SetStaticMesh(SM_ELEVATOR_FLOOR.Object);
	}

	checkNumOfCobot = 0;
	checkMyStage = 1;
}

// Called when the game starts or when spawned
void ACPP_Elevator::BeginPlay()
{
	Super::BeginPlay();

}

void ACPP_Elevator::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	boxCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Elevator::OnComponentBeginOverlap_boxCollision);
	boxCollision->OnComponentEndOverlap.AddDynamic(this, &ACPP_Elevator::OnComponentEndOverlap_boxCollision);
}

// Called every frame
void ACPP_Elevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_Elevator::OnComponentBeginOverlap_boxCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	checkNumOfCobot += 1;

	UE_LOG(LogTemp, Warning, TEXT("hi~"));
	// 서버로 엘베 들어온거 보내기 (패킷)
	//if (checkNumOfCobot == 2)
	ElevatorOperateCameraMoveLevelChange();
}

void ACPP_Elevator::OnComponentEndOverlap_boxCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	checkNumOfCobot -= 1;

	//서버로 엘베 나간거 보내기 (패킷)
}

void ACPP_Elevator::ElevatorOperateCameraMoveLevelChange()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->SetViewTargetWithBlend(camera->GetOwner(), 1.5, VTBlend_Linear);	// 카메라 바꿔주고

	boxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//waitTimer = new FTimerHandle;
	//elevatorTimer = new FTimerHandle;

	GetWorld()->GetTimerManager().SetTimer(waitTimer, FTimerDelegate::CreateLambda([&]()
	{
		UE_LOG(LogTemp, Warning, TEXT("waitTimer"));
		GetWorldTimerManager().ClearTimer(waitTimer);
		//delete waitTimer;

		choose = 0;
		GetWorldTimerManager().SetTimer(elevatorTimer, this, &ACPP_Elevator::DoorCloseFloorUpTimer, 0.05f, true);
	}), 1.f, false);
}

void ACPP_Elevator::DoorCloseFloorUpTimer()
{
	UE_LOG(LogTemp, Warning, TEXT("timer2"));

	switch (choose) {
	case 0 :
		doorLeft->AddLocalOffset(FVector(-2.f, 0, 0));
		doorRight->AddLocalOffset(FVector(-2.f, 0, 0));

		if (-120.f >= doorLeft->GetRelativeLocation().X)
			choose = 1;
		break;
	case 1 :
		doorFloor->AddLocalOffset({ 0, 0, 4.f });

		if (500.f <= doorFloor->GetRelativeLocation().Z)
			choose = 2;
		break;
	case 2 :
		UE_LOG(LogTemp, Warning, TEXT("FIN"));
		GetWorldTimerManager().ClearTimer(elevatorTimer);
		break;
	default :
		UE_LOG(LogTemp, Warning, TEXT("default"));
		break;
	}
}