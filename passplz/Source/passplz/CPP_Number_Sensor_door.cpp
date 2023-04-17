// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Number_Sensor_door.h"

// Sets default values
ACPP_Number_Sensor_door::ACPP_Number_Sensor_door()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; //매프레임마다 Tick이 실행되냐

	Foothold = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Foothold"));
	Doorframe = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Doorframe"));
	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Trigger_Foothold = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger_Foothold"));
	
	RootComponent = Trigger_Foothold;

	Foothold->SetupAttachment(RootComponent);
	Doorframe->SetupAttachment(RootComponent);
	Door->SetupAttachment(Doorframe);

	Foothold->SetRelativeScale3D(FVector(3.f, 3.f, 1.f));
	Doorframe->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	Doorframe->SetRelativeLocation(FVector(180.f, 0.f, 50.f));
	Trigger_Foothold->SetBoxExtent(FVector(128.f, 128.f, 128.f));
	Trigger_Foothold->SetRelativeLocation(FVector(0.f, 0.f, 40.f)); //루트 컴포넌트에서 얼마큼 거리 있는지 정하는 함수

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_FOOTHOLD(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (SM_FOOTHOLD.Succeeded()) {
		Foothold->SetStaticMesh(SM_FOOTHOLD.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_DOORFRAME(TEXT("/Game/model/STAGE_2/doorframe.doorframe"));
	if (SM_DOORFRAME.Succeeded()) {
		Doorframe->SetStaticMesh(SM_DOORFRAME.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_DOOR(TEXT("/Game/model/STAGE_2/door.door"));
	if (SM_DOOR.Succeeded()) {
		Door->SetStaticMesh(SM_DOOR.Object);
	}

	num_currnet_foot = 0;
	num_Dooropen = 1;
}

// Called when the game starts or when spawned
void ACPP_Number_Sensor_door::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACPP_Number_Sensor_door::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("ACPP_Number_Sensor_door::Tick"));
}

void ACPP_Number_Sensor_door::PostInitializeComponents()
{
	UE_LOG(LogTemp, Warning, TEXT("ACPP_Number_Sensor_door::PostInitializeComponents"));
	Super::PostInitializeComponents();
	Trigger_Foothold->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Number_Sensor_door::OnFootOverlap);
	Trigger_Foothold->OnComponentEndOverlap.AddDynamic(this, &ACPP_Number_Sensor_door::OnFootEndOverlap);
}

void ACPP_Number_Sensor_door::OnFootOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("ACPP_Number_Sensor_door::OnFootOverlap"));
	++num_currnet_foot;
	FLatentActionInfo Info;
	Info.CallbackTarget = this;
	if (num_currnet_foot == num_Dooropen) {
		UKismetSystemLibrary::MoveComponentTo(
			Door,
			FVector(0.f, 0.f, 200.f),
			FRotator(0.f, 0.f, 0.f),
			false,
			false,
			5.f,
			false,
			EMoveComponentAction::Move,
			Info);
		Trigger_Foothold->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ACPP_Number_Sensor_door::OnFootEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	--num_currnet_foot;
}

