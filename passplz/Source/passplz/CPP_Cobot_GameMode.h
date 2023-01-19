// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CPP_Cobot_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class PASSPLZ_API ACPP_Cobot_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ACPP_Cobot_GameMode();

	// 플레이어가 로그인을 완료하면 실행되는 함수
	// 조종할 폰을 생성하고 컨트롤러가 폰에 빙의하는 작업이 이루어진다.
	// PostInitializeComponents함수에서 폰과 컨트롤러가 생성됨을 볼 수 있다. 
	// 빙의는 컨트롤러는 Possess, 폰은 PossessedBy로 볼 수 있다. 
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
