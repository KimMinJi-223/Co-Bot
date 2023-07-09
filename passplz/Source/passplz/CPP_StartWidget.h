// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

public:
	/*UFUNCTION(BlueprintCallable, Category = "print")
	void IDprint() {
		UE_LOG(LogTemp, Warning, TEXT("%s, %s"), *ID, *Passward );
	};*/
	
protected:
	UFUNCTION(BlueprintCallable)
		void SendLoginIDPW();
	void CallEventSuccessLogin(bool loginResult);

	UFUNCTION(BlueprintCallable)
		void SendSingupIDPW();
	void CallEventSuccessSignup(bool signupResult);


protected:
	UPROPERTY(BlueprintReadWrite)
		bool Is_start; //서버에게 이제 시작해도 되는 받는 불 변수이다.

	UPROPERTY(BlueprintReadWrite)
		FString ID;
	UPROPERTY(BlueprintReadWrite)
		FString Passward;
	UPROPERTY(BlueprintReadWrite)
		bool isSuccessLogin;

	UPROPERTY(BlueprintReadWrite)
		FString signupID;
	UPROPERTY(BlueprintReadWrite)
		FString signupPassward;
	UPROPERTY(BlueprintReadWrite)
		bool isSuccessSignup;
};
