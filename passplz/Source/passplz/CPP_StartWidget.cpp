// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_StartWidget.h"
#include "CPP_Cobot_Controller.h"

void UCPP_StartWidget::SendLoginIDPW()
{

	Cast<ACPP_Cobot_Controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->SetWidget(Cast<UUserWidget>(this));
	//Send필요 id랑 passward
	CallEventSuccessLogin(true);
}

void UCPP_StartWidget::CallEventSuccessLogin(bool loginResult)
{
	isSuccessLogin = loginResult;

	if(isSuccessLogin)
		Cast<ACPP_Cobot_Controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->SetWidget(nullptr); //로그인이 성공하면 현재 위젯을 nullptr로 바꾼다.

	FOutputDeviceNull pAR;
	CallFunctionByNameWithArguments(TEXT("Success_Login"), pAR, nullptr, true);
}

void UCPP_StartWidget::SendSingupIDPW()
{
	Cast<ACPP_Cobot_Controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->SetWidget(Cast<UUserWidget>(this));
		//ignupID, signupPassward 이 두개 서버에 보내서 있는 아이디 인지 확인하는 작업 필요 없다면 계정 생성
	//자신의 포인터도 같이 넘겨주자
	// send해주고 CallEventSuccessSignup 여기에 성공 여부 bool값으로 보내주기
	CallEventSuccessSignup(true);
}

void UCPP_StartWidget::CallEventSuccessSignup(bool signupResult)
{
	isSuccessSignup = signupResult; //bool값 설정하고 커스텀이벤트 호출

	FOutputDeviceNull pAR;
	CallFunctionByNameWithArguments(TEXT("Success_Signup"), pAR, nullptr, true);
}

void UCPP_StartWidget::CreateRoom()
{
	CallEventSuccessAddRoom(roomName, roomMode, 0);
}

void UCPP_StartWidget::CallEventSuccessAddRoom(FString name, int mode, int id)
{
	roomName = name;
	roomMode = mode;
	roomID = id;

	FOutputDeviceNull pAR;
	CallFunctionByNameWithArguments(TEXT("add_Room"), pAR, nullptr, true);

}

