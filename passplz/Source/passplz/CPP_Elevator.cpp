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

	RootComponent = doorFloor;

	doorLeft->SetupAttachment(RootComponent);
	doorRight->SetupAttachment(RootComponent);
	boxCollision->SetupAttachment(RootComponent);
	camera->SetupAttachment(RootComponent);
	doorFloorForSeekButton->SetupAttachment(RootComponent);

	doorLeft->SetRelativeLocation(FVector(0.f, 130.f, 0.f));
	doorRight->SetRelativeLocation(FVector(0.f, 130, 0.f));
	doorRight->SetRelativeRotation(FRotator(0.f, -180.f, 0.f));
	boxCollision->SetBoxExtent(FVector(71.069857, 84.826179, 779.276830));
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
	elevatorDoorLeftPos = { 130.f, 0.f, 0.f };
	elevatorDoorRightPos = { 130.f, 0.f, 0.f };
	elevatorFloorPos = { 0.f, 0.f, 0.f };
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
	boxCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Elevator::OnComponentBeginOverlap_box_collision);
	boxCollision->OnComponentEndOverlap.AddDynamic(this, &ACPP_Elevator::OnComponentEndOverlap_box_collision);
}

// Called every frame
void ACPP_Elevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_Elevator::OnComponentBeginOverlap_box_collision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	checkNumOfCobot += 1;

	UE_LOG(LogTemp, Warning, TEXT("hi~"));
	// 서버로 엘베 들어온거 보내기 (패킷)
}

void ACPP_Elevator::OnComponentEndOverlap_box_collision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	checkNumOfCobot -= 1;

	//서버로 엘베 나간거 보내기 (패킷)
}


