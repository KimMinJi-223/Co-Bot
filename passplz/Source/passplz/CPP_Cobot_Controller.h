// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../server/server/protocol.h"
#include "../../../server/server/RingBuffer.h"

#include "CPP_CobotGameInstance.h"

#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/prewindowsapi.h"

#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")

#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"

#include "CPP_Elevator.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "CPP_Cobot_Controller.generated.h"

#define FOOTSTEP_SPEED 2
#define STEP_HEIGHT 0.8f;
#define FOOTSTEP_LENGTH 60.f

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

	TArray<AActor*> maze_actor;
	TArray<AActor*> gear_actor;
	TArray<AActor*> forklift_actor;
	TArray<AActor*> board_actor;

	UUserWidget* current_widget; //���� ������ ������� widget;

	bool is_maze_button_push_forward;
	bool is_maze_button_push_back;
	bool is_maze_button_push_left;
	bool is_maze_button_push_right;

protected:
	virtual void BeginPlay() override;

private:
	virtual void Tick(float DeltaTime) override;
	void RecvPacket();
	void ProcessPacket(char* packet);
	void SendEnterPacket();
	void SendMovePacket(direction direction);

	UCPP_CobotGameInstance* instance;
	
	// ��Ŷ �������� ���� �κ� ���߿� ������ ����
	int		prev_remain;
	int		prev_packet_size;
	char	prev_packet_buff[10000];

//	std::chrono::high_resolution_clock::time_point last_move_time;

protected:
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void SetupInputComponent() override;

private:
	void Left_Right(float NewAxisValue);
	void Turn(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Union_Pressed();
	void Union_Released();
	void Run_Pressed();
	void Run_Released();

private:
	float CalculateSpeedAndCurveValue();
	void UpdateFootMovement(float& time, FVector& current, FVector& start, FVector& target, const FVector& forward_vector, int directionMultiplier);
	void EndFootMovement(FVector& start, FVector& target, FVector& current, float& time, direction dir);

public: //��ֹ��� ���� ������ ����� ���� �ʿ��� API
	SOCKET* GetSocket() { return sock; };
	void SetWidget(UUserWidget* widget) { current_widget = widget; }

	UFUNCTION(BlueprintCallable)
		void SendEsc();

private:
	class ACPP_Cobot* player;
	FVector player_pos;
	float new_axis_value;

	class ACPP_Cobot* Player_2; //��� �÷��̾� ����

protected:
	UPROPERTY(BlueprintReadWrite)
		FString ID; //���̵�
	UPROPERTY(BlueprintReadWrite)
		FString Passward; //��й�ȣ

private:
	float curve_value;
	bool isClear;


};