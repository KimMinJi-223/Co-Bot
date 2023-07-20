// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CPP_Cobot_Controller.h"

#include "passplz.h"
#include "Blueprint/UserWidget.h"
#include "CPP_StartWidget.generated.h"

/**
 *
 */
UCLASS()
class PASSPLZ_API UCPP_StartWidget : public UUserWidget
{
	GENERATED_BODY()

		//UCPP_StartWidget();

protected:
	virtual void NativeOnInitialized();

private:
	SOCKET* sock;
	UCPP_CobotGameInstance* instance;

public:
	/*UFUNCTION(BlueprintCallable, Category = "print")
	void IDprint() {
		UE_LOG(LogTemp, Warning, TEXT("%s, %s"), *ID, *Passward );
	};*/
	
	UFUNCTION(BlueprintCallable)
		bool SendLoginIDPW();
	//void CallEventSuccessLogin(bool loginResult);

	UFUNCTION(BlueprintCallable)
		bool SendSingupIDPW();
	//void CallEventSuccessSignup(bool signupResult);

	//방만들기
	UFUNCTION(BlueprintCallable)
		void CreateRoom();
	void CallEventSuccessAddRoom(FString name, int mode, int id);

	//새로고침
	UFUNCTION(BlueprintCallable)
		void NormalModeRefresh();

	UFUNCTION(BlueprintCallable)
		bool IsPLayGame(int roomId);

protected:
	UPROPERTY(BlueprintReadWrite)
		bool Is_start; //서버에게 이제 시작해도 되는 받는 불 변수이다.

	//로그인 관련
	UPROPERTY(BlueprintReadWrite)
		FString ID;
	UPROPERTY(BlueprintReadWrite)
		FString Passward;
	UPROPERTY(BlueprintReadWrite)
		bool isSuccessLogin;

	//회원가입 관련
	UPROPERTY(BlueprintReadWrite)
		FString signupID;
	UPROPERTY(BlueprintReadWrite)
		FString signupPassward;
	UPROPERTY(BlueprintReadWrite)
		bool isSuccessSignup;

	//방 관련
	UPROPERTY(BlueprintReadWrite)
		FString roomName;
	UPROPERTY(BlueprintReadWrite)
		int stageNum;
	UPROPERTY(BlueprintReadWrite)
		FString userName;
	UPROPERTY(BlueprintReadWrite)
		int roomMode;
	UPROPERTY(BlueprintReadWrite)
		int roomID;

	//방의 수
	UPROPERTY(BlueprintReadWrite)
		int roomCount;
	UPROPERTY(BlueprintReadWrite)
		bool isRoomAdd;
};
