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

	TArray<AActor*> cannon_actor;

	// 패킷 재조립을 위한 부분 나중에 무조건 수정
	int		prev_remain;
	int		prev_packet_size;
	char	prev_packet_buff[10000];

	int		player_number;

	void RecvPacket();
	void ProcessPacket(char* packet);

private:
	virtual void Tick(float DeltaTime) override;

	class ACPP_Cobot_Car* player;

	UPROPERTY(VisibleAnywhere, Category = Input)
		class UInputMappingContext* DefaultContext;

	UPROPERTY(VisibleAnywhere, Category = Input)
		class UInputAction* Move;

	UPROPERTY(VisibleAnywhere, Category = Input)
		class UInputAction* MouseWheel;

	UPROPERTY(VisibleAnywhere, Category = Input)
		class UInputAction* MouseLeft;

	UPROPERTY(VisibleAnywhere, Category = Input)
		class UInputAction* Rotate;

	AActor* cannon;

	int mode;

public:

	//a나 d를 누름
	void CarInput(const FInputActionValue& Value);

	//1클라는 대포의 목표지점을 앞뒤로, 2클라는 목표지점을 양옆으로 조준한다.(서버에서 계산)
	//들어오는 값을 서버가 판단해서 최종 목표지점을 보내준다.
	void CannonInput(const FInputActionValue& Value);

	//클릭버튼 발사하겠다는 신호를 보내는 것
	//양쪽 클라가 모두 클릭버튼을 눌렀다면(bool값으로 저장한다) 발사하라는 패킷을 보내줘야함
	//단 (0,0,0)위치에서 1번클라가 클릭을 했는데, 1번클라나 2번클라가 마우스휠을 움직여서 조준 위치를 바꾼다면 bool값을 둘다 false로 바꾼다.
	void FireCannonInput(const FInputActionValue& Value);

	void RotateInput(const FInputActionValue& Value);

	//서버 : 앞으로 가라는 함수 이 함수 호출하면 됨
	void CarForward(float acceleration);
	//서버 : 회전 하라는 함수 이 함수 호출하면 됨
	void CarRotation(float rotationValue);
	
	void ChangeMode(int Mode);


};
