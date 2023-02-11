// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../server/server/RingBuffer.h"

#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/prewindowsapi.h"

#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")

#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPP_Cobot_Controller.generated.h"

/**
 * 
 */
UCLASS()
class PASSPLZ_API ACPP_Cobot_Controller : public APlayerController
{
	GENERATED_BODY()

	ACPP_Cobot_Controller();

private:
	SOCKET sock;
	double x, y, z;
	RingBuffer ring_buff;
	// TMap<int, AActor*> actorList;

private:
	void RecvPacket();
	void ProcessPacket(char* packet);

public:
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;
protected:
	virtual void SetupInputComponent() override;

private:
	void Move_Forward(float NewAxisValue);
	void Rotate(float NewAxisValue);
	
};
