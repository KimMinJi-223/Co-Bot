// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Cobot_GameMode.h"
#include "CPP_Cobot.h"
#include "CPP_Cobot_Controller.h"

ACPP_Cobot_GameMode::ACPP_Cobot_GameMode()
{
	DefaultPawnClass = ACPP_Cobot::StaticClass();
	PlayerControllerClass = ACPP_Cobot_Controller::StaticClass();
}

void ACPP_Cobot_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}
