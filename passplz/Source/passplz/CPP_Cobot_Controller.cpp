// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_Cobot_Controller.h"
#include "CPP_Cobot.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "CPP_Maze_2S.h"
#include "CPP_GearKey_2S.h"
#include "CPP_Color_Forklift.h"
#include "CPP_Cobot.h"
#include "CPP_Time_Color_Button.h"
#include "CPP_Tutorial_Light.h"
#include <chrono>

ACPP_Cobot_Controller::ACPP_Cobot_Controller()
	: location(0.f, 0.f, 0.f)
	, tm_location(0.f, 0.f, 0.f)
	, tm_time_left(0.f)
	, tm_time_right(0.f)
	, prev_remain(0)
	, is_maze_button_push_forward(false)
	, is_maze_button_push_back(false)
	, is_maze_button_push_left(false)
	, is_maze_button_push_right(false)
{
}

ACPP_Cobot_Controller::~ACPP_Cobot_Controller()
{
	UE_LOG(LogTemp, Warning, TEXT("~ACPP_Cobot_Controller()"));

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
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay()"));

	instance = Cast<UCPP_CobotGameInstance>(GetWorld()->GetGameInstance());

	sock = instance->GetSocketMgr()->GetSocket();

	cs_enter_packet pack;
	pack.size = sizeof(pack);
	pack.type = static_cast<char>(packet_type::cs_enter);

	int ret = send(*sock, reinterpret_cast<char*>(&pack), sizeof(pack), 0);
	//SendEnterPacket();

	Player_2 = GetWorld()->SpawnActor<ACPP_Cobot>(ACPP_Cobot::StaticClass(), FVector(-7500.f, 3470.f, 500.f), FRotator(0.0f, 0.0f, 0.0f));
	Player_2->GetMesh()->SetVectorParameterValueOnMaterials(TEXT("cobot_color"), FVector(0.0f, 1.0f, 0.0f));
	Player_2->SetCollision();

	player = Cast<ACPP_Cobot>(GetPawn());
	if (!player)
		return;
	player->IsUnion_Jump_anim = false;
	previous_input = 0;
	current_input = 0;


	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACPP_Maze_2S::StaticClass(), maze_actor);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACPP_GearKey_2S::StaticClass(), gear_actor);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACPP_Color_Forklift::StaticClass(), forklift_actor);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACPP_Time_Color_Button::StaticClass(), board_actor);
}

