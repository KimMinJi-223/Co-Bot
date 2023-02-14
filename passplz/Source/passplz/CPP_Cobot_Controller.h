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
	int	id;
	RingBuffer ring_buff;
	// TMap<int, AActor*> actorList;

	double p2_x, p2_y, p2_z;

private:
	virtual void Tick(float DeltaTime) override;
	void RecvPacket();
	void ProcessPacket(char* packet);

protected:
	virtual void BeginPlay() override;
public:
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;
protected:
	virtual void SetupInputComponent() override;

private:
	void Move_Forward(float NewAxisValue);
	void Rotate(float NewAxisValue);

	class ACPP_Cobot* Player_2; //상대 플레이어 정보

public:
	bool Is_Set_IDPW(FString I, FString p);

protected:
	UPROPERTY(BlueprintReadWrite)
		FString ID; //아이디
	UPROPERTY(BlueprintReadWrite)
		FString Passward; //비밀번호
};