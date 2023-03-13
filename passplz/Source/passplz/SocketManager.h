// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../../server/server/protocol.h"

#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/prewindowsapi.h"

#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")

#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"

/**
 * 
 */
class PASSPLZ_API SocketManager
{
public:
	SocketManager();
	~SocketManager();

private:
	SOCKET socket;

public:
	SOCKET* GetSocket()
	{
		return &socket;
	}

public:
	// char* ConvertWCtoC(wchar_t* str);
	bool ConnectServer();
};
