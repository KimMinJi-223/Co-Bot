// Fill out your copyright notice in the Description page of Project Settings.


#include "SocketManager.h"

SocketManager::SocketManager()
{
}

SocketManager::~SocketManager()
{
}

bool SocketManager::ConnectServer()
{
    UE_LOG(LogTemp, Warning, TEXT("Start connect server!"));

    // 윈속 초기화
    WSADATA wsaData;
    int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);   // Winsock 초기화
    if (nRet != 0) return false;

    // 소켓 생성
    socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (INVALID_SOCKET == socket) return false;

    // IP, Port 정보 입력
    SOCKADDR_IN stServerAddr;
    stServerAddr.sin_family = AF_INET;
    stServerAddr.sin_port = htons(7000);
    stServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 접속
    nRet = connect(socket, (sockaddr*)&stServerAddr, sizeof(sockaddr));
    if (nRet == SOCKET_ERROR) return false;

    u_long nonBlockingMode = 1;
    ioctlsocket(socket, FIONBIO, &nonBlockingMode); // sock을 논블로킹 모드로 설정

    //cs_login_packet login_pack;
    //login_pack.size = sizeof(login_pack);
    //login_pack.type = static_cast<char>(type::cs_login);

    //int ret = send(socket, reinterpret_cast<char*>(&login_pack), sizeof(login_pack), 0);

    UE_LOG(LogTemp, Warning, TEXT("Success server connect"));

    return true;
}

