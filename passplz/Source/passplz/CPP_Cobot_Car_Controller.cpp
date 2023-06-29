// Fill out your copyright notice in the Description page of Project Settings.

//기존 컨트롤러에서 했던거 패킷 처리 필요

#define _CRT_SECURE_NO_WARNINGS

#include "CPP_Cobot_Car_Controller.h"
#include "CPP_Cobot_Car.h"
//#include "Engine/World.h"
#include "CPP_Cannon.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

#include "../../../server/server/protocol.h"

ACPP_Cobot_Car_Controller::ACPP_Cobot_Car_Controller()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>DEFAULT_CONTEXT
	(TEXT("/Game/K_Test/input/stage3IMCTest.stage3IMCTest"));
	if (DEFAULT_CONTEXT.Succeeded())
		DefaultContext = DEFAULT_CONTEXT.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_MOVE
	(TEXT("/Game/K_Test/input/Move.Move"));
	if (IA_MOVE.Succeeded())
		Move = IA_MOVE.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_MOUSEWHEEL
	(TEXT("/Game/K_Test/input/MouseWheel.MouseWheel"));
	if (IA_MOUSEWHEEL.Succeeded())
		MouseWheel = IA_MOUSEWHEEL.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_MOUSELEFT
	(TEXT("/Game/K_Test/input/MouseLeft.MouseLeft"));
	if (IA_MOUSELEFT.Succeeded())
		MouseLeft = IA_MOUSELEFT.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_ROTATE
	(TEXT("/Game/K_Test/input/rotate.rotate"));
	if (IA_ROTATE.Succeeded())
		Rotate = IA_ROTATE.Object;
}

ACPP_Cobot_Car_Controller::~ACPP_Cobot_Car_Controller()
{
}

void ACPP_Cobot_Car_Controller::BeginPlay()
{
	Super::BeginPlay();

	instance = Cast<UCPP_CobotGameInstance>(GetWorld()->GetGameInstance());
	sock = instance->GetSocketMgr()->GetSocket();

	cs_stage3_enter_packet pack;
	pack.size = sizeof(pack);
	pack.type = static_cast<char>(packet_type::cs_stage3_enter);

	int ret = send(*sock, reinterpret_cast<char*>(&pack), sizeof(pack), 0);

	UE_LOG(LogTemp, Warning, TEXT("car controller enter!"));

	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		SubSystem->AddMappingContext(DefaultContext, 0);

	player = Cast<ACPP_Cobot_Car>(GetPawn());
	if (!player)
		return;

	cannon = UGameplayStatics::GetActorOfClass(GetWorld(), ACPP_Cannon::StaticClass());
	if(cannon)
		UE_LOG(LogTemp, Warning, TEXT("cannon OK"));

}

void ACPP_Cobot_Car_Controller::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ACPP_Cobot_Car_Controller::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	UE_LOG(LogTemp, Warning, TEXT("OnPossess"));
}

void ACPP_Cobot_Car_Controller::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		//자동차 움직이기
		EnhancedInputComponent->BindAction(Move, ETriggerEvent::Triggered, this, &ACPP_Cobot_Car_Controller::CarInput);
		//조준점 조정
		EnhancedInputComponent->BindAction(MouseWheel, ETriggerEvent::Triggered, this, &ACPP_Cobot_Car_Controller::CannonInput);
		//발사
		EnhancedInputComponent->BindAction(MouseLeft, ETriggerEvent::Triggered, this, &ACPP_Cobot_Car_Controller::FireCannonInput);
		//시점 변화
		EnhancedInputComponent->BindAction(Rotate, ETriggerEvent::Triggered, this, &ACPP_Cobot_Car_Controller::RotateInput);
	}
}

void ACPP_Cobot_Car_Controller::RecvPacket()
{
	char buff[BUF_SIZE];

	int ret = recv(*sock, reinterpret_cast<char*>(&buff), BUF_SIZE, 0);
	if (ret <= 0)
	{
		GetLastError();
		std::cout << "recv() fail!" << std::endl;
		return;
	}

	if (prev_remain > 0) // 만약 전에 남아있는 데이터가 있다면
	{
		strcat(prev_packet_buff, buff);
	} else
	{
		memcpy(prev_packet_buff, buff, ret);
	}

	int remain_data = ret + prev_remain;
	char* p = prev_packet_buff;

	while (remain_data > 0)
	{
		int packet_size = p[0];
		if (packet_size <= remain_data)
		{
			ProcessPacket(p);
			p = p + packet_size;
			remain_data -= packet_size;
		} else break;
	}

	prev_remain = remain_data;

	if (remain_data > 0)
	{
		memcpy(prev_packet_buff, p, remain_data);
	}
}

