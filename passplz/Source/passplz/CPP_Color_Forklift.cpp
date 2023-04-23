// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Color_Forklift.h"

// Sets default values
ACPP_Color_Forklift::ACPP_Color_Forklift()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

	pillar1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("pillar 1"));
	pillar2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("pillar 2"));
	pillar3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("pillar 3"));
	pillar4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("pillar 4"));
	pillar5 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("pillar 5"));
	pillar6 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("pillar 6"));
	redForklift = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("redForklift"));
	greenForklift = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("greenForklift"));
	blueForklift = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("blueForklift"));
	blackForklift = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("blackForklift"));
	whiteForklift = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("whiteForklift"));
	yellowForklift = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("yellowForklift"));
	magentaForklift = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("magentaForklift"));
	cyanForklift = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("cyanForklift"));

	RootComponent = pillar1;

	pillar2->SetupAttachment(RootComponent);
	pillar3->SetupAttachment(RootComponent);
	pillar4->SetupAttachment(RootComponent);
	pillar5->SetupAttachment(RootComponent);
	pillar6->SetupAttachment(RootComponent);

	redForklift->SetupAttachment(RootComponent);
	greenForklift->SetupAttachment(RootComponent);
	blueForklift->SetupAttachment(RootComponent);
	blackForklift->SetupAttachment(RootComponent);
	whiteForklift->SetupAttachment(RootComponent);
	yellowForklift->SetupAttachment(RootComponent);
	magentaForklift->SetupAttachment(RootComponent);
	cyanForklift->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BUTTON(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (SM_BUTTON.Succeeded()) {
		pillar1->SetStaticMesh(SM_BUTTON.Object);
		pillar2->SetStaticMesh(SM_BUTTON.Object);
		pillar3->SetStaticMesh(SM_BUTTON.Object);
		pillar4->SetStaticMesh(SM_BUTTON.Object);
		pillar5->SetStaticMesh(SM_BUTTON.Object);
		pillar6->SetStaticMesh(SM_BUTTON.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_FORKLIFT(TEXT("/Game/model/STAGE_1/forklift.forklift"));
	if (SM_BUTTON.Succeeded()) {
		redForklift->SetStaticMesh(SM_FORKLIFT.Object);
		greenForklift->SetStaticMesh(SM_FORKLIFT.Object);
		blueForklift->SetStaticMesh(SM_FORKLIFT.Object);
		blackForklift->SetStaticMesh(SM_FORKLIFT.Object);
		whiteForklift->SetStaticMesh(SM_FORKLIFT.Object);
		yellowForklift->SetStaticMesh(SM_FORKLIFT.Object);
		magentaForklift->SetStaticMesh(SM_FORKLIFT.Object);
		cyanForklift->SetStaticMesh(SM_FORKLIFT.Object);
	}
}
// Called when the game starts or when spawned
void ACPP_Color_Forklift::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACPP_Color_Forklift::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

