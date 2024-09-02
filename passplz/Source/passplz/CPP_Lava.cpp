// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Lava.h"
#include "Kismet/KismetRenderingLibrary.h"
// Sets default values
ACPP_Lava::ACPP_Lava()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	explosionDecal = CreateDefaultSubobject<UDecalComponent>("targetLocation");
	lavaBall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("lavaBall"));
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	fireNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("fireNiagara"));
	CollisionComponent->InitSphereRadius(4.0f);

	RootComponent = CollisionComponent;
	lavaBall->SetupAttachment(CollisionComponent);
	explosionDecal->SetupAttachment(CollisionComponent);
	fireNiagara->SetupAttachment(CollisionComponent);

	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	ProjectileMovementComponent->InitialSpeed = 2000.f; //초기속도
	ProjectileMovementComponent->MaxSpeed = 2000.f; //최대속도
	ProjectileMovementComponent->bRotationFollowsVelocity = true; // 회전을 이동 방향에 따라 설정

	explosionDecal->SetWorldScale3D(FVector(5.0f, 1.4f, 1.4f));
	explosionDecal->SetVisibility(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_LAVABALL(TEXT("/Game/K_Test/input/fireball.fireball"));
	if (SM_LAVABALL.Succeeded()) {
		lavaBall->SetStaticMesh(SM_LAVABALL.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> m_MeltLava(TEXT("/Game/K_Test/input/m_MeltLava.m_MeltLava"));
	if (m_MeltLava.Succeeded())
	{
		explosionDecal->SetDecalMaterial(m_MeltLava.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> m_brush(TEXT("/Game/K_Test/input/m_brush.m_brush"));
	if (m_MeltLava.Succeeded())
	{
		brushMaterial = m_brush.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSystemAsset(TEXT("/Game/particles/missile/ns_missile.ns_missile"));
	if (NiagaraSystemAsset.Succeeded())
	{
		fireNiagara->SetAsset(NiagaraSystemAsset.Object);
	}


	isExplosion = false;
	opacityValue = 1.0f;
}

// Called when the game starts or when spawned
void ACPP_Lava::BeginPlay()
{
	Super::BeginPlay();
	lavaBall->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	explosionDecalDynamicMaterial = explosionDecal->CreateDynamicMaterialInstance();
	DynamicMaterial = UMaterialInstanceDynamic::Create(brushMaterial, nullptr);
}


// Called every frame
void ACPP_Lava::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isExplosion) {
		UE_LOG(LogTemp, Warning, TEXT("ACPP_Lava::Tick"));
		explosionDecal->SetWorldLocation(crashLocation);
		explosionDecal->SetWorldRotation(FRotator(90.f, 0.f, 0.f));
		
		MeltLavaFloor();
		explosionDecalDynamicMaterial->SetScalarParameterValue(TEXT("opacityValue"), opacityValue);
		opacityValue -= DeltaTime/7;
		if (opacityValue <= 0)
			Destroy();
	}

}

void ACPP_Lava::MeltLavaFloor()
{
	

	if (!DynamicMaterial)
		UE_LOG(LogTemp, Warning, TEXT("DynamicMaterial NO"));
	if (!RenderTargetTexture)
		UE_LOG(LogTemp, Warning, TEXT("RenderTargetTexture NO"));

	DynamicMaterial->SetVectorParameterValue(TEXT("Location"), FLinearColor(collisionUV.X, collisionUV.Y, 0.f, 1.0f));
	UKismetRenderingLibrary::DrawMaterialToRenderTarget(GetWorld(), RenderTargetTexture, DynamicMaterial);
	UE_LOG(LogTemp, Warning, TEXT("%f, %f"), collisionUV.X, collisionUV.Y);

	
}

void ACPP_Lava::MeltLavaFloorLocation()
{
	UE_LOG(LogTemp, Warning, TEXT("FindCollisionUV %f, %f"), collisionUV.X, collisionUV.Y);

	crashLocation = GetActorLocation();
	explosionDecal->SetVisibility(true);
	lavaBall->SetVisibility(false);
	

	FHitResult HitResult;
	FVector StartTraceLocation = crashLocation + FVector(0.f, 0.f, 500.f);
	FVector EndTraceLocation = StartTraceLocation + FVector(0.f, 0.f, -1000.f);
	FCollisionQueryParams TraceParams;
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnFaceIndex = true;

	bool result = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartTraceLocation,
		EndTraceLocation,
		ECollisionChannel::ECC_Visibility,
		TraceParams
	);

	UGameplayStatics::FindCollisionUV(HitResult, 0, collisionUV);

	UE_LOG(LogTemp, Warning, TEXT("FindCollisionUV %f, %f"), collisionUV.X, collisionUV.Y);


	isExplosion = true;
}

void ACPP_Lava::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Lava::OnComponentBeginOverlap_CollisionComponent);
}


//void ACPP_Lava::OnComponentBeginOverlap_CollisionComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	UE_LOG(LogTemp, Warning, TEXT("OnComponentHit_CollisionComponent"));
//
//	//Destroy();
//}