void ACPP_Cobot_Controller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (is_maze_button_push_forward) {
	//    Cast<ACPP_Maze_2S>(arrOutActors[0])->target_forward();
	//}
	//if (is_maze_button_push_back) {
	//    Cast<ACPP_Maze_2S>(arrOutActors[0])->target_back();
	//}
	//if (is_maze_button_push_left) {
	//    Cast<ACPP_Maze_2S>(arrOutActors[0])->target_left();
	//}
	//if (is_maze_button_push_right) {
	//    Cast<ACPP_Maze_2S>(arrOutActors[0])->target_right();
	//}

	//if (IsUnion) {
	//    UE_LOG(LogTemp, Warning, TEXT("Union"));
	//    //합체 요청을 하는 곳, 스페이스 바를 계속 누르고 있으면 여기에 계속 들어온다.
	//    //여기서 서버에 상대 플레이어에게 합체 요청을 보내고 서버는 상대방에게 합체 요청을 보내줘야함
	//}
	//else {
	//    UE_LOG(LogTemp, Warning, TEXT("No Union"));
	//    //스페이스바를 누르지 않으면 여기에 계속 들어온다.
	//}
   /* FHitResult HitResult;
	FVector StartTraceLocation = player->Current_left + FVector(0.f, 0.f, 30.f);
	FVector EndTraceLocation = player->Current_left + FVector(0.f, 0.f, -500.f);
	GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartTraceLocation,
		EndTraceLocation,
		ECollisionChannel::ECC_Visibility
	);
	if (HitResult.IsValidBlockingHit()) {
		player->Current_left = HitResult.Location;
	}



	StartTraceLocation = player->Current_right + FVector(0.f, 0.f, 30.f);
	EndTraceLocation = player->Current_right + FVector(0.f, 0.f, -500.f);
	GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartTraceLocation,
		EndTraceLocation,
		ECollisionChannel::ECC_Visibility
	);
	if (HitResult.IsValidBlockingHit()) {
		player->Current_right = HitResult.Location;
	}
 */
 //UE_LOG(LogTemp, Warning, TEXT("tick: %d"), id);
	if (isClear) {
		FVector P2Location = Player_2->GetActorLocation();
		P2Location.Z = player->GetActorLocation().Z;
		Player_2->SetActorLocation(P2Location);
	}
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
	} else if (ret > BUF_SIZE - prev_remain) {
		UE_LOG(LogTemp, Warning, TEXT("in dat too big!!!!!!!!! recv data size: %d"), ret);
		prev_remain = 0;
		return;
	}

	if (prev_remain > 0) { // 만약 전에 남아있는 데이터가 있다면
		UE_LOG(LogTemp, Warning, TEXT("in prev remain!!!!!!!!!!!!!!"));
		strcat_s(prev_packet_buff, buff);
	} else {
		memcpy(prev_packet_buff, buff, ret);
	}
	int remain_data = ret + prev_remain;
	char* p = prev_packet_buff;
	while (remain_data > 0)
	{
		int packet_size = p[0];
		if (0 == packet_size) {
			UE_LOG(LogTemp, Warning, TEXT("packet size: 0!!!!!!!!!!!! ret: %d"), ret);
			prev_remain = 0;
			return;
		}

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

	/*char recv_buff[BUF_SIZE];

	int recv_ret = recv(*sock, reinterpret_cast<char*>(&recv_buff), BUF_SIZE, 0);
	if (recv_ret <= 0)
	   return;

	UE_LOG(LogTemp, Warning, TEXT("number of copies: %d"), ring_buff.number_of_copies);
	UE_LOG(LogTemp, Warning, TEXT("write pos: %d"), ring_buff.write_pos);
	UE_LOG(LogTemp, Warning, TEXT("read pos: %d"), ring_buff.read_pos);
	int enqueue_ret = ring_buff.enqueue(recv_buff, recv_ret);
	if (static_cast<int>(error::full_buffer) == enqueue_ret) {
	   UE_LOG(LogTemp, Warning, TEXT("full buffer"));
	   return;
	} else if (static_cast<int>(error::in_data_is_too_big) == enqueue_ret) {
	   UE_LOG(LogTemp, Warning, TEXT("in data is too big"));
	   return;
	} else if(static_cast<int>(error::no_seat_in_buffer) == enqueue_ret) {
	   UE_LOG(LogTemp, Warning, TEXT("no seat in buffer"));
	   return;
	}

	while ((ring_buff.remain_data() != 0 && ring_buff.peek_front() != 0) && (ring_buff.peek_front() <= ring_buff.remain_data()))
	{
	   char pack_size = ring_buff.peek_front();
	   char dequeue_data[BUFFER_SIZE] = {};

	   UE_LOG(LogTemp, Warning, TEXT("enqueue ret: %d"), enqueue_ret);
	   UE_LOG(LogTemp, Warning, TEXT("remain data: %d"), ring_buff.remain_data());
	   UE_LOG(LogTemp, Warning, TEXT("pack size: %d"), pack_size);
	   int dequeue_ret = ring_buff.dequeue(reinterpret_cast<char*>(&dequeue_data), pack_size);
	   if (static_cast<int>(error::no_data_in_buffer) == dequeue_ret) {
		  UE_LOG(LogTemp, Warning, TEXT("no data in buffer"));
		  break;
	   } else if (static_cast<int>(error::out_data_is_too_big) == dequeue_ret) {
		  UE_LOG(LogTemp, Warning, TEXT("out data is too buffer"));
		  break;
	   }
	   UE_LOG(LogTemp, Warning, TEXT("dequeue ret: %d"), dequeue_ret);

	   ProcessPacket(dequeue_data);
	}*/
}

