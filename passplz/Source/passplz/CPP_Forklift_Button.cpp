// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Forklift_Button.h"

// Sets default values
ACPP_Forklift_Button::ACPP_Forklift_Button()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	center = CreateDefaultSubobject<UArrowComponent>(TEXT("Center"));
	red = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("red"));
	redCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("redCollision"));
	green = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("green"));
	greenCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("greenCollision"));
	blue = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("blue"));
	blueCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("blueCollision"));
	black = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("black"));
	blackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("blackCollision"));

	RootComponent = center;

	red->SetupAttachment(RootComponent);
	green->SetupAttachment(RootComponent);
	blue->SetupAttachment(RootComponent);
	black->SetupAttachment(RootComponent);

	redCollision->SetupAttachment(red);
	greenCollision->SetupAttachment(green);
	blueCollision->SetupAttachment(blue);
	blackCollision->SetupAttachment(black);

	red->SetRelativeScale3D(FVector(0.5f, 0.5f, 1.0f));
	green->SetRelativeScale3D(FVector(0.5f, 0.5f, 1.0f));
	blue->SetRelativeScale3D(FVector(0.5f, 0.5f, 1.0f));
	black->SetRelativeScale3D(FVector(0.5f, 0.5f, 1.0f));

	red->SetRelativeLocation(FVector(0.0f, 120.0f, 0.0f));
	green->SetRelativeLocation(FVector(0.0f, 40.0f, 0.0f));
	blue->SetRelativeLocation(FVector(0.0f, -40.0f, 0.0f));
	black->SetRelativeLocation(FVector(0.0f, -120.0f, 0.0f));


	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BUTTON(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (SM_BUTTON.Succeeded()) {
		red->SetStaticMesh(SM_BUTTON.Object);
		green->SetStaticMesh(SM_BUTTON.Object);
		blue->SetStaticMesh(SM_BUTTON.Object);
		black->SetStaticMesh(SM_BUTTON.Object);
	}
}

// Called when the game starts or when spawned
void ACPP_Forklift_Button::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_Forklift_Button::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

