// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Secret_Door.h"

// Sets default values
ACPP_Secret_Door::ACPP_Secret_Door()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Foothold = CreateDefaultSubobject<UBoxComponent>(TEXT("Foothold"));
	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));

	RootComponent = Foothold;

	Door->SetupAttachment(RootComponent);

	Door->SetRelativeLocation(FVector(0.f, 590.f, 0.f));

	Door->SetRelativeScale3D(FVector(1.2f, 1.0f, 1.0f));

	Foothold->SetBoxExtent(FVector(128.f, 128.f, 128.f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_DOOR(TEXT("/Game/model/STAGE_2/door.door"));
	if (SM_DOOR.Succeeded()) {
		Door->SetStaticMesh(SM_DOOR.Object);
	}
}

// Called when the game starts or when spawned
void ACPP_Secret_Door::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACPP_Secret_Door::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Foothold->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Secret_Door::OnFootOverlap);
	Foothold->OnComponentEndOverlap.AddDynamic(this, &ACPP_Secret_Door::OnFootEndOverlap);
}

// Called every frame
void ACPP_Secret_Door::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_Secret_Door::OnFootOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("ACPP_Number_Sensor_door::OnFootOverlap"));
	++num_currnet_foot;
	FLatentActionInfo Info;
	Info.CallbackTarget = this;
	if (num_currnet_foot >= 1) {
		UKismetSystemLibrary::MoveComponentTo(
			Door,
			FVector(0.f, 590.f, 200.f),
			FRotator(0.f, 0.f, 0.f),
			false,
			false,
			5.f,
			false,
			EMoveComponentAction::Move,
			Info);
		Foothold->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ACPP_Secret_Door::OnFootEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	--num_currnet_foot;
}