void ACPP_Cobot_Car_Controller::ProcessPacket(char* packet)
{
    switch (packet[1])
    {
	case static_cast<int>(packet_type::sc_car_direction):
	{
		//UE_LOG(LogTemp, Warning, TEXT("recv sc_car_direction"));

		sc_car_direction_packet* pack = reinterpret_cast<sc_car_direction_packet*>(packet);
		// 일단은 0이 직진
		// 좌는 5ms마다 -0.1
		// 우는 5ms마다 +0.1

		// 클라: 여기 자동차 돌리는 함수 호출
		//CarForward();
		UE_LOG(LogTemp, Warning, TEXT("direction: %lf"), pack->direction);

		// 근데 이거 너무 뚝뚝 끊김 부드럽게 안되나?
		if (0.0 == pack->direction)
			//CarForward(서버에서 받는 값이 필요);
			;
		else 
			CarRotation(pack->direction);

	} break;
    }
}

void ACPP_Cobot_Car_Controller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RecvPacket();
}

void ACPP_Cobot_Car_Controller::CarInput(const FInputActionValue& Value)
{
	//서버 : 여기서 키를 누르거나 떼면 여길 들어오는데 이때 서버에 패킷 보내야함
	//서버에는 각 클라 키에 대한 bool값을 가진다.
	//서버에서 가속과 관련된 계산이 필요합니다. -> 점점 커지는 값 그리고 뗐을때는 점점 작아지는 값 필요 그 값을 넘겨주세요
	UE_LOG(LogTemp, Warning, TEXT("%f"), Value.Get<float>());

	cs_car_direction_packet pack;
	pack.size = sizeof(pack);
	pack.type = static_cast<char>(packet_type::cs_car_direction);

	if (0.0 != Value.Get<float>())
		pack.direction = true;
	else
		pack.direction = false;

	int ret = send(*sock, reinterpret_cast<char*>(&pack), sizeof(pack), 0);
	if (mode == 0) {
		UE_LOG(LogTemp, Warning, TEXT("CarInput %f"), Value.Get<float>());
	}
}


void ACPP_Cobot_Car_Controller::CannonInput(const FInputActionValue& Value)
{
	if (mode == 1) {
		UE_LOG(LogTemp, Warning, TEXT("CannonInput %f"), Value.Get<float>());
	}

	//서버 : 1번 클라는 조준점을 앞뒤로, 2번클라는 양옆으로 움직이기 해야함
	//서버에 패킷 보낼때 Value.Get<float>() 이 값 가져가야함 이 값은 마우스 휠 움직인 방향에 따라 음수, 양수 값인데 이걸로 조준점 위치 계산해서 보내주셈
	//서버는 이를 위해 대포의 회전값을 가지는 FRotator가 필요 yaw는 1번클라, pitch는 2번클라가 보내준 값을 더해서 최종 대포의 회전 값을 다시 클라에게 보내줘야함
	//cpp_cannon에 있는 SetBombDropLocation함수 호출하면 됨

	//서버에는 이런식으로 게산해서 밑에 코드 어딘가에 추가해서 호출하면 됨
	//FRotator rota;
	//rota.Roll = 서버에서 넘겨준값;
	//rota.Yaw = 서버에서 넘겨준값;
	//rota.Pitch = 서버에서 넘겨준값;
	//Cast<ACPP_Cannon>(cannon)->SetBombDropLocation(rota);
	// 이를 위해 Rotator 구조체 하나 선언해서 하는게...
	// struct Rotator{
	// float Roll
	// ...
	// ...
	// } 이거 //코드 짜고 이제 필요없는 주석 설명은 지워주세요
}

void ACPP_Cobot_Car_Controller::FireCannonInput(const FInputActionValue& Value)
{
	if (mode == 1) {
		UE_LOG(LogTemp, Warning, TEXT("FireCannonInput %s"), Value.Get<bool>() ? TEXT("true") : TEXT("false"));
		Cast<ACPP_Cannon>(cannon)->FireLava();
	}
}

void ACPP_Cobot_Car_Controller::RotateInput(const FInputActionValue& Value)
{
	//컨트롤러가 아닌 플레이어에 있는 스프링암을 회전해야함
	//컨트롤러 회전은 서버에서 결정한다.
	player->SpringArm->AddRelativeRotation(FRotator(Value.Get<FVector2D>().Y, Value.Get<FVector2D>().X, 0.0f));
}

void ACPP_Cobot_Car_Controller::CarForward(float acceleration)
{
	UE_LOG(LogTemp, Warning, TEXT("CarForward"));
	player->AddActorWorldOffset(player->GetActorForwardVector()* acceleration);
}

void ACPP_Cobot_Car_Controller::CarRotation(float rotationValue)
{
	FRotator control_rotation = GetControlRotation();
	control_rotation.Yaw += rotationValue;
	SetControlRotation(control_rotation);
}

void ACPP_Cobot_Car_Controller::ChangeMode(int Mode)
{
	UE_LOG(LogTemp, Warning, TEXT("ChangeMode"));

	switch (Mode)
	{
		//일반 모드
		//카메라 설정 같은것도 필요함
	case 0:
		mode = 0;
		break;
		//대포 모드
	case 1:
		mode = 1;
		break;

	default:
		break;
	}
}





