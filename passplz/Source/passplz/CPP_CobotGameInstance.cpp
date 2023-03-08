// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_CobotGameInstance.h"

//struct ClientInfo {
//	int client_id;
//	double x, y, z;
//	int tm_id;
//	double tm_x, tm_y, tm_z;
//};

UCPP_CobotGameInstance::UCPP_CobotGameInstance()
{
	socket_mgr.ConnectServer();
	if (socket_mgr.ConnectServer())
	{
		is_connect = true;
		UE_LOG(LogTemp, Warning, TEXT("Success the server connect"));
	}
	else
	{
		is_connect = false;
		UE_LOG(LogTemp, Warning, TEXT("Fail the server connect"));
	}

	 SOCKET sock = socket_mgr.socket;

	if (sock == SOCKET_ERROR)
	{
		UE_LOG(LogTemp, Warning, TEXT("SOCKK"));
	}
}

//void UCPP_CobotGameInstance::SetInfo()
//{
//
//}
