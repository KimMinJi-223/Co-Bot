// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../server/server/RingBuffer.h"

#include "CPP_CobotGameInstance.h"

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
		~ACPP_Cobot_Controller();

private:
	SOCKET* sock;
	vector_d3 location;
	int	id;
	RingBuffer ring_buff;
	// TMap<int, AActor*> actorList;

	vector_d3 tm_location;
	vector_d3 tm_current;
	float tm_time_left;
	float tm_time_right;
	double tm_yaw;

protected:
	virtual void BeginPlay() override;

private:
	virtual void Tick(float DeltaTime) override;
	void RecvPacket();
	void ProcessPacket(char* packet);
	void SendEnterPacket();
	void SendMovePacket(direction move_direction);

	UCPP_CobotGameInstance* instance;
	
	// 패킷 재조립을 위한 부분 나중에 무조건 수정
	int		prev_remain;
	int		prev_packet_size;
	char	prev_packet_buff[BUF_SIZE];

	std::chrono::high_resolution_clock::time_point last_move_time;

public:
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;
protected:
	virtual void SetupInputComponent() override;

private:
	void Move_Forward(float NewAxisValue);
	void Left_Right(float NewAxisValue);
	void Turn(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Union_Pressed();
	void Union_Released();

	class ACPP_Cobot* player;
	FVector player_pos;
	float new_axis_value;

	class ACPP_Cobot* Player_2; //상대 플레이어 정보


public:
	bool Is_Set_IDPW(FString I, FString p);

protected:
	UPROPERTY(BlueprintReadWrite)
		FString ID; //아이디
	UPROPERTY(BlueprintReadWrite)
		FString Passward; //비밀번호


protected:
	UCurveFloat* Cobot_Curve;
	bool IsUnion;

};