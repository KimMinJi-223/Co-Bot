// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_Cobot_Controller.h"
#include "CPP_Cobot.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"

#include "../../../server/server/protocol.h"

ACPP_Cobot_Controller::ACPP_Cobot_Controller()
    : x(0.f), y(0.f), z(0.f), yaw(0.f)
    , tm_x(0.f), tm_y(0.f), tm_z(0.f), tm_yaw(0.f)
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

    // 현재 game instance 받아온다
    instance = Cast<UCPP_CobotGameInstance>(GetWorld()->GetGameInstance());

    sock = instance->socket_mgr.socket;

    //++instance->level;

    //if (1 == instance->level)
    //{
    //    cs_login_packet login_pack;
    //    login_pack.size = sizeof(login_pack);
    //    login_pack.type = static_cast<char>(type::cs_login);

    //    int ret = send(sock, reinterpret_cast<char*>(&login_pack), sizeof(login_pack), 0);
    //}
    //else if (2 == instance->level)
    //{
    //    cs_level2_packet level_pack;
    //    level_pack.size = sizeof(level_pack);
    //    level_pack.type = static_cast<char>(type::cs_level2);
    //    level_pack.client_id = id;

    //    int ret = send(sock, reinterpret_cast<char*>(&level_pack), sizeof(level_pack), 0);
    //}

    cs_login_packet login_pack;
    login_pack.size = sizeof(login_pack);
    login_pack.type = static_cast<char>(type::cs_login);

    int ret = send(sock, reinterpret_cast<char*>(&login_pack), sizeof(login_pack), 0);

    Player_2 = GetWorld()->SpawnActor<ACPP_Cobot>(ACPP_Cobot::StaticClass(), FVector(tm_x, tm_y, tm_z), FRotator(0.0f, tm_yaw, 0.0f));
}

void ACPP_Cobot_Controller::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    RecvPacket();
    //UE_LOG(LogTemp, Warning, TEXT("Tick"));
}

void ACPP_Cobot_Controller::RecvPacket()
{
    char buff[BUF_SIZE];

    int ret = recv(sock, reinterpret_cast<char*>(&buff), BUF_SIZE, 0);
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
    case static_cast<int>(type::sc_login):
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
    case static_cast<int>(type::sc_move):
    {
        sc_move_packet* pack = reinterpret_cast<sc_move_packet*>(packet);
        if (pack->client_id != id) {
            tm_x = pack->x;
            tm_y = pack->y;
            tm_z = pack->z;
            tm_yaw = pack->yaw;

            Player_2->SetActorLocation(FVector(tm_x, tm_y, tm_z));
            Player_2->SetActorRotation(FRotator(0.f, tm_yaw, 0.f));
            double zz;
            zz = Player_2->GetActorLocation().Z;
            UE_LOG(LogTemp, Warning, TEXT("%d, %lf"), pack->client_id, zz);
            //UE_LOG(LogTemp, Warning, TEXT("recv p2 move packet"));
        } else {
            x = pack->x;
            y = pack->y;
            z = pack->z;
            yaw = pack->yaw;
            //UE_LOG(LogTemp, Warning, TEXT("recv my move packet"));
        }        
    } break;
    }
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

    APawn* player = GetPawn();

    if (!player)
        return;

    FRotator rotator_controller = GetControlRotation();
    FRotator rotator_forward = UKismetMathLibrary::MakeRotator(0.0f, 0.0f, rotator_controller.Yaw);
    FVector forward_vector = UKismetMathLibrary::GetForwardVector(rotator_forward);

    player->AddMovementInput(forward_vector, NewAxisValue);

    // 서버로 움직임 보냄
 // 매프레임 여기를 지나감
 // 따라서 움직임이 바뀌었을 때만 보내도록 함
 /*
    여기는 일단 내가 움직이는 거 위치 받아오려고 해둔 곳
    추후에 어디가 움직이는 곳인지 보고 고쳐야 함
 */

    //double yaw1 = player->GetActorRotation().Yaw;

    //UE_LOG(LogTemp, Warning, TEXT("Yaw: %lf"), yaw1);

    if (x != player->GetActorLocation().X || y != player->GetActorLocation().Y || z != player->GetActorLocation().Z || yaw != player->GetActorRotation().Yaw)
    {
        x = player->GetActorLocation().X;
        y = player->GetActorLocation().Y;
        z = player->GetActorLocation().Z;
        yaw = player->GetActorRotation().Yaw;

        cs_move_packet pack;
        pack.size = sizeof(pack);
        pack.type = static_cast<char>(type::cs_move);
        pack.x = x;
        pack.y = y;
        pack.z = z;
        pack.yaw = yaw;

        int ret = send(sock, reinterpret_cast<char*>(&pack), sizeof(pack), 0);

        // 이게 패킷 받는거
        // 패킷 받을 곳에 복붙해서 놓으면 됨
        //RecvPacket();
    }


    //RecvPacket();
}

void ACPP_Cobot_Controller::Rotate(float NewAxisValue)
{
    float delta_time = GetWorld()->GetDeltaSeconds();
    AddYawInput(delta_time * NewAxisValue * 20.0f);
}

bool ACPP_Cobot_Controller::Is_Set_IDPW(FString I, FString p)
{
    /*서버! 여기에 서버에 send해줘야함 false라면 바로 리턴 true면 아래 작업 후 리턴*/
    //ID = I;
    //Passward = p;

    //const wchar_t* input_id = TCHAR_TO_WCHAR(*I);
    //const wchar_t* input_pw = TCHAR_TO_WCHAR(*p);

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
