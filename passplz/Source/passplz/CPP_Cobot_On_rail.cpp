// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Cobot_On_rail.h"

// Sets default values
ACPP_Cobot_On_rail::ACPP_Cobot_On_rail()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Cobot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cobot"));
	rail = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("rail"));
	collision = CreateDefaultSubobject<UBoxComponent>(TEXT("collision"));

	RootComponent = collision;
	Cobot->SetupAttachment(RootComponent);
	rail->SetupAttachment(RootComponent);


	Cobot->SetRelativeLocation(FVector(0.f, 0.f, 13.f));
	rail->SetRelativeScale3D(FVector(1.7f, 1.2f, 0.3f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_COBOT(TEXT("/Game/model/STAGE_5/cobot_move2.cobot_move2"));
	if (SM_COBOT.Succeeded()) {
		Cobot->SetStaticMesh(SM_COBOT.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_CUBE(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (SM_CUBE.Succeeded()) {
		rail->SetStaticMesh(SM_CUBE.Object);
	}

}

// Called when the game starts or when spawned
void ACPP_Cobot_On_rail::BeginPlay()
{
	Super::BeginPlay();
	Cobot->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called every frame
void ACPP_Cobot_On_rail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