void ACPP_Cobot_Controller::ProcessPacket(char* packet)
{
	switch (packet[1])
	{
	case static_cast<int>(packet_type::sc_enter):
	{
		sc_enter_packet* pack = reinterpret_cast<sc_enter_packet*>(packet);
		id = pack->id;

		UE_LOG(LogTemp, Warning, TEXT("recv login packet, my id: %d"), id);
	} break;
	case static_cast<int>(packet_type::sc_move):
	{
		sc_move_packet* pack = reinterpret_cast<sc_move_packet*>(packet);

		if (pack->client_id != id) { // 상대방이 움직였다
			tm_location = pack->location;

			if (direction::left == pack->direction) {
				Player_2->Current_left.X = pack->current.x;
				Player_2->Current_left.Y = pack->current.y;
				Player_2->Current_left.Z = pack->current.z;

				Player_2->Time_left = pack->time;
			}
			else if (direction::right == pack->direction) {
				Player_2->Current_right.X = pack->current.x;
				Player_2->Current_right.Y = pack->current.y;
				Player_2->Current_right.Z = pack->current.z;

				Player_2->Time_right = pack->time;
			}

			if (!Player_2) {
				UE_LOG(LogTemp, Warning, TEXT("Player_2 Spawn"));
				Player_2 = GetWorld()->SpawnActor<ACPP_Cobot>(ACPP_Cobot::StaticClass(), FVector(-7500.f, 3470.f, 500.f), FRotator(0.0f, 0.0f, 0.0f));
				Player_2->GetMesh()->SetVectorParameterValueOnMaterials(TEXT("cobot_color"), FVector(0.0f, 1.0f, 0.0f));
				Player_2->SetCollision();
			}

			Player_2->SetActorLocation(FVector(tm_location.x, tm_location.y, tm_location.z));
			Player_2->SetActorRotation(FRotator(0.f, pack->yaw, 0.f));

			//UE_LOG(LogTemp, Warning, TEXT("Recv Player2 move packet!"));
		}
		else { // 내가 움직였다

			//auto delay = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - pack->move_time;
			//UE_LOG(LogTemp, Warning, TEXT("Delay: %d"), delay);
		}
	} break;
	case static_cast<int>(packet_type::sc_login_fail):
	{
		// id나 pw가 다르다는 메시지 띄우기
	} break;
	case static_cast<int>(packet_type::sc_push_button_maze_forward):
	{
		UE_LOG(LogTemp, Warning, TEXT("push_forward"));
		if (maze_actor[0])
			Cast<ACPP_Maze_2S>(maze_actor[0])->target_forward();
		else
			UE_LOG(LogTemp, Warning, TEXT("No push_forward"));

		//TArray<AActor*> arrOutActors;
		//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACPP_Maze_2S::StaticClass(), arrOutActors);

		//Cast<ACPP_Maze_2S>(arrOutActors[0])->target_forward();
	} break;
	case static_cast<int>(packet_type::sc_push_button_maze_back):
	{
		UE_LOG(LogTemp, Warning, TEXT("push_back"));
		if (maze_actor[0])
			Cast<ACPP_Maze_2S>(maze_actor[0])->target_back();
		else
			UE_LOG(LogTemp, Warning, TEXT("No push_back"));

	} break;
	case static_cast<int>(packet_type::sc_push_button_maze_left):
	{
		UE_LOG(LogTemp, Warning, TEXT("push_left"));
		if (maze_actor[0])
			Cast<ACPP_Maze_2S>(maze_actor[0])->target_left();
		else
			UE_LOG(LogTemp, Warning, TEXT("No push_left"));

	} break;
	case static_cast<int>(packet_type::sc_push_button_maze_right):
	{
		UE_LOG(LogTemp, Warning, TEXT("push_right"));
		if (maze_actor[0])
			Cast<ACPP_Maze_2S>(maze_actor[0])->target_right();
		else
			UE_LOG(LogTemp, Warning, TEXT("No push_right"));

	} break;
	case static_cast<int>(packet_type::sc_push_button_gear_forward):
	{
		UE_LOG(LogTemp, Warning, TEXT("push_forward"));
		if (gear_actor[0])
			Cast<ACPP_GearKey_2S>(gear_actor[0])->target_forward();
		else
			UE_LOG(LogTemp, Warning, TEXT("No push_forward"));

		//TArray<AActor*> arrOutActors;
		//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACPP_Maze_2S::StaticClass(), arrOutActors);

		//Cast<ACPP_Maze_2S>(arrOutActors[0])->target_forward();
	} break;
	case static_cast<int>(packet_type::sc_push_button_gear_back):
	{
		UE_LOG(LogTemp, Warning, TEXT("push_back"));
		if (gear_actor[0])
			Cast<ACPP_GearKey_2S>(gear_actor[0])->target_back();
		else
			UE_LOG(LogTemp, Warning, TEXT("No push_back"));

	} break;
	case static_cast<int>(packet_type::sc_push_button_gear_left):
	{
		UE_LOG(LogTemp, Warning, TEXT("push_left"));
		if (gear_actor[0])
			Cast<ACPP_GearKey_2S>(gear_actor[0])->target_left();
		else
			UE_LOG(LogTemp, Warning, TEXT("No push_left"));

	} break;
	case static_cast<int>(packet_type::sc_push_button_gear_right):
	{
		UE_LOG(LogTemp, Warning, TEXT("push_right"));
		if (gear_actor[0])
			Cast<ACPP_GearKey_2S>(gear_actor[0])->target_right();
		else
			UE_LOG(LogTemp, Warning, TEXT("No push_right"));

	} break;
	case static_cast<int>(packet_type::sc_elevator):
	{
		Cast<ACPP_Elevator>(UGameplayStatics::GetActorOfClass(GetWorld(), ACPP_Elevator::StaticClass()))->ElevatorOperateCameraMoveLevelChange();
		UE_LOG(LogTemp, Warning, TEXT("sc_elevator"));
		player->GetMesh()->SetAnimInstanceClass(nullptr);
		Player_2->GetMesh()->SetAnimInstanceClass(nullptr);
		isClear = true;

	} break;
	case static_cast<int>(packet_type::sc_push_button_Forklift_red):
	{
		Cast<ACPP_Color_Forklift>(forklift_actor[0])->RecvColor(0);
	} break;
	case static_cast<int>(packet_type::sc_push_button_Forklift_green):
	{
		Cast<ACPP_Color_Forklift>(forklift_actor[0])->RecvColor(1);
	} break;
	case static_cast<int>(packet_type::sc_push_button_Forklift_blue):
	{
		Cast<ACPP_Color_Forklift>(forklift_actor[0])->RecvColor(2);
	} break;
	case static_cast<int>(packet_type::sc_push_button_Forklift_black):
	{
		Cast<ACPP_Color_Forklift>(forklift_actor[0])->RecvColor(3);
	} break;
	case static_cast<int>(packet_type::sc_push_button_cobot_red):
	{
		//Cast<ACPP_Cobot>(cobot_actor[0])->RecvColor(0);
		player->RecvColor(0);
		Player_2->RecvColor(0);
	} break;
	case static_cast<int>(packet_type::sc_push_button_cobot_green):
	{
		player->RecvColor(1);
		Player_2->RecvColor(1);
	} break;
	case static_cast<int>(packet_type::sc_push_button_cobot_blue):
	{
		player->RecvColor(2);
		Player_2->RecvColor(2);
	} break;
	case static_cast<int>(packet_type::sc_push_button_cobot_black):
	{
		player->RecvColor(3);
		Player_2->RecvColor(3);
	} break;
	case static_cast<int>(packet_type::sc_board_color):
	{
		sc_board_color_packet* pack = reinterpret_cast<sc_board_color_packet*>(packet);
		UE_LOG(LogTemp, Warning, TEXT("packet_type::sc_board_color, color: %d"), pack->color);
		if (pack->id == id) {
			Cast<ACPP_Time_Color_Button>(board_actor[0])->RecvAndSendColor(pack->color);
		}
		else {
			Cast<ACPP_Time_Color_Button>(board_actor[0])->OnlyRecvColor(pack->color);
		}
	} break;
	case static_cast<int>(packet_type::sc_logout):
	{
		UE_LOG(LogTemp, Warning, TEXT("packet_type::sc_logout"));
	} break;
	case static_cast<int>(packet_type::sc_esc):
	{
		sc_esc_packet* pack = reinterpret_cast<sc_esc_packet*>(packet);
		if (pack->stage == 1) {

			FOutputDeviceNull pAR;
			Cast<ACPP_Tutorial_Light>(UGameplayStatics::GetActorOfClass(GetWorld(), ACPP_Tutorial_Light::StaticClass()))->clearActor->CallFunctionByNameWithArguments(TEXT("GameEnd"), pAR, nullptr, true);
		}
		else if (pack->stage == 2) {
			FOutputDeviceNull pAR;
			Cast<ACPP_Time_Color_Button>(UGameplayStatics::GetActorOfClass(GetWorld(), ACPP_Time_Color_Button::StaticClass()))->clearActor->CallFunctionByNameWithArguments(TEXT("GameEnd"), pAR, nullptr, true);
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
	wcscpy_s(enter_pack.id, MAX_NAME, L"ex_id");
	wcscpy_s(enter_pack.passward, MAX_NAME, L"ex_pw");

	int ret = send(*sock, reinterpret_cast<char*>(&enter_pack), sizeof(enter_pack), 0);
}

void ACPP_Cobot_Controller::SendMovePacket(direction direction)
{
	cs_move_packet pack;
	pack.size = sizeof(pack);
	pack.type = static_cast<char>(packet_type::cs_move);
	pack.direction = direction;
	pack.yaw = player->GetActorRotation().Yaw;
	pack.location.x = player->GetActorLocation().X;
	pack.location.y = player->GetActorLocation().Y;
	pack.location.z = player->GetActorLocation().Z;

	if (direction::left == direction) {
		pack.time = player->Time_left;
		pack.current.x = player->Current_left.X;
		pack.current.y = player->Current_left.Y;
		pack.current.z = player->Current_left.Z;
	}
	else if (direction::right == direction) {
		pack.time = player->Time_right;
		pack.current.x = player->Current_right.X;
		pack.current.y = player->Current_right.Y;
		pack.current.z = player->Current_right.Z;
	}

	int ret = send(*sock, reinterpret_cast<char*>(&pack), sizeof(pack), 0);
	UE_LOG(LogTemp, Warning, TEXT("send move packet ret: %d"), ret);
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

	//회전
	InputComponent->BindAxis(TEXT("ROTATE"), this, &ACPP_Cobot_Controller::Turn);
	InputComponent->BindAxis(TEXT("LookUp"), this, &ACPP_Cobot_Controller::LookUp);

	//A, D키
	InputComponent->BindAxis(TEXT("LEFT_RIGHT"), this, &ACPP_Cobot_Controller::Left_Right);

	//합체
	InputComponent->BindAction(TEXT("COBOT_UNION"), IE_Pressed, this, &ACPP_Cobot_Controller::Union_Pressed);
	InputComponent->BindAction(TEXT("COBOT_UNION"), IE_Released, this, &ACPP_Cobot_Controller::Union_Released);

	//달리기
	InputComponent->BindAction(TEXT("RUN"), IE_Pressed, this, &ACPP_Cobot_Controller::Run_Pressed);
	InputComponent->BindAction(TEXT("RUN"), IE_Released, this, &ACPP_Cobot_Controller::Run_Released);
}

void ACPP_Cobot_Controller::Turn(float NewAxisValue)
{
	float delta_time = GetWorld()->GetDeltaSeconds();
	AddYawInput(delta_time * NewAxisValue * 20.0f);
	FRotator control_rotate = GetControlRotation();
	//  SetControlRotation(FRotator(control_rotate.Pitch, UKismetMathLibrary::ClampAngle(control_rotate.Yaw, rotate_min, rotate_max), control_rotate.Roll));
}

void ACPP_Cobot_Controller::LookUp(float NewAxisValue)
{
	float delta_time = GetWorld()->GetDeltaSeconds();
	AddPitchInput(delta_time * NewAxisValue * 20.0f);
}

void ACPP_Cobot_Controller::Union_Pressed()
{
	player->IsUnion_Jump_anim = (!(player->IsUnion_Jump_anim));
}

void ACPP_Cobot_Controller::Union_Released()
{
	// IsUnion_Jump_anim = false;
}

void ACPP_Cobot_Controller::Run_Pressed()
{
	player->IsFast_run = true;
	player->AnimatiomChange(1);
}

void ACPP_Cobot_Controller::Run_Released()
{
	player->IsFast_run = false;
	player->AnimatiomChange(0);
}

void ACPP_Cobot_Controller::SendEsc()
{
	cs_esc_packet pack;
	pack.size = sizeof(pack);
	pack.type = static_cast<char>(packet_type::cs_esc);

	int ret = send(*sock, reinterpret_cast<char*>(&pack), sizeof(pack), 0);
}

void ACPP_Cobot_Controller::Left_Right(float NewAxisValue)
{
	//발 충돌 박스 위치 업데이트
	player->Foot_left_Zone->SetWorldLocation(player->GetMesh()->GetSocketLocation("left"));
	player->Foot_right_Zone->SetWorldLocation(player->GetMesh()->GetSocketLocation("right"));

	if (player->IsUnion_Jump_anim) {
		//위치를 계속 보내는거 필요(pawn위치만)
		return;
	}

	// 이동하기 전에 캐릭터의 상태를 계산해서 보폭과 걸음 속도를 설정한다.
	float distance_two_feet = 50.f;
	if (60.f > (player->Current_left - player->Current_right).Size()) {
		distance_two_feet = 50.f;
		player->GetCharacterMovement()->MaxWalkSpeed = 200.f;
	}
	else {
		distance_two_feet = 0.f;
		player->GetCharacterMovement()->MaxWalkSpeed = 0.f;
	}

	FRotator rotator_controller = GetControlRotation();
	FRotator rotator_forward = UKismetMathLibrary::MakeRotator(0.0f, 0.0f, rotator_controller.Yaw);
	FVector forward_vector = UKismetMathLibrary::GetForwardVector(rotator_forward);


	if (((int)NewAxisValue) == 1) { // A key
		player->Start_right = player->Current_right;
		player->Time_right = 0.f;
		player->Time_left += (GetWorld()->GetDeltaSeconds() * 2);
		if (player->Time_left < 1.0f) {
			//캐릭터를 움직인다.
			player->AddMovementInput(forward_vector, 1.0f);

			//목표 왼발 위치를 정한다.
			player->Target_left = player->GetActorLocation() +
				forward_vector * 50.f;


			//현재 발의 위치를 업데이트 한다.
			float curvevalue = FMath::Sin(FMath::DegreesToRadians(player->Time_left * 180.f)) * 0.8f;
			player->Current_left = (player->GetActorRightVector() * -curvevalue * distance_two_feet) + (UKismetMathLibrary::VLerp(player->Start_left, player->Target_left, player->Time_left));

			//UE_LOG(LogTemp, Warning, TEXT("Target_left %f"), player->Target_left.Z);
			//UE_LOG(LogTemp, Warning, TEXT("Current_left %f"), player->Current_left.Z);

			//FHitResult HitResult;
			//FVector StartTraceLocation = player->Current_left + FVector(0.f, 0.f, 30.f);
			//FVector EndTraceLocation = player->Current_left + FVector(0.f, 0.f, -500.f);

			//트레이스로 다음 발의 위치를 찾는다.
			//GetWorld()->LineTraceSingleByChannel(
			//    HitResult,
			//    StartTraceLocation,
			//    EndTraceLocation,
			//    ECollisionChannel::ECC_Visibility
			//);
			////그 값을 현재 발 위치로 넣어준다.
			//if(HitResult.IsValidBlockingHit())
			//    player->Current_left = HitResult.Location;

		  //  UE_LOG(LogTemp, Warning, TEXT("%f"), HitResult.Location.Z);

			SendMovePacket(direction::left);

		}
	}
	else {
		if (player->Start_left != player->Current_left) {
			player->Start_left = player->Current_left;
			player->Target_left = player->Current_left;
			player->Time_left = 0.f;

			SendMovePacket(direction::left);

		}

		if (((int)NewAxisValue) == 2) { // D key
			player->Start_left = player->Current_left;
			player->Time_left = 0.f;
			player->Time_right += (GetWorld()->GetDeltaSeconds() * 2);
			if (player->Time_right < 1.0f) {
				//캐릭터를 움직인다.
				player->AddMovementInput(forward_vector, 1.0f);

				//목표 왼발 위치를 정한다.
				player->Target_right = player->GetActorLocation() +
					forward_vector * 50.f;


				//현재 발의 위치를 업데이트 한다.
				float curvevalue = FMath::Sin(FMath::DegreesToRadians(player->Time_right * 180.f)) * 0.8f;
				player->Current_right = (player->GetActorRightVector() * curvevalue * distance_two_feet) + (UKismetMathLibrary::VLerp(player->Start_right, player->Target_right, player->Time_right));

				FHitResult HitResult;
				FVector StartTraceLocation = player->Current_right + FVector(0.f, 0.f, 30.f);
				FVector EndTraceLocation = player->Current_right + FVector(0.f, 0.f, -500.f);

				/*            GetWorld()->LineTraceSingleByChannel(
								HitResult,
								StartTraceLocation,
								EndTraceLocation,
								ECollisionChannel::ECC_Visibility
							);
							if (HitResult.IsValidBlockingHit())
								player->Current_right = HitResult.Location;*/


				SendMovePacket(direction::right);


			}
		}
		else {
			if (player->Start_right != player->Current_right) {
				player->Start_right = player->Current_right;
				player->Start_right = player->Current_right;

				player->Time_right = 0.f;

				SendMovePacket(direction::right);

			}
		}
	}

	previous_input = current_input;
	current_input = ((int)NewAxisValue);
	if (current_input != previous_input) {
		rotate_min = GetControlRotation().Yaw - 90.f;
		rotate_max = GetControlRotation().Yaw + 90.f;
		// UE_LOG(LogTemp, Warning, TEXT("current_input"));

	}
}

bool ACPP_Cobot_Controller::Is_Set_IDPW(FString I, FString p)
{
	///*서버! 여기에 서버에 send해줘야함 false라면 바로 리턴 true면 아래 작업 후 리턴*/
	////ID = I;
	////Passward = p;

	//wchar_t* input_id = TCHAR_TO_WCHAR(*I);
	//wchar_t* input_pw = TCHAR_TO_WCHAR(*p);

	//// 서버한테 들어왔다고 알려주는 거
	//cs_login_packet login_pack;
	//login_pack.size = sizeof(login_pack);
	//login_pack.type = static_cast<char>(packet_type::cs_login);
	//wcscpy_s(login_pack.id, MAX_NAME, input_id);
	//wcscpy_s(login_pack.passward, MAX_NAME, input_pw);

	//int ret = send(*sock, reinterpret_cast<char*>(&login_pack), sizeof(login_pack), 0);

	// UE_LOG(LogTemp, Warning, TEXT("ID: %s, PW: %s"), input_id, input_pw);

	return true;
}

