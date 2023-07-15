// Fill out your copyright notice in the Description page of Project Settings.
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "SocketManager.h"
#include <fstream>

SocketManager::SocketManager()
{
}

SocketManager::~SocketManager()
{
}

//char* SocketManager::ConvertWCtoC(wchar_t* str) //wchar을 char로 변경
//{
//    char* pStr;
//    int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
//    pStr = new char[strSize];
//    WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0, 0);
//
//    return pStr;
//}

bool SocketManager::ConnectServer()
{
    UE_LOG(LogTemp, Warning, TEXT("Start connect server!"));

    FString ip_path = FPaths::ProjectUserDir();
    ip_path.Append(TEXT("server_ip.txt"));

    char ip[30];
    std::ifstream ip_file;             //읽을 목적의 파일 선언
    ip_file.open(*ip_path);    //파일 열기
    if (ip_file.is_open())    //파일이 열렸는지 확인
    {
        while (!ip_file.eof())    //파일 끝까지 읽었는지 확인
        {
            ip_file.getline(ip, 30);    //한줄씩 읽어오기
        }
    }
    ip_file.close();    //파일 닫기

    // 윈속 초기화
    WSADATA wsaData;
    int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);   // Winsock 초기화
    if (nRet != 0) return false;

    // 소켓 생성
    socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (INVALID_SOCKET == socket) return false;

    // IP, Port 정보 입력
    SOCKADDR_IN server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(7000);
    server_addr.sin_addr.s_addr = inet_addr(ip);
    //stServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 접속
    nRet = connect(socket, (sockaddr*)&server_addr, sizeof(sockaddr));
    if (nRet == SOCKET_ERROR) return false;

    // 비동기를 위한 코드
    u_long nonBlockingMode = 1;
    ioctlsocket(socket, FIONBIO, &nonBlockingMode); // sock을 논블로킹 모드로 설정

    //cs_login_packet login_pack;
    //login_pack.size = sizeof(login_pack);
    //login_pack.type = static_cast<char>(type::cs_login);

    //int ret = send(socket, reinterpret_cast<char*>(&login_pack), sizeof(login_pack), 0);

    UE_LOG(LogTemp, Warning, TEXT("Success server connect"));

    return true;
}

