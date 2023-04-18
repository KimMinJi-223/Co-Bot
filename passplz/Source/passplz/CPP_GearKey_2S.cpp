// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_GearKey_2S.h"

// Sets default values
ACPP_GearKey_2S::ACPP_GearKey_2S()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	clear = CreateDefaultSubobject<UBoxComponent>(TEXT("clear"));
	forward_Key_collision = CreateDefaultSubobject<UBoxComponent>(TEXT("forward_Key_collision"));
	back_Key_collision = CreateDefaultSubobject<UBoxComponent>(TEXT("back_Key_collision"));
	right_Key_collision = CreateDefaultSubobject<UBoxComponent>(TEXT("right_Key_collision"));
	left_Key_collision = CreateDefaultSubobject<UBoxComponent>(TEXT("left_Key_collision"));

	center_gear = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("center_gear"));
	child_gear1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("child_gear1"));
	child_gear2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("child_gear2"));
	child_gear3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("child_gear3"));
	Key = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Key"));
	Key_frame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Key_frame"));


	RootComponent = clear;

	center_gear->SetupAttachment(RootComponent);
	child_gear1->SetupAttachment(RootComponent);
	child_gear2->SetupAttachment(RootComponent);
	child_gear3->SetupAttachment(RootComponent);

	Key->SetupAttachment(RootComponent);
	forward_Key_collision->SetupAttachment(Key);
	back_Key_collision->SetupAttachment(Key);
	right_Key_collision->SetupAttachment(Key);
	left_Key_collision->SetupAttachment(Key);

	Key_frame->SetupAttachment(RootComponent);

	center_gear->SetRelativeLocation(FVector(-390.f, 0.f, 0.f));
	child_gear1->SetRelativeLocation(FVector(-390.f, 230.f, -80.f));
	child_gear2->SetRelativeLocation(FVector(-180.f, 460.f, -10.f));
	child_gear3->SetRelativeLocation(FVector(-500.f, -220.f, -80.f));
	Key->SetRelativeLocation(FVector(-600.f, 0.f, 290.f));
	Key_frame->SetRelativeLocation(FVector(30.f, 0.f, 0.f));
	forward_Key_collision->SetRelativeLocation(FVector(20.f, 0.0f, -20.0f));
	back_Key_collision->SetRelativeLocation(FVector(-20.f, 0.0f, -20.0f));
	right_Key_collision->SetRelativeLocation(FVector(0.f, 20.0f, -20.0f));
	left_Key_collision->SetRelativeLocation(FVector(0.f, -20.0f, -20.0f));

	child_gear2->SetRelativeScale3D(FVector(0.5f, 1.f, 1.0f));
	child_gear3->SetRelativeScale3D(FVector(0.5f, 1.f, 1.0f));
	Key->SetRelativeScale3D(FVector(0.25f, 0.25f, 5.0f));

	child_gear2->SetRelativeRotation(FRotator(0.0f, 0.0f, -15.0f));


	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GEAR(TEXT("/Game/model/STAGE_2/gear_1.gear_1"));
	if (SM_GEAR.Succeeded()) {
		center_gear->SetStaticMesh(SM_GEAR.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_CHILD_GEAR(TEXT("/Game/model/STAGE_2/gear_2.gear_2"));
	if (SM_CHILD_GEAR.Succeeded()) {
		child_gear1->SetStaticMesh(SM_CHILD_GEAR.Object);
		child_gear2->SetStaticMesh(SM_CHILD_GEAR.Object);
		child_gear3->SetStaticMesh(SM_CHILD_GEAR.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_KEY(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (SM_KEY.Succeeded()) {
		Key->SetStaticMesh(SM_KEY.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_KEY_FRAME(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (SM_KEY_FRAME.Succeeded()) {
		Key_frame->SetStaticMesh(SM_KEY_FRAME.Object);
	}
}

// Called when the game starts or when spawned
void ACPP_GearKey_2S::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACPP_GearKey_2S::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACPP_GearKey_2S::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	clear->OnComponentBeginOverlap.AddDynamic(this, &ACPP_GearKey_2S::OnClearOverlap);
}

void ACPP_GearKey_2S::target_forward()
{
	UE_LOG(LogTemp, Warning, TEXT("target_forward"));

	if (!forward_Key_collision->IsOverlappingComponent(center_gear)) {
		Key->AddLocalOffset(FVector(1, 0.0f, 0.0f));
	}
}

void ACPP_GearKey_2S::target_back()
{
	UE_LOG(LogTemp, Warning, TEXT("target_back"));
	if (!back_Key_collision->IsOverlappingComponent(center_gear)) {
		Key->AddLocalOffset(FVector(-1.f, 0.0f, 0.0f));
	}
}

void ACPP_GearKey_2S::target_right()
{

	UE_LOG(LogTemp, Warning, TEXT("target_right"));

	Key->AddLocalOffset(FVector(0.f, -0.001f, 0.0));

	if (!center_gear->IsOverlappingComponent(left_Key_collision)) {
		center_gear->AddLocalRotation(FRotator(0.0f, 0.0f, 1 * 0.5));
		child_gear1->AddLocalRotation(FRotator(0.0f, 0.0f, -1 * 0.5));
		child_gear2->AddLocalRotation(FRotator(0.0f, 0.0f, 1 * 0.5));
		child_gear3->AddLocalRotation(FRotator(0.0f, 0.0f, -1 * 0.5));
	}
	Key->AddLocalOffset(FVector(0.f, 0.001f, 0.0));
}

void ACPP_GearKey_2S::target_left()
{
	UE_LOG(LogTemp, Warning, TEXT("target_left"));

	Key->AddLocalOffset(FVector(0.f, 0.001f, 0.0));

	if (!center_gear->IsOverlappingComponent(right_Key_collision)) {
		center_gear->AddLocalRotation(FRotator(0.0f, 0.0f, -1 * 0.5));
		child_gear1->AddLocalRotation(FRotator(0.0f, 0.0f, 1 * 0.5));
		child_gear2->AddLocalRotation(FRotator(0.0f, 0.0f, -1 * 0.5));
		child_gear3->AddLocalRotation(FRotator(0.0f, 0.0f, 1 * 0.5));

	}
	Key->AddLocalOffset(FVector(0.f, -0.001f, 0.0));
}

void ACPP_GearKey_2S::OnClearOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//clear->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//forward_lever_collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//back_lever_collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//right_button_collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//left_button_collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//forward_Key_collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//back_Key_collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//right_Key_collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//left_Key_collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

