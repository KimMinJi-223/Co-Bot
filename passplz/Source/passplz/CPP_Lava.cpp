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
	CollisionComponent->InitSphereRadius(4.0f);

	RootComponent = CollisionComponent;
	lavaBall->SetupAttachment(CollisionComponent);
	explosionDecal->SetupAttachment(CollisionComponent);

	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	ProjectileMovementComponent->InitialSpeed = 2000.f; //초기속도
	ProjectileMovementComponent->MaxSpeed = 2000.f; //최대속도
	ProjectileMovementComponent->bRotationFollowsVelocity = true; // 회전을 이동 방향에 따라 설정

	explosionDecal->SetWorldScale3D(FVector(5.0f, 1.4f, 1.4f));
	explosionDecal->SetVisibility(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_LAVABALL(TEXT("/Game/K_Test/input/cap_Object004.cap_Object004"));
	if (SM_LAVABALL.Succeeded()) {
		lavaBall->SetStaticMesh(SM_LAVABALL.Object);
	}

	isExplosion = false;
	opacityValue = 1.0f;
}

// Called when the game starts or when spawned
void ACPP_Lava::BeginPlay()
{
	Super::BeginPlay();
	lavaBall->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	explosionDecal->SetDecalMaterial(LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/K_Test/lava/m_MeltLava.m_MeltLava")));
	explosionDecalDynamicMaterial = explosionDecal->CreateDynamicMaterialInstance();
	//머티리얼 설정
	UMaterialInterface* BaseMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/K_Test/lava/m_brush.m_brush"));
	DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, nullptr);

	//RenderTargetTexture = Cast<UTextureRenderTarget2D>(StaticLoadObject(UTextureRenderTarget2D::StaticClass(), nullptr, TEXT("/Game/K_Test/lava/rendertaget.rendertaget")));
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
	
	FVector2D collisionUV(0.f, 0.f);
	UGameplayStatics::FindCollisionUV(HitResult, 0, collisionUV);

	if (!DynamicMaterial)
		UE_LOG(LogTemp, Warning, TEXT("DynamicMaterial NO"));
	if (!RenderTargetTexture)
		UE_LOG(LogTemp, Warning, TEXT("RenderTargetTexture NO"));

	DynamicMaterial->SetVectorParameterValue(TEXT("Location"), FLinearColor(collisionUV.X, collisionUV.Y, 0.f, 1.0f));
	if (result) {
		UKismetRenderingLibrary::DrawMaterialToRenderTarget(GetWorld(), RenderTargetTexture, DynamicMaterial);
		UE_LOG(LogTemp, Warning, TEXT("%f, %f"), collisionUV.X, collisionUV.Y);
		UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), crashLocation.X, crashLocation.Y, crashLocation.Z);
		UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), HitResult.Location.X, HitResult.Location.Y, HitResult.Location.Z);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), crashLocation.X, crashLocation.Y, crashLocation.Z);
		UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), EndTraceLocation.X, EndTraceLocation.Y, EndTraceLocation.Z);
		UE_LOG(LogTemp, Warning, TEXT("False"));
	}



}

void ACPP_Lava::MeltLavaFloorLocation()
{
	crashLocation = GetActorLocation();
	explosionDecal->SetVisibility(true);
	lavaBall->SetVisibility(false);
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