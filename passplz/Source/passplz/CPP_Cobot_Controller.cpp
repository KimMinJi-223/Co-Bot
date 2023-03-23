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
        } else break;
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
}

void ACPP_Cobot_Controller::Move_Forward(float NewAxisValue)
{
    new_axis_value = NewAxisValue;

    player = GetPawn();

    if (!player)
        return;

    FRotator rotator_controller = GetControlRotation();
    FRotator rotator_forward = UKismetMathLibrary::MakeRotator(0.0f, 0.0f, rotator_controller.Yaw);
    FVector forward_vector = UKismetMathLibrary::GetForwardVector(rotator_forward);

    player->AddMovementInput(forward_vector, NewAxisValue);

    if (x != player->GetActorLocation().X || y != player->GetActorLocation().Y || z != player->GetActorLocation().Z || yaw != player->GetActorRotation().Yaw)
    {
        x = player->GetActorLocation().X;
        y = player->GetActorLocation().Y;
        z = player->GetActorLocation().Z;
        yaw = player->GetActorRotation().Yaw;

        last_move_time = std::chrono::high_resolution_clock::now();
        cs_move_packet pack;
        pack.size = sizeof(pack);
        pack.type = static_cast<char>(packet_type::cs_move);
        pack.x = x;
        pack.y = y;
        pack.z = z;
        pack.yaw = yaw;
        pack.move_time = static_cast<unsigned>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());

        int ret = send(*sock, reinterpret_cast<char*>(&pack), sizeof(pack), 0);
    }
}

void ACPP_Cobot_Controller::Rotate(float NewAxisValue)
{
    float delta_time = GetWorld()->GetDeltaSeconds();
    AddYawInput(delta_time * NewAxisValue * 20.0f);
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