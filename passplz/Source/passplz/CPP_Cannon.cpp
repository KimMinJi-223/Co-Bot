// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Cannon.h"
#include "CPP_Cobot_Car.h"
#include "CPP_Cobot_Car_Controller.h"
#include "CPP_Lava.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACPP_Cannon::ACPP_Cannon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	cannon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("cannon"));
	damageRadius = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("damageRadius"));
	targetLocation = CreateDefaultSubobject<UDecalComponent>("targetLocation");
	lavaCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("lavaCollision"));

	RootComponent = lavaCollision;
	cannon->SetupAttachment(RootComponent);
	damageRadius->SetupAttachment(RootComponent);
	targetLocation->SetupAttachment(damageRadius);

	
	targetLocation->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_CANNON(TEXT("/Game/K_Test/input/cannon_sub.cannon_sub"));
	if (SM_CANNON.Succeeded()) {
		cannon->SetStaticMesh(SM_CANNON.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SPHERE(TEXT("/Game/K_Test/input/Sphere.Sphere"));
	if (SM_SPHERE.Succeeded()) {
		damageRadius->SetStaticMesh(SM_SPHERE.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("/Game/K_Test/lava/m_circle.m_circle"));
	if (DecalMaterialAsset.Succeeded())
	{
		targetLocation->SetDecalMaterial(DecalMaterialAsset.Object);
	}

}

// Called when the game starts or when spawned
void ACPP_Cannon::BeginPlay()
{
	Super::BeginPlay();

}

void ACPP_Cannon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	lavaCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Cannon::OnComponentBeginOverlap_lavaCollision);
	lavaCollision->OnComponentEndOverlap.AddDynamic(this, &ACPP_Cannon::OnComponentEndOverlap_lavaCollision);

}

// Called every frame
void ACPP_Cannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	



}

void ACPP_Cannon::SetBombDropLocation(int player_number, double value)
{
	static FRotator rotate = FRotator(0.0, 0.0, 0.0);
	
	if (1 == player_number)
		rotate.Yaw = value;
	else if (2 == player_number)
		rotate.Pitch = value;
	else
		UE_LOG(LogTemp, Warning, TEXT("rotate err!"));

	UE_LOG(LogTemp, Warning, TEXT("yaw, pitch, roll: %lf, %lf, %lf"), rotate.Yaw, rotate.Pitch, rotate.Roll);

	cannon->SetRelativeRotation(rotate);
}

void ACPP_Cannon::FireLava()
{
	ACPP_Lava* lavaBall = GetWorld()->SpawnActor<ACPP_Lava>(ACPP_Lava::StaticClass(), fireStartLocation, GetActorRotation());
	lavaBall->ProjectileMovementComponent->Velocity = projectileVelocity;
	lavaBall->ProjectileMovementComponent->Activate();
}

void ACPP_Cannon::OnComponentBeginOverlap_lavaCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPP_Cobot_Car* car = Cast<ACPP_Cobot_Car>(OtherActor);

	if (car != nullptr) {
		Cast<ACPP_Cobot_Car_Controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->ChangeMode(1);
	}
}

void ACPP_Cannon::OnComponentEndOverlap_lavaCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACPP_Cobot_Car* car = Cast<ACPP_Cobot_Car>(OtherActor);

	if (car != nullptr) {
		Cast<ACPP_Cobot_Car_Controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->ChangeMode(0);
	}
}

