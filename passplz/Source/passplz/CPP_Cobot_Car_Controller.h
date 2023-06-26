// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CPP_CobotGameInstance.h"

#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/prewindowsapi.h"

#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")

#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"

#include "passplz.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "CPP_Cobot_Car_Controller.generated.h"

/**
 *
 */
UCLASS()
class PASSPLZ_API ACPP_Cobot_Car_Controller : public APlayerController
{
	GENERATED_BODY()
	ACPP_Cobot_Car_Controller();
	~ACPP_Cobot_Car_Controller();


protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void SetupInputComponent() override;

private:
	UCPP_CobotGameInstance* instance;

	SOCKET* sock;

	// 패킷 재조립을 위한 부분 나중에 무조건 수정
	int		prev_remain;
	int		prev_packet_size;
	char	prev_packet_buff[10000];

	void RecvPacket();
	void ProcessPacket(char* packet);

private:
	class ACPP_Cobot_Car* player;

	UPROPERTY(VisibleAnywhere, Category = Input)
		class UInputMappingContext* DefaultContext;

	UPROPERTY(VisibleAnywhere, Category = Input)
		class UInputAction* Move1;

	UPROPERTY(VisibleAnywhere, Category = Input)
		class UInputAction* Move2;

public:

	//a나 d를 누름
	void CarInput(const FInputActionValue& Value);
	//서버 : 앞으로 가라는 함수 이 함수 호출하면 됨
	void CarForward();
	//서버 : 회전 하라는 함수 이 함수 호출하면 됨
	void CarRotation(float rotationValue);
};
