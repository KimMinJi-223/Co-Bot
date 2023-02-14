// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_GearKey_2S.h"

// Sets default values
ACPP_GearKey_2S::ACPP_GearKey_2S()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	clear = CreateDefaultSubobject<UBoxComponent>(TEXT("clear"));
	forward_lever_collision = CreateDefaultSubobject<UBoxComponent>(TEXT("forward_lever_collision"));
	back_lever_collision = CreateDefaultSubobject<UBoxComponent>(TEXT("back_lever_collision"));
	right_button_collision = CreateDefaultSubobject<UBoxComponent>(TEXT("right_button_collision"));
	left_button_collision = CreateDefaultSubobject<UBoxComponent>(TEXT("left_button_collision"));
	center_gear = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("center_gear"));
	child_gear1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("child_gear1"));
	child_gear2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("child_gear2"));
	child_gear3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("child_gear3"));
	Key = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Key"));
	Key_frame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Key_frame"));
	lever = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("lever"));
	right_button = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("right_button"));
	left_button = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("left_button"));

	RootComponent = clear;

	center_gear->SetupAttachment(RootComponent);
	child_gear1->SetupAttachment(RootComponent);
	child_gear2->SetupAttachment(RootComponent);
	child_gear3->SetupAttachment(RootComponent);

	Key->SetupAttachment(RootComponent);
	Key_frame->SetupAttachment(RootComponent);

	lever->SetupAttachment(RootComponent);
	forward_lever_collision->SetupAttachment(lever);
	back_lever_collision->SetupAttachment(lever);

	right_button->SetupAttachment(RootComponent);
	right_button_collision->SetupAttachment(right_button);

	left_button->SetupAttachment(RootComponent);
	left_button_collision->SetupAttachment(left_button);

	center_gear->SetRelativeLocation(FVector(-390.f, 0.f, 0.f));
	child_gear1->SetRelativeLocation(FVector(-390.f, 230.f, -80.f));
	child_gear2->SetRelativeLocation(FVector(-180.f, 460.f, -10.f));
	child_gear3->SetRelativeLocation(FVector(-500.f, -220.f, -80.f));
	Key->SetRelativeLocation(FVector(-600.f, 0.f, 290.f));
	Key_frame->SetRelativeLocation(FVector(30.f, 0.f, 0.f));
	lever->SetRelativeLocation(FVector(-800.f, -50.f, 0.f));
	forward_lever_collision->SetRelativeLocation(FVector(50.f,0.f, 30.f));
	back_lever_collision->SetRelativeLocation(FVector(-50.f, 0.f, 30.f));
	right_button->SetRelativeLocation(FVector(-750.f, 0.f, -90.f));
	right_button_collision->SetRelativeLocation(FVector(0.f, 0.f, 30.f));
	left_button->SetRelativeLocation(FVector(-850.f, 0.f, -90.f));
	left_button_collision->SetRelativeLocation(FVector(0.f, 0.f, 30.f));

	child_gear2->SetRelativeScale3D(FVector(0.5f, 1.f, 1.0f));
	child_gear3->SetRelativeScale3D(FVector(0.5f, 1.f, 1.0f));
	Key->SetRelativeScale3D(FVector(0.25f, 0.25f, 5.0f));
	right_button->SetRelativeScale3D(FVector(0.5f, 0.5f, 1.0f));
	left_button->SetRelativeScale3D(FVector(0.5f, 0.5f, 1.0f));
	lever->SetRelativeScale3D(FVector(0.5f, 0.5f, 5.0f));

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
		lever->SetStaticMesh(SM_KEY.Object);
		right_button->SetStaticMesh(SM_KEY.Object);
		left_button->SetStaticMesh(SM_KEY.Object);
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

	//move_forward = 0;
	//move_right = 0;
	////앞으로 가는 버튼을 누른 상태
	//if (Is_forward) {
	//	//앞으로 못간다.
	//	if (forward_target_collision->IsOverlappingComponent(Maze)) {
	//		move_forward = 0;
	//		UE_LOG(LogTemp, Warning, TEXT("NO"));

	//	}
	//	//앞으로 간다.
	//	else {
	//		UE_LOG(LogTemp, Warning, TEXT("Go"));

	//		move_forward = 1;
	//	}
	//	//버튼은 한 순간에 하나만 누르기 가능 
	//	//여기에 왔다는 것은 forward버튼을 누른 것
	//	//바로 리턴 가능
	//	//하지만 바로 리턴하면 타겟이 안 움직인다. 
	//	//추가코드 필요
	//	Target->AddWorldOffset(FVector(move_forward, 0.0f, 0.0f));
	//	return;
	//}

	////앞으로 가는 버튼을 누르고 있지 않다.그렇다면 뒤로 가는 버튼을 누르고 있는가?
	//else if (Is_back) {
	//	//뒤로 못 간다..
	//	if (back_target_collision->IsOverlappingComponent(Maze)) {
	//		move_forward = 0;
	//	}
	//	//뒤로 간다.
	//	else {
	//		move_forward = -1;
	//	}
	//	Target->AddWorldOffset(FVector(move_forward, 0.0f, 0.0f));
	//	return;
	//}

	//else if (Is_right) {
	//	//오른쪽으로 못간다.
	//	if (right_target_collision->IsOverlappingComponent(Maze)) {
	//		move_right = 0;
	//	}
	//	//오른쪽으로 간다.
	//	else {
	//		move_right = 1;
	//	}
	//	Target->AddWorldOffset(FVector(0.0f, move_right, 0.0f));
	//	return;
	//}

	//else if (Is_left) {
	//	//왼쪽로 못 간다..
	//	if (left_target_collision->IsOverlappingComponent(Maze)) {
	//		move_right = 0;
	//	}
	//	//왼쪽로 간다.
	//	else {
	//		move_right = -1;
	//	}
	//	Target->AddWorldOffset(FVector(0.0f, move_right, 0.0f));
	//	return;
	//}

}

void ACPP_GearKey_2S::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	forward_lever_collision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_GearKey_2S::OnForwardLeverOverlap);
	forward_lever_collision->OnComponentEndOverlap.AddDynamic(this, &ACPP_GearKey_2S::OnForwardLeverEndOverlap);

	back_lever_collision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_GearKey_2S::OnBackLeverOverlap);
	back_lever_collision->OnComponentEndOverlap.AddDynamic(this, &ACPP_GearKey_2S::OnBackLeverEndOverlap);

	right_button_collision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_GearKey_2S::OnRightButtonOverlap);
	right_button_collision->OnComponentEndOverlap.AddDynamic(this, &ACPP_GearKey_2S::OnRightButtonEndOverlap);

	left_button_collision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_GearKey_2S::OnLeftButtonOverlap);
	left_button_collision->OnComponentEndOverlap.AddDynamic(this, &ACPP_GearKey_2S::OnLeftButtonEndOverlap);
}

void ACPP_GearKey_2S::OnForwardLeverOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Is_forward = true;
}

void ACPP_GearKey_2S::OnForwardLeverEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Is_forward = false;
}

void ACPP_GearKey_2S::OnBackLeverOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Is_back = true;
}

void ACPP_GearKey_2S::OnBackLeverEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Is_back = false;
}

void ACPP_GearKey_2S::OnRightButtonOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Is_right = true;
}

void ACPP_GearKey_2S::OnRightButtonEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Is_right = false;
}

void ACPP_GearKey_2S::OnLeftButtonOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Is_left = true;
}

void ACPP_GearKey_2S::OnLeftButtonEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Is_left = false;
}
