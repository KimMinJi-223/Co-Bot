// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_ChildSky_2S.h"

// Sets default values
ACPP_ChildSky_2S::ACPP_ChildSky_2S()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Sky = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sky"));
	RootComponent = Sky;

	Sky->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SKY(TEXT("/Game/model/STAGE_2/sky.sky"));
	if (SM_SKY.Succeeded()) {
		Sky->SetStaticMesh(SM_SKY.Object);
	}
}

// Called when the game starts or when spawned
void ACPP_ChildSky_2S::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_ChildSky_2S::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

