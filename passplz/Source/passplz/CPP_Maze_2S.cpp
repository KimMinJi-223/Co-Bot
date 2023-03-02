// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Maze_2S.h"

// Sets default values
ACPP_Maze_2S::ACPP_Maze_2S()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	center = CreateDefaultSubobject<UBoxComponent>(TEXT("center"));
	Maze = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Maze"));
	plan = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("plan"));
	Target = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Target"));
	forward_target_collision = CreateDefaultSubobject<UBoxComponent>(TEXT("forward_target_collision"));
	back_target_collision = CreateDefaultSubobject<UBoxComponent>(TEXT("back_target_collision"));
	right_target_collision = CreateDefaultSubobject<UBoxComponent>(TEXT("right_target_collision"));
	left_target_collision = CreateDefaultSubobject<UBoxComponent>(TEXT("left_target_collision"));
	forward_button = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("forward_button"));
	forward_button_collision = CreateDefaultSubobject<UBoxComponent>(TEXT("forward_button_collision"));
	back_button = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("back_button"));
	back_button_collision = CreateDefaultSubobject<UBoxComponent>(TEXT("back_button_collision"));
	right_button = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("right_button"));
	right_button_collision = CreateDefaultSubobject<UBoxComponent>(TEXT("right_button_collision"));
	left_button = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("left_button"));
	left_button_collision = CreateDefaultSubobject<UBoxComponent>(TEXT("left_button_collision"));

	RootComponent = center;

	Maze->SetupAttachment(RootComponent);

	plan->SetupAttachment(RootComponent);

	Target->SetupAttachment(RootComponent);
	forward_target_collision->SetupAttachment(Target);
	back_target_collision->SetupAttachment(Target);
	right_target_collision->SetupAttachment(Target);
	left_target_collision->SetupAttachment(Target);

	forward_button->SetupAttachment(RootComponent);
	forward_button_collision->SetupAttachment(forward_button);

	back_button->SetupAttachment(RootComponent);
	back_button_collision->SetupAttachment(back_button);

	right_button->SetupAttachment(RootComponent);
	right_button_collision->SetupAttachment(right_button);

	left_button->SetupAttachment(RootComponent);
	left_button_collision->SetupAttachment(left_button);

	Maze->SetRelativeRotation(FRotator(0.f, -90.f, -90.f)); // y, z, x

	plan->SetRelativeLocation(FVector(-10.f, 0.f, -100.f));
	plan->SetRelativeScale3D(FVector(10.f, 10.f, 1.0f));

	Target->SetRelativeLocation(FVector(-410.f, 0.0f, 0.0f));
	Target->SetRelativeScale3D(FVector(0.6f, 0.6f, 1.0f));

	forward_target_collision->SetRelativeLocation(FVector(50.f, 0.0f, 0.0f));
	back_target_collision->SetRelativeLocation(FVector(-50.f, 0.0f, 0.0f));
	right_target_collision->SetRelativeLocation(FVector(0.f, 50.0f, 0.0f));
	left_target_collision->SetRelativeLocation(FVector(0.f, -50.0f, 0.0f));
	forward_target_collision->SetBoxExtent(FVector(10.f, 50.f, 32.f));
	back_target_collision->SetBoxExtent(FVector(10.f, 50.f, 32.f));
	right_target_collision->SetBoxExtent(FVector(50.f, 10.f, 32.f));
	left_target_collision->SetBoxExtent(FVector(50.f, 10.f, 32.f));

	forward_button->SetRelativeLocation(FVector(-1030.f, 0.f, -20.f));
	forward_button->SetRelativeScale3D(FVector(0.5f, 0.5f, 1.0f));
	forward_button_collision->SetRelativeLocation(FVector(0.f, 0.f, 30.f));

	back_button->SetRelativeLocation(FVector(-1110.f, 0.f, -20.f));
	back_button->SetRelativeScale3D(FVector(0.5f, 0.5f, 1.0f));
	back_button_collision->SetRelativeLocation(FVector(0.f, 0.f, 30.f));

	right_button->SetRelativeLocation(FVector(-1030.f, 400.f, -20.f));
	right_button->SetRelativeScale3D(FVector(0.5f, 0.5f, 1.0f));
	right_button_collision->SetRelativeLocation(FVector(0.f, 0.f, 30.f));

	left_button->SetRelativeLocation(FVector(-1110.f, 400.f, -20.f));
	left_button->SetRelativeScale3D(FVector(0.5f, 0.5f, 1.0f));
	left_button_collision->SetRelativeLocation(FVector(0.f, 0.f, 30.f));


	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_MAZE(TEXT("/Game/model/STAGE_2/mazepipe3.mazepipe3"));
	if (SM_MAZE.Succeeded()) {
		Maze->SetStaticMesh(SM_MAZE.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_CUBE(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (SM_CUBE.Succeeded()) {
		plan->SetStaticMesh(SM_CUBE.Object);
		Target->SetStaticMesh(SM_CUBE.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BUTTON(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (SM_BUTTON.Succeeded()) {
		forward_button->SetStaticMesh(SM_BUTTON.Object);
		back_button->SetStaticMesh(SM_BUTTON.Object);
		right_button->SetStaticMesh(SM_BUTTON.Object);
		left_button->SetStaticMesh(SM_BUTTON.Object);
	}

	center->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	move_forward = 0;
	move_right = 0;
}

// Called when the game starts or when spawned
void ACPP_Maze_2S::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_Maze_2S::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	move_forward = 0;
	move_right = 0;
	//앞으로 가는 버튼을 누른 상태
	if (Is_forward) {
		//앞으로 못간다.
		if (forward_target_collision->IsOverlappingComponent(Maze)) {
			move_forward = 0;
			UE_LOG(LogTemp, Warning, TEXT("NO"));

		}
		//앞으로 간다.
		else {
			UE_LOG(LogTemp, Warning, TEXT("Go"));

			move_forward = 1;
		}
		//버튼은 한 순간에 하나만 누르기 가능 
		//여기에 왔다는 것은 forward버튼을 누른 것
		//바로 리턴 가능
		//하지만 바로 리턴하면 타겟이 안 움직인다. 
		//추가코드 필요
		Target->AddWorldOffset(FVector(move_forward, 0.0f, 0.0f));
		return;
	}

	//앞으로 가는 버튼을 누르고 있지 않다.그렇다면 뒤로 가는 버튼을 누르고 있는가?
	else if(Is_back) {
		//뒤로 못 간다..
		if (back_target_collision->IsOverlappingComponent(Maze)) {
			move_forward = 0;
		}
		//뒤로 간다.
		else {
			move_forward = -1;
		}
		Target->AddWorldOffset(FVector(move_forward, 0.0f, 0.0f));
		return;
	}

	else if (Is_right) {
		//오른쪽으로 못간다.
		if (right_target_collision->IsOverlappingComponent(Maze)) {
			move_right = 0;
		}
		//오른쪽으로 간다.
		else {
			move_right = 1;
		}
		Target->AddWorldOffset(FVector(0.0f, move_right, 0.0f));
		return;
	}

	else if (Is_left) {
		//왼쪽로 못 간다..
		if (left_target_collision->IsOverlappingComponent(Maze)) {
			move_right = 0;
		}
		//왼쪽로 간다.
		else {
			move_right = -1;
		}
		Target->AddWorldOffset(FVector(0.0f, move_right, 0.0f));
		return;
	}
}

void ACPP_Maze_2S::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	forward_button_collision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Maze_2S::OnForwardButtonOverlap);
	forward_button_collision->OnComponentEndOverlap.AddDynamic(this, &ACPP_Maze_2S::OnForwardButtonEndOverlap);

	back_button_collision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Maze_2S::OnBackButtonOverlap);
	back_button_collision->OnComponentEndOverlap.AddDynamic(this, &ACPP_Maze_2S::OnBackButtonEndOverlap);

	right_button_collision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Maze_2S::OnRightButtonOverlap);
	right_button_collision->OnComponentEndOverlap.AddDynamic(this, &ACPP_Maze_2S::OnRightButtonEndOverlap);

	left_button_collision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Maze_2S::OnLeftButtonOverlap);
	left_button_collision->OnComponentEndOverlap.AddDynamic(this, &ACPP_Maze_2S::OnLeftButtonEndOverlap);
}

void ACPP_Maze_2S::OnForwardButtonOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Is_forward = true;
}

void ACPP_Maze_2S::OnForwardButtonEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Is_forward = false;
}

void ACPP_Maze_2S::OnBackButtonOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Is_back = true;
}

void ACPP_Maze_2S::OnBackButtonEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Is_back = false;
}

void ACPP_Maze_2S::OnRightButtonOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Is_right = true;
}

void ACPP_Maze_2S::OnRightButtonEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Is_right = false;
}

void ACPP_Maze_2S::OnLeftButtonOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Is_left = true;
}

void ACPP_Maze_2S::OnLeftButtonEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Is_left = false;
}

