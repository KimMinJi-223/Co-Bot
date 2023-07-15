// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_StartWidget.h"


//UCPP_StartWidget::UCPP_StartWidget()
//
//{
//	
//}

void UCPP_StartWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UE_LOG(LogTemp, Warning, TEXT("startwidget()"));

	instance = Cast<UCPP_CobotGameInstance>(GetWorld()->GetGameInstance());
	sock = instance->GetSocketMgr()->GetSocket();
}

bool UCPP_StartWidget::SendLoginIDPW()
{
	//서버! ID, Passward->이거 서버로 보내서 로그인 여부확인
	//서버에서 받은거를 리턴값으로 넘겨준다.
	UE_LOG(LogTemp, Warning, TEXT("SendLoginIDPW()"));

	u_long BlockingMode = 0;
	ioctlsocket(*sock, FIONBIO, &BlockingMode); // sock을 논블로킹 모드로 설정

	wchar_t* id = TCHAR_TO_WCHAR(*ID);
	wchar_t* pw = TCHAR_TO_WCHAR(*Passward);

	// 서버한테 들어왔다고 알려주는 거
	cs_login_packet login_pack;
	login_pack.size = sizeof(login_pack);
	login_pack.type = static_cast<char>(packet_type::cs_login);
	wcscpy_s(login_pack.id, MAX_NAME, id);
	wcscpy_s(login_pack.passward, MAX_NAME, pw);

	int ret = send(*sock, reinterpret_cast<char*>(&login_pack), sizeof(login_pack), 0);

	char buff[BUF_SIZE];
	ret = recv(*sock, reinterpret_cast<char*>(&buff), BUF_SIZE, 0);

	u_long nonBlockingMode = 1;
	ioctlsocket(*sock, FIONBIO, &nonBlockingMode); // sock을 논블로킹 모드로 설정

	if (ret != buff[0]) {
		UE_LOG(LogTemp, Warning, TEXT("%d start widget signup recv err"), ret);
		return false;
	}

	switch (buff[1])
	{
	case static_cast<int>(packet_type::sc_login_fail): return false;
	case static_cast<int>(packet_type::sc_login_success): return true;
	}

	return false;
}

//bool UCPP_StartWidget::CallEventSuccessLogin(bool loginResult)
//{
//	isSuccessLogin = loginResult;
//
//	if(isSuccessLogin)
//		Cast<ACPP_Cobot_Controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->SetWidget(nullptr); //로그인이 성공하면 현재 위젯을 nullptr로 바꾼다.
//
//	FOutputDeviceNull pAR;
//	CallFunctionByNameWithArguments(TEXT("Success_Login"), pAR, nullptr, true);
//}

bool UCPP_StartWidget::SendSingupIDPW()
{
	//서버! signupID, signupPassward->이거 서버로 보내서 로그인 여부확인
	//서버에서 받은거를 리턴값으로 넘겨준다.
	UE_LOG(LogTemp, Warning, TEXT("SendSignupIDPW()"));

	u_long BlockingMode = 0;
	ioctlsocket(*sock, FIONBIO, &BlockingMode); // sock을 논블로킹 모드로 설정

	wchar_t* id = TCHAR_TO_WCHAR(*signupID);
	wchar_t* pw = TCHAR_TO_WCHAR(*signupPassward);

	// 서버한테 들어왔다고 알려주는 거
	cs_login_packet login_pack;
	login_pack.size = sizeof(login_pack);
	login_pack.type = static_cast<char>(packet_type::cs_signup);
	wcscpy_s(login_pack.id, MAX_NAME, id);
	wcscpy_s(login_pack.passward, MAX_NAME, pw);

	int ret = send(*sock, reinterpret_cast<char*>(&login_pack), sizeof(login_pack), 0);

	char buff[BUF_SIZE];
	ret = recv(*sock, reinterpret_cast<char*>(&buff), BUF_SIZE, 0);

	u_long nonBlockingMode = 1;
	ioctlsocket(*sock, FIONBIO, &nonBlockingMode); // sock을 논블로킹 모드로 설정

	if (ret != buff[0]) {
		UE_LOG(LogTemp, Warning, TEXT("start widget signup recv err"));
		return false;
	}

	switch (buff[1])
	{
	case static_cast<int>(packet_type::sc_signup_fail): return false;
	case static_cast<int>(packet_type::sc_signup_success): return true;
	}

	return false;
}

//void UCPP_StartWidget::CallEventSuccessSignup(bool signupResult)
//{
//	isSuccessSignup = signupResult; //bool값 설정하고 커스텀이벤트 호출
//
//	FOutputDeviceNull pAR;
//	CallFunctionByNameWithArguments(TEXT("Success_Signup"), pAR, nullptr, true);
//}

void UCPP_StartWidget::CreateRoom()
{
	UE_LOG(LogTemp, Warning, TEXT("CreateRoom()"));

	wchar_t* room_name = TCHAR_TO_WCHAR(*roomName);

	cs_create_room_packet pack;
	pack.size = sizeof(pack);
	pack.type = static_cast<char>(packet_type::cs_create_room);
	wcscpy_s(pack.room_name, MAX_NAME, room_name);
	pack.room_mode = roomMode;

	int ret = send(*sock, reinterpret_cast<char*>(&pack), sizeof(pack), 0);
	if (ret <= 0) UE_LOG(LogTemp, Warning, TEXT("CreateRoom() send err"));

	u_long BlockingMode = 0;
	ioctlsocket(*sock, FIONBIO, &BlockingMode); // sock을 논블로킹 모드로 설정

	char buff[BUF_SIZE];
	ret = recv(*sock, reinterpret_cast<char*>(&buff), BUF_SIZE, 0);

	u_long nonBlockingMode = 1;
	ioctlsocket(*sock, FIONBIO, &nonBlockingMode); // sock을 논블로킹 모드로 설정

	if (ret != buff[0])
		UE_LOG(LogTemp, Warning, TEXT("start widget signup recv err"));

	switch (buff[1])
	{
	case static_cast<int>(packet_type::sc_create_room_ok):
	{
		sc_create_room_ok_packet* recv_pack = reinterpret_cast<sc_create_room_ok_packet*>(&buff);
		CallEventSuccessAddRoom(roomName, recv_pack->room_mode, recv_pack->host_id);
	} break;
	}
}

void UCPP_StartWidget::CallEventSuccessAddRoom(FString name, int mode, int id)
{
	roomName = name;
	roomMode = mode;
	roomID = id;

	FOutputDeviceNull pAR;
	CallFunctionByNameWithArguments(TEXT("add_Room"), pAR, nullptr, true);
}

