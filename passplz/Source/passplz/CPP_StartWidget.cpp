// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_StartWidget.h"
#include "CPP_Cobot_Controller.h"

bool UCPP_StartWidget::Is_Set_IDPW_Controller()
{

	return Cast<ACPP_Cobot_Controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->Is_Set_IDPW(ID, Passward);
	/*서버! 컨트롤러에서 서버에 send후 서버는 컨트롤러에게 bool값을 보내주고 컨트롤러가 이 함수에 bool값을 줘야함*/
}

void UCPP_StartWidget::Send_Singup_IDPW()
{
		//ignupID, signupPassward 이 두개 서버에 보내서 있는 아이디 인지 확인하는 작업 필요 없다면 계정 생성
	//자신의 포인터도 같이 넘겨주자
	CallEventSuccess_Signup(true);
}

void UCPP_StartWidget::CallEventSuccess_Signup(bool signupResult)
{
	isSuccessSignup = signupResult;

	FOutputDeviceNull pAR;
	CallFunctionByNameWithArguments(TEXT("Success_Signup"), pAR, nullptr, true);
}

