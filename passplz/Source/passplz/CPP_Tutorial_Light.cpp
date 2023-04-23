// Fill out your copyright notice in the Description page of Project Settings.
#include "CPP_Tutorial_Light.h"

// Sets default values
ACPP_Tutorial_Light::ACPP_Tutorial_Light()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	spotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("spotLight"));
	boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("boxCollision"));

	RootComponent = boxCollision;

	spotLight->SetupAttachment(RootComponent);

	boxCollision->SetBoxExtent(FVector(64.f, 64.f, 16.f));
	spotLight->SetRelativeLocation(FVector(0.f, 0.f, 1281.f));
	spotLight->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	spotLight->SetIntensity(0.f);
	spotLight->SetLightColor(FLinearColor(255, 255, 0));
	spotLight->SetAttenuationRadius(1708.195801f);
	spotLight->SetInnerConeAngle(5.f);
	spotLight->SetOuterConeAngle(6.f);
	spotLight->SetVolumetricScatteringIntensity(30.f);
	spotLight->SetIntensityUnits(ELightUnits::Unitless);

	lightLocation = { FVector(-8640.f, 2692.f, 20.f),
		FVector(-6247.f, 4250.f, 20.f),
		FVector(-1031.f, 5088.f, 20.f),
	};
	lightIndex = 0;
}

// Called when the game starts or when spawned
void ACPP_Tutorial_Light::BeginPlay()
{
	Super::BeginPlay();

	boxCollision->SetWorldLocation(lightLocation.at(lightIndex));
}

void ACPP_Tutorial_Light::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	boxCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Tutorial_Light::OnComponentBeginOverlap_boxCollision);
}

// Called every frame
void ACPP_Tutorial_Light::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACPP_Tutorial_Light::OnComponentBeginOverlap_boxCollision(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	lightIndex += 1;
	UE_LOG(LogTemp, Error, TEXT("+++++++++++ %d"), int(lightIndex));
	boxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TeleportTo(lightLocation.at(lightIndex), boxCollision->GetRelativeRotation());
	UE_LOG(LogTemp, Warning, TEXT("---------------- %d"), int(lightIndex));
	//boxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	if (3 <= lightIndex)
		spotLight->SetIntensity(0.f);
}