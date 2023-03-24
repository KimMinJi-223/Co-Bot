// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_Cobot_Controller.h"
#include "CPP_Cobot.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"

#include "../../../server/server/protocol.h"

#include <chrono>

ACPP_Cobot_Controller::ACPP_Cobot_Controller()
	: x(-1.f), y(-1.f), z(-1.f), yaw(-1.f)
	, tm_x(-1.f), tm_y(-1.f), tm_z(-1.f), tm_yaw(-1.f)
	, prev_remain(0)
{
}

ACPP_Cobot_Controller::~ACPP_Cobot_Controller()
{
	//cs_logout_packet pack;
	//pack.size = sizeof(pack);
	//pack.type = static_cast<int>(type::cs_logout);

	//send(sock, reinterpret_cast<char*>(&pack), sizeof(pack), 0);

	//closesocket(sock);
	//WSACleanup();
}

void ACPP_Cobot_Controller::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("코봇생성"));

    instance = Cast<UCPP_CobotGameInstance>(GetWorld()->GetGameInstance());

	sock = instance->GetSocketMgr()->GetSocket();

    SendEnterPacket();

    Player_2 = GetWorld()->SpawnActor<ACPP_Cobot>(ACPP_Cobot::StaticClass(), FVector(tm_x, tm_y, tm_z), FRotator(0.0f, tm_yaw, 0.0f));
}

void ACPP_Cobot_Controller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    RecvPacket();
}

void ACPP_Cobot_Controller::RecvPacket()
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
	}
	else
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
		}
		else break;
	}

	prev_remain = remain_data;

	if (remain_data > 0)
	{
		memcpy(prev_packet_buff, p, remain_data);
	}

	//ring_buff.enqueue(buff, ret);

	//int read_point = 0;
	//while (buff[read_point] <= ring_buff.diff() && !ring_buff.empty())
	//{
	//    ProcessPacket(buff);
	//    ring_buff.move_read_pos(buff[0]);
	//    read_point = buff[read_point];
	//}
}

void ACPP_Cobot_Controller::ProcessPacket(char* packet)
{
	switch (packet[1])
	{
	case static_cast<int>(packet_type::sc_login):
	{
		sc_login_packet* pack = reinterpret_cast<sc_login_packet*>(packet);
		id = pack->id;
		x = pack->x;
		y = pack->y;
		z = pack->z;
		yaw = pack->yaw;
		tm_x = pack->tm_x;
		tm_y = pack->tm_y;
		tm_z = pack->tm_z;
		tm_yaw = pack->tm_yaw;

		UE_LOG(LogTemp, Warning, TEXT("recv login packet"));
	} break;
	case static_cast<int>(packet_type::sc_move):
	{
		sc_move_packet* pack = reinterpret_cast<sc_move_packet*>(packet);
		if (pack->client_id != id) {
			tm_x = pack->x;
			tm_y = pack->y;
			tm_z = pack->z;
			tm_yaw = pack->yaw;

			Player_2->SetActorLocation(FVector(tm_x, tm_y, tm_z));
			Player_2->SetActorRotation(FRotator(0.f, tm_yaw, 0.f));

            UE_LOG(LogTemp, Warning, TEXT("Recv Player2 move packet!"));
        } else {
            auto delay = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - pack->move_time;
            UE_LOG(LogTemp, Warning, TEXT("Delay: %d"), delay);
        }        
    } break;
    }
}

void ACPP_Cobot_Controller::SendEnterPacket()
{
    //cs_enter_packet enter_pack;
    //enter_pack.size = sizeof(enter_pack);
    //enter_pack.type = static_cast<char>(packet_type::cs_enter);

    /* 로그인을 하기 전 임시로 구현해놓음. 이거 안하면 MATCHING이 안됨 */
    cs_login_packet enter_pack;
    enter_pack.size = sizeof(enter_pack);
    enter_pack.type = static_cast<char>(packet_type::cs_login);

    int ret = send(*sock, reinterpret_cast<char*>(&enter_pack), sizeof(enter_pack), 0);
}

void ACPP_Cobot_Controller::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ACPP_Cobot_Controller::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	UE_LOG(LogTemp, Warning, TEXT("OnPossess"));
}

void ACPP_Cobot_Controller::SetupInputComponent()
{
	Super::SetupInputComponent();

	// W, S 키 눌리면 Move_Forward() 함수를 실행
	InputComponent->BindAxis(TEXT("MOVE_FORWARD"), this, &ACPP_Cobot_Controller::Move_Forward);
	InputComponent->BindAxis(TEXT("ROTATE"), this, &ACPP_Cobot_Controller::Rotate);

	//A, D키
	InputComponent->BindAxis(TEXT("LEFT_RIGHT"), this, &ACPP_Cobot_Controller::Left_Right);

}

void ACPP_Cobot_Controller::Move_Forward(float NewAxisValue)
{
	new_axis_value = NewAxisValue;

	player = Cast<ACPP_Cobot>(GetPawn());

	if (!player)
		return;

    FRotator rotator_controller = GetControlRotation();
    FRotator rotator_forward = UKismetMathLibrary::MakeRotator(0.0f, 0.0f, rotator_controller.Yaw);
    FVector forward_vector = UKismetMathLibrary::GetForwardVector(rotator_forward);

    player->AddMovementInput(forward_vector, NewAxisValue);



}

