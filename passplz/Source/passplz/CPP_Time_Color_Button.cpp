// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Time_Color_Button.h"

// Sets default values
ACPP_Time_Color_Button::ACPP_Time_Color_Button()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	center = CreateDefaultSubobject<UArrowComponent>(TEXT("Center"));
	timeColorFoothold = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("timeColorFoothold"));
	timeColorFootholdCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("timeColorFootholdCollision"));

	RootComponent = center;

	timeColorFoothold->SetupAttachment(RootComponent);
	timeColorFootholdCollision->SetupAttachment(timeColorFoothold);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_FOOTHOLD(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (SM_FOOTHOLD.Succeeded()) {
		timeColorFoothold->SetStaticMesh(SM_FOOTHOLD.Object);
	}
}

// Called when the game starts or when spawned
void ACPP_Time_Color_Button::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACPP_Time_Color_Button::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

