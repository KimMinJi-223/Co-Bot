// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SocketManager.h"

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CPP_CobotGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PASSPLZ_API UCPP_CobotGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UCPP_CobotGameInstance();

private:
	SocketManager	socket_mgr;

public:
	SocketManager* GetSocketMgr()
	{
		return &socket_mgr;
	}
	//bool			is_connect;
	//ClientInfo		cl_info;

	virtual void Init() override;
	//void GetInfo(int client_id, double x, double y, double z, int tm_id, double tm_x, double tm_y, double);

	UPROPERTY(BlueprintReadWrite)
		int  permitStage;
};