void ACPP_Cobot_Controller::Rotate(float NewAxisValue)
{
	float delta_time = GetWorld()->GetDeltaSeconds();
	AddYawInput(delta_time * NewAxisValue * 20.0f);
}

void ACPP_Cobot_Controller::Left_Right(float NewAxisValue)
{

	player = Cast<ACPP_Cobot>(GetPawn());

	if (!player)
		return;

	FRotator rotator_controller = GetControlRotation();
	FRotator rotator_forward = UKismetMathLibrary::MakeRotator(0.0f, 0.0f, rotator_controller.Yaw);
	FVector forward_vector = UKismetMathLibrary::GetForwardVector(rotator_forward);

	float distance_two_feet = 50.f;

	if (((int)NewAxisValue) == 1) {
		player->Time_left += GetWorld()->GetDeltaSeconds();
		if (player->Time_left < 1.0f) {
			//캐릭터를 움직인다.
			player->AddMovementInput(forward_vector, 1.0f);

			//목표 왼발 위치를 정한다.
			player->Target_left = player->GetActorLocation() +
				player->GetActorForwardVector() * 50.f;
			player->Target_left.Z -= -50.f;
			//UE_LOG(LogTemp, Warning, TEXT("Target_left %f"), player->Target_left.X);

			//현재 발의 위치를 업데이트 한다.
			float curvevalue = Cobot_Curve->GetFloatValue(player->Time_left);
			player->Current_left = (player->GetActorRightVector() * -curvevalue * distance_two_feet) + (UKismetMathLibrary::VLerp(player->Start_left, player->Target_left, player->Time_left));
			UE_LOG(LogTemp, Warning, TEXT("Time %f"), curvevalue);
			UE_LOG(LogTemp, Warning, TEXT("%f"), player->Current_left.Z);

			FHitResult HitResult;
			FVector StartTraceLocation = player->Current_left + FVector(0.f, 0.f, 50.f);
			FVector EndTraceLocation = player->Current_left + FVector(0.f, 0.f, -500.f);
			//UE_LOG(LogTemp, Warning, TEXT("%f"), player->Current_left.X);

			GetWorld()->LineTraceSingleByChannel(
				HitResult,
				StartTraceLocation,
				EndTraceLocation,
				ECollisionChannel::ECC_Visibility
			);
			//if(HitResult.IsValidBlockingHit())
				player->Current_left = HitResult.Location;
			UE_LOG(LogTemp, Warning, TEXT("%f"), HitResult.Location.Z);
		}
	}
	else {
		player->Start_left = player->Current_left;
		player->Time_left = 0.f;

		if (((int)NewAxisValue) == 2) {
			player->Time_right += GetWorld()->GetDeltaSeconds();
			if (player->Time_right < 1.0f) {
				//캐릭터를 움직인다.
				player->AddMovementInput(forward_vector, 1.0f);

				//목표 왼발 위치를 정한다.
				player->Target_right = player->GetActorLocation() +
					player->GetActorForwardVector() * 50.f;
				player->Target_right.Z -= -50.f;
				//UE_LOG(LogTemp, Warning, TEXT("Target_left %f"), player->Target_right.X);

				//현재 발의 위치를 업데이트 한다.
				float curvevalue = Cobot_Curve->GetFloatValue(player->Time_right);
				player->Current_right = (player->GetActorRightVector() * curvevalue * distance_two_feet) + (UKismetMathLibrary::VLerp(player->Start_right, player->Target_right, player->Time_right));

				FHitResult HitResult;
				FVector StartTraceLocation = player->Current_right + FVector(0.f, 0.f, 50.f);
				FVector EndTraceLocation = player->Current_right + FVector(0.f, 0.f, -500.f);
				//UE_LOG(LogTemp, Warning, TEXT("%f"), player->Current_right.X);

				GetWorld()->LineTraceSingleByChannel(
					HitResult,
					StartTraceLocation,
					EndTraceLocation,
					ECollisionChannel::ECC_Visibility
				);
				player->Current_right = HitResult.Location;
			}
		}
		else {
			player->Start_right = player->Current_right;
			player->Time_right = 0.f;
		}






	}
}

bool ACPP_Cobot_Controller::Is_Set_IDPW(FString I, FString p)
{
	///*서버! 여기에 서버에 send해줘야함 false라면 바로 리턴 true면 아래 작업 후 리턴*/
	////ID = I;
	////Passward = p;

	//wchar_t* input_id = TCHAR_TO_WCHAR(*I);
	//wchar_t* input_pw = TCHAR_TO_WCHAR(*p);

	//if (!instance->is_connect)
	//{
	//    instance->socket_mgr.ConnectServer(input_id);
	//    sock = instance->socket_mgr.socket;
	//    instance->is_connect = true;
	//}

	//// 서버한테 들어왔다고 알려주는 거
	//cs_login_packet login_pack;
	//login_pack.size = sizeof(login_pack);
	//login_pack.type = static_cast<char>(type::cs_login);
	////wcscpy_s(login_pack.id, MAX_LOGIN_LEN, input_id);
	////wcscpy_s(login_pack.passward, MAX_LOGIN_LEN, input_pw);

	//int ret = send(sock, reinterpret_cast<char*>(&login_pack), sizeof(login_pack), 0);


    //UE_LOG(LogTemp, Warning, TEXT("ID: %s, PW: %s"), input_id, input_pw);
    return true; 
}