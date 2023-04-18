// Fill out your copyright notice in the Description page of Project Settings.

#include "../../../server/server/protocol.h"
#include "CPP_Cobot_Controller.h"
#include "CPP_Stage2_MissionButton.h"

// Sets default values
ACPP_Stage2_MissionButton::ACPP_Stage2_MissionButton()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Center = CreateDefaultSubobject<UArrowComponent>(TEXT("Center"));
	Maze_target_forward = CreateDefaultSubobject<UBoxComponent>(TEXT("Maze_target_forward"));
	Maze_target_back = CreateDefaultSubobject<UBoxComponent>(TEXT("Maze_target_back"));
	Maze_target_right = CreateDefaultSubobject<UBoxComponent>(TEXT("Maze_target_right"));
	Maze_target_left = CreateDefaultSubobject<UBoxComponent>(TEXT("Maze_target_left"));

	RootComponent = Center;
	Maze_target_forward->SetupAttachment(RootComponent);
	Maze_target_back->SetupAttachment(RootComponent);
	Maze_target_right->SetupAttachment(RootComponent);
	Maze_target_left->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACPP_Stage2_MissionButton::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACPP_Stage2_MissionButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_Stage2_MissionButton::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//충돌 시작 함수 지정
	Maze_target_forward->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Stage2_MissionButton::OnMazeforwardOverlap);
	Maze_target_back->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Stage2_MissionButton::OnMazeBackOverlap);
	Maze_target_right->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Stage2_MissionButton::OnMazeRightOverlap);
	Maze_target_left->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Stage2_MissionButton::OnMazeLeftOverlap);

	//충돌 종료 함수 지정
	Maze_target_forward->OnComponentEndOverlap.AddDynamic(this, &ACPP_Stage2_MissionButton::OnMazeforwardEndOverlap);
	Maze_target_back->OnComponentEndOverlap.AddDynamic(this, &ACPP_Stage2_MissionButton::OnMazeBackEndOverlap);
	Maze_target_right->OnComponentEndOverlap.AddDynamic(this, &ACPP_Stage2_MissionButton::OnMazeRightEndOverlap);
	Maze_target_left->OnComponentEndOverlap.AddDynamic(this, &ACPP_Stage2_MissionButton::OnMazeLeftEndOverlap);
}

//코봇이 버튼을 밟았을때 어떤 버튼을 밟았는지 서버로 보낸다======================================================
void ACPP_Stage2_MissionButton::OnMazeforwardOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SOCKET* sock = Cast<ACPP_Cobot_Controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->GetSocket();

	cs_button_packet button_pack;
	button_pack.size = sizeof(button_pack);
	button_pack.type = static_cast<char>(packet_type::cs_push_button_maze_forward);

	int ret = send(*sock, reinterpret_cast<char*>(&button_pack), sizeof(button_pack), 0);

	UE_LOG(LogTemp, Warning, TEXT("OnMazeforwardOverlap"));
}

void ACPP_Stage2_MissionButton::OnMazeBackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SOCKET* sock = Cast<ACPP_Cobot_Controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->GetSocket();

	cs_button_packet button_pack;
	button_pack.size = sizeof(button_pack);
	button_pack.type = static_cast<char>(packet_type::cs_push_button_maze_back);

	int ret = send(*sock, reinterpret_cast<char*>(&button_pack), sizeof(button_pack), 0);

	UE_LOG(LogTemp, Warning, TEXT("OnMazeBackOverlap"));
}

void ACPP_Stage2_MissionButton::OnMazeRightOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SOCKET* sock = Cast<ACPP_Cobot_Controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->GetSocket();

	cs_button_packet button_pack;
	button_pack.size = sizeof(button_pack);
	button_pack.type = static_cast<char>(packet_type::cs_push_button_maze_right);

	int ret = send(*sock, reinterpret_cast<char*>(&button_pack), sizeof(button_pack), 0);

	UE_LOG(LogTemp, Warning, TEXT("OnMazeRightOverlap"));
}

void ACPP_Stage2_MissionButton::OnMazeLeftOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SOCKET* sock = Cast<ACPP_Cobot_Controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->GetSocket();

	cs_button_packet button_pack;
	button_pack.size = sizeof(button_pack);
	button_pack.type = static_cast<char>(packet_type::cs_push_button_maze_left);

	int ret = send(*sock, reinterpret_cast<char*>(&button_pack), sizeof(button_pack), 0);

	UE_LOG(LogTemp, Warning, TEXT("OnMazeLeftOverlap"));
}
//버튼 충돌이 끝났음을 알린다.
void ACPP_Stage2_MissionButton::OnMazeforwardEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	SOCKET* sock = Cast<ACPP_Cobot_Controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->GetSocket();

	cs_button_packet button_pack;
	button_pack.size = sizeof(button_pack);
	button_pack.type = static_cast<char>(packet_type::cs_end_button_maze_forward);

	int ret = send(*sock, reinterpret_cast<char*>(&button_pack), sizeof(button_pack), 0);

	UE_LOG(LogTemp, Warning, TEXT("OnMazeforwardEndOverlap"));
}
void ACPP_Stage2_MissionButton::OnMazeBackEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	SOCKET* sock = Cast<ACPP_Cobot_Controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->GetSocket();

	cs_button_packet button_pack;
	button_pack.size = sizeof(button_pack);
	button_pack.type = static_cast<char>(packet_type::cs_end_button_maze_back);

	int ret = send(*sock, reinterpret_cast<char*>(&button_pack), sizeof(button_pack), 0);

	UE_LOG(LogTemp, Warning, TEXT("OnMazeBackEndOverlap"));
}
void ACPP_Stage2_MissionButton::OnMazeRightEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	SOCKET* sock = Cast<ACPP_Cobot_Controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->GetSocket();

	cs_button_packet button_pack;
	button_pack.size = sizeof(button_pack);
	button_pack.type = static_cast<char>(packet_type::cs_end_button_maze_right);

	int ret = send(*sock, reinterpret_cast<char*>(&button_pack), sizeof(button_pack), 0);

	UE_LOG(LogTemp, Warning, TEXT("OnMazeRightEndOverlap"));
}
void ACPP_Stage2_MissionButton::OnMazeLeftEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	SOCKET* sock = Cast<ACPP_Cobot_Controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->GetSocket();

	cs_button_packet button_pack;
	button_pack.size = sizeof(button_pack);
	button_pack.type = static_cast<char>(packet_type::cs_end_button_maze_left);

	int ret = send(*sock, reinterpret_cast<char*>(&button_pack), sizeof(button_pack), 0);

	UE_LOG(LogTemp, Warning, TEXT("OnMazeLeftEndOverlap"));
}
//===================================================================================================================

