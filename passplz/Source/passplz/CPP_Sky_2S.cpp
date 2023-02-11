// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Sky_2S.h"

// Sets default values
ACPP_Sky_2S::ACPP_Sky_2S()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CenterSky = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CenterSky"));
	FrontSky = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrontSky"));
	BackSky = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackSky"));
	Button = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Button"));
	Trigger_Button = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger_Button"));

	RootComponent = Trigger_Button;

	CenterSky->SetupAttachment(RootComponent);
	FrontSky->SetupAttachment(RootComponent);
	BackSky->SetupAttachment(RootComponent);
	Button->SetupAttachment(RootComponent);

	CenterSky->SetRelativeLocation(FVector(0.f, 0.f, -100.f));
	CenterSky->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	FrontSky->SetRelativeLocation(FVector(300.f, 0.f, -100.f));
	FrontSky->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	BackSky->SetRelativeLocation(FVector(-300.f, 0.f, -100.f));
	BackSky->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	Button->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	Button->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	Button->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	Trigger_Button->SetBoxExtent(FVector(16.f, 16.f, 16.f));


	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SKY(TEXT("/Game/model/STAGE_2/sky.sky"));
	if (SM_SKY.Succeeded()) {
		CenterSky->SetStaticMesh(SM_SKY.Object);
		FrontSky->SetStaticMesh(SM_SKY.Object);
		BackSky->SetStaticMesh(SM_SKY.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BUTTON(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (SM_BUTTON.Succeeded()) {
		Button->SetStaticMesh(SM_BUTTON.Object);
	}

	move_value = 4000;
}

// Called when the game starts or when spawned
void ACPP_Sky_2S::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACPP_Sky_2S::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_Sky_2S::PostInitializeComponents()
{
	UE_LOG(LogTemp, Warning, TEXT("ACPP_Number_Sensor_door::PostInitializeComponents"));
	Super::PostInitializeComponents();
	Trigger_Button->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Sky_2S::OnButtonOverlap);
	//Trigger_Button->OnComponentEndOverlap.AddDynamic(this, &ACPP_Sky_2S::OnButtonEndOverlap);
}

void ACPP_Sky_2S::OnButtonOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("ACPP_Sky_2S::OnFootOverlap"));

	FLatentActionInfo Info;
	Info.CallbackTarget = this;

	FVector currnet_locatoin = GetActorLocation();

	UKismetSystemLibrary::MoveComponentTo(
		RootComponent,
		FVector(move_value, currnet_locatoin.Y, currnet_locatoin.Z),
		FRotator(0.f, 0.f, 0.f),
		false,
		false,
		7.f,
		false,
		EMoveComponentAction::Move,
		Info);
	Trigger_Button->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

//충돌이 끝났을때 실행 현재 기획상 필요는 없다.
//void ACPP_Sky_2S::OnButtonEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//}